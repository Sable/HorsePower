#pragma once

#include "../../misc/InfixOStreamIterator.h"
#include "../AST.h"

namespace horseIR
{
namespace ast
{

class StringLiteral : public Literal {
 public:
  using StringType = std::vector<std::uint8_t>;
  using ElementType = Literal::VectorElemWrapper<StringType>;

  explicit StringLiteral (ASTNodeMemory &mem);
  StringLiteral (ASTNodeMemory &mem, const CSTType *cst);
  StringLiteral (StringLiteral &&stringLiteral) = default;
  StringLiteral (const StringLiteral &stringLiteral) = default;
  StringLiteral &operator= (StringLiteral &&stringLiteral) = delete;
  StringLiteral &operator= (const StringLiteral &stringLiteral) = delete;
  ~StringLiteral () override = default;

  std::vector<ElementType> getValue () const;
  template<class T>
  std::enable_if_t<std::is_assignable<std::vector<ElementType>, T>::value>
  setValue (T &&valueContainer);

  std::size_t getNumNodesRecursively () const override;
  std::vector<ASTNode *> getChildren () const override;
  StringLiteral *duplicateDeep (ASTNodeMemory &mem) const override;
  std::string toString () const override;

 protected:
  std::vector<ElementType> value = {};
  void __duplicateDeep (ASTNodeMemory &mem, const StringLiteral *str);
  static std::string elementToString (const ElementType &elementType);
};

StringLiteral::StringLiteral (ASTNodeMemory &mem)
    : Literal (mem, ASTNodeClass::StringLiteral, LiteralClass::String)
{}

StringLiteral::StringLiteral (ASTNodeMemory &mem, const CSTType *cst)
    : Literal (mem, ASTNodeClass::StringLiteral, cst, LiteralClass::String)
{}

inline std::vector<StringLiteral::ElementType> StringLiteral::getValue () const
{ return value; }

template<class T>
inline std::enable_if_t<
    std::is_assignable<std::vector<StringLiteral::ElementType>, T>::value>
StringLiteral::setValue (T &&valueContainer)
{ value = std::forward<T> (valueContainer); }

inline std::size_t StringLiteral::getNumNodesRecursively () const
{
  if (literalType == nullptr) return 1;
  return 1 + literalType->getNumNodesRecursively ();
}

inline std::vector<ASTNode *> StringLiteral::getChildren () const
{
  if (literalType == nullptr) return std::vector<ASTNode *> {};
  return std::vector<ASTNode *> {static_cast<ASTNode *>(literalType)};
}

inline StringLiteral *StringLiteral::duplicateDeep (ASTNodeMemory &mem) const
{
  auto stringLiteral = new StringLiteral (mem);
  stringLiteral->__duplicateDeep (mem, this);
  return stringLiteral;
}

inline std::string StringLiteral::toString () const
{
  std::ostringstream stream;
  stream << '(';
  std::vector<std::string> segments{};
  std::transform (
      value.cbegin (), value.cend (), std::back_inserter (segments),
      [] (const ElementType &elementType) -> std::string
      { return StringLiteral::elementToString (elementType); });
  std::copy (segments.cbegin (), segments.cend (),
             misc::InfixOStreamIterator<std::string> (stream, ", "));
  stream << ") :"
         << ((literalType == nullptr) ? "nullptr" : literalType->toString ());
  return stream.str ();
}

inline void
StringLiteral::__duplicateDeep (ASTNodeMemory &mem, const StringLiteral *str)
{
  assert (str != nullptr);
  Literal::__duplicateDeep (mem, str);
  value = str->value;
}

inline std::string
StringLiteral::elementToString (const ElementType &elementType)
{
  if (elementType.isNull ()) return "null";
  const ElementType::ValueType stringValue = elementType.getValue ();
  std::ostringstream stream;
  stream << std::hex << '"';
  for (const auto character : stringValue)
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

}
}