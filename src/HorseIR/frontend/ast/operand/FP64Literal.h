#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class FP64Literal : public VectorLiteral<double> {
 public:
  explicit FP64Literal (ASTNodeMemory &mem)
      : VectorLiteral<double>
            (mem, ASTNodeClass::FP64Literal, LiteralClass::FP64)
  {}

  FP64Literal (ASTNodeMemory &mem, const CSTType *cst)
      : VectorLiteral<double>
            (mem, ASTNodeClass::FP64Literal, LiteralClass::FP64)
  {}

  FP64Literal (FP64Literal &&literal) = default;
  FP64Literal (const FP64Literal &literal) = default;
  FP64Literal &operator= (FP64Literal &&literal) = delete;
  FP64Literal &operator= (const FP64Literal &literal) = delete;
  ~FP64Literal () override = default;

  FP64Literal *duplicateDeep (ASTNodeMemory &mem) const override
  {
    auto fp64Literal = new FP64Literal (mem);
    fp64Literal->VectorLiteral<double>::__duplicateDeep (mem, this);
    return fp64Literal;
  }

 protected:
  std::string elementToString (const double &elementType) const override
  {
    std::ostringstream stream;
    stream << std::scientific << elementType;
    return stream.str ();
  }
};

}
}