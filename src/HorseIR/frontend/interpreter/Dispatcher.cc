#include "Dispatcher.h"

using namespace horseIR::interpreter;

Dispatcher::Dispatcher (const ast::CompilationUnit *pCompilationUnit)
    : compilationUnit (pCompilationUnit)
{
  assert (pCompilationUnit != nullptr);
  registerExternalMethods ();
  collectInternalMethods (compilationUnit);
  analysis (compilationUnit);
}

#ifndef NDEBUG

void Dispatcher::DispatcherPrinter::print ()
{ ast::ASTPrinter::print (dispatcher.compilationUnit); }

void Dispatcher::DispatcherPrinter
::caseInvokeStatement (InvokeStmtPtr stmt, std::size_t indent)
{
  assert (stmt != nullptr);
  ast::ASTPrinter astPrinter (this->stream);
  astPrinter.print (stmt, indent);
  this->stream << this->INDENT << "// =>"
               << dispatcher.invokeStatementMap.at (stmt)->getMoudleName ()
               << '.'
               << dispatcher.invokeStatementMap.at (stmt)->getMethodName ();
}

void Dispatcher::DispatcherPrinter
::caseFunctionLiteral (FunctionLiteralPtr literal)
{
  assert (literal != nullptr);
  ast::ASTPrinter astPrinter (this->stream);
  astPrinter.print (literal);
  const auto dispatchVector = dispatcher.functionLiteralMap.at (literal);
  this->stream << "/* => (";
  std::transform (
      dispatchVector.cbegin (), dispatchVector.cend (),
      misc::InfixOStreamIterator<std::string> (this->stream, ", "),
      [] (const MethodMETA *methodMETA) -> std::string
      {
        return methodMETA->getMoudleName () + "."
               + methodMETA->getMethodName ();
      });
  this->stream << ") */";
}

Dispatcher::DispatcherPrinter
Dispatcher::getPrinter (std::ostream &stream) const
{ return DispatcherPrinter (stream, *this); }

#endif

Dispatcher::MethodMETA *
Dispatcher::getMethodMETA (const ast::InvokeStatement *statement) const
{
  assert (statement != nullptr);
  return invokeStatementMap.at (statement);
}

std::vector<Dispatcher::MethodMETA *>
Dispatcher::getMethodMETA (const ast::FunctionLiteral *literal) const
{
  assert (literal != nullptr);
  return functionLiteralMap.at (literal);
}

void Dispatcher::registerExternalMethods ()
{
  addExternalMethodMETA ("Builtin", "len",
                         [] (V ret, std::size_t argc, V argv[])
                         { pfnLen (ret, argv[0]); });
  addExternalMethodMETA ("Builtin", "lt",
                         [] (V ret, std::size_t argc, V argv[])
                         { pfnLt (ret, argv[0], argv[1]); });
  addExternalMethodMETA ("Builtin", "range",
                         [] (V ret, std::size_t argc, V argv[])
                         { pfnRange (ret, argv[0]); });
  addExternalMethodMETA ("Builtin", "compress",
                         [] (V ret, std::size_t argc, V argv[])
                         { pfnCompress (ret, argv[0], argv[1]); });
  addExternalMethodMETA ("Builtin", "load_table",
                         [] (V ret, std::size_t argc, V argv[])
                         { pfnLoadTable (ret, argv[0]); });
  addExternalMethodMETA ("Builtin", "column_value",
                         [] (V ret, std::size_t argc, V argv[])
                         { pfnColumnValue (ret, argv[0], argv[1]); });
  addExternalMethodMETA ("Builtin", "index_of",
                         [] (V ret, std::size_t argc, V argv[])
                         { pfnIndexOf (ret, argv[0], argv[1]); });
  addExternalMethodMETA ("Builtin", "index",
                         [] (V ret, std::size_t argc, V argv[])
                         { pfnIndex (ret, argv[0], argv[1]); });
  addExternalMethodMETA ("Builtin", "tolist",
                         [] (V ret, std::size_t argc, V argv[])
                         { pfnTolist (ret, argv[0]); });
  addExternalMethodMETA ("Builtin", "list",
                         [] (V ret, std::size_t argc, V argv[])
                         { pfnList (ret, static_cast<L>(argc), argv); });
  addExternalMethodMETA ("Builtin", "table",
                         [] (V ret, std::size_t argc, V argv[])
                         { pfnTable (ret, argv[0], argv[1]); });
}

void Dispatcher::addMethodMETA (MethodMETA *methodMETA)
{
  assert (methodMETA != nullptr);
  using MethodMETAClass = MethodMETA::MethodMETAClass;
  // TODO: add ambiguity check here
  methodMETAs.emplace_back (methodMETA);
}

void Dispatcher::addExternalMethodMETA (
    const std::string &moduleName, const std::string &methodName,
    void (*funcPtr) (V, size_t, V *)
)
{
  auto methodMETA = new __ExternalMethodMETA ();
  methodMETA->setModuleName (moduleName);
  methodMETA->setMethodName (methodName);
  methodMETA->setInvokeTarget (funcPtr);
  addMethodMETA (methodMETA);
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

#define CAST_APPLY(T, x, y) analysis(dynamic_cast<const T*>(x), y)

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
  const ast::Operand *operand = assignStatement->getRHSOperand ();
  analysis (operand, visibleMethodMETAs);
}

void Dispatcher::analysis (const ast::BranchStatement *branchStatement,
                           const std::vector<MethodMETA *> &visibleMethodMETAs)
{
  assert (branchStatement != nullptr);
  if (!branchStatement->isConditional ()) return;
  const ast::Operand *operand = branchStatement->getOperand ();
  analysis (operand, visibleMethodMETAs);
}

void Dispatcher::analysis (const ast::InvokeStatement *invokeStatement,
                           const std::vector<MethodMETA *> &visibleMethodMETAs)
{
  assert (invokeStatement != nullptr);
  const std::string moduleName = invokeStatement->getTargetModuleName ();
  const std::string methodName = invokeStatement->getTargetMethodName ();
  MethodMETA *methodMETA = nullptr;
  if (moduleName.empty ())
    {
      const auto pos = std::find_if (
          visibleMethodMETAs.cbegin (), visibleMethodMETAs.cend (),
          [&methodName] (MethodMETA *searchMethodMETA) -> bool
          { return searchMethodMETA->getMethodName () == methodName; });
      assert (pos != visibleMethodMETAs.cend ());
#ifndef NDEBUG
      const auto alterPos = std::find_if (
          std::next (pos), visibleMethodMETAs.cend (),
          [&methodName] (MethodMETA *searchMethodMETA) -> bool
          { return searchMethodMETA->getMethodName () == methodName; });
#endif
      assert (alterPos == visibleMethodMETAs.cend ());
      methodMETA = *pos;
    }
  else
    {
      const auto pos = std::find_if (
          visibleMethodMETAs.cbegin (), visibleMethodMETAs.cend (),
          [&moduleName, &methodName] (MethodMETA *searchMethodMETA) -> bool
          {
            return searchMethodMETA->getMoudleName () == moduleName &&
                   searchMethodMETA->getMethodName () == methodName;
          });
      assert (pos != visibleMethodMETAs.cend ());
      methodMETA = *pos;
    }
  invokeStatementMap.emplace (std::make_pair (invokeStatement, methodMETA));
  std::for_each (
      invokeStatement->rhsOperandsConstBegin (),
      invokeStatement->rhsOperandsConstEnd (),
      [this, &visibleMethodMETAs] (const ast::Operand *operand) -> void
      { analysis (operand, visibleMethodMETAs); });
}

void Dispatcher::analysis (const ast::LabelStatement *labelStatement,
                           const std::vector<MethodMETA *> &visibleMethodMETAs)
{
  assert (labelStatement != nullptr);
  /* NO-OP */
}

void Dispatcher::analysis (const ast::PhiStatement *phiStatement,
                           const std::vector<MethodMETA *> &visibleMethodMETAs)
{
  assert (phiStatement != nullptr);
  using ValueType = ast::PhiStatement::RHSMapConstIterator::value_type;
  std::for_each (
      phiStatement->rhsMapConstBegin (), phiStatement->rhsMapConstEnd (),
      [this, &visibleMethodMETAs] (const ValueType &value) -> void
      { analysis (value.second, visibleMethodMETAs); });
}

void Dispatcher::analysis (const ast::ReturnStatement *returnStatement,
                           const std::vector<MethodMETA *> &visibleMethodMETAs)
{
  assert (returnStatement != nullptr);
  analysis (returnStatement->getOperand (), visibleMethodMETAs);
}

void Dispatcher::analysis (const ast::Operand *operand,
                           const std::vector<MethodMETA *> &visibleMethodMETAs)
{
  assert (operand != nullptr);
  using OperandClass = ast::Operand::OperandClass;
  using LiteralClass = ast::Literal::LiteralClass;
  if (operand->getOperandClass () != OperandClass::Literal) /* NO-OP */ return;
  auto literal = dynamic_cast<const ast::Literal *>(operand);
  if (literal->getLiteralClass () != LiteralClass::Function) /* NO-OP */ return;
  auto functionLiteral = dynamic_cast<const ast::FunctionLiteral *>(literal);
  analysis (functionLiteral, visibleMethodMETAs);
}

void Dispatcher::analysis (const ast::FunctionLiteral *functionLiteral,
                           const std::vector<MethodMETA *> &visibleMethodMETAs)
{
  assert (functionLiteral != nullptr);
  std::vector<MethodMETA *> dispatchMap;
  for (auto elementIter = functionLiteral->valueConstBegin ();
       elementIter != functionLiteral->valueConstEnd (); ++elementIter)
    {
      if (elementIter->isNull ())
        { dispatchMap.push_back (nullptr); }
      else
        {
          const std::string moduleName = elementIter->getValue ().moduleName;
          const std::string methodName = elementIter->getValue ().methodName;
          MethodMETA *methodMETA = nullptr;
          if (moduleName.empty ())
            {
              const auto pos = std::find_if (
                  visibleMethodMETAs.cbegin (), visibleMethodMETAs.cend (),
                  [&methodName] (MethodMETA *searchMethodMETA) -> bool
                  { return searchMethodMETA->getMethodName () == methodName; });
              assert (pos != visibleMethodMETAs.cend ());
#ifndef NDEBUG
              const auto alterPos = std::find_if (
                  std::next (pos), visibleMethodMETAs.cend (),
                  [&methodName] (MethodMETA *searchMethodMETA) -> bool
                  { return searchMethodMETA->getMethodName () == methodName; });
#endif
              assert (alterPos == visibleMethodMETAs.cend ());
              methodMETA = *pos;
            }
          else
            {
              const auto pos = std::find_if (
                  visibleMethodMETAs.cbegin (), visibleMethodMETAs.cend (),
                  [&moduleName, &methodName] (MethodMETA *searchMethodMETA)
                  {
                    return searchMethodMETA->getMoudleName () == moduleName &&
                           searchMethodMETA->getMethodName () == methodName;
                  });
              assert (pos != visibleMethodMETAs.cend ());
              methodMETA = *pos;
            }
          dispatchMap.push_back (methodMETA);
        }
    }
  functionLiteralMap.emplace (
      std::make_pair (functionLiteral, std::move (dispatchMap))
  );
}

std::vector<Dispatcher::MethodMETA *>
Dispatcher::getVisibleMethodMETAs (const ast::Module *module) const
{
  assert (module != nullptr);
  const std::string moduleName = module->getModuleName ();
  std::vector<MethodMETA *> visibleMethodMETAs;
  for (const auto &methodMETA : methodMETAs)
    {
      if (methodMETA->getMoudleName () == moduleName)
        { visibleMethodMETAs.push_back (methodMETA.get ()); }
      else
        {
          using VType = ast::Module::ImportedModuleConstIterator::value_type;
          const bool visible = std::any_of (
              module->importedModulesConstBegin (),
              module->importedModulesConstEnd (),
              [&methodMETA] (const VType &importedEntry) -> bool
              {
                MethodMETA *candidateMETA = methodMETA.get ();
                if (importedEntry.first == candidateMETA->getMoudleName () &&
                    importedEntry.second == "*")
                  { return true; }
                return importedEntry.first == candidateMETA->getMoudleName () &&
                       importedEntry.second == candidateMETA->getMethodName ();
              });
          if (visible)
            { visibleMethodMETAs.push_back (methodMETA.get ()); }
        }
    }
  return visibleMethodMETAs;
}