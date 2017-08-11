#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class WildcardType : public Type {
 public:
  explicit WildcardType (ASTNodeMemory &mem);
  WildcardType (ASTNodeMemory &mem, const CSTType *cst);
  WildcardType (WildcardType &&wildcardType) = default;
  WildcardType (const WildcardType &wildcardType) = default;
  WildcardType &operator= (WildcardType &&wildcardType) = delete;
  WildcardType &operator= (const WildcardType &wildcardType) = delete;
  ~WildcardType () override = default;

  std::size_t getNumNodesRecursively () const override;
  std::vector<ASTNode *> getChildren () const override;
  WildcardType *duplicateDeep (ASTNodeMemory &mem) const override;
  std::string toString () const override;

 protected:
  void __duplicateDeep (ASTNodeMemory &mem, const WildcardType *type);
};

inline WildcardType::WildcardType (ASTNodeMemory &mem)
    : Type (mem, ASTNodeClass::WildcardType, TypeClass::Wildcard)
{}

inline WildcardType::WildcardType (ASTNodeMemory &mem, const CSTType *cst)
    : Type (mem, ASTNodeClass::WildcardType, cst, TypeClass::Wildcard)
{}

inline std::size_t WildcardType::getNumNodesRecursively () const
{ return 1; }

inline std::vector<ASTNode *> WildcardType::getChildren () const
{ return std::vector<ASTNode *> {}; }

inline WildcardType *WildcardType::duplicateDeep (ASTNodeMemory &mem) const
{
  auto wildcardType = new WildcardType (mem);
  wildcardType->__duplicateDeep (mem, this);
  return wildcardType;
}

inline std::string WildcardType::toString () const
{ return "?"; }

inline void
WildcardType::__duplicateDeep (ASTNodeMemory &mem, const WildcardType *type)
{
  assert (type != nullptr);
  Type::__duplicateDeep (mem, type);
}

}
}