#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class Primitive {
 public:
  enum class PrimitiveClass {
    Primitive, Wildcard, List, Dictionary, Enumeration, Function
  };
 protected:
};

}
}