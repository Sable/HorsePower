#include "../global.h"

#define out_stream stdout
#define comma ','

extern Node *entryMain;
static I depth, numArg;
static B needInd;
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

#define genStr(s) FP(out_stream, "%s", s)
#define genLine() genStr("\n");

#define glueCode(s)  strcat(ptr,s)
#define glueLine()   strcat(ptr,"\n")
#define cleanCode()  code[0]=0
#define resetCode()  if(ptr[0]!=0) ptr+=strlen(ptr)
#define glueChar(c)  do{resetCode(); ptr[0]=c; ptr[1]=0; ptr++;}while(0)
#define glueInt(x)   do{resetCode(); SP(ptr, "%d", x);     }while(0)
#define glueLong(x)  do{resetCode(); SP(ptr, "%lld", x);   }while(0)
#define glueAny(...) do{resetCode(); SP(ptr, __VA_ARGS__); }while(0)
#define glueCodeLine(x)  do{genIndent(); resetCode(); SP(ptr, "%s\n",x);}while(0)
#define glueAnyLine(...) do{genIndent(); glueAny(__VA_ARGS__);}while(0)

typedef struct CodeList{
    char code[128];
    struct CodeList *next;
}CodeList;

#define CODE_MAX_SIZE 10240
static char code[CODE_MAX_SIZE], *ptr;
static CodeList *codeList;

static void genIndent(){ DOI(depth, glueCode("    ")); }

static int countInfoNodeList(InfoNodeList *list){
    if(list) return 1 + countInfoNodeList(list->next); return 0;
}

static void genMethodHead(Node *n){
    glueAny("I horse_%s(", n->val.method.fname);
    int total = countInfoNodeList(n->val.method.meta->returnTypes);
    if(total > 0) glueCode("V *h_rtn");
    glueCodeLine("){");
}

static void addToCodeList(CodeList *list, CodeList *c){
    c->next = list->next;
    list->next = c;
}

static void storeCode(S s){
    if(strlen(code) < 128) {
        CodeList *c = NEW(CodeList);
        SP(c->code, "%s", s);
        addToCodeList(codeList, c);
    }
    else EP("A line of the code should be less than 128\n");
}

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

static void genList(List *list, C sep){
    if(list){
        genList(list->next, sep);
        if(list->next) glueChar(sep);
        scanNode(list->val);
    }
}

static int genListReturn(List *list, C sep, S text){
    if(list){
        int k = genListReturn(list->next, sep, text);
        glueAny("%s[%d]=",text,k);
        scanNode(list->val);
        glueChar(sep);
        return k+1;
    }
    return 0;
}

char getTypeAlias(Type t){
    switch(t){
        case boolT: return 'B';
        case   i8T: return 'J';
        case  i16T: return 'H';
        case  i32T: return 'I';
        case  i64T: return 'L';
        case  f32T: return 'F';
        case  f64T: return 'E';
        case  strT: return 'S';
        case  symT: return 'Q';
        case charT: return 'C';
        case dateT: return 'D';
        default: EP("Add more types: %d\n", t);
    }
}

static char getTypeAliasFromNode(Node *n){
    InfoNode *in = n->val.type.in;
    if(!in->subInfo) return getTypeAlias(in->type);
    else EP("Type problem");
}

static char *genConstFunc(Node *n){
    InfoNode *in = n->val.type.in;
    if(!in->subInfo){
        switch(in->type){
            case boolT: return "LiteralBool";
            case   i8T: return "LiteralI8";
            case  i16T: return "LiteralI16";
            case  i32T: return "LiteralI32";
            case  i64T: return "LiteralI64";
            case  f32T: return "LiteralF32";
            case  f64T: return "LiteralF64";
            case  strT: return "LiteralString";
            case  symT: return "LiteralSymbol";
            case charT: return "LiteralChar";
            case dateT: return "LiteralDate";
            default: EP("Add more types: %d\n", in->type);
        }
        return NULL;
    }
    else EP("Type problem");
}

static void addStrConst(S s){
    L size = strlen(s) + 2 + 1;
    L preSize = ptr - code;
    if(size + preSize > CODE_MAX_SIZE)
        EP("size is more than expected: %lld vs. %d\n", size + preSize, CODE_MAX_SIZE);
    SP(ptr, "\"%s\"", s);
}

/* ------ gen functions defined above ------ */

static void scanExprStmt(Node *n){
    scanNode(n->val.exprStmt.expr);
}

static void scanFuncBuiltin(S func){
    FuncUnit x;
    getFuncIndexByName(func, &x);
    switch(x.kind){
        case 1: glueCode(  monFnName[x.u]); break;
        case 2: glueCode(  dyaFnName[x.b]); break;
        case 3: glueCode(otherFnName[x.t]); break;
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
    scanFuncName(n->val.call.func);
    glueCode("(");
    genList(lhsVars, comma);
    numArg = countNode(lhsVars);
    scanNode(n->val.call.param);
    numArg = prevNumArg;
    glueCode(")");
}

static void scanVar(Node *n){
    glueCode(n->val.param.id);
    // n->val.param.id
    // n->val.param.typ
}

static void scanName(Node *n){
    if(numArg == 0) numArg = 1;
    else if(numArg > 0) { glueChar(comma); numArg++; }
    glueCode(n->val.name.id2);
    // n->val.name.id1
    // n->val.name.id2
}

static void scanAssignStmt(Node *n){
    B prevNeedInd= needInd;
    needInd = false;
    List *prevVars = lhsVars;
    lhsVars = n->val.assignStmt.vars;
    scanNode(n->val.assignStmt.expr);
    needInd = prevNeedInd;
    lhsVars = prevVars;
}

static void scanVector(Node *n){
    if(numArg == 0) numArg = 1;
    else if(numArg > 0) { glueChar(comma); numArg++; }
    Node *typeNode = n->val.vec.typ;
    I c = countNode(n->val.vec.val);
    glueCode(genConstFunc(typeNode));
    if(c == 1){
        glueCode("(");
        genList(n->val.vec.val, comma);
        glueCode(")");
    }
    else if(c > 1){
        glueCode("Vector(");
        resetCode(); SP(ptr, "%d, (%c []){", c, getTypeAliasFromNode(typeNode));
        genList(n->val.vec.val, comma);
        glueCode("})");
    }
}

static void scanConst(Node *n){
    char temp[99]; temp[0]=0;
    ConstValue *v = n->val.nodeC;
    switch(v->type){
        case  strC: addStrConst(v->valS); break;
        case  intC: SP(temp, "%d"  , v->valI); break;
        case longC: SP(temp, "%lld", v->valL); break;
        default: EP("Add more constant types: %d\n", v->type);
    }
    if(temp[0]) glueCode(temp);
}

// TODO: think how to return multiple values
static void scanReturn(Node *n){
    if(n->val.listS){
        glueCode("return ");
        genListReturn(n->val.listS, comma, "h_rtn");
        glueCode("0;");
    }
    else {
        glueCode("return ;");
    }
}

static void genStatement(Node *n, B f){
    switch(n->kind){
        case   callK: if(f){ if(needInd) genIndent(); }
                      else { if(needInd) { glueCode(";"); glueLine(); }  } break;
        case returnK: if(f) genIndent();
                      else glueLine(); break;
        case   stmtK: if(f) { genIndent(); glueCode("CHECKE("); }
                      else { glueCode(");"); glueLine(); } break;
    }
}

static void scanNode(Node *n){
    resetCode();
    genStatement(n, true);
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
    genStatement(n, false);
}

static void scanList(List *list){
    if(list){
        scanList(list->next);
        scanNode(list->val);
    }
}

// TODO: utilize information stored in chains
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
    // show the list of all local variables
    //printSymbolNameList(n->val.method.meta->localVars);
    //genMethodHead(n->val.method.fname); // TODO: method name
    genMethodHead(n);
    depth++;
    //if(n == entryMain) genTic();
    compileChainList(chains);
    //if(n == entryMain) genToc();
    depth--;
    glueCodeLine("}\n");
}

static void genEntry(){
    resetCode();
    int numRtns = countInfoNodeList(entryMain->val.method.meta->returnTypes);
    glueCodeLine("I horse_entry(){");
    depth++;
    if(numRtns > 0){
        glueCodeLine("V rtns[99];");
        glueCodeLine("tic();");
        glueCodeLine("horse_main(rtns);");
        glueCodeLine("time_toc(\"The elapsed time (ms): %g\\n\", elapsed);");
        glueCodeLine("P(\"Output:\\n\");");
        glueAnyLine("DOI(%d, printV(rtns[i]))\n", numRtns);
    }
    glueCodeLine("return 0;");
    depth--;
    glueCodeLine("}");
}

static void init(){
    depth    = 0;
    numArg   = -1;
    needInd  = true;
    lhsVars  = NULL;
    codeList = NEW(CodeList);
    ptr      = code;
    code[0]  = 0;
}

static void result(){
    resetCode();
    I size = ptr - code;
    P("%s\n",code);
    P("Profile:\n>> Used buffer %.2lf%% [%d/%d]\n", \
            percent(size,CODE_MAX_SIZE), size, CODE_MAX_SIZE);
}

I HorseCompiler(){
    printBanner("Compiling");
    init();
    compileMethod(entryMain);
    genEntry();
    //if(H_DEBUG) printChainList(); // display visited chains
    result();
    R 0;
}
