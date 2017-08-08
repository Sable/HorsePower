#include <vector>
#include <string>
#include <sstream>

#include "../AST.h"

using namespace horseIR::ast ;

Identifier::Identifier(ASTNode* parent, HorseIRParser::NameContext *cst, ASTNode::MemManagerType &mem)
    : Operand(parent, cst, mem, ASTNode::ASTNodeClass::Identifier, OperandClass::Identifier)
{
    assert(cst != nullptr) ;
    idName = ASTNode::CSTNameToString(cst) ;
}

Identifier::Identifier(ASTNode::MemManagerType &mem)
    : Operand(mem, ASTNode::ASTNodeClass::Identifier, OperandClass::Identifier)
{}

std::string Identifier::getName() const
{
    return idName ;
}

Identifier& Identifier::setName(const std::string &p_idName)
{
    idName = p_idName ;
    return *this ;
}

std::size_t Identifier::getNumNodesRecursively() const
{
    return 1 ;
}

std::vector<ASTNode*> Identifier::getChildren() const
{
    return std::vector<ASTNode*>{} ;
}

std::string Identifier::toString() const
{
    return idName ;
}

std::string Identifier::toTreeString() const
{
    return "(Identifier)" ;
}
