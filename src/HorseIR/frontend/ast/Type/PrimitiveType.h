#pragma once

#include <vector>
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

  explicit PrimitiveType (ASTNodeMemory &mem)
      : Type (mem, ASTNodeClass::PrimitiveType, TypeClass::Primitive)
  {}
  PrimitiveType (ASTNodeMemory &mem, const CSTType *parseTree)
      : Type (mem, ASTNodeClass::PrimitiveType, parseTree, TypeClass::Primitive)
  {}
  PrimitiveType (ASTNodeMemory &mem, const CSTType *parseTree, ASTNode *parent)
      : Type (mem, ASTNodeClass::PrimitiveType, parseTree, parent,
              TypeClass::Primitive)
  {}
  PrimitiveType (PrimitiveType &&externPrimitive) = default;
  PrimitiveType (const PrimitiveType &externPrimitive) = default;
  PrimitiveType &operator= (PrimitiveType &&externPrimitive) = default;
  PrimitiveType &operator= (const PrimitiveType &externPrimitive) = default;
  virtual ~PrimitiveType () override = default;

  virtual std::size_t getNumNodesRecursively () const override;
  virtual std::vector<ASTNode *> getChildren () const override;
  virtual std::string toString () const override;
  virtual PrimitiveType *duplicateDeep (ASTNodeMemory &mem) const override;

  virtual bool isGeneralizationOf (Type *type) const override;
  virtual bool isSameAs (Type *type) const override;

  PrimitiveClass getPrimitiveClass () const;
  PrimitiveClass &setPrimitiveClass (const PrimitiveClass &p_primitiveClass);
 protected:
  PrimitiveClass primitiveClass;
};

inline std::size_t PrimitiveType::getNumNodesRecursively () const
{
  return 1;
}

inline std::vector<ASTNode *> PrimitiveType::getChildren () const
{
  return std::vector<ASTNode *> {};
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

}
}