#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

namespace storage
{

struct Function {
  std::string moduleName;
  std::string methodName;
};

}

class FunctionLiteral : public VectorLiteral<storage::Function> {
 public:
  FunctionLiteral ()
      : VectorLiteral<storage::Function>
            (ASTNodeClass::FunctionLiteral, LiteralClass::Function)
  {}

  explicit FunctionLiteral (const CSTType *cst)
      : VectorLiteral<storage::Function>
            (ASTNodeClass::FunctionLiteral, cst, LiteralClass::Function)
  {}

  FunctionLiteral (FunctionLiteral &&literal) = default;
  FunctionLiteral (const FunctionLiteral &literal) = default;
  FunctionLiteral &operator= (FunctionLiteral &&literal) = delete;
  FunctionLiteral &operator= (const FunctionLiteral &literal) = delete;
  ~FunctionLiteral () override = default;

  FunctionLiteral *duplicateDeep (ASTNodeMemory &mem) const override
  {
    auto functionLiteral = mem.alloc<FunctionLiteral> ();
    functionLiteral->
        VectorLiteral<storage::Function>::__duplicateDeep (mem, this);
    return functionLiteral;
  }

 protected:
  std::string
  elementToString (const storage::Function &elementType) const override
  {
    std::ostringstream stream;
    stream << '@'
           << elementType.moduleName
           << (elementType.moduleName.empty () ? "" : ".")
           << elementType.methodName;
    return stream.str ();
  }
};

}
}