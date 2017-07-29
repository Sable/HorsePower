#pragma once

#include <vector>
#include <string>

#include "MethodMETA.h"
#include "../ast/AST.h"
#include "../ast/ASTVisitor.h"

namespace horseIR {
    namespace interpreter {
        template <typename IntermediateType>
        class InternalMethod : public MethodMETA<IntermediateType> {
        public:
            InternalMethod(const std::string& moduleName,
                           const std::string& methodName,
                           const std::string& signatureString,
                           ast::Method* p_method) ;
            InternalMethod(ast::Method* p_method) ;
            virtual ~InternalMethod() = default ;

            ast::Method* getInvokedMethodAST() const ;
        protected:
            ast::Method* method ;
        } ;
    }
}

template <typename T>
inline horseIR::interpreter::InternalMethod<T>::InternalMethod(
    const std::string& moduleName,
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
    return ;
}
