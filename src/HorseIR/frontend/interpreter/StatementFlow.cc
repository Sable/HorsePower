#include "StatementFlow.h"

using namespace horseIR;
using namespace horseIR::interpreter;

StatementFlow::StatementFlow (const ast::CompilationUnit *compilationUnit)
{
  assert (compilationUnit != nullptr);
  analysis (compilationUnit);
}

const ast::Statement *
StatementFlow::getOutwardFlowOnTrue (const ast::Statement *statement) const
{ return outwardFlowMap.at (statement).first; }

const ast::Statement *
StatementFlow::getOutwardFlowOnFalse (const ast::Statement *statement) const
{ return outwardFlowMap.at (statement).second; }

const StatementFlow::OutwardFlowType
StatementFlow::getOutwardFlow (const ast::Statement *statement) const
{ return outwardFlowMap.at (statement); }

const StatementFlow::InwardFlowType
StatementFlow::getInwardFlow (const ast::Statement *statement) const
{ return inwardFlowMap.at (statement); }

StatementFlow::InwardFlowType::iterator
StatementFlow::inwardFlowBegin (const ast::Statement *statement)
{ return inwardFlowMap.at (statement).begin (); }

StatementFlow::InwardFlowType::iterator
StatementFlow::inwardFlowEnd (const ast::Statement *statement)
{ return inwardFlowMap.at (statement).end (); }

StatementFlow::InwardFlowType::const_iterator
StatementFlow::inwardFlowConstBegin (const ast::Statement *statement) const
{ return inwardFlowMap.at (statement).cbegin (); }

StatementFlow::InwardFlowType::const_iterator
StatementFlow::inwardFlowConstEnd (const ast::Statement *statement) const
{ return inwardFlowMap.at (statement).cend (); }

#ifndef NDEBUG

StatementFlow::StatementFlowPrinter
StatementFlow::getPrinter (std::ostream &stream) const
{ return StatementFlowPrinter (stream, *this); }

#endif

void StatementFlow::analysis (const ast::CompilationUnit *pCompilationUnit)
{
  assert (pCompilationUnit != nullptr);
  compilationUnit = pCompilationUnit;
  std::for_each (
      compilationUnit->modulesConstBegin (),
      compilationUnit->modulesConstEnd (),
      [this] (const ast::CompilationUnit::ModuleConstIterator::value_type &v)
      { return analysis (v); });
}

void StatementFlow::analysis (const ast::Module *module)
{
  assert (module != nullptr);
  std::for_each (
      module->methodsConstBegin (), module->methodsConstEnd (),
      [this] (const ast::Module::MethodConstIterator::value_type &value) -> void
      { return analysis (value); });
}

void StatementFlow::analysis (const ast::Method *method)
{
  assert (method != nullptr);
  std::unordered_map<std::string, const ast::Statement *> labelMap;
  for (auto statementIter = method->statementsConstBegin ();
       statementIter != method->statementsConstEnd (); ++statementIter)
    {
      const ast::Statement *statement = *statementIter;
      using StatementClass = ast::Statement::StatementClass;
      if (statement->getStatementClass () == StatementClass::Label)
        {
          auto castedPtr = dynamic_cast<const ast::LabelStatement *>(statement);
          labelMap[castedPtr->getLabelName ()] = castedPtr;
        }
      InwardFlowType inwardFlow;
      inwardFlowMap.emplace (
          std::make_pair (statement, std::move (inwardFlow))
      );
    }
  for (auto statementIter = method->statementsConstBegin ();
       statementIter != method->statementsConstEnd (); ++statementIter)
    {
      const ast::Statement *stmt = *statementIter;
      const ast::Statement *nextStmt =
          (std::next (statementIter) == method->statementsConstEnd ()) ?
          nullptr : (*std::next (statementIter));
      using StatementClass = ast::Statement::StatementClass;
      const auto statementClass = stmt->getStatementClass ();
      OutwardFlowType outwardFlow (nullptr, nullptr);
      if (statementClass == StatementClass::Label)
        { outwardFlow = std::make_pair (nextStmt, nextStmt); }
      if (statementClass == StatementClass::Return)
        { outwardFlow = std::make_pair (nullptr, nullptr); }
      if (statementClass == StatementClass::Phi)
        { outwardFlow = std::make_pair (nextStmt, nextStmt); }
      if (statementClass == StatementClass::Invoke)
        { outwardFlow = std::make_pair (nextStmt, nextStmt); }
      if (statementClass == StatementClass::Branch)
        {
          auto castedPtr = dynamic_cast<const ast::BranchStatement *>(stmt);
          const ast::Statement *target =
              labelMap[castedPtr->getTargetLabelName ()];
          if (castedPtr->isConditional ())
            { outwardFlow = std::make_pair (target, nextStmt); }
          else
            { outwardFlow = std::make_pair (target, target); }
        }
      if (statementClass == StatementClass::Assign)
        { outwardFlow = std::make_pair (nextStmt, nextStmt); }

      outwardFlowMap.emplace (std::make_pair (stmt, outwardFlow));
      if (outwardFlow.first != nullptr)
        { inwardFlowMap[outwardFlow.first].emplace (stmt); }
      if (outwardFlow.second != nullptr)
        { inwardFlowMap[outwardFlow.second].emplace (stmt); }
    }
}

#ifndef NDEBUG

void StatementFlow::StatementFlowPrinter::print ()
{ ast::ASTPrinter::print (statementFlow.compilationUnit); }

StatementFlow
::StatementFlowPrinter::StatementFlowPrinter (std::ostream &stream,
                                              const StatementFlow &flow)
    : ast::ASTPrinter (stream), statementFlow (flow)
{
  const ast::CompilationUnit *compilationUnit = flow.compilationUnit;
  for (auto moduleIter = compilationUnit->modulesConstBegin ();
       moduleIter != compilationUnit->modulesConstEnd (); ++moduleIter)
    {
      const ast::Module *module = *moduleIter;
      for (auto methodIter = module->methodsConstBegin ();
           methodIter != module->methodsConstEnd (); ++methodIter)
        {
          const ast::Method *method = *methodIter;
          std::size_t counter = 0;
          for (auto statementIter = method->statementsConstBegin ();
               statementIter != method->statementsConstEnd (); ++statementIter)
            {
              const ast::Statement *statement = *statementIter;
              stmtNumberMap.emplace (std::make_pair (statement, counter));
              counter = counter + 1;
            }
        }
    }
}

void StatementFlow::StatementFlowPrinter
::caseAssignStatement (AssignStmtPtr stmt, std::size_t indent)
{ return printStatementWithWrapper (stmt, indent); }

void StatementFlow::StatementFlowPrinter
::caseBranchStatement (BranchStmtPtr stmt, std::size_t indent)
{ return printStatementWithWrapper (stmt, indent); }

void StatementFlow::StatementFlowPrinter
::caseInvokeStatement (InvokeStmtPtr stmt, std::size_t indent)
{ return printStatementWithWrapper (stmt, indent); }

void StatementFlow::StatementFlowPrinter
::caseLabelStatement (LabelStmtPtr stmt, std::size_t indent)
{ return printStatementWithWrapper (stmt, indent); }

void StatementFlow::StatementFlowPrinter
::casePhiStatement (PhiStmtPtr stmt, std::size_t indent)
{ return printStatementWithWrapper (stmt, indent); }

void StatementFlow::StatementFlowPrinter
::caseReturnStatement (ReturnStmtPtr stmt, std::size_t indent)
{ return printStatementWithWrapper (stmt, indent); }

void StatementFlow::StatementFlowPrinter
::printStatementWithWrapper (const ast::Statement *stmt, size_t indent)
{
  assert (stmt != nullptr);
  for (std::size_t iter = 0; iter < indent; ++iter)
    { this->stream << this->INDENT; }
  std::ostringstream stream;
  ast::ASTPrinter printer (stream);
  printer.print (stmt);
  this->stream << "/* [" << stmtNumberMap[stmt] << "] */"
               << this->INDENT
               << stream.str ()
               << this->INDENT << "// ";
  printOutwardFlow (stmt);
  this->stream << this->INDENT;
  printInwardFlow (stmt);
}

void StatementFlow::StatementFlowPrinter
::printOutwardFlow (const ast::Statement *statement)
{
  assert (statement != nullptr);
  OutwardFlowType outwardFlow (statementFlow.outwardFlowMap.at (statement));
  this->stream << "Outward:<";
  if (outwardFlow.first != nullptr)
    { this->stream << stmtNumberMap[outwardFlow.first]; }
  else
    { this->stream << "nullptr"; }
  this->stream << ", ";
  if (outwardFlow.second != nullptr)
    { this->stream << stmtNumberMap[outwardFlow.second]; }
  else
    { this->stream << "nullptr"; }
  this->stream << '>';
}

void StatementFlow::StatementFlowPrinter
::printInwardFlow (const ast::Statement *statement)
{
  assert (statement != nullptr);
  InwardFlowType inwardFlow (statementFlow.inwardFlowMap.at (statement));
  this->stream << "Inward:{";
  std::transform (
      inwardFlow.cbegin (), inwardFlow.cend (),
      misc::InfixOStreamIterator<std::size_t> (this->stream, ", "),
      [this] (const ast::Statement *inflowStatement) -> std::size_t
      { return stmtNumberMap[inflowStatement]; });
  this->stream << '}';
}

#endif