#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class BoolLiteral : public VectorLiteral<bool> {
 public:
  BoolLiteral ()
      : VectorLiteral<bool>
            (ASTNodeClass::BoolLiteral, LiteralClass::Bool)
  {}

  explicit BoolLiteral (const CSTType *parseTree)
      : VectorLiteral<bool>
            (ASTNodeClass::BoolLiteral, parseTree, LiteralClass::Bool)
  {}

  BoolLiteral (BoolLiteral &&boolLiteral) = default;
  BoolLiteral (const BoolLiteral &boolLiteral) = default;
  BoolLiteral &operator= (BoolLiteral &&boolLiteral) = delete;
  BoolLiteral &operator= (const BoolLiteral &boolLiteral) = delete;
  ~BoolLiteral () override = default;

  BoolLiteral *duplicateDeep (ASTNodeMemory &mem) const override
  {
    auto boolLiter = mem.alloc<BoolLiteral> ();
    boolLiter->VectorLiteral<bool>::__duplicateDeep (mem, this);
    return boolLiter;
  }

 protected:
  std::string elementToString (const bool &elementType) const override
  { return (elementType) ? "1" : "0"; }
};

}
}