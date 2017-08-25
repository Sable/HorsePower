#include "Interpreter.h"

#include <stdexcept>
#define NOT_YET_IMPLEMENTED std::runtime_error("not yet implemented")
#define ALLOC_NODE(x) {                                  \
            (x) = allocNode();                           \
            if ((x) == nullptr) throw std::bad_alloc (); \
        }

using namespace horseIR::interpreter;

Interpreter::Interpreter (const ast::CompilationUnit *pCompilationUnit)
    : dispatcher (pCompilationUnit), statementFlow (pCompilationUnit),
      compilationUnit (pCompilationUnit)
{}

V Interpreter::interpret (const std::string &moduleName,
                          const std::string &methodName)
{
  const auto modulePos = std::find_if (
      compilationUnit->modulesConstBegin (),
      compilationUnit->modulesConstEnd (),
      [&] (const ast::Module *module) -> bool
      { return module->getModuleName () == moduleName; });
  assert (modulePos != compilationUnit->modulesConstEnd ());
  const ast::Module *module = *modulePos;
  const auto methodPos = std::find_if (
      module->methodsConstBegin (), module->methodsConstEnd (),
      [&] (const ast::Method *method) -> bool
      { return method->getMethodName () == methodName; });
  assert (methodPos != module->methodsConstEnd ());
  return interpret (*methodPos);
}

V Interpreter::interpret (const ast::Method *method)
{ return interpret (method, 0, nullptr); }

void Interpreter::InterpretContext::writeLocalVariable (
    const ast::Identifier *id, V value
)
{ localStorage.emplace (std::make_pair (id->getName (), value)); }

V Interpreter::InterpretContext::readLocalVariable (
    const ast::Identifier *identifier
) const
{ return localStorage.at (identifier->getName ()); }

#define CAST_INTERPRET(T, x, y) interpret(dynamic_cast<const T*>(x), y)

V Interpreter::interpret (const ast::Method *method, std::size_t argc, V *argv)
{
  assert (method != nullptr);
  InterpretContext context;

  const auto parametersAST = method->getParameters ();
  assert (argc == parametersAST.size ());
  for (std::size_t pos = 0; pos < argc; ++pos)
    {
      const ast::Identifier *identifier = parametersAST[pos].first;
      context.writeLocalVariable (identifier, argv[pos]);
    }
  context.pc = *(method->statementsConstBegin ());

  while (context.pc != nullptr)
    {
      using StatementClass = ast::Statement::StatementClass;
      const ast::Statement *stmt = context.pc;
      const auto statementClass = context.pc->getStatementClass ();
      if (statementClass == StatementClass::Assign)
        { CAST_INTERPRET (ast::AssignStatement, stmt, context); }
      if (statementClass == StatementClass::Branch)
        { CAST_INTERPRET (ast::BranchStatement, stmt, context); }
      if (statementClass == StatementClass::Invoke)
        { CAST_INTERPRET (ast::InvokeStatement, stmt, context); }
      if (statementClass == StatementClass::Label)
        { CAST_INTERPRET (ast::LabelStatement, stmt, context); }
      if (statementClass == StatementClass::Phi)
        { CAST_INTERPRET (ast::PhiStatement, stmt, context); }
      if (statementClass == StatementClass::Return)
        { CAST_INTERPRET (ast::ReturnStatement, stmt, context); }
    }

  return context.returnValue;
}

void Interpreter::interpret (const ast::AssignStatement *s, InterpretContext &c)
{
  assert (s != nullptr);
  V rhsValue = fetchOperand (s->getRHSOperand (), c);
  const ast::Identifier *lhsID = s->getLHSIdentifier ();
  c.writeLocalVariable (lhsID, rhsValue);
  c.pc = statementFlow.getOutwardFlowOnTrue (s);
}

void Interpreter::interpret (const ast::ReturnStatement *s, InterpretContext &c)
{
  assert (s != nullptr);
  V returnValue = fetchOperand (s->getOperand (), c);
  c.returnValue = returnValue;
  c.pc = statementFlow.getOutwardFlowOnTrue (s);
}

void Interpreter::interpret (const ast::BranchStatement *s, InterpretContext &c)
{
  assert (s != nullptr);
  throw NOT_YET_IMPLEMENTED;
}

void Interpreter::interpret (const ast::InvokeStatement *s, InterpretContext &c)
{
  assert (s != nullptr);
  const auto rhsOperands = s->getRHSOperands ();
  V argumentVector[rhsOperands.size ()];
  for (std::size_t pos = 0; pos < rhsOperands.size (); ++pos)
    { argumentVector[pos] = fetchOperand (rhsOperands.at (pos), c); }
  V rhsValue = nullptr;
  Dispatcher::MethodMETA *methodMETA = dispatcher.getMethodMETA (s);
  using MethodMETAClass = Dispatcher::MethodMETA::MethodMETAClass;
  if (methodMETA->getMethodMETAClass () == MethodMETAClass::Internal)
    {
      auto castedPtr =
          dynamic_cast<Dispatcher::InternalMethodMETA *>(methodMETA);
      const ast::Method *method = castedPtr->getMethodAST ();
      rhsValue = interpret (method, rhsOperands.size (), argumentVector);
    }
  if (methodMETA->getMethodMETAClass () == MethodMETAClass::External)
    {
      ALLOC_NODE (rhsValue);
      auto castedPtr =
          dynamic_cast<Dispatcher::ExternalMethodMETA *>(methodMETA);
      castedPtr->invoke (rhsValue, rhsOperands.size (), argumentVector);
    }
  c.writeLocalVariable (s->getLHSIdentifier (), rhsValue);
  c.pc = statementFlow.getOutwardFlowOnTrue (s);
}

void Interpreter::interpret (const ast::LabelStatement *s, InterpretContext &c)
{
  assert (s != nullptr);
  c.pc = statementFlow.getOutwardFlowOnTrue (s);
}

void Interpreter::interpret (const ast::PhiStatement *s, InterpretContext &c)
{
  assert (s != nullptr);
  throw NOT_YET_IMPLEMENTED;
}

V Interpreter::fetchOperand (const ast::Operand *operand, InterpretContext &c)
{
  assert (operand != nullptr);
  if (operand->getOperandClass () == ast::Operand::OperandClass::Identifier)
    {
      auto castedPtr = dynamic_cast<const ast::Identifier *>(operand);
      return c.readLocalVariable (castedPtr);
    }
  assert (operand->getOperandClass () == ast::Operand::OperandClass::Literal);
  auto castedPtr = dynamic_cast<const ast::Literal *>(operand);
  return LiteralConverter::convert (castedPtr);
}