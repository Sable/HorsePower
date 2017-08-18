#pragma once

#include "../misc/InfixOStreamIterator.h"
#include "AST.h"

namespace horseIR
{
namespace ast
{

class Module : public ASTNode {
 public:
  using GlobalVariableEntryType = std::pair<Identifier *, Type *>;
  using ImportedModuleEntryType = std::pair<std::string, std::string>;

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
  template<class T>
  std::enable_if_t<std::is_assignable<std::vector<Method *>, T>::value>
  setMethods (T &&newMethods);

  std::vector<GlobalVariableEntryType> getGlobalVariables () const;
  template<class T>
  std::enable_if_t<
      std::is_assignable<std::vector<GlobalVariableEntryType>, T>::value
  >
  setGlobalVariables (T &&newGlobalVariables);

  std::vector<ImportedModuleEntryType> getImportedModules () const;
  template<class T>
  std::enable_if_t<
      std::is_assignable<std::vector<ImportedModuleEntryType>, T>::value
  >
  setImportedModules (T &&newImportedModules);

  std::size_t getNumNodesRecursively () const override;
  std::vector<ASTNode *> getChildren () const override;
  Module *duplicateDeep (ASTNodeMemory &mem) const override;
  std::string toString () const override;

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

template<class T>
inline std::enable_if_t<
    std::is_assignable<std::vector<Module::ImportedModuleEntryType>, T>::value
>
Module::setImportedModules (T &&newImportedModues)
{ importedModules = std::forward<T> (newImportedModues); }

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

inline std::string Module::toString () const
{
  std::ostringstream stream;
  stream << "module "
         << moduleName << " {" << HORSEIR_AST_PRETTY_PRINT_LINE_BREAK;
  std::transform (
      importedModules.cbegin (), importedModules.cend (),
      std::ostream_iterator<std::string> (
          stream, HORSEIR_AST_PRETTY_PRINT_LINE_BREAK
      ),
      [] (const ImportedModuleEntryType &importedModule) -> std::string
      {
        std::ostringstream s;
        s << HORSEIR_AST_PRETTY_PRINT_INDENT << "import "
          << importedModule.first << '.' << importedModule.second << ';';
        return s.str ();
      });
  std::transform (
      globalVariables.cbegin (), globalVariables.cend (),
      std::ostream_iterator<std::string> (
          stream, HORSEIR_AST_PRETTY_PRINT_LINE_BREAK
      ),
      [] (const GlobalVariableEntryType &globalVariable) -> std::string
      {
        Identifier *const id = globalVariable.first;
        Type *const type = globalVariable.second;
        std::ostringstream s;
        s << HORSEIR_AST_PRETTY_PRINT_INDENT << "def "
          << ((id == nullptr) ? "nullptr" : id->toString ())
          << " :"
          << ((type == nullptr) ? "nullptr" : type->toString ())
          << ';';
        return s.str ();
      });
  std::transform (
      methods.cbegin (), methods.cend (),
      std::ostream_iterator<std::string> (stream),
      [] (Method *method) -> std::string
      {
        std::ostringstream s;
        if (method == nullptr)
          {
            s << HORSEIR_AST_PRETTY_PRINT_INDENT << "nullptr";
            return s.str ();
          }
        std::string buffer;
        std::istringstream iStream (method->toString ());
        while (std::getline (iStream, buffer, '\n'))
          {
            s << HORSEIR_AST_PRETTY_PRINT_INDENT
              << buffer
              << HORSEIR_AST_PRETTY_PRINT_LINE_BREAK;
          }
        return s.str ();
      });
  stream << '}';
  return stream.str ();
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