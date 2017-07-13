#include <vector>
#include <cassert>
#include <string>
#include <utility>
#include <sstream>

#include "../AST.h"

using namespace horseIR::ast ;

PhiStatement::PhiStatement(ASTNode* parent, HorseIRParser::StmtCoreContext *cst, ASTNode::MemManagerType &mem)
    : Statement(parent, cst, mem, ASTNode::ASTNodeClass::PhiStatement, StatementClass::Phi)
{
    assert(cst != nullptr) ;
    assert(cst->statementCore() != nullptr) ;
    auto stmtNameExprContext = dynamic_cast<HorseIRParser::StmtNameExprContext*>(cst->statementCore()) ;
    assert(stmtNameExprContext != nullptr) ;

    lhsID = new Identifier(this, stmtNameExprContext->generalName(), mem) ;
    lhsType = Type::makeTypeASTNode(this, stmtNameExprContext->type(), mem) ;

    HorseIRParser::ExpressionContext* expressionContext = stmtNameExprContext->expression() ;
    HorseIRParser::ExprPhiContext* exprPhiContext = dynamic_cast<decltype(exprPhiContext)>(expressionContext) ;
    assert(exprPhiContext != nullptr) ;
    const std::vector<HorseIRParser::LabelContext*> labels (exprPhiContext->label()) ;
    const std::vector<HorseIRParser::NameContext*> names (exprPhiContext->name()) ;
    assert(labels.size() == names.size()) ;
    const std::size_t numFlow = labels.size() ;
    for (std::size_t iter = 0; iter < numFlow; ++iter) {
        auto label = labels[iter] ;
        auto name = names[iter] ;
        std::string targetLabelName = ASTNode::CSTNameToString(label->name()) ;
        Identifier* conditionID = new Identifier(this, name, mem) ;
        inFlowMap.insert(std::make_pair(std::move(targetLabelName), conditionID)) ;
    }
}

PhiStatement::PhiStatement(ASTNode::MemManagerType &mem)
    : Statement(mem, ASTNode::ASTNodeClass::PhiStatement, StatementClass::Phi),
      lhsID{nullptr},
      lhsType{nullptr}
{}

std::size_t PhiStatement::getNumNodesRecursively() const
{
    std::size_t count = 1 ;
    for (auto iter = inFlowMap.cbegin(); iter != inFlowMap.cend(); ++iter) {
        count = count + iter->second->getNumNodesRecursively() ;
    }
    count += (lhsID == nullptr)? 0 : lhsID->getNumNodesRecursively() ;
    count += (lhsType == nullptr)? 0 : lhsType->getNumNodesRecursively() ;
    return count ;
}

std::vector<ASTNode*> PhiStatement::getChildren() const
{
    std::vector<ASTNode*> retVector ;
    if (lhsID != nullptr) retVector.push_back(static_cast<ASTNode*>(lhsID)) ;
    if (lhsType != nullptr) retVector.push_back(static_cast<ASTNode*>(lhsType)) ;
    for (auto iter = inFlowMap.cbegin(); iter != inFlowMap.cend(); ++iter) {
        retVector.push_back(static_cast<ASTNode*>(iter->second)) ;
    }
    return retVector ;
}

std::string PhiStatement::toString() const
{
    std::ostringstream stream ;
    stream << lhsID->toString() << " :" << lhsType->toString() << " = phi(" ;
    std::vector<std::pair<const std::string*, Identifier*>> buffer ;
    for (auto iter = inFlowMap.cbegin(); iter != inFlowMap.cend(); ++iter) {
        std::pair<const std::string*, Identifier*> pair = std::make_pair(&(iter->first), iter->second) ;
        buffer.push_back(std::move(pair)) ;
    }
    for (auto iter = buffer.cbegin(); iter != buffer.cend(); ++iter) {
        if (iter + 1 != buffer.cend()) {
            stream << '[' << *(iter->first) << "] " << iter->second->toString() << ", " ;
        } else {
            stream << '[' << *(iter->first) << "] " << iter->second->toString() ;
        }
    }
    stream << ')' ;
    return stream.str() ;
}

std::string PhiStatement::toTreeString() const
{
    return "(PhiStatement)" ;
}