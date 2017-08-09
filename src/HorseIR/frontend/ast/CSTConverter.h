#pragma once

#include <unordered_map>
#include "AST.h"

namespace horseIR
{
namespace ast
{

struct CSTConverter {
  using ASTNodeMemory = ASTNode::ASTNodeMemory;

  using TypeCasePrimitiveContext = HorseIRParser::TypeCasePrimitiveContext;

  static PrimitiveType *
  convert (ASTNodeMemory &mem, const TypeCasePrimitiveContext *cst)
  {
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    const std::string cstText = cst->token->getText ();
    const std::unordered_map<const std::string, PrimitiveClass> convertMap = {
        {"bool", PrimitiveClass::Bool},
        {"char", PrimitiveClass::Character},
        {"i8", PrimitiveClass::Integer8}, 
    };
    auto primitiveType = new PrimitiveType (mem, cst);
    if (cstText == "bool")
      {
        primitiveType->setPrimitiveClass (PrimitiveClass::Bool);
      }
    else if
      {

      }
  }
};

}
}