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

  explicit PrimitiveType ();
  explicit PrimitiveType (const CSTType *cst);
  PrimitiveType (PrimitiveType &&externPrimitive) = default;
  PrimitiveType (const PrimitiveType &externPrimitive) = default;
  PrimitiveType &operator= (PrimitiveType &&externPrimitive) = delete;
  PrimitiveType &operator= (const PrimitiveType &externPrimitive) = delete;
  ~PrimitiveType () override = default;

  std::size_t getNumNodesRecursively () const override;
  std::vector<ASTNode *> getChildren () const override;
  PrimitiveType *duplicateDeep (ASTNodeMemory &mem) const override;

  PrimitiveClass getPrimitiveClass () const;
  void setPrimitiveClass (const PrimitiveClass &p_primitiveClass);

 protected:
  PrimitiveClass primitiveClass;
  void __duplicateDeep (ASTNodeMemory &mem, const PrimitiveType *type);
};

inline PrimitiveType::PrimitiveType ()
    : Type (ASTNodeClass::PrimitiveType, TypeClass::Primitive),
      primitiveClass (PrimitiveClass::Bool)
{}

inline PrimitiveType::PrimitiveType (const CSTType *cst)
    : Type (ASTNodeClass::PrimitiveType, cst, TypeClass::Primitive),
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
  PrimitiveType *primitiveType = mem.alloc<PrimitiveType> ();
  primitiveType->__duplicateDeep (mem, this);
  return primitiveType;
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