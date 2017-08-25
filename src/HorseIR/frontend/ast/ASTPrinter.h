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

  virtual void caseCompilationUnit (const CompilationUnit *unit, size_t indent);
  virtual void caseModule (const Module *module, size_t indent);
  virtual void caseMethod (const Method *method, size_t indent);

  virtual void caseAssignStatement (const AssignStatement *stmt, size_t indent);
  virtual void caseBranchStatement (const BranchStatement *stmt, size_t indent);
  virtual void caseInvokeStatement (const InvokeStatement *stmt, size_t indent);
  virtual void caseLabelStatement (const LabelStatement *stmt, size_t indent);
  virtual void casePhiStatement (const PhiStatement *stmt, size_t indent);
  virtual void caseReturnStatement (const ReturnStatement *stmt, size_t indent);

  virtual void caseBoolLiteral (const BoolLiteral *boolLiteral);
  virtual void caseCharLiteral (const CharLiteral *charLiteral);
  virtual void caseComplexLiteral (const ComplexLiteral *complexLiteral);
  virtual void caseDateLiteral (const DateLiteral *dateLiteral);
  virtual void caseDateTimeLiteral (const DateTimeLiteral *dateTimeLiteral);
  virtual void caseDictionaryLiteral (const DictionaryLiteral *dictionaryLiteral);
  virtual void caseEnumerationLiteral (const EnumerationLiteral *enumLiteral);
  virtual void caseFP32Literal (const FP32Literal *fp32Literal);
  virtual void caseFP64Literal (const FP64Literal *fp64Literal);
  virtual void caseFunctionLiteral (const FunctionLiteral *functionLiteral);
  virtual void caseInteger8Literal (const Integer8Literal *integer8Literal);
  virtual void caseInteger16Literal (const Integer16Literal *integer16Literal);
  virtual void caseInteger32Literal (const Integer32Literal *integer32Literal);
  virtual void caseInteger64Literal (const Integer64Literal *integer64Literal);
  virtual void caseKeyTableLiteral (const KeyTableLiteral *keyTableLiteral);
  virtual void caseListLiteral (const ListLiteral *listLiteral);
  virtual void caseMinuteLiteral (const MinuteLiteral *minuteLiteral);
  virtual void caseMonthLiteral (const MonthLiteral *monthLiteral);
  virtual void caseSecondLiteral (const SecondLiteral *secondLiteral);
  virtual void caseStringLiteral (const StringLiteral *stringLiteral);
  virtual void caseSymbolLiteral (const SymbolLiteral *symbolLiteral);
  virtual void caseTableLiteral (const TableLiteral *tableLiteral);
  virtual void caseTimeLiteral (const TimeLiteral *timeLiteral);

  virtual void caseDictionaryType (const DictionaryType *dictionaryType);
  virtual void caseEnumerationType (const EnumerationType *enumerationType);
  virtual void caseFunctionType (const FunctionType *functionType);
  virtual void caseListType (const ListType *listType);
  virtual void casePrimitiveType (const PrimitiveType *primitiveType);
  virtual void caseWildcardType (const WildcardType *wildcardType);

  virtual void caseIdentifier (const Identifier *identifier);

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