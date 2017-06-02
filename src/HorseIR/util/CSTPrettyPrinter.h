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
            virtual antlrcpp::Any visitTypeFunc0(HorseIRParser::TypeFunc0Context* ctx) override ;
            virtual antlrcpp::Any visitTypeFunc1(HorseIRParser::TypeFunc1Context* ctx) override ;
            virtual antlrcpp::Any visitTypeFunc2(HorseIRParser::TypeFunc2Context* ctx) override ;
            virtual antlrcpp::Any visitTypeFunc3(HorseIRParser::TypeFunc3Context* ctx) override ;
        private:
            std::ostream& strm ;
        } ;
    }
}
