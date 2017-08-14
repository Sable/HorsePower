#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

namespace storage
{

struct Month {
  std::uint16_t year;
  std::uint8_t month;
};

}

class MonthLiteral : public VectorLiteral<storage::Month> {
 public:
  explicit MonthLiteral (ASTNodeMemory &mem)
      : VectorLiteral<storage::Month>
            (mem, ASTNodeClass::MonthLiteral, LiteralClass::Month)
  {}

  MonthLiteral (ASTNodeMemory &mem, const CSTType *cst)
      : VectorLiteral<storage::Month>
            (mem, ASTNodeClass::MonthLiteral, cst, LiteralClass::Month)
  {}

  MonthLiteral (MonthLiteral &&literal) = default;
  MonthLiteral (const MonthLiteral &literal) = default;
  MonthLiteral &operator= (MonthLiteral &&literal) = delete;
  MonthLiteral &operator= (const MonthLiteral &literal) = delete;
  ~MonthLiteral () override = default;

  MonthLiteral *duplicateDeep (ASTNodeMemory &mem) const override
  {
    auto monthLiteral = new MonthLiteral (mem);
    monthLiteral->VectorLiteral<storage::Month>::__duplicateDeep (mem, this);
    return monthLiteral;
  }

 protected:
  std::string elementToString (const storage::Month &elementType) const override
  {
    std::ostringstream stream;
    stream << elementType.year
           << '.'
           << unsigned(elementType.month);
    return stream.str ();
  }
};

}
}