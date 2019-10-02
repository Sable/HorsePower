#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class FP32Literal : public VectorLiteral<float> {
 public:
  FP32Literal ()
      : VectorLiteral<float>
            (ASTNodeClass::FP32Literal, LiteralClass::FP32)
  {}

  explicit FP32Literal (const CSTType *cst)
      : VectorLiteral<float>
            (ASTNodeClass::FP32Literal, cst, LiteralClass::FP32)
  {}

  FP32Literal (FP32Literal &&literal) = default;
  FP32Literal (const FP32Literal &literal) = default;
  FP32Literal &operator= (FP32Literal &&literal) = delete;
  FP32Literal &operator= (const FP32Literal &literal) = delete;
  ~FP32Literal () override = default;

  FP32Literal *duplicateDeep (ASTNodeMemory &mem) const override
  {
    auto fp32Literal = mem.alloc<FP32Literal> ();
    fp32Literal->VectorLiteral<float>::__duplicateDeep (mem, this);
    return fp32Literal;
  }
};

}
}