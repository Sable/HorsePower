#include <vector>
#include <cassert>
#include <string>
#include <utility>
#include <sstream>
#include "../grammar/HorseIRParser.h"

#include "../Structure.h"

using namespace horseIR::ast ;

PhiStatement::PhiStatement(HorseIRParser::StmtCoreContext *cst, ASTNode::MemManagerType &mem)
    : Statement(cst, mem, ASTNode::ASTNodeClass::PhiStatement, StatementClass::Phi)
{
    assert(cst != nullptr) ;
    assert(cst->statementCore() != nullptr) ;
    auto stmtNameExprContext = dynamic_cast<HorseIRParser::StmtNameExprContext*>(cst->statementCore()) ;
    assert(stmtNameExprContext != nullptr) ;

    lhsID = new Identifier(stmtNameExprContext->generalName(), mem) ;
    lhsType = Type::makeTypeASTNode(stmtNameExprContext->type(), mem) ;

    HorseIRParser::ExpressionContext* expressionContext = stmtNameExprContext->expression() ;
    HorseIRParser::ExprPhiContext* exprPhiContext = dynamic_cast<decltype(exprPhiContext)>(expressionContext) ;
    assert(exprPhiContext != nullptr) ;
    const std::vector<HorseIRParser::LabelContext*> labels (exprPhiContext->label()) ;
    const std::vector<HorseIRParser::NameContext*> names (exprPhiContext->name()) ;
    assert(labels.size() == names.size()) ;
    const std::size_t numFlow = labels.size() ;
    for (std::size_t iter = 0; iter < numFlow; ++iter) {
        auto label = labels[iter] ;
        auto name = name[iter] ;
        std::string targetLabelName = ASTNode::CSTNameToString(label->name()) ;
        Identifier* conditionID = new Identifier(name, mem) ;
        inFlowMap.insert(std::make_pair(std::move(targetLabelName), conditionID)) ;
    }
}

PhiStatement::PhiStatement(ASTNode::MemManagerType &mem)
    : Statement(mem, ASTNode::ASTNodeClass::PhiStatement, StatementClass::Phi),
      lhsID{nullptr},
      lhsType{nullptr}
{}