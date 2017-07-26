#pragma once

#include <string>
#include <vector>
#include "../ast/AST.h"
#include "../ast/ASTVisitor.h"
#include "../misc/Hasher.h"
#include "../misc/Collections.h"

#include "MethodMETA.h"

namespace horseIR {
    namespace interpreter {
        template <typename IntermediateType,
                  typename HashCodeType = std::uint32_t,
                  typename Hasher = misc::Hasher<std::basic_string<char>, HashCodeType>>
        class ExternalMethod : public MethodMETA<IntermediateType, HashCodeType, Hasher> {
        public:
            ExternalMethod(const std::string& moduleName,
                           const std::string& methodName,
                           const std::string& signatureString) ;
            ExternalMethod(const std::string& moduleName,
                           const std::string& methodName,
                           const std::vector<ast::Type*> inputTypes,
                           ast::Type* outputType) ;
            virtual ~ExternalMethod() = default ;

            // virtual IntermediateType invoke(std::size_t argc, IntermediateType args[]) = 0 ;
        } ;
    }
}

template <typename T, typename V, typename R>
inline horseIR::interpreter::ExternalMethod<T, V, R>::ExternalMethod(
    const std::string& p_moduleName,
    const std::string& p_methodName,
    const std::string& signatureString)
    : MethodMETA<T, V, R>(p_moduleName, p_methodName, MethodMETA<T, V, R>::MethodMETAClass::Primitive)
{
    antlr4::ANTLRInputStream inStream(signatureString.c_str()) ;
    horseIR::HorseIRLexer lexer(&inStream) ;
    antlr4::CommonTokenStream tokenStream(&lexer) ;
    horseIR::HorseIRParser parser(&tokenStream) ;
    auto signatureContext = parser.typeSignatureList() ;
    auto signatures = horseIR::ast::Type::makeTypeSignatureASTNodes(signatureContext, this->mem) ;
    horseIR::misc::Collections::apply(
        signatures,
        [] (horseIR::ast::Type* type) -> void {
            horseIR::ast::ASTVisitors::applyToEachNode(type, [] (horseIR::ast::ASTNode* ast) -> void {
                    (void) ast->setCST(nullptr) ;
                }) ;
        }) ;    
    for (auto iter = signatures.cbegin(); iter != signatures.cend(); ++iter) {
        auto iter_p = iter ;
        if ((++iter_p) != signatures.cend()) {
            this->inputTypes.push_back(*iter) ;
        } else {
            this->outputType = *iter ;
        }
    }
}

template <typename T, typename V, typename R>
inline horseIR::interpreter::ExternalMethod<T, V, R>::ExternalMethod(
    const std::string& p_moduleName,
    const std::string& p_methodName,
    const std::vector<ast::Type*> p_inputTypes,
    ast::Type* p_outputType)
    : MethodMETA<T, V, R>(p_moduleName, p_methodName, MethodMETA<T, V, R>::MethodMETAClass::Primitive)
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
