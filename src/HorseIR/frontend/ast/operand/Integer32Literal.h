#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class Integer32Literal : public VectorLiteral<std::int32_t> {
 public:
  explicit Integer32Literal (ASTNodeMemory &mem)
      : VectorLiteral<std::int32_t>
            (mem, ASTNodeClass::Integer32Literal, LiteralClass::Integer16)
  {}

  Integer32Literal (ASTNodeMemory &mem, const CSTType *cst)
      : VectorLiteral<std::int32_t>
            (mem, ASTNodeClass::Integer32Literal, LiteralClass::Integer32)
  {}

  Integer32Literal (Integer32Literal &&literal) = default;
  Integer32Literal (const Integer32Literal &literal) = default;
  Integer32Literal &operator= (Integer32Literal &&literal) = delete;
  Integer32Literal &operator= (const Integer32Literal &literal) = delete;
  ~Integer32Literal () override = default;

  Integer32Literal *duplicateDeep (ASTNodeMemory &mem) const override
  {
    auto integer32Literal = new Integer32Literal (mem);
    integer32Literal->VectorLiteral<std::int32_t>::__duplicateDeep (mem, this);
    return integer32Literal;
  }

 protected:
  std::string elementToString (const std::int32_t &elementType) const override
  { return std::to_string (elementType); }
};

}
}