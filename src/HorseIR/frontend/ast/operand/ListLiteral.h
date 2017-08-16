#pragma once

#include "../../misc/InfixOStreamIterator.h"
#include "../AST.h"

namespace horseIR
{
namespace ast
{

class ListLiteral : public Literal {
 public:
  using ElementType = Literal *;

  ListLiteral ();
  explicit ListLiteral (const CSTType *cst);
  ListLiteral (ListLiteral &&literal) = default;
  ListLiteral (const ListLiteral &literal);
  ListLiteral &operator= (ListLiteral &&literal) = delete;
  ListLiteral &operator= (const ListLiteral &literal) = delete;
  ~ListLiteral () override = default;

  bool isNull () const;
  std::vector<ElementType> getValue () const;
  template<class T>
  std::enable_if_t<std::is_constructible<std::vector<ElementType>, T>::value>
  setValue (T &&valueContainer);
  void setValue (std::nullptr_t);

  std::size_t getNumNodesRecursively () const override;
  std::vector<ASTNode *> getChildren () const override;
  ListLiteral *duplicateDeep (ASTNodeMemory &mem) const override;
  std::string toString () const override;

 protected:
  std::unique_ptr<std::vector<ElementType>> value = nullptr;
  void __duplicateDeep (ASTNodeMemory &mem, const ListLiteral *literal);
  std::string elementToString (const ElementType &elementType) const;
};

inline ListLiteral::ListLiteral ()
    : Literal (ASTNodeClass::ListLiteral, LiteralClass::List)
{}

inline ListLiteral::ListLiteral (const CSTType *cst)
    : Literal (ASTNodeClass::ListLiteral, cst, LiteralClass::List)
{}

inline ListLiteral::ListLiteral (const ListLiteral &literal)
    : Literal (literal)
{
  if (literal.value == nullptr)
    { value.reset (nullptr); }
  else
    { value.reset (new std::vector<ElementType> (*(literal.value))); }
}

inline bool ListLiteral::isNull () const
{ return value == nullptr; }

inline std::vector<ListLiteral::ElementType> ListLiteral::getValue () const
{ return *value; }

template<class T>
inline std::enable_if_t<
    std::is_constructible<std::vector<ListLiteral::ElementType>, T>::value
>
ListLiteral::setValue (T &&valueContainer)
{
  value.reset (new std::vector<ElementType> (std::forward<T> (valueContainer)));
  for (const ElementType &element : *value)
    { if (element != nullptr) element->setParentASTNode (this); }
}

inline void ListLiteral::setValue (std::nullptr_t)
{ value.reset (nullptr); }

inline std::size_t ListLiteral::getNumNodesRecursively () const
{
  std::size_t counter = 1;
  if (literalType != nullptr)
    { counter = counter + literalType->getNumNodesRecursively (); }
  if (value != nullptr)
    {
      for (const ElementType &element : *value)
        if (element != nullptr)
          { counter = counter + element->getNumNodesRecursively (); }
    }
  return counter;
}

inline std::vector<ASTNode *> ListLiteral::getChildren () const
{
  std::vector<ASTNode *> childrenNodes{};
  if (value != nullptr)
    {
      for (const ElementType &element : *value)
        if (element != nullptr)
          { childrenNodes.push_back (static_cast<ASTNode *>(element)); }
    }
  if (literalType != nullptr)
    { childrenNodes.push_back (static_cast<ASTNode *>(literalType)); }
  return childrenNodes;
}

inline ListLiteral *ListLiteral::duplicateDeep (ASTNodeMemory &mem) const
{
  auto listLiteral = mem.alloc<ListLiteral> ();
  listLiteral->__duplicateDeep (mem, this);
  return listLiteral;
}

inline std::string ListLiteral::toString () const
{
  std::ostringstream s;
  if (value == nullptr)
    {
      s << "null :"
        << ((literalType == nullptr) ? "nullptr" : literalType->toString ());
      return s.str ();
    }
  s << '[';
  std::transform (
      value->cbegin (), value->cend (),
      misc::InfixOStreamIterator<std::string> (s, ", "),
      [&] (const ElementType &element) -> std::string
      { return elementToString (element); });
  s << "] :"
    << ((literalType == nullptr) ? "nullptr" : literalType->toString ());
  return s.str ();
}

inline void
ListLiteral::__duplicateDeep (ASTNodeMemory &mem, const ListLiteral *literal)
{
  assert (literal != nullptr);
  Literal::__duplicateDeep (mem, literal);
  if (literal->value == nullptr)
    { value.reset (nullptr); }
  else
    {
      value.reset (new std::vector<ElementType> ());
      value->reserve (literal->value->size ());
      std::transform (
          literal->value->cbegin (), literal->value->cend (),
          std::back_inserter (*value),
          [&] (const ElementType &element) -> ElementType
          {
            if (element == nullptr) return nullptr;
            auto duplicateElement = static_cast<ElementType>(
                element->duplicateDeep (mem)
            );
            duplicateElement->setParentASTNode (this);
            return duplicateElement;
          });
    }
}

inline std::string
ListLiteral::elementToString (const ElementType &elementType) const
{
  if (elementType == nullptr) return "nullptr";
  return elementType->toString ();
}

}
}