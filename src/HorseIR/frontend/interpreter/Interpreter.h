#pragma once

#include <string>
#include <vector>
#include <stack>
#include <memory>
#include <utility>
#include <algorithm>

#include "../ast/AST.h"

#include "VectorDispatcher.h"
#include "VectorVarStorage.h"
#include "ASTLiteralConverter.h"
#include "BackendTraits.h"

namespace horseIR {
namespace interpreter {

template <typename T> struct __STLStack : protected std::deque<T> {
public:
    typedef T ValueType ;
    typedef typename std::deque<T>::const_iterator ConstIteratorType ;
    
    static const T& top(const __STLStack<T>& s)
        {
            return s.std::deque<T>::back() ;
        }
    static void push(const __STLStack<T>& s, T&& v)
        {
            return s.std::deque<T>::push_back(v) ;
        }
    static void pop(const __STLStack<T>& s) 
        {
            return s.std::deque<T>::pop_back() ;
        }
    static std::string containerToString(const __STLStack<T>& s) ;

    static ConstIteratorType cbegin(const __STLStack<T>& stack)
        {
            return stack.cbegin() ;
        }
    static ConstIteratorType cend(const __STLStack<T>& stack)
        {
            return stack.cend() ;
        }
} ;

template <typename IntermediateType,
          template <typename> class Dispatcher = VectorDispatcher,
          template <typename> class VarStorage = VectorVarStorage,
          template <typename> class RuntimeStack = __STLStack>
struct InterpretContext {
    typedef struct StackFrameType {
        ast::Module* contextModule ;
        ast::Method* contextMethod ;
        ast::Statement* contextStatement ;
        ast::StatementIterator programCounter ;
        std::string previousLabel ;
        typename VarStorage<IntermediateType>::ContainerType stackStorage ;
        IntermediateType returnStorage ;

        static std::string containerToString(const struct StackFrameType& stackFrame) ;
    } StackFrameType ;
    
    typedef Dispatcher<IntermediateType> ContextDispatcher ;
    typedef VarStorage<IntermediateType> ContextVarStorage ;
    typedef RuntimeStack<std::unique_ptr<StackFrameType>> ContextRuntimeStackType ;
    
    typename Dispatcher<IntermediateType>::ContainerType dispatcherContainer ;
    typename VarStorage<IntermediateType>::ContainerType globalVarContainer ;
    ContextRuntimeStackType runtimeStack ;
} ;

template <typename IntermediateType,
          template <typename> class Dispatcher = VectorDispatcher,
          template <typename> class VarStorage = VectorVarStorage>
class TreeWalkInterpreter {
public:
    using InterpretContextType = InterpretContext<IntermediateType, Dispatcher, VarStorage> ;
    using BackendTraitsType = BackendTraits<IntermediateType> ;
    
// protected:
    ast::ASTNode::MemManagerType astMemory ;
    ast::CompilationUnit* evalUnit ;
    ast::Method* entryMethod ;

    InterpretContextType interpretContext ;

    void handleReturnStatement(ast::ReturnStatement* returnStatement) ;
    void handleBranchStatement(ast::BranchStatement* branchStatement) ;
    void handleAssignStatement(ast::AssignStatement* assignStatement) ;
    void handleLabelStatement(ast::LabelStatement* labelStatement) ;
    void handlePhiStatement(ast::PhiStatement* phiStatement) ;

    IntermediateType handleLiteral(ast::Literal* literal) const ;

    void resolveFunctionLiteralName() ;
} ;

template <typename T, template<typename> class V, template<typename> class R>
inline void TreeWalkInterpreter<T, V, R>::handleReturnStatement(ast::ReturnStatement* returnStatement)
{
    assert(returnStatement != nullptr) ;
    using StackFrameType = typename InterpretContextType::StackFrameType ;
    using RuntimeStackType = typename InterpretContextType::ContextRuntimeStackType ;
    using VarStorageType = typename InterpretContextType::ContextVarStorage ;
    StackFrameType* const curFrame = RuntimeStackType::top(interpretContext.runtimeStack).get() ;
    curFrame->contextStatement = returnStatement ;
    
    ast::Identifier* returnIdentifier = returnStatement->getIdentifier() ;
    std::string returnIdentifierName = returnIdentifier->getName() ;
    T backendValue = VarStorageType::read(curFrame->stackStorage, returnIdentifierName) ;
    curFrame->returnStorage = backendValue ;
    curFrame->programCounter = curFrame->contextMethod->end() ;
    return ;
}

template <typename T, template<typename> class V, template<typename> class R>
inline void TreeWalkInterpreter<T, V, R>::handleBranchStatement(ast::BranchStatement* branchStatement)
{
    assert(branchStatement != nullptr) ;
    using StackFrameType = typename InterpretContextType::StackFrameType ;
    using RuntimeStackType = typename InterpretContextType::ContextRuntimeStackType ;
    using VarStorageType = typename InterpretContextType::ContextVarStorage ;
    StackFrameType* const curFrame = RuntimeStackType::top(interpretContext.runtimeStack).get() ;
    curFrame->contextStatement = branchStatement ;
    
    const bool isConditionalBranch = branchStatement->getIsConditional() ;
    if (isConditionalBranch) {
        (void) curFrame->programCounter.nextOnTrue() ;
    } else {
        ast::Identifier* conditionID = branchStatement->getConditionID() ;
        std::string conditionIDName = conditionID->getName() ;
        T conditionValue = VarStorageType::read(curFrame->stackStorage, conditionIDName) ;
        if (BackendTraitsType::isBooleanTrue(conditionValue)) {
            (void) curFrame->programCounter.nextOnTrue() ;
        } else if (BackendTraitsType::isBooleanFalse(conditionValue)) {
            (void) curFrame->programCounter.nextOnFalse() ;
        } else {
            assert(false) ;
        }
    }
    return ;
}

template <typename T, template<typename> class V, template<typename> class R>
inline void TreeWalkInterpreter<T, V, R>::handleAssignStatement(ast::AssignStatement* assignStatement)
{
    assert(assignStatement != nullptr) ;
    using StackFrameType = typename InterpretContextType::StackFrameType ;
    using RuntimeStackType = typename InterpretContextType::ContextRuntimeStackType ;
    using VarStorageType = typename InterpretContextType::ContextVarStorage ;
    StackFrameType* const curFrame = RuntimeStackType::top(interpretContext.runtimeStack).get() ;
    curFrame->contextStatement = assignStatement ;

    const bool isInvocation = assignStatement->getIsInvocation() ;
    if (isInvocation) {

    } else {
        
    }
}

template <typename T, template<typename> class V, template<typename> class R>
inline void TreeWalkInterpreter<T, V, R>::handleLabelStatement(ast::LabelStatement* labelStatement)
{
    assert(labelStatement != nullptr) ;
    using StackFrameType = typename InterpretContextType::StackFrameType ;
    using RuntimeStackType = typename InterpretContextType::ContextRuntimeStackType ;
    using VarStorageType = typename InterpretContextType::ContextVarStorage ;
    StackFrameType* const curFrame = RuntimeStackType::top(interpretContext.runtimeStack).get() ;
    curFrame->contextStatement = labelStatement ;

    curFrame->previousLabel = labelStatement->getLabelName() ;
    (void) curFrame->programCounter.nextOnTrue() ;
    return ;
}

template <typename T, template<typename> class V, template<typename> class R>
inline void TreeWalkInterpreter<T, V, R>::handlePhiStatement(ast::PhiStatement* phiStatement)
{
    assert(phiStatement != nullptr) ;
    using StackFrameType = typename InterpretContextType::StackFrameType ;
    using RuntimeStackType = typename InterpretContextType::ContextRuntimeStackType ;
    using VarStorageType = typename InterpretContextType::ContextVarStorage ;
    StackFrameType* const curFrame = RuntimeStackType::top(interpretContext.runtimeStack).get() ;
    curFrame->contextStatement = phiStatement ;

    ast::Identifier* const selectedID = phiStatement->getInFlowMapAt(curFrame->previousLabel) ;
    const std::string selectedIDName = selectedID->getName() ;
    const T selectedValue = VarStorageType::read(curFrame->stackStorage, selectedIDName) ;
    const T duplicateValue = BackendTraitsType::createDuplicate(selectedValue) ;
    
    ast::Identifier* const lhsID = phiStatement->getLHSID() ;
    const std::string lhsIDName = lhsID->getName() ;
    ast::Type* const lhsType = phiStatement->getLHSType() ;
    VarStorageType::alloc(curFrame->stackStorage, lhsIDName, lhsType) ;
    (void) VarStorageType::write(curFrame->stackStorage, lhsIDName, duplicateValue) ;

    (void) curFrame->programCounter.nextOnTrue() ;
    return ;
}

template <typename T, template<typename> class V, template<typename> class R>
inline T TreeWalkInterpreter<T, V, R>::handleLiteral(ast::Literal *literal) const
{
    assert(literal != nullptr) ;
    using LiteralConverter = typename BackendTraitsType::ASTLiteralConverter ;
    switch (literal->getLiteralClass()) {
    case ast::Literal::LiteralClass::BoolLiteral: {
        auto const castedPtr = static_cast<ast::BoolLiteral*>(literal) ;
        return LiteralConverter::caseBoolLiteral(castedPtr) ;
    }
    case ast::Literal::LiteralClass::CharLiteral: {
        auto const castedPtr = static_cast<ast::CharLiteral*>(literal) ;
        return LiteralConverter::caseCharLiteral(castedPtr) ;
    }
    case ast::Literal::LiteralClass::ComplexLiteral: {
        auto const castedPtr = static_cast<ast::ComplexLiteral*>(literal) ;
        return LiteralConverter::caseComplexLiteral(castedPtr) ;
    }
    case ast::Literal::LiteralClass::FunctionLiteral: {
        using ContextDispatcher = typename InterpretContextType::ContextDispatcher ;
        auto castedPtr = static_cast<ast::FunctionLiteral*>(literal) ;
        auto signatureType = static_cast<ast::FunctionType*>(castedPtr->getLiteralType()) ;
        std::string const moduleName = castedPtr->getModuleName() ;
        std::string const methodName = castedPtr->getFunctionName() ;
        std::vector<ast::Type*> const paramTypes = signatureType->getParameterTypes() ;
        MethodMETA<T>* const dispatchedMethod = ContextDispatcher::fetch(
            interpretContext.dispatcherContainer,
            moduleName,
            methodName,
            paramTypes) ;
        assert(dispatchedMethod != nullptr) ;
        return BackendTraitsType::packMethodMETA(dispatchedMethod) ;
    }
    case ast::Literal::LiteralClass::IntegerLiteral: {
        auto const castedPtr = static_cast<ast::IntegerLiteral*>(literal) ;
        switch (castedPtr->getIntegerLiteralClass()) {
        case ast::IntegerLiteral::IntegerLiteralClass::Integer8Literal: {
            auto const int8Ptr = static_cast<ast::Integer8Literal*>(castedPtr) ;
            return LiteralConverter::caseInteger8Literal(int8Ptr) ;
        }
        case ast::IntegerLiteral::IntegerLiteralClass::Integer16Literal: {
            auto const int16Ptr = static_cast<ast::Integer16Literal*>(castedPtr) ;
            return LiteralConverter::caseInteger16Literal(int16Ptr) ;
        }
        case ast::IntegerLiteral::IntegerLiteralClass::Integer32Literal: {
            auto const int32Ptr = static_cast<ast::Integer32Literal*>(castedPtr) ;
            return LiteralConverter::caseInteger32Literal(int32Ptr) ;
        }
        case ast::IntegerLiteral::IntegerLiteralClass::Integer64Literal: {
            auto const int64Ptr = static_cast<ast::Integer64Literal*>(castedPtr) ;
            return LiteralConverter::caseInteger64Literal(int64Ptr) ;           
        }
        }
    }
    case ast::Literal::LiteralClass::NilLiteral: {
        auto const castedPtr = static_cast<ast::NilLiteral*>(literal) ;
        return LiteralConverter::caseNilLiteral(castedPtr) ;
    }
    case ast::Literal::LiteralClass::SymbolLiteral: {
        auto const castedPtr = static_cast<ast::SymbolLiteral*>(literal) ;
        return LiteralConverter::caseSymbolLiteral(castedPtr) ;
    }
    case ast::Literal::LiteralClass::TableLiteral: {
        auto const castedPtr = static_cast<ast::TableLiteral*>(literal) ;
        return LiteralConverter::caseTableLiteral(castedPtr) ;
    }
    } 
}

template <typename T, template<typename> class V, template<typename> class R>
inline void TreeWalkInterpreter<T, V, R>::resolveFunctionLiteralName()
{
    using ContextDispatcherType = typename InterpretContextType::ContextDispatcher ;
    std::vector<MethodMETA<T>*> externalFunctions ;
    std::copy_if(
        ContextDispatcherType::cbegin(interpretContext.dispatcherContainer),
        ContextDispatcherType::cend(interpretContext.dispatcherContainer),
        std::back_inserter(externalFunctions),
        [&] (MethodMETA<T>* method) -> bool {
            return method->getMethodMETAClass() == MethodMETA<T>::MethodMETAClass::External ;
        }) ;

    using InvokeCompletionEntry = std::pair<std::string, std::string> ;
    using InvokeCompletionEntrySelector = enum class __InvokeCompletionEntrySelector {
        ModuleName = 0, Methodname = 1
    } ;
    std::vector<InvokeCompletionEntry> invokeCompletionEntries {} ;
    for (auto iter = externalFunctions.cbegin(); iter != externalFunctions.cend(); ++iter) {
        std::string moduleName = (*iter)->getModuleName() ;
        std::string methodName = (*iter)->getMethodName() ;
        InvokeCompletionEntry invokeCompeletionEntry = std::make_pair(
            std::move(moduleName),
            std::move(methodName)) ;
        invokeCompletionEntries.emplace_back(std::move(invokeCompeletionEntry)) ;
    }
    const std::vector<ast::Module*> modules = evalUnit->getModules() ;
    for (auto moduleIter = modules.cbegin(); moduleIter != modules.cend(); ++moduleIter) {
        assert(*moduleIter != nullptr) ;
        const std::vector<ast::Method*> methods = (*moduleIter)->getMethods() ;
        const std::string moduleName = (*moduleIter)->getModuleName() ;
        for (auto methodIter = methods.cbegin(); methodIter != methods.cend(); ++methodIter) {
            assert(*methodIter != nullptr) ;
            std::string duplicateModuleName = moduleName ;
            std::string methodName = (*methodIter)->getMethodName() ;
            InvokeCompletionEntry invokeCompletionEntry = std::make_pair(
                std::move(duplicateModuleName),
                std::move(methodName)) ;
            invokeCompletionEntries.emplace_back(std::move(invokeCompletionEntries)) ;
        }
    }

    for (auto moduleIter = modules.cbegin(); moduleIter != modules.cend(); ++moduleIter) {
        assert(*moduleIter != nullptr) ;
        const std::vector<ast::Method*> methods = (*moduleIter)->getMethods() ;
        const std::vector<std::string> importModules = (*moduleIter)->getImportedModules() ;
    }
}

}
}
