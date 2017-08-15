#pragma once

#include "../../misc/InfixOStreamIterator.h"
#include "../AST.h"

namespace horseIR
{
namespace ast
{

class ListLiteral : public Literal {
 public:
  explicit ListLiteral (ASTNodeMemory &mem)
      : Literal (mem, ASTNodeClass::ListLiteral, LiteralClass::List)
  {}

  ListLiteral (ASTNodeMemory &mem, const CSTType *cst)
      : Literal (mem, ASTNodeClass::ListLiteral, cst, LiteralClass::List)
  {}

  ListLiteral (ListLiteral &&literal) = default;

  ListLiteral (const ListLiteral &literal)
      : Literal (literal)
  {
    if (literal.value.get () == nullptr)
      { value.reset (nullptr); }
    else
      { value.reset (new std::vector<Literal *> (*(literal.value))); }
  }

  ListLiteral &operator= (ListLiteral &&literal) = delete;
  ListLiteral &operator= (const ListLiteral &literal) = delete;
  ~ListLiteral () override = default;

  bool isNull () const;
  std::vector<Literal *> getValue ();
  template<class T>
  std::enable_if_t<std::is_constructible<std::vector<Literal *>, T>::value>
  setValue (T &&valueContainer);

  std::size_t getNumNodesRecursively () const override;
  std::vector<ASTNode *> getChildren () const override;
  ListLiteral *duplicateDeep (ASTNodeMemory &mem) const override;
  std::string toString () const override;

 protected:
  std::unique_ptr<std::vector<Literal *>> value = nullptr;
  void
  __duplicateDeep (ASTNodeMemory &mem, const ListLiteral *literal);
};

inline bool ListLiteral::isNull () const
{ return value.get () == nullptr; }

inline std::vector<Literal *> ListLiteral::getValue ()
{ return *value; }

template<class T>
inline std::enable_if_t<std::is_constructible<std::vector<Literal *>, T>::value>
ListLiteral::setValue (T &&valueContainer)
{ value.reset (new std::vector<Literal *> (std::forward<T> (valueContainer))); }

inline std::size_t ListLiteral::getNumNodesRecursively () const
{
  if (value.get () == nullptr) return 1;
  std::size_t counter = 1;
  for (const Literal *literal : *value)
    { counter = counter + literal->getNumNodesRecursively (); }
  return counter;
}

inline std::vector<ASTNode *> ListLiteral::getChildren () const
{
  if (value.get () == nullptr) return std::vector<ASTNode *> {};
  std::vector<ASTNode *> childrenNodes{};
  childrenNodes.reserve (value->size ());
  std::transform (
      value->cbegin (), value->cend (), std::back_inserter (childrenNodes),
      [] (Literal *literal) -> ASTNode *
      { return static_cast<ASTNode *>(literal); });
  return childrenNodes;
}

inline ListLiteral *ListLiteral::duplicateDeep (ASTNodeMemory &mem) const
{
  auto listLiteral = new ListLiteral (mem);
  listLiteral->__duplicateDeep (mem, this);
  return listLiteral;
}

inline std::string ListLiteral::toString () const
{
  std::ostringstream s;
  if (value.get () == nullptr)
    {
      s << "[] :"
        << ((literalType == nullptr) ? "nullptr" : literalType->toString ());
    }
  else
    {
      s << '[';
      std::transform (
          value->cbegin (), value->cend (),
          misc::InfixOStreamIterator<std::string> (s, ", "),
          [] (Literal *literal) -> std::string
          { return literal->toString (); });
      s << "] :"
        << ((literalType == nullptr) ? "nullptr" : literalType->toString ());
    }
  return s.str ();
}

inline void
ListLiteral::__duplicateDeep (ASTNodeMemory &mem, const ListLiteral *literal)
{
  assert (literal != nullptr);
  Literal::__duplicateDeep (mem, literal);
  if (literal->value.get () == nullptr)
    { value.reset (nullptr); }
  else
    {
      value.reset (new std::vector<Literal *>{});
      value->reserve (literal->value->size ());
      std::transform (
          literal->value->cbegin (), literal->value->cend (),
          std::back_inserter ((*value)),
          [&] (Literal *paramLiteral) -> Literal *
          { return static_cast<Literal *>(paramLiteral->duplicateDeep (mem)); }
      );
    }
}

}
}