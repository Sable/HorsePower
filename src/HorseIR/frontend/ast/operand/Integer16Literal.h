#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class Integer16Literal : public VectorLiteral<std::int16_t> {
 public:
  Integer16Literal ()
      : VectorLiteral<std::int16_t>
            (ASTNodeClass::Integer16Literal, LiteralClass::Integer16)
  {}

  explicit Integer16Literal (const CSTType *cst)
      : VectorLiteral<std::int16_t>
            (ASTNodeClass::Integer16Literal, cst, LiteralClass::Integer16)
  {}

  Integer16Literal (Integer16Literal &&literal) = default;
  Integer16Literal (const Integer16Literal &literal) = default;
  Integer16Literal &operator= (Integer16Literal &&literal) = delete;
  Integer16Literal &operator= (const Integer16Literal &literal) = delete;
  ~Integer16Literal () override = default;

  Integer16Literal *duplicateDeep (ASTNodeMemory &mem) const override
  {
    auto integer16Literal = mem.alloc<Integer16Literal> ();
    integer16Literal->VectorLiteral<std::int16_t>::__duplicateDeep (mem, this);
    return integer16Literal;
  }

 protected:
  std::string elementToString (const std::int16_t &elementType) const override
  { return std::to_string (elementType); }
};

}
}