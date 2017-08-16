#pragma once

#include "../../misc/InfixOStreamIterator.h"
#include "../AST.h"

namespace horseIR
{
namespace ast
{

namespace storage
{

struct DictionaryEntry {
  Literal *key;
  Literal *value;
};

}

class DictionaryLiteral : public Literal {
 public:
  using ElementType = storage::DictionaryEntry;

  DictionaryLiteral ();
  explicit DictionaryLiteral (const CSTType *cst);
  DictionaryLiteral (DictionaryLiteral &&literal) = default;
  DictionaryLiteral (const DictionaryLiteral &literal);
  DictionaryLiteral &operator= (DictionaryLiteral &&literal) = delete;
  DictionaryLiteral &operator= (const DictionaryLiteral &literal) = delete;
  ~DictionaryLiteral () override = default;

  bool isNull () const;
  std::vector<ElementType> getValue () const;
  template<class T>
  std::enable_if_t<std::is_constructible<std::vector<ElementType>, T>::value>
  setValue (T &&valueContainer);
  void setValue (std::nullptr_t);

  std::size_t getNumNodesRecursively () const override;
  std::vector<ASTNode *> getChildren () const override;
  DictionaryLiteral *duplicateDeep (ASTNodeMemory &mem) const override;
  std::string toString () const override;

 protected:
  std::unique_ptr<std::vector<ElementType>> value = nullptr;
  void __duplicateDeep (ASTNodeMemory &mem, const DictionaryLiteral *literal);
  std::string elementToString (const ElementType &elementType) const;
};

inline DictionaryLiteral::DictionaryLiteral ()
    : Literal (ASTNodeClass::DictionaryLiteral, LiteralClass::Dictionary)
{}

inline DictionaryLiteral::DictionaryLiteral (const CSTType *cst)
    : Literal (ASTNodeClass::DictionaryLiteral, cst, LiteralClass::Dictionary)
{}

inline DictionaryLiteral::DictionaryLiteral (const DictionaryLiteral &literal)
    : Literal (literal)
{
  if (literal.value == nullptr)
    { value.reset (nullptr); }
  else
    { value.reset (new std::vector<ElementType> (*(literal.value))); }
}

inline bool DictionaryLiteral::isNull () const
{ return value == nullptr; }

inline std::vector<DictionaryLiteral::ElementType>
DictionaryLiteral::getValue () const
{ return *value; }

template<class T>
inline std::enable_if_t<
    std::is_constructible<std::vector<DictionaryLiteral::ElementType>, T>::value
>
DictionaryLiteral::setValue (T &&valueContainer)
{
  value.reset (new std::vector<ElementType> (std::forward<T> (valueContainer)));
  for (const ElementType &element : *value)
    {
      if (element.key != nullptr) element.key->setParentASTNode (this);
      if (element.value != nullptr) element.value->setParentASTNode (this);
    }
}

inline void DictionaryLiteral::setValue (std::nullptr_t)
{ value.reset (nullptr); }

inline std::size_t DictionaryLiteral::getNumNodesRecursively () const
{
  std::size_t counter = 1;
  if (literalType != nullptr)
    { counter = counter + literalType->getNumNodesRecursively (); }
  if (value != nullptr)
    {
      for (const ElementType &element: *value)
        {
          if (element.key != nullptr)
            { counter = counter + element.key->getNumNodesRecursively (); }
          if (element.value != nullptr)
            { counter = counter + element.value->getNumNodesRecursively (); }
        }
    }
  return counter;
}

inline std::vector<ASTNode *> DictionaryLiteral::getChildren () const
{
  std::vector<ASTNode *> childrenNodes{};
  if (value != nullptr)
    {
      for (const ElementType &element: *value)
        {
          if (element.key != nullptr)
            { childrenNodes.push_back (static_cast<ASTNode *>(element.key)); }
          if (element.value != nullptr)
            { childrenNodes.push_back (static_cast<ASTNode *>(element.value)); }
        }
    }
  if (literalType != nullptr)
    { childrenNodes.push_back (static_cast<ASTNode *>(literalType)); }
  return childrenNodes;
}

inline DictionaryLiteral *
DictionaryLiteral::duplicateDeep (ASTNodeMemory &mem) const
{
  auto dictionaryLiteral = mem.alloc<DictionaryLiteral> ();
  dictionaryLiteral->__duplicateDeep (mem, this);
  return dictionaryLiteral;
}

inline std::string DictionaryLiteral::toString () const
{
  std::ostringstream s;
  if (value == nullptr)
    {
      s << "null :"
        << ((literalType == nullptr) ? "nullptr" : literalType->toString ());
      return s.str ();
    }
  s << '{';
  std::transform (
      value->cbegin (), value->cend (),
      misc::InfixOStreamIterator<std::string> (s, ", "),
      [&] (const ElementType &element) -> std::string
      { return elementToString (element); });
  s << "} :"
    << ((literalType == nullptr) ? "nullptr" : literalType->toString ());
  return s.str ();
}

inline void
DictionaryLiteral::__duplicateDeep (ASTNodeMemory &mem,
                                    const DictionaryLiteral *literal)
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
            Literal *duplicateKey = nullptr;
            if (element.key != nullptr)
              {
                duplicateKey =
                    dynamic_cast<Literal *>(element.key->duplicateDeep (mem));
                duplicateKey->setParentASTNode (this);
              }
            Literal *duplicateValue = nullptr;
            if (element.value != nullptr)
              {
                duplicateValue =
                    dynamic_cast<Literal *>(element.value->duplicateDeep (mem));
                duplicateValue->setParentASTNode (this);
              }
            ElementType returnElement{};
            returnElement.key = duplicateKey;
            returnElement.value = duplicateValue;
            return returnElement;
          });
    }
}

inline std::string
DictionaryLiteral::elementToString (const ElementType &elementType) const
{
  std::ostringstream stream;
  if (elementType.key != nullptr)
    { stream << elementType.key->toString (); }
  else
    { stream << "nullptr"; }
  stream << " -> ";
  if (elementType.value != nullptr)
    { stream << elementType.value->toString (); }
  else
    { stream << "nullptr"; }
  return stream.str ();
}

}
}