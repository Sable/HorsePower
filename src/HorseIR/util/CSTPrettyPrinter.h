#pragma once

#include <ostream>
#include <vector>
#include <utility>

#include "antlr4-runtime.h"
#include "../HorseIRBaseVisitor.h"
#include "../HorseIRParser.h"

namespace horseIR {
    namespace util {
        class CSTPrettyPrinter : protected HorseIRBaseVisitor {
        public:
            CSTPrettyPrinter() = delete ;
            CSTPrettyPrinter(std::ostream& outputStream)
                : strm(outputStream)
            {}

            std::ostream& prettyPrint(antlr4::tree::ParseTree* parseTree) ;
        protected:
            virtual antlrcpp::Any visitTypeFunc0(HorseIRParser::TypeFunc0Context* ctx) override ;
            virtual antlrcpp::Any visitTypeFunc1(HorseIRParser::TypeFunc1Context* ctx) override ;
            virtual antlrcpp::Any visitTypeFunc2(HorseIRParser::TypeFunc2Context* ctx) override ;
            virtual antlrcpp::Any visitTypeFunc3(HorseIRParser::TypeFunc3Context* ctx) override ;
        private:
            std::ostream& strm ;
        } ;
    }
}
