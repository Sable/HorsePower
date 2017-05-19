#pragma once

#include <iostream>

#include "antlr4-runtime.h"
#include "MIRBaseVisitor.h"

namespace mir {
    namespace util {
        class PrettyPrinter : protected MIRBaseVisitor {
        public:   
            PrettyPrinter() = delete ;
            PrettyPrinter( std::ostream& strm )
                : outputStream( strm )
            {}

            void prettyPrint(antlr4::tree::ParseTree *parseTree) ;
        protected:
            virtual antlrcpp::Any visitTypeCaseScalar( MIRParser::TypeCaseScalarContext* ) override ;
            virtual antlrcpp::Any visitTypeCaseWildcard( MIRParser::TypeCaseWildcardContext* ) override ;
            virtual antlrcpp::Any visitTypeList( MIRParser::TypeListContext* ) override ;
            virtual antlrcpp::Any visitTypeDict( MIRParser::TypeDictContext* ) override ;
            virtual antlrcpp::Any visitTypeEnum( MIRParser::TypeEnumContext* ) override ;
            virtual antlrcpp::Any visitTypeFuncCase0( MIRParser::TypeFuncCase0Context* ) override ;
            virtual antlrcpp::Any visitTypeFuncCase1( MIRParser::TypeFuncCase1Context* ) override ;
            virtual antlrcpp::Any visitTypeFuncCase2( MIRParser::TypeFuncCase2Context* ) override ;
            virtual antlrcpp::Any visitTypeFuncCase3( MIRParser::TypeFuncCase3Context* ) override ;
        private:
            std::ostream& outputStream ;
    };
}
}
