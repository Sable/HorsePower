#pragma once

#include "../ast/AST.h"

namespace horseIR
{
namespace interpreter
{

class StatementFlow {
 public:
  StatementFlow (StatementFlow &&statementFlow) = default;
  StatementFlow (const StatementFlow &statementFlow) = default;
  StatementFlow &operator= (StatementFlow &&statementFlow) = default;
  StatementFlow &operator= (const StatementFlow &statementFlow) = default;
  ~StatementFlow () = default;

  static StatementFlow construct (const ast::CompilationUnit *unit);

  struct FlowEntry {
    std::set<const ast::Statement *> inwardFlow;
    std::pair<const ast::Statement *, const ast::Statement *> outwardFlow;
  };

  FlowEntry getFlowInfo (const ast::Statement *statement) const;
  std::set<const ast::Statement *>
  getInwardFlow (const ast::Statement *statement) const;
  std::pair<const ast::Statement *, const ast::Statement *>
  getOutwardFlow (const ast::Statement *statement) const;

  const ast::Statement *
  getOutwardFlowOnTrue (const ast::Statement *statement) const;
  const ast::Statement *
  getOutwardFlowOnFalse (const ast::Statement *statement) const;

 protected:
  StatementFlow () = default;
  std::map<const ast::Statement *, FlowEntry> flowMap{};

  void constructOnMethod (const ast::Method *method);
};

}
}
