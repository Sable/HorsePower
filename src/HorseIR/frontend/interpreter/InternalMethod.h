#pragma once

#include <vector>
#include <string>

#include "../ast/AST.h"
#include "../ast/ASTVisitor.h"

#include "MethodMETA.h"

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
    horseIR::HorseIRParser::TypeFuncContext* context = parser.typeFunc() ;
    this->methodType = new ast::FunctionType(context, this->mem) ;
    ast::ASTVisitors::applyToEachNode(this->methodType, [](ast::ASTNode* node) -> void {
            node->setCST(nullptr) ;
            return ;
        }) ;
    (void) this->methodType->setParentASTNode(nullptr) ;
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
    this->methodType = p_method->makeSignatureFunctionType(this->mem) ;
    ast::ASTVisitors::applyToEachNode(this->methodType, [](ast::ASTNode* node) -> void {
            (void) node->setCST(nullptr) ;
            return ;
        }) ;
    (void) this->methodType->setParentASTNode(nullptr) ;
    return ;
}

template <typename T>
inline InternalMethod<T>::InternalMethod(ast::Method* p_method)
    : MethodMETA<T>(static_cast<ast::Module*>(p_method->getParentASTNode())->getModuleName(),
                    p_method->getMethodName(),
                    MethodMETA<T>::MethodMETAClass::Internal)
{
    assert(p_method != nullptr) ;
    this->methodType = p_method->makeSignatureFunctionType(this->mem) ;
    ast::ASTVisitors::applyToEachNode(this->methodType, [](ast::ASTNode* node) -> void {
            (void) node->setCST(nullptr) ;
            return ;
        }) ;
    (void) this->methodType->setParentASTNode(nullptr) ;
    return ;
}

template <typename T>
inline horseIR::ast::Method*
InternalMethod<T>::getInvokedMethodAST() const
{
    return method ;
}

}
}

