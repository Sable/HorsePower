#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class CharLiteral : public VectorLiteral<std::uint8_t> {
 public:
  explicit CharLiteral (ASTNodeMemory &mem)
      : VectorLiteral<std::uint8_t>
            (mem, ASTNodeClass::CharLiteral, LiteralClass::Character)
  {}

  CharLiteral (ASTNodeMemory &mem, const CSTType *parseTree)
      : VectorLiteral<std::uint8_t>
            (mem, ASTNodeClass::CharLiteral, parseTree, LiteralClass::Character)
  {}

  CharLiteral (CharLiteral &&charLiteral) = default;
  CharLiteral (const CharLiteral &charLiteral) = default;
  CharLiteral &operator= (CharLiteral &&charLiteral) = delete;
  CharLiteral &operator= (const CharLiteral &charLiteral) = delete;
  ~CharLiteral () override = default;

  CharLiteral *duplicateDeep (ASTNodeMemory &mem) const override
  {
    auto charLiteral = new CharLiteral (mem);
    charLiteral->VectorLiteral<std::uint8_t>::__duplicateDeep (mem, this);
    return charLiteral;
  }

 protected:
  std::string elementToString (const std::uint8_t &elementType) const override
  {
    switch (elementType)
      {
        case 0x07: return R"('\a')";
        case 0x08: return R"('\b')";
        case 0x0C: return R"('\f')";
        case 0x0A: return R"('\n')";
        case 0x0D: return R"('\r')";
        case 0x09: return R"('\t')";
        case 0x0B: return R"('\v')";
        case 0x5C: return R"('\\')";
        case 0x27: return R"('\'')";
        case 0x22: return R"('\"')";
        case 0x3F: return R"('\?')";
        default:
          {
            std::locale cLocale ("C");
            std::ostringstream stream;
            if (std::isprint (static_cast<char>(elementType), cLocale))
              { stream << '\'' << static_cast<char>(elementType) << '\''; }
            else
              { stream << "'\\x" << std::hex << unsigned (elementType) << "'"; }
            return stream.str ();
          }
      }
  }
};

}
}