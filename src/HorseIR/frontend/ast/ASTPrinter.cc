#include "ASTPrinter.h"

#define APPLY(func, x, T, indent)                       \
  func(dynamic_cast<const T *>(x), indent);             \
  return
#define APPLY_NO_INDENT(func, x, T)                     \
  func(dynamic_cast<const T *>(x));                     \
  return

using namespace horseIR::ast;

ASTPrinter::ASTPrinter (std::ostream &stream)
    : stream (stream)
{}

void ASTPrinter::print (const ASTNode *astNode, std::size_t indent)
{
  if (astNode == nullptr)
    {
      for (std::size_t iter = 0; iter < indent; ++iter) stream << INDENT;
      stream << "nullptr";
      return;
    }
  switch (astNode->getASTNodeClass ())
    {
      case ASTNode::ASTNodeClass::CompilationUnit:
        { APPLY (caseCompilationUnit, astNode, CompilationUnit, indent); }
      case ASTNode::ASTNodeClass::Module:
        { APPLY (caseModule, astNode, Module, indent); }
      case ASTNode::ASTNodeClass::Method:
        { APPLY (caseMethod, astNode, Method, indent); }
      case ASTNode::ASTNodeClass::AssignStatement:
        { APPLY (caseAssignStatement, astNode, AssignStatement, indent); }
      case ASTNode::ASTNodeClass::BranchStatement:
        { APPLY (caseBranchStatement, astNode, BranchStatement, indent); }
      case ASTNode::ASTNodeClass::InvokeStatement:
        { APPLY (caseInvokeStatement, astNode, InvokeStatement, indent); }
      case ASTNode::ASTNodeClass::LabelStatement:
        { APPLY (caseLabelStatement, astNode, LabelStatement, indent); }
      case ASTNode::ASTNodeClass::PhiStatement:
        { APPLY (casePhiStatement, astNode, PhiStatement, indent); }
      case ASTNode::ASTNodeClass::ReturnStatement:
        { APPLY (caseReturnStatement, astNode, ReturnStatement, indent); }
      case ASTNode::ASTNodeClass::BoolLiteral:
        { APPLY_NO_INDENT (caseBoolLiteral, astNode, BoolLiteral); }
      case ASTNode::ASTNodeClass::CharLiteral:
        { APPLY_NO_INDENT (caseCharLiteral, astNode, CharLiteral); }
      case ASTNode::ASTNodeClass::ComplexLiteral:
        { APPLY_NO_INDENT (caseComplexLiteral, astNode, ComplexLiteral); }
      case ASTNode::ASTNodeClass::DateLiteral:
        { APPLY_NO_INDENT (caseDateLiteral, astNode, DateLiteral); }
      case ASTNode::ASTNodeClass::DateTimeLiteral:
        { APPLY_NO_INDENT (caseDateTimeLiteral, astNode, DateTimeLiteral); }
      case ASTNode::ASTNodeClass::DictionaryLiteral:
        { APPLY_NO_INDENT (caseDictionaryLiteral, astNode, DictionaryLiteral); }
      case ASTNode::ASTNodeClass::EnumerationLiteral:
        {
          APPLY_NO_INDENT (caseEnumerationLiteral, astNode, EnumerationLiteral);
        }
      case ASTNode::ASTNodeClass::FP32Literal:
        { APPLY_NO_INDENT (caseFP32Literal, astNode, FP32Literal); }
      case ASTNode::ASTNodeClass::FP64Literal:
        { APPLY_NO_INDENT (caseFP64Literal, astNode, FP64Literal); }
      case ASTNode::ASTNodeClass::FunctionLiteral:
        { APPLY_NO_INDENT (caseFunctionLiteral, astNode, FunctionLiteral); }
      case ASTNode::ASTNodeClass::Integer8Literal:
        { APPLY_NO_INDENT (caseInteger8Literal, astNode, Integer8Literal); }
      case ASTNode::ASTNodeClass::Integer16Literal:
        { APPLY_NO_INDENT (caseInteger16Literal, astNode, Integer16Literal); }
      case ASTNode::ASTNodeClass::Integer32Literal:
        { APPLY_NO_INDENT (caseInteger32Literal, astNode, Integer32Literal); }
      case ASTNode::ASTNodeClass::Integer64Literal:
        { APPLY_NO_INDENT (caseInteger64Literal, astNode, Integer64Literal); }
      case ASTNode::ASTNodeClass::KeyTableLiteral:
        { APPLY_NO_INDENT (caseKeyTableLiteral, astNode, KeyTableLiteral); }
      case ASTNode::ASTNodeClass::ListLiteral:
        { APPLY_NO_INDENT (caseListLiteral, astNode, ListLiteral); }
      case ASTNode::ASTNodeClass::MinuteLiteral:
        { APPLY_NO_INDENT (caseMinuteLiteral, astNode, MinuteLiteral); }
      case ASTNode::ASTNodeClass::MonthLiteral:
        { APPLY_NO_INDENT (caseMonthLiteral, astNode, MonthLiteral); }
      case ASTNode::ASTNodeClass::SecondLiteral:
        { APPLY_NO_INDENT (caseSecondLiteral, astNode, SecondLiteral); }
      case ASTNode::ASTNodeClass::StringLiteral:
        { APPLY_NO_INDENT (caseStringLiteral, astNode, StringLiteral); }
      case ASTNode::ASTNodeClass::SymbolLiteral:
        { APPLY_NO_INDENT (caseSymbolLiteral, astNode, SymbolLiteral); }
      case ASTNode::ASTNodeClass::TableLiteral:
        { APPLY_NO_INDENT (caseTableLiteral, astNode, TableLiteral); }
      case ASTNode::ASTNodeClass::TimeLiteral:
        { APPLY_NO_INDENT (caseTimeLiteral, astNode, TimeLiteral); }
      case ASTNode::ASTNodeClass::DictionaryType:
        { APPLY_NO_INDENT (caseDictionaryType, astNode, DictionaryType); }
      case ASTNode::ASTNodeClass::EnumerationType:
        { APPLY_NO_INDENT (caseEnumerationType, astNode, EnumerationType); }
      case ASTNode::ASTNodeClass::FunctionType:
        { APPLY_NO_INDENT (caseFunctionType, astNode, FunctionType); }
      case ASTNode::ASTNodeClass::ListType:
        { APPLY_NO_INDENT (caseListType, astNode, ListType); }
      case ASTNode::ASTNodeClass::PrimitiveType:
        { APPLY_NO_INDENT (casePrimitiveType, astNode, PrimitiveType); }
      case ASTNode::ASTNodeClass::WildcardType:
        { APPLY_NO_INDENT (caseWildcardType, astNode, WildcardType); }
      case ASTNode::ASTNodeClass::Identifier:
        { APPLY_NO_INDENT (caseIdentifier, astNode, Identifier); }
    }
}

void
ASTPrinter::caseCompilationUnit (const CompilationUnit *unit, size_t indent)
{
  assert (unit != nullptr);
  std::for_each (
      unit->modulesConstBegin (), unit->modulesConstEnd (),
      [&] (const Module *module) -> void
      {
        print (module, indent);
        stream << LINE_BREAK;
      });
}

void ASTPrinter::caseModule (const Module *module, size_t indent)
{
  assert (module != nullptr);
  for (std::size_t iter = 0; iter < indent; ++iter) stream << INDENT;
  stream << "module " << module->getModuleName () << " {" << LINE_BREAK;
  std::for_each (
      module->importedModulesConstBegin (),
      module->importedModulesConstEnd (),
      [&] (const Module::ImportedModuleConstIterator::value_type &value) -> void
      {
        for (std::size_t iter = 0; iter < indent + 1; ++iter) stream << INDENT;
        stream << "def "
               << value.first << '.'
               << value.second << ';' << LINE_BREAK;
      });
  std::for_each (
      module->globalVariablesConstBegin (),
      module->globalVariablesConstEnd (),
      [&] (const Module::GlobalVariableConstIterator::value_type &value) -> void
      {
        for (std::size_t iter = 0; iter < indent + 1; ++iter) stream << INDENT;
        stream << "def "
               << value.first << " :";
        print (value.second, 0);
        stream << ';' << LINE_BREAK;
      });
  std::for_each (
      module->methodsConstBegin (),
      module->methodsConstEnd (),
      [&] (const Module::MethodConstIterator::value_type &value) -> void
      {
        print (value, indent + 1);
        stream << LINE_BREAK;
      });
  for (std::size_t iter = 0; iter < indent; ++iter) stream << INDENT;
  stream << '}';
}

void ASTPrinter::caseMethod (const Method *method, size_t indent)
{
  assert (method != nullptr);
  for (std::size_t iter = 0; iter < indent; ++iter) stream << INDENT;
  stream << "def " << method->getMethodName () << " (";
  for (auto parameterIter = method->parametersConstBegin ();
       parameterIter != method->parametersConstEnd (); ++parameterIter)
    {
      print (parameterIter->first, 0);
      stream << " :";
      print (parameterIter->second, 0);
      if (std::next (parameterIter) != method->parametersConstEnd ())
        { stream << ", "; }
    }
  stream << ") :";
  print (method->getReturnType (), 0);
  stream << " {" << LINE_BREAK;
  std::for_each (
      method->statementsConstBegin (),
      method->statementsConstEnd (),
      [&] (const Method::StatementConstIterator::value_type &value) -> void
      {
        print (value, indent + 1);
        stream << LINE_BREAK;
      });
  for (std::size_t iter = 0; iter < indent; ++iter) stream << INDENT;
  stream << '}';
}

void
ASTPrinter::caseAssignStatement (const AssignStatement *stmt, size_t indent)
{
  assert (stmt != nullptr);
  for (std::size_t iter = 0; iter < indent; ++iter) stream << INDENT;
  using ReturnTypePolicy = Statement::ReturnTypePolicy;
  const auto lhsInfo = stmt->getLHSInfo ();
  const auto rhsInfo = stmt->getRHSInfo ();
  print (lhsInfo.first, 0);
  stream << " :";
  print (lhsInfo.second, 0);
  stream << " = ";
  if (stmt->getReturnTypePolicy () == ReturnTypePolicy::Direct)
    {
      print (rhsInfo.first, 0);
    }
  else if (stmt->getReturnTypePolicy () == ReturnTypePolicy::Cast)
    {
      stream << '(';
      print (rhsInfo.second, 0);
      stream << ") ";
      print (rhsInfo.first, 0);
    }
  else if (stmt->getReturnTypePolicy () == ReturnTypePolicy::IsType)
    {
      stream << "is_type (";
      print (rhsInfo.first, 0);
      stream << ", ";
      print (rhsInfo.second, 0);
      stream << ')';
    }
  else if (stmt->getReturnTypePolicy () == ReturnTypePolicy::CheckCast)
    {
      stream << "check_cast (";
      print (rhsInfo.first, 0);
      stream << ", ";
      print (rhsInfo.second, 0);
      stream << ')';
    }
  stream << ';';
}

void
ASTPrinter::caseBranchStatement (const BranchStatement *stmt, size_t indent)
{
  assert (stmt != nullptr);
  for (std::size_t iter = 0; iter < indent; ++iter) stream << INDENT;
  stream << "goto [" << stmt->getTargetLabelName () << ']';
  if (stmt->isConditional ())
    {
      stream << ' ';
      print (stmt->getOperand ());
    }
  stream << ';';
}

void
ASTPrinter::caseInvokeStatement (const InvokeStatement *stmt, size_t indent)
{
  assert (stmt != nullptr);
  for (std::size_t iter = 0; iter < indent; ++iter) stream << INDENT;
  const auto lhsInfo = stmt->getLHSInfo ();
  print (lhsInfo.first, 0);
  stream << " :";
  print (lhsInfo.second, 0);
  stream << " = ";

  using ReturnTypePolicy = Statement::ReturnTypePolicy;
  if (stmt->getReturnTypePolicy () == ReturnTypePolicy::Direct)
    { /* NOOP */ }
  else if (stmt->getReturnTypePolicy () == ReturnTypePolicy::Cast)
    {
      stream << '(';
      print (stmt->getRHSType ());
      stream << ") ";
    }
  else if (stmt->getReturnTypePolicy () == ReturnTypePolicy::IsType)
    { stream << "is_type ("; }
  else if (stmt->getReturnTypePolicy () == ReturnTypePolicy::CheckCast)
    { stream << "check_cast ("; }

  stream << '@'
         << ((stmt->getTargetModuleName ().empty ()) ?
             "" : (stmt->getTargetModuleName () + "."))
         << stmt->getTargetMethodName ()
         << " (";
  for (auto operandIter = stmt->rhsOperandsConstBegin ();
       operandIter != stmt->rhsOperandsConstEnd (); ++operandIter)
    {
      print (*operandIter, 0);
      if (std::next (operandIter) != stmt->rhsOperandsConstEnd ())
        { stream << ", "; }
    }
  stream << ')';

  if (stmt->getReturnTypePolicy () == ReturnTypePolicy::Direct)
    { /* NOOP */ }
  else if (stmt->getReturnTypePolicy () == ReturnTypePolicy::Cast)
    { /* NOOP */ }
  else if (stmt->getReturnTypePolicy () == ReturnTypePolicy::IsType)
    {
      stream << ", ";
      print (stmt->getRHSType (), 0);
      stream << ')';
    }
  else if (stmt->getReturnTypePolicy () == ReturnTypePolicy::CheckCast)
    {
      stream << ", ";
      print (stmt->getRHSType (), 0);
      stream << ')';
    }
  stream << ';';
}

void ASTPrinter::caseLabelStatement (const LabelStatement *stmt, size_t indent)
{
  assert (stmt != nullptr);
  for (std::size_t iter = 0; iter < indent; ++iter) stream << INDENT;
  stream << "[ " << stmt->getLabelName () << " ]";
}

void ASTPrinter::casePhiStatement (const PhiStatement *stmt, size_t indent)
{
  assert (stmt != nullptr);
  for (std::size_t iter = 0; iter < indent; ++iter) stream << INDENT;
  const auto lhsInfo = stmt->getLHSInfo ();
  print (lhsInfo.first, 0);
  stream << " :";
  print (lhsInfo.second, 0);
  stream << " = phi (";
  for (auto rhsMapIter = stmt->rhsMapConstBegin ();
       rhsMapIter != stmt->rhsMapConstEnd (); ++rhsMapIter)
    {
      stream << "[ " << rhsMapIter->first << " ] ";
      print (rhsMapIter->second, 0);
      if (std::next (rhsMapIter) != stmt->rhsMapConstEnd ())
        { stream << ", "; }
    }
  stream << ");";
}

void
ASTPrinter::caseReturnStatement (const ReturnStatement *stmt, size_t indent)
{
  assert (stmt != nullptr);
  for (std::size_t iter = 0; iter < indent; ++iter) stream << INDENT;
  stream << "return ";
  print (stmt->getOperand (), 0);
  stream << ';';
}

void ASTPrinter::caseBoolLiteral (const BoolLiteral *boolLiteral)
{
  assert (boolLiteral != nullptr);
  using ValueType = BoolLiteral::ElementType::ValueType;
  caseVectorLiteral (
      boolLiteral,
      [&] (const ValueType &value) -> void
      { stream << ((value) ? "1" : "0"); });
}

void ASTPrinter::caseCharLiteral (const CharLiteral *charLiteral)
{
  assert (charLiteral != nullptr);

  using ValueType = CharLiteral::ElementType::ValueType;
  caseVectorLiteral (
      charLiteral,
      [&] (const ValueType &value) -> void
      { printCharWithEscape (value); });
}

void ASTPrinter::caseComplexLiteral (const ComplexLiteral *complexLiteral)
{
  assert (complexLiteral != nullptr);
  using ValueType = ComplexLiteral::ElementType::ValueType;
  caseVectorLiteral (
      complexLiteral,
      [&] (const ValueType &value) -> void
      {
        stream << value.real ();
        if (value.imag () >= 0) stream << '+';
        stream << value.imag () << 'i';
      });
}

void ASTPrinter::caseDateLiteral (const DateLiteral *dateLiteral)
{
  assert (dateLiteral != nullptr);
  using ValueType = DateLiteral::ElementType::ValueType;
  caseVectorLiteral (
      dateLiteral,
      [&] (const ValueType &value) -> void
      {
        stream << std::to_string (value.year) << '.'
               << std::to_string (value.month) << '.'
               << std::to_string (value.day);
      });
}

void ASTPrinter::caseDateTimeLiteral (const DateTimeLiteral *dateTimeLiteral)
{
  assert (dateTimeLiteral != nullptr);
  using ValueType = DateTimeLiteral::ElementType::ValueType;
  caseVectorLiteral (
      dateTimeLiteral,
      [&] (const ValueType &value) -> void
      {
        stream << std::to_string (value.year) << '.'
               << std::to_string (value.month) << '.'
               << std::to_string (value.day) << 'T'
               << std::to_string (value.hour) << ':'
               << std::to_string (value.minute) << ':'
               << std::to_string (value.second) << '.'
               << std::to_string (value.millisecond);
      });
}

void
ASTPrinter::caseDictionaryLiteral (const DictionaryLiteral *dictionaryLiteral)
{
  assert (dictionaryLiteral != nullptr);
  if (dictionaryLiteral->isNull ())
    {
      stream << "null :";
      print (dictionaryLiteral->getLiteralType (), 0);
      return;
    }
  stream << '{';
  for (auto dictEntryIter = dictionaryLiteral->valueConstBegin ();
       dictEntryIter != dictionaryLiteral->valueConstEnd (); ++dictEntryIter)
    {
      print (dictEntryIter->key, 0);
      stream << " -> ";
      print (dictEntryIter->value, 0);
      if (std::next (dictEntryIter) != dictionaryLiteral->valueConstEnd ())
        { stream << ", "; }
    }
  stream << "} :";
  print (dictionaryLiteral->getLiteralType (), 0);
}

void
ASTPrinter::caseEnumerationLiteral (const EnumerationLiteral *enumLiteral)
{
  assert (enumLiteral != nullptr);
  if (enumLiteral->isNull ())
    {
      stream << "null :";
      print (enumLiteral->getLiteralType (), 0);
      return;
    }
  const auto enumValue = enumLiteral->getValue ();
  stream << '<' << enumValue.head << " -> ";
  print (enumValue.content, 0);
  stream << "> :";
  print (enumLiteral->getLiteralType (), 0);
}

void ASTPrinter::caseFP32Literal (const FP32Literal *fp32Literal)
{
  assert (fp32Literal != nullptr);
  using ValueType = FP32Literal::ElementType::ValueType;
  caseVectorLiteral (
      fp32Literal,
      [&] (const ValueType &value) -> void
      { stream << std::to_string (value); });
}

void ASTPrinter::caseFP64Literal (const FP64Literal *fp64Literal)
{
  assert (fp64Literal != nullptr);
  using ValueType = FP64Literal::ElementType::ValueType;
  caseVectorLiteral (
      fp64Literal,
      [&] (const ValueType &value) -> void
      { stream << std::to_string (value); });
}

void ASTPrinter::caseFunctionLiteral (const FunctionLiteral *functionLiteral)
{
  assert (functionLiteral != nullptr);
  using ValueType = FunctionLiteral::ElementType::ValueType;
  caseVectorLiteral (
      functionLiteral,
      [&] (const ValueType &value) -> void
      {
        stream << '@';
        if (value.moduleName.empty ())
          {
            stream << value.methodName;
            return;
          }
        stream << value.moduleName << '.' << value.methodName;
      });
}

void ASTPrinter::caseInteger8Literal (const Integer8Literal *integer8Literal)
{
  assert (integer8Literal != nullptr);
  using ValueType = Integer8Literal::ElementType::ValueType;
  caseVectorLiteral (
      integer8Literal,
      [&] (const ValueType &value) -> void
      { stream << std::to_string (value); });
}

void ASTPrinter::caseInteger16Literal (const Integer16Literal *integer16Literal)
{
  assert (integer16Literal != nullptr);
  using ValueType = Integer16Literal::ElementType::ValueType;
  caseVectorLiteral (
      integer16Literal,
      [&] (const ValueType &value) -> void
      { stream << std::to_string (value); });
}

void ASTPrinter::caseInteger32Literal (const Integer32Literal *integer32Literal)
{
  assert (integer32Literal != nullptr);
  using ValueType = Integer32Literal::ElementType::ValueType;
  caseVectorLiteral (
      integer32Literal,
      [&] (const ValueType &value) -> void
      { stream << std::to_string (value); });
}

void ASTPrinter::caseInteger64Literal (const Integer64Literal *integer64Literal)
{
  assert (integer64Literal != nullptr);
  using ValueType = Integer64Literal::ElementType::ValueType;
  caseVectorLiteral (
      integer64Literal,
      [&] (const ValueType &value) -> void
      { stream << std::to_string (value); });
}

void ASTPrinter::caseKeyTableLiteral (const KeyTableLiteral *keyTableLiteral)
{
  assert (keyTableLiteral != nullptr);
  if (keyTableLiteral->isNull ())
    {
      stream << "null :";
      print (keyTableLiteral->getLiteralType (), 0);
      return;
    }
  stream << '{';
  std::vector<KeyTableLiteral::ElementType> keyColumns{};
  std::vector<KeyTableLiteral::ElementType> nonKeyColumns{};
  for (auto columnIter = keyTableLiteral->valueConstBegin ();
       columnIter != keyTableLiteral->valueConstEnd (); ++columnIter)
    {
      if (columnIter->isKey)
        { keyColumns.emplace_back (*columnIter); }
      else
        { nonKeyColumns.emplace_back (*columnIter); }
    }
  for (auto keyColumnIter = keyColumns.cbegin ();
       keyColumnIter != keyColumns.cend (); ++keyColumnIter)
    {
      stream << '[' << keyColumnIter->head << " -> ";
      print (keyColumnIter->content, 0);
      stream << "]";
      if (std::next (keyColumnIter) != keyColumns.cend () ||
          (!nonKeyColumns.empty ()))
        { stream << ", "; }
    }
  for (auto nonKeyColumnIter = nonKeyColumns.cbegin ();
       nonKeyColumnIter != nonKeyColumns.cend (); ++nonKeyColumnIter)
    {
      stream << nonKeyColumnIter->head << " -> ";
      print (nonKeyColumnIter->content, 0);
      if (std::next (nonKeyColumnIter) != nonKeyColumns.cend ())
        { stream << ", "; }
    }
  stream << "} :";
  print (keyTableLiteral->getLiteralType (), 0);
}

void ASTPrinter::printCharWithEscape (std::uint8_t value)
{
  std::locale cLocale ("C");
  if (value == 0x07)
    { stream << R"STR('\a')STR"; }
  else if (value == 0x08)
    { stream << R"STR('\b')STR"; }
  else if (value == 0x0C)
    { stream << R"STR('\f')STR"; }
  else if (value == 0x0A)
    { stream << R"STR('\n')STR"; }
  else if (value == 0x0D)
    { stream << R"STR('\r')STR"; }
  else if (value == 0x09)
    { stream << R"STR('\t')STR"; }
  else if (value == 0x0B)
    { stream << R"STR('\v')STR"; }
  else if (value == 0x5C)
    { stream << R"STR('\\')STR"; }
  else if (value == 0x27)
    { stream << R"STR('\'')STR"; }
  else if (value == 0x22)
    { stream << R"STR('\"')STR"; }
  else if (value == 0x3F)
    { stream << R"STR('?')STR"; }
  else if (std::isprint (static_cast<char>(value), cLocale))
    { stream << '\'' << static_cast<char>(value) << '\''; }
  else
    {
      stream << R"STR(\x)STR";
      if (value <= 0x0F) stream << '0';
      static std::array<char, 16> digits = {
          '0', '1', '2', '3', '4', '5', '6', '7',
          '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
      };
      auto highBits = static_cast<std::size_t>(((value & 0xf0) >> 4) & 0x0f);
      auto lowBits = static_cast<std::size_t>(value & 0x0f);
      stream << digits.at (highBits) << digits.at (lowBits);
    }
}

void ASTPrinter::caseListLiteral (const ListLiteral *listLiteral)
{
  assert (listLiteral != nullptr);
  stream << '[';
  for (auto valueIter = listLiteral->valueConstBegin ();
       valueIter != listLiteral->valueConstEnd (); ++valueIter)
    {
      print (*valueIter, 0);
      if (std::next (valueIter) != listLiteral->valueConstEnd ())
        { stream << ", "; }
    }
  stream << "] :";
  print (listLiteral->getLiteralType (), 0);
}

void ASTPrinter::caseMinuteLiteral (const MinuteLiteral *minuteLiteral)
{
  assert (minuteLiteral != nullptr);
  using ValueType = MinuteLiteral::ElementType::ValueType;
  caseVectorLiteral (
      minuteLiteral,
      [&] (const ValueType &value) -> void
      {
        stream << std::to_string (value.hour) << ':'
               << std::to_string (value.minute);
      });
}

void ASTPrinter::caseMonthLiteral (const MonthLiteral *monthLiteral)
{
  assert (monthLiteral != nullptr);
  using ValueType = MonthLiteral::ElementType::ValueType;
  caseVectorLiteral (
      monthLiteral,
      [&] (const ValueType &value) -> void
      {
        stream << std::to_string (value.year) << '.'
               << std::to_string (value.month);
      });
}

void ASTPrinter::caseSecondLiteral (const SecondLiteral *secondLiteral)
{
  assert (secondLiteral != nullptr);
  using ValueType = SecondLiteral::ElementType::ValueType;
  caseVectorLiteral (
      secondLiteral,
      [&] (const ValueType &value) -> void
      { stream << value.hour << ':' << value.minute << ':' << value.second; });
}

void ASTPrinter::caseStringLiteral (const StringLiteral *stringLiteral)
{
  assert (stringLiteral != nullptr);
  using ValueType = StringLiteral::ElementType::ValueType;
  caseVectorLiteral (
      stringLiteral,
      [&] (const ValueType &value) -> void
      {
        stream << '\"';
        for (const auto &character : value) printCharWithEscape (character);
        stream << '\"';
      });
}

void ASTPrinter::caseSymbolLiteral (const SymbolLiteral *symbolLiteral)
{
  assert (symbolLiteral != nullptr);
  using ValueType = SymbolLiteral::ElementType::ValueType;
  caseVectorLiteral (
      symbolLiteral,
      [&] (const ValueType &value) -> void
      { stream << '`' << value; });
}

void ASTPrinter::caseTableLiteral (const TableLiteral *tableLiteral)
{
  assert (tableLiteral != nullptr);
  stream << '{';
  for (auto columnIter = tableLiteral->valueConstBegin ();
       columnIter != tableLiteral->valueConstEnd (); ++columnIter)
    {
      stream << columnIter->head << " -> ";
      print (columnIter->content, 0);
      if (std::next (columnIter) != tableLiteral->valueConstEnd ())
        { stream << ", "; }
    }
  stream << "} :";
  print (tableLiteral->getLiteralType (), 0);
}

void ASTPrinter::caseTimeLiteral (const TimeLiteral *timeLiteral)
{
  assert (timeLiteral != nullptr);
  using ValueType = TimeLiteral::ElementType::ValueType;
  caseVectorLiteral (
      timeLiteral,
      [&] (const ValueType &value) -> void
      {
        stream << std::to_string (value.hour) << ':'
               << std::to_string (value.minute) << ':'
               << std::to_string (value.second) << '.'
               << std::to_string (value.millisecond);
      });
}

void ASTPrinter::caseDictionaryType (const DictionaryType *dictionaryType)
{
  assert (dictionaryType != nullptr);
  stream << "dict<";
  print (dictionaryType->getKeyType ());
  stream << ", ";
  print (dictionaryType->getValueType ());
  stream << '>';
}

void ASTPrinter::caseEnumerationType (const EnumerationType *enumerationType)
{
  assert (enumerationType != nullptr);
  stream << "enum<";
  print (enumerationType->getElementType ());
  stream << '>';
}

void ASTPrinter::caseFunctionType (const FunctionType *functionType)
{
  assert (functionType != nullptr);
  stream << "func<";
  for (auto iterator = functionType->parameterTypesConstBegin ();
       iterator != functionType->parameterTypesConstEnd (); ++iterator)
    {
      print (*iterator, 0);
      if (std::next (iterator) != functionType->parameterTypesConstEnd () ||
          functionType->getIsFlexible ())
        { stream << ", "; }
    }
  if (functionType->getIsFlexible ()) stream << "...";
  stream << " :";
  print (functionType->getReturnType (), 0);
  stream << '>';
}

void ASTPrinter::caseListType (const ListType *listType)
{
  assert (listType != nullptr);
  stream << "list<";
  print (listType->getElementType (), 0);
  stream << '>';
}

void ASTPrinter::casePrimitiveType (const PrimitiveType *primitiveType)
{
  assert (primitiveType != nullptr);
  const auto primitiveClass = primitiveType->getPrimitiveClass ();
  using PrimitiveClass = PrimitiveType::PrimitiveClass;
  if (primitiveClass == PrimitiveClass::Bool)
    { stream << "bool"; }
  else if (primitiveClass == PrimitiveClass::Character)
    { stream << "char"; }
  else if (primitiveClass == PrimitiveClass::Integer8)
    { stream << "i8"; }
  else if (primitiveClass == PrimitiveClass::Integer16)
    { stream << "i16"; }
  else if (primitiveClass == PrimitiveClass::Integer32)
    { stream << "i32"; }
  else if (primitiveClass == PrimitiveClass::Integer64)
    { stream << "i64"; }
  else if (primitiveClass == PrimitiveClass::FP32)
    { stream << "f32"; }
  else if (primitiveClass == PrimitiveClass::FP64)
    { stream << "f64"; }
  else if (primitiveClass == PrimitiveClass::Complex)
    { stream << "complex"; }
  else if (primitiveClass == PrimitiveClass::Symbol)
    { stream << "sym"; }
  else if (primitiveClass == PrimitiveClass::String)
    { stream << "str"; }
  else if (primitiveClass == PrimitiveClass::Month)
    { stream << 'm'; }
  else if (primitiveClass == PrimitiveClass::Date)
    { stream << 'd'; }
  else if (primitiveClass == PrimitiveClass::DateTime)
    { stream << 'z'; }
  else if (primitiveClass == PrimitiveClass::Minute)
    { stream << 'u'; }
  else if (primitiveClass == PrimitiveClass::Second)
    { stream << 'v'; }
  else if (primitiveClass == PrimitiveClass::Time)
    { stream << 't'; }
  else if (primitiveClass == PrimitiveClass::Table)
    { stream << "table"; }
  else if (primitiveClass == PrimitiveClass::KeyTable)
    { stream << "ktable"; }
}

void ASTPrinter::caseWildcardType (const WildcardType *wildcardType)
{
  assert (wildcardType != nullptr);
  stream << '?';
}

void ASTPrinter::caseIdentifier (const Identifier *identifier)
{
  assert (identifier != nullptr);
  stream << identifier->getName ();
}