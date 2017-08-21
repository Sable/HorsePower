#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class DictionaryType : public Type {
 public:
  DictionaryType ();
  explicit DictionaryType (const CSTType *cst);
  DictionaryType (DictionaryType &&dictionaryType) = default;
  DictionaryType (const DictionaryType &dictionaryType) = default;
  DictionaryType &operator= (DictionaryType &&dictionaryType) = delete;
  DictionaryType &operator= (const DictionaryType &dictionaryType) = delete;
  ~DictionaryType () override = default;

  std::size_t getNumNodesRecursively () const override;
  std::vector<ASTNode *> getChildren () const override;
  DictionaryType *duplicateDeep (ASTNodeMemory &mem) const override;

  Type *getKeyType () const;
  Type *getValueType () const;
  void setKeyType (Type *type);
  void setValueType (Type *type);
 protected:
  Type *keyType = nullptr;
  Type *valueType = nullptr;
  void __duplicateDeep (ASTNodeMemory &mem, const DictionaryType *type);
};

inline DictionaryType::DictionaryType ()
    : Type (ASTNodeClass::DictionaryType, TypeClass::Dictionary)
{}

inline DictionaryType::DictionaryType (const CSTType *cst)
    : Type (ASTNodeClass::DictionaryType, cst, TypeClass::Dictionary)
{}

inline std::size_t DictionaryType::getNumNodesRecursively () const
{
  std::size_t count = 1;
  if (keyType != nullptr) count += keyType->getNumNodesRecursively ();
  if (valueType != nullptr) count += valueType->getNumNodesRecursively ();
  return count;
}

inline std::vector<ASTNode *> DictionaryType::getChildren () const
{
  std::vector<ASTNode *> returnVector{};
  if (keyType != nullptr)
    returnVector.push_back (static_cast<ASTNode *>(keyType));
  if (valueType != nullptr)
    returnVector.push_back (static_cast<ASTNode *>(valueType));
  return returnVector;
}

inline DictionaryType *DictionaryType::duplicateDeep (ASTNodeMemory &mem) const
{
  auto dictionaryType = mem.alloc<DictionaryType> ();
  dictionaryType->__duplicateDeep (mem, this);
  return dictionaryType;
}

inline Type *DictionaryType::getKeyType () const
{ return keyType; }

inline Type *DictionaryType::getValueType () const
{ return valueType; }

inline void DictionaryType::setKeyType (Type *type)
{
  keyType = type;
  if (keyType != nullptr) keyType->setParentASTNode (this);
}

inline void DictionaryType::setValueType (Type *type)
{
  valueType = type;
  if (valueType != nullptr) valueType->setParentASTNode (this);
}

inline void
DictionaryType::__duplicateDeep (ASTNodeMemory &mem, const DictionaryType *type)
{
  assert (type != nullptr);
  Type::__duplicateDeep (mem, type);
  if (type->keyType != nullptr)
    {
      auto p = dynamic_cast<Type *>(type->keyType->duplicateDeep (mem));
      assert (p != nullptr);
      p->setParentASTNode (this);
      keyType = p;
    }
  if (type->valueType != nullptr)
    {
      auto p = dynamic_cast<Type *>(type->valueType->duplicateDeep (mem));
      assert (p != nullptr);
      p->setParentASTNode (this);
      valueType = p;
    }
}

}
}