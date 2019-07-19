#include "../global.h"

typedef struct CodeList{
    C code[128];
    struct CodeList *next;
}CodeList;

#define comma ','

extern Node *entryMain;

static I depth, numArg;
static B needInd;
static List *lhsVars;
static Node *currentMethod;

static void scanNode(Node *n);
static void scanList(List *list);
static void scanList1(List *list, Node *first);
static void genList(List *list, C sep);
static void genVarList(List *list, C sep);

extern List *compiledMethodList;
extern OC   qOpts[99];
extern I    numOpts;
extern sHashTable *hashOpt;

#define scanArgExpr(n)   scanList(n->val.listS)
#define scanParamExpr(n) scanList(n->val.listS)
#define scanCast(n)      scanNode(n->val.cast.exp)

// two macros moved the generated C macro "HORSE_UDF"
//#define VAR_BEGIN "I h_prev_v = h_buff_v;"
//#define VAR_END   "h_buff_v = h_prev_v;"
#define C_MACRO_UDF "HORSE_UDF"
#define NUM_VAR_ROW 4           // # of var declarations in a row

#define isTrueMacro     "isTrue"
#define scanBreak(n)    glueCodeLine("break")
#define scanContinue(n) glueCodeLine("continue")

#define HEAD_MAX_SIZE 1024
#define FUNC_MAX_SIZE 10240
#define CODE_MAX_SIZE 10240
static C head_code[HEAD_MAX_SIZE], *htr;
static C func_code[FUNC_MAX_SIZE], *ftr;
static C code[CODE_MAX_SIZE], *ptr;
static CodeList *codeList;

static void genIndent(){ DOI(depth, glueCode("    ")); }

/* ------ declaration above ------ */

#define resetPtr(x) if(x[0]!=0) x+=strlen(x)
static void glueMethodHead(S part1, S part2){
    resetPtr(htr);
    SP(htr, "%s%s\n", part1, part2);
}

static void glueMethodFunc(S func){
    resetPtr(ftr);
    SP(ftr, "%s\n", func);
}

static I countInfoNodeList(InfoNodeList *list){
    if(list) return 1 + countInfoNodeList(list->next); return 0;
}

static void genMethodHead(Node *n){
    resetCode(); S line = ptr;
    glueAny("static I horse_%s(", n->val.method.fname);
    I numRtn   = countInfoNodeList(n->val.method.meta->returnTypes);
    I numParam = totalSymbolNameList(n->val.method.meta->paramVars);
    if(numRtn > 0)
        glueCode("V *h_rtn");
    if(numParam > 0){
        if(numRtn > 0) glueCode(", ");
        genVarList(n->val.method.param->val.listS, comma);
    }
    glueMethodHead(line, ");");
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
static I countNode(List *list){
    if(list) return 1 + countNode(list->next);
    return 0;
}

static void genTic(){
    genIndent();
    glueCodeLine("tic();");
}

static void genToc(){
    genIndent();
    glueCodeLine("return toc();");
    //genStr("time_toc(\"The elapsed time: %g\\n\", elapsed);");
}

static void genList(List *list, C sep){
    if(list){
        genList(list->next, sep);
        if(list->next) glueAny("%c ",sep);
        scanNode(list->val);
    }
}

static void genVarArray(List *list, C sep){
    glueCode("(V []){");
    genList(list, sep);
    glueCode("}");
}

static void genVarList(List *list, C sep){
    if(list){
        genList(list->next, sep);
        if(list->next) glueAny("%c ",sep);
        glueCode("V ");
        scanNode(list->val);
    }
}

static I genListReturn(List *list, C sep, S text){
    if(list){
        I k = genListReturn(list->next, sep, text);
        if(list->next) glueAny("%c ",sep);
        glueAny("%s[%d] = ",text,k);
        scanNode(list->val);
        return k+1;
    }
    return 0;
}

static C obtainTypeAlias(Type t){
    switch(t){
        case boolT: return 'B';
        case   i8T: return 'J';
        case  i16T: return 'H';
        case  i32T: return 'I';
        case  i64T: return 'L';
        case  f32T: return 'F';
        case  f64T: return 'E';
        case  strT: return 'S';
        case  symT: return 'S'; // Q -> S
        case charT: return 'C';
        case dateT: return 'I'; // D -> I
        default: EP("Add more types: %d\n", t);
    }
}

static C getNodeTypeAlias(Node *n){
    InfoNode *in = n->val.type.in;
    if(!in->subInfo)
        return obtainTypeAlias(in->type);
    else EP("Type problem");
}

static S obtainLiteralFunc(Node *n){
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

static void genLocalVar(SymbolName *sn, I k){
    if(k == 0) genIndent();
    else if(k % NUM_VAR_ROW == 0){
        glueLine(); genIndent();
    }
    glueAny("V %s = incV(); ", sn->name);
}

static void genLocalVars(Node *block){
    SymbolTable *st = block->val.block.st;
    I k=0;
    DOI(SymbolTableSize, if(st->table[i]) genLocalVar(st->table[i], k++))
    glueLine();
}

static void genProfileStmt(B f){
    if(f) { genIndent(); glueCode("PROFILE("); }
    else { glueCode(");"); glueLine(); }
}

static void genCodeExtra(ChainExtra *extra){
    glueMethodHead(extra->funcDecl, "");
    genProfileStmt(1); glueCode(extra->funcInvc); genProfileStmt(0);
    glueMethodFunc(extra->funcFunc);
}

/* ------ gen functions defined above ------ */

static void scanExprStmt(Node *n){
    scanNode(n->val.exprStmt.expr);
}

static void scanFuncBuiltin(S func){
    glueCode(getBuiltinStr(func));
}

static void scanFuncMethod(S func){
    glueAny(C_MACRO_UDF "(%s", func);
}

static SymbolKind scanFuncName(Node *n){
    S moduleName = n->val.name.id1;
    S methodName = n->val.name.id2;
    SymbolKind sk = n->val.name.sn->kind;
    switch(sk){
        case builtinS: scanFuncBuiltin(methodName); break;
        case  methodS: scanFuncMethod (methodName); break;
        default:
        TODO("Add support for symbol kind: %d\n", sk); break;
    }
    return sk;
}

static Node* getLastNodeFromList(List *list){
    while(list && list->next)
        list = list->next;
    R list?list->val:0;
}

static void scanCall(Node *n){
    I prevNumArg = numArg;
    SymbolKind sk = scanFuncName(nodeCallFunc(n));
    glueCode("(");
    if(sk == methodS) {
        genVarArray(lhsVars, comma);
        numArg = 1;
        scanNode(nodeCallParam(n)); // argExprK
    }
    else {
        genList(lhsVars, comma);
        numArg = countNode(lhsVars);
        List *args = nodeCallParam(n)->val.listS;
        Node *first = getLastNodeFromList(args);
        if(first && instanceOf(first, funcK)){
            scanList1(args, first);
        }
        else scanNode(nodeCallParam(n)); // argExprK
    }
    numArg = prevNumArg;
    glueCode(")");
    if(sk == methodS) glueCode(")");
}

static void scanVar(Node *n){
    glueCode(n->val.param.id);
    // n->val.param.id
    // n->val.param.typ
}

static void scanName(Node *n){
    if(numArg == 0) numArg = 1;
    else if(numArg > 0) { glueAny("%c ", comma); numArg++; }
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
    else if(numArg > 0) {
        glueAny("%c ", comma);
        numArg++;
    }
    Node *typeNode = n->val.vec.typ;
    I c = countNode(n->val.vec.val);
    glueCode(obtainLiteralFunc(typeNode));
    if(c == 1){
        glueCode("(");
        genList(n->val.vec.val, comma);
        glueCode(")");
    }
    else if(c > 1){
        glueCode("Vector(");
        resetCode(); SP(ptr, "%d, (%c []){", c, getNodeTypeAlias(typeNode));
        genList(n->val.vec.val, comma);
        glueCode("})");
    }
}

static void scanConst(Node *n){
    C temp[99]; temp[0]=0;
    ConstValue *v = n->val.nodeC;
    switch(v->type){
        case    symC:
        case    strC: addStrConst(v->valS); break;
        case   dateC:
        case  monthC:
        case   timeC:
        case minuteC:
        case secondC:
        case    intC: SP(temp, "%d"  , v->valI); break;
        case  floatC: SP(temp, "%g"  , v->valF); break;
        case     dtC:
        case   longC: SP(temp, "%lld", v->valL); break;
        case   clexC: SP(temp, v->valX[1]>=0?"%g+%g":"%g%g", \
                              v->valX[0], v->valX[1]); break;
        default: EP("Add more constant types: %d\n", v->type);
    }
    if(temp[0]) glueCode(temp);
}

// TODO: think how to return multiple values
static void scanReturn(Node *n){
    if(n->val.listS){
        //glueCode("return ");
        genListReturn(n->val.listS, comma, "h_rtn");
        glueChar(';');
        glueLine();
    }
    glueCodeLine("return 0;");
}

static void scanIf(Node *n){
    glueCode("if(");
    glueCode(isTrueMacro); // isTrue is a macro
    glueCode("(");
    scanNode(n->val.ifStmt.condExpr);
    glueCode(")){\n");
    Node *thenBlock = n->val.ifStmt.thenBlock;
    Node *elseBlock = n->val.ifStmt.elseBlock;
    scanNode(thenBlock);
    glueCodeLine("}");
    if(elseBlock){
        glueCodeLine("else {");
        scanNode(elseBlock);
        glueCodeLine("}");
    }
}

static void scanWhile(Node *n){
    glueCode("while(");
    glueCode(isTrueMacro);
    glueCode("(");
    scanNode(n->val.whileStmt.condExpr);
    glueCode(")){\n");
    scanNode(n->val.whileStmt.bodyBlock);
    glueCodeLine("}");
}

static void scanRepeat(Node *n){
    glueCode("repeat(");
    glueCode(isTrueMacro);
    glueCode("(");
    scanNode(n->val.repeatStmt.condExpr);
    glueCode(")){\n");
    scanNode(n->val.repeatStmt.bodyBlock);
    glueCodeLine("}");
}

static void scanBlock(Node *n){
    depth++;
    genLocalVars(n);
    scanList(n->val.listS);
    //P("last stmt %s\n", getNodeTypeStr(n->val.listS->val));
    //Node *lastStmt = n->val.listS?n->val.listS->val:NULL;
    //if(lastStmt && !instanceOf(lastStmt, returnK))
    //    glueCodeLine(VAR_END);
    depth--;
}


static void scanFunc(Node *n){
    if(1==totalList(n->val.listS)){
        Node *first = n->val.listS->val;
        glueCode(", ");
        scanFuncName(first);
    }
    else EP("Add impl.");
}



static void genStatement(Node *n, B f){
    switch(n->kind){
        case   callK: if(f){ if(needInd) genIndent(); }
                      else { if(needInd) { glueCode(";"); glueLine(); }  } break;
        case   stmtK: genProfileStmt(f); break;
        case     ifK:
        case  whileK:
        case repeatK:
        case returnK: if(f) genIndent(); break;
    }
}

static B checkSimpleHash(Node *n){
    L x = lookupSimpleHash(hashOpt,(L)n);
    if(x){
        ChainExtra *extra = (ChainExtra*)x;
        //P("kind = %s\n", getExtraKind(extra->kind)); getchar();
        switch(extra->kind){
            case NativeG: R 0;
            case   SkipG: R 1;
            case    OptG: genCodeExtra(extra); R 1;
            default: EP("Unknown kind: %d\n", extra->kind);
        }
    }
    R 0;
}

static void scanNode(Node *n){
    if(checkSimpleHash(n)) R ;
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
        case      castK: scanCast      (n); break;
        case    vectorK: scanVector    (n); break;
        case     constK: scanConst     (n); break;
        case    returnK: scanReturn    (n); break;
        case        ifK: scanIf        (n); break;
        case     whileK: scanWhile     (n); break;
        case    repeatK: scanRepeat    (n); break;
        case     breakK: scanBreak     (n); break;
        case  continueK: scanContinue  (n); break;
        case     blockK: scanBlock     (n); break;
        case      funcK: scanFunc      (n); break;
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

static void scanListFirst(List *list){
    // scan all items except the last item
    if(list){
        scanListFirst(list->next);
        if(list->next)
            scanNode(list->val);
    }
}

static void scanList1(List *list, Node *first){
    scanListFirst(list);
    scanNode(first); // put the first node last
}


// TODO: utilize information stored in chains
static void compileChain(Chain *chain){
    scanNode(chain->cur);

}

static I compileChainList(ChainList *list){
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
    //ChainList *chains = n->val.method.meta->chains;
    MetaMethod *meta = n->val.method.meta;
    meta->isCompiled = true;
    //printChainList(chains);
    // show the list of all local variables
    //printSymbolNameList(n->val.method.meta->localVars);
    //genMethodHead(n->val.method.fname); // TODO: method name
    genMethodHead(n);
    //depth++;
    //glueCodeLine("I prevBuffS = buffS;");
    //genLocalVars(n->val.method.block);
    //if(n == entryMain) genTic();
    // stategy 1: naive
    scanNode(n->val.method.block);
    // stategy 2: optimize with chain info
    //compileChainList(chains);
    //if(!instanceOf(n->val.method.meta->lastStmt, returnK))
    //    glueCodeLine("END:");
    //glueCodeLine("buffS = prevBuffS;");
    glueCodeLine("}\n");
    currentMethod = prevMethod;
}

static void genEntry(){
    resetCode();
    I numRtns = countInfoNodeList(entryMain->val.method.meta->returnTypes);
    glueCodeLine("I horse_entry(){");
    depth++;
    if(numRtns > 0){
        glueCodeLine("V rtns[99];");
        glueCodeLine("tic();");
        glueAnyLine("%s(horse_main(rtns));",C_MACRO_UDF);
        glueCodeLine("time_toc(\"The elapsed time (ms): %g\\n\", elapsed);");
        glueCodeLine("P(\"Output:\\n\");");
        glueAnyLine("DOI(%d, printV(rtns[i]))", numRtns);
    }
    glueCodeLine("return 0;");
    depth--;
    glueCodeLine("}");
}

static void saveToFile(S path, S head, S func, S code){
    printBanner("Generated Code Below");
    P("%s\n%s\n%s\n", head,func,code);
    return ;
    FILE *fp = fopen(path, "w");
    FP(fp, "%s%s", head,code);
    fclose(fp);
}

static void dispStatsBuff(L cur, L total, S name){
    P("Profile:\n>> Used buffer %s %.2lf%% [%lld/%lld]\n", \
        name, percent(cur,total), cur, total);
    if(cur >= total)
        EP("Code buffer full!!!");
}

static void dispStats(){
    resetCode(); resetPtr(htr); resetPtr(ftr);
    dispStatsBuff(htr-head_code, HEAD_MAX_SIZE, "head");
    dispStatsBuff(ftr-func_code, FUNC_MAX_SIZE, "func");
    dispStatsBuff(ptr-code     , CODE_MAX_SIZE, "code");
}

static void compileNaive(List *list){
    if(list){ compileNaive(list->next); compileMethod(list->val); }
}

static void dumpCode(){
    //if(H_DEBUG) printChainList(); // display visited chains
    genEntry();
    dispStats();
    saveToFile("out/gen.h", head_code, func_code, code);
}

static void init(){
    depth    = 0;
    numArg   = -1;
    needInd  = true;
    lhsVars  = NULL;
    codeList = NEW(CodeList);
    ptr      = code;
    code[0]  = head_code[0] = func_code[0] = 0;
    htr      = head_code;
    ftr      = func_code;
}

I genOptimizedCode(){
    init();
    compileNaive(compiledMethodList->next);
    dumpCode();
    R 0;
}

I HorseCompilerNaive(){
    printBanner("Compiling without Optimizations");
    init();
    compileNaive(compiledMethodList->next);
    dumpCode();
    R 0;
}

