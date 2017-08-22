#pragma once

#include "../ast/AST.h"

namespace horseIR
{
namespace interpreter
{

template<class IType>
class BackendLiteralConverter {
 public:
  IType convertLiteral (const ast::Literal *literal);
 protected:
  virtual IType caseBool (const ast::BoolLiteral *literal) = 0;
  virtual IType caseChar (const ast::CharLiteral *literal) = 0;
  virtual IType caseComplex (const ast::ComplexLiteral *literal) = 0;
  virtual IType caseDate (const ast::DateLiteral *literal) = 0;
  virtual IType caseDateTime (const ast::DateTimeLiteral *literal) = 0;
  virtual IType caseDictionary (const ast::DictionaryLiteral *literal) = 0;
  virtual IType caseEnumeration (const ast::EnumerationLiteral *literal) = 0;
  virtual IType caseFP32 (const ast::FP32Literal *literal) = 0;
  virtual IType caseFP64 (const ast::FP64Literal *literal) = 0;
  virtual IType caseInteger8 (const ast::Integer8Literal *literal) = 0;
  virtual IType caseInteger16 (const ast::Integer16Literal *literal) = 0;
  virtual IType caseInteger32 (const ast::Integer32Literal *literal) = 0;
  virtual IType caseInteger64 (const ast::Integer64Literal *literal) = 0;
  virtual IType caseKeyTable (const ast::KeyTableLiteral *literal) = 0;
  virtual IType caseList (const ast::ListLiteral *literal) = 0;
  virtual IType caseMinute (const ast::MinuteLiteral *literal) = 0;
  virtual IType caseMonth (const ast::MonthLiteral *literal) = 0;
  virtual IType caseSecond (const ast::SecondLiteral *literal) = 0;
  virtual IType caseString (const ast::StringLiteral *literal) = 0;
  virtual IType caseSymbol (const ast::SymbolLiteral *literal) = 0;
  virtual IType caseTable (const ast::TableLiteral *literal) = 0;
 private:
  template<class T, class Function, class Candidate>
  constexpr IType castAndApply (Function function, Candidate candidate)
  { return function (dynamic_cast<const T *>(candidate)); }
};

template<class IType>
inline IType
BackendLiteralConverter<IType>::convertLiteral (const ast::Literal *literal)
{
  assert (literal != nullptr);
  using LiteralClass = ast::Literal::LiteralClass;
  const auto literalClass = literal->getLiteralClass ();
  if (literalClass == LiteralClass::Bool)
    { return castAndApply<ast::BoolLiteral> (caseBool, literal); }
  if (literalClass == LiteralClass::Character)
    { return castAndApply<ast::CharLiteral> (caseChar, literal); }
  if (literalClass == LiteralClass::Complex)
    { return castAndApply<ast::ComplexLiteral> (caseComplex, literal); }
  if (literalClass == LiteralClass::Date)
    { return castAndApply<ast::DateLiteral> (caseDate, literal); }
  if (literalClass == LiteralClass::DateTime)
    { return castAndApply<ast::DateTimeLiteral> (caseDateTime, literal); }
  if (literalClass == LiteralClass::Dictionary)
    { return castAndApply<ast::DictionaryLiteral> (caseDictionary, literal); }
  if (literalClass == LiteralClass::Enumeration)
    { return castAndApply<ast::EnumerationLiteral> (caseEnumeration, literal); }
  if (literalClass == LiteralClass::FP32)
    { return castAndApply<ast::FP32Literal> (caseFP32, literal); }
  if (literalClass == LiteralClass::FP64)
    { return castAndApply<ast::FP64Literal> (caseFP64, literal); }
  if (literalClass == LiteralClass::Integer8)
    { return castAndApply<ast::Integer8Literal> (caseInteger8, literal); }
  if (literalClass == LiteralClass::Integer16)
    { return castAndApply<ast::Integer16Literal> (caseInteger16, literal); }
  if (literalClass == LiteralClass::Integer32)
    { return castAndApply<ast::Integer32Literal> (caseInteger32, literal); }
  if (literalClass == LiteralClass::Integer64)
    { return castAndApply<ast::Integer64Literal> (caseInteger64, literal); }
  if (literalClass == LiteralClass::KeyTable)
    { return castAndApply<ast::KeyTableLiteral> (caseKeyTable, literal); }
  if (literalClass == LiteralClass::List)
    { return castAndApply<ast::ListLiteral> (caseList, literal); }
  if (literalClass == LiteralClass::Minute)
    { return castAndApply<ast::MinuteLiteral> (caseMinute, literal); }
  if (literalClass == LiteralClass::Month)
    { return castAndApply<ast::MonthLiteral> (caseMonth, literal); }
  if (literalClass == LiteralClass::Second)
    { return castAndApply<ast::SecondLiteral> (caseSecond, literal); }
  if (literalClass == LiteralClass::String)
    { return castAndApply<ast::StringLiteral> (caseString, literal); }
  if (literalClass == LiteralClass::Symbol)
    { return castAndApply<ast::SymbolLiteral> (caseSymbol, literal); }
  if (literalClass == LiteralClass::Table)
    { return castAndApply<ast::TableLiteral> (caseTable, literal); }
  assert (false);
  return nullptr;
}

}
}