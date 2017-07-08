#include "../Operand.h"

using namespace horseIR::ast ;

Operand::Operand(antlr4::tree::ParseTree *cst, ASTNode::MemManagerType &mem, ASTNode::ASTNodeClass type,
                 OperandClass p_operandClass)
    : ASTNode(cst, mem, type),
      operandClass{p_operandClass}
{}

Operand::Operand(ASTNode::MemManagerType &mem, ASTNode::ASTNodeClass type, OperandClass p_operandClass)
    : ASTNode(mem, type),
      operandClass{p_operandClass}
{}