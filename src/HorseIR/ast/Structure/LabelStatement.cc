#include <vector>
#include <cassert>
#include <string>
#include "../grammar/HorseIRParser.h"

#include "../Structure.h"

using namespace horseIR::ast ;

LabelStatement::LabelStatement(HorseIRParser::StmtLabelContext *cst, ASTNode::MemManagerType &mem)
    : Statement(cst, mem, ASTNode::ASTNodeClass::LabelStatement, StatementClass::Label)
{
    assert(cst != nullptr) ;
    HorseIRParser::LabelContext* labelContext = cst->label() ;
    labelName = ASTNode::CSTNameToString(labelContext->name()) ;
}

LabelStatement::LabelStatement(ASTNode::MemManagerType &mem)
    : Statement(mem, ASTNode::ASTNodeClass::LabelStatement, StatementClass::Label)
{}

std::string LabelStatement::getLabelName() const
{
    return labelName ;
}

std::size_t LabelStatement::getNumNodesRecursively() const
{
    return 1 ;
}

std::vector<ASTNode*> LabelStatement::getChildren() const
{
    return std::vector<ASTNode*>() ;
}

std::string LabelStatement::toString() const
{
    return "[ " + labelName + " ]" ;
}

std::string LabelStatement::toTreeString() const
{
    return "(LabelStatement)" ;
}