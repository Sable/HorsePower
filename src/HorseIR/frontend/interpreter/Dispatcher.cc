#include "Dispatcher.h"

using namespace horseIR::interpreter;

void Dispatcher::addMethodMETA (MethodMETA *methodMETA)
{
  assert (methodMETA != nullptr);
  using MethodMETAClass = MethodMETA::MethodMETAClass;
  // TODO: add ambiguity check here
  methodMETAs.emplace_back (methodMETA);
}

void
Dispatcher::collectInternalMethods (const ast::CompilationUnit *compilationUnit)
{
  assert (compilationUnit != nullptr);
  using ValueType = ast::CompilationUnit::ModuleConstIterator::value_type;
  std::for_each (
      compilationUnit->modulesConstBegin (),
      compilationUnit->modulesConstEnd (),
      [this] (const ValueType &module) -> void
      { collectInternalMethods (module); });
}

void Dispatcher::collectInternalMethods (const ast::Module *module)
{
  assert (module != nullptr);
  using ValueType = ast::Module::MethodConstIterator::value_type;
  const std::string moduleName = module->getModuleName ();
  std::for_each (
      module->methodsConstBegin (), module->methodsConstEnd (),
      [this, &moduleName] (const ValueType &method) -> void
      {
        auto internalMethodMETA = new InternalMethodMETA ();
        internalMethodMETA->setModuleName (moduleName);
        internalMethodMETA->setMethodName (method->getMethodName ());
        internalMethodMETA->setMethodAST (method);
        addMethodMETA (internalMethodMETA);
      });
}

void Dispatcher::analysis (const ast::CompilationUnit *compilationUnit)
{
  assert (compilationUnit != nullptr);
  using ValueType = ast::CompilationUnit::ModuleConstIterator::value_type;
  std::for_each (
      compilationUnit->modulesConstBegin (),
      compilationUnit->modulesConstEnd (),
      [this] (const ValueType &module) -> void
      { analysis (module); });
}

void Dispatcher::analysis (const ast::Module *module)
{
  assert (module != nullptr);
  const auto visibleMethodMETA = getVisibleMethodMETAs (module);
  using ValueType = ast::Module::MethodConstIterator::value_type;
  std::for_each (
      module->methodsConstBegin (), module->methodsConstEnd (),
      [this, &visibleMethodMETA] (const ValueType &method) -> void
      { analysis (method, visibleMethodMETA); });
}

#define CAST_APPLY(T, x, y) return analysis(dynamic_cast<const T*>(x), y)

void Dispatcher::analysis (const ast::Method *method,
                           const std::vector<MethodMETA *> &visibleMethodMETAs)
{
  assert (method != nullptr);
  using StatementClass = ast::Statement::StatementClass;
  for (auto statementIter = method->statementsConstBegin ();
       statementIter != method->statementsConstEnd (); ++statementIter)
    {
      const ast::Statement *statement = *statementIter;
      const auto statementClass = statement->getStatementClass ();
      if (statementClass == StatementClass::Assign)
        { CAST_APPLY (ast::AssignStatement, statement, visibleMethodMETAs); }
      if (statementClass == StatementClass::Branch)
        { CAST_APPLY (ast::BranchStatement, statement, visibleMethodMETAs); }
      if (statementClass == StatementClass::Invoke)
        { CAST_APPLY (ast::InvokeStatement, statement, visibleMethodMETAs); }
      if (statementClass == StatementClass::Label)
        { CAST_APPLY (ast::LabelStatement, statement, visibleMethodMETAs); }
      if (statementClass == StatementClass::Phi)
        { CAST_APPLY (ast::PhiStatement, statement, visibleMethodMETAs); }
      if (statementClass == StatementClass::Return)
        { CAST_APPLY (ast::ReturnStatement, statement, visibleMethodMETAs); }
    }
}

void Dispatcher::analysis (const ast::AssignStatement *assignStatement,
                           const std::vector<MethodMETA *> &visibleMethodMETAs)
{
  assert (assignStatement != nullptr);
}