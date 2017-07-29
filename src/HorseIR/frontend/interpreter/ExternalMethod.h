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
                           const std::vector<ast::Type*> inputTypes,
                           ast::Type* outputType) ;
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
    return ;
}

template <typename T>
inline horseIR::interpreter::ExternalMethod<T>::ExternalMethod(
    const std::string& p_moduleName,
    const std::string& p_methodName,
    const std::vector<ast::Type*> p_inputTypes,
    ast::Type* p_outputType)
    : MethodMETA<T>(p_moduleName, p_methodName, MethodMETA<T>::MethodMETAClass::Primitive)
{
    this->inputTypes = horseIR::misc::Collections::applyAndCollect(
        p_inputTypes,
        [&] (horseIR::ast::Type* type) -> horseIR::ast::Type* {
            return static_cast<horseIR::ast::Type*>(type->duplicateDeep(this->mem)) ;
        }) ;
    this->outputType = static_cast<horseIR::ast::Type*>(p_outputType->duplicateDeep(this->mem)) ;
    horseIR::misc::Collections::apply(
        this->inputTypes,
        [] (horseIR::ast::Type* type) -> void {
            horseIR::ast::ASTVisitors::applyToEachNode(
                type,
                [] (horseIR::ast::ASTNode* ast) -> void {
                    (void) ast->setCST(nullptr) ;
                }) ;
        }) ;
    horseIR::ast::ASTVisitors::applyToEachNode(
        this->outputType,
        [] (horseIR::ast::ASTNode* ast) -> void{
            (void) ast->setCST(nullptr) ;
        }) ;
}
