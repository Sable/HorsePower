#pragma once

#include "../AST.h"

namespace horseIR
{
namespace ast
{

class Identifier : public Operand {
 public:
  explicit Identifier (ASTNodeMemory &mem);
  Identifier (ASTNodeMemory &mem, const CSTType *cst);
  Identifier (Identifier &&identifier) = default;
  Identifier (const Identifier &identifier) = default;
  Identifier &operator= (Identifier &&identifier) = delete;
  Identifier &operator= (const Identifier &identifier) = delete;
  ~Identifier () override = default;

  std::size_t getNumNodesRecursively () const override;
  std::vector<ASTNode *> getChildren () const override;
  Identifier *duplicateDeep (ASTNodeMemory &mem) const override;
  std::string toString () const override;

  std::string getName () const;
  template<class T>
  std::enable_if_t<std::is_assignable<std::string, T>::value>
  setName (T &&identifierName);

 protected:
  std::string name = "";
  void __duplicateDeep (ASTNodeMemory &mem, const Identifier *identifier);
};

inline Identifier::Identifier (ASTNodeMemory &mem)
    : Operand (mem, ASTNodeClass::Identifier, OperandClass::Identifier)
{}

inline Identifier::Identifier (ASTNodeMemory &mem, const CSTType *cst)
    : Operand (mem, ASTNodeClass::Identifier, cst, OperandClass::Identifier)
{}

inline std::size_t Identifier::getNumNodesRecursively () const
{ return 1; }

inline std::vector<ASTNode *> Identifier::getChildren () const
{ return std::vector<ASTNode *> {}; }

inline Identifier *Identifier::duplicateDeep (ASTNodeMemory &mem) const
{
  auto identifier = new Identifier (mem);
  identifier->__duplicateDeep (mem, this);
  return identifier;
}

inline std::string Identifier::toString () const
{ return name; }

inline std::string Identifier::getName () const
{ return name; }

template<class T>
inline std::enable_if_t<std::is_assignable<std::string, T>::value>
Identifier::setName (T &&identifierName)
{ name = std::forward<T> (identifierName); }

inline void
Identifier::__duplicateDeep (ASTNodeMemory &mem, const Identifier *identifier)
{
  assert (identifier != nullptr);
  Operand::__duplicateDeep (mem, identifier);
  name = identifier->name;
}

}
}