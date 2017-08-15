#pragma once

#include <complex>
#include "../AST.h"

namespace horseIR
{
namespace ast
{

class ComplexLiteral : public VectorLiteral<std::complex<double>> {
 public:
  ComplexLiteral ()
      : VectorLiteral<std::complex<double>>
            (ASTNodeClass::ComplexLiteral, LiteralClass::Complex)
  {}

  explicit ComplexLiteral (const CSTType *cst)
      : VectorLiteral<std::complex<double>>
            (ASTNodeClass::ComplexLiteral, cst, LiteralClass::Complex)
  {}

  ComplexLiteral (ComplexLiteral &&literal) = default;
  ComplexLiteral (const ComplexLiteral &literal) = default;
  ComplexLiteral &operator= (ComplexLiteral &&literal) = delete;
  ComplexLiteral &operator= (const ComplexLiteral &literal) = delete;
  ~ComplexLiteral () override = default;

  ComplexLiteral *duplicateDeep (ASTNodeMemory &mem) const override
  {
    auto complexLiteral = mem.alloc<ComplexLiteral> ();
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