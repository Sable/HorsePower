#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

template<class T, typename = std::enable_if_t<!std::is_pointer<T>::value>>
class VectorLiteral : public Literal {
 public:
  class ElementType {
   public:
    typedef T ValueType;

    explicit ElementType (T &&elementValue)
        : ptr (new T (elementValue)), nullElement (false)
    {}

    explicit ElementType (const T &elementValue)
        : ptr (new T (elementValue)), nullElement (false)
    {}

    explicit ElementType (std::nullptr_t)
        : ptr (nullptr), nullElement (true)
    {}

    bool isNull () const
    { return nullElement; }

    T getValue () const
    { return *ptr; }

   protected:
    std::shared_ptr<T> ptr;
    bool nullElement;
  };

  using ElementIterator = typename std::vector<ElementType>::iterator;
  using ElementConstIterator = typename std::vector<
      ElementType
  >::const_iterator;

  VectorLiteral (const ASTNodeClass &astNodeClass,
                 const LiteralClass &literalClass)
      : Literal (astNodeClass, literalClass)
  {}

  VectorLiteral (const ASTNodeClass &astNodeClass, const CSTType *cst,
                 const LiteralClass &literalClass)
      : Literal (astNodeClass, cst, literalClass)
  {}

  VectorLiteral (VectorLiteral &&vectorLiteral) noexcept = default;
  VectorLiteral (const VectorLiteral &vectorLiteral) = default;
  VectorLiteral &operator= (VectorLiteral &&vectorLiteral) = delete;
  VectorLiteral &operator= (const VectorLiteral &vectorLiteral) = delete;
  ~VectorLiteral () override = default;

  std::vector<ElementType> getValue () const
  { return value; }

  ElementIterator valueBegin ()
  { return value.begin (); }

  ElementIterator valueEnd ()
  { return value.end (); }

  ElementConstIterator valueConstBegin () const
  { return value.cbegin (); }

  ElementConstIterator valueConstEnd () const
  { return value.cend (); }

  typename std::vector<ElementType>::size_type size () const
  { return value.size (); }

  template<class V>
  std::enable_if_t<std::is_assignable<std::vector<ElementType>, V>::value>
  setValue (V &&valueContainer)
  { value = std::forward<V> (valueContainer); }

  std::size_t getNumNodesRecursively () const override
  {
    if (literalType == nullptr) return 1;
    return 1 + literalType->getNumNodesRecursively ();
  }

  std::vector<ASTNode *> getChildren () const override
  {
    if (literalType == nullptr) return std::vector<ASTNode *> {};
    return std::vector<ASTNode *>{static_cast<ASTNode *>(literalType)};
  }

 protected:
  std::vector<ElementType> value = {};

  void __duplicateDeep (ASTNodeMemory &mem, const VectorLiteral *literal)
  {
    assert (literal != nullptr);
    Literal::__duplicateDeep (mem, literal);
    value = literal->value;
  }
};

}
}