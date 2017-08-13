#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class Integer64Literal : public VectorLiteral<std::int64_t> {
 public:
  explicit Integer64Literal (ASTNodeMemory &mem)
      : VectorLiteral<std::int64_t>
            (mem, ASTNodeClass::Integer64Literal, LiteralClass::Integer64)
  {}

  Integer64Literal (ASTNodeMemory &mem, const CSTType *cst)
      : VectorLiteral<std::int64_t>
            (mem, ASTNodeClass::Integer64Literal, cst, LiteralClass::Integer64)
  {}

  Integer64Literal (Integer64Literal &&literal) = default;
  Integer64Literal (const Integer64Literal &literal) = default;
  Integer64Literal &operator= (Integer64Literal &&literal) = delete;
  Integer64Literal &operator= (const Integer64Literal &literal) = delete;
  ~Integer64Literal () override = default;

  Integer64Literal *duplicateDeep (ASTNodeMemory &mem) const override
  {
    auto integer64Literal = new Integer64Literal (mem);
    integer64Literal->VectorLiteral<std::int64_t>::__duplicateDeep (mem, this);
    return integer64Literal;
  }

 protected:
  std::string elementToString (const std::int64_t &elementType) const override
  { return std::to_string (elementType); }
};

}
}