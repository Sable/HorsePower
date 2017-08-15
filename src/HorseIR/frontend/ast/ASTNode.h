#pragma once

#include <ostream>
#include "AST.h"

namespace horseIR
{
namespace ast
{

class ASTNode {
 public:
  typedef antlr4::tree::ParseTree CSTType;

  enum class ASTNodeClass {
    NilLiteral, ComplexLiteral, BoolLiteral, CharLiteral, Integer8Literal,
    Integer16Literal, Integer32Literal, Integer64Literal, FP32Literal,
    FP64Literal, SymbolLiteral, StringLiteral, MonthLiteral, SecondLiteral,
    TimeLiteral, DateLiteral, DateTimeLiteral, MinuteLiteral,
    FunctionLiteral, ListLiteral, DictionaryLiteral, TableLiteral,
    KeyTableLiteral, EnumerationLiteral, Identifier, PrimitiveType,
    WildcardType, ListType, DictionaryType, EnumerationType, FunctionType,
    CompilationUnit, Module, Method, LabelStatement, BranchStatement,
    PhiStatement, InvokeStatement, AssignStatement, ReturnStatement
  };

  class ASTNodeMemory {
   public:
    ASTNodeMemory () = default;
    ASTNodeMemory (ASTNodeMemory &&astNodeMemory) = default;
    ASTNodeMemory (const ASTNodeMemory &astNodeMemory) = delete;
    ASTNodeMemory &operator= (ASTNodeMemory &&astNodeMemory) = default;
    ASTNodeMemory &operator= (const ASTNodeMemory &astNodeMemory) = delete;
    ~ASTNodeMemory () = default;

    template<class T, class ...Args>
    std::enable_if_t<
        std::is_base_of<ASTNode, T>::value &&
        std::is_constructible<T, Args...>::value,
        T *>
    alloc (Args &&... args);

   protected:
    std::vector<std::unique_ptr<const ASTNode>> pool;
    void manage (const ast::ASTNode *managedPtr);
    void release (const ast::ASTNode *releasedPtr);
  };

  explicit ASTNode (const ASTNodeClass &p_astNodeClass);
  ASTNode (const ASTNodeClass &p_astNodeClass, const CSTType *parseTree);
  ASTNode (ASTNode &&externASTNode) = default;
  ASTNode (const ASTNode &externASTNode) = default;
  ASTNode &operator= (ASTNode &&externASTNode) = delete;
  ASTNode &operator= (const ASTNode &externASTNode) = delete;
  virtual ~ASTNode () = default;

  const CSTType *getCST () const;
  ASTNodeClass getASTNodeClass () const;
  std::string getEnclosingFilename () const;
  ASTNode *getParentASTNode () const;

  void setCST (const CSTType *p_cst);
  template<class T>
  std::enable_if_t<std::is_assignable<std::string, T>::value>
  setEnclosingFilename (T &&filename);
  void setParentASTNode (ast::ASTNode *p_parent);

  virtual std::size_t getNumNodesRecursively () const = 0;
  virtual std::vector<ASTNode *> getChildren () const = 0;
  virtual ASTNode *duplicateDeep (ASTNode::ASTNodeMemory &mem) const = 0;
  virtual std::string toString () const = 0;

 protected:
  const CSTType *cst;
  const ASTNodeClass astNodeClass;
  std::string enclosingFilename = "unknown";
  ast::ASTNode *parent;

  void __duplicateDeep (ASTNodeMemory &mem, const ASTNode *astNode);
};

template<class T, class ...Args>
inline std::enable_if_t<
    std::is_base_of<ASTNode, T>::value &&
    std::is_constructible<T, Args...>::value,
    T *>
ASTNode::ASTNodeMemory::alloc (Args &&...args)
{
  T* allocatedNode = new T (std::forward<Args> (args)...);
  manage (allocatedNode);
  return allocatedNode;
}

inline void ASTNode::ASTNodeMemory::manage (const ast::ASTNode *managedPtr)
{
  const auto searchItr = std::find_if (
      pool.cbegin (), pool.cend (),
      [&] (const std::unique_ptr<const ASTNode> &iterPtr) -> bool
      {
        return iterPtr.get () == managedPtr;
      });
  if (searchItr == pool.cend ()) pool.emplace_back (managedPtr);
}

inline void ASTNode::ASTNodeMemory::release (const ast::ASTNode *releasedPtr)
{
  const auto removeIter = std::remove_if (
      pool.begin (), pool.end (),
      [&] (const std::unique_ptr<const ASTNode> &iterPtr) -> bool
      {
        return iterPtr.get () == releasedPtr;
      });
  pool.erase (removeIter, pool.end ());
}

inline ASTNode::ASTNode (const ASTNodeClass &p_astNodeClass)
    : cst (nullptr), astNodeClass (p_astNodeClass), parent (nullptr)
{}

inline ASTNode::ASTNode (const ASTNodeClass &p_astNodeClass,
                         const CSTType *parseTree)
    : cst (parseTree), astNodeClass (p_astNodeClass), parent (nullptr)
{}

inline const ASTNode::CSTType *ASTNode::getCST () const
{ return cst; }

inline ASTNode::ASTNodeClass ASTNode::getASTNodeClass () const
{ return astNodeClass; }

inline std::string ASTNode::getEnclosingFilename () const
{ return enclosingFilename; }

inline ASTNode *ASTNode::getParentASTNode () const
{ return parent; }

inline void ASTNode::setCST (const CSTType *p_cst)
{ cst = p_cst; }

template<class T>
inline std::enable_if_t<std::is_assignable<std::string, T>::value>
ASTNode::setEnclosingFilename (T &&filename)
{ enclosingFilename = std::forward<T> (filename); };

inline void ASTNode::setParentASTNode (ast::ASTNode *p_parent)
{ parent = p_parent; }

inline void
ASTNode::__duplicateDeep (ASTNodeMemory &mem, const ASTNode *astNode)
{
  assert (astNode != nullptr);
  cst = astNode->cst;
  enclosingFilename = astNode->enclosingFilename;
  parent = astNode->parent;
}

inline std::ostream &
operator<< (std::ostream &stream, const ASTNode::ASTNodeClass &astNodeClass)
{
  using ASTNodeClass = ASTNode::ASTNodeClass;
  switch (astNodeClass)
    {
      case ASTNodeClass::NilLiteral: return stream << "NilLiteral";
      case ASTNodeClass::ComplexLiteral: return stream << "ComplexLiteral";
      case ASTNodeClass::BoolLiteral: return stream << "BoolLiteral";
      case ASTNodeClass::CharLiteral: return stream << "CharLiteral";
      case ASTNodeClass::Integer8Literal: return stream << "Integer8Literal";
      case ASTNodeClass::Integer16Literal: return stream << "Integer16Literal";
      case ASTNodeClass::Integer32Literal: return stream << "Integer32Literal";
      case ASTNodeClass::Integer64Literal: return stream << "Integer64Literal";
      case ASTNodeClass::FP32Literal: return stream << "FP32Literal";
      case ASTNodeClass::FP64Literal: return stream << "FP64Literal";
      case ASTNodeClass::SymbolLiteral: return stream << "SymbolLiteral";
      case ASTNodeClass::StringLiteral: return stream << "StringLiteral";
      case ASTNodeClass::DateLiteral: return stream << "DateLiteral";
      case ASTNodeClass::DateTimeLiteral: return stream << "DateTimeLiteral";
      case ASTNodeClass::MinuteLiteral: return stream << "MinuteLiteral";
      case ASTNodeClass::MonthLiteral:return stream << "MonthLiteral";
      case ASTNodeClass::SecondLiteral: return stream << "SecondLiteral";
      case ASTNodeClass::TimeLiteral: return stream << "TimeLiteral";
      case ASTNodeClass::FunctionLiteral: return stream << "FunctionLiteral";
      case ASTNodeClass::ListLiteral: return stream << "ListLiteral";
      case ASTNodeClass::DictionaryLiteral :
        return stream << "DictionaryLiteral";
      case ASTNodeClass::TableLiteral: return stream << "TableLiteral";
      case ASTNodeClass::KeyTableLiteral: return stream << "KeyTableLiteral";
      case ASTNodeClass::EnumerationLiteral:
        return stream << "EnumerationLiteral";
      case ASTNodeClass::Identifier: return stream << "Identifier";
      case ASTNodeClass::PrimitiveType: return stream << "PrimitiveType";
      case ASTNodeClass::WildcardType: return stream << "WildcardType";
      case ASTNodeClass::ListType: return stream << "ListType";
      case ASTNodeClass::DictionaryType: return stream << "DictionaryType";
      case ASTNodeClass::EnumerationType: return stream << "EnumerationType";
      case ASTNodeClass::FunctionType: return stream << "FunctionType";
      case ASTNodeClass::CompilationUnit: return stream << "CompilationUnit";
      case ASTNodeClass::Module: return stream << "Module";
      case ASTNodeClass::Method: return stream << "Method";
      case ASTNodeClass::LabelStatement: return stream << "LabelStatement";
      case ASTNodeClass::BranchStatement: return stream << "BranchStatement";
      case ASTNodeClass::PhiStatement: return stream << "PhiStatement";
      case ASTNodeClass::InvokeStatement: return stream << "InvokeStatement";
      case ASTNodeClass::AssignStatement: return stream << "AssignStatement";
      case ASTNodeClass::ReturnStatement: return stream << "ReturnStatement";
    }
}

}
}