#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class SymbolLiteral : public VectorLiteral<std::string> {
 public:
  SymbolLiteral ()
      : VectorLiteral<std::string>
            (ASTNodeClass::SymbolLiteral, LiteralClass::Symbol)
  {}

  explicit SymbolLiteral (const CSTType *cst)
      : VectorLiteral<std::string>
            (ASTNodeClass::SymbolLiteral, cst, LiteralClass::Symbol)
  {}

  SymbolLiteral (SymbolLiteral &&literal) = default;
  SymbolLiteral (const SymbolLiteral &literal) = default;
  SymbolLiteral &operator= (SymbolLiteral &&literal) = delete;
  SymbolLiteral &operator= (const SymbolLiteral &literal) = delete;
  ~SymbolLiteral () override = default;

  SymbolLiteral *duplicateDeep (ASTNodeMemory &mem) const override
  {
    auto symbolLiteral = mem.alloc<SymbolLiteral> ();
    symbolLiteral->VectorLiteral<std::string>::__duplicateDeep (mem, this);
    return symbolLiteral;
  }
};

}
}