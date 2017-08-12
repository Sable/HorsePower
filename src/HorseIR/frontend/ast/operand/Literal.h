#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class Literal : public Operand {
 public:
  enum class LiteralClass {
    Bool, Character, Integer8, Integer16, Integer32, Integer64, FP32, FP64,
    Complex, Symbol, String, Date, DateTime, Minute, Month, Second, Time,
    Function, List, Dictionary, Enumeration, Table, KeyTable
  };

  template<class T, typename = std::enable_if_t<!std::is_pointer<T>::value>>
  class VectorElemWrapper {
   public:
    typedef T ValueType;

    explicit VectorElemWrapper (T &&elementValue)
        : ptr (new T (elementValue)), nullElement (false)
    {}

    explicit VectorElemWrapper (const T &elementValue)
        : ptr (new T (elementValue)), nullElement (false)
    {}

    explicit VectorElemWrapper (std::nullptr_t)
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

  Literal (ASTNodeMemory &mem, const ASTNodeClass &astNodeClass,
           const LiteralClass &p_literalClass)
      : Operand (mem, astNodeClass, OperandClass::Literal),
        literalClass (p_literalClass)
  {}

  Literal (ASTNodeMemory &mem, const ASTNodeClass &astNodeClass,
           const CSTType *parseTree, const LiteralClass &p_literalClass)
      : Operand (mem, astNodeClass, parseTree, OperandClass::Literal),
        literalClass (p_literalClass)
  {}

  Literal (Literal &&literal) = default;
  Literal (const Literal &literal) = default;
  Literal &operator= (Literal &&literal) = delete;
  Literal &operator= (const Literal &literal) = delete;
  ~Literal () override = default;

  LiteralClass getLiteralClass () const
  { return literalClass; }

  Type *getLiteralType () const
  { return literalType; }

  void setLiteralType (Type *type)
  { literalType = type; }

 protected:
  const LiteralClass literalClass;
  Type *literalType = nullptr;

  void __duplicateDeep (ASTNodeMemory &mem, const Literal *literal)
  {
    assert (literal != nullptr);
    Operand::__duplicateDeep (mem, literal);
    Type *type = nullptr;
    if (literal->literalType != nullptr)
      {
        type = dynamic_cast<Type *>(literal->literalType->duplicateDeep (mem));
        assert (type != nullptr);
        type->setParentASTNode (this);
      }
    literalType = type;
  }
};

}
}