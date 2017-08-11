#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class PrimitiveType : public Type {
 public:
  enum class PrimitiveClass {
    Bool, Character, Integer8, Integer16, Integer32, Integer64, FP32, FP64,
    Complex, Symbol, Month, Date, DateTime, Minute, Second, Time, String, Table,
    KeyTable
  };

  explicit PrimitiveType (ASTNodeMemory &mem);
  PrimitiveType (ASTNodeMemory &mem, const CSTType *cst);
  PrimitiveType (PrimitiveType &&externPrimitive) = default;
  PrimitiveType (const PrimitiveType &externPrimitive) = default;
  PrimitiveType &operator= (PrimitiveType &&externPrimitive) = delete;
  PrimitiveType &operator= (const PrimitiveType &externPrimitive) = delete;
  ~PrimitiveType () override = default;

  std::size_t getNumNodesRecursively () const override;
  std::vector<ASTNode *> getChildren () const override;
  PrimitiveType *duplicateDeep (ASTNodeMemory &mem) const override;
  std::string toString () const override;

  PrimitiveClass getPrimitiveClass () const;
  void setPrimitiveClass (const PrimitiveClass &p_primitiveClass);

 protected:
  PrimitiveClass primitiveClass;
  void __duplicateDeep (ASTNodeMemory &mem, const PrimitiveType *type);
};

inline PrimitiveType::PrimitiveType (ASTNodeMemory &mem)
    : Type (mem, ASTNodeClass::PrimitiveType, TypeClass::Primitive),
      primitiveClass (PrimitiveClass::Bool)
{}

inline PrimitiveType::PrimitiveType (ASTNodeMemory &mem, const CSTType *cst)
    : Type (mem, ASTNodeClass::PrimitiveType, cst, TypeClass::Primitive),
      primitiveClass (PrimitiveClass::Bool)
{}

inline std::size_t PrimitiveType::getNumNodesRecursively () const
{
  return 1;
}

inline std::vector<ASTNode *> PrimitiveType::getChildren () const
{
  return std::vector<ASTNode *> {};
}

inline PrimitiveType *PrimitiveType::duplicateDeep (ASTNodeMemory &mem) const
{
  PrimitiveType *primitiveType = new PrimitiveType (mem);
  primitiveType->__duplicateDeep (mem, this);
  return primitiveType;
}

inline std::string PrimitiveType::toString () const
{
  switch (primitiveClass)
    {
      case PrimitiveClass::Bool: return "bool";
      case PrimitiveClass::Character: return "char";
      case PrimitiveClass::Integer8: return "i8";
      case PrimitiveClass::Integer16: return "i16";
      case PrimitiveClass::Integer32: return "i32";
      case PrimitiveClass::Integer64: return "i64";
      case PrimitiveClass::FP32: return "f32";
      case PrimitiveClass::FP64: return "f64";
      case PrimitiveClass::Complex: return "complex";
      case PrimitiveClass::Symbol: return "sym";
      case PrimitiveClass::Month: return "m";
      case PrimitiveClass::Date: return "d";
      case PrimitiveClass::DateTime: return "z";
      case PrimitiveClass::Minute: return "u";
      case PrimitiveClass::Second: return "v";
      case PrimitiveClass::Time: return "t";
      case PrimitiveClass::String: return "str";
      case PrimitiveClass::Table: return "table";
      case PrimitiveClass::KeyTable: return "ktable";
    }
}

inline PrimitiveType::PrimitiveClass PrimitiveType::getPrimitiveClass () const
{ return primitiveClass; }

inline void
PrimitiveType::setPrimitiveClass (const PrimitiveClass &p_primitiveClass)
{ primitiveClass = p_primitiveClass; }

inline void
PrimitiveType::__duplicateDeep (ASTNodeMemory &mem, const PrimitiveType *type)
{
  assert (type != nullptr);
  Type::__duplicateDeep (mem, type);
  primitiveClass = type->primitiveClass;
}

}
}