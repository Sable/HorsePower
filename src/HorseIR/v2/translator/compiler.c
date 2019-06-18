#include "../global.h"

#define out_stream stdout
#define comma ','

extern Node *entryMain;

static I depth, numArg;
static B needInd;
static List *lhsVars;
static Node *currentMethod;

static void scanNode(Node *n);
static void scanList(List *list);
static void genList(List *list, C sep);
static void genVarList(List *list, C sep);
extern List *compiledMethodList;

#define scanArgExpr(n)   scanList(n->val.listS)
#define scanParamExpr(n) scanList(n->val.listS)
#define scanBlock(n)     scanList(n->val.listS)
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
#define glueCodeLine(x)  do{genIndent(); resetCode(); SP(ptr, "%s\n",x); }while(0)
#define glueAnyLine(...) do{genIndent(); glueAny(__VA_ARGS__);glueLine();}while(0)

#define isTrueMacro     "isTrue"
#define scanBreak(n)    glueCodeLine("break")
#define scanContinue(n) glueCodeLine("continue")

typedef struct CodeList{
    char code[128];
    struct CodeList *next;
}CodeList;

#define CODE_MAX_SIZE 10240
#define HEAD_MAX_SIZE 1024
static char code[CODE_MAX_SIZE], *ptr;
static char head[HEAD_MAX_SIZE], *htr;
static CodeList *codeList;

static void genIndent(){ DOI(depth, glueCode("    ")); }

static void glueMethodHead(S part1, S part2){
    if(htr[0]!=0) htr+=strlen(htr);
    SP(htr, "%s%s\n", part1, part2);
}

static int countInfoNodeList(InfoNodeList *list){
    if(list) return 1 + countInfoNodeList(list->next); return 0;
}

static void genMethodHead(Node *n){
    resetCode(); char *line = ptr;
    glueAny("static I horse_%s(", n->val.method.fname);
    int numRtn   = countInfoNodeList(n->val.method.meta->returnTypes);
    int numParam = totalSymbolNames(n->val.method.meta->paramVars);
    if(numRtn > 0)
        glueCode("V *h_rtn");
    if(numParam > 0){
        if(numRtn > 0) glueCode(",");
        genVarList(n->val.method.param->val.listS, comma);
    }
    glueMethodHead(line, ");\n");
    glueCode("){");
    glueLine();
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

static void genVarList(List *list, C sep){
    if(list){
        genList(list->next, sep);
        if(list->next) glueChar(sep);
        glueCode("V ");
        scanNode(list->val);
    }
}

static int genListReturn(List *list, C sep, S text){
    if(list){
        int k = genListReturn(list->next, sep, text);
        if(list->next) glueChar(sep);
        glueAny("%s[%d]=",text,k);
        scanNode(list->val);
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

static void genLocalVar(SymbolName *sn){
    glueAnyLine("V %s = incV();", sn->name);
}

static void genLocalVars(Node *block){
    SymbolTable *st = block->val.block.st;
    DOI(SymbolTableSize, if(st->table[i]) genLocalVar(st->table[i]))
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

static void scanFuncMethod(S func){
    glueCode(func);
}

static void scanFuncName(Node *n){
    S moduleName = n->val.name.id1;
    S methodName = n->val.name.id2;
    SymbolKind sk = n->val.name.sn->kind;
    switch(sk){
        case builtinS: scanFuncBuiltin(methodName); break;
        case  methodS: scanFuncMethod (methodName); break;
        default:
        TODO("Add support for symbol kind: %d\n", sk); break;
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
    B notLast = (n != currentMethod->val.method.meta->lastStmt);
    if(n->val.listS){
        //glueCode("return ");
        genListReturn(n->val.listS, comma, "h_rtn");
        glueChar(';');
    }
    if(notLast) glueCode("goto END;");
}

static void scanIf(Node *n){
    genIndent();
    glueCode("if(");
    glueCode(isTrueMacro); // isTrue is a macro
    glueCode("(");
    scanNode(n->val.ifStmt.condExpr);
    glueCode(")){\n");
    Node *thenBlock = n->val.ifStmt.thenBlock;
    Node *elseBlock = n->val.ifStmt.elseBlock;
    depth++;
    scanNode(thenBlock);
    depth--;
    glueCodeLine("}");
    if(elseBlock){
        glueCodeLine("else {");
        depth++;
        scanNode(elseBlock);
        depth--;
        glueCodeLine("}");
    }
}

static void scanWhile(Node *n){
    genIndent();
    glueCode("while(");
    glueCode(isTrueMacro);
    glueCode("(");
    scanNode(n->val.whileStmt.condExpr);
    glueCode(")){\n");
    depth++;
    scanNode(n->val.whileStmt.bodyBlock);
    depth--;
    glueCodeLine("}");
}

static void scanRepeat(Node *n){
    genIndent();
    glueCode("repeat(");
    glueCode(isTrueMacro);
    glueCode("(");
    scanNode(n->val.repeatStmt.condExpr);
    glueCode(")){\n");
    depth++;
    scanNode(n->val.repeatStmt.bodyBlock);
    depth--;
    glueCodeLine("}");
}

static void genStatement(Node *n, B f){
    switch(n->kind){
        case   callK: if(f){ if(needInd) genIndent(); }
                      else { if(needInd) { glueCode(";"); glueLine(); }  } break;
        case returnK: if(f) genIndent();
                      else glueLine(); break;
        case   stmtK: if(f) { genIndent(); glueCode("PROFILE("); }
                      else { glueCode(");"); glueLine(); } break;
    }
}

static void scanNode(Node *n){
    resetCode();
    genStatement(n, true);
    switch(n->kind){
        case      callK: scanCall      (n); break;
        case  exprstmtK: scanExprStmt  (n); break;
        case   argExprK: scanArgExpr   (n); break;
        case paramExprK: scanParamExpr (n); break;
        case       varK: scanVar       (n); break;
        case      nameK: scanName      (n); break;
        case      stmtK: scanAssignStmt(n); break;
        case    vectorK: scanVector    (n); break;
        case     constK: scanConst     (n); break;
        case    returnK: scanReturn    (n); break;
        case        ifK: scanIf        (n); break;
        case     whileK: scanWhile     (n); break;
        case    repeatK: scanRepeat    (n); break;
        case     breakK: scanBreak     (n); break;
        case  continueK: scanContinue  (n); break;
        case     blockK: scanBlock     (n); break;
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

static int compileChainList(ChainList *list){
    if(list){
        compileChainList(list->next);
        //printChain(list->chain); getchar();
        compileChain(list->chain);
    }
    return 0;
}

static void compileMethod(Node *n){
    Node *prevMethod = currentMethod;
    currentMethod = n;
    ChainList *chains = n->val.method.meta->chains;
    MetaMethod *meta = n->val.method.meta;
    meta->isCompiled = true;
    //printChainList(chains);
    // show the list of all local variables
    //printSymbolNameList(n->val.method.meta->localVars);
    //genMethodHead(n->val.method.fname); // TODO: method name
    genMethodHead(n);
    depth++;
    glueCodeLine("I prevBuffS = buffS;");
    genLocalVars(n->val.method.block);
    //if(n == entryMain) genTic();
    // stategy 1: naive
    scanNode(n->val.method.block);
    // stategy 2: optimize with chain info
    //compileChainList(chains);
    if(!instanceOf(n->val.method.meta->lastStmt, returnK))
        glueCodeLine("END:");
    glueCodeLine("buffS = prevBuffS;");
    glueCodeLine("return 0;");
    //if(n == entryMain) genToc();
    depth--;
    glueCodeLine("}\n");
    currentMethod = prevMethod;
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
        glueAnyLine("DOI(%d, printV(rtns[i]))", numRtns);
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
    htr      = head;
    head[0]  = 0;
}

static void saveToFile(S path, S head, S code){
    printBanner("Generated Code Below");
    P("%s%s", head,code);
    return ;
    FILE *fp = fopen(path, "w");
    FP(fp, "%s%s", head,code);
    fclose(fp);
}

static void result(){
    resetCode();
    I size = ptr - code;
    P("Profile:\n>> Used buffer %.2lf%% [%d/%d]\n", \
            percent(size,CODE_MAX_SIZE), size, CODE_MAX_SIZE);
    saveToFile("out/gen.h", head, code);
}

static void compileMethods(List *list){
    if(list){ compileMethods(list->next); compileMethod(list->val); }
}

I HorseCompiler(){
    printBanner("Compiling");
    init();
    compileMethods(compiledMethodList->next);
    genEntry();
    //if(H_DEBUG) printChainList(); // display visited chains
    result();
    R 0;
}
