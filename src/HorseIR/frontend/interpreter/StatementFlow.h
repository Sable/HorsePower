#pragma once

#include <unordered_map>
#include <unordered_set>
#include "../ast/AST.h"

#ifndef NDEBUG
#include <sstream>
#include "../ast/ASTPrinter.h"
#include "../misc/InfixOStreamIterator.h"
#endif

namespace horseIR
{
namespace interpreter
{

class StatementFlow {
 public:
  using OutwardFlowType = std::pair<const ast::Statement *,
                                    const ast::Statement *>;
  using InwardFlowType = std::unordered_set<const ast::Statement *>;

  explicit StatementFlow (const ast::CompilationUnit *compilationUnit);
  StatementFlow (StatementFlow &&statementFlow) = default;
  StatementFlow (const StatementFlow &statementFlow) = default;
  StatementFlow &operator= (StatementFlow &&statementFlow) = default;
  StatementFlow &operator= (const StatementFlow &statementFlow) = default;
  ~StatementFlow () = default;

  const ast::Statement *
  getOutwardFlowOnTrue (const ast::Statement *statement) const;
  const ast::Statement *
  getOutwardFlowOnFalse (const ast::Statement *statement) const;
  const OutwardFlowType getOutwardFlow (const ast::Statement *statement) const;

  const InwardFlowType getInwardFlow (const ast::Statement *statement) const;
  InwardFlowType::iterator inwardFlowBegin (const ast::Statement *statement);
  InwardFlowType::iterator inwardFlowEnd (const ast::Statement *statement);
  InwardFlowType::const_iterator
  inwardFlowConstBegin (const ast::Statement *statement) const;
  InwardFlowType::const_iterator
  inwardFlowConstEnd (const ast::Statement *statement) const;

#ifndef NDEBUG
  class StatementFlowPrinter : public ast::ASTPrinter {
   public:
    StatementFlowPrinter (std::ostream &stream, const StatementFlow &flow);
    StatementFlowPrinter (StatementFlowPrinter &&p) = default;
    StatementFlowPrinter (const StatementFlowPrinter &p) = default;
    StatementFlowPrinter &operator= (StatementFlowPrinter &&p) = delete;
    StatementFlowPrinter &operator= (const StatementFlowPrinter &p) = delete;
    ~StatementFlowPrinter () override = default;

    void print ();
   protected:
    using AssignStmtPtr = const ast::AssignStatement *;
    using BranchStmtPtr = const ast::BranchStatement *;
    using InvokeStmtPtr = const ast::InvokeStatement *;
    using LabelStmtPtr = const ast::LabelStatement *;
    using PhiStmtPtr = const ast::PhiStatement *;
    using ReturnStmtPtr = const ast::ReturnStatement *;

    void caseAssignStatement (AssignStmtPtr stmt, std::size_t indent) override;
    void caseBranchStatement (BranchStmtPtr stmt, std::size_t indent) override;
    void caseInvokeStatement (InvokeStmtPtr stmt, std::size_t indent) override;
    void caseLabelStatement (LabelStmtPtr stmt, std::size_t indent) override;
    void casePhiStatement (PhiStmtPtr stmt, std::size_t indent) override;
    void caseReturnStatement (ReturnStmtPtr stmt, std::size_t indent) override;

    void printStatementWithWrapper (const ast::Statement *stmt, size_t indent);

    void printOutwardFlow (const ast::Statement *statement);
    void printInwardFlow (const ast::Statement *statement);

    std::unordered_map<const ast::Statement *, std::size_t> stmtNumberMap;
    const StatementFlow &statementFlow;
  };
  StatementFlowPrinter getPrinter (std::ostream &stream) const;
#endif

 protected:
  void analysis (const ast::CompilationUnit *pCompilationUnit);
  void analysis (const ast::Module *module);
  void analysis (const ast::Method *method);

  const ast::CompilationUnit *compilationUnit;
  std::unordered_map<const ast::Statement *, OutwardFlowType> outwardFlowMap;
  std::unordered_map<const ast::Statement *, InwardFlowType> inwardFlowMap;
};

}
}