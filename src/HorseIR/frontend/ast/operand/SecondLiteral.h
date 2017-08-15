#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

namespace storage
{

struct Second {
  std::uint8_t hour;
  std::uint8_t minute;
  std::uint8_t second;
};

}

class SecondLiteral : public VectorLiteral<storage::Second> {
 public:
  SecondLiteral ()
      : VectorLiteral<storage::Second>
            (ASTNodeClass::SecondLiteral, LiteralClass::Second)
  {}

  explicit SecondLiteral (const CSTType *cst)
      : VectorLiteral<storage::Second>
            (ASTNodeClass::SecondLiteral, cst, LiteralClass::Second)
  {}

  SecondLiteral (SecondLiteral &&literal) = default;
  SecondLiteral (const SecondLiteral &literal) = default;
  SecondLiteral &operator= (SecondLiteral &&literal) = delete;
  SecondLiteral &operator= (const SecondLiteral &literal) = delete;
  ~SecondLiteral () override = default;

  SecondLiteral *duplicateDeep (ASTNodeMemory &mem) const override
  {
    auto secondLiteral = mem.alloc<SecondLiteral> ();
    secondLiteral->VectorLiteral<storage::Second>::__duplicateDeep (mem, this);
    return secondLiteral;
  }

 protected:
  std::string
  elementToString (const storage::Second &elementType) const override
  {
    std::ostringstream stream;
    stream << unsigned (elementType.hour) << ':'
           << unsigned (elementType.minute) << ':'
           << unsigned (elementType.second);
    return stream.str ();
  }
};

}
}