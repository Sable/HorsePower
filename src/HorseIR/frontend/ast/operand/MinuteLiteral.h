#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

namespace storage
{

struct Minute {
  std::uint8_t hour;
  std::uint8_t minute;
};

}

class MinuteLiteral : public VectorLiteral<storage::Minute> {
 public:
  MinuteLiteral ()
      : VectorLiteral<storage::Minute>
            (ASTNodeClass::MinuteLiteral, LiteralClass::Minute)
  {}

  explicit MinuteLiteral (const CSTType *cst)
      : VectorLiteral<storage::Minute>
            (ASTNodeClass::MinuteLiteral, cst, LiteralClass::Minute)
  {}

  MinuteLiteral (MinuteLiteral &&literal) = default;
  MinuteLiteral (const MinuteLiteral &literal) = default;
  MinuteLiteral &operator= (MinuteLiteral &&literal) = delete;
  MinuteLiteral &operator= (const MinuteLiteral &literal) = delete;
  ~MinuteLiteral () override = default;

  MinuteLiteral *duplicateDeep (ASTNodeMemory &mem) const override
  {
    auto minuteLiteral = mem.alloc<MinuteLiteral> ();
    minuteLiteral->VectorLiteral<storage::Minute>::__duplicateDeep (mem, this);
    return minuteLiteral;
  }

 protected:
  std::string
  elementToString (const storage::Minute &elementType) const override
  {
    std::ostringstream stream;
    stream << unsigned (elementType.hour) << ':'
           << unsigned (elementType.minute);
    return stream.str ();
  }
};

}
}