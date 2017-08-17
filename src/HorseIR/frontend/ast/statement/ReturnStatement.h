#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class ReturnStatement : public Statement {
 public:
  ReturnStatement ();
  explicit ReturnStatement (const CSTType *cst);
  ReturnStatement (ReturnStatement &&statement) = default;
  ReturnStatement (const ReturnStatement &statement) = default;
  ReturnStatement &operator= (ReturnStatement &&statement) = delete;
  ReturnStatement &operator= (const ReturnStatement &statement) = delete;
  ~ReturnStatement () override = default;

  Operand *getOperand () const;
  void setOperand (Operand *newOperand);

  std::size_t getNumNodesRecursively () const override;
  std::vector<ASTNode *> getChildren () const override;
  ReturnStatement *duplicateDeep (ASTNodeMemory &mem) const override;
  std::string toString () const override;

 protected:
  Operand *operand = nullptr;
  void __duplicateDeep (ASTNodeMemory &mem, const ReturnStatement *stmt);
};

inline ReturnStatement::ReturnStatement ()
    : Statement (ASTNodeClass::ReturnStatement, StatementClass::Return)
{}

inline ReturnStatement::ReturnStatement (const CSTType *cst)
    : Statement (ASTNodeClass::ReturnStatement, cst, StatementClass::Return)
{}

inline Operand *ReturnStatement::getOperand () const
{ return operand; }

inline void ReturnStatement::setOperand (Operand *newOperand)
{
  operand = newOperand;
  if (operand != nullptr) operand->setParentASTNode (this);
}

inline std::size_t ReturnStatement::getNumNodesRecursively () const
{
  if (operand == nullptr) return 1;
  return 1 + operand->getNumNodesRecursively ();
}

inline std::vector<ASTNode *> ReturnStatement::getChildren () const
{
  if (operand == nullptr) return std::vector<ASTNode *>{};
  return std::vector<ASTNode *>{static_cast<ASTNode *>(operand)};
}

inline ReturnStatement *
ReturnStatement::duplicateDeep (ASTNodeMemory &mem) const
{
  auto returnStatement = mem.alloc<ReturnStatement> ();
  returnStatement->__duplicateDeep (mem, this);
  return returnStatement;
}

inline std::string ReturnStatement::toString () const
{
  std::ostringstream stream;
  stream << "return "
         << ((operand == nullptr) ? "nullptr" : operand->toString ())
         << ";";
  return stream.str ();
}

inline void
ReturnStatement::__duplicateDeep (ASTNodeMemory &mem,
                                  const ReturnStatement *stmt)
{
  assert (stmt != nullptr);
  Statement::__duplicateDeep (mem, stmt);
  Operand *duplicateOperand = nullptr;
  if (stmt->operand != nullptr)
    {
      duplicateOperand = dynamic_cast<Operand *>(
          stmt->operand->duplicateDeep (mem)
      );
      duplicateOperand->setParentASTNode (this);
    }
  operand = duplicateOperand;
}

}
}