#pragma once

#include <string>
#include <vector>
#include <functional>

#include "../ast/AST.h"
#include "../ast/ASTVisitor.h"
#include "../misc/Collections.h"

#include "MethodMETA.h"
#include "Exception.h"

namespace horseIR {
namespace interpreter {

template <typename IntermediateType>
class ExternalMethod : public MethodMETA<IntermediateType> {
public:
    ExternalMethod(const std::string& moduleName,
                   const std::string& methodName,
                   const std::string& signatureString) ;
    ExternalMethod(const std::string& moduleName,
                   const std::string& methodName,
                   ast::FunctionType* signatureType) ;
    virtual ~ExternalMethod() = default ;

    virtual std::string toString() const override ;
    virtual IntermediateType invoke(std::size_t argc, IntermediateType args[]) const  = 0 ;
    
    typedef std::function<IntermediateType(std::size_t, IntermediateType[])> BindFunctionType ;
    static ExternalMethod* bindExternalMethod(const std::string& moduleName,
                                              const std::string& methodName,
                                              const std::string& signatureString,
                                              const BindFunctionType& func) ;
} ;

template <typename IntermediateType>
class __ExternalMethodLambda : public ExternalMethod<IntermediateType> {
public:
    __ExternalMethodLambda(const std::string& moduleName,
                           const std::string& methodName,
                           const std::string& signatureString,
                           const std::function<IntermediateType(std::size_t, IntermediateType[])>& func) ;
    __ExternalMethodLambda(const std::string& moduleName,
                           const std::string& methodName,
                           ast::FunctionType* signatureType,
                           const std::function<IntermediateType(std::size_t, IntermediateType[])>& func) ;
    virtual ~__ExternalMethodLambda() = default ;
    
    virtual IntermediateType invoke(std::size_t argc, IntermediateType argv[]) const override ;
protected:
    const std::function<IntermediateType(std::size_t, IntermediateType[])> wrapFunc ;
} ;

template <typename T>
inline ExternalMethod<T>::ExternalMethod(const std::string& p_moduleName,
                                         const std::string& p_methodName,
                                         const std::string& signatureString)
    : MethodMETA<T>(p_moduleName, p_methodName, MethodMETA<T>::MethodMETAClass::External)
{
    antlr4::ANTLRInputStream inStream(signatureString.c_str()) ;
    horseIR::HorseIRLexer lexer(&inStream) ;
    antlr4::CommonTokenStream tokenStream(&lexer) ;
    horseIR::HorseIRParser parser(&tokenStream) ;
    
    lexer.removeErrorListeners() ;
    parser.removeErrorListeners() ;
    InvalidSignatureString::SignatureStringErrorListener errorListener(&signatureString) ;
    lexer.addErrorListener(&errorListener) ;
    parser.addErrorListener(&errorListener) ;
    
    horseIR::HorseIRParser::TypeFuncContext* context = parser.typeFunc() ;
    ast::FunctionType* functionType = new ast::FunctionType(context, this->mem) ;
    ast::ASTVisitors::applyToEachNode(functionType, [](ast::ASTNode* node) -> void {
            node->setCST(nullptr) ;
            return ;
        }) ;
    (void) functionType->setParentASTNode(nullptr) ;
    this->methodType = functionType ;
    this->dispatchType = functionType->duplicateDeep(this->mem) ;
    (void) this->dispatchType->setReturnType(new ast::WildcardType(this->mem)) ;
    return ;
}

template <typename T>
inline ExternalMethod<T>::ExternalMethod(const std::string& p_moduleName,
                                         const std::string& p_methodName,
                                         ast::FunctionType* p_signatureType)
    : MethodMETA<T>(p_moduleName, p_methodName, MethodMETA<T>::MethodMETAClass::External)
{
    assert(p_signatureType) ;
    ast::FunctionType* functionType = p_signatureType->duplicateDeep(this->mem) ;
    ast::ASTVisitors::applyToEachNode(functionType, [](ast::ASTNode* node) -> void {
            (void) node->setCST(nullptr) ;
            return ;
        }) ;
    (void) functionType->setParentASTNode(nullptr) ;
    this->methodType = functionType ;
    this->dispatchType = functionType->duplicateDeep(this->mem) ;
    (void) this->dispatchType->setReturnType(new ast::WildcardType(this->mem)) ;
    return ;
}

template <typename T>
inline std::string ExternalMethod<T>::toString() const
{
    std::ostringstream stream ;
    stream << this->moduleName << '.' << this->methodName
           << '(' ;
    const std::vector<ast::Type*> inParamTypes (this->methodType->getParameterTypes()) ;
    auto inParamTypesSegments = misc::Collections::map(
        inParamTypes,
        [](ast::Type* type) -> std::string {return type->toString() ;}) ;
    misc::Collections::writeToStream(
        stream,
        inParamTypesSegments,
        ", ",
        (this->methodType->getIsFlexible()? ", ...) -> " : ") -> ")) ;
    ast::Type* const outParamType = this->methodType->getReturnType() ;
    stream << outParamType->toString() 
           << " [external]" ;
    return stream.str() ;
}

template <typename T>
inline ExternalMethod<T>*
ExternalMethod<T>::bindExternalMethod(const std::string &moduleName,
                                      const std::string &methodName,
                                      const std::string &signatureString,
                                      const std::function<T(std::size_t, T[])> &func)
{
    using ImplementationType = horseIR::interpreter::__ExternalMethodLambda<T> ;
    return new ImplementationType(moduleName, methodName, signatureString, func) ;
}

template <typename T>
inline __ExternalMethodLambda<T>::__ExternalMethodLambda(const std::string& p_moduleName,
                                                         const std::string& p_methodName,
                                                         const std::string& p_signatureString,
                                                         const std::function<T(std::size_t, T[])>& func)
    : ExternalMethod<T>(p_moduleName, p_methodName, p_signatureString),
    wrapFunc(func)
{}

template <typename T>
inline __ExternalMethodLambda<T>::__ExternalMethodLambda(const std::string& p_moduleName,
                                                         const std::string& p_methodName,
                                                         horseIR::ast::FunctionType* p_signatureType,
                                                         const std::function<T(std::size_t, T[])>& func)
    : ExternalMethod<T>(p_moduleName, p_methodName, p_signatureType),
    wrapFunc(func)
{}

template <typename T>
inline T __ExternalMethodLambda<T>::invoke(std::size_t argc, T argv[]) const
{
    return wrapFunc(argc, argv) ;
}


}
}

