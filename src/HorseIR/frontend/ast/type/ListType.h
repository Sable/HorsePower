#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class ListType : public Type {
 public:
  ListType ();
  explicit ListType (const CSTType *cst);
  ListType (ListType &&externListType) = default;
  ListType (const ListType &externListType) = default;
  ListType &operator= (ListType &&externListType) = delete;
  ListType &operator= (const ListType &externListType) = delete;
  ~ListType () override = default;

  std::size_t getNumNodesRecursively () const override;
  std::vector<ASTNode *> getChildren () const override;
  ListType *duplicateDeep (ASTNodeMemory &mem) const override;

  Type *getElementType () const;
  void setElementType (Type *type);
 protected:
  Type *elementType = nullptr;
  void __duplicateDeep (ASTNodeMemory &mem, const ListType *listType);
};

inline ListType::ListType ()
    : Type (ASTNodeClass::ListType, TypeClass::List)
{}

inline ListType::ListType (const CSTType *cst)
    : Type (ASTNodeClass::ListType, cst, TypeClass::List)
{}

inline std::size_t ListType::getNumNodesRecursively () const
{
  std::size_t count = 1;
  if (elementType != nullptr) count += elementType->getNumNodesRecursively ();
  return count;
}

inline std::vector<ASTNode *> ListType::getChildren () const
{
  std::vector<ASTNode *> returnVector{};
  if (elementType != nullptr)
    returnVector.push_back (static_cast<ASTNode *>(elementType));
  return returnVector;
}

inline ListType *ListType::duplicateDeep (ASTNodeMemory &mem) const
{
  auto listType = mem.alloc<ListType> ();
  listType->__duplicateDeep (mem, this);
  return listType;
}

inline Type *ListType::getElementType () const
{ return elementType; }

inline void ListType::setElementType (Type *type)
{
  elementType = type;
  if (elementType != nullptr) elementType->setParentASTNode (this);
}

inline void
ListType::__duplicateDeep (ASTNodeMemory &mem, const ListType *listType)
{
  assert (listType != nullptr);
  Type::__duplicateDeep (mem, listType);
  if (listType->elementType != nullptr)
    {
      auto p = dynamic_cast<Type *>(listType->elementType->duplicateDeep (mem));
      assert (p != nullptr);
      p->setParentASTNode (this);
      elementType = p;
    }
}

}
}