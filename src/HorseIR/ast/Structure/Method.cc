#include <vector>
#include <cassert>
#include <sstream>
#include <utility>
#include <string>
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

    const auto statements = cst->statement() ;
    for (auto iter = statements.cbegin(); iter != statements.cend(); ++iter) {
        Statement* stmt = Statement::makeStatementASTNode(*iter, mem) ;
        this->statements.push_back(stmt) ;
    }
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