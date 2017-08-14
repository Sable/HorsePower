#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class Integer8Literal : public VectorLiteral<std::int8_t> {
 public:
  explicit Integer8Literal (ASTNodeMemory &mem)
      : VectorLiteral<std::int8_t>
            (mem, ASTNodeClass::Integer8Literal, LiteralClass::Integer8)
  {}

  Integer8Literal (ASTNodeMemory &mem, const CSTType *cst)
      : VectorLiteral<std::int8_t>
            (mem, ASTNodeClass::Integer8Literal, cst, LiteralClass::Integer8)
  {}

  Integer8Literal (Integer8Literal &&literal) = default;
  Integer8Literal (const Integer8Literal &literal) = default;
  Integer8Literal &operator= (Integer8Literal &&literal) = delete;
  Integer8Literal &operator= (const Integer8Literal &literal) = delete;
  ~Integer8Literal () override = default;

  Integer8Literal *duplicateDeep (ASTNodeMemory &mem) const override
  {
    auto integer8Literal = new Integer8Literal (mem);
    integer8Literal->
        VectorLiteral<std::int8_t>::__duplicateDeep (mem, this);
    return integer8Literal;
  }

 protected:
  std::string elementToString (const std::int8_t &elementType) const override
  { return std::to_string (elementType); }
};

}
}