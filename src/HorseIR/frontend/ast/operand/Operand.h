#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class Operand : public ASTNode {
 public:
  enum class OperandClass {
    Identifier, Literal
  };

  Operand (ASTNodeMemory &mem, const ASTNodeClass &astNodeClass,
           const OperandClass &p_operandClass)
      : ASTNode (mem, astNodeClass), operandClass (p_operandClass)
  {}

  Operand (ASTNodeMemory &mem, const ASTNodeClass &astNodeClass,
           const CSTType *parseTree, const OperandClass &p_operandClass)
      : ASTNode (mem, astNodeClass, parseTree), operandClass (p_operandClass)
  {}

  Operand (Operand &&operand) = default;
  Operand (const Operand &operand) = default;
  Operand &operator= (Operand &&operand) = delete;
  Operand &operator= (const Operand &operand) = delete;
  ~Operand () override = default;

  OperandClass getOperandClass () const
  { return operandClass; }

 protected:
  const OperandClass operandClass;

  void __duplicateDeep (ASTNodeMemory &mem, const Operand *operand)
  {
    assert (operand != nullptr);
    ASTNode::__duplicateDeep (mem, operand);
  }
};

}
}