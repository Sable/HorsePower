#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

namespace storage
{

struct KeyTableColumn {
  std::string head{};
  Literal *content = nullptr;
  bool isKey = false;
};

}

class KeyTableLiteral : public Literal {
 public:
  using ElementType = storage::KeyTableColumn;
  using ValueIterator = std::vector<ElementType>::iterator;
  using ValueConstIterator = std::vector<ElementType>::const_iterator;

  KeyTableLiteral ();
  explicit KeyTableLiteral (const CSTType *cst);
  KeyTableLiteral (KeyTableLiteral &&literal) = default;
  KeyTableLiteral (const KeyTableLiteral &literal);
  KeyTableLiteral &operator= (KeyTableLiteral &&literal) = delete;
  KeyTableLiteral &operator= (const KeyTableLiteral &literal) = delete;
  ~KeyTableLiteral () override = default;

  bool isNull () const;
  std::vector<ElementType> getValue () const;
  ValueIterator valueBegin ();
  ValueIterator valueEnd ();
  ValueConstIterator valueConstBegin () const;
  ValueConstIterator valueConstEnd () const;
  template<class T>
  std::enable_if_t<std::is_constructible<std::vector<ElementType>, T>::value>
  setValue (T &&valueContainer);
  void setValue (std::nullptr_t);

  std::size_t getNumNodesRecursively () const override;
  std::vector<ASTNode *> getChildren () const override;
  KeyTableLiteral *duplicateDeep (ASTNodeMemory &mem) const override;

 protected:
  std::unique_ptr<std::vector<ElementType>> value = nullptr;
  void __duplicateDeep (ASTNodeMemory &mem, const KeyTableLiteral *literal);
  std::string elementToString (const ElementType &elementType) const;
};

inline KeyTableLiteral::KeyTableLiteral ()
    : Literal (ASTNodeClass::KeyTableLiteral, LiteralClass::KeyTable)
{}

inline KeyTableLiteral::KeyTableLiteral (const CSTType *cst)
    : Literal (ASTNodeClass::KeyTableLiteral, cst, LiteralClass::KeyTable)
{}

inline KeyTableLiteral::KeyTableLiteral (const KeyTableLiteral &literal)
    : Literal (literal)
{
  if (literal.value == nullptr)
    { value.reset (nullptr); }
  else
    { value.reset (new std::vector<ElementType> (*(literal.value))); }
}

inline bool KeyTableLiteral::isNull () const
{ return value == nullptr; }

inline std::vector<KeyTableLiteral::ElementType>
KeyTableLiteral::getValue () const
{ return *value; }

inline KeyTableLiteral::ValueIterator KeyTableLiteral::valueBegin ()
{ return value->begin (); }

inline KeyTableLiteral::ValueIterator KeyTableLiteral::valueEnd ()
{ return value->end (); }

inline KeyTableLiteral::ValueConstIterator
KeyTableLiteral::valueConstBegin () const
{ return value->cbegin (); }

inline KeyTableLiteral::ValueConstIterator
KeyTableLiteral::valueConstEnd () const
{ return value->cend (); }

template<class T>
inline std::enable_if_t<
    std::is_constructible<std::vector<KeyTableLiteral::ElementType>, T>::value
>
KeyTableLiteral::setValue (T &&valueContainer)
{
  value.reset (new std::vector<ElementType> (std::forward<T> (valueContainer)));
  for (const ElementType &element: *value)
    if (element.content != nullptr)
      { element.content->setParentASTNode (this); }
}

inline void KeyTableLiteral::setValue (std::nullptr_t)
{ value.reset (nullptr); }

inline std::size_t KeyTableLiteral::getNumNodesRecursively () const
{
  std::size_t counter = 1;
  if (literalType != nullptr)
    { counter = counter + literalType->getNumNodesRecursively (); }
  if (value != nullptr)
    {
      for (const ElementType &element: *value)
        if (element.content != nullptr)
          { counter = counter + element.content->getNumNodesRecursively (); }
    }
  return counter;
}

inline std::vector<ASTNode *> KeyTableLiteral::getChildren () const
{
  std::vector<ASTNode *> childrenNodes{};
  if (value != nullptr)
    {
      for (const ElementType &element: *value)
        if (element.content != nullptr)
          { childrenNodes.push_back (static_cast<ASTNode *>(element.content)); }
    }
  if (literalType != nullptr)
    { childrenNodes.push_back (static_cast<ASTNode *>(literalType)); }
  return childrenNodes;
}

inline KeyTableLiteral *
KeyTableLiteral::duplicateDeep (ASTNodeMemory &mem) const
{
  auto keyTableLiteral = mem.alloc<KeyTableLiteral> ();
  keyTableLiteral->__duplicateDeep (mem, this);
  return keyTableLiteral;
}

inline void
KeyTableLiteral::__duplicateDeep (ASTNodeMemory &mem,
                                  const KeyTableLiteral *literal)
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
            ElementType returnElement{};
            returnElement.head = element.head;
            returnElement.isKey = element.isKey;
            if (element.content != nullptr)
              {
                returnElement.content = dynamic_cast<Literal *>(
                    element.content->duplicateDeep (mem)
                );
                returnElement.content->setParentASTNode (this);
              }
            else
              { returnElement.content = nullptr; }
            return returnElement;
          });
    }
}

}
}