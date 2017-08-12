#pragma once

#include "../../misc/InfixOStreamIterator.h"
#include "../AST.h"

namespace horseIR
{
namespace ast
{

class CharLiteral : public Literal {
 public:
  using ElementType = Literal::VectorElemWrapper<std::uint8_t>;

  explicit CharLiteral (ASTNodeMemory &mem);
  CharLiteral (ASTNodeMemory &mem, const CSTType *cst);
  CharLiteral (CharLiteral &&charLiteral) = default;
  CharLiteral (const CharLiteral &charLiteral) = default;
  CharLiteral &operator= (CharLiteral &&charLiteral) = delete;
  CharLiteral &operator= (const CharLiteral &charLiteral) = delete;
  ~CharLiteral () override = default;

  std::vector<ElementType> getValue () const;
  template<class T>
  std::enable_if_t<std::is_assignable<std::vector<ElementType>, T>::value>
  setValue (T &&valueContainer);

  std::size_t getNumNodesRecursively () const override;
  std::vector<ASTNode *> getChildren () const override;
  CharLiteral *duplicateDeep (ASTNodeMemory &mem) const override;
  std::string toString () const override;

 protected:
  std::vector<ElementType> value = {};
  void __duplicateDeep (ASTNodeMemory &mem, const CharLiteral *literal);
  static std::string elementToString (const ElementType &elementType);
};

inline CharLiteral::CharLiteral (ASTNodeMemory &mem)
    : Literal (mem, ASTNodeClass::CharLiteral, LiteralClass::Character)
{}

inline CharLiteral::CharLiteral (ASTNodeMemory &mem, const CSTType *cst)
    : Literal (mem, ASTNodeClass::CharLiteral, cst, LiteralClass::Character)
{}

inline std::vector<CharLiteral::ElementType> CharLiteral::getValue () const
{ return value; }

template<class T>
inline std::enable_if_t<
    std::is_assignable<std::vector<CharLiteral::ElementType>, T>::value>
CharLiteral::setValue (T &&valueContainer)
{ value = std::forward<T> (valueContainer); }

inline std::size_t CharLiteral::getNumNodesRecursively () const
{
  if (literalType == nullptr) return 1;
  return 1 + literalType->getNumNodesRecursively ();
}

inline std::vector<ASTNode *> CharLiteral::getChildren () const
{
  if (literalType == nullptr) return std::vector<ASTNode *> {};
  return std::vector<ASTNode *> {static_cast<ASTNode *>(literalType)};
}

inline CharLiteral *CharLiteral::duplicateDeep (ASTNodeMemory &mem) const
{
  auto charLiteral = new CharLiteral (mem);
  charLiteral->__duplicateDeep (mem, this);
  return charLiteral;
}

inline std::string CharLiteral::toString () const
{
  std::ostringstream stream;
  stream << '(';
  std::vector<std::string> segments{};
  std::transform (
      value.begin (), value.cend (), std::back_inserter (segments),
      [] (const ElementType &elementType) -> std::string
      { return CharLiteral::elementToString (elementType); });
  std::copy (segments.cbegin (), segments.cend (),
             misc::InfixOStreamIterator<std::string> (stream, ", "));
  stream << ") :"
         << ((literalType == nullptr) ? "nullptr" : literalType->toString ());
  return stream.str ();
}

inline void
CharLiteral::__duplicateDeep (ASTNodeMemory &mem, const CharLiteral *literal)
{
  assert (literal != nullptr);
  Literal::__duplicateDeep (mem, literal);
  value = literal->value;
}

inline std::string CharLiteral::elementToString (const ElementType &elementType)
{
  if (elementType.isNull ()) return "null";
  const ElementType::ValueType character = elementType.getValue ();
  switch (character)
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
          if (std::isprint (static_cast<char>(character), cLocale))
            { stream << '\'' << static_cast<char>(character) << '\''; }
          else
            { stream << "'\\x" << std::hex << unsigned (character) << "'"; }
          return stream.str ();
        }
    }
}

}
}