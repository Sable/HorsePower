#pragma once

#include "antlr4-runtime.h"

#include "ASTNode.h"
#include "Type.h"

namespace horseIR {
    namespace ast {
        class ASTVisitor {
        public:
            virtual antlrcpp::Any visit(ASTNode* node) ;
        protected:
            virtual antlrcpp::Any visitScalarType(ScalarType* scalarType) = 0 ;
            virtual antlrcpp::Any visitWildcardType(WildcardType* wildcardType) = 0 ;
            virtual antlrcpp::Any visitListType(ListType* listType) = 0 ;
            virtual antlrcpp::Any visitDictionaryType(DictionaryType* dictionaryType) = 0 ;
            virtual antlrcpp::Any visitEnumerationType(EnumerationType* enumerationType) = 0 ;
            virtual antlrcpp::Any visitFunctionType(FunctionType* functionType) = 0 ;
        } ;
    }
}
