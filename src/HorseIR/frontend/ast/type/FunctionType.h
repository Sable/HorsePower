#pragma once

#include "../AST.h"
#include "../../misc/InfixOStreamIterator.h"

namespace horseIR
{
namespace ast
{

class FunctionType : public Type {
 public:
  FunctionType ();
  explicit FunctionType (const CSTType *cst);
  FunctionType (FunctionType &&functionType) = default;
  FunctionType (const FunctionType &functionType) = default;
  FunctionType &operator= (FunctionType &&functionType) = delete;
  FunctionType &operator= (const FunctionType &functionType) = delete;
  ~FunctionType () override = default;

  std::size_t getNumNodesRecursively () const override;
  std::vector<ASTNode *> getChildren () const override;
  FunctionType *duplicateDeep (ASTNodeMemory &mem) const override;
  std::string toString () const override;

  std::vector<Type *> getParameterTypes () const;
  template<class T>
  std::enable_if_t<std::is_assignable<std::vector<Type *>, T>::value>
  setParameterTypes (T &&types);

  Type *getReturnType () const;
  void setReturnType (Type *type);

  bool getIsFlexible () const;
  void setIsFlexible (bool flexible);
 protected:
  std::vector<Type *> parameterTypes = {};
  Type *returnType = nullptr;
  bool isFlexible = false;
  void __duplicateDeep (ASTNodeMemory &mem, const FunctionType *type);
};

inline FunctionType::FunctionType ()
    : Type (ASTNodeClass::FunctionType, TypeClass::Function)
{}

inline FunctionType::FunctionType (const CSTType *cst)
    : Type (ASTNodeClass::FunctionType, cst, TypeClass::Function)
{}

inline std::size_t FunctionType::getNumNodesRecursively () const
{
  std::size_t count = 1;
  for (const auto &parameterType : parameterTypes)
    {
      if (parameterType == nullptr) continue;
      count += parameterType->getNumNodesRecursively ();
    }
  if (returnType != nullptr) count += returnType->getNumNodesRecursively ();
  return count;
}

inline std::vector<ASTNode *> FunctionType::getChildren () const
{
  std::vector<ASTNode *> returnVector{};
  for (const auto &parameterType : parameterTypes)
    {
      if (parameterType == nullptr) continue;
      returnVector.push_back (static_cast<ASTNode *>(parameterType));
    }
  if (returnType != nullptr)
    returnVector.push_back (static_cast<ASTNode *>(returnType));
  return returnVector;
}

inline FunctionType *FunctionType::duplicateDeep (ASTNodeMemory &mem) const
{
  auto functionType = mem.alloc<FunctionType> ();
  functionType->__duplicateDeep (mem, this);
  return functionType;
}

inline std::string FunctionType::toString () const
{
  std::vector<std::string> paramSegments{};
  std::transform (
      parameterTypes.cbegin (), parameterTypes.cend (),
      std::back_inserter (paramSegments),
      [] (const Type *type) -> std::string
      {
        return (type == nullptr) ? "nullptr" : type->toString ();
      }
  );
  if (isFlexible) paramSegments.emplace_back ("...");
  auto retSeg = (returnType == nullptr) ? "nullptr" : returnType->toString ();
  std::ostringstream stream;
  stream << "func<";
  std::copy (paramSegments.cbegin (), paramSegments.cend (),
             horseIR::misc::InfixOStreamIterator<std::string> (stream, ", "));
  stream << " :" << retSeg << ">";
  return stream.str ();
}

inline std::vector<Type *> FunctionType::getParameterTypes () const
{ return parameterTypes; }

template<class T>
inline std::enable_if_t<std::is_assignable<std::vector<Type *>, T>::value>
FunctionType::setParameterTypes (T &&types)
{
  parameterTypes = std::forward<T> (types);
  for (Type *&iter : parameterTypes) iter->setParentASTNode (this);
}

inline Type *FunctionType::getReturnType () const
{ return returnType; }

inline void FunctionType::setReturnType (Type *type)
{
  returnType = type;
  if (returnType != nullptr) returnType->setParentASTNode (this);
}

inline bool FunctionType::getIsFlexible () const
{ return isFlexible; }

inline void FunctionType::setIsFlexible (bool flexible)
{ isFlexible = flexible; }

inline void
FunctionType::__duplicateDeep (ASTNodeMemory &mem, const FunctionType *type)
{
  assert (type != nullptr);
  parameterTypes = {};
  parameterTypes.reserve (type->parameterTypes.size ());
  for (const auto &parameterType : type->parameterTypes)
    {
      Type *newType = nullptr;
      if (parameterType != nullptr)
        {
          newType = dynamic_cast<Type *>(parameterType->duplicateDeep (mem));
          assert (newType != nullptr);
          newType->setParentASTNode (this);
        }
      parameterTypes.push_back (newType);
    }
  Type *newRetType = nullptr;
  if (type->returnType != nullptr)
    {
      newRetType = dynamic_cast<Type *>(type->returnType->duplicateDeep (mem));
      assert (newRetType != nullptr);
      newRetType->setParentASTNode (this);
    }
  returnType = newRetType;
  isFlexible = type->isFlexible;
}

}
}