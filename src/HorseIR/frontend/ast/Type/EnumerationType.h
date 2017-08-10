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

class EnumerationType : public Type {
 public:
  explicit EnumerationType (ASTNodeMemory &mem);
  EnumerationType (ASTNodeMemory &mem, const CSTType *cst);
  EnumerationType (EnumerationType &&enumerationType) = default;
  EnumerationType (const EnumerationType &enumerationType) = default;
  EnumerationType &operator= (EnumerationType &&enumerationType) = delete;
  EnumerationType &operator= (const EnumerationType &enumerationType) = delete;
  ~EnumerationType () override = default;

  std::size_t getNumNodesRecursively () const override;
  std::vector<ASTNode *> getChildren () const override;
  EnumerationType *duplicateDeep (ASTNodeMemory &mem) const override;
  std::string toString () const override;

  Type *getElementType () const;
  void setElementType (Type *type);
 protected:
  Type *elementType = nullptr;
  void __duplicateDeep (ASTNodeMemory &mem, const EnumerationType *t);
};

inline EnumerationType::EnumerationType (ASTNodeMemory &mem)
    : Type (mem, ASTNodeClass::EnumerationType, TypeClass::Enumeration)
{}

inline EnumerationType::EnumerationType (ASTNodeMemory &mem, const CSTType *cst)
    : Type (mem, ASTNodeClass::EnumerationType, cst, TypeClass::Enumeration)
{}

inline std::size_t EnumerationType::getNumNodesRecursively () const
{
  std::size_t count = 1;
  if (elementType != nullptr) count += elementType->getNumNodesRecursively ();
  return count;
}

inline std::vector<ASTNode *> EnumerationType::getChildren () const
{
  std::vector<ASTNode *> returnVector{};
  if (elementType != nullptr)
    returnVector.push_back (static_cast<ASTNode *>(elementType));
  return returnVector;
}

inline EnumerationType *
EnumerationType::duplicateDeep (ASTNodeMemory &mem) const
{
  auto enumerationType = new EnumerationType (mem);
  enumerationType->__duplicateDeep (mem, this);
  return enumerationType;
}

inline std::string EnumerationType::toString () const
{
  std::ostringstream stream;
  stream << "enum<"
         << ((elementType == nullptr) ? "nullptr" : elementType->toString ())
         << '>';
  return stream.str ();
}

inline Type *EnumerationType::getElementType () const
{
  return elementType;
}

inline void EnumerationType::setElementType (Type *type)
{
  elementType = type;
}

inline void
EnumerationType::__duplicateDeep (ASTNodeMemory &mem, const EnumerationType *t)
{
  assert (t != nullptr);
  Type::__duplicateDeep (mem, t);
  if (t->elementType != nullptr)
    {
      auto p = dynamic_cast<Type *>(t->elementType->duplicateDeep (mem));
      assert (p != nullptr);
      p->setParentASTNode (this);
      elementType = p;
    }
}

}
}