#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

namespace storage
{

struct DateTime {
  std::uint16_t year;
  std::uint8_t month;
  std::uint8_t day;
  std::uint8_t hour;
  std::uint8_t minute;
  std::uint8_t second;
  std::uint16_t millisecond;
};

}

class DateTimeLiteral : public VectorLiteral<storage::DateTime> {
 public:
  DateTimeLiteral ()
      : VectorLiteral<storage::DateTime>
            (ASTNodeClass::DateTimeLiteral, LiteralClass::DateTime)
  {}

  explicit DateTimeLiteral (const CSTType *cst)
      : VectorLiteral<storage::DateTime>
            (ASTNodeClass::DateTimeLiteral, cst, LiteralClass::DateTime)
  {}

  DateTimeLiteral (DateTimeLiteral &&literal) = default;
  DateTimeLiteral (const DateTimeLiteral &literal) = default;
  DateTimeLiteral &operator= (DateTimeLiteral &&literal) = delete;
  DateTimeLiteral &operator= (const DateTimeLiteral &literal) = delete;
  ~DateTimeLiteral () override = default;

  DateTimeLiteral *duplicateDeep (ASTNodeMemory &mem) const override
  {
    auto dateTimeLiteral = mem.alloc<DateTimeLiteral> ();
    dateTimeLiteral->
        VectorLiteral<storage::DateTime>::__duplicateDeep (mem, this);
    return dateTimeLiteral;
  }
};

}
}