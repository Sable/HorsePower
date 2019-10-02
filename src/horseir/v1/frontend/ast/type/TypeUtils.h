#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

struct TypeUtils {
  using ASTNodeMemory = ASTNode::ASTNodeMemory;
  static bool isSame (const Type *lhs, const Type *rhs);
  static bool isSubType (const Type *lhs, const Type *rhs);
  static bool isInvokable (const FunctionType *functionType,
                           const std::vector<Type *> &arguments);
  static Type *join (ASTNodeMemory &mem, const Type *lhs, const Type *rhs);
  static Type *meet (ASTNodeMemory &mem, const Type *lhs, const Type *rhs);
};

}
}