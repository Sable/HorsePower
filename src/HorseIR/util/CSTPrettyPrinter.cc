#include "CSTPrettyPrinter.h"

using CSTPrettyPrinter = horseIR::util::CSTPrettyPrinter ;

antlrcpp::Any
visitTypeFuncTemplate(HorseIRParser::TypeContext* 

std::ostream&
CSTPrettyPrinter::prettyPrint(antlr4::tree::ParseTree *parseTree)
{
    parseTree->accept(this) ;
    return strm ;
}

antlrcpp::Any
CSTPrettyPrinter::visitTypeFunc0(HorseIRParser::TypeFunc0Context *ctx)
{
    strm << "func<:" ;
    const auto typePtr = static_cast<HorseIRParser::TypeContext*>(ctx->type()) ;
    (void) prettyPrint(typePtr) ;
    strm << ">" ;
    return nullptr ;
}

antlrcpp::Any
CSTPrettyPrinter::visitTypeFunc1(HorseIRParser::TypeFunc1Context *ctx)
{
    strm << "func<... :" ;
    const auto typePtr = static_cast<HorseIRParser::TypeContext*>(ctx->type()) ;
    (void) prettyPrint(typePtr) ;
    strm << ">" ;
    return nullptr ;
}

antlrcpp::Any
CSTPrettyPrinter::visitTypeFunc2(HorseIRParser::TypeFunc2Context *ctx)
{
    strm << "func<" ;
    std::vector<HorseIRParser::TypeContext*> types(std::move(ctx->type())) ;
    for (auto iter = types.cbegin(); iter != types.cend(); ++iter) {
        const auto ptr = static_cast<HorseIRParser::TypeContext*>(*iter) ;
        if (iter == types.cbegin()) {
            (void) prettyPrint(ptr) ;
        } else if (iter + 1 == types.cend()) {
            strm << " :" ;
            (void) prettyPrint(ptr) ;
        } else {
            strm << ", " ;
            (void) prettyPrint(ptr) ;
        }
    }
    strm << ">" ;
    return nullptr ;
}

antlrcpp::Any
CSTPrettyPrinter::visitTypeFunc3(HorseIRParser::TypeFunc3Context *ctx)
{
    strm << "func<" ;
    std::vector<HorseIRParser::TypeContext*> types(std::move(ctx->type())) ;
    for (auto iter = types.cbegin(); iter != types.cend(); ++iter) {
        const auto ptr = static_cast<HorseIRParser::TypeContext*>(*iter) ;
        if (iter == types.cbegin()) {
            (void) prettyPrint(ptr) ;
        } else if (iter + 1 == types.cend()) {
            strm << "... :" ;
            (void) prettyPrint(ptr) ;
        } else {
            strm << ", " ;
            (void) prettyPrint(ptr) ;
        }
    }
    strm << ">" ;
    return nullptr ;
    
}
