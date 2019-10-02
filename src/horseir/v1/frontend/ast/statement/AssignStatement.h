#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class AssignStatement : public Statement {
 public:
  AssignStatement ();
  explicit AssignStatement (const CSTType *cst);
  AssignStatement (AssignStatement &&statement) = default;
  AssignStatement (const AssignStatement &statement) = default;
  AssignStatement &operator= (AssignStatement &&statement) = delete;
  AssignStatement &operator= (const AssignStatement &statement) = delete;
  ~AssignStatement () override = default;

  ReturnTypePolicy getReturnTypePolicy () const;
  void setReturnTypePolicy (const ReturnTypePolicy &policy);

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

  std::pair<Operand *, Type *> getRHSInfo () const;
  template<class T>
  std::enable_if_t<std::is_assignable<std::pair<Operand *, Type *>, T>::value>
  setRHSInfo (T &&newRHSInfo);
  Operand *getRHSOperand () const;
  void setRHSOperand (Operand *operand);
  Type *getRHSType () const;
  void setRHSType (Type *type);

  std::size_t getNumNodesRecursively () const override;
  std::vector<ASTNode *> getChildren () const override;
  AssignStatement *duplicateDeep (ASTNodeMemory &mem) const override;

 protected:
  ReturnTypePolicy returnTypePolicy;
  std::pair<Identifier *, Type *> lhs;
  std::pair<Operand *, Type *> rhs;

  void __duplicateDeep (ASTNodeMemory &mem, const AssignStatement *stmt);
};

inline AssignStatement::AssignStatement ()
    : Statement (ASTNodeClass::AssignStatement, StatementClass::Assign),
      returnTypePolicy (ReturnTypePolicy::Direct),
      lhs (std::make_pair (nullptr, nullptr)),
      rhs (std::make_pair (nullptr, nullptr))
{}

inline AssignStatement::AssignStatement (const CSTType *cst)
    : Statement (ASTNodeClass::AssignStatement, cst, StatementClass::Assign),
      returnTypePolicy (ReturnTypePolicy::Direct),
      lhs (std::make_pair (nullptr, nullptr)),
      rhs (std::make_pair (nullptr, nullptr))
{}

inline Statement::ReturnTypePolicy AssignStatement::getReturnTypePolicy () const
{ return returnTypePolicy; }

inline void
AssignStatement::setReturnTypePolicy (const ReturnTypePolicy &policy)
{ returnTypePolicy = policy; }

inline std::pair<Identifier *, Type *> AssignStatement::getLHSInfo () const
{ return lhs; }

template<class T>
inline std::enable_if_t<
    std::is_assignable<std::pair<Identifier *, Type *>, T>::value
>
AssignStatement::setLHSInfo (T &&newLHSInfo)
{
  lhs = std::forward<T> (newLHSInfo);
  if (lhs.first != nullptr) lhs.first->setParentASTNode (this);
  if (lhs.second != nullptr) lhs.second->setParentASTNode (this);
}

inline Identifier *AssignStatement::getLHSIdentifier () const
{ return lhs.first; }

inline void AssignStatement::setLHSIdentifier (Identifier *identifier)
{
  lhs.first = identifier;
  if (lhs.first != nullptr) lhs.first->setParentASTNode (this);
}

inline Type *AssignStatement::getLHSType () const
{ return lhs.second; }

inline void AssignStatement::setLHSType (Type *type)
{
  lhs.second = type;
  if (lhs.second != nullptr) lhs.second->setParentASTNode (this);
}

inline std::pair<Operand *, Type *> AssignStatement::getRHSInfo () const
{ return rhs; }

template<class T>
inline std::enable_if_t<
    std::is_assignable<std::pair<Operand *, Type *>, T>::value
>
AssignStatement::setRHSInfo (T &&newRHSInfo)
{
  rhs = std::forward<T> (newRHSInfo);
  if (rhs.first != nullptr) rhs.first->setParentASTNode (this);
  if (rhs.second != nullptr) rhs.second->setParentASTNode (this);
}

inline Operand *AssignStatement::getRHSOperand () const
{ return rhs.first; }

inline void AssignStatement::setRHSOperand (Operand *operand)
{
  rhs.first = operand;
  if (rhs.first != nullptr) rhs.first->setParentASTNode (this);
}

inline Type *AssignStatement::getRHSType () const
{ return rhs.second; }

inline void AssignStatement::setRHSType (Type *type)
{
  rhs.second = type;
  if (rhs.second != nullptr) rhs.second->setParentASTNode (this);
}

inline std::size_t AssignStatement::getNumNodesRecursively () const
{
  std::size_t counter = 1;
  if (lhs.first != nullptr)
    { counter = counter + lhs.first->getNumNodesRecursively (); }
  if (lhs.second != nullptr)
    { counter = counter + lhs.second->getNumNodesRecursively (); }
  if (rhs.first != nullptr)
    { counter = counter + rhs.first->getNumNodesRecursively (); }
  if (rhs.second != nullptr)
    { counter = counter + rhs.second->getNumNodesRecursively (); }
  return counter;
}

inline std::vector<ASTNode *> AssignStatement::getChildren () const
{
  std::vector<ASTNode *> childrenNodes{};
  if (lhs.first != nullptr)
    { childrenNodes.push_back (static_cast<ASTNode *>(lhs.first)); }
  if (lhs.second != nullptr)
    { childrenNodes.push_back (static_cast<ASTNode *>(lhs.second)); }
  if (rhs.first != nullptr)
    { childrenNodes.push_back (static_cast<ASTNode *>(rhs.first)); }
  if (rhs.second != nullptr)
    { childrenNodes.push_back (static_cast<ASTNode *>(rhs.second)); }
  return childrenNodes;
}

inline AssignStatement *
AssignStatement::duplicateDeep (ASTNodeMemory &mem) const
{
  auto assignStatement = mem.alloc<AssignStatement> ();
  assignStatement->__duplicateDeep (mem, this);
  return assignStatement;
}

inline void
AssignStatement::__duplicateDeep (ASTNodeMemory &mem,
                                  const AssignStatement *stmt)
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
  Operand *rhsOperand = nullptr;
  if (stmt->rhs.first != nullptr)
    {
      rhsOperand = dynamic_cast<Operand *>(
          stmt->rhs.first->duplicateDeep (mem)
      );
      rhsOperand->setParentASTNode (this);
    }
  Type *rhsType = nullptr;
  if (stmt->rhs.second != nullptr)
    {
      rhsType = dynamic_cast<Type *> (stmt->rhs.second->duplicateDeep (mem));
      rhsType->setParentASTNode (this);
    }
  returnTypePolicy = stmt->returnTypePolicy;
  lhs = std::make_pair (lhsIdentifier, lhsType);
  rhs = std::make_pair (rhsOperand, rhsType);
}

}
}