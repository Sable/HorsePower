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

#include <stdexcept>
#include <h_system.h>
#define HORSEIR_BUILTIN_ARG1(func, registerName)                               \
{                                                                              \
  addExternalMethodMETA("Builtin", registerName,                               \
                        [](V ret, std::size_t argc, V argv[])                  \
                        {                                                      \
                          L opCode = func(ret, argv[0]);                       \
                          if (opCode != 0)                                     \
                          {                                                    \
                            printErrMsg(opCode);                               \
                            throw std::runtime_error("runtime exception");     \
                          }                                                    \
                        }, (void*) func);                                      \
}
#define HORSEIR_BUILTIN_ARG2(func, registerName)                                             \
{                                                                              \
  addExternalMethodMETA("Builtin", registerName,                               \
                        [](V ret, std::size_t argc, V argv[])                  \
                        {                                                      \
                          L opCode = func(ret, argv[0], argv[1]);              \
                          if (opCode != 0)                                     \
                          {                                                    \
                            printErrMsg(opCode);                               \
                            throw std::runtime_error("runtime exception");     \
                          }                                                    \
                        }, (void*) func);                                      \
}
#define HORSEIR_BUILTIN_ARG3(func, registerName)                                             \
{                                                                              \
  addExternalMethodMETA("Builtin", registerName,                               \
                        [](V ret, std::size_t argc, V argv[])                  \
                        {                                                      \
                          L opCode = func(ret, argv[0], argv[1], argv[2]);     \
                          if (opCode != 0)                                     \
                          {                                                    \
                            printErrMsg(opCode);                               \
                            throw std::runtime_error("runtime exception");     \
                          }                                                    \
                        }, (void*) func);                                      \
}

void Dispatcher::registerExternalMethods ()
{
  HORSEIR_BUILTIN_ARG1 (pfnLoadTable, "load_table");
  addExternalMethodMETA ("Builtin", "list",
                         [] (V ret, std::size_t argc, V argv[])
                         {
                             L opCode = pfnList (ret, (L) argc, argv);
                             if (opCode != 0)
                               {
                                 printErrMsg (opCode);
                                 throw std::runtime_error ("runtime exception");
                               }
                         }, (void *) pfnList);
  HORSEIR_BUILTIN_ARG2 (pfnIndex, "index");
  addExternalMethodMETA ("Buitlin", "index_assign",
                         [] (V ret, std::size_t argc, V argv[])
                         {
                             L opCode = pfnIndexA (argv[0], argv[1], argv[2]);
                             if (opCode != 0)
                               {
                                 printErrMsg (opCode);
                                 throw std::runtime_error ("runtime exception");
                               }
                         }, (void *) pfnIndexA);
  HORSEIR_BUILTIN_ARG1 (pfnFlip, "flip");
  HORSEIR_BUILTIN_ARG2 (pfnMatch, "match");
  HORSEIR_BUILTIN_ARG1 (pfnMeta, "meta");
  HORSEIR_BUILTIN_ARG1 (pfnKeys, "keys");
  HORSEIR_BUILTIN_ARG1 (pfnValues, "values");
  HORSEIR_BUILTIN_ARG1 (pfnFetch, "fetch");
  HORSEIR_BUILTIN_ARG2 (pfnColumnValue, "column_value");

  HORSEIR_BUILTIN_ARG1 (pfnAbs, "abs");
  HORSEIR_BUILTIN_ARG1 (pfnNeg, "neg");
  HORSEIR_BUILTIN_ARG1 (pfnCeil, "ceil");
  HORSEIR_BUILTIN_ARG1 (pfnFloor, "floor");
  HORSEIR_BUILTIN_ARG1 (pfnRound, "round");

  HORSEIR_BUILTIN_ARG1 (pfnTrigSin, "sin");
  HORSEIR_BUILTIN_ARG1 (pfnTrigAsin, "asin");
  HORSEIR_BUILTIN_ARG1 (pfnTrigCos, "cos");
  HORSEIR_BUILTIN_ARG1 (pfnTrigAcos, "acos");
  HORSEIR_BUILTIN_ARG1 (pfnTrigTan, "tan");
  HORSEIR_BUILTIN_ARG1 (pfnTrigAtan, "atan");

  HORSEIR_BUILTIN_ARG1 (pfnHyperSinh, "sinh");
  HORSEIR_BUILTIN_ARG1 (pfnHyperAsinh, "asinh");
  HORSEIR_BUILTIN_ARG1 (pfnHyperCosh, "cosh");
  HORSEIR_BUILTIN_ARG1 (pfnHyperAcosh, "acosh");
  HORSEIR_BUILTIN_ARG1 (pfnHyperTanh, "tanh");
  HORSEIR_BUILTIN_ARG1 (pfnHyperAtanh, "atanh");

  HORSEIR_BUILTIN_ARG1 (pfnConj, "conj");
  HORSEIR_BUILTIN_ARG1 (pfnRecip, "recip");
  HORSEIR_BUILTIN_ARG1 (pfnSignum, "signum");
  HORSEIR_BUILTIN_ARG1 (pfnPi, "pi");
  HORSEIR_BUILTIN_ARG1 (pfnNot, "not");
  HORSEIR_BUILTIN_ARG1 (pfnExp, "exp");
  HORSEIR_BUILTIN_ARG1 (pfnLog, "log");
  HORSEIR_BUILTIN_ARG1 (pfnLen, "len");
  HORSEIR_BUILTIN_ARG1 (pfnRange, "range");
  HORSEIR_BUILTIN_ARG1 (pfnFact, "fact");
  HORSEIR_BUILTIN_ARG1 (pfnReverse, "reverse");
  HORSEIR_BUILTIN_ARG1 (pfnUnique, "unique");
  HORSEIR_BUILTIN_ARG1 (pfnWhere, "where");
  HORSEIR_BUILTIN_ARG1 (pfnSum, "sum");
  HORSEIR_BUILTIN_ARG1 (pfnAvg, "avg");

  HORSEIR_BUILTIN_ARG1 (pfnMin, "min");
  HORSEIR_BUILTIN_ARG1 (pfnMax, "max");

  HORSEIR_BUILTIN_ARG1 (pfnDateYear, "year");
  HORSEIR_BUILTIN_ARG1 (pfnDateMonth, "month");
  HORSEIR_BUILTIN_ARG1 (pfnDateDay, "day");
  HORSEIR_BUILTIN_ARG1 (pfnDate, "date");

  HORSEIR_BUILTIN_ARG1 (pfnTimeHour, "hour");
  HORSEIR_BUILTIN_ARG1 (pfnTimeMinute, "minute");
  HORSEIR_BUILTIN_ARG1 (pfnTimeSecond, "second");
  HORSEIR_BUILTIN_ARG1 (pfnTimeMill, "mill");
  HORSEIR_BUILTIN_ARG1 (pfnTime, "time");

  HORSEIR_BUILTIN_ARG1 (pfnEnlist, "enlist");
  HORSEIR_BUILTIN_ARG1 (pfnRaze, "raze");
  HORSEIR_BUILTIN_ARG1 (pfnToList, "to_list");
  HORSEIR_BUILTIN_ARG1 (pfnToIndex, "to_index");
  HORSEIR_BUILTIN_ARG1 (pfnGroup, "group");
  HORSEIR_BUILTIN_ARG1 (pfnGroupBucket, "group_bucket"); // FIXME

  HORSEIR_BUILTIN_ARG2 (pfnLt, "lt");
  HORSEIR_BUILTIN_ARG2 (pfnLeq, "leq");
  HORSEIR_BUILTIN_ARG2 (pfnGt, "gt");
  HORSEIR_BUILTIN_ARG2 (pfnGeq, "geq");
  HORSEIR_BUILTIN_ARG2 (pfnEq, "eq");
  HORSEIR_BUILTIN_ARG2 (pfnNeq, "neq");

  HORSEIR_BUILTIN_ARG2 (pfnPlus, "plus");
  HORSEIR_BUILTIN_ARG2 (pfnMinus, "minus");
  HORSEIR_BUILTIN_ARG2 (pfnMul, "mul");
  HORSEIR_BUILTIN_ARG2 (pfnDiv, "div");

  HORSEIR_BUILTIN_ARG2 (pfnAnd, "and");
  HORSEIR_BUILTIN_ARG2 (pfnNand, "nand");
  HORSEIR_BUILTIN_ARG2 (pfnOr, "or");
  HORSEIR_BUILTIN_ARG2 (pfnNor, "nor");
  HORSEIR_BUILTIN_ARG2 (pfnXor, "xor");

  HORSEIR_BUILTIN_ARG2 (pfnPower, "power");
  HORSEIR_BUILTIN_ARG2 (pfnLog2, "log2");
  HORSEIR_BUILTIN_ARG2 (pfnMod, "mod");
  HORSEIR_BUILTIN_ARG2 (pfnCompress, "compress");

  HORSEIR_BUILTIN_ARG2 (pfnIndexOf, "index_of");
  HORSEIR_BUILTIN_ARG2 (pfnAppend, "append");
  HORSEIR_BUILTIN_ARG2 (pfnLike, "like");
  HORSEIR_BUILTIN_ARG2 (pfnOrderBy, "order");

  addExternalMethodMETA ("Builtin", "each",
                         [] (V ret, std::size_t argc, V argv[])
                         {
                             L opCode = pfnEach (ret, argv[0],
                                                 (L(*) (V, V)) argv[1]);
                             if (opCode != 0)
                               {
                                 printErrMsg (opCode);
                                 throw std::runtime_error ("runtime exception");
                               }
                         }, (void *) pfnEach);
  addExternalMethodMETA ("Builtin", "each_item",
                         [] (V ret, std::size_t argc, V argv[])
                         {
                             L opCode = pfnEachItem (ret, argv[0], argv[1],
                                                     (L(*) (V, V, V)) argv[2]);
                             if (opCode != 0)
                               {
                                 printErrMsg (opCode);
                                 throw std::runtime_error ("runtime exception");
                               }
                         }, (void *) pfnEachItem);
  addExternalMethodMETA ("Builtin", "each_left",
                         [] (V ret, std::size_t argc, V argv[])
                         {
                             L opCode = pfnEachLeft (ret, argv[0], argv[1],
                                                     (L(*) (V, V, V)) argv[2]);
                             if (opCode != 0)
                               {
                                 printErrMsg (opCode);
                                 throw std::runtime_error ("runtime exception");
                               }
                         }, (void *) pfnEachLeft);
  addExternalMethodMETA ("Builtin", "each_right",
                         [] (V ret, std::size_t argc, V argv[])
                         {
                             L opCode = pfnEachRight (ret, argv[0], argv[1],
                                                      (L(*) (V, V, V)) argv[2]);
                             if (opCode != 0)
                               {
                                 printErrMsg (opCode);
                                 throw std::runtime_error ("runtime exception");
                               }
                         }, (void *) pfnEachRight);

  HORSEIR_BUILTIN_ARG2 (pfnDict, "dict");
  HORSEIR_BUILTIN_ARG2 (pfnTable, "table");
  HORSEIR_BUILTIN_ARG2 (pfnEnum, "enum");
  HORSEIR_BUILTIN_ARG2 (pfnKTable, "ktable");

  HORSEIR_BUILTIN_ARG2 (pfnMember, "member");
  HORSEIR_BUILTIN_ARG2 (pfnVector, "vector");

  HORSEIR_BUILTIN_ARG3 (pfnDatetimeAdd, "datetime_add");
  HORSEIR_BUILTIN_ARG3 (pfnDatetimeSub, "datetime_sub");
  HORSEIR_BUILTIN_ARG3 (pfnBetween, "between");
  HORSEIR_BUILTIN_ARG3 (pfnAddFKey, "add_fkey");
  HORSEIR_BUILTIN_ARG2 (pfnSubString, "sub_string");
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
    void (*funcPtr) (V, size_t, V *),
    void *rawPtr
)
{
  auto methodMETA = new __ExternalMethodMETA ();
  methodMETA->setModuleName (moduleName);
  methodMETA->setMethodName (methodName);
  methodMETA->setInvokeTarget (funcPtr, rawPtr);
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
      else if (!methodMETA->getMoudleName ().empty ())
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
                  return importedEntry.first == candidateMETA->getMoudleName ()
                         &&
                         importedEntry.second
                         == candidateMETA->getMethodName ();
              });
          if (visible)
            { visibleMethodMETAs.push_back (methodMETA.get ()); }
        }
    }
  return visibleMethodMETAs;
}
