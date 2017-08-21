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
  MonthLiteral ()
      : VectorLiteral<storage::Month>
            (ASTNodeClass::MonthLiteral, LiteralClass::Month)
  {}

  explicit MonthLiteral (const CSTType *cst)
      : VectorLiteral<storage::Month>
            (ASTNodeClass::MonthLiteral, cst, LiteralClass::Month)
  {}

  MonthLiteral (MonthLiteral &&literal) = default;
  MonthLiteral (const MonthLiteral &literal) = default;
  MonthLiteral &operator= (MonthLiteral &&literal) = delete;
  MonthLiteral &operator= (const MonthLiteral &literal) = delete;
  ~MonthLiteral () override = default;

  MonthLiteral *duplicateDeep (ASTNodeMemory &mem) const override
  {
    auto monthLiteral = mem.alloc<MonthLiteral> ();
    monthLiteral->VectorLiteral<storage::Month>::__duplicateDeep (mem, this);
    return monthLiteral;
  }
};

}
}