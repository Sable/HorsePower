#include <vector>
#include <cassert>
#include <sstream>
#include <utility>
#include <string>
#include <map>
#include "../grammar/HorseIRParser.h"

#include "../Structure.h"

using namespace horseIR::ast ;

Method::Method(HorseIRParser::MethodContext *cst, ASTNode::MemManagerType &mem)
    : ASTNode(cst, mem, ASTNode::ASTNodeClass::Method)
{
    assert(cst != nullptr) ;
    methodName = cst->name()->getText() ;

    HorseIRParser::ParameterListContext* parameterListContext = cst->parameterList() ;
    const auto parameterNames = parameterListContext->name() ;
    const auto parameterTypes = parameterListContext->type() ;
    assert(parameterNames.size() == parameterTypes.size()) ;
    const std::size_t numParameter = parameterNames.size() ;
    for (std::size_t i = 0; i < numParameter; ++i) {
        HorseIRParser::NameContext* nameContext = parameterNames[i] ;
        HorseIRParser::TypeContext* typeContext = parameterTypes[i] ;
        std::string parameterName = ASTNode::CSTNameToString(nameContext) ;
        Type* parameterType = Type::makeTypeASTNode(typeContext, mem) ;
        parameters.push_back(std::make_pair(std::move(parameterName), parameterType)) ;
    }

    returnType = Type::makeTypeASTNode(cst->type(), mem) ;

    const auto cstStatement = cst->statement() ;
    for (auto iter = cstStatement.cbegin(); iter != cstStatement.cend(); ++iter) {
        Statement* stmt = Statement::makeStatementASTNode(*iter, mem) ;
        statements.push_back(stmt) ;
    }

    linkStatementFlow() ;
}

Method::Method(ASTNode::MemManagerType &mem)
    : ASTNode(mem, ASTNode::ASTNodeClass::Method),
      returnType{nullptr}
{}

std::size_t Method::getNumNodesRecursively() const
{
    std::size_t count = 1 ;
    for (auto iter = parameters.cbegin(); iter != parameters.cend(); ++iter) {
        count = count + iter->second->getNumNodesRecursively() ;
    }
    for (auto iter = statements.cbegin(); iter != statements.cend(); ++iter) {
        count = count + (*iter)->getNumNodesRecursively() ;
    }
    count = count + returnType->getNumNodesRecursively() ;
    return count ;
}

std::vector<ASTNode*> Method::getChildren() const
{
    std::vector<ASTNode*> retVector ;
    for (auto iter = parameters.cbegin(); iter != parameters.cend(); ++iter) {
        retVector.push_back(static_cast<ASTNode*>(iter->second)) ;
    }
    retVector.push_back(static_cast<ASTNode*>(returnType)) ;
    for (auto iter = statements.cbegin(); iter != statements.cend(); ++iter) {
        retVector.push_back(static_cast<ASTNode*>(*iter)) ;
    }
    return retVector ;
}

std::string Method::toString() const
{
    std::ostringstream stream ;
    stream << "def " << methodName << "(" ;
    for (auto iter = parameters.cbegin(); iter != parameters.cend(); ++iter) {
        if (iter + 1 != parameters.cend()) {
            stream << iter->first << " :" << iter->second->toString() << ", " ;
        } else {
            stream << iter->first << " :" << iter->second->toString() ;
        }
    }
    stream << ") :" << returnType->toString() << " {" << std::endl ;
    for (auto iter = statements.cbegin(); iter != statements.cend(); ++iter) {
        stream << ASTNode::INDENT << (*iter)->toString() << std::endl ;
        StatementIterator iterator = (*iter)->getIterator() ;
        stream << "        OnTrue:  " << (((*iterator.nextOnTrue()) == nullptr)? "nullptr" : (*iterator.nextOnTrue())->toString()) << std::endl ;
        stream << "        OnFalse: " << (((*iterator.nextOnFalse()) == nullptr)? "nullptr" : (*iterator.nextOnFalse())->toString()) << std::endl ;
    }
    stream << "}" ;
    return stream.str() ;
}

std::string Method::toTreeString() const
{
    std::ostringstream stream ;
    stream << "(Method name:" << methodName << ' ';
    for (auto iter = parameters.cbegin(); iter != parameters.cend(); ++iter) {
        stream << iter->second->toTreeString() << ' ' ;
    }
    stream << returnType->toTreeString() ;
    for (auto iter = statements.cbegin(); iter != statements.cend(); ++iter) {
        stream << ' ' << (*iter)->toTreeString() ;
    }
    stream << ')' ;
    return stream.str() ;
}

void Method::linkStatementFlow() {
    std::map<const std::string, Statement*> labelMap ;
    for (auto iter = statements.cbegin(); iter != statements.cend(); ++iter) {
        if ((*iter)->getStatementClass() == Statement::StatementClass::Label) {
            LabelStatement* labelStatement = static_cast<LabelStatement*>(*iter) ;
            const std::string labelName = labelStatement->getLabelName() ;
            assert(labelMap.find(labelName) == labelMap.end()) ;
            labelMap.insert(std::make_pair(std::move(labelName), std::move(labelStatement))) ;
        }
    }
    for (auto iter = statements.cbegin(); iter != statements.cend(); ++iter) {
        switch ((*iter)->getStatementClass()) {
            case Statement::StatementClass::Return : {
                ReturnStatement* const statement = static_cast<ReturnStatement*>(*iter) ;
                (void) statement->setOutwardFlow(nullptr, nullptr) ;
                break ;
            }
            case Statement::StatementClass::Label : {
                assert(iter + 1 != statements.cend()) ;
                LabelStatement* const statement = static_cast<LabelStatement*>(*iter) ;
                Statement* const nextStatement = *(iter + 1) ;
                (void) statement->setOutwardFlow(nextStatement, nextStatement) ;
                (void) nextStatement->appendInwardFlow(statement) ;
                break ;
            }
            case Statement::StatementClass::Assign : {
                assert(iter + 1 != statements.cend()) ;
                AssignStatement* const statement = static_cast<AssignStatement*>(*iter) ;
                Statement* const nextStatement = *(iter + 1) ;
                (void) statement->setOutwardFlow(nextStatement, nextStatement) ;
                (void) nextStatement->appendInwardFlow(statement) ;
                break ;
            }
            case Statement::StatementClass::Phi: {
                assert(iter + 1 != statements.cend()) ;
                PhiStatement* const statement = static_cast<PhiStatement*>(*iter) ;
                Statement* const nextStatement = *(iter + 1) ;
                (void) statement->setOutwardFlow(nextStatement, nextStatement) ;
                (void) nextStatement->appendInwardFlow(statement) ;
                break ;
            }
            case Statement::StatementClass::Branch : {
                assert(iter + 1 != statements.cend()) ;
                BranchStatement* const statement = static_cast<BranchStatement*>(*iter) ;
                const std::string targetLabelName = statement->getTargetLabelName() ;
                assert(labelMap.find(targetLabelName) != labelMap.end()) ;
                Statement* const trueStatement = labelMap[targetLabelName] ;
                Statement* const falseStatement = *(iter + 1) ;
                (void) statement->setOutwardFlow(trueStatement, falseStatement) ;
                (void) trueStatement->appendInwardFlow(statement) ;
                (void) falseStatement->appendInwardFlow(statement) ;
                break ;
            }
        }
    }
}