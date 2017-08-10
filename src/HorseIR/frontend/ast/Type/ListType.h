#pragma once

#include <vector>
#include <string>
#include <cstddef>
#include <sstream>
#include "../AST.h"

namespace horseIR
{
namespace ast
{

class ListType : public Type {
 public:
  explicit ListType (ASTNodeMemory &mem);
  ListType (ASTNodeMemory &mem, const CSTType *cst);
  ListType (ListType &&externListType) = default;
  ListType (const ListType &externListType) = default;
  ListType &operator= (ListType &&externListType) = delete;
  ListType &operator= (const ListType &externListType) = delete;
  ~ListType () override = default;

  std::size_t getNumNodesRecursively () const override;
  std::vector<ASTNode *> getChildren () const override;
  ListType *duplicateDeep (ASTNodeMemory &mem) const override;
  std::string toString () const override;

  Type *getElementType () const;
  void setElementType (Type *type);
 protected:
  Type *elementType = nullptr;
  void __duplicateDeep (ASTNodeMemory &mem, const ListType *listType);
};

inline ListType::ListType (ASTNodeMemory &mem)
    : Type (mem, ASTNodeClass::ListType, TypeClass::List)
{}

inline ListType::ListType (ASTNodeMemory &mem, const CSTType *cst)
    : Type (mem, ASTNodeClass::ListType, cst, TypeClass::List)
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
  auto listType = new ListType (mem);
  listType->__duplicateDeep (mem, this);
  return listType;
}

inline std::string ListType::toString () const
{
  std::ostringstream stream;
  stream << "list<"
         << ((elementType == nullptr) ? "nullptr" : elementType->toString ())
         << '>';
  return stream.str ();
}

inline Type *ListType::getElementType () const
{
  return elementType;
}

inline void ListType::setElementType (Type *type)
{
  elementType = type;
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