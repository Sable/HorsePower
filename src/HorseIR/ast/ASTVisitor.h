#pragma once

#include "antlr4-runtime.h"

#include "ASTNode.h"
#include "Type.h"
#include "Literal.h"

namespace horseIR {
    namespace ast {
        class ASTVisitor {
        public:
            virtual antlrcpp::Any visit(ASTNode* node) = 0;
        protected:
            virtual antlrcpp::Any visitNilLiteral(NilLiteral* nilLiteral) = 0 ;
            virtual antlrcpp::Any visitComplexLiteral(ComplexLiteral* complexLiteral) = 0 ;
            virtual antlrcpp::Any visitBoolLiteral(BoolLiteral* boolLiteral) = 0 ;
            virtual antlrcpp::Any visitCharLiteral(CharLiteral* charLiteral) = 0 ;
            virtual antlrcpp::Any visitInteger8Literal(Integer8Literal* integer8Literal) = 0 ;
            virtual antlrcpp::Any visitInteger16Literal(Integer16Literal* integer16Literal) = 0 ;
            virtual antlrcpp::Any visitInteger32Literal(Integer32Literal* integer32Literal) = 0 ;
            virtual antlrcpp::Any visitInteger64Literal(Integer64Literal* integer64Literal) = 0 ;
            //virtual antlrcpp::Any visitFP32Literal(FP32Literal* fp32Literal) = 0 ;
            //virtual antlrcpp::Any visitFP64Literal(FP64Literal* fp64Literal) = 0 ;
            virtual antlrcpp::Any visitSymbolLiteral(SymbolLiteral* symbolLiteral) = 0 ;
            /*
            virtual antlrcpp::Any visitTimeMonthLiteral(TimeMonthLiteral* timeMonthLiteral) = 0 ;
            virtual antlrcpp::Any visitTimeDateLiteral(TimeDateLiteral* timeDateLiteral) = 0 ;
            virtual antlrcpp::Any visitTimeDateTimeLiteral(TimeDateTimeLiteral* timeDateTimeLiteral) = 0 ;
            virtual antlrcpp::Any visitTimeMinuteLiteral(TimeMinuteLiteral* timeMinuteLiteral) = 0 ;
            virtual antlrcpp::Any visitTimeSecondLiteral(TimeSecondLiteral* timeSecondLiteral) = 0 ;
            virtual antlrcpp::Any visitTimeTimeLiteral(TimeTimeLiteral* timeTimeLiteral) = 0 ;
            */
            virtual antlrcpp::Any visitFunctionLiteral(FunctionLiteral* functionLiteral) = 0 ;
            virtual antlrcpp::Any visitTableLiteral(TableLiteral* tableLiteral) = 0 ;
            //virtual antlrcpp::Any visitKeyTableLiteral(KeyTableLiteral* keyTableLiteral) = 0 ;
            //virtual antlrcpp::Any visitStringLiteral(StringLiteral* stringLiteral) = 0 ;

            virtual antlrcpp::Any visitScalarType(ScalarType* scalarType) = 0 ;
            virtual antlrcpp::Any visitWildcardType(WildcardType* wildcardType) = 0 ;
            virtual antlrcpp::Any visitListType(ListType* listType) = 0 ;
            virtual antlrcpp::Any visitDictionaryType(DictionaryType* dictionaryType) = 0 ;
            virtual antlrcpp::Any visitEnumerationType(EnumerationType* enumerationType) = 0 ;
            virtual antlrcpp::Any visitFunctionType(FunctionType* functionType) = 0 ;
        } ;

        class ASTBaseVisitor : protected ASTVisitor {
        public:
            virtual antlrcpp::Any visit(ASTNode* node) override ;

        protected:
            antlrcpp::Any visitChildren(ASTNode* node) ;

            virtual antlrcpp::Any visitNilLiteral(NilLiteral* nilLiteral) override ;
            virtual antlrcpp::Any visitComplexLiteral(ComplexLiteral* complexLiteral) override ;
            virtual antlrcpp::Any visitBoolLiteral(BoolLiteral* boolLiteral) override ;
            virtual antlrcpp::Any visitCharLiteral(CharLiteral* charLiteral) override ;
            virtual antlrcpp::Any visitInteger8Literal(Integer8Literal* integer8Literal) override ;
            virtual antlrcpp::Any visitInteger16Literal(Integer16Literal* integer16Literal) override ;
            virtual antlrcpp::Any visitInteger32Literal(Integer32Literal* integer32Literal) override ;
            virtual antlrcpp::Any visitInteger64Literal(Integer64Literal* integer64Literal) override ;
            //virtual antlrcpp::Any visitFP32Literal(FP32Literal* fp32Literal) override ;
            //virtual antlrcpp::Any visitFP64Literal(FP64Literal* fp64Literal) override ;
            virtual antlrcpp::Any visitSymbolLiteral(SymbolLiteral* symbolLiteral) override ;
            /*
            virtual antlrcpp::Any visitTimeMonthLiteral(TimeMonthLiteral* timeMonthLiteral) override ;
            virtual antlrcpp::Any visitTimeDateLiteral(TimeDateLiteral* timeDateLiteral) override ;
            virtual antlrcpp::Any visitTimeDateTimeLiteral(TimeDateTimeLiteral* timeDateTimeLiteral) override ;
            virtual antlrcpp::Any visitTimeMinuteLiteral(TimeMinuteLiteral* timeMinuteLiteral) override ;
            virtual antlrcpp::Any visitTimeSecondLiteral(TimeSecondLiteral* timeSecondLiteral) override ;
            virtual antlrcpp::Any visitTimeTimeLiteral(TimeTimeLiteral* timeTimeLiteral) override ;
            */
            virtual antlrcpp::Any visitFunctionLiteral(FunctionLiteral* functionLiteral) override ;
            virtual antlrcpp::Any visitTableLiteral(TableLiteral* tableLiteral) override ;
            //virtual antlrcpp::Any visitKeyTableLiteral(KeyTableLiteral* keyTableLiteral) override ;
            //virtual antlrcpp::Any visitStringLiteral(StringLiteral* stringLiteral) override ;

            virtual antlrcpp::Any visitScalarType(ScalarType* type) override ;
            virtual antlrcpp::Any visitWildcardType(WildcardType* wildcardType) override ;
            virtual antlrcpp::Any visitListType(ListType* listType) override ;
            virtual antlrcpp::Any visitDictionaryType(DictionaryType* dictionaryType) override ;
            virtual antlrcpp::Any visitEnumerationType(EnumerationType* enumerationType) override ;
            virtual antlrcpp::Any visitFunctionType(FunctionType* functionType) override ;
        };
    }
}
