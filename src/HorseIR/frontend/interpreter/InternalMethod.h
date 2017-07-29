#pragma once

#include <vector>
#include <string>

#include "MethodMETA.h"
#include "../ast/AST.h"

namespace horseIR {
    namespace interpreter {
        template <typename IntermediateType>
        class InternalMethod : public MethodMETA<IntermediateType> {
        public:
            InternalMethod(const std::string& moduleName,
                           const std::string& methodName,
                           const std::string& signatureString,
                           ast::Method* p_method) ;
            InternalMethod(const std::string& moduleName,
                           const std::string& methodName,
                           const std::vector<ast::Type*>& inputTypes,
                           ast::Type* outputType,
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
    
}
