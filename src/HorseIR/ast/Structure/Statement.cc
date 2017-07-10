#include <vector>
#include <cassert>
#include <string>
#include <utility>
#include <algorithm>
#include "../grammar/HorseIRParser.h"

#include "../Structure.h"

using namespace horseIR::ast ;

Statement::Statement(HorseIRParser::StatementContext *cst, ASTNode::MemManagerType &mem, ASTNode::ASTNodeClass type,
                     StatementClass p_StatementClass)
    : ASTNode(cst, mem, type),
      statementClass{p_StatementClass},
      outwardFlow{std::make_pair(nullptr, nullptr)}
{}

Statement::Statement(ASTNode::MemManagerType &mem, ASTNode::ASTNodeClass type, StatementClass p_StatementClass)
    : ASTNode(mem, type),
      statementClass{p_StatementClass},
      outwardFlow{std::make_pair(nullptr, nullptr)}
{}

Statement* Statement::makeStatementASTNode(HorseIRParser::StatementContext *cst, ASTNode::MemManagerType &mem)
{
    assert(cst != nullptr) ;
    HorseIRParser::StmtCoreContext* stmtCoreContext = nullptr ;
    HorseIRParser::StmtLabelContext* stmtLabelContext = nullptr ;
    if ((stmtCoreContext = dynamic_cast<decltype(stmtCoreContext)>(cst)) != nullptr) {
        HorseIRParser::StatementCoreContext* coreContext = stmtCoreContext->statementCore() ;

        HorseIRParser::StmtNameExprContext* nameExprContext = nullptr ;
        HorseIRParser::StmtReturnContext* returnContext = nullptr ;
        HorseIRParser::StmtGotoContext* gotoContext = nullptr ;
        if ((nameExprContext = dynamic_cast<decltype(nameExprContext)>(coreContext)) != nullptr) {
            AssignStatement* assignStatement = new AssignStatement(stmtCoreContext, mem) ;
            return assignStatement ;
        } else if ((returnContext = dynamic_cast<decltype(returnContext)>(coreContext)) != nullptr) {
            ReturnStatement* returnStatement = new ReturnStatement(stmtCoreContext, mem) ;
            return returnStatement ;
        } else if ((gotoContext = dynamic_cast<decltype(gotoContext)>(coreContext)) != nullptr) {
            BranchStatement* branchStatement = new BranchStatement(stmtCoreContext, mem) ;
            return branchStatement ;
        } else {
            assert(false) ;
            return nullptr ;
        }
    } else if ((stmtLabelContext = dynamic_cast<decltype(stmtLabelContext)>(cst)) != nullptr) {
        LabelStatement* labelStatement = new LabelStatement(stmtLabelContext, mem) ;
        return labelStatement ;
    } else {
        assert(false) ;
        return nullptr ;
    }
}

Statement::StatementClass Statement::getStatementClass() const
{
    return statementClass ;
}

std::pair<Statement*, Statement*> Statement::getOutwardFlow() const
{
    return outwardFlow ;
}

std::vector<Statement*> Statement::getInwardFlow() const
{
    return inwardFlow ;
}

Statement& Statement::setOutwardFlow(const std::pair<Statement *, Statement *> &flow)
{
    outwardFlow = std::move(flow) ;
    return *this ;
}

Statement& Statement::setOutwardFlow(Statement *trueFlow, Statement *falseFlow)
{
    outwardFlow = std::make_pair(trueFlow, falseFlow) ;
    return *this ;
}

Statement& Statement::appendInwardFlow(Statement *flow)
{
    assert(flow != nullptr) ;
    if (std::find(inwardFlow.begin(), inwardFlow.end(), flow) != inwardFlow.end()) return *this ;
    inwardFlow.push_back(flow) ;
    return *this ;
}

StatementIterator Statement::getIterator()
{
    return StatementIterator(this) ;
}