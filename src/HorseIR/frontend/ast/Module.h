#pragma once

#include "AST.h"

namespace horseIR
{
namespace ast
{

class Module : public ASTNode {
 public:
  using GlobalVariableEntryType = std::pair<Identifier *, Type *>;
  using ImportedModuleEntryType = std::pair<std::string, std::string>;

  using MethodIterator = std::vector<Method *>::iterator;
  using MethodConstIterator = std::vector<Method *>::const_iterator;
  using GlobalVariableIterator = std::vector<GlobalVariableEntryType>::iterator;
  using GlobalVariableConstIterator = std::vector<
      GlobalVariableEntryType
  >::const_iterator;
  using ImportedModuleIterator = std::vector<ImportedModuleEntryType>::iterator;
  using ImportedModuleConstIterator = std::vector<
      ImportedModuleEntryType
  >::const_iterator;

  Module ();
  explicit Module (const CSTType *cst);
  Module (Module &&module) = default;
  Module (const Module &module) = default;
  Module &operator= (Module &&module) = delete;
  Module &operator= (const Module &module) = delete;
  ~Module () override = default;

  std::string getModuleName () const;
  template<class T>
  std::enable_if_t<std::is_assignable<std::string, T>::value>
  setModuleName (T &&newModuleName);

  std::vector<Method *> getMethods () const;
  MethodIterator methodsBegin ();
  MethodIterator methodsEnd ();
  MethodConstIterator methodsConstBegin () const;
  MethodConstIterator methodsConstEnd () const;
  template<class T>
  std::enable_if_t<std::is_assignable<std::vector<Method *>, T>::value>
  setMethods (T &&newMethods);

  std::vector<GlobalVariableEntryType> getGlobalVariables () const;
  GlobalVariableIterator globalVariablesBegin ();
  GlobalVariableIterator globalVariablesEnd ();
  GlobalVariableConstIterator globalVariablesConstBegin () const;
  GlobalVariableConstIterator globalVariablesConstEnd () const;
  template<class T>
  std::enable_if_t<
      std::is_assignable<std::vector<GlobalVariableEntryType>, T>::value
  >
  setGlobalVariables (T &&newGlobalVariables);

  std::vector<ImportedModuleEntryType> getImportedModules () const;
  ImportedModuleIterator importedModulesBegin ();
  ImportedModuleIterator importedModulesEnd ();
  ImportedModuleConstIterator importedModulesConstBegin () const;
  ImportedModuleConstIterator importedModulesConstEnd () const;
  template<class T>
  std::enable_if_t<
      std::is_assignable<std::vector<ImportedModuleEntryType>, T>::value
  >
  setImportedModules (T &&newImportedModules);

  void merge (Module &&module);

  std::size_t getNumNodesRecursively () const override;
  std::vector<ASTNode *> getChildren () const override;
  Module *duplicateDeep (ASTNodeMemory &mem) const override;

 protected:
  std::string moduleName{};
  std::vector<Method *> methods{};
  std::vector<GlobalVariableEntryType> globalVariables{};
  std::vector<ImportedModuleEntryType> importedModules{};

  void __duplicateDeep (ASTNodeMemory &mem, const Module *module);
};

inline Module::Module ()
    : ASTNode (ASTNodeClass::Module)
{}

inline Module::Module (const CSTType *cst)
    : ASTNode (ASTNodeClass::Module, cst)
{}

inline std::string Module::getModuleName () const
{ return moduleName; }

template<class T>
inline std::enable_if_t<std::is_assignable<std::string, T>::value>
Module::setModuleName (T &&newModuleName)
{ moduleName = std::forward<T> (newModuleName); }

inline std::vector<Method *> Module::getMethods () const
{ return methods; }

inline Module::MethodIterator Module::methodsBegin ()
{ return methods.begin (); }

inline Module::MethodIterator Module::methodsEnd ()
{ return methods.end (); }

inline Module::MethodConstIterator Module::methodsConstBegin () const
{ return methods.cbegin (); }

inline Module::MethodConstIterator Module::methodsConstEnd () const
{ return methods.cend (); }

template<class T>
inline std::enable_if_t<std::is_assignable<std::vector<Method *>, T>::value>
Module::setMethods (T &&newMethods)
{
  methods = std::forward<T> (newMethods);
  for (const auto &method : methods)
    if (method != nullptr) method->setParentASTNode (this);
}

inline std::vector<Module::GlobalVariableEntryType>
Module::getGlobalVariables () const
{ return globalVariables; }

inline Module::GlobalVariableIterator Module::globalVariablesBegin ()
{ return globalVariables.begin (); }

inline Module::GlobalVariableIterator Module::globalVariablesEnd ()
{ return globalVariables.end (); }

inline Module::GlobalVariableConstIterator
Module::globalVariablesConstBegin () const
{ return globalVariables.cbegin (); }

inline Module::GlobalVariableConstIterator
Module::globalVariablesConstEnd () const
{ return globalVariables.cend (); }

template<class T>
inline std::enable_if_t<
    std::is_assignable<std::vector<Module::GlobalVariableEntryType>, T>::value
>
Module::setGlobalVariables (T &&newGlobalVariables)
{
  globalVariables = std::forward<T> (newGlobalVariables);
  for (const GlobalVariableEntryType &globalVariable : globalVariables)
    {
      if (globalVariable.first != nullptr)
        { globalVariable.first->setParentASTNode (this); }
      if (globalVariable.second != nullptr)
        { globalVariable.second->setParentASTNode (this); }
    }
}

inline std::vector<Module::ImportedModuleEntryType>
Module::getImportedModules () const
{ return importedModules; }

inline Module::ImportedModuleIterator Module::importedModulesBegin ()
{ return importedModules.begin (); }

inline Module::ImportedModuleIterator Module::importedModulesEnd ()
{ return importedModules.end (); }

inline Module::ImportedModuleConstIterator
Module::importedModulesConstBegin () const
{ return importedModules.cbegin (); }

inline Module::ImportedModuleConstIterator
Module::importedModulesConstEnd () const
{ return importedModules.cend (); }

template<class T>
inline std::enable_if_t<
    std::is_assignable<std::vector<Module::ImportedModuleEntryType>, T>::value
>
Module::setImportedModules (T &&newImportedModues)
{ importedModules = std::forward<T> (newImportedModues); }

void Module::merge (Module &&module)
{
  if (module.moduleName != moduleName) return;
  for (const auto &method : module.methods)
    { methods.push_back (method); }
  for (auto &globalVariable : module.globalVariables)
    { globalVariables.emplace_back (std::move (globalVariable)); }
  for (auto &importedModule : module.importedModules)
    { importedModules.emplace_back (std::move (importedModule)); }
  module.methods.clear ();
  module.globalVariables.clear ();
  module.importedModules.clear ();
}

inline std::size_t Module::getNumNodesRecursively () const
{
  std::size_t counter = 1;
  for (const auto &method : methods)
    { if (method != nullptr) counter += method->getNumNodesRecursively (); }
  for (const auto &globalVariable : globalVariables)
    {
      if (globalVariable.first != nullptr)
        { counter += globalVariable.first->getNumNodesRecursively (); }
      if (globalVariable.second != nullptr)
        { counter += globalVariable.second->getNumNodesRecursively (); }
    }
  return counter;
}

inline std::vector<ASTNode *> Module::getChildren () const
{
  std::vector<ASTNode *> childrenNodes{};
  for (const auto &method : methods)
    if (method != nullptr)
      { childrenNodes.push_back (static_cast<ASTNode *>(method)); }
  for (const auto &globalVariable : globalVariables)
    {
      if (globalVariable.first != nullptr)
        childrenNodes.push_back (static_cast<ASTNode *>(globalVariable.first));
      if (globalVariable.second != nullptr)
        childrenNodes.push_back (static_cast<ASTNode *>(globalVariable.second));
    }
  return childrenNodes;
}

inline Module *Module::duplicateDeep (ASTNode::ASTNodeMemory &mem) const
{
  auto module = mem.alloc<Module> ();
  module->__duplicateDeep (mem, this);
  return module;
}

inline void Module::__duplicateDeep (ASTNodeMemory &mem, const Module *module)
{
  assert (module != nullptr);
  ASTNode::__duplicateDeep (mem, module);
  moduleName = module->moduleName;
  std::vector<Method *> duplicateMethods{};
  std::transform (
      module->methods.cbegin (), module->methods.cend (),
      std::back_inserter (duplicateMethods),
      [&] (Method *method) -> Method *
      {
        if (method == nullptr) return nullptr;
        Method *duplicateMethod = method->duplicateDeep (mem);
        duplicateMethod->setParentASTNode (this);
        return duplicateMethod;
      });
  methods = std::move (duplicateMethods);
  std::vector<GlobalVariableEntryType> duplicateGlobalVariables{};
  std::transform (
      module->globalVariables.cbegin (), module->globalVariables.cend (),
      std::back_inserter (duplicateGlobalVariables),
      [&] (const GlobalVariableEntryType &variable) -> GlobalVariableEntryType
      {
        Identifier *id = nullptr;
        Type *type = nullptr;
        if (variable.first != nullptr)
          {
            id = variable.first->duplicateDeep (mem);
            id->setParentASTNode (this);
          }
        if (variable.second != nullptr)
          {
            type = dynamic_cast<Type *>(variable.second->duplicateDeep (mem));
            type->setParentASTNode (this);
          }
        return std::make_pair (id, type);
      });
  globalVariables = std::move (duplicateGlobalVariables);
  importedModules = module->importedModules;
}

}
}