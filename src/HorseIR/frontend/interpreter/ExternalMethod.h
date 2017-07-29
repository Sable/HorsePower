#pragma once

#include <string>
#include <vector>
#include "../ast/AST.h"
#include "../ast/ASTVisitor.h"
#include "../misc/Collections.h"

#include "MethodMETA.h"

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

            // virtual IntermediateType invoke(std::size_t argc, IntermediateType args[]) const  = 0 ;
        } ;
    }
}

template <typename T>
inline horseIR::interpreter::ExternalMethod<T>::ExternalMethod(
    const std::string& p_moduleName,
    const std::string& p_methodName,
    const std::string& signatureString)
    : MethodMETA<T>(p_moduleName, p_methodName, MethodMETA<T>::MethodMETAClass::External)
{
    antlr4::ANTLRInputStream inStream(signatureString.c_str()) ;
    horseIR::HorseIRLexer lexer(&inStream) ;
    antlr4::CommonTokenStream tokenStream(&lexer) ;
    horseIR::HorseIRParser parser(&tokenStream) ;
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
inline horseIR::interpreter::ExternalMethod<T>::ExternalMethod(
    const std::string& p_moduleName,
    const std::string& p_methodName,
    ast::FunctionType* p_signatureType)
    : MethodMETA<T>(p_moduleName, p_methodName, MethodMETA<T>::MethodMETAClass::External)
{
    assert(p_signatureType) ;
    this->methodType = p_signatureType->duplicateDeep(this->mem) ;
    ast::ASTVisitors::applyToEachNode(this->methodType, [](ast::ASTNode* node) -> void {
            (void) node->setCST(nullptr) ;
            return ;
        }) ;
    (void) this->methodType->setParentASTNode(nullptr) ;
    return ;
}
