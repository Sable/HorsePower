#include <vector>
#include <cassert>
#include <string>
#include <utility>
#include <sstream>

#include "../AST.h"

using namespace horseIR::ast ;

BranchStatement::BranchStatement(ASTNode* parent, HorseIRParser::StmtCoreContext *cst, ASTNode::MemManagerType &mem)
    : Statement(parent, cst, mem, ASTNode::ASTNodeClass::BranchStatement, StatementClass::Branch)
{
    assert(cst != nullptr) ;
    assert(cst->statementCore() != nullptr) ;
    auto gotoContext = dynamic_cast<HorseIRParser::StmtGotoContext*>(cst->statementCore()) ;
    assert(gotoContext != nullptr) ;

    HorseIRParser::LabelContext* labelContext = gotoContext->label() ;
    targetLabelName = ASTNode::CSTNameToString(labelContext->name()) ;
    if (gotoContext->name() != nullptr) {
        Identifier* identifier = new Identifier(this, gotoContext->name(), mem) ;
        checkCondition = std::make_pair(true, identifier) ;
    } else {
        checkCondition = std::make_pair(false, nullptr) ;
    }
}

BranchStatement::BranchStatement(ASTNode::MemManagerType &mem)
    : Statement(mem, ASTNode::ASTNodeClass::BranchStatement, StatementClass::Branch),
      targetLabelName{""},
      checkCondition{std::make_pair(false, nullptr)}
{}

std::string BranchStatement::getTargetLabelName() const
{
    return targetLabelName ;
}

std::size_t BranchStatement::getNumNodesRecursively() const
{
    return (checkCondition.first)? checkCondition.second->getNumNodesRecursively() + 1 : 1 ;
}

std::vector<ASTNode*> BranchStatement::getChildren() const
{
    return (checkCondition.first)? std::vector<ASTNode*> {checkCondition.second} : std::vector<ASTNode*> {} ;
}

std::string BranchStatement::toString() const
{
    std::ostringstream stream ;
    stream << "goto " << targetLabelName ;
    if (checkCondition.first) {
        stream << ' ' << checkCondition.second->toString() ;
    }
    stream << " ;" ;
    return stream.str() ;
}

std::string BranchStatement::toTreeString() const
{
    return "(BranchStatement)" ;
}