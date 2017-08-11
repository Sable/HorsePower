#pragma once

#include "../AST.h"
#include "../../misc/InfixOStreamIterator.h"

namespace horseIR
{
namespace ast
{

class BoolLiteral : public Literal {
 public:
  using ElementType = Literal::VectorElemWrapper<bool>;

  explicit BoolLiteral (ASTNodeMemory &mem);
  BoolLiteral (ASTNodeMemory &mem, const CSTType *parseTree);
  BoolLiteral (BoolLiteral &&boolLiteral) = default;
  BoolLiteral (const BoolLiteral &boolLiteral) = default;
  BoolLiteral &operator= (BoolLiteral &&boolLiteral) = delete;
  BoolLiteral &operator= (const BoolLiteral &boolLiteral) = delete;
  ~BoolLiteral () override = default;

  std::vector<ElementType> getValue () const;
  template<class T>
  std::enable_if_t<std::is_assignable<std::vector<ElementType>, T>::value>
  setValue (T &&valueContainer);

  std::size_t getNumNodesRecursively () const override;
  std::vector<ASTNode *> getChildren () const override;
  BoolLiteral *duplicateDeep (ASTNodeMemory &mem) const override;
  std::string toString () const override;

 protected:
  std::vector<ElementType> value = {};
  void __duplicateDeep (ASTNodeMemory &mem, const BoolLiteral *literal);
};

inline BoolLiteral::BoolLiteral (ASTNodeMemory &mem)
    : Literal (mem, ASTNodeClass::BoolLiteral, LiteralClass::Bool)
{}

inline BoolLiteral::BoolLiteral (ASTNodeMemory &mem, const CSTType *parseTree)
    : Literal (mem, ASTNodeClass::BoolLiteral, parseTree, LiteralClass::Bool)
{}

inline std::vector<BoolLiteral::ElementType> BoolLiteral::getValue () const
{ return value; }

template<class T>
inline std::enable_if_t<
    std::is_assignable<std::vector<BoolLiteral::ElementType>, T>::value>
BoolLiteral::setValue (T &&valueContainer)
{ value = std::forward<T> (valueContainer); }

inline std::size_t BoolLiteral::getNumNodesRecursively () const
{
  if (literalType == nullptr) return 1;
  return 1 + literalType->getNumNodesRecursively ();
}

inline std::vector<ASTNode *> BoolLiteral::getChildren () const
{
  if (literalType == nullptr) return std::vector<ASTNode *> {};
  return std::vector<ASTNode *> {static_cast<ASTNode *>(literalType)};
}

inline BoolLiteral *BoolLiteral::duplicateDeep (ASTNodeMemory &mem) const
{
  auto boolLiteral = new BoolLiteral (mem);
  boolLiteral->__duplicateDeep (mem, this);
  return boolLiteral;
}

inline std::string BoolLiteral::toString () const
{
  std::ostringstream stream;
  stream << '(';
  std::vector<std::string> segments{};
  std::transform (
      value.cbegin (), value.cend (), std::back_inserter (segments),
      [] (const ElementType &elementType) -> std::string
      {
        if (elementType.isNull ()) return "null";
        return elementType.getValue () ? "1" : "0";
      });
  std::copy (segments.cbegin (), segments.cend (),
             misc::InfixOStreamIterator<std::string> (stream, ", "));
  stream << ") :"
         << ((literalType == nullptr) ? "nullptr" : literalType->toString ());
  return stream.str ();
}

inline void
BoolLiteral::__duplicateDeep (ASTNodeMemory &mem, const BoolLiteral *literal)
{
  assert (literal != nullptr);
  Literal::__duplicateDeep (mem, literal);
  value = literal->value;
}

}
}