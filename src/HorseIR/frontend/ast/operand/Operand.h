#pragma once

#include <ostream>
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

  explicit Operand (const ASTNodeClass &astNodeClass,
                    const OperandClass &p_operandClass)
      : ASTNode (astNodeClass), operandClass (p_operandClass)
  {}

  Operand (const ASTNodeClass &astNodeClass,
           const CSTType *parseTree, const OperandClass &p_operandClass)
      : ASTNode (astNodeClass, parseTree), operandClass (p_operandClass)
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

inline std::ostream &
operator<< (std::ostream &stream, const Operand::OperandClass &operandClass)
{
  using OperandClass = Operand::OperandClass;
  switch (operandClass)
    {
      case OperandClass::Literal: return stream << "Literal";
      case OperandClass::Identifier: return stream << "Identifier";
    }
}

}
}