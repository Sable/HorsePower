#pragma once

#include <ostream>
#include <vector>
#include <utility>

#include "antlr4-runtime.h"
#include "../grammar/HorseIRBaseVisitor.h"
#include "../grammar/HorseIRParser.h"

namespace horseIR {
    namespace util {
        class CSTPrettyPrinter : HorseIRBaseVisitor {
        public:
            //CSTPrettyPrinter() = delete ;
            CSTPrettyPrinter(std::ostream& outputStream)
                : strm(outputStream)
            {}

            std::ostream& prettyPrint(antlr4::tree::ParseTree* parseTree) ;
        //protected:
        public:
            virtual antlrcpp::Any visitModule(HorseIRParser::ModuleContext *ctx) override ;
            virtual antlrcpp::Any visitMethod(HorseIRParser::MethodContext *ctx) override;
            virtual antlrcpp::Any visitParameterList(HorseIRParser::ParameterListContext *ctx) override;
            virtual antlrcpp::Any visitLabel(HorseIRParser::LabelContext *ctx) override;
            virtual antlrcpp::Any visitStmtCore(HorseIRParser::StmtCoreContext   *ctx) override;
            virtual antlrcpp::Any visitStmtLabel(HorseIRParser::StmtLabelContext *ctx) override;
            virtual antlrcpp::Any visitStmtNameExpr(HorseIRParser::StmtNameExprContext *ctx) override;
            virtual antlrcpp::Any visitStmtCNameExpr(HorseIRParser::StmtCNameExprContext *ctx) override;
            virtual antlrcpp::Any visitStmtReturn(HorseIRParser::StmtReturnContext *ctx) override;
            virtual antlrcpp::Any visitStmtGoto(HorseIRParser::StmtGotoContext *ctx) override;

            virtual antlrcpp::Any visitNameKey(HorseIRParser::NameKeyContext *ctx) override;

            virtual antlrcpp::Any visitExprBasicType(HorseIRParser::ExprBasicTypeContext *ctx) override;
            virtual antlrcpp::Any visitExprCheckType(HorseIRParser::ExprCheckTypeContext *ctx) override;
            virtual antlrcpp::Any visitExprCheckCast(HorseIRParser::ExprCheckCastContext *ctx) override;
            virtual antlrcpp::Any visitExprPhi(HorseIRParser::ExprPhiContext *ctx) override;


            virtual antlrcpp::Any visitTypeCaseScalar(HorseIRParser::TypeCaseScalarContext     *ctx) override;
            virtual antlrcpp::Any visitTypeCaseWildcard(HorseIRParser::TypeCaseWildcardContext *ctx) override;
            virtual antlrcpp::Any visitTypeCaseList(HorseIRParser::TypeCaseListContext *ctx) override;
            virtual antlrcpp::Any visitTypeCaseDict(HorseIRParser::TypeCaseDictContext *ctx) override;
            virtual antlrcpp::Any visitTypeCaseEnum(HorseIRParser::TypeCaseEnumContext *ctx) override;

            virtual antlrcpp::Any visitTypeFunc0(HorseIRParser::TypeFunc0Context* ctx) override ;
            virtual antlrcpp::Any visitTypeFunc1(HorseIRParser::TypeFunc1Context* ctx) override ;
            virtual antlrcpp::Any visitTypeFunc2(HorseIRParser::TypeFunc2Context* ctx) override ;
            virtual antlrcpp::Any visitTypeFunc3(HorseIRParser::TypeFunc3Context* ctx) override ;

            virtual antlrcpp::Any visitNameId(HorseIRParser::NameIdContext *ctx) override;
        private:
            std::ostream& strm ;
        } ;
    }
}
