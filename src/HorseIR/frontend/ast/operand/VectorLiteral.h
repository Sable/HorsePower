#pragma once

#include "../../misc/InfixOStreamIterator.h"
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

  explicit VectorLiteral (ASTNodeMemory &mem, const ASTNodeClass &astNodeClass,
                          const LiteralClass &literalClass)
      : Literal (mem, astNodeClass, literalClass)
  {}

  VectorLiteral (ASTNodeMemory &mem, const ASTNodeClass &astNodeClass,
                 const CSTType *cst, const LiteralClass &literalClass)
      : Literal (mem, astNodeClass, cst, literalClass)
  {}

  VectorLiteral (VectorLiteral &&vectorLiteral) noexcept = default;
  VectorLiteral (const VectorLiteral &vectorLiteral) = default;
  VectorLiteral &operator= (VectorLiteral &&vectorLiteral) = delete;
  VectorLiteral &operator= (const VectorLiteral &vectorLiteral) = delete;
  ~VectorLiteral () override = default;

  std::vector<ElementType> getValue () const
  { return value; }

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

  std::string toString () const override
  {
    std::ostringstream stream;
    stream << '(';
    std::transform (
        value.cbegin (), value.cend (),
        misc::InfixOStreamIterator<std::string> (stream, ", "),
        [=] (const ElementType &elementType) -> std::string
        {
          if (elementType.isNull ()) return "null";
          return elementToString (elementType.getValue ());
        });
    stream << ") :"
           << ((literalType == nullptr) ? "nullptr" : literalType->toString ());
    return stream.str ();
  }

 protected:
  std::vector<ElementType> value = {};
  virtual std::string
  elementToString (const T &elementType) const = 0;

  void __duplicateDeep (ASTNodeMemory &mem, const VectorLiteral *literal)
  {
    assert (literal != nullptr);
    Literal::__duplicateDeep (mem, literal);
    value = literal->value;
  }
};

}
}