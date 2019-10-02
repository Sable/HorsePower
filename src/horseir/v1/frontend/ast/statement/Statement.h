#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class Statement : public ASTNode {
 public:
  enum class StatementClass {
    Label, Assign, Invoke, Branch, Return, Phi
  };

  enum class ReturnTypePolicy {
    Direct, Cast, IsType, CheckCast
  };

  Statement (const ASTNodeClass &astNodeClass,
             const StatementClass &p_statementClass);
  Statement (const ASTNodeClass &astNodeClass, const CSTType *cst,
             const StatementClass &p_statementClass);
  Statement (Statement &&statement) = default;
  Statement (const Statement &statement) = default;
  Statement &operator= (Statement &&statement) = delete;
  Statement &operator= (const Statement &statement) = delete;
  ~Statement () override = default;

  StatementClass getStatementClass () const;

 protected:
  const StatementClass statementClass;
  void __duplicateDeep (ASTNodeMemory &mem, const Statement *stmt);
};

inline Statement::Statement (const ASTNodeClass &astNodeClass,
                             const StatementClass &p_statementClass)
    : ASTNode (astNodeClass), statementClass (p_statementClass)
{}

inline Statement::Statement (const ASTNodeClass &astNodeClass, const CSTType *cst,
                             const StatementClass &p_statementClass)
    : ASTNode (astNodeClass, cst), statementClass (p_statementClass)
{}

inline Statement::StatementClass Statement::getStatementClass () const
{ return statementClass; }

inline void
Statement::__duplicateDeep (ASTNodeMemory &mem, const Statement *stmt)
{ ASTNode::__duplicateDeep (mem, stmt); }

inline std::ostream &
operator<< (std::ostream &stream, const Statement::StatementClass &stmtClass)
{
  using StatementClass = Statement::StatementClass;
  switch (stmtClass)
    {
      case StatementClass::Label : return stream << "Label";
      case StatementClass::Assign: return stream << "Assign";
      case StatementClass::Invoke: return stream << "Invoke";
      case StatementClass::Branch: return stream << "Branch";
      case StatementClass::Return: return stream << "Return";
      case StatementClass::Phi: return stream << "Phi";
    }
}

inline std::ostream &
operator<< (std::ostream &stream, const Statement::ReturnTypePolicy &policy)
{
  using ReturnTypePolicy = Statement::ReturnTypePolicy;
  switch (policy)
    {
      case ReturnTypePolicy::Direct: return stream << "Direct";
      case ReturnTypePolicy::Cast: return stream << "Cast";
      case ReturnTypePolicy::CheckCast: return stream << "CheckCast";
      case ReturnTypePolicy::IsType: return stream << "IsType";
    }
}

}
}
