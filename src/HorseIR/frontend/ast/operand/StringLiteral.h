#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class StringLiteral : public VectorLiteral<std::vector<std::uint8_t>> {
 public:
  StringLiteral ()
      : VectorLiteral<std::vector<std::uint8_t>>
            (ASTNodeClass::StringLiteral, LiteralClass::String)
  {}

  StringLiteral (const CSTType *parseTree)
      : VectorLiteral<std::vector<std::uint8_t>>
            (ASTNodeClass::StringLiteral, parseTree, LiteralClass::String)
  {}

  StringLiteral (StringLiteral &&stringLiteral) = default;
  StringLiteral (const StringLiteral &stringLiteral) = default;
  StringLiteral &operator= (StringLiteral &&stringLiteral) = delete;
  StringLiteral &operator= (const StringLiteral &stringLiteral) = delete;
  ~StringLiteral () override = default;

  StringLiteral *duplicateDeep (ASTNodeMemory &mem) const override
  {
    auto stringLiteral = mem.alloc<StringLiteral> ();
    stringLiteral->
        VectorLiteral<std::vector<std::uint8_t>>::__duplicateDeep (mem, this);
    return stringLiteral;
  }
};

}
}