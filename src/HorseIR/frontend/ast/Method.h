#pragma once

#include "AST.h"

namespace horseIR
{
namespace ast
{

class Method : public ASTNode {
 public:
  using ParameterIterator = std::vector<
      std::pair<Identifier *, Type *>
  >::iterator;
  using ParameterConstIterator = std::vector<
      std::pair<Identifier *, Type *>
  >::const_iterator;
  using StatementIterator = std::vector<Statement *>::iterator;
  using StatementConstIterator = std::vector<Statement *>::const_iterator;

  Method ();
  explicit Method (const CSTType *cst);
  Method (Method &&method) = default;
  Method (const Method &method) = default;
  Method &operator= (Method &&method) = delete;
  Method &operator= (const Method &method) = delete;
  ~Method () override = default;

  std::string getMethodName () const;
  template<class T>
  std::enable_if_t<std::is_assignable<std::string, T>::value>
  setMethodName (T &&newMethodName);

  std::vector<std::pair<Identifier *, Type *>> getParameters () const;
  ParameterIterator parametersBegin ();
  ParameterIterator parametersEnd ();
  ParameterConstIterator parametersConstBegin () const;
  ParameterConstIterator parametersConstEnd () const;
  template<class T>
  std::enable_if_t<
      std::is_assignable<std::vector<std::pair<Identifier *, Type *>>, T>::value
  >
  setParameters (T &&newParameters);

  Type *getReturnType () const;
  void setReturnType (Type *type);

  std::vector<Statement *> getStatements () const;
  StatementIterator statementsBegin ();
  StatementIterator statementsEnd ();
  StatementConstIterator statementsConstBegin () const;
  StatementConstIterator statementsConstEnd () const;
  template<class T>
  std::enable_if_t<std::is_assignable<std::vector<Statement *>, T>::value>
  setStatements (T &&newStatements);

  std::size_t getNumNodesRecursively () const override;
  std::vector<ASTNode *> getChildren () const override;
  Method *duplicateDeep (ASTNodeMemory &mem) const override;

 protected:
  std::string methodName{};
  std::vector<std::pair<Identifier *, Type *>> parameters{};
  Type *returnType = nullptr;
  std::vector<Statement *> statements{};

  void __duplicateDeep (ASTNodeMemory &mem, const Method *method);
};

inline Method::Method ()
    : ASTNode (ASTNodeClass::Method)
{}

inline Method::Method (const CSTType *cst)
    : ASTNode (ASTNodeClass::Method, cst)
{}

inline std::string Method::getMethodName () const
{ return methodName; }

template<class T>
inline std::enable_if_t<std::is_assignable<std::string, T>::value>
Method::setMethodName (T &&newMethodName)
{ methodName = std::forward<T> (newMethodName); }

inline std::vector<std::pair<Identifier *, Type *>>
Method::getParameters () const
{ return parameters; }

inline Method::ParameterIterator Method::parametersBegin ()
{ return parameters.begin (); }

inline Method::ParameterIterator Method::parametersEnd ()
{ return parameters.end (); }

inline Method::ParameterConstIterator Method::parametersConstBegin () const
{ return parameters.cbegin (); }

inline Method::ParameterConstIterator Method::parametersConstEnd () const
{ return parameters.cend (); }

template<class T>
inline std::enable_if_t<
    std::is_assignable<std::vector<std::pair<Identifier *, Type *>>, T>::value
>
Method::setParameters (T &&newParameters)
{
  parameters = std::forward<T> (newParameters);
  for (std::pair<Identifier *, Type *> &parameter : parameters)
    {
      if (parameter.first != nullptr)
        { parameter.first->setParentASTNode (this); }
      if (parameter.second != nullptr)
        { parameter.second->setParentASTNode (this); }
    }
}

inline Type *Method::getReturnType () const
{ return returnType; }

inline void Method::setReturnType (Type *type)
{
  returnType = type;
  if (returnType != nullptr) returnType->setParentASTNode (this);
}

inline std::vector<Statement *> Method::getStatements () const
{ return statements; }

inline Method::StatementIterator Method::statementsBegin ()
{ return statements.begin (); }

inline Method::StatementIterator Method::statementsEnd ()
{ return statements.end (); }

inline Method::StatementConstIterator Method::statementsConstBegin () const
{ return statements.cbegin (); }

inline Method::StatementConstIterator Method::statementsConstEnd () const
{ return statements.cend (); }

template<class T>
inline std::enable_if_t<std::is_assignable<std::vector<Statement *>, T>::value>
Method::setStatements (T &&newStatements)
{
  statements = std::forward<T> (newStatements);
  for (Statement *&statement : statements)
    { if (statement != nullptr) statement->setParentASTNode (this); }
}

inline std::size_t Method::getNumNodesRecursively () const
{
  std::size_t counter = 1;
  for (const auto &parameter : parameters)
    {
      if (parameter.first != nullptr)
        { counter = counter + parameter.first->getNumNodesRecursively (); }
      if (parameter.second != nullptr)
        { counter = counter + parameter.second->getNumNodesRecursively (); }
    }
  if (returnType != nullptr) counter += returnType->getNumNodesRecursively ();
  for (const auto &statement : statements)
    if (statement != nullptr)
      { counter = counter + statement->getNumNodesRecursively (); }
  return counter;
}

inline std::vector<ASTNode *> Method::getChildren () const
{
  std::vector<ASTNode *> childrenNodes{};
  childrenNodes.reserve (1 + parameters.size () + statements.size ());
  for (const auto &parameter : parameters)
    {
      if (parameter.first != nullptr)
        { childrenNodes.push_back (static_cast<ASTNode *>(parameter.first)); }
      if (parameter.second != nullptr)
        { childrenNodes.push_back (static_cast<ASTNode *>(parameter.second)); }
    }
  if (returnType != nullptr)
    { childrenNodes.push_back (static_cast<ASTNode *>(returnType)); }
  for (const auto &statement : statements)
    if (statement != nullptr)
      { childrenNodes.push_back (static_cast<ASTNode *>(statement)); }
  return childrenNodes;
}

inline Method *Method::duplicateDeep (ASTNodeMemory &mem) const
{
  auto method = mem.alloc<Method> ();
  method->__duplicateDeep (mem, this);
  return method;
}

inline void Method::__duplicateDeep (ASTNodeMemory &mem, const Method *method)
{
  assert (method != nullptr);
  ASTNode::__duplicateDeep (mem, this);
  methodName = method->methodName;
  std::vector<std::pair<Identifier *, Type *>> duplicateParameters{};
  duplicateParameters.reserve (method->parameters.size ());
  std::transform (
      method->parameters.cbegin (), method->parameters.cend (),
      std::back_inserter (duplicateParameters),
      [&] (const auto &parameter) -> std::pair<Identifier *, Type *>
      {
        Identifier *duplicateIdentifier = nullptr;
        if (parameter.first != nullptr)
          {
            duplicateIdentifier = parameter.first->duplicateDeep (mem);
            duplicateIdentifier->setParentASTNode (this);
          }
        Type *duplicateType = nullptr;
        if (parameter.second != nullptr)
          {
            duplicateType = dynamic_cast<Type *>(
                parameter.second->duplicateDeep (mem)
            );
            duplicateType->setParentASTNode (this);
          }
        return std::make_pair (duplicateIdentifier, duplicateType);
      });
  parameters = std::move (duplicateParameters);
  Type *duplicateRetType = nullptr;
  if (method->returnType != nullptr)
    {
      duplicateRetType = dynamic_cast<Type *>(
          method->returnType->duplicateDeep (mem)
      );
      duplicateRetType->setParentASTNode (this);
    }
  returnType = duplicateRetType;
  std::vector<Statement *> duplicateStatements{};
  duplicateStatements.reserve (method->statements.size ());
  std::transform (
      method->statements.cbegin (), method->statements.cend (),
      std::back_inserter (duplicateStatements),
      [&] (Statement *statement) -> Statement *
      {
        if (statement == nullptr) return nullptr;
        Statement *duplicateStatement = dynamic_cast<Statement *>(
            statement->duplicateDeep (mem)
        );
        duplicateStatement->setParentASTNode (this);
        return duplicateStatement;
      });
  statements = std::move (duplicateStatements);
}

}
}