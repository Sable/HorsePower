#pragma once

#include <ostream>
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

  Literal (const ASTNodeClass &astNodeClass, const LiteralClass &p_literalClass)
      : Operand (astNodeClass, OperandClass::Literal),
        literalClass (p_literalClass)
  {}

  Literal (const ASTNodeClass &astNodeClass, const CSTType *parseTree,
           const LiteralClass &p_literalClass)
      : Operand (astNodeClass, parseTree, OperandClass::Literal),
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
  {
    if (type != nullptr) type->setParentASTNode (this);
    literalType = type;
  }

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

inline std::ostream &
operator<< (std::ostream &stream, const Literal::LiteralClass &literalClass)
{
  using LiteralClass = Literal::LiteralClass;
  switch (literalClass)
    {
      case LiteralClass::Bool: return stream << "Bool";
      case LiteralClass::Character: return stream << "Character";
      case LiteralClass::Integer8: return stream << "Integer8";
      case LiteralClass::Integer16: return stream << "Integer16";
      case LiteralClass::Integer32: return stream << "Integer32";
      case LiteralClass::Integer64: return stream << "Integer64";
      case LiteralClass::FP32: return stream << "FP32";
      case LiteralClass::FP64: return stream << "FP64";
      case LiteralClass::Complex: return stream << "Complex";
      case LiteralClass::Symbol: return stream << "Symbol";
      case LiteralClass::String: return stream << "String";
      case LiteralClass::Date: return stream << "Date";
      case LiteralClass::DateTime : return stream << "DateTime";
      case LiteralClass::Minute: return stream << "Minute";
      case LiteralClass::Month: return stream << "Month";
      case LiteralClass::Second: return stream << "Second";
      case LiteralClass::Time: return stream << "Time";
      case LiteralClass::Function: return stream << "Function";
      case LiteralClass::List:return stream << "List";
      case LiteralClass::Dictionary: return stream << "Dictionary";
      case LiteralClass::Enumeration: return stream << "Enumeration";
      case LiteralClass::Table: return stream << "Table";
      case LiteralClass::KeyTable: return stream << "KeyTable";
    }
}

}
}