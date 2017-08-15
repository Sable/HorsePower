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
  explicit FunctionLiteral (ASTNodeMemory &mem)
      : VectorLiteral<storage::Function>
            (mem, ASTNodeClass::FunctionLiteral, LiteralClass::Function)
  {}

  FunctionLiteral (ASTNodeMemory &mem, const CSTType *cst)
      : VectorLiteral<storage::Function>
            (mem, ASTNodeClass::FunctionLiteral, cst, LiteralClass::Function)
  {}

  FunctionLiteral (FunctionLiteral &&literal) = default;
  FunctionLiteral (const FunctionLiteral &literal) = default;
  FunctionLiteral &operator= (FunctionLiteral &&literal) = delete;
  FunctionLiteral &operator= (const FunctionLiteral &literal) = delete;
  ~FunctionLiteral () override = default;

  FunctionLiteral *duplicateDeep (ASTNodeMemory &mem) const override
  {
    auto functionLiteral = new FunctionLiteral (mem);
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