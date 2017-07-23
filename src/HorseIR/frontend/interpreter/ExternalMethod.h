#pragma once

#include <string>
#include <vector>
#include "../ast/AST.h"
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
                           const std::vector<const ast::Type*> inputTypes,
                           const ast::Type* types) ;
            virtual ~ExternalMethod() = default ;

            
        } ;
    }
}

template <typename T, typename V, typename R>
inline horseIR::interpreter::ExternalMethod<T, V, R>::ExternalMethod(
    const std::string& moduleName,
    const std::string& methodName,
    const std::string& signatureString)
    : MethodMETA<T, V, R>(moduleName, methodName, MethodMETA<T, V, R>::MethodMETAClass::Primitive)
{
    antlr4::ANTLRInputStream inStream(signatureString.c_str()) ;
    horseIR::HorseIRLexer lexer(&inStream) ;
    antlr4::CommonTokenStream tokenStream(&lexer) ;
    horseIR::HorseIRParser parser(&tokenStream) ;
    auto signatureContext = parser.typeSignatureList() ;
    auto signatures = horseIR::ast::Type::makeTypeSignatureASTNodes(signatureContext, this->mem) ;
    horseIR::misc::Collections::apply(signatures,
                                      [] (horseIR::ast::Type* type) -> void {
                                          (void) type->setCST(nullptr) ;
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
