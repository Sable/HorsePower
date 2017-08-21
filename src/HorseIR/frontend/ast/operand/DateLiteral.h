#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

namespace storage
{

struct Date {
  std::uint16_t year;
  std::uint8_t month;
  std::uint8_t day;
};

}

class DateLiteral : public VectorLiteral<storage::Date> {
 public:
  DateLiteral ()
      : VectorLiteral<storage::Date>
            (ASTNodeClass::DateLiteral, LiteralClass::Date)
  {}

  explicit DateLiteral (const CSTType *cst)
      : VectorLiteral<storage::Date>
            (ASTNodeClass::DateLiteral, cst, LiteralClass::Date)
  {}

  DateLiteral (DateLiteral &&literal) = default;
  DateLiteral (const DateLiteral &literal) = default;
  DateLiteral &operator= (DateLiteral &&literal) = delete;
  DateLiteral &operator= (const DateLiteral &literal) = delete;
  ~DateLiteral () override = default;

  DateLiteral *duplicateDeep (ASTNodeMemory &mem) const override
  {
    auto dateLiteral = mem.alloc<DateLiteral> ();
    dateLiteral->VectorLiteral<storage::Date>::__duplicateDeep (mem, this);
    return dateLiteral;
  }
};

}
}