#pragma once

#include <stdexcept>

#include "../ast/AST.h"
#include "../../backend/h_global.h"

namespace horseIR
{
namespace interpreter
{

class Dispatcher {
 public:
  class MethodMETA {
   public:
    enum class MethodMETAClass { Internal, External };
    virtual MethodMETAClass getMethodMETAClass () const = 0;

    std::string getModuleName () const
    { return moduleName; }

    template<class T>
    std::enable_if_t<std::is_assignable<std::string, T>::value>
    setModuleName (T &&newModuleName)
    { moduleName = std::forward<T> (newModuleName); }

    std::string getMethodName () const
    { return methodName; }

    template<class T>
    std::enable_if_t<std::is_assignable<std::string, T>::value>
    setMethodName (T &&newMethodName)
    { methodName = std::forward<T> (newMethodName); }

   protected:
    std::string moduleName;
    std::string methodName;
  };

  class InternalMethodMETA : public MethodMETA {
   public:
    MethodMETAClass getMethodMETAClass () const override
    { return MethodMETAClass::Internal; }

    const ast::Method *getMethodAST () const
    { return target; }

    void setMethodAST (const ast::Method *targetMethod)
    { target = targetMethod; }

   protected:
    const ast::Method *target = nullptr;
  };

  class ExternalMethodMETA : public MethodMETA {
   public:
    MethodMETAClass getMethodMETAClass () const override
    { return MethodMETAClass::External; }

    virtual V invoke (std::size_t argc, V argv[]) = 0;
  };

  class __ExternalMethodMETA : public ExternalMethodMETA {
   public:
    explicit __ExternalMethodMETA (V(*pInvokeFunction) (std::size_t, V[]))
        : invokeFunction (pInvokeFunction)
    {}

    V invoke (std::size_t argc, V argv[]) override
    { return invokeFunction (argc, argv); }

   protected:
    V (*invokeFunction) (std::size_t, V[]) = nullptr;
  };

  void addExternalMethod (const std::string &moudleName,
                          const std::string &methodName,
                          V(*invokeFunction) (std::size_t, V[]));

  //protected:
  std::vector<MethodMETA *> collectVisibleMethods (ast::Module *module);
  void addInternalMethod (ast::Module *module, ast::Method *method);

  void analysis (ast::CompilationUnit *unit);
  void analysis (ast::Module *module);
  void analysis (ast::Method *method,
                 const std::vector<MethodMETA *> &candidates);
  void analysis (ast::InvokeStatement *invokeStatement,
                 const std::vector<MethodMETA *> &candidates);
  void analysis (ast::AssignStatement *assignStatement,
                 const std::vector<MethodMETA *> &candidates);
  void analysis (ast::BranchStatement *branchStatement,
                 const std::vector<MethodMETA *> &candidates);
  void analysis (ast::ReturnStatement *returnStatement,
                 const std::vector<MethodMETA *> &candidates);
  void analysis (ast::PhiStatement *phiStatement,
                 const std::vector<MethodMETA *> &candidates);
  void analysis (ast::FunctionLiteral *functionLiteral,
                 const std::vector<MethodMETA *> &candidates);

  std::vector<std::unique_ptr<MethodMETA>> candidateMethods;
  std::map<ast::InvokeStatement *, MethodMETA *> invokeStatementMap;
  std::map<ast::FunctionLiteral *,
           std::vector<MethodMETA *>> functionLiteralMap;
};

}
}
