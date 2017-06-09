#include "CSTPrettyPrinter.h"

using CSTPrettyPrinter = horseIR::util::CSTPrettyPrinter ;

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
    depth++;
    for(auto it=contentList.begin(); it<contentList.end(); it++){
        indent();
        (void) prettyPrint(*it);
    }
    depth--;
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
    depth++;
    for(auto it=statementList.begin(); it<statementList.end(); it++){
        indent();
        (void) prettyPrint(*it);
    }
    depth--;
    indent(); strm << "}" << std::endl;
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
        strm << ":";
        prettyPrint(types[i]);
    }
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitGlobalVar(HorseIRParser::GlobalVarContext *ctx) {
    strm << "def ";
    prettyPrint(ctx->name());
    strm << ":";
    prettyPrint(ctx->type());
    strm << ";" << std::endl;
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitImportCID(HorseIRParser::ImportCIDContext *ctx) {
    strm << "import";
    printToken(ctx->IMPORT_COMPOUND_ID());
    strm << ";" << std::endl;
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitImportID(HorseIRParser::ImportIDContext *ctx) {
    strm << "import";
    printToken(ctx->COMPOUND_ID());
    strm << ";" << std::endl;
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
    prettyPrint(ctx->generalName());
    strm << ":";
    prettyPrint(ctx->type());
    strm << " = ";
    prettyPrint(ctx->expression());
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
        strm << " ";
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
    strm << "(";
    prettyPrint(ctx->type());
    strm << ")";
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitExprCheckType(HorseIRParser::ExprCheckTypeContext *ctx) {
    strm << "check_type" << "(";
    prettyPrint(ctx->methodCall());
    strm << ", ";
    prettyPrint(ctx->type());
    strm << ")";
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitExprCheckCast(HorseIRParser::ExprCheckCastContext *ctx) {
    strm << "check_cast" << "(";
    prettyPrint(ctx->methodCall());
    strm << ", ";
    prettyPrint(ctx->type());
    strm << ")";
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitExprPhi(HorseIRParser::ExprPhiContext *ctx) {
    strm << "phi" << "(";
    std::vector<HorseIRParser::LabelContext *> labels = ctx->label();
    std::vector<HorseIRParser::NameContext *>  names  = ctx->name();
    int len = labels.size();
    for(int i=0; i<len; ++i){
        if(i!=0) strm << ", ";
        prettyPrint(labels[i]);
        strm << " ";
        prettyPrint(names[i]);
    }
    strm << ")";
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
CSTPrettyPrinter::visitMethodInv(HorseIRParser::MethodInvContext *ctx) {
    prettyPrint(ctx->generalName());
    strm << "(";
    prettyPrint(ctx->argumentList());
    strm << ")";
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitMethodFun(HorseIRParser::MethodFunContext *ctx) {
    prettyPrint(ctx->literalFunction());
    strm << "(";
    prettyPrint(ctx->argumentList());
    strm << ")";
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitArgumentList(HorseIRParser::ArgumentListContext *ctx) {
    std::vector<HorseIRParser::OperandContext *> content = ctx->operand();
    int i = 0;
    for(auto it = content.begin(); it != content.end(); ++it, ++i){
        if(i>0) strm << ", ";
        prettyPrint(*it);
    }
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitCompoundName(HorseIRParser::CompoundNameContext *ctx) {
    printToken(ctx->COMPOUND_ID());
    return NULL;
}

/* literals */
antlrcpp::Any
CSTPrettyPrinter::visitLiteralNil(HorseIRParser::LiteralNilContext *ctx) {
    printToken(ctx->value);
    strm << ":";
    prettyPrint(ctx->valueType);
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitLiteralComplex(HorseIRParser::LiteralComplexContext *ctx) {
    if(ctx->opReal){
        if(ctx->opIm == nullptr){ // 2:complex
            if(ctx->opReal) printToken(ctx->opReal);
            printToken(ctx->real);
        }
        else {
            if(ctx->opReal) printToken(ctx->opReal);
            printToken(ctx->real);
            printToken(ctx->opIm);
            if(ctx->im) printToken(ctx->im);
            printToken(ctx->unit);
        }
    }
    else {
        if(ctx->opIm) printToken(ctx->opIm);
        if(ctx->im){
            printToken(ctx->im);
        }
        printToken(ctx->unit);
    }
    strm << ":";
    printToken(ctx->valueType);
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitLiteralBool(HorseIRParser::LiteralBoolContext *ctx) {
    if(ctx->op){
        printToken(ctx->op);
    }
    printToken(ctx->value);
    strm << ":";
    printToken(ctx->valueType);
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitLiteralChar(HorseIRParser::LiteralCharContext *ctx) {
    printToken(ctx->value);
    if(ctx->valueType){
        strm << ":";
        printToken(ctx->valueType);
    }
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitLiteralInteger(HorseIRParser::LiteralIntegerContext *ctx) {
    if(ctx->op != nullptr) printToken(ctx->op);
    printToken(ctx->value);
    strm << ":";
    printToken(ctx->valueType);
    return NULL;
}
antlrcpp::Any
CSTPrettyPrinter::visitLiteralFloat(HorseIRParser::LiteralFloatContext *ctx) {
    if(ctx->op != nullptr) printToken(ctx->op);
    printToken(ctx->value);
    strm << ":";
    printToken(ctx->valueType);
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitLiteralSymbol(HorseIRParser::LiteralSymbolContext *ctx) {
    printToken(ctx->value);
    if(ctx->valueType != nullptr){
        strm << ":";
        printToken(ctx->valueType);
    }
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitLiteralTimeMonth(HorseIRParser::LiteralTimeMonthContext *ctx) {
    printToken(ctx->value);
    strm << ":";
    printToken(ctx->valueType);
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitLiteralTimeDate(HorseIRParser::LiteralTimeDateContext *ctx) {
    printToken(ctx->value);
    strm << ":";
    printToken(ctx->valueType);
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitLiteralTimeDateTime(HorseIRParser::LiteralTimeDateTimeContext *ctx) {
    printToken(ctx->value);
    if(ctx->valueType){
        strm << ":";
        printToken(ctx->valueType);
    }
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitLiteralTimeMinute(HorseIRParser::LiteralTimeMinuteContext *ctx) {
    printToken(ctx->value);
    strm << ":";
    printToken(ctx->valueType);
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitLiteralTimeSecond(HorseIRParser::LiteralTimeSecondContext *ctx) {
    printToken(ctx->value);
    strm << ":";
    printToken(ctx->valueType);
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitLiteralTimeTime(HorseIRParser::LiteralTimeTimeContext *ctx) {
    printToken(ctx->value);
    if(ctx->valueType){
        strm << ":";
        printToken(ctx->valueType);
    }
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitLiteralFunction(HorseIRParser::LiteralFunctionContext *ctx) {
    printToken(ctx->value);
    if(ctx->valueType){
        prettyPrint(ctx->valueType);
    }
    return NULL;
}

antlrcpp::Any
CSTPrettyPrinter::visitNameId(HorseIRParser::NameIdContext *ctx) {
    strm << ctx->ID()->getSymbol()->getText();
    return NULL;
}

/* helper functions */
void
CSTPrettyPrinter::printToken(antlr4::tree::TerminalNode *tokNode) {
    strm << tokNode->getSymbol()->getText();
}

void
CSTPrettyPrinter::printToken(antlr4::Token *tok) {
    strm << tok->getText();
}

void
CSTPrettyPrinter::indent(){
    int n = 4;
    for(int i=0, i2=depth*n; i<i2; ++i){
        strm << " ";
    }
}

