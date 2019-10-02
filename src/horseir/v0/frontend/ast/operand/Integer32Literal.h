#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class Integer32Literal : public VectorLiteral<std::int32_t> {
 public:
  Integer32Literal ()
      : VectorLiteral<std::int32_t>
            (ASTNodeClass::Integer32Literal, LiteralClass::Integer16)
  {}

  explicit Integer32Literal (const CSTType *cst)
      : VectorLiteral<std::int32_t>
            (ASTNodeClass::Integer32Literal, cst, LiteralClass::Integer32)
  {}

  Integer32Literal (Integer32Literal &&literal) = default;
  Integer32Literal (const Integer32Literal &literal) = default;
  Integer32Literal &operator= (Integer32Literal &&literal) = delete;
  Integer32Literal &operator= (const Integer32Literal &literal) = delete;
  ~Integer32Literal () override = default;

  Integer32Literal *duplicateDeep (ASTNodeMemory &mem) const override
  {
    auto integer32Literal = mem.alloc<Integer32Literal> ();
    integer32Literal->VectorLiteral<std::int32_t>::__duplicateDeep (mem, this);
    return integer32Literal;
  }
};

}
}