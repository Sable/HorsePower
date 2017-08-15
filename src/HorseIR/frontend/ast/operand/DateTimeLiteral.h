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
  explicit DateTimeLiteral (ASTNodeMemory &mem)
      : VectorLiteral<storage::DateTime>
            (mem, ASTNodeClass::DateTimeLiteral, LiteralClass::DateTime)
  {}

  DateTimeLiteral (ASTNodeMemory &mem, const CSTType *cst)
      : VectorLiteral<storage::DateTime>
            (mem, ASTNodeClass::DateTimeLiteral, cst, LiteralClass::DateTime)
  {}

  DateTimeLiteral (DateTimeLiteral &&literal) = default;
  DateTimeLiteral (const DateTimeLiteral &literal) = default;
  DateTimeLiteral &operator= (DateTimeLiteral &&literal) = delete;
  DateTimeLiteral &operator= (const DateTimeLiteral &literal) = delete;
  ~DateTimeLiteral () override = default;

  DateTimeLiteral *duplicateDeep (ASTNodeMemory &mem) const override
  {
    auto dateTimeLiteral = new DateTimeLiteral (mem);
    dateTimeLiteral->
        VectorLiteral<storage::DateTime>::__duplicateDeep (mem, this);
    return dateTimeLiteral;
  }

 protected:
  std::string
  elementToString (const storage::DateTime &elementType) const override
  {
    std::ostringstream stream;
    stream << elementType.year << '.'
           << unsigned (elementType.month) << '.'
           << unsigned (elementType.day) << 'T'
           << unsigned (elementType.hour) << ':'
           << unsigned (elementType.minute) << '.'
           << elementType.millisecond;
    return stream.str ();
  }
};

}
}