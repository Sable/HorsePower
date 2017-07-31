#pragma once

#include <vector>
#include <string>

#include "../ast/AST.h"
#include "../ast/ASTVisitor.h"

#include "MethodMETA.h"
#include "Exception.h"

namespace horseIR {
namespace interpreter {

template <typename IntermediateType>
class InternalMethod : public MethodMETA<IntermediateType> {
public:
    InternalMethod(const std::string& moduleName,
                   const std::string& methodName,
                   const std::string& signatureString,
                   ast::Method* method) ;
    InternalMethod(const std::string& moduleName,
                   const std::string& methodName,
                   ast::FunctionType* signatureType,
                   ast::Method* method) ;
    InternalMethod(ast::Method* method) ;
    virtual ~InternalMethod() = default ;
    virtual std::string toString() const override ;
    ast::Method* getInvokedMethodAST() const ;
protected:
    ast::Method* method ;
} ;

template <typename T>
inline InternalMethod<T>::InternalMethod(const std::string& moduleName,
                                         const std::string& methodName,
                                         const std::string& signatureString,
                                         ast::Method* p_method)
    : MethodMETA<T>(moduleName, methodName, MethodMETA<T>::MethodMETAClass::Internal),
    method(p_method)
{
    antlr4::ANTLRInputStream inStream(signatureString.c_str()) ;
    horseIR::HorseIRLexer lexer(&inStream) ;
    antlr4::CommonTokenStream stream(&lexer) ;
    horseIR::HorseIRParser parser(&stream) ;

    lexer.removeErrorListeners() ;
    parser.removeParseListeners() ;
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
inline InternalMethod<T>::InternalMethod(const std::string& p_moduleName,
                                         const std::string& p_methodName,
                                         ast::FunctionType* p_signatureType,
                                         ast::Method* p_method)
    : MethodMETA<T>(p_moduleName, p_methodName, MethodMETA<T>::MethodMETAClass::Internal),
    method(p_method)
{
    assert(p_method != nullptr) ;
    ast::FunctionType* functionType = p_method->makeSignatureFunctionType(this->mem) ;
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
inline InternalMethod<T>::InternalMethod(ast::Method* p_method)
    : MethodMETA<T>(static_cast<ast::Module*>(p_method->getParentASTNode())->getModuleName(),
                    p_method->getMethodName(),
                    MethodMETA<T>::MethodMETAClass::Internal),
    method(p_method)
{
    assert(p_method != nullptr) ;
    ast::FunctionType* functionType = p_method->makeSignatureFunctionType(this->mem) ;
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
inline std::string InternalMethod<T>::toString() const
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
    stream << outParamType->toString() ;
    const antlr4::tree::ParseTree* tree = method->getCST() ;
    if (tree == nullptr) {
        stream << " [unknown]" ;
    } else {
        auto context = static_cast<const HorseIRParser::MethodContext*>(tree) ;
        stream << " [in file: " << method->getEnclosingFilePath()
               << " at "
               << context->start->getLine()
               << ':'
               << context->start->getCharPositionInLine()
               << ']' ;
         
    }
    return stream.str() ;
}

template <typename T>
inline horseIR::ast::Method* InternalMethod<T>::getInvokedMethodAST() const
{
    return method ;
}

}
}

