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
    ASTNodeMemory &manage (ast::ASTNode *managedPtr);
    ASTNodeMemory &release (ast::ASTNode *releasedPtr);

   protected:
    std::vector<std::unique_ptr<ASTNode>> pool;
  };

  ASTNode (ASTNodeMemory &mem, const ASTNodeClass &p_astNodeClass)
      : cst (nullptr), astNodeClass (p_astNodeClass), parent (nullptr)
  {
    mem.manage (this);
  }
  ASTNode (ASTNodeMemory &mem, const ASTNodeClass &p_astNodeClass,
           const CSTType *parseTree)
      : cst (parseTree), astNodeClass (p_astNodeClass), parent (nullptr)
  {
    mem.manage (this);
  }
  ASTNode (ASTNodeMemory &mem, const ASTNodeClass &p_astNodeClass,
           const CSTType *parseTree, ASTNode *p_parent)
      : cst (parseTree), astNodeClass (p_astNodeClass), parent (p_parent)
  {
    mem.manage (this);
  }
  ASTNode (ASTNode &&externASTNode) = default;
  ASTNode (const ASTNode &externASTNode) = default;
  ASTNode &operator= (ASTNode &&externASTNode) = delete;
  ASTNode &operator= (const ASTNode &externASTNode) = delete;
  virtual ~ASTNode () = default;

  virtual std::size_t getNumNodesRecursively () const = 0;
  virtual std::vector<ASTNode *> getChildren () const = 0;
  virtual ASTNode *duplicateDeep (ASTNode::ASTNodeMemory &mem) const = 0;

  const CSTType *getCST () const
  {
    return cst;
  }
  ASTNodeClass getASTNodeClass () const
  {
    return astNodeClass;
  }
  std::string getEnclosingFilename () const
  {
    return enclosingFilename;
  }
  ASTNode *getParentASTNode () const
  {
    return parent;
  }

  virtual std::string toString () const = 0;
 protected:
  static const char *PRETTY_PRINT_INDENT = "    ";
  const CSTType *cst;
  const ASTNodeClass astNodeClass;
  std::string enclosingFilename = "unknown";
  ast::ASTNode *parent;
};

inline ASTNode::ASTNodeMemory &
ASTNode::ASTNodeMemory::manage (ast::ASTNode *managedPtr)
{
  auto findIter = std::find (pool.cbegin (), pool.cend (), managedPtr);
  if (findIter == pool.cend ()) pool.push_back (std::make_unique (managedPtr));
  return *this;
}

inline ASTNode::ASTNodeMemory &
ASTNode::ASTNodeMemory::release (ast::ASTNode *releasedPtr)
{
  const auto findIter = std::find (pool.cbegin (), pool.cend (), releasedPtr);
  if (findIter != pool.cend ())
    {
      pool.erase (std::remove (pool.begin (), pool.end (), releasedPtr),
                  pool.end ());
    }
  return *this;
}

}
}