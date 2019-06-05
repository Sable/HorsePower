#include "../global.h"

#define outF stdout

extern Node *entryMain;
static I depth; // 0
static B lhs; // false

static void scanNode(Node *n);
static void scanList(List *list);

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

static void genMethodHead(){ genStr("E compiled_query(){\n"); }
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
    if(!lhs) genIndent();
    scanFuncName(n->val.call.func);
    genStr("(");
    scanNode(n->val.call.param);
    genStr(");");
    genLine();
}

static void scanVar(Node *n){
    // n->val.param.id
    // n->val.param.typ
}

static void scanName(Node *n){
    P("%s\n", n->val.name.id2);
    // n->val.name.id1
    // n->val.name.id2
}

static void scanAssignStmt(Node *n){
    genIndent();
    lhs = true;
    scanList(n->val.assignStmt.vars);
    lhs = false;
    scanNode(n->val.assignStmt.expr);
}

static void scanVector(Node *n){
    scanList(n->val.vec.val);
}

static void scanConst(Node *n){
    ConstValue *v = n->val.nodeC;
    switch(v->type){
        case strC: P("\"%s\"", v->valS); break;
        default: EP("Add more constant types: %d\n", v->type);
    }
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

static void compileMethod(Node *n){
    ChainList *chains = n->val.method.meta->chains;
    //printChainList(chains);
    genMethodHead();
    depth++;
    if(n == entryMain) genTic();
    compileChainList(chains);
    if(n == entryMain) genToc();
    depth--;
    genMethodTail();
}

static void init(){
    depth = 0;
    lhs = false;
}

I HorseCompiler(){
    printBanner("Compiling");
    init();
    compileMethod(entryMain);
    //if(H_DEBUG) printChainList(); // display visited chains
    R 0;
}
