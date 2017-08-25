#pragma once

#include <unordered_map>
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
    MethodMETA () = default;
    MethodMETA (MethodMETA &&methodMETA) = default;
    MethodMETA (const MethodMETA &methodMETA) = default;
    MethodMETA &operator= (MethodMETA &&methodMETA) = default;
    MethodMETA &operator= (const MethodMETA &methodMETA) = default;
    virtual ~MethodMETA () = default;
    enum class MethodMETAClass { Internal, External };

    virtual MethodMETAClass getMethodMETAClass () const = 0;

    std::string getMoudleName () const
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
    InternalMethodMETA () = default;
    InternalMethodMETA (InternalMethodMETA &&) = default;
    InternalMethodMETA (const InternalMethodMETA &) = default;
    InternalMethodMETA &operator= (InternalMethodMETA &&) = default;
    InternalMethodMETA &operator= (const InternalMethodMETA &) = default;
    ~InternalMethodMETA () override = default;

    MethodMETAClass getMethodMETAClass () const override
    { return MethodMETAClass::Internal; }

    const ast::Method *getMethodAST () const
    { return methodAST; }

    void setMethodAST (const ast::Method *newMethodAST)
    { methodAST = newMethodAST; }

   protected:
    const ast::Method *methodAST = nullptr;
  };

  class ExternalMethodMETA : public MethodMETA {
   public:
    ExternalMethodMETA () = default;
    ExternalMethodMETA (ExternalMethodMETA &&) = default;
    ExternalMethodMETA (const ExternalMethodMETA &) = default;
    ExternalMethodMETA &operator= (ExternalMethodMETA &&) = default;
    ExternalMethodMETA &operator= (const ExternalMethodMETA &) = default;
    ~ExternalMethodMETA () override = default;

    MethodMETAClass getMethodMETAClass () const override
    { return MethodMETAClass::External; }

    virtual void invoke (V ret, std::size_t argc, V argv[]) const = 0;
  };

  class __ExternalMethodMETA : public ExternalMethodMETA {
   public:
    __ExternalMethodMETA () = default;
    __ExternalMethodMETA (__ExternalMethodMETA &&) = default;
    __ExternalMethodMETA (const __ExternalMethodMETA &) = default;
    __ExternalMethodMETA &operator= (__ExternalMethodMETA &&) = default;
    __ExternalMethodMETA &operator= (const __ExternalMethodMETA &) = default;
    ~__ExternalMethodMETA () override = default;
    using BindingFunctionType = void (*) (V, std::size_t, V[]);

    void invoke (V ret, std::size_t argc, V argv[]) const override
    { invokePtr (ret, argc, argv); }

    BindingFunctionType getInvokeTarget () const
    { return invokePtr; }

    void setInvokeTarget (BindingFunctionType newInvokePtr)
    { invokePtr = newInvokePtr; }

   protected:
    BindingFunctionType invokePtr = nullptr;
  };

  explicit Dispatcher (const ast::CompilationUnit *compilationUnit);
  Dispatcher (Dispatcher &&) = default;
  Dispatcher (const Dispatcher &dispatcher) = default;
  Dispatcher &operator= (Dispatcher &&) = default;
  Dispatcher &operator= (const Dispatcher &) = default;
  ~Dispatcher () = default;

 protected:
  std::vector<std::unique_ptr<MethodMETA>> methodMETAs;
  std::unordered_map<const ast::InvokeStatement *,
                     MethodMETA *> invokeStatementMap;
  std::unordered_map<const ast::FunctionLiteral *,
                     std::vector<MethodMETA *>> functionLiteralMap;

  void registerExternalMethods ();
  void addMethodMETA (MethodMETA *methodMETA);

  void collectInternalMethods (const ast::CompilationUnit *compilationUnit);
  void collectInternalMethods (const ast::Module *module);
  void analysis (const ast::CompilationUnit *compilationUnit);
  void analysis (const ast::Module *module);
  void analysis (const ast::Method *method,
                 const std::vector<MethodMETA *> &visibleMethodMETAs);
  void analysis (const ast::AssignStatement *assignStatement,
                 const std::vector<MethodMETA *> &visibleMethodMETAs);
  void analysis (const ast::BranchStatement *branchStatement,
                 const std::vector<MethodMETA *> &visibleMethodMETAs);
  void analysis (const ast::InvokeStatement *invokeStatement,
                 const std::vector<MethodMETA *> &visibleMethodMETAs);
  void analysis (const ast::LabelStatement *labelStatement,
                 const std::vector<MethodMETA *> &visibleMethodMETAs);
  void analysis (const ast::PhiStatement *phiStatement,
                 const std::vector<MethodMETA *> &visibleMethodMETAs);
  void analysis (const ast::ReturnStatement *returnStatement,
                 const std::vector<MethodMETA *> &visibleMethodMETAs);

  std::vector<MethodMETA *>
  getVisibleMethodMETAs (const ast::Module *module) const;
};

}
}