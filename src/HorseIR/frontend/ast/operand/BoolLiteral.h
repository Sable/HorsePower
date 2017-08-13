#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class BoolLiteral : public VectorLiteral<bool> {
 public:

  explicit BoolLiteral (ASTNodeMemory &mem)
      : VectorLiteral<bool> (mem, ASTNodeClass::BoolLiteral, LiteralClass::Bool)
  {}

  BoolLiteral (ASTNodeMemory &mem, const CSTType *parseTree)
      : VectorLiteral<bool>
            (mem, ASTNodeClass::BoolLiteral, parseTree, LiteralClass::Bool)
  {}

  BoolLiteral (BoolLiteral &&boolLiteral) = default;
  BoolLiteral (const BoolLiteral &boolLiteral) = default;
  BoolLiteral &operator= (BoolLiteral &&boolLiteral) = delete;
  BoolLiteral &operator= (const BoolLiteral &boolLiteral) = delete;
  ~BoolLiteral () override = default;

  BoolLiteral *duplicateDeep (ASTNodeMemory &mem) const override
  {
    auto boolLiter = new BoolLiteral (mem);
    boolLiter->VectorLiteral<bool>::__duplicateDeep (mem, this);
    return boolLiter;
  }

 protected:
  std::string elementToString (const bool &elementType) const override
  { return (elementType) ? "1" : "0"; }
};

}
}