#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class InvokeStatement : public Statement {
 public:
  using OperandIterator = std::vector<Operand *>::iterator;
  using OperandConstIterator = std::vector<Operand *>::const_iterator;

  InvokeStatement ();
  explicit InvokeStatement (const CSTType *cst);
  InvokeStatement (InvokeStatement &&statement) = default;
  InvokeStatement (const InvokeStatement &statement) = default;
  InvokeStatement &operator= (InvokeStatement &&statement) = default;
  InvokeStatement &operator= (const InvokeStatement &statement) = default;
  ~InvokeStatement () override = default;

  ReturnTypePolicy getReturnTypePolicy () const;
  void setReturnTypePolicy (const ReturnTypePolicy &policy);

  std::string getTargetModuleName () const;
  template<class T>
  std::enable_if_t<std::is_assignable<std::string, T>::value>
  setTargetModuleName (T &&newModuleName);

  std::string getTargetMethodName () const;
  template<class T>
  std::enable_if_t<std::is_assignable<std::string, T>::value>
  setTargetMethodName (T &&newMethodName);

  std::pair<Identifier *, Type *> getLHSInfo () const;
  template<class T>
  std::enable_if_t<
      std::is_assignable<std::pair<Identifier *, Type *>, T>::value
  >
  setLHSInfo (T &&newLHSInfo);
  Identifier *getLHSIdentifier () const;
  void setLHSIdentifier (Identifier *identifier);
  Type *getLHSType () const;
  void setLHSType (Type *type);

  std::pair<std::vector<Operand *>, Type *> getRHSInfo () const;
  template<class T>
  std::enable_if_t<
      std::is_assignable<std::pair<std::vector<Operand *>, Type *>, T>::value
  >
  setRHSInfo (T &&newRHSInfo);
  std::vector<Operand *> getRHSOperands () const;
  OperandIterator rhsOperandsBegin ();
  OperandIterator rhsOperandsEnd ();
  OperandConstIterator rhsOperandsConstBegin () const;
  OperandConstIterator rhsOperandsConstEnd () const;
  template<class T>
  std::enable_if_t<std::is_assignable<std::vector<Operand *>, T>::value>
  setRHSOperands (T &&newOperands);
  Type *getRHSType () const;
  void setRHSType (Type *type);

  std::size_t getNumNodesRecursively () const override;
  std::vector<ASTNode *> getChildren () const override;
  InvokeStatement *duplicateDeep (ASTNodeMemory &mem) const override;

 protected:
  ReturnTypePolicy returnTypePolicy;
  std::string moduleName;
  std::string methodName;
  std::pair<Identifier *, Type *> lhs;
  std::pair<std::vector<Operand *>, Type *> rhs;

  void __duplicateDeep (ASTNodeMemory &mem, const InvokeStatement *stmt);
};

inline InvokeStatement::InvokeStatement ()
    : Statement (ASTNodeClass::InvokeStatement, StatementClass::Invoke),
      returnTypePolicy (ReturnTypePolicy::Direct),
      moduleName{}, methodName{},
      lhs (std::make_pair (nullptr, nullptr)),
      rhs (std::make_pair (std::vector<Operand *>{}, nullptr))
{}

inline InvokeStatement::InvokeStatement (const CSTType *cst)
    : Statement (ASTNodeClass::InvokeStatement, cst, StatementClass::Invoke),
      returnTypePolicy (ReturnTypePolicy::Direct),
      moduleName{}, methodName{},
      lhs (std::make_pair (nullptr, nullptr)),
      rhs (std::make_pair (std::vector<Operand *>{}, nullptr))
{}

inline Statement::ReturnTypePolicy InvokeStatement::getReturnTypePolicy () const
{ return returnTypePolicy; }

inline void
InvokeStatement::setReturnTypePolicy (const ReturnTypePolicy &policy)
{ returnTypePolicy = policy; }

inline std::string InvokeStatement::getTargetModuleName () const
{ return moduleName; }

template<class T>
inline std::enable_if_t<std::is_assignable<std::string, T>::value>
InvokeStatement::setTargetModuleName (T &&newModuleName)
{ moduleName = std::forward<T> (newModuleName); }

inline std::string InvokeStatement::getTargetMethodName () const
{ return methodName; }

template<class T>
inline std::enable_if_t<std::is_assignable<std::string, T>::value>
InvokeStatement::setTargetMethodName (T &&newMethodName)
{ methodName = std::forward<T> (newMethodName); }

inline std::pair<Identifier *, Type *> InvokeStatement::getLHSInfo () const
{ return lhs; }

template<class T>
inline std::enable_if_t<
    std::is_assignable<std::pair<Identifier *, Type *>, T>::value
>
InvokeStatement::setLHSInfo (T &&newLHSInfo)
{
  lhs = std::forward<T> (newLHSInfo);
  if (lhs.first != nullptr) lhs.first->setParentASTNode (this);
  if (lhs.second != nullptr) lhs.second->setParentASTNode (this);
}

inline Identifier *InvokeStatement::getLHSIdentifier () const
{ return lhs.first; }

inline void InvokeStatement::setLHSIdentifier (Identifier *identifier)
{
  lhs.first = identifier;
  if (lhs.first != nullptr) lhs.first->setParentASTNode (this);
}

inline Type *InvokeStatement::getLHSType () const
{ return lhs.second; }

inline void InvokeStatement::setLHSType (Type *type)
{
  lhs.second = type;
  if (lhs.second != nullptr) lhs.second->setParentASTNode (this);
}

inline std::pair<std::vector<Operand *>, Type *>
InvokeStatement::getRHSInfo () const
{ return rhs; }

template<class T>
inline std::enable_if_t<
    std::is_assignable<std::pair<std::vector<Operand *>, Type *>, T>::value
>
InvokeStatement::setRHSInfo (T &&newRHSInfo)
{
  rhs = std::forward<T> (newRHSInfo);
  for (auto iter = rhs.first.cbegin (); iter != rhs.first.end (); ++iter)
    if (*iter != nullptr)
      { (*iter)->setParentASTNode (this); }
  if (rhs.second != nullptr) rhs.second->setParentASTNode (this);
}

inline std::vector<Operand *> InvokeStatement::getRHSOperands () const
{ return rhs.first; }

inline InvokeStatement::OperandIterator InvokeStatement::rhsOperandsBegin ()
{ return rhs.first.begin (); }

inline InvokeStatement::OperandIterator InvokeStatement::rhsOperandsEnd ()
{ return rhs.first.end (); }

inline InvokeStatement::OperandConstIterator
InvokeStatement::rhsOperandsConstBegin () const
{ return rhs.first.cbegin (); }

inline InvokeStatement::OperandConstIterator
InvokeStatement::rhsOperandsConstEnd () const
{ return rhs.first.cend (); }

template<class T>
inline std::enable_if_t<std::is_assignable<std::vector<Operand *>, T>::value>
InvokeStatement::setRHSOperands (T &&newOperands)
{
  lhs.first = std::forward<T> (newOperands);
  for (auto iter = rhs.first.cbegin (); iter != rhs.first.cend (); ++iter)
    if (*iter != nullptr)
      { (*iter)->setParentASTNode (this); }
}

inline Type *InvokeStatement::getRHSType () const
{ return rhs.second; }

inline void InvokeStatement::setRHSType (Type *type)
{
  rhs.second = type;
  if (rhs.second != nullptr) rhs.second->setParentASTNode (this);
}

inline std::size_t InvokeStatement::getNumNodesRecursively () const
{
  std::size_t counter = 1;
  if (lhs.first != nullptr)
    { counter = counter + lhs.first->getNumNodesRecursively (); }
  if (lhs.second != nullptr)
    { counter = counter + lhs.second->getNumNodesRecursively (); }
  for (auto iter = rhs.first.cbegin (); iter != rhs.first.cend (); ++iter)
    if (*iter != nullptr)
      { counter = counter + (*iter)->getNumNodesRecursively (); }
  if (rhs.second != nullptr)
    { counter = counter + rhs.second->getNumNodesRecursively (); }
  return counter;
}

inline std::vector<ASTNode *> InvokeStatement::getChildren () const
{
  std::vector<ASTNode *> childrenNodes{};
  childrenNodes.reserve (3 + rhs.first.size ());
  if (lhs.first != nullptr)
    { childrenNodes.push_back (static_cast<ASTNode *>(lhs.first)); }
  if (lhs.second != nullptr)
    { childrenNodes.push_back (static_cast<ASTNode *>(lhs.second)); }
  for (auto iter = rhs.first.cbegin (); iter != rhs.first.cend (); ++iter)
    if (*iter != nullptr)
      { childrenNodes.push_back (static_cast<ASTNode *>(*iter)); }
  if (rhs.second != nullptr)
    { childrenNodes.push_back (static_cast<ASTNode *>(rhs.second)); }
  return childrenNodes;
}

inline InvokeStatement *
InvokeStatement::duplicateDeep (ASTNodeMemory &mem) const
{
  auto invokeStatement = mem.alloc<InvokeStatement> ();
  invokeStatement->__duplicateDeep (mem, this);
  return invokeStatement;
}

inline void
InvokeStatement::__duplicateDeep (ASTNodeMemory &mem,
                                  const InvokeStatement *stmt)
{
  assert (stmt != nullptr);
  Statement::__duplicateDeep (mem, stmt);
  Identifier *lhsIdentifier = nullptr;
  if (stmt->lhs.first != nullptr)
    {
      lhsIdentifier = stmt->lhs.first->duplicateDeep (mem);
      lhsIdentifier->setParentASTNode (this);
    }
  Type *lhsType = nullptr;
  if (stmt->lhs.second != nullptr)
    {
      lhsType = dynamic_cast<Type *>(stmt->lhs.second->duplicateDeep (mem));
      lhsType->setParentASTNode (this);
    }
  std::vector<Operand *> invokeOperands{};
  invokeOperands.reserve (stmt->rhs.first.size ());
  for (Operand *iterOperand : stmt->rhs.first)
    if (iterOperand != nullptr)
      {
        Operand *duplicateOperand = dynamic_cast<Operand *>(
            iterOperand->duplicateDeep (mem)
        );
        duplicateOperand->setParentASTNode (this);
        invokeOperands.push_back (duplicateOperand);
      }
    else
      { invokeOperands.push_back (nullptr); }
  Type *rhsType = nullptr;
  if (stmt->rhs.second != nullptr)
    {
      rhsType = dynamic_cast<Type *>(stmt->rhs.second->duplicateDeep (mem));
      rhsType->setParentASTNode (this);
    }
  returnTypePolicy = stmt->returnTypePolicy;
  moduleName = stmt->moduleName;
  methodName = stmt->methodName;
  lhs = std::make_pair (lhsIdentifier, lhsType);
  rhs = std::make_pair (std::move (invokeOperands), rhsType);
}

}
}