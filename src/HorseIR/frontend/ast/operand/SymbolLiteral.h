#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class SymbolLiteral : public VectorLiteral<std::string> {
 public:
  explicit SymbolLiteral (ASTNodeMemory &mem)
      : VectorLiteral<std::string>
            (mem, ASTNodeClass::SymbolLiteral, LiteralClass::Symbol)
  {}

  SymbolLiteral (ASTNodeMemory &mem, const CSTType *cst)
      : VectorLiteral<std::string>
            (mem, ASTNodeClass::SymbolLiteral, cst, LiteralClass::Symbol)
  {}

  SymbolLiteral (SymbolLiteral &&literal) = default;
  SymbolLiteral (const SymbolLiteral &literal) = default;
  SymbolLiteral &operator= (SymbolLiteral &&literal) = delete;
  SymbolLiteral &operator= (const SymbolLiteral &literal) = delete;
  ~SymbolLiteral () override = default;

  SymbolLiteral *duplicateDeep (ASTNodeMemory &mem) const override
  {
    auto symbolLiteral = new SymbolLiteral (mem);
    symbolLiteral->VectorLiteral<std::string>::__duplicateDeep (mem, this);
    return symbolLiteral;
  }

 protected:
  std::string
  elementToString (const std::string &elementType) const override
  { return "`" + elementType; }
};

}
}