#pragma once

#include <ostream>
#include <vector>
#include <utility>

#include "antlr4-runtime.h"
#include "../grammar/HorseIRBaseVisitor.h"
#include "../grammar/HorseIRParser.h"

/*
 *  REAMAIN
 *  - Literals: list / enum / dictionary / table / keyed table
 */

namespace horseIR {
    namespace util {
        class CSTPrettyPrinter : HorseIRBaseVisitor {
        public:
            //CSTPrettyPrinter() = delete ;
            CSTPrettyPrinter(std::ostream& outputStream)
                : strm(outputStream)
            { depth = 0; }

            std::ostream& prettyPrint(antlr4::tree::ParseTree* parseTree) ;
        //protected:
        public:
            virtual antlrcpp::Any visitModule(HorseIRParser::ModuleContext *ctx) override ;
            virtual antlrcpp::Any visitMethod(HorseIRParser::MethodContext *ctx) override;
            virtual antlrcpp::Any visitParameterList(HorseIRParser::ParameterListContext *ctx) override;
            virtual antlrcpp::Any visitGlobalVar(HorseIRParser::GlobalVarContext *ctx) override;
            virtual antlrcpp::Any visitImportCID(HorseIRParser::ImportCIDContext *ctx) override;
            virtual antlrcpp::Any visitImportID(HorseIRParser::ImportIDContext *ctx) override;

            virtual antlrcpp::Any visitLabel(HorseIRParser::LabelContext *ctx) override;
            virtual antlrcpp::Any visitStmtCore(HorseIRParser::StmtCoreContext   *ctx) override;
            virtual antlrcpp::Any visitStmtLabel(HorseIRParser::StmtLabelContext *ctx) override;
            virtual antlrcpp::Any visitStmtNameExpr(HorseIRParser::StmtNameExprContext *ctx) override;
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

            virtual antlrcpp::Any visitMethodInv(HorseIRParser::MethodInvContext *ctx) override;
            virtual antlrcpp::Any visitMethodFun(HorseIRParser::MethodFunContext *ctx) override;
            virtual antlrcpp::Any visitArgumentList(HorseIRParser::ArgumentListContext *ctx) override;
            virtual antlrcpp::Any visitCompoundName(HorseIRParser::CompoundNameContext *ctx) override;

            virtual antlrcpp::Any visitLiteralNil(HorseIRParser::LiteralNilContext *ctx) override;
            virtual antlrcpp::Any visitLiteralComplex(HorseIRParser::LiteralComplexContext *ctx) override;
            virtual antlrcpp::Any visitLiteralBool(HorseIRParser::LiteralBoolContext *ctx) override;
            virtual antlrcpp::Any visitLiteralChar(HorseIRParser::LiteralCharContext *ctx) override;
            virtual antlrcpp::Any visitLiteralInteger(HorseIRParser::LiteralIntegerContext *ctx) override;
            virtual antlrcpp::Any visitLiteralFloat(HorseIRParser::LiteralFloatContext *ctx) override;
            virtual antlrcpp::Any visitLiteralSymbol(HorseIRParser::LiteralSymbolContext *ctx) override;
            virtual antlrcpp::Any visitLiteralTimeMonth(HorseIRParser::LiteralTimeMonthContext *ctx) override;
            virtual antlrcpp::Any visitLiteralTimeDate(HorseIRParser::LiteralTimeDateContext *ctx) override;
            virtual antlrcpp::Any visitLiteralTimeDateTime(HorseIRParser::LiteralTimeDateTimeContext *ctx) override;
            virtual antlrcpp::Any visitLiteralTimeMinute(HorseIRParser::LiteralTimeMinuteContext *ctx) override;
            virtual antlrcpp::Any visitLiteralTimeSecond(HorseIRParser::LiteralTimeSecondContext *ctx) override;
            virtual antlrcpp::Any visitLiteralTimeTime(HorseIRParser::LiteralTimeTimeContext *ctx) override;

            virtual antlrcpp::Any visitLiteralFunction(HorseIRParser::LiteralFunctionContext *ctx) override;

            virtual antlrcpp::Any visitNameId(HorseIRParser::NameIdContext *ctx) override;
        public:
            void printToken(antlr4::tree::TerminalNode *tokNode);
            void printToken(antlr4::Token *tok);
            void indent();
        private:
            std::ostream& strm ;
            int depth;
        } ;
    }
}
