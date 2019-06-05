#include "../global.h"

#define outF stdout
#define comma ','

extern Node *entryMain;
static I depth, numArg;
static B needInd; // false
static List *lhsVars;

static void scanNode(Node *n);
static void scanList(List *list);
static void genList(List *list, C sep);

#define scanArgExpr(n) scanList(n->val.listS)
/* ------ declaration above ------ */

static char *monFnName[] = {
    "pfnAbs", "pfnNeg", "pfnCeil", "pfnFloor", "pfnRound", "pfnConj", "pfnRecip", "pfnSignum", "pfnPi"  , "pfnNot" ,
    "pfnLog", "pfnLog2", "pfnLog10", "pfnExp", "pfnTrigCos", "pfnTrigSin", "pfnTrigTan", "pfnTrigAcos", "pfnTrigAsin", "pfnTrigAtan", "pfnHyperCosh", "pfnHyperSinh",
    "pfnHyperTanh", "pfnHyperAcosh", "pfnHyperAsinh", "pfnHyperAtanh",
    "pfnDate", "pfnDateYear", "pfnDateMonth", "pfnDateDay",
    "pfnTime", "pfnTimeHour", "pfnTimeMinute", "pfnTimeSecond", "pfnTimeMill",
    "pfnUnique", NULL, "pfnLen", "pfnRange", "pfnFact", NULL, NULL, "pfnFlip", "pfnReverse",
    "pfnWhere", "pfnGroup", NULL, "pfnSum", "pfnAvg", "pfnMin", "pfnMax", "pfnRaze", "pfnEnlist", "pfnToList",
    NULL, "pfnKeys", "pfnValues", "pfnMeta", "pfnLoadTable", "pfnFetch", "pfnPrint"
};

static char *dyaFnName[] = {
    "pfnLt", "pfnGt", "pfnLeq", "pfnGeq", "pfnEq", "pfnNeq", "pfnPlus", "pfnMinus", "pfnMul", "pfnDiv",
    "pfnPower", "pfnLog2", "pfnMod", "pfnAnd", "pfnOr", "pfnNand", "pfnNor", "pfnXor",
    NULL, 
    "pfnAppend", "pfnLike", "pfnCompress", NULL, "pfnIndexOf", NULL, NULL, "pfnOrderBy",
    "pfnMember", "pfnVector", "pfnMatch", "pfnIndex", "pfnColumnValue", "pfnSubString"
};

static char *otherFnName[] = {
    "pfnEach", "pfnEachItem", "pfnEachLeft", "pfnEachRight", "pfnEnum", "pfnDict", "pfnTable",
    "pfnKTable", "pfnIndexA", "pfnList", "pfnOuter", "pfnJoinIndex", "pfnDatetimeAdd", "pfnDatetimeSub"
};

#define genStr(s) FP(outF, "%s", s)
#define genLine() genStr("\n");

static char code[1024];

static void genMethodHead(S n){ SP(code, "I horse_%s(){\n",n); genStr(code); }
static void genMethodTail(){ genStr("}\n"); }
static void genIndent(){ DOI(depth, P("    ")); }

// no dummy
static int countNode(List *list){
    if(list) return 1 + countNode(list->next);
    return 0;
}

static void genTic(){
    genIndent();
    genStr("tic();");
    genLine();
}

static void genToc(){
    genIndent();
    genStr("return toc();");
    //genStr("time_toc(\"The elapsed time: %g\\n\", elapsed);");
    genLine();
}

/* ------ gen functions defined above ------ */

static void scanExprStmt(Node *n){
    scanNode(n->val.exprStmt.expr);
}

static void scanFuncBuiltin(S func){
    FuncUnit x;
    getFuncIndexByName(func, &x);
    switch(x.kind){
        case 1: genStr(  monFnName[x.u]); break;
        case 2: genStr(  dyaFnName[x.b]); break;
        case 3: genStr(otherFnName[x.t]); break;
        default: EP("kind not found: %d\n", x.kind);
    }
}

static void scanFuncName(Node *n){
    S moduleName = n->val.name.id1;
    S methodName = n->val.name.id2;
    SymbolKind sk = n->val.name.sn->kind;
    if(sk == builtinS){
        scanFuncBuiltin(methodName);
    }
    else {
        TODO("Add support for a normal function name");
    }
}

static void scanCall(Node *n){
    I prevNumArg = numArg;
    if(needInd) genIndent();
    scanFuncName(n->val.call.func);
    genStr("(");
    genList(lhsVars, comma);
    numArg = countNode(lhsVars);
    scanNode(n->val.call.param);
    numArg = prevNumArg;
    genStr(");");
    genLine();
}

static void scanVar(Node *n){
    genStr(n->val.param.id);
    // n->val.param.id
    // n->val.param.typ
}

static void scanName(Node *n){
    if(numArg == 0) numArg = 1;
    else if(numArg > 0) { putchar(comma); numArg++; }
    P("%s", n->val.name.id2);
    // n->val.name.id1
    // n->val.name.id2
}

static void scanAssignStmt(Node *n){
    B prevNeedInd= needInd;
    List *prevVars = lhsVars;
    genIndent(); needInd = false;
    lhsVars = n->val.assignStmt.vars;
    scanNode(n->val.assignStmt.expr);
    needInd = prevNeedInd;
    lhsVars = prevVars;
}

static void genList(List *list, C sep){
    if(list){
        genList(list->next, sep);
        if(list->next) P("%c", sep);
        scanNode(list->val);
    }
}

static void scanVector(Node *n){
    if(numArg == 0) numArg = 1;
    else if(numArg > 0) { putchar(comma); numArg++; }
    I c = countNode(n->val.vec.val);
    if(c > 1) P("(");
    genList(n->val.vec.val, comma);
    if(c > 1) P(")");
}

static void scanConst(Node *n){
    ConstValue *v = n->val.nodeC;
    switch(v->type){
        case  strC: P("\"%s\"", v->valS); break;
        case  intC: P("%d"    , v->valI); break;
        case longC: P("%lld"  , v->valL); break;
        default: EP("Add more constant types: %d\n", v->type);
    }
}

static void scanReturn(Node *n){
    genIndent();
    if(n->val.listS){
        P("return ");
        genList(n->val.listS, comma);
        P(";");
    }
    else {
        P("return ;");
    }
    genLine();
}

static void scanNode(Node *n){
    switch(n->kind){
        case     callK: scanCall      (n); break;
        case exprstmtK: scanExprStmt  (n); break;
        case  argExprK: scanArgExpr   (n); break;
        case      varK: scanVar       (n); break;
        case     nameK: scanName      (n); break;
        case     stmtK: scanAssignStmt(n); break;
        case   vectorK: scanVector    (n); break;
        case    constK: scanConst     (n); break;
        case   returnK: scanReturn    (n); break;
        default: EP("Add more node types: %s\n", getNodeTypeStr(n));
    }
}

static void scanList(List *list){
    if(list){
        scanList(list->next);
        scanNode(list->val);
    }
}

static void compileChain(Chain *chain){
    scanNode(chain->cur);
}

static void compileChainList(ChainList *list){
    if(list){
        compileChainList(list->next);
        //printChain(list->chain);
        compileChain(list->chain);
    }
}

static void printSymbolNameList(SymbolNameList *list){
    if(list){
        printSymbolNameList(list->next);
        printSymbolName(list->symName);
    }
}

static void compileMethod(Node *n){
    ChainList *chains = n->val.method.meta->chains;
    //printChainList(chains);
    printSymbolNameList(n->val.method.meta->localVars);
    genMethodHead(n->val.method.fname); // TODO: method name
    depth++;
    //if(n == entryMain) genTic();
    compileChainList(chains);
    //if(n == entryMain) genToc();
    depth--;
    genMethodTail();
}

static void init(){
    depth = 0;
    numArg = -1;
    needInd = true;
    lhsVars = NULL;
}

I HorseCompiler(){
    printBanner("Compiling");
    init();
    compileMethod(entryMain);
    //if(H_DEBUG) printChainList(); // display visited chains
    R 0;
}
