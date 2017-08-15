#pragma once

#include <ostream>
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

  Type (const ASTNodeClass &astNodeClass, const TypeClass &p_typeClass)
      : ASTNode (astNodeClass), typeClass (p_typeClass)
  {}

  Type (const ASTNodeClass &astNodeClass, const CSTType *parseTree,
        const TypeClass &p_typeClass)
      : ASTNode (astNodeClass, parseTree), typeClass (p_typeClass)
  {}

  Type (Type &&externType) = default;
  Type (const Type &externType) = default;
  Type &operator= (Type &&externType) = delete;
  Type &operator= (const Type &externType) = delete;
  ~Type () override = default;

  TypeClass getTypeClass () const
  { return typeClass; }

 protected:
  const TypeClass typeClass;

  void __duplicateDeep (ASTNodeMemory &mem, const Type *type)
  {
    assert (type != nullptr);
    ASTNode::__duplicateDeep (mem, type);
  }
};

std::ostream &
operator<< (std::ostream &stream, const Type::TypeClass &typeClass)
{
  using TypeClass = Type::TypeClass;
  switch (typeClass)
    {
      case TypeClass::Primitive: return stream << "PrimitiveType";
      case TypeClass::Wildcard: return stream << "WildcardType";
      case TypeClass::List: return stream << "ListType";
      case TypeClass::Dictionary: return stream << "DictionaryType";
      case TypeClass::Enumeration: return stream << "EnumerationType";
      case TypeClass::Function: return stream << "FunctionType";
    }
}

}
}