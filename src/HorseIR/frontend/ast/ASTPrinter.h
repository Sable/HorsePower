#pragma once

#include <ostream>
#include <locale>
#include <array>
#include "AST.h"

namespace horseIR
{
namespace ast
{

class ASTPrinter {
 public:
  explicit ASTPrinter (std::ostream &stream);
  ASTPrinter (ASTPrinter &&printer) = default;
  ASTPrinter (const ASTPrinter &printer) = default;
  ASTPrinter &operator= (ASTPrinter &&printer) = delete;
  ASTPrinter &operator= (const ASTPrinter &printer) = delete;
  ~ASTPrinter () = default;

  void print (const ASTNode *astNode, size_t indent = 0);

 protected:
  std::ostream &stream;

  void caseCompilationUnit (const CompilationUnit *unit, size_t indent);
  void caseModule (const Module *module, size_t indent);
  void caseMethod (const Method *method, size_t indent);

  void caseAssignStatement (const AssignStatement *stmt, size_t indent);
  void caseBranchStatement (const BranchStatement *stmt, size_t indent);
  void caseInvokeStatement (const InvokeStatement *stmt, size_t indent);
  void caseLabelStatement (const LabelStatement *stmt, size_t indent);
  void casePhiStatement (const PhiStatement *stmt, size_t indent);
  void caseReturnStatement (const ReturnStatement *stmt, size_t indent);

  void caseBoolLiteral (const BoolLiteral *boolLiteral);
  void caseCharLiteral (const CharLiteral *charLiteral);
  void caseComplexLiteral (const ComplexLiteral *complexLiteral);
  void caseDateLiteral (const DateLiteral *dateLiteral);
  void caseDateTimeLiteral (const DateTimeLiteral *dateTimeLiteral);
  void caseDictionaryLiteral (const DictionaryLiteral *dictionaryLiteral);
  void caseEnumerationLiteral (const EnumerationLiteral *enumLiteral);
  void caseFP32Literal (const FP32Literal *fp32Literal);
  void caseFP64Literal (const FP64Literal *fp64Literal);
  void caseFunctionLiteral (const FunctionLiteral *functionLiteral);
  void caseInteger8Literal (const Integer8Literal *integer8Literal);
  void caseInteger16Literal (const Integer16Literal *integer16Literal);
  void caseInteger32Literal (const Integer32Literal *integer32Literal);
  void caseInteger64Literal (const Integer64Literal *integer64Literal);
  void caseKeyTableLiteral (const KeyTableLiteral *keyTableLiteral);
  void caseListLiteral (const ListLiteral *listLiteral);
  void caseMinuteLiteral (const MinuteLiteral *minuteLiteral);
  void caseMonthLiteral (const MonthLiteral *monthLiteral);
  void caseSecondLiteral (const SecondLiteral *secondLiteral);
  void caseStringLiteral (const StringLiteral *stringLiteral);
  void caseSymbolLiteral (const SymbolLiteral *symbolLiteral);
  void caseTableLiteral (const TableLiteral *tableLiteral);
  void caseTimeLiteral (const TimeLiteral *timeLiteral);

  void caseDictionaryType (const DictionaryType *dictionaryType);
  void caseEnumerationType (const EnumerationType *enumerationType);
  void caseFunctionType (const FunctionType *functionType);
  void caseListType (const ListType *listType);
  void casePrimitiveType (const PrimitiveType *primitiveType);
  void caseWildcardType (const WildcardType *wildcardType);

  void caseIdentifier (const Identifier *identifier);

 private:
  template<class VectorLiteralType, class ElementPrintFunc>
  void caseVectorLiteral (VectorLiteralType literal, ElementPrintFunc func)
  {
    if (literal->size () == 1)
      {
        if (literal->valueConstBegin ()->isNull ())
          { stream << "null"; }
        else
          {
            const auto elementValue = literal->valueConstBegin ()->getValue ();
            func (elementValue);
          };
        stream << " :";
        print (literal->getLiteralType (), 0);
        return;
      }

    stream << '(';
    for (auto literalIter = literal->valueConstBegin ();
         literalIter != literal->valueConstEnd (); ++literalIter)
      {
        if (literalIter->isNull ())
          { stream << "null"; }
        else
          {
            const auto elementValue = literalIter->getValue ();
            func (elementValue);
          }
        if (std::next (literalIter) != literal->valueConstEnd ())
          { stream << ", "; }
      }
    stream << ") :";
    print (literal->getLiteralType (), 0);
  }

  void printCharWithEscape (std::uint8_t value);
};

}
}