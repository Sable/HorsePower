#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class Type : public ASTNode {
 public:
  enum class TypeClass {
    Primitive, Wildcard, List, Dictionary, Enumeration, Function
  };

  Type (ASTNodeMemory &mem, const ASTNodeClass &astNodeClass,
        const TypeClass &p_typeClass)
      : ASTNode (mem, astNodeClass), typeClass (p_typeClass)
  {}
  Type (ASTNodeMemory &mem, const ASTNodeClass &astNodeClass,
        const CSTType *parseTree, const TypeClass &p_typeClass)
      : ASTNode (mem, astNodeClass, parseTree), typeClass (p_typeClass)
  {}
  Type (ASTNodeMemory &mem, const ASTNodeClass &astNodeClass,
        const CSTType *parseTree, ASTNode *parent, const TypeClass &p_typeClass)
      : ASTNode (mem, astNodeClass, parseTree, parent), typeClass (p_typeClass)
  {}
  Type (Type &&externType) = default;
  Type (const Type &externType) = default;
  Type &operator= (Type &&externType) = delete;
  Type &operator= (const Type &externType) = delete;
  virtual ~Type () override = default;

  virtual bool isGeneralizationOf (Type *type) const = 0;
  virtual bool isSameAs (Type *type) const = 0;

 protected:
  const TypeClass typeClass;
};

}
}