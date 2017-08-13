#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class StringLiteral : public VectorLiteral<std::vector<std::uint8_t>> {
 public:
  explicit StringLiteral (ASTNodeMemory &mem)
      : VectorLiteral<std::vector<std::uint8_t>>
            (mem, ASTNodeClass::StringLiteral, LiteralClass::String)
  {}

  StringLiteral (ASTNodeMemory &mem, const CSTType *parseTree)
      : VectorLiteral<std::vector<std::uint8_t>>
            (mem, ASTNodeClass::StringLiteral, parseTree, LiteralClass::String)
  {}

  StringLiteral (StringLiteral &&stringLiteral) = default;
  StringLiteral (const StringLiteral &stringLiteral) = default;
  StringLiteral &operator= (StringLiteral &&stringLiteral) = delete;
  StringLiteral &operator= (const StringLiteral &stringLiteral) = delete;
  ~StringLiteral () override = default;

  StringLiteral *duplicateDeep (ASTNodeMemory &mem) const override
  {
    auto stringLiteral = new StringLiteral (mem);
    stringLiteral->
        VectorLiteral<std::vector<std::uint8_t>>::__duplicateDeep (mem, this);
    return stringLiteral;
  }

 protected:
  std::string
  elementToString (const std::vector<std::uint8_t> &elementType) const override
  {
    std::ostringstream stream;
    stream << std::hex << '"';
    for (const auto character : elementType)
      {
        if (character == 0x07)
          { stream << R"(\a)"; }
        else if (character == 0x08)
          { stream << R"(\b)"; }
        else if (character == 0x0C)
          { stream << R"(\f)"; }
        else if (character == 0x0A)
          { stream << R"(\n)"; }
        else if (character == 0x0D)
          { stream << R"(\r)"; }
        else if (character == 0x09)
          { stream << R"(\t)"; }
        else if (character == 0x0B)
          { stream << R"(\v)"; }
        else if (character == 0x5C)
          { stream << R"(\\)"; }
        else if (character == 0x27)
          { stream << R"(\')"; }
        else if (character == 0x22)
          { stream << R"(\")"; }
        else if (character == 0x3F)
          { stream << R"(\?)"; }
        else
          {
            std::locale cLocale ("C");
            if (std::isprint (static_cast<char>(character), cLocale))
              { stream << static_cast<char>(character); }
            else
              { stream << R"(\x)" << unsigned (character); }
          }
      }
    stream << '"';
    return stream.str ();
  }
};

}
}