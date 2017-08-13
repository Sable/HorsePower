#pragma once

#include <complex>
#include "../AST.h"

namespace horseIR
{
namespace ast
{

class ComplexLiteral : public VectorLiteral<std::complex<double>> {
 public:
  explicit ComplexLiteral (ASTNodeMemory &mem)
      : VectorLiteral<std::complex<double>>
            (mem, ASTNodeClass::ComplexLiteral, LiteralClass::Complex)
  {}

  ComplexLiteral (ASTNodeMemory &mem, const CSTType *cst)
      : VectorLiteral<std::complex<double>>
            (mem, ASTNodeClass::ComplexLiteral, LiteralClass::Complex)
  {}

  ComplexLiteral (ComplexLiteral &&literal) = default;
  ComplexLiteral (const ComplexLiteral &literal) = default;
  ComplexLiteral &operator= (ComplexLiteral &&literal) = delete;
  ComplexLiteral &operator= (const ComplexLiteral &literal) = delete;
  ~ComplexLiteral () override = default;

  ComplexLiteral *duplicateDeep (ASTNodeMemory &mem) const override
  {
    auto complexLiteral = new ComplexLiteral (mem);
    complexLiteral->
        VectorLiteral<std::complex<double>>::__duplicateDeep (mem, this);
    return complexLiteral;
  }

 protected:
  std::string
  elementToString (const std::complex<double> &elementType) const override
  {
    std::ostringstream stream;
    stream << elementType.real ()
           << ((elementType.imag () >= 0) ? "+" : "")
           << elementType.imag ()
           << "i";
    return stream.str ();
  }
};

}
}