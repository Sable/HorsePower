#include "CSTPrettyPrinter.h"

using CSTPrettyPrinter = horseIR::util::CSTPrettyPrinter ;

//antlrcpp::Any
//visitTypeFuncTemplate(HorseIRParser::TypeContext* 

std::ostream&
CSTPrettyPrinter::prettyPrint(antlr4::tree::ParseTree *parseTree)
{
    parseTree->accept(this) ;
    return strm ;
}

antlrcpp::Any
CSTPrettyPrinter::visitModule(HorseIRParser::ModuleContext *ctx) {
    strm << "module ";
    prettyPrint(ctx->name());
    strm << " ";
    std::vector<HorseIRParser::ContentContext *> contentList = ctx->content();
    strm << "{" << std::endl;
    for(auto it=contentList.begin(); it<contentList.end(); it++){
        (void) prettyPrint(*it);
    }
    strm << "}" << std::endl;
    return NULL;
}
antlrcpp::Any
CSTPrettyPrinter::visitMethod(HorseIRParser::MethodContext *ctx) {
    strm << "def ";
    prettyPrint(ctx->name());
    strm << "(";
    (void) prettyPrint(ctx->parameterList());
    strm << ") : ";
    (void) prettyPrint(ctx->type()); // return type
    std::vector<HorseIRParser::StatementContext *> statementList = ctx->statement();
    strm << "{" << std::endl;
    for(auto it=statementList.begin(); it<statementList.end(); it++){
        (void) prettyPrint(*it);
    }
    strm << "}" << std::endl;
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitParameterList(HorseIRParser::ParameterListContext *ctx) {
    auto names = ctx->name();
    auto types = ctx->type();
    int size = names.size();
    for(int i=0; i<size; i++){
        if(i>0) strm << ", ";
        prettyPrint(names[i]);
        strm << " : ";
        prettyPrint(types[i]);
    }
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitLabel(HorseIRParser::LabelContext *ctx) {
    strm << "[ ";
    prettyPrint(ctx->name());
    strm << " ]";
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitStmtCore(HorseIRParser::StmtCoreContext *ctx) {
    (void)prettyPrint(ctx->statementCore());
    strm << ";" << std::endl;
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitStmtLabel(HorseIRParser::StmtLabelContext *ctx) {
    prettyPrint(ctx->label());
    strm << std::endl;
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitStmtNameExpr(HorseIRParser::StmtNameExprContext *ctx){
    prettyPrint(ctx->name());
    strm << " : ";
    prettyPrint(ctx->type());
    strm << " = ";
    prettyPrint(ctx->expression());
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitStmtCNameExpr(HorseIRParser::StmtCNameExprContext *ctx) {
    // prettyPrint(ctx->compoundName());
    // strm << " : ";
    // prettyPrint(ctx->type());
    // strm << " = ";
    // prettyPrint(ctx->expression());
    return NULL;
}
antlrcpp::Any
CSTPrettyPrinter::visitStmtReturn(HorseIRParser::StmtReturnContext *ctx) {
    strm << "return ";
    prettyPrint(ctx->name());
    return NULL;
}
antlrcpp::Any
CSTPrettyPrinter::visitStmtGoto(HorseIRParser::StmtGotoContext *ctx) {
    strm << "goto ";
    prettyPrint(ctx->label());
    if(ctx->name()){
        prettyPrint(ctx->name());
    }
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitNameKey(HorseIRParser::NameKeyContext *ctx) {
    if(ctx->idKey != nullptr){
        strm << ctx->idKey->getText();
    }
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitExprBasicType(HorseIRParser::ExprBasicTypeContext *ctx) {
    strm << "( ";
    prettyPrint(ctx->type());
    strm << " )";
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitExprCheckType(HorseIRParser::ExprCheckTypeContext *ctx) {
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitExprCheckCast(HorseIRParser::ExprCheckCastContext *ctx) {
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitExprPhi(HorseIRParser::ExprPhiContext *ctx) {
    return NULL;
}


antlrcpp::Any
CSTPrettyPrinter::visitTypeCaseScalar(HorseIRParser::TypeCaseScalarContext *ctx) {
    strm << ctx->tokenValue->getText();
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitTypeCaseWildcard(HorseIRParser::TypeCaseWildcardContext *ctx) {
    strm << ctx->tokenValue->getText();
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitTypeCaseList(HorseIRParser::TypeCaseListContext *ctx) {
    strm << "list<";
    (void)prettyPrint(ctx->typeList());
    strm << ">";
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitTypeCaseDict(HorseIRParser::TypeCaseDictContext *ctx) {
    strm << "dict<";
    (void)prettyPrint(ctx->typeDict());
    strm << ">";
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitTypeCaseEnum(HorseIRParser::TypeCaseEnumContext *ctx) {
    strm << "enum<";
    (void)prettyPrint(ctx->typeEnum());
    strm << ">";
    return NULL;
}

/* function */
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

antlrcpp::Any
CSTPrettyPrinter::visitNameId(HorseIRParser::NameIdContext *ctx) {
    strm << ctx->ID()->getSymbol()->getText();
    return NULL;
}

