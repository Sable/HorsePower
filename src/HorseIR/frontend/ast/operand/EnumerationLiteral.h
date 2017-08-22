#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

namespace storage
{

struct Enumeration {
  Enumeration () = default;
  Enumeration (Enumeration &&enumeration) = default;
  Enumeration (const Enumeration &enumeration) = default;
  Enumeration &operator= (Enumeration &&enumeration) = default;
  Enumeration &operator= (const Enumeration &enumeration) = default;
  ~Enumeration () = default;
  std::string head{};
  Literal *content = nullptr;
};

}

class EnumerationLiteral : public Literal {
 public:
  using ElementType = storage::Enumeration;

  EnumerationLiteral ();
  explicit EnumerationLiteral (const CSTType *cst);
  EnumerationLiteral (EnumerationLiteral &&literal) = default;
  EnumerationLiteral (const EnumerationLiteral &l);
  EnumerationLiteral &operator= (EnumerationLiteral &&literal) = delete;
  EnumerationLiteral &operator= (const EnumerationLiteral &literal) = delete;
  ~EnumerationLiteral () override = default;

  bool isNull () const;
  ElementType getValue () const;
  template<class T>
  std::enable_if_t<std::is_constructible<ElementType, T>::value>
  setValue (T &&valueContainer);
  void setValue (std::nullptr_t);

  std::size_t getNumNodesRecursively () const override;
  std::vector<ASTNode *> getChildren () const override;
  EnumerationLiteral *duplicateDeep (ASTNodeMemory &mem) const override;

 protected:
  std::unique_ptr<ElementType> value = nullptr;
  void __duplicateDeep (ASTNodeMemory &mem, const EnumerationLiteral *literal);
};

inline EnumerationLiteral::EnumerationLiteral ()
    : Literal (ASTNodeClass::EnumerationLiteral, LiteralClass::Enumeration)
{}

inline EnumerationLiteral::EnumerationLiteral (const CSTType *cst)
    : Literal (ASTNodeClass::EnumerationLiteral, cst, LiteralClass::Enumeration)
{}

inline EnumerationLiteral::EnumerationLiteral (const EnumerationLiteral &l)
    : Literal (l)
{
  if (l.value == nullptr)
    { value.reset (nullptr); }
  else
    { value.reset (new ElementType (*(l.value))); }
}

inline bool EnumerationLiteral::isNull () const
{ return value == nullptr; }

inline EnumerationLiteral::ElementType EnumerationLiteral::getValue () const
{ return *value; }

template<class T>
inline std::enable_if_t<
    std::is_constructible<EnumerationLiteral::ElementType, T>::value
>
EnumerationLiteral::setValue (T &&valueContainer)
{
  value.reset (new ElementType (std::forward<T> (valueContainer)));
  if (value->content != nullptr)
    { value->content->setParentASTNode (this); }
}

inline void EnumerationLiteral::setValue (std::nullptr_t)
{ value.reset (nullptr); }

inline std::size_t EnumerationLiteral::getNumNodesRecursively () const
{
  std::size_t counter = 1;
  if (value != nullptr)
    if (value->content != nullptr)
      { counter = counter + value->content->getNumNodesRecursively (); }
  if (literalType != nullptr)
    { counter = counter + literalType->getNumNodesRecursively (); }
  return counter;
}

inline std::vector<ASTNode *> EnumerationLiteral::getChildren () const
{
  std::vector<ASTNode *> childrenNodes{};
  if (value != nullptr)
    if (value->content != nullptr)
      { childrenNodes.push_back (static_cast<ASTNode *>(value->content)); }
  if (literalType != nullptr)
    { childrenNodes.push_back (static_cast<ASTNode *>(literalType)); }
  return childrenNodes;
}

inline EnumerationLiteral *
EnumerationLiteral::duplicateDeep (ASTNodeMemory &mem) const
{
  auto enumerationLiteral = mem.alloc<EnumerationLiteral> ();
  enumerationLiteral->__duplicateDeep (mem, this);
  return enumerationLiteral;
}

inline void
EnumerationLiteral::__duplicateDeep (ASTNodeMemory &mem,
                                     const EnumerationLiteral *literal)
{
  assert (literal != nullptr);
  Literal::__duplicateDeep (mem, literal);
  if (literal->value == nullptr)
    { value.reset (nullptr); }
  else
    {
      value.reset (new ElementType ());
      value->head = literal->value->head;
      value->content = dynamic_cast<Literal *>(
          literal->value->content->duplicateDeep (mem)
      );
    }
}

}
}