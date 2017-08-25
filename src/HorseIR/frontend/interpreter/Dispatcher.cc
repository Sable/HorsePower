#include "Dispatcher.h"

#define INVALID_CANDIDATE std::runtime_error("invalid overloading candidate")
#define NO_DISPATCH_TARGET std::runtime_error("cannot find a dispatch target")

using namespace horseIR::interpreter;

void Dispatcher::addExternalMethod (const std::string &moudleName,
                                    const std::string &methodName,
                                    V (*invokeFunction) (size_t, V *))
{
  auto methodMETA = new __ExternalMethodMETA (invokeFunction);
  methodMETA->setModuleName (moudleName);
  methodMETA->setMethodName (methodName);
  const auto pos = std::find_if (
      candidateMethods.cbegin (), candidateMethods.cend (),
      [&] (const std::unique_ptr<MethodMETA> &ptr) -> bool
      {
        return ptr->getMethodName () == methodMETA->getMethodName () &&
               ptr->getModuleName () == methodMETA->getModuleName ();
      });
  if (pos != candidateMethods.cend ())
    {
      delete methodMETA;
      throw INVALID_CANDIDATE;
    }
  candidateMethods.emplace_back (methodMETA);
}

std::vector<Dispatcher::MethodMETA *>
Dispatcher::collectVisibleMethods (ast::Module *module)
{
  assert (module != nullptr);
  std::vector<MethodMETA *> visibleMethods{};
  visibleMethods.reserve (candidateMethods.size ());
  for (const auto &methodMETAPtr : candidateMethods)
    {
      using ValueType = ast::Module::ImportedModuleConstIterator::value_type;
      bool visible = std::any_of (
          module->importedModulesConstBegin (),
          module->importedModulesConstEnd (),
          [&] (const ValueType &value) -> bool
          {
            const std::string moduleName = methodMETAPtr->getModuleName ();
            const std::string methodName = methodMETAPtr->getMethodName ();
            if (moduleName == value.first && value.second == "*") return true;
            return moduleName == value.first && methodName == value.second;
          });
      visible = visible ||
                (methodMETAPtr->getModuleName () == module->getModuleName ());
      if (visible) visibleMethods.push_back (methodMETAPtr.get ());
    }
  return visibleMethods;
}

void Dispatcher::addInternalMethod (ast::Module *module, ast::Method *method)
{
  assert (method != nullptr && module != nullptr);
  auto *methodMETA = new InternalMethodMETA ();
  methodMETA->setModuleName (module->getModuleName ());
  methodMETA->setMethodName (method->getMethodName ());
  methodMETA->setMethodAST (method);
  const auto pos = std::find_if (
      candidateMethods.cbegin (), candidateMethods.cend (),
      [&] (const std::unique_ptr<MethodMETA> &ptr) -> bool
      {
        return ptr->getMethodName () == methodMETA->getMethodName () &&
               ptr->getModuleName () == methodMETA->getModuleName ();
      });
  if (pos != candidateMethods.cend ())
    {
      delete methodMETA;
      throw INVALID_CANDIDATE;
    }
  candidateMethods.emplace_back (methodMETA);
}

void Dispatcher::analysis (ast::CompilationUnit *unit)
{
  assert (unit != nullptr);
  using ValueType = ast::CompilationUnit::ModuleConstIterator::value_type;
  std::for_each (
      unit->modulesConstBegin (), unit->modulesConstEnd (),
      [this] (const ValueType &value) -> void
      {
        for (auto methodIter = value->methodsConstBegin ();
             methodIter != value->methodsConstEnd (); ++methodIter)
          { addInternalMethod (value, *methodIter); }
      });
  std::for_each (
      unit->modulesConstBegin (), unit->modulesConstEnd (),
      [this] (const ValueType &value) -> void
      { analysis (value); });
}

void Dispatcher::analysis (ast::Module *module)
{
  assert (module != nullptr);
  const auto visibleMethods = collectVisibleMethods (module);
  using ValueType = ast::Module::MethodConstIterator::value_type;
  std::for_each (
      module->methodsConstBegin (), module->methodsConstEnd (),
      [&] (const ValueType &value) -> void
      { analysis (value, visibleMethods); });
}

#define CAST_ANALYSIS(T, x) analysis(dynamic_cast<T*>(x), candidates)

void Dispatcher::analysis (ast::Method *method,
                           const std::vector<MethodMETA *> &candidates)
{
  assert (method != nullptr);
  using StatementClass = ast::Statement::StatementClass;
  for (auto statementIter = method->statementsConstBegin ();
       statementIter != method->statementsConstEnd (); ++statementIter)
    {
      ast::Statement *stmt = *statementIter;
      const auto statementClass = stmt->getStatementClass ();
      if (statementClass == StatementClass::Invoke)
        { CAST_ANALYSIS (ast::InvokeStatement, stmt); }
      if (statementClass == StatementClass::Assign)
        { CAST_ANALYSIS (ast::AssignStatement, stmt); }
      if (statementClass == StatementClass::Phi)
        { CAST_ANALYSIS (ast::PhiStatement, stmt); }
      if (statementClass == StatementClass::Return)
        { CAST_ANALYSIS (ast::ReturnStatement, stmt); }
      if (statementClass == StatementClass::Branch)
        { CAST_ANALYSIS (ast::BranchStatement, stmt); }
      if (statementClass == StatementClass::Label)
        { /* NO-OP */ }
    }
}

void Dispatcher::analysis (ast::InvokeStatement *invokeStatement,
                           const std::vector<MethodMETA *> &candidates)
{
  assert (invokeStatement != nullptr);
  const std::string targetModuleName = invokeStatement->getTargetModuleName ();
  const std::string targetMethodName = invokeStatement->getTargetMethodName ();
  MethodMETA *methodMETA = nullptr;
  if (!targetModuleName.empty ())
    {
      const auto pos = std::find_if (
          candidates.cbegin (), candidates.cend (),
          [&] (MethodMETA *searchMethodMETA) -> bool
          {
            return searchMethodMETA->getModuleName () == targetModuleName &&
                   searchMethodMETA->getMethodName () == targetMethodName;
          });
      if (pos == candidates.cend ()) throw NO_DISPATCH_TARGET;
      methodMETA = *pos;
    }
  else
    {
      const auto pos = std::find_if (
          candidates.cbegin (), candidates.cend (),
          [&] (MethodMETA *searchMethodMETA) -> bool
          { return searchMethodMETA->getMethodName () == targetMethodName; });
      if (pos == candidates.cend ()) throw NO_DISPATCH_TARGET;
      const auto duplicatePos = std::find_if (
          std::next (pos), candidates.cend (),
          [&] (MethodMETA *searchMethodMETA) -> bool
          { return searchMethodMETA->getMethodName () == targetMethodName; });
      if (duplicatePos != candidates.cend ()) throw NO_DISPATCH_TARGET;
      methodMETA = *pos;
    }
  invokeStatementMap.insert (std::make_pair (invokeStatement, methodMETA));
  for (auto operandIter = invokeStatement->rhsOperandsConstBegin ();
       operandIter != invokeStatement->rhsOperandsConstEnd (); ++operandIter)
    {
      ast::Operand *operand = *operandIter;
      if (operand->getOperandClass () == ast::Operand::OperandClass::Literal)
        {
          using LiteralClass = ast::Literal::LiteralClass;
          auto literal = dynamic_cast<ast::Literal *>(operand);
          if (literal->getLiteralClass () != LiteralClass::Function) continue;
          auto function = dynamic_cast<ast::FunctionLiteral *>(literal);
          analysis (function, candidates);
        }
    }
}

void Dispatcher::analysis (ast::AssignStatement *assignStatement,
                           const std::vector<MethodMETA *> &candidates)
{
  assert (assignStatement != nullptr);
  ast::Operand *rhsOperand = assignStatement->getRHSOperand ();
  if (rhsOperand->getOperandClass () == ast::Operand::OperandClass::Literal)
    {
      using LiteralClass = ast::Literal::LiteralClass;
      auto literal = dynamic_cast<ast::Literal *>(rhsOperand);
      if (literal->getLiteralClass () != LiteralClass::Function) return;
      auto function = dynamic_cast<ast::FunctionLiteral *>(literal);
      analysis (function, candidates);
    }
}

void Dispatcher::analysis (ast::BranchStatement *branchStatement,
                           const std::vector<MethodMETA *> &candidates)
{
  assert (branchStatement != nullptr);
  ast::Operand *operand = branchStatement->getOperand ();
  if (operand->getOperandClass () == ast::Operand::OperandClass::Literal)
    {
      using LiteralClass = ast::Literal::LiteralClass;
      auto literal = dynamic_cast<ast::Literal *>(operand);
      if (literal->getLiteralClass () != LiteralClass::Function) return;
      auto function = dynamic_cast<ast::FunctionLiteral *>(literal);
      analysis (function, candidates);
    }
}

void Dispatcher::analysis (ast::ReturnStatement *returnStatement,
                           const std::vector<MethodMETA *> &candidates)
{
  assert (returnStatement != nullptr);
  ast::Operand *operand = returnStatement->getOperand ();
  if (operand->getOperandClass () == ast::Operand::OperandClass::Literal)
    {
      using LiteralClass = ast::Literal::LiteralClass;
      auto literal = dynamic_cast<ast::Literal *>(operand);
      if (literal->getLiteralClass () != LiteralClass::Function) return;
      auto function = dynamic_cast<ast::FunctionLiteral *>(literal);
      analysis (function, candidates);
    }
}

void Dispatcher::analysis (ast::PhiStatement *phiStatement,
                           const std::vector<MethodMETA *> &candidates)
{
  assert (phiStatement != nullptr);
  for (auto iter = phiStatement->rhsMapConstBegin ();
       iter != phiStatement->rhsMapConstEnd (); ++iter)
    {
      using ValueType = ast::PhiStatement::RHSMapConstIterator::value_type;
      const ValueType &value = *iter;
      ast::Operand *operand = value.second;
      if (operand->getOperandClass () == ast::Operand::OperandClass::Literal)
        {
          using LiteralClass = ast::Literal::LiteralClass;
          auto literal = dynamic_cast<ast::Literal *>(operand);
          if (literal->getLiteralClass () != LiteralClass::Function) continue;
          auto function = dynamic_cast<ast::FunctionLiteral *>(literal);
          analysis (function, candidates);
        }
    }
}

void Dispatcher::analysis (ast::FunctionLiteral *functionLiteral,
                           const std::vector<MethodMETA *> &candidates)
{
  assert (functionLiteral != nullptr);
  const auto elements = functionLiteral->getValue ();
  std::vector<MethodMETA *> dispatchMap{};
  dispatchMap.reserve (elements.size ());
  std::transform (
      elements.cbegin (), elements.cend (),
      std::back_inserter (dispatchMap),
      [&] (const ast::FunctionLiteral::ElementType &element) -> MethodMETA *
      {
        if (element.isNull ()) return nullptr;
        const std::string targetModule = element.getValue ().moduleName;
        const std::string targetMethod = element.getValue ().methodName;
        MethodMETA *methodMETA = nullptr;
        if (!targetModule.empty ())
          {
            const auto pos = std::find_if (
                candidates.cbegin (), candidates.cend (),
                [&] (MethodMETA *searchMethodMETA) -> bool
                {
                  return searchMethodMETA->getModuleName () == targetModule &&
                         searchMethodMETA->getMethodName () == targetMethod;
                });
            if (pos == candidates.cend ()) throw NO_DISPATCH_TARGET;
            methodMETA = *pos;
          }
        else
          {
            const auto pos = std::find_if (
                candidates.cbegin (), candidates.cend (),
                [&] (MethodMETA *searchMethodMETA) -> bool
                { return searchMethodMETA->getMethodName () == targetMethod; });
            if (pos == candidates.cend ()) throw NO_DISPATCH_TARGET;
            const auto duplicatePos = std::find_if (
                std::next (pos), candidates.cend (),
                [&] (MethodMETA *searchMethodMETA) -> bool
                { return searchMethodMETA->getMethodName () == targetMethod; });
            if (duplicatePos != candidates.cend ()) throw NO_DISPATCH_TARGET;
            methodMETA = *pos;
          }
        return methodMETA;
      });
  functionLiteralMap.insert (
      std::make_pair (functionLiteral, std::move (dispatchMap))
  );
}