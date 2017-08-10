#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <antlr4-runtime.h>

namespace horseIR
{
namespace ast
{

class ASTNode {
 public:
  typedef antlr4::tree::ParseTree CSTType;

  enum class ASTNodeClass {
    NilLiteral, ComplexLiteral, BoolLiteral, CharLiteral, Integer8Literal,
    Integer16Literal, Integer32Literal, Integer64Literal, SymbolLiteral,
    FunctionLiteral, TableLiteral, Identifier, PrimitiveType, WildcardType,
    ListType, DictionaryType, EnumerationType, FunctionType,
    CompilationUnit, Module, Method, LabelStatement, BranchStatement,
    PhiStatement, InvokeStatement, AssignStatement, ReturnStatement
  };

  class ASTNodeMemory {
   public:
    ASTNodeMemory &manage (const ast::ASTNode *managedPtr);
    ASTNodeMemory &release (const ast::ASTNode *releasedPtr);

   protected:
    std::vector<std::unique_ptr<const ASTNode>> pool;
  };

  ASTNode (ASTNodeMemory &mem, const ASTNodeClass &p_astNodeClass);
  ASTNode (ASTNodeMemory &mem, const ASTNodeClass &p_astNodeClass,
           const CSTType *parseTree);
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
  void setEnclosingFilename (const std::string &filename);
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

inline ASTNode::ASTNodeMemory &
ASTNode::ASTNodeMemory::manage (const ast::ASTNode *managedPtr)
{
  const auto searchItr = std::find_if (
      pool.cbegin (), pool.cend (),
      [&] (const std::unique_ptr<const ASTNode> &iterPtr) -> bool
      {
        return iterPtr.get () == managedPtr;
      });
  if (searchItr == pool.cend ()) pool.emplace_back (managedPtr);
  return *this;
}

inline ASTNode::ASTNodeMemory &
ASTNode::ASTNodeMemory::release (const ast::ASTNode *releasedPtr)
{
  const auto removeIter = std::remove_if (
      pool.begin (), pool.end (),
      [&] (const std::unique_ptr<const ASTNode> &iterPtr) -> bool
      {
        return iterPtr.get () == releasedPtr;
      });
  pool.erase (removeIter, pool.end ());
  return *this;
}

inline ASTNode::ASTNode (ASTNodeMemory &mem, const ASTNodeClass &p_astNodeClass)
    : cst (nullptr), astNodeClass (p_astNodeClass), parent (nullptr)
{
  (void) mem.manage (this);
}

inline ASTNode::ASTNode (ASTNodeMemory &mem, const ASTNodeClass &p_astNodeClass,
                         const CSTType *parseTree)
    : cst (parseTree), astNodeClass (p_astNodeClass), parent (nullptr)
{
  (void) mem.manage (this);
}

inline const ASTNode::CSTType *ASTNode::getCST () const
{
  return cst;
}

inline ASTNode::ASTNodeClass ASTNode::getASTNodeClass () const
{
  return astNodeClass;
}

inline std::string ASTNode::getEnclosingFilename () const
{
  return enclosingFilename;
}

inline ASTNode *ASTNode::getParentASTNode () const
{
  return parent;
}

inline void ASTNode::setCST (const CSTType *p_cst)
{
  cst = p_cst;
}

inline void ASTNode::setEnclosingFilename (const std::string &filename)
{
  enclosingFilename = filename;
}

inline void ASTNode::setParentASTNode (ast::ASTNode *p_parent)
{
  parent = p_parent;
}

inline void
ASTNode::__duplicateDeep (ASTNodeMemory &mem, const ASTNode *astNode)
{
  assert (astNode != nullptr);
  cst = astNode->cst;
  enclosingFilename = astNode->enclosingFilename;
  parent = astNode->parent;
}

}
}