#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class LabelStatement : public Statement {
 public:
  LabelStatement ();
  explicit LabelStatement (const CSTType *cst);
  LabelStatement (LabelStatement &&statement) = default;
  LabelStatement (const LabelStatement &statement) = default;
  LabelStatement &operator= (LabelStatement &&statement) = delete;
  LabelStatement &operator= (const LabelStatement &statement) = delete;
  ~LabelStatement () override = default;

  std::string getLabelName () const;
  template<class T>
  std::enable_if_t<std::is_assignable<std::string, T>::value>
  setValue (T &&newLabelName);

  std::size_t getNumNodesRecursively () const override;
  std::vector<ASTNode *> getChildren () const override;
  LabelStatement *duplicateDeep (ASTNode::ASTNodeMemory &mem) const override;

 protected:
  std::string labelName{};
  void __duplicateDeep (ASTNodeMemory &mem, const LabelStatement *stmt);
};

inline LabelStatement::LabelStatement ()
    : Statement (ASTNodeClass::LabelStatement, StatementClass::Label)
{}

inline LabelStatement::LabelStatement (const CSTType *cst)
    : Statement (ASTNodeClass::LabelStatement, cst, StatementClass::Label)
{}

inline std::string LabelStatement::getLabelName () const
{ return labelName; }

template<class T>
inline std::enable_if_t<std::is_assignable<std::string, T>::value>
LabelStatement::setValue (T &&newLabelName)
{ labelName = std::forward<T> (newLabelName); }

inline std::size_t LabelStatement::getNumNodesRecursively () const
{ return 1; }

inline std::vector<ASTNode *> LabelStatement::getChildren () const
{ return std::vector<ASTNode *>{}; }

inline LabelStatement *
LabelStatement::duplicateDeep (ASTNode::ASTNodeMemory &mem) const
{
  auto labelStatement = mem.alloc<LabelStatement> ();
  labelStatement->__duplicateDeep (mem, this);
  return labelStatement;
}

inline void
LabelStatement::__duplicateDeep (ASTNodeMemory &mem, const LabelStatement *stmt)
{
  assert (stmt != nullptr);
  Statement::__duplicateDeep (mem, stmt);
  labelName = stmt->labelName;
}

}
}