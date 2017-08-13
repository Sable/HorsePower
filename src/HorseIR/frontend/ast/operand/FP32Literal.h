#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class FP32Literal : public VectorLiteral<float> {
 public:
  explicit FP32Literal (ASTNodeMemory &mem)
      : VectorLiteral<float>
            (mem, ASTNodeClass::FP32Literal, LiteralClass::FP32)
  {}

  FP32Literal (ASTNodeMemory &mem, const CSTType *cst)
      : VectorLiteral<float>
            (mem, ASTNodeClass::FP32Literal, cst, LiteralClass::FP32)
  {}

  FP32Literal (FP32Literal &&literal) = default;
  FP32Literal (const FP32Literal &literal) = default;
  FP32Literal &operator= (FP32Literal &&literal) = delete;
  FP32Literal &operator= (const FP32Literal &literal) = delete;
  ~FP32Literal () override = default;

  FP32Literal *duplicateDeep (ASTNodeMemory &mem) const override
  {
    auto fp32Literal = new FP32Literal (mem);
    fp32Literal->VectorLiteral<float>::__duplicateDeep (mem, this);
    return fp32Literal;
  }

 protected:
  std::string elementToString (const float &elementType) const override
  {
    std::ostringstream stream;
    stream << std::scientific << elementType;
    return stream.str ();
  }
};

}
}