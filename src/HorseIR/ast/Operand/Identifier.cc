#include "../Operand.h"

using namespace horseIR::ast ;

Identifier::Identifier(HorseIRParser::GeneralNameContext *cst, ASTNode::MemManagerType &mem)
    : Operand(cst, mem, ASTNode::ASTNodeClass::Identifier, OperandClass::Identifier)
{
    assert(cst != nullptr) ;
    if (cst->name() != nullptr) {
        idName = ASTNode::CSTNameToString(cst->name()) ;
    } else if (cst->compoundName() != nullptr) {
        HorseIRParser::CompoundNameContext* context = cst->compoundName() ;
        idName = context->COMPOUND_ID()->getText() ;
    } else {
        assert(false) ;
    }
}

Identifier::Identifier(HorseIRParser::NameContext *cst, ASTNode::MemManagerType &mem)
    : Operand(cst, mem, ASTNode::ASTNodeClass::Identifier, OperandClass::Identifier)
{
    assert(cst != nullptr) ;
    idName = ASTNode::CSTNameToString(cst) ;
}

Identifier::Identifier(ASTNode::MemManagerType &mem)
    : Operand(mem, ASTNode::ASTNodeClass::Identifier, OperandClass::Identifier)
{}

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