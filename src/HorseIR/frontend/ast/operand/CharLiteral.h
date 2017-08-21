#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class CharLiteral : public VectorLiteral<std::uint8_t> {
 public:
  CharLiteral ()
      : VectorLiteral<std::uint8_t>
            (ASTNodeClass::CharLiteral, LiteralClass::Character)
  {}

  explicit CharLiteral (const CSTType *parseTree)
      : VectorLiteral<std::uint8_t>
            (ASTNodeClass::CharLiteral, parseTree, LiteralClass::Character)
  {}

  CharLiteral (CharLiteral &&charLiteral) = default;
  CharLiteral (const CharLiteral &charLiteral) = default;
  CharLiteral &operator= (CharLiteral &&charLiteral) = delete;
  CharLiteral &operator= (const CharLiteral &charLiteral) = delete;
  ~CharLiteral () override = default;

  CharLiteral *duplicateDeep (ASTNodeMemory &mem) const override
  {
    auto charLiteral = mem.alloc<CharLiteral> ();
    charLiteral->VectorLiteral<std::uint8_t>::__duplicateDeep (mem, this);
    return charLiteral;
  }
};

}
}