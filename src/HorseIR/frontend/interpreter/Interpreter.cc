#include "Interpreter.h"

using namespace horseIR::interpreter;

void Interpreter::createIdentifierNumbering (ast::CompilationUnit *unit)
{
  assert (unit != nullptr);
  std::for_each (
      unit->modulesConstBegin (), unit->modulesConstEnd (),
      [&] (const ast::CompilationUnit::ModuleConstIterator::value_type &value)
      { return createIdentifierNumbering (value); });
}

void Interpreter::createIdentifierNumbering (ast::Module *module)
{
  assert (module != nullptr);
  std::for_each (
      module->methodsConstBegin (), module->methodsConstEnd (),
      [&] (const ast::Module::MethodConstIterator::value_type &value) -> void
      { return createIdentifierNumbering (value); });
}

void Interpreter::createIdentifierNumbering (ast::Method *method)
{
  assert (method != nullptr);
  IdentifierNumberType number = 0;
  using AssignMapType = std::unordered_map<std::string, IdentifierNumberType>;
  AssignMapType assignMapType{};

  for (auto iter = method->parametersConstBegin ();
       iter != method->parametersConstEnd (); ++iter)
    {
      auto result = assignMapType.insert (
          std::make_pair ((*iter).first->getName (), number)
      );
      assert (result.second);
      idNumberingMap.insert (std::make_pair ((*iter).first, number));
      number = number + 1;
    }

  for (auto iter = method->statementsConstBegin ();
       iter != method->statementsConstEnd (); ++iter)
    {
      ast::Statement *statement = *iter;
      using StatementClass = ast::Statement::StatementClass;
      if (statement->getStatementClass () == StatementClass::Phi)
        {
          auto castedPtr = dynamic_cast<ast::PhiStatement *>(statement);
          ast::Identifier *lhsIdentifier = castedPtr->getLHSIdentifier ();
          auto result = assignMapType.insert (
              std::make_pair (lhsIdentifier->getName (), number)
          );
          assert (result.second);
          idNumberingMap.insert (std::make_pair (lhsIdentifier, number));
          number = number + 1;
        }
      if (statement->getStatementClass () == StatementClass::Assign)
        {
          auto castedPtr = dynamic_cast<ast::AssignStatement *>(statement);
          ast::Identifier *lhsIdentifier = castedPtr->getLHSIdentifier ();
          auto result = assignMapType.insert (
              std::make_pair (lhsIdentifier->getName (), number)
          );
          assert (result.second);
          idNumberingMap.insert (std::make_pair (lhsIdentifier, number));
          number = number + 1;
        }
      if (statement->getStatementClass () == StatementClass::Invoke)
        {
          auto castedPtr = dynamic_cast<ast::InvokeStatement *>(statement);
          ast::Identifier *lhsIdentifier = castedPtr->getLHSIdentifier ();
          auto result = assignMapType.insert (
              std::make_pair (lhsIdentifier->getName (), number)
          );
          assert (result.second);
          idNumberingMap.insert (std::make_pair (lhsIdentifier, number));
          number = number + 1;
        }
    }
  for (auto iter = method->statementsConstBegin ();
       iter != method->statementsConstEnd (); ++iter)
    {
      ast::Statement *statement = *iter;
      using StatementClass = ast::Statement::StatementClass;
      using OperandClass = ast::Operand::OperandClass;
      if (statement->getStatementClass () == StatementClass::Assign)
        {
          auto castedPtr = dynamic_cast<ast::AssignStatement *>(statement);
          ast::Operand *operand = castedPtr->getRHSOperand ();
          if (operand->getOperandClass () == OperandClass::Identifier)
            {
              auto id = dynamic_cast<ast::Identifier *>(operand);
              IdentifierNumberType n = assignMapType.at (id->getName ());
              idNumberingMap.insert (std::make_pair (id, n));
            }
        }
      if (statement->getStatementClass () == StatementClass::Branch)
        {
          auto castedPtr = dynamic_cast<ast::BranchStatement *>(statement);
          ast::Operand *operand = castedPtr->getOperand ();
          if (operand->getOperandClass () == OperandClass::Identifier)
            {
              auto id = dynamic_cast<ast::Identifier *>(operand);
              IdentifierNumberType n = assignMapType.at (id->getName ());
              idNumberingMap.insert (std::make_pair (id, n));
            }
        }
      if (statement->getStatementClass () == StatementClass::Invoke)
        {
          auto castedPtr = dynamic_cast<ast::InvokeStatement *>(statement);
          std::for_each (
              castedPtr->rhsOperandsConstBegin (),
              castedPtr->rhsOperandsConstEnd (),
              [&] (ast::Operand *operand) -> void
              {
                if (operand->getOperandClass () == OperandClass::Identifier)
                  {
                    auto id = dynamic_cast<ast::Identifier *>(operand);
                    IdentifierNumberType n = assignMapType.at (id->getName ());
                    idNumberingMap.insert (std::make_pair (id, n));
                  }
              });
        }
      if (statement->getStatementClass () == StatementClass::Label)
        {/* NO-OP */}
      if (statement->getStatementClass () == StatementClass::Phi)
        {
          auto castedPtr = dynamic_cast<ast::PhiStatement *>(statement);
          using ValueType = ast::PhiStatement::RHSMapConstIterator::value_type;
          std::for_each (
              castedPtr->rhsMapConstBegin (), castedPtr->rhsMapConstEnd (),
              [&] (const ValueType &entry) -> void
              {
                ast::Operand *operand = entry.second;
                if (operand->getOperandClass () == OperandClass::Identifier)
                  {
                    auto id = dynamic_cast<ast::Identifier *>(operand);
                    IdentifierNumberType n = assignMapType.at (id->getName ());
                    idNumberingMap.insert (std::make_pair (id, n));
                  }
              });
        }
      if (statement->getStatementClass () == StatementClass::Return)
        {
          auto castedPtr = dynamic_cast<ast::ReturnStatement *>(statement);
          ast::Operand *operand = castedPtr->getOperand ();
          if (operand->getOperandClass () == OperandClass::Identifier)
            {
              auto id = dynamic_cast<ast::Identifier *>(operand);
              IdentifierNumberType n = assignMapType.at (id->getName ());
              idNumberingMap.insert (std::make_pair (id, n));
            }
        }
    }
}