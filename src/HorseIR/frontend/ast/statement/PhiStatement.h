#pragma once

#include "../../misc/InfixOStreamIterator.h"
#include "../AST.h"

namespace horseIR
{
namespace ast
{

class PhiStatement : public Statement {
 public:
  PhiStatement ();
  explicit PhiStatement (const CSTType *cst);
  PhiStatement (PhiStatement &&statement) = default;
  PhiStatement (const PhiStatement &statement) = default;
  PhiStatement &operator= (PhiStatement &&statement) = delete;
  PhiStatement &operator= (const PhiStatement &statement) = delete;
  ~PhiStatement () override = default;

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

  std::map<std::string, Operand *> getRHSMap () const;
  template<class T>
  std::enable_if_t<
      std::is_assignable<std::map<std::string, Operand *>, T>::value
  >
  setRHSMap (T &&newRHSMap);

  std::size_t getNumNodesRecursively () const override;
  std::vector<ASTNode *> getChildren () const override;
  PhiStatement *duplicateDeep (ASTNodeMemory &mem) const override;
  std::string toString () const override;

 protected:
  std::pair<Identifier *, Type *> lhs;
  std::map<std::string, Operand *> rhs;

  void __duplicateDeep (ASTNodeMemory &mem, const PhiStatement *stmt);
};

inline PhiStatement::PhiStatement ()
    : Statement (ASTNodeClass::PhiStatement, StatementClass::Phi),
      lhs{std::make_pair (nullptr, nullptr)},
      rhs{}
{}

inline PhiStatement::PhiStatement (const CSTType *cst)
    : Statement (ASTNodeClass::PhiStatement, cst, StatementClass::Phi),
      lhs{std::make_pair (nullptr, nullptr)},
      rhs{}
{}

inline std::pair<Identifier *, Type *> PhiStatement::getLHSInfo () const
{ return lhs; }

template<class T>
inline std::enable_if_t<
    std::is_assignable<std::pair<Identifier *, Type *>, T>::value
>
PhiStatement::setLHSInfo (T &&newLHSInfo)
{
  lhs = std::forward<T> (newLHSInfo);
  if (lhs.first != nullptr) lhs.first->setParentASTNode (this);
  if (lhs.second != nullptr) lhs.second->setParentASTNode (this);
}

inline Identifier *PhiStatement::getLHSIdentifier () const
{ return lhs.first; }

inline void PhiStatement::setLHSIdentifier (Identifier *identifier)
{
  lhs.first = identifier;
  if (lhs.first != nullptr) lhs.first->setParentASTNode (this);
}

inline Type *PhiStatement::getLHSType () const
{ return lhs.second; }

inline void PhiStatement::setLHSType (Type *type)
{
  lhs.second = type;
  if (lhs.second != nullptr) lhs.second->setParentASTNode (this);
}

inline std::map<std::string, Operand *> PhiStatement::getRHSMap () const
{ return rhs; }

template<class T>
inline std::enable_if_t<
    std::is_assignable<std::map<std::string, Operand *>, T>::value
>
PhiStatement::setRHSMap (T &&newRHSMap)
{
  rhs = std::forward<T> (newRHSMap);
  for (auto &entryPair : rhs)
    if (entryPair.second != nullptr)
      { entryPair.second->setParentASTNode (this); }
}

inline std::size_t PhiStatement::getNumNodesRecursively () const
{
  std::size_t counter = 1;
  if (lhs.first != nullptr)
    { counter = counter + lhs.first->getNumNodesRecursively (); }
  if (lhs.second != nullptr)
    { counter = counter + lhs.second->getNumNodesRecursively (); }
  for (auto &entryPair : rhs)
    if (entryPair.second != nullptr)
      { counter = counter + entryPair.second->getNumNodesRecursively (); }
  return counter;
}

inline std::vector<ASTNode *> PhiStatement::getChildren () const
{
  std::vector<ASTNode *> childrenNodes{};
  if (lhs.first != nullptr)
    { childrenNodes.push_back (static_cast<ASTNode *>(lhs.first)); }
  if (lhs.second != nullptr)
    { childrenNodes.push_back (static_cast<ASTNode *>(lhs.second)); }
  for (auto &entryPair : rhs)
    if (entryPair.second != nullptr)
      { childrenNodes.push_back (static_cast<ASTNode *>(entryPair.second)); }
  return childrenNodes;
}

inline PhiStatement *PhiStatement::duplicateDeep (ASTNodeMemory &mem) const
{
  auto phiStatement = mem.alloc<PhiStatement> ();
  phiStatement->__duplicateDeep (mem, this);
  return phiStatement;
}

inline std::string PhiStatement::toString () const
{
  std::ostringstream stream;
  stream << ((lhs.first == nullptr) ? "nullptr" : lhs.first->toString ())
         << " :"
         << ((lhs.second == nullptr) ? "nullptr" : lhs.second->toString ())
         << " = phi (";
  std::transform (
      rhs.cbegin (), rhs.cend (),
      misc::InfixOStreamIterator<std::string> (stream, ", "),
      [] (const std::pair<std::string, Operand *> &entryPair) -> std::string
      {
        std::ostringstream s;
        s << "[ " << entryPair.first << " ] "
          << ((entryPair.second == nullptr) ? "nullptr"
                                            : entryPair.second->toString ());
        return s.str ();
      });
  stream << ");";
  return stream.str ();
}

inline void
PhiStatement::__duplicateDeep (ASTNodeMemory &mem, const PhiStatement *stmt)
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
  std::map<std::string, Operand *> rhsMap{};
  for (auto &entryPair : stmt->rhs)
    {
      std::string label = entryPair.first;
      auto operand = dynamic_cast<Operand *>(
          entryPair.second->duplicateDeep (mem)
      );
      rhsMap.insert (std::make_pair (std::move (label), operand));
    }
  lhs = std::make_pair (lhsIdentifier, lhsType);
  rhs = std::move (rhsMap);
}

}
}