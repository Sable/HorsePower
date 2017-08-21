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
};

template<class IType>
inline IType
BackendLiteralConverter<IType>::convertLiteral (const ast::Literal *literal)
{
  using LiteralClass = ast::Literal::LiteralClass;
  switch (literal->getLiteralClass ())
    {
      case LiteralClass::Bool:
        { return caseBool (dynamic_cast<const ast::BoolLiteral *>(literal)); }
      case LiteralClass::Character:
        { return caseChar (dynamic_cast<const ast::CharLiteral *>(literal)); }
      case LiteralClass::Complex:
        { return caseComplex (dynamic_cast<const ast::ComplexLiteral *>(literal)); }
      case LiteralClass::Date:
        { caseDate (dynamic_cast<const ast::DateLiteral *>(literal)); }
      case LiteralClass::DateTime:
        { caseDateTime (dynamic_cast<const ast::DateTimeLiteral *>(literal)); }
      case LiteralClass::Dictionary:
        {
          using DictionaryLiteral = ast::DictionaryLiteral;
          caseDictionary (dynamic_cast<const DictionaryLiteral *>(literal));
        }
      case LiteralClass::Enumeration:
        {
          using EnumerationLiteral = ast::EnumerationLiteral;
          caseEnumeration (dynamic_cast<const EnumerationLiteral *>(literal));
        }
      case LiteralClass::FP32:
        { caseFP32 (dynamic_cast<const ast::FP32Literal *>(literal)); }
      case LiteralClass::FP64:
        { caseFP64 (dynamic_cast<const ast::FP64Literal *>(literal)); }
    }
}

}
}