#include "../AST.h"

using namespace horseIR::ast ;

Operand::Operand(ASTNode* parent, const antlr4::tree::ParseTree *cst, ASTNode::MemManagerType &mem, ASTNode::ASTNodeClass type,
                 OperandClass p_operandClass)
    : ASTNode(parent, cst, mem, type),
      operandClass{p_operandClass}
{}

Operand::Operand(ASTNode::MemManagerType &mem, ASTNode::ASTNodeClass type, OperandClass p_operandClass)
    : ASTNode(mem, type),
      operandClass{p_operandClass}
{}

Operand::OperandClass Operand::getOperandClass() const
{
    return operandClass ;
}
