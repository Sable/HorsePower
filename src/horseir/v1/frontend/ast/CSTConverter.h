#pragma once

#include "AST.h"

namespace horseIR
{
namespace ast
{

struct CSTConverter {
  class CSTConverterException : public std::logic_error {
   public:
    using CSTType = antlr4::tree::ParseTree;

    explicit CSTConverterException (const CSTType *exceptionSite)
        : logic_error ("error occurred during transformation from CST to AST"),
          site (exceptionSite)
    {}

    const antlr4::tree::ParseTree *getExceptionSite () const
    { return site; }

   protected:
    const antlr4::tree::ParseTree *site = nullptr;
  };

  using ASTNodeMemory = ASTNode::ASTNodeMemory;

  using TypeContext = HorseIRParser::TypeContext;
  using TypeCasePrimitiveContext = HorseIRParser::TypeCasePrimitiveContext;
  using TypeCaseWildcardContext = HorseIRParser::TypeCaseWildcardContext;
  using TypeListContext = HorseIRParser::TypeListContext;
  using TypeDictContext = HorseIRParser::TypeDictContext;
  using TypeEnumContext = HorseIRParser::TypeEnumContext;
  using TypeFuncContext = HorseIRParser::TypeFuncContext;

  static Type *convert (ASTNodeMemory &mem, TypeContext *type);
  static WildcardType *
  convert (ASTNodeMemory &mem, TypeCaseWildcardContext *type);
  static PrimitiveType *
  convert (ASTNodeMemory &mem, TypeCasePrimitiveContext *type);
  static ListType *convert (ASTNodeMemory &mem, TypeListContext *type);
  static DictionaryType *convert (ASTNodeMemory &mem, TypeDictContext *type);
  static EnumerationType *convert (ASTNodeMemory &mem, TypeEnumContext *type);

  using TypeFuncCase0Context = HorseIRParser::TypeFuncCase0Context;
  using TypeFuncCase1Context = HorseIRParser::TypeFuncCase1Context;
  using TypeFuncCase2Context = HorseIRParser::TypeFuncCase2Context;
  using TypeFuncCase3Context = HorseIRParser::TypeFuncCase3Context;

  static FunctionType *convert (ASTNodeMemory &mem, TypeFuncContext *type);
  static FunctionType *convert (ASTNodeMemory &mem, TypeFuncCase0Context *type);
  static FunctionType *convert (ASTNodeMemory &mem, TypeFuncCase1Context *type);
  static FunctionType *convert (ASTNodeMemory &mem, TypeFuncCase2Context *type);
  static FunctionType *convert (ASTNodeMemory &mem, TypeFuncCase3Context *type);

  using LiteralContext = HorseIRParser::LiteralContext;
  using LiteralBoolContext = HorseIRParser::LiteralBoolContext;
  using LiteralCharContext = HorseIRParser::LiteralCharContext;
  using LiteralStringContext = HorseIRParser::LiteralStringContext;
  using LiteralIntegerContext = HorseIRParser::LiteralIntegerContext;
  using LiteralFloatContext = HorseIRParser::LiteralFloatContext;
  using LiteralComplexContext = HorseIRParser::LiteralComplexContext;
  using LiteralSymbolContext = HorseIRParser::LiteralSymbolContext;
  using LiteralTMonthContext = HorseIRParser::LiteralTMonthContext;
  using LiteralTDateContext = HorseIRParser::LiteralTDateContext;
  using LiteralTDateTimeContext = HorseIRParser::LiteralTDateTimeContext;
  using LiteralTMinuteContext = HorseIRParser::LiteralTMinuteContext;
  using LiteralTSecondContext = HorseIRParser::LiteralTSecondContext;
  using LiteralTTimeContext = HorseIRParser::LiteralTTimeContext;
  using LiteralFunctionContext = HorseIRParser::LiteralFunctionContext;
  using LiteralListContext = HorseIRParser::LiteralListContext;
  using LiteralDictContext = HorseIRParser::LiteralDictContext;
  using LiteralTableContext = HorseIRParser::LiteralTableContext;
  using LiteralKeyTableContext = HorseIRParser::LiteralKTableContext;
  using LiteralEnumContext = HorseIRParser::LiteralEnumContext;

  static Literal *convert (ASTNodeMemory &mem, LiteralContext *literal);

  using LiteralBoolCase0 = HorseIRParser::LiteralBoolCase0Context;
  using LiteralBoolCase1 = HorseIRParser::LiteralBoolCase1Context;

  static BoolLiteral *convert (ASTNodeMemory &mem, LiteralBoolContext *literal);
  static BoolLiteral *
  convert (ASTNodeMemory &mem, LiteralBoolCase0 *literalBool);
  static BoolLiteral *
  convert (ASTNodeMemory &mem, LiteralBoolCase1 *literalBool);

  using LiteralCharCase0Context = HorseIRParser::LiteralCharCase0Context;
  using LiteralCharCase1Context = HorseIRParser::LiteralCharCase1Context;
  using LiteralCharCase2Context = HorseIRParser::LiteralCharCase2Context;
  using LiteralCharCase3Context = HorseIRParser::LiteralCharCase3Context;
  using LiteralCharCase4Context = HorseIRParser::LiteralCharCase4Context;
  using LiteralCharCase5Context = HorseIRParser::LiteralCharCase5Context;

  static CharLiteral *
  convert (ASTNodeMemory &mem, LiteralCharContext *literalChar);
  static std::vector<std::uint8_t>
  convertEscapedChar (const std::basic_string<char> &rawString);
  static bool verifyEscapeChar (const std::string &rawString);
  static CharLiteral::ElementType
  convertCharValue (HorseIRParser::CharValueContext *charValue);
  static CharLiteral *
  convert (ASTNodeMemory &mem, LiteralCharCase0Context *literalChar);
  static CharLiteral *
  convert (ASTNodeMemory &mem, LiteralCharCase1Context *literalChar);
  static CharLiteral *
  convert (ASTNodeMemory &mem, LiteralCharCase2Context *literalChar);
  static CharLiteral *
  convert (ASTNodeMemory &mem, LiteralCharCase3Context *literalChar);
  static CharLiteral *
  convert (ASTNodeMemory &mem, LiteralCharCase4Context *literalChar);
  static CharLiteral *
  convert (ASTNodeMemory &mem, LiteralCharCase5Context *literalChar);

  using LiteralStringCase0Context = HorseIRParser::LiteralStringCase0Context;
  using LiteralStringCase1Context = HorseIRParser::LiteralStringCase1Context;
  using LiteralStringCase2Context = HorseIRParser::LiteralStringCase2Context;
  using LiteralStringCase3Context = HorseIRParser::LiteralStringCase3Context;
  using LiteralStringCase4Context = HorseIRParser::LiteralStringCase4Context;

  static StringLiteral *
  convert (ASTNodeMemory &mem, LiteralStringContext *stringContext);
  static StringLiteral::ElementType
  convertStringValue (HorseIRParser::StringValueContext *stringCST);
  static StringLiteral *
  convert (ASTNodeMemory &mem, LiteralStringCase0Context *stringCST);
  static StringLiteral *
  convert (ASTNodeMemory &mem, LiteralStringCase1Context *stringCST);
  static StringLiteral *
  convert (ASTNodeMemory &mem, LiteralStringCase2Context *stringCST);
  static StringLiteral *
  convert (ASTNodeMemory &mem, LiteralStringCase3Context *stringCST);
  static StringLiteral *
  convert (ASTNodeMemory &mem, LiteralStringCase4Context *stringCST);

  using LiteralIntegerCase0Context = HorseIRParser::LiteralIntegerCase0Context;
  using LiteralIntegerCase1Context = HorseIRParser::LiteralIntegerCase1Context;

  static Literal *convert (ASTNodeMemory &mem, LiteralIntegerContext *literal);

  template<class T, typename = std::enable_if_t<std::is_signed<T>::value>>
  static typename VectorLiteral<T>::ElementType
  convertIntValueN (HorseIRParser::IntValueNContext *context)
  {
    assert (context != nullptr);
    using ElementType = typename VectorLiteral<T>::ElementType;
    if (context->NULL_TOKEN () != nullptr)
      { return ElementType (nullptr); }
    int sign = 1;
    const auto signToken = context->op;
    if (signToken != nullptr)
      {
        if (signToken->getText () == "-") sign = -1;
      }
    const std::string valueString = context->LITERAL_INTEGER ()->getText ();
    try
      {
        std::size_t pos = 0;
        signed long long rawValue = std::stoll (valueString, &pos, 10);
        if (rawValue < std::numeric_limits<T>::min ())
          { throw CSTConverterException (context); }
        if (rawValue > std::numeric_limits<T>::max ())
          { throw CSTConverterException (context); }
        auto castedValue = static_cast<T>(rawValue);
        return ElementType (sign * castedValue);
      }
    catch (const std::invalid_argument &exception)
      { throw CSTConverterException (context); }
    catch (const std::out_of_range &exception)
      { throw CSTConverterException (context); }
  }

  static Literal *
  convert (ASTNodeMemory &mem, LiteralIntegerCase0Context *literal);
  static Literal *
  convert (ASTNodeMemory &mem, LiteralIntegerCase1Context *literal);

  using LiteralFloatCase0Context = HorseIRParser::LiteralFloatCase0Context;
  using LiteralFloatCase1Context = HorseIRParser::LiteralFloatCase1Context;

  static Literal *convert (ASTNodeMemory &mem, LiteralFloatContext *literal);

  template<class T, typename = std::enable_if<std::is_floating_point<T>::value>>
  static typename VectorLiteral<T>::ElementType
  convertFloatValueN (
      HorseIRParser::FloatValueNContext *context,
      T (*func) (const std::string &, std::size_t *))
  {
    assert (context != nullptr);
    assert (func != nullptr);
    using ElementType = typename VectorLiteral<T>::ElementType;
    if (context->NULL_TOKEN () != nullptr) return ElementType (nullptr);
    std::ostringstream stream;
    const auto opToken = context->op;
    if (opToken != nullptr) stream << opToken->getText ();
    const std::string valueString = context->value->getText ();
    stream << valueString;
    try
      {
        const std::string streamStream = stream.str ();
        size_t pos = 0;
        T converted = func (streamStream, &pos);
        return ElementType (converted);
      }
    catch (const std::invalid_argument &exception)
      { throw CSTConverterException (context); }
    catch (const std::out_of_range &exception)
      { throw CSTConverterException (context); }
  }

  static Literal *
  convert (ASTNodeMemory &mem, LiteralFloatCase0Context *literal);
  static Literal *
  convert (ASTNodeMemory &mem, LiteralFloatCase1Context *literal);

  using LiteralComplexCase0Context = HorseIRParser::LiteralComplexCase0Context;
  using LiteralComplexCase1Context = HorseIRParser::LiteralComplexCase1Context;
  using ComplexValueNContext = HorseIRParser::ComplexValueNContext;
  using ComplexValueNCase0Context = HorseIRParser::ComplexValueNCase0Context;
  using ComplexValueNCase1Context = HorseIRParser::ComplexValueNCase1Context;
  using ComplexValueNCase2Context = HorseIRParser::ComplexValueNCase2Context;
  using ComplexValueNCase3Context = HorseIRParser::ComplexValueNCase3Context;
  using ComplexValueNCase4Context = HorseIRParser::ComplexValueNCase4Context;

  static ComplexLiteral *
  convert (ASTNodeMemory &mem, LiteralComplexContext *literal);
  static ComplexLiteral::ElementType
  convertComplexValueN (ComplexValueNContext *valueContext);
  static ComplexLiteral::ElementType
  convertComplexValueN (ComplexValueNCase0Context *valueContext);
  static ComplexLiteral::ElementType
  convertComplexValueN (ComplexValueNCase1Context *valueContext);
  static ComplexLiteral::ElementType
  convertComplexValueN (ComplexValueNCase2Context *valueContext);
  static ComplexLiteral::ElementType
  convertComplexValueN (ComplexValueNCase3Context *valueContext);
  static ComplexLiteral::ElementType
  convertComplexValueN (ComplexValueNCase4Context *valueContext);
  static ComplexLiteral *
  convert (ASTNodeMemory &mem, LiteralComplexCase0Context *literal);
  static ComplexLiteral *
  convert (ASTNodeMemory &mem, LiteralComplexCase1Context *literal);

  using SymbolValueContext = HorseIRParser::SymbolValueContext;
  using LiteralSymbolCase0Context = HorseIRParser::LiteralSymbolCase0Context;
  using LiteralSymbolCase1Context = HorseIRParser::LiteralSymbolCase1Context;
  using LiteralSymbolCase2Context = HorseIRParser::LiteralSymbolCase2Context;
  using LiteralSymbolCase3Context = HorseIRParser::LiteralSymbolCase3Context;
  using LiteralSymbolCase4Context = HorseIRParser::LiteralSymbolCase4Context;

  static SymbolLiteral *
  convert (ASTNodeMemory &mem, LiteralSymbolContext *literal);
  static SymbolLiteral::ElementType
  convertSymbolValue (SymbolValueContext *valueContext);
  static SymbolLiteral *
  convert (ASTNodeMemory &mem, LiteralSymbolCase0Context *literal);
  static SymbolLiteral *
  convert (ASTNodeMemory &mem, LiteralSymbolCase1Context *literal);
  static SymbolLiteral *
  convert (ASTNodeMemory &mem, LiteralSymbolCase2Context *literal);
  static SymbolLiteral *
  convert (ASTNodeMemory &mem, LiteralSymbolCase3Context *literal);
  static SymbolLiteral *
  convert (ASTNodeMemory &mem, LiteralSymbolCase4Context *literal);

  using LiteralTMonthCase0Context = HorseIRParser::LiteralTMonthCase0Context;
  using LiteralTMonthCase1Context = HorseIRParser::LiteralTMonthCase1Context;

  static MonthLiteral *
  convert (ASTNodeMemory &mem, LiteralTMonthContext *literal);
  static MonthLiteral::ElementType
  convertMonthValueN (HorseIRParser::TMonthValueNContext *valueContext);
  static MonthLiteral *
  convert (ASTNodeMemory &mem, LiteralTMonthCase0Context *literal);
  static MonthLiteral *
  convert (ASTNodeMemory &mem, LiteralTMonthCase1Context *literal);

  using LiteralTDateCase0Context = HorseIRParser::LiteralTDateCase0Context;
  using LiteralTDateCase1Context = HorseIRParser::LiteralTDateCase1Context;
  using LiteralTDateCase2Context = HorseIRParser::LiteralTDateCase2Context;
  using LiteralTDateCase3Context = HorseIRParser::LiteralTDateCase3Context;
  using LiteralTDateCase4Context = HorseIRParser::LiteralTDateCase4Context;

  static DateLiteral *
  convert (ASTNodeMemory &mem, LiteralTDateContext *literal);
  static DateLiteral::ElementType
  convertDateValue (HorseIRParser::TDateValueContext *valueContext);
  static DateLiteral *
  convert (ASTNodeMemory &mem, LiteralTDateCase0Context *literal);
  static DateLiteral *
  convert (ASTNodeMemory &mem, LiteralTDateCase1Context *literal);
  static DateLiteral *
  convert (ASTNodeMemory &mem, LiteralTDateCase2Context *literal);
  static DateLiteral *
  convert (ASTNodeMemory &mem, LiteralTDateCase3Context *literal);
  static DateLiteral *
  convert (ASTNodeMemory &mem, LiteralTDateCase4Context *literal);

  using LiteralTDateTime0Context = HorseIRParser::LiteralTDateTimeCase0Context;
  using LiteralTDateTime1Context = HorseIRParser::LiteralTDateTimeCase1Context;
  using LiteralTDateTime2Context = HorseIRParser::LiteralTDateTimeCase2Context;
  using LiteralTDateTime3Context = HorseIRParser::LiteralTDateTimeCase3Context;
  using LiteralTDateTime4Context = HorseIRParser::LiteralTDateTimeCase4Context;

  static DateTimeLiteral *
  convert (ASTNodeMemory &mem, LiteralTDateTimeContext *literal);
  static DateTimeLiteral::ElementType
  convertDateTimeValue (HorseIRParser::TDateTimeValueContext *valueContext);
  static DateTimeLiteral *
  convert (ASTNodeMemory &mem, LiteralTDateTime0Context *literal);
  static DateTimeLiteral *
  convert (ASTNodeMemory &mem, LiteralTDateTime1Context *literal);
  static DateTimeLiteral *
  convert (ASTNodeMemory &mem, LiteralTDateTime2Context *literal);
  static DateTimeLiteral *
  convert (ASTNodeMemory &mem, LiteralTDateTime3Context *literal);
  static DateTimeLiteral *
  convert (ASTNodeMemory &mem, LiteralTDateTime4Context *literal);

  using LiteralTMinuteCase0Context = HorseIRParser::LiteralTMinuteCase0Context;
  using LiteralTMinuteCase1Context = HorseIRParser::LiteralTMinuteCase1Context;
  using LiteralTMinuteCase2Context = HorseIRParser::LiteralTMinuteCase2Context;
  using LiteralTMinuteCase3Context = HorseIRParser::LiteralTMinuteCase3Context;
  using LiteralTMinuteCase4Context = HorseIRParser::LiteralTMinuteCase4Context;

  static MinuteLiteral *
  convert (ASTNodeMemory &mem, LiteralTMinuteContext *literal);
  static MinuteLiteral::ElementType
  convertMinuteValue (HorseIRParser::TMinuteValueContext *valueContext);
  static MinuteLiteral *
  convert (ASTNodeMemory &mem, LiteralTMinuteCase0Context *literal);
  static MinuteLiteral *
  convert (ASTNodeMemory &mem, LiteralTMinuteCase1Context *literal);
  static MinuteLiteral *
  convert (ASTNodeMemory &mem, LiteralTMinuteCase2Context *literal);
  static MinuteLiteral *
  convert (ASTNodeMemory &mem, LiteralTMinuteCase3Context *literal);
  static MinuteLiteral *
  convert (ASTNodeMemory &mem, LiteralTMinuteCase4Context *literal);

  using LiteralTSecondCase0Context = HorseIRParser::LiteralTSecondCase0Context;
  using LiteralTSecondCase1Context = HorseIRParser::LiteralTSecondCase1Context;
  using LiteralTSecondCase2Context = HorseIRParser::LiteralTSecondCase2Context;
  using LiteralTSecondCase3Context = HorseIRParser::LiteralTSecondCase3Context;
  using LiteralTSecondCase4Context = HorseIRParser::LiteralTSecondCase4Context;

  static SecondLiteral *
  convert (ASTNodeMemory &mem, LiteralTSecondContext *literal);
  static SecondLiteral::ElementType
  convertSecondValue (HorseIRParser::TSecondValueContext *valueContext);
  static SecondLiteral *
  convert (ASTNodeMemory &mem, LiteralTSecondCase0Context *literal);
  static SecondLiteral *
  convert (ASTNodeMemory &mem, LiteralTSecondCase1Context *literal);
  static SecondLiteral *
  convert (ASTNodeMemory &mem, LiteralTSecondCase2Context *literal);
  static SecondLiteral *
  convert (ASTNodeMemory &mem, LiteralTSecondCase3Context *literal);
  static SecondLiteral *
  convert (ASTNodeMemory &mem, LiteralTSecondCase4Context *literal);

  using LiteralTTimeCase0Context = HorseIRParser::LiteralTTimeCase0Context;
  using LiteralTTimeCase1Context = HorseIRParser::LiteralTTimeCase1Context;
  using LiteralTTimeCase2Context = HorseIRParser::LiteralTTimeCase2Context;
  using LiteralTTimeCase3Context = HorseIRParser::LiteralTTimeCase3Context;
  using LiteralTTimeCase4Context = HorseIRParser::LiteralTTimeCase4Context;

  static TimeLiteral *
  convert (ASTNodeMemory &mem, LiteralTTimeContext *literal);
  static TimeLiteral::ElementType
  convertTimeValue (HorseIRParser::TTimeValueContext *valueContext);
  static TimeLiteral *
  convert (ASTNodeMemory &mem, LiteralTTimeCase0Context *literal);
  static TimeLiteral *
  convert (ASTNodeMemory &mem, LiteralTTimeCase1Context *literal);
  static TimeLiteral *
  convert (ASTNodeMemory &mem, LiteralTTimeCase2Context *literal);
  static TimeLiteral *
  convert (ASTNodeMemory &mem, LiteralTTimeCase3Context *literal);
  static TimeLiteral *
  convert (ASTNodeMemory &mem, LiteralTTimeCase4Context *literal);

  using LiteralFunction0Context = HorseIRParser::LiteralFunctionCase0Context;
  using LiteralFunction1Context = HorseIRParser::LiteralFunctionCase1Context;

  static FunctionLiteral *
  convert (ASTNodeMemory &mem, LiteralFunctionContext *literal);
  static FunctionLiteral::ElementType
  convertFunctionValue (HorseIRParser::FunctionValueNContext *valueContext);
  static FunctionLiteral *
  convert (ASTNodeMemory &mem, LiteralFunction0Context *literal);
  static FunctionLiteral *
  convert (ASTNodeMemory &mem, LiteralFunction1Context *literal);

  using LiteralListCase0Context = HorseIRParser::LiteralListCase0Context;
  using LiteralListCase1Context = HorseIRParser::LiteralListCase1Context;
  using LiteralListCase2Context = HorseIRParser::LiteralListCase2Context;

  static ListLiteral *
  convert (ASTNodeMemory &mem, LiteralListContext *literal);
  static ListLiteral *
  convert (ASTNodeMemory &mem, LiteralListCase0Context *literal);
  static ListLiteral *
  convert (ASTNodeMemory &mem, LiteralListCase1Context *literal);
  static ListLiteral *
  convert (ASTNodeMemory &mem, LiteralListCase2Context *literal);

  using LiteralDictCase0Context = HorseIRParser::LiteralDictCase0Context;
  using LiteralDictCase1Context = HorseIRParser::LiteralDictCase1Context;
  using LiteralDictCase2Context = HorseIRParser::LiteralDictCase2Context;

  static DictionaryLiteral *
  convert (ASTNodeMemory &mem, LiteralDictContext *literal);
  static DictionaryLiteral *
  convert (ASTNodeMemory &mem, LiteralDictCase0Context *literal);
  static DictionaryLiteral *
  convert (ASTNodeMemory &mem, LiteralDictCase1Context *literal);
  static DictionaryLiteral *
  convert (ASTNodeMemory &mem, LiteralDictCase2Context *literal);

  using LiteralTableCase0Context = HorseIRParser::LiteralTableCase0Context;
  using LiteralTableCase1Context = HorseIRParser::LiteralTableCase1Context;
  using LiteralTableCase2Context = HorseIRParser::LiteralTableCase2Context;

  static TableLiteral *
  convert (ASTNodeMemory &mem, LiteralTableContext *literal);
  static std::string
  convertTableHead (HorseIRParser::TableHeaderContext *context);
  static Literal *
  convert (ASTNodeMemory &mem, HorseIRParser::DbContentContext *context);
  static TableLiteral *
  convert (ASTNodeMemory &mem, LiteralTableCase0Context *literal);
  static TableLiteral *
  convert (ASTNodeMemory &mem, LiteralTableCase1Context *literal);
  static TableLiteral *
  convert (ASTNodeMemory &mem, LiteralTableCase2Context *literal);

  using LiteralKeyTableCase0Context = HorseIRParser::LiteralKTableCase0Context;
  using LiteralKeyTableCase1Context = HorseIRParser::LiteralKTableCase1Context;

  static KeyTableLiteral *
  convert (ASTNodeMemory &mem, LiteralKeyTableContext *literal);
  static KeyTableLiteral *
  convert (ASTNodeMemory &mem, LiteralKeyTableCase0Context *literal);
  static KeyTableLiteral *
  convert (ASTNodeMemory &mem, LiteralKeyTableCase1Context *literal);

  using LiteralEnumCase0Context = HorseIRParser::LiteralEnumCase0Context;
  using LiteralEnumCase1Context = HorseIRParser::LiteralEnumCase1Context;

  static EnumerationLiteral *
  convert (ASTNodeMemory &mem, LiteralEnumContext *literal);
  static EnumerationLiteral *
  convert (ASTNodeMemory &mem, LiteralEnumCase0Context *literal);
  static EnumerationLiteral *
  convert (ASTNodeMemory &mem, LiteralEnumCase1Context *literal);

  using NameContext = HorseIRParser::NameContext;

  static Identifier *convert (ASTNodeMemory &mem, NameContext *context);

  using OperandContext = HorseIRParser::OperandContext;

  static Operand *convert (ASTNodeMemory &mem, OperandContext *context);

  using StatementContext = HorseIRParser::StatementContext;
  using StatementCase0Context = HorseIRParser::StatementCase0Context;
  using StatementCase1Context = HorseIRParser::StatementCase1Context;
  using StatementCase2Context = HorseIRParser::StatementCase2Context;
  using StatementCase3Context = HorseIRParser::StatementCase3Context;
  using StatementCase4Context = HorseIRParser::StatementCase4Context;
  using StatementCase5Context = HorseIRParser::StatementCase5Context;
  using StatementCase6Context = HorseIRParser::StatementCase6Context;
  using StatementCase7Context = HorseIRParser::StatementCase7Context;
  using StatementCase8Context = HorseIRParser::StatementCase8Context;
  using StatementCase9Context = HorseIRParser::StatementCase9Context;
  using StatementCase10Context = HorseIRParser::StatementCase10Context;
  using StatementCase11Context = HorseIRParser::StatementCase11Context;

  static Statement *convert (ASTNodeMemory &mem, StatementContext *statement);
  static LabelStatement *
  convert (ASTNodeMemory &mem, StatementCase0Context *statementContext);
  static AssignStatement *
  convert (ASTNodeMemory &mem, StatementCase1Context *statementContext);
  static AssignStatement *
  convert (ASTNodeMemory &mem, StatementCase2Context *statementContext);
  static AssignStatement *
  convert (ASTNodeMemory &mem, StatementCase3Context *statementContext);
  static AssignStatement *
  convert (ASTNodeMemory &mem, StatementCase4Context *statementContext);
  static std::pair<std::string, std::string>
  convertFunctionName (const std::string &rawTokenString);
  static InvokeStatement *
  convert (ASTNodeMemory &mem, StatementCase5Context *statementContext);
  static InvokeStatement *
  convert (ASTNodeMemory &mem, StatementCase6Context *statementContext);
  static InvokeStatement *
  convert (ASTNodeMemory &mem, StatementCase7Context *statementContext);
  static InvokeStatement *
  convert (ASTNodeMemory &mem, StatementCase8Context *statementContext);
  static PhiStatement *
  convert (ASTNodeMemory &mem, StatementCase9Context *statementContext);
  static ReturnStatement *
  convert (ASTNodeMemory &mem, StatementCase10Context *statementContext);
  static BranchStatement *
  convert (ASTNodeMemory &mem, StatementCase11Context *statementContext);

  using MethodContext = HorseIRParser::MethodContext;

  static Method *convert (ASTNodeMemory &mem, MethodContext *context);

  using ModuleContext = HorseIRParser::ModuleContext;

  static Module *convert (ASTNodeMemory &mem, ModuleContext *context);

  using ProgramContext = HorseIRParser::ProgramContext;

  static CompilationUnit *convert (ASTNodeMemory &mem, ProgramContext *context);
};

}
}