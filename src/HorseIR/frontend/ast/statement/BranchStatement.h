#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class BranchStatement : public Statement {
 public:
  BranchStatement ();
  explicit BranchStatement (const CSTType *cst);
  BranchStatement (BranchStatement &&statement) = default;
  BranchStatement (const BranchStatement &statement) = default;
  BranchStatement &operator= (BranchStatement &&statement) = delete;
  BranchStatement &operator= (const BranchStatement &statement) = delete;
  ~BranchStatement () override = default;

  bool isConditional () const;
  Operand *getOperand () const;
  void setOperand (Operand *newOperand);

  std::string getTargetLabelName () const;
  template<class T>
  std::enable_if_t<std::is_assignable<std::string, T>::value>
  setTargetLabelName (T &&newTargetLabelName);

  std::size_t getNumNodesRecursively () const override;
  std::vector<ASTNode *> getChildren () const override;
  BranchStatement *duplicateDeep (ASTNodeMemory &mem) const override;

 protected:
  std::string targetLabelName{};
  Operand *operand = nullptr;
  void __duplicateDeep (ASTNodeMemory &mem, const BranchStatement *stmt);
};

inline BranchStatement::BranchStatement ()
    : Statement (ASTNodeClass::BranchStatement, StatementClass::Branch)
{}

inline BranchStatement::BranchStatement (const CSTType *cst)
    : Statement (ASTNodeClass::BranchStatement, cst, StatementClass::Branch)
{}

inline bool BranchStatement::isConditional () const
{ return operand == nullptr; }

inline Operand *BranchStatement::getOperand () const
{ return operand; }

inline void BranchStatement::setOperand (Operand *newOperand)
{
  operand = newOperand;
  if (operand != nullptr) operand->setParentASTNode (this);
}

inline std::string BranchStatement::getTargetLabelName () const
{ return targetLabelName; }

template<class T>
inline std::enable_if_t<std::is_assignable<std::string, T>::value>
BranchStatement::setTargetLabelName (T &&newTargetLabelName)
{ targetLabelName = std::forward<T> (newTargetLabelName); }

inline std::size_t BranchStatement::getNumNodesRecursively () const
{
  if (operand == nullptr) return 1;
  return 1 + operand->getNumNodesRecursively ();
}

inline std::vector<ASTNode *> BranchStatement::getChildren () const
{
  if (operand == nullptr) return std::vector<ASTNode *> {};
  return std::vector<ASTNode *>{static_cast<ASTNode *>(operand)};
}

inline BranchStatement *
BranchStatement::duplicateDeep (ASTNodeMemory &mem) const
{
  auto branchStatement = mem.alloc<BranchStatement> ();
  branchStatement->__duplicateDeep (mem, this);
  return branchStatement;
}

inline void
BranchStatement::__duplicateDeep (ASTNodeMemory &mem,
                                  const BranchStatement *stmt)
{
  assert (stmt != nullptr);
  Operand *duplicateOperand = nullptr;
  if (stmt->operand != nullptr)
    {
      duplicateOperand = dynamic_cast<Operand *>(
          stmt->operand->duplicateDeep (mem)
      );
      duplicateOperand->setParentASTNode (this);
    }
  operand = duplicateOperand;
  targetLabelName = stmt->targetLabelName;
}

}
}