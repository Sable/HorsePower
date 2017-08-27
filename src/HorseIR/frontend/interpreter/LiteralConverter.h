#pragma once

#include "../ast/AST.h"
#include "h_global.h"

namespace horseIR
{
namespace interpreter
{

class LiteralConverter {
 public:
  static V convert (const ast::Literal *literal);

  static V caseBoolLiteral (const ast::BoolLiteral *literal);
  static V caseCharLiteral (const ast::CharLiteral *literal);
  static V caseComplexLiteral (const ast::ComplexLiteral *literal);
  static V caseDateLiteral (const ast::DateLiteral *literal);
  static V caseDateTimeLiteral (const ast::DateTimeLiteral *literal);
  static V caseDictionaryLiteral (const ast::DictionaryLiteral *literal);
  static V caseEnumerationLiteral (const ast::EnumerationLiteral *literal);
  static V caseFP32Literal (const ast::FP32Literal *literal);
  static V caseFP64Literal (const ast::FP64Literal *literal);
  static V caseInteger8Literal (const ast::Integer8Literal *literal);
  static V caseInteger16Literal (const ast::Integer16Literal *literal);
  static V caseInteger32Literal (const ast::Integer32Literal *literal);
  static V caseInteger64Literal (const ast::Integer64Literal *literal);
  static V caseKeyTableLiteral (const ast::KeyTableLiteral *literal);
  static V caseListLiteral (const ast::ListLiteral *literal);
  static V caseMinuteLiteral (const ast::MinuteLiteral *literal);
  static V caseMonthLiteral (const ast::MonthLiteral *literal);
  static V caseSecondLiteral (const ast::SecondLiteral *literal);
  static V caseStringLiteral (const ast::StringLiteral *literal);
  static V caseSymbolLiteral (const ast::SymbolLiteral *literal);
  static V caseTableLiteral (const ast::TableLiteral *literal);
  static V caseTimeLiteral (const ast::TimeLiteral *literal);
 protected:
  template<class VectorType, class NullFunc, class ValueFunc, class WriteFunc>
  static void writeVectorLiteral (V v, const VectorType &vector,
                                  NullFunc nullFunc, ValueFunc valueFunc,
                                  WriteFunc writeFunc)
  {
    const std::size_t vectorSize = vector.size ();
    for (std::size_t pos = 0; pos < vectorSize; ++pos)
      {
        const typename VectorType::value_type &element = vector[pos];
        if (element.isNull ())
          { writeFunc (v, nullFunc (element), pos); }
        else
          { writeFunc (v, valueFunc (element.getValue()), pos); }
      }
  }
};

}
}