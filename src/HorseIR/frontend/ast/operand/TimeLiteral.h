#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

namespace storage
{

struct Time {
  std::uint8_t hour;
  std::uint8_t minute;
  std::uint8_t second;
  std::uint16_t millisecond;
};

}

class TimeLiteral : public VectorLiteral<storage::Time> {
 public:
  explicit TimeLiteral (ASTNodeMemory &mem)
      : VectorLiteral<storage::Time>
            (mem, ASTNodeClass::TimeLiteral, LiteralClass::Time)
  {}

  TimeLiteral (ASTNodeMemory &mem, const CSTType *cst)
      : VectorLiteral<storage::Time>
            (mem, ASTNodeClass::TimeLiteral, cst, LiteralClass::Time)
  {}

  TimeLiteral (TimeLiteral &&literal) = default;
  TimeLiteral (const TimeLiteral &literal) = default;
  TimeLiteral &operator= (TimeLiteral &&literal) = delete;
  TimeLiteral &operator= (const TimeLiteral &literal) = delete;
  ~TimeLiteral () override = default;

  TimeLiteral *duplicateDeep (ASTNodeMemory &mem) const override
  {
    auto timeLiteral = new TimeLiteral (mem);
    timeLiteral->VectorLiteral<storage::Time>::__duplicateDeep (mem, this);
    return timeLiteral;
  }

 protected:
  std::string elementToString (const storage::Time &elementType) const override
  {
    std::ostringstream stream;
    stream << unsigned (elementType.hour) << ':'
           << unsigned (elementType.minute) << ':'
           << unsigned (elementType.second) << '.'
           << elementType.millisecond;
    return stream.str ();
  }
};

}
}