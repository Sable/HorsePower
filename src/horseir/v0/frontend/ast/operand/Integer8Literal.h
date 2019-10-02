#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class Integer8Literal : public VectorLiteral<std::int8_t> {
 public:
  Integer8Literal ()
      : VectorLiteral<std::int8_t>
            (ASTNodeClass::Integer8Literal, LiteralClass::Integer8)
  {}

  explicit Integer8Literal (const CSTType *cst)
      : VectorLiteral<std::int8_t>
            ( ASTNodeClass::Integer8Literal, cst, LiteralClass::Integer8)
  {}

  Integer8Literal (Integer8Literal &&literal) = default;
  Integer8Literal (const Integer8Literal &literal) = default;
  Integer8Literal &operator= (Integer8Literal &&literal) = delete;
  Integer8Literal &operator= (const Integer8Literal &literal) = delete;
  ~Integer8Literal () override = default;

  Integer8Literal *duplicateDeep (ASTNodeMemory &mem) const override
  {
    auto integer8Literal = mem.alloc<Integer8Literal> ();
    integer8Literal->
        VectorLiteral<std::int8_t>::__duplicateDeep (mem, this);
    return integer8Literal;
  }
};

}
}