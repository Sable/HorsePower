#pragma once

#include "AST.h"

namespace horseIR
{
namespace ast
{

class CompilationUnit : public ASTNode {
 public:
  using ModuleIterator = std::vector<Module *>::iterator;
  using ModuleConstIterator = std::vector<Module *>::const_iterator;

  CompilationUnit ();
  explicit CompilationUnit (const CSTType *cst);
  CompilationUnit (CompilationUnit &&unit) = default;
  CompilationUnit (const CompilationUnit &unit) = default;
  CompilationUnit &operator= (CompilationUnit &&unit) = delete;
  CompilationUnit &operator= (const CompilationUnit &unit) = delete;
  ~CompilationUnit () override = default;

  std::vector<Module *> getModules () const;
  ModuleIterator modulesBegin ();
  ModuleIterator modulesEnd ();
  ModuleConstIterator modulesConstBegin () const;
  ModuleConstIterator modulesConstEnd () const;

  template<class T>
  std::enable_if_t<std::is_assignable<std::vector<Module *>, T>::value>
  setModules (T &&newModules);

  std::size_t getNumNodesRecursively () const override;
  std::vector<ASTNode *> getChildren () const override;
  CompilationUnit *duplicateDeep (ASTNodeMemory &mem) const override;

 protected:
  std::vector<Module *> modules{};

  void __duplicateDeep (ASTNodeMemory &mem, const CompilationUnit *unit);
};

inline CompilationUnit::CompilationUnit ()
    : ASTNode (ASTNodeClass::CompilationUnit)
{}

inline CompilationUnit::CompilationUnit (const CSTType *cst)
    : ASTNode (ASTNodeClass::CompilationUnit, cst)
{}

inline std::vector<Module *> CompilationUnit::getModules () const
{ return modules; }

inline CompilationUnit::ModuleIterator CompilationUnit::modulesBegin ()
{ return modules.begin (); }

inline CompilationUnit::ModuleIterator CompilationUnit::modulesEnd ()
{ return modules.end (); }

inline CompilationUnit::ModuleConstIterator
CompilationUnit::modulesConstBegin () const
{ return modules.cbegin (); }

inline CompilationUnit::ModuleConstIterator
CompilationUnit::modulesConstEnd () const
{ return modules.cend (); }

template<class T>
inline std::enable_if_t<std::is_assignable<std::vector<Module *>, T>::value>
CompilationUnit::setModules (T &&newModules)
{
  modules = std::forward<T> (newModules);
  for (const auto &module : modules)
    if (module != nullptr) module->setParentASTNode (this);
}

inline std::size_t CompilationUnit::getNumNodesRecursively () const
{
  std::size_t counter = 1;
  for (const auto &module : modules)
    if (module != nullptr) counter += module->getNumNodesRecursively ();
  return counter;
}

inline std::vector<ASTNode *> CompilationUnit::getChildren () const
{
  std::vector<ASTNode *> childrenNodes{};
  for (const auto &module: modules)
    if (module != nullptr)
      { childrenNodes.push_back (static_cast<ASTNode *>(module)); }
  return childrenNodes;
}

inline CompilationUnit *
CompilationUnit::duplicateDeep (ASTNodeMemory &mem) const
{
  auto compilationUnit = mem.alloc<CompilationUnit> ();
  compilationUnit->__duplicateDeep (mem, this);
  return compilationUnit;
}

inline void CompilationUnit::__duplicateDeep (ASTNodeMemory &mem,
                                              const CompilationUnit *unit)
{
  assert (unit != nullptr);
  std::vector<Module *> duplicateModules{};
  for (const auto &module : unit->modules)
    {
      if (module == nullptr)
        { duplicateModules.push_back (nullptr); }
      else
        {
          Module *duplicateModule = module->duplicateDeep (mem);
          duplicateModule->setParentASTNode (this);
          duplicateModules.push_back (duplicateModule);
        }
    }
  modules = std::move (duplicateModules);
}

}
}