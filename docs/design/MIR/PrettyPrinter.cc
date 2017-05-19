#include <cstddef>  /* for std::size_t */
#include <utility>  /* for std::move   */
#include <vector>

#include "PrettyPrinter.h"

using mir::util::PrettyPrinter ;

void
PrettyPrinter::prettyPrint(antlr4::tree::ParseTree *parseTree)
{
    parseTree->accept(this) ;
}

antlrcpp::Any
PrettyPrinter::visitTypeCaseScalar(MIRParser::TypeCaseScalarContext* ctx)
{
    outputStream << ctx->tokenValue->getText() ;
    return nullptr ;
}

antlrcpp::Any
PrettyPrinter::visitTypeCaseWildcard(MIRParser::TypeCaseWildcardContext *ctx)
{
    outputStream << ctx->tokenValue->getText() ;
    return nullptr ;
}

antlrcpp::Any
PrettyPrinter::visitTypeList(MIRParser::TypeListContext *ctx)
{
    outputStream << "list<" ;
    auto ptr = static_cast<antlr4::tree::ParseTree*>(ctx->element) ;
    prettyPrint(ptr) ;
    outputStream << ">" ;
    return nullptr ;
}

antlrcpp::Any
PrettyPrinter::visitTypeDict(MIRParser::TypeDictContext *ctx)
{
    outputStream << "dict<" ;
    auto keyPtr = static_cast<antlr4::tree::ParseTree*>(ctx->key) ;
    prettyPrint(keyPtr) ;
    outputStream << ", " ;
    auto valuePtr = static_cast<antlr4::tree::ParseTree*>(ctx->value) ;
    outputStream << ">" ;
    return nullptr ;
}

antlrcpp::Any
PrettyPrinter::visitTypeEnum(MIRParser::TypeEnumContext *ctx)
{
    outputStream << "enum<" ;
    auto ptr = static_cast<antlr4::tree::ParseTree*>(ctx->element) ;
    prettyPrint(ptr) ;
    outputStream << ">" ;
    return nullptr ;
}

antlrcpp::Any
PrettyPrinter::visitTypeFuncCase0(MIRParser::TypeFuncCase0Context *ctx)
{
    outputStream << "func< :" ;
    auto ptr = static_cast<antlr4::tree::ParseTree*>(ctx->type()) ;
    prettyPrint(ptr) ;
    outputStream << ">" ;
    return nullptr ;
}

antlrcpp::Any
PrettyPrinter::visitTypeFuncCase1(MIRParser::TypeFuncCase1Context *ctx)
{
    outputStream << "func<... :" ;
    auto ptr = static_cast<antlr4::tree::ParseTree*>(ctx->type()) ;
    prettyPrint(ptr) ;
    outputStream << ">" ;
    return nullptr ;
}

antlrcpp::Any
PrettyPrinter::visitTypeFuncCase2(MIRParser::TypeFuncCase2Context *ctx)
{
    outputStream << "func<" ;
    const std::vector<MIRParser::TypeContext*> types(std::move(ctx->type())) ;
    for (auto iter = types.cbegin(); iter != types.cend(); ++iter) {
        auto ptr = static_cast<antlr4::tree::ParseTree*>(*iter) ;
        if (iter == types.cbegin()) {
            prettyPrint(ptr) ;
        } else if (iter + 1 == types.cend()) {
            outputStream << " :";
            prettyPrint(ptr) ;
        } else {
            outputStream << ", ";
        }
    }
    outputStream << ">" ;
    return nullptr ;
}

antlrcpp::Any
PrettyPrinter::visitTypeFuncCase3(MIRParser::TypeFuncCase3Context *ctx)
{
    outputStream << "func<" ;
    const std::vector<MIRParser::TypeContext*> types(std::move(ctx->type())) ;
    for (auto iter = types.cbegin(); iter != types.cend(); ++iter) {
        auto ptr = static_cast<antlr4::tree::ParseTree*>(*iter) ;
        if (iter == types.cbegin()) {
            prettyPrint(ptr) ;
        } else if (iter + 1 == types.cend()) {
            outputStream << ",... :";
            prettyPrint(ptr) ;
        } else {
            outputStream << ", ";
        }
    }
    outputStream << ">" ;
    return nullptr ;
}
