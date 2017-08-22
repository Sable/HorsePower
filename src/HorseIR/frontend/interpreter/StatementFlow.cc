#include "StatementFlow.h"

using namespace horseIR::ast;
using namespace horseIR::interpreter;

using FlowEntry = StatementFlow::FlowEntry;

StatementFlow StatementFlow::construct (const CompilationUnit *unit)
{
  assert (unit != nullptr);
  StatementFlow statementFlow{};
  std::for_each (
      unit->modulesConstBegin (), unit->modulesConstEnd (),
      [&] (const CompilationUnit::ModuleConstIterator::value_type &v) -> void
      {
        for (auto methodIter = v->methodsConstBegin ();
             methodIter != v->methodsConstEnd (); ++methodIter)
          { statementFlow.constructOnMethod (*methodIter); }
      });
  return statementFlow;
}

FlowEntry StatementFlow::getFlowInfo (const Statement *statement) const
{ return flowMap.at (statement); }

std::set<const Statement *>
StatementFlow::getInwardFlow (const ast::Statement *statement) const
{ return flowMap.at (statement).inwardFlow; }

std::pair<const Statement *, const Statement *>
StatementFlow::getOutwardFlow (const ast::Statement *statement) const
{ return flowMap.at (statement).outwardFlow; }

const Statement *
StatementFlow::getOutwardFlowOnTrue (const Statement *statement) const
{ return flowMap.at (statement).outwardFlow.first; }

const Statement *
StatementFlow::getOutwardFlowOnFalse (const Statement *statement) const
{ return flowMap.at (statement).outwardFlow.second; }

void StatementFlow::constructOnMethod (const Method *method)
{
  assert (method != nullptr);
  std::map<std::string, const LabelStatement *> labelMap{};
  std::for_each (
      method->statementsConstBegin (), method->statementsConstEnd (),
      [&] (const Method::StatementConstIterator::value_type &value) -> void
      {
        if (value->getStatementClass () != Statement::StatementClass::Label)
          { return; }
        auto labelStmt = dynamic_cast<const LabelStatement *>(value);
        labelMap[labelStmt->getLabelName ()] = labelStmt;
      });
  std::for_each (
      method->statementsConstBegin (), method->statementsConstEnd (),
      [&] (const Method::StatementConstIterator::value_type &value) -> void
      {
        FlowEntry newFlowEntry{};
        newFlowEntry.outwardFlow.first = nullptr;
        newFlowEntry.outwardFlow.second = nullptr;
        flowMap.insert (std::make_pair (value, std::move (newFlowEntry)));
      });
  for (auto statementIter = method->statementsConstBegin ();
       statementIter != method->statementsConstEnd (); ++statementIter)
    {
      Statement *statement = *statementIter;
      Statement *nextStatement =
          (std::next (statementIter) == method->statementsConstEnd ()) ?
          nullptr : *std::next (statementIter);
      const auto statementClass = statement->getStatementClass ();
      if (statementClass == Statement::StatementClass::Label ||
          statementClass == Statement::StatementClass::Phi ||
          statementClass == Statement::StatementClass::Invoke ||
          statementClass == Statement::StatementClass::Assign)
        {
          flowMap[statement].outwardFlow.first = nextStatement;
          flowMap[statement].outwardFlow.second = nextStatement;
          if (nextStatement != nullptr)
            flowMap[nextStatement].inwardFlow.emplace (statement);
          continue;
        }
      if (statement->getStatementClass () == Statement::StatementClass::Branch)
        {
          auto castedStmt = static_cast<BranchStatement *>(statement);
          const std::string targetName = castedStmt->getTargetLabelName ();
          const Statement *targetStatement = labelMap.at (targetName);
          if (castedStmt->isConditional ())
            {
              flowMap[statement].outwardFlow.first = targetStatement;
              flowMap[statement].outwardFlow.second = nextStatement;
              flowMap[targetStatement].inwardFlow.emplace (statement);
              if (nextStatement != nullptr)
                flowMap[nextStatement].inwardFlow.emplace (statement);
            }
          else
            {
              flowMap[statement].outwardFlow.first = targetStatement;
              flowMap[statement].outwardFlow.second = targetStatement;
              flowMap[targetStatement].inwardFlow.emplace (statement);
            }
          continue;
        }
      if (statement->getStatementClass () == Statement::StatementClass::Return)
        {
          flowMap[statement].outwardFlow.first = nullptr;
          flowMap[statement].outwardFlow.second = nullptr;
          continue;
        }
    }
}