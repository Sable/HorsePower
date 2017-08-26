#pragma once

#include <unordered_map>
#include "../ast/AST.h"
#include "../../backend/h_global.h"
#include "../../backend/h_primitive.h"
#include "Dispatcher.h"
#include "LiteralConverter.h"
#include "StatementFlow.h"

namespace horseIR
{
namespace interpreter
{

class Interpreter {
 public:
  explicit Interpreter (const ast::CompilationUnit *pCompilationUnit);

  V interpret (const std::string &moduleName, const std::string &methodName);
  V interpret (const ast::Method *method);
  V interpret (const ast::Method *method, std::size_t argc, V argv[]);
 protected:
  Dispatcher dispatcher;
  StatementFlow statementFlow;

  const ast::CompilationUnit *compilationUnit;

  struct InterpretContext {
    std::unordered_map<std::string, V> localStorage;
    void writeLocalVariable (const ast::Identifier *id, V value);
    V readLocalVariable (const ast::Identifier *identifier) const;

    const ast::Statement *pc = nullptr;
    V returnValue = nullptr;
  };

  void interpret (const ast::AssignStatement *s, InterpretContext &c);
  void interpret (const ast::BranchStatement *s, InterpretContext &c);
  void interpret (const ast::InvokeStatement *s, InterpretContext &c);
  void interpret (const ast::LabelStatement *s, InterpretContext &c);
  void interpret (const ast::PhiStatement *s, InterpretContext &c);
  void interpret (const ast::ReturnStatement *s, InterpretContext &c);

  V fetchOperand (const ast::Operand *operand, InterpretContext &context);
};

}
}