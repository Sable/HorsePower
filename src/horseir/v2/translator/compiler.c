#include "../global.h"

typedef struct CodeList{
    C code[128];
    struct CodeList *next;
}CodeList;

extern Node *entryMain;

static I depth;
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

sHashTable *hashOpt;

#define scanArgExpr(n)   scanList(n->val.listS)
#define scanParamExpr(n) scanList(n->val.listS)
#define scanCast(n)      scanNode(n->val.cast.exp)

// two macros moved the generated C macro "HORSE_UDF"
//#define VAR_BEGIN "I h_prev_v = h_buff_v;"
//#define VAR_END   "h_buff_v = h_prev_v;"
#define C_MACRO_UDF  "HORSE_UDF"  // macro(lineno, call, callback)
#define C_MACRO_TIME "PROFILE"
#define NUM_VAR_ROW 4           // # of var declarations in a row
#define NUM_CHAR_LINE 128

#define isTrueMacro     "isTrue"
#define scanBreak(n)    glueCodeLine("break")
#define scanContinue(n) glueCodeLine("continue")
#define isNodeUDF(n)  instanceOf(nodeStmtExpr(n), callK)

#define HEAD_MAX_SIZE 1024
#define FUNC_MAX_SIZE 10240
static C head_code[HEAD_MAX_SIZE], *htr;
static C func_code[FUNC_MAX_SIZE], *ftr;
static C code[CODE_MAX_SIZE], *ptr;
static CodeList *codeList;

/* ------ declaration above ------ */

#define resetPtr(x) if(x[0]!=0) x+=strlen(x)

static void addToCodeList(CodeList *list, CodeList *c){
    c->next = list->next;
    list->next = c;
}

static void storeCode(S s){
    if(strlen(code) < NUM_CHAR_LINE) { // 128
        CodeList *c = NEW(CodeList);
        strcpy(c->code, s);
        addToCodeList(codeList, c);
    }
    else EP("A line of the code should be less than %d", NUM_CHAR_LINE);
}

static C obtainTypeAlias(Type t){
    switch(t){
        case boolT: R 'B';
        case   i8T: R 'J';
        case  i16T: R 'H';
        case  i32T: R 'I';
        case  i64T: R 'L';
        case  f32T: R 'F';
        case  f64T: R 'E';
        case  strT: R 'S';
        case  symT: R 'S'; // Q -> S
        case charT: R 'C';
        case dateT: R 'I'; // D -> I
        default: EP("Add more types: %d\n", t);
    }
}

static C obtainNodeTypeAlias(Node *n){
    InfoNode *in = n->val.type.in;
    if(!in->subInfo)
        return obtainTypeAlias(in->type);
    else EP("Type problem");
}

static S obtainTypeShort(Type t){
    switch(t){
        case boolT: R "Bool";
        case   i8T: R "I8";
        case  i16T: R "I16";
        case  i32T: R "I32";
        case  i64T: R "I64";
        case  f32T: R "F32";
        case  f64T: R "F64";
        case  strT: R "String";
        case  symT: R "Symbol";
        case charT: R "Char";
        case dateT: R "Date";
        default: EP("Add more types: %d\n", t);
    }
    return NULL;
}

static S obtainLiteralFunc(Node *n, B isScalar){
    InfoNode *in = n->val.type.in;
    if(!in->subInfo){
        C tmp[99];
        SP(tmp, isScalar?"Literal%s":"LiteralVector%s", obtainTypeShort(in->type));
        return strdup(tmp);
    }
    else EP("Type problem");
}

static void addStrConst(S s){
    L size = strlen(s) + 2 + 1;
    L preSize = ptr - code;
    if(size + preSize > CODE_MAX_SIZE)
        EP("size is more than expected: %lld vs. %d", size + preSize, CODE_MAX_SIZE);
    SP(ptr, "\"%s\"", s);
}

static B isNodeMethod(Node *n){
    if(instanceOf(n, stmtK)){
        Node *expr = nodeStmtExpr(n);
        if(instanceOf(expr, callK)){
            Node *func = nodeCallFunc(expr); // nameK
            SymbolKind sk = nodeNameKind(func);
            return sk == methodS;
        }
    }
    return false;
}

/* ------ helper functions above ------ */

static void genIndent(){
    DOI(depth, glueCode("    "));
}

static void glueMethodHead(S part1, S part2){
    resetPtr(htr);
    SP(htr, "%s%s\n", part1, part2);
}

static void glueMethodFunc(S func){
    resetPtr(ftr);
    SP(ftr, "%s\n", func);
}

static void genMethodHead(Node *n){
    resetCode(); S line = ptr;
    glueAny("static I horse_%s(", n->val.method.fname);
    I numRtn   = totalInfoNodeList(nodeMethodMeta(n)->returnTypes);
    I numParam = totalSymbolNameList(nodeMethodMeta(n)->paramVars);
    if(numRtn > 0)
        glueCode("V *h_rtn");
    if(numParam > 0){
        if(numRtn > 0) glueCode(", ");
        genVarList(nodeMethodParameters(n)->val.listS, comma);
    }
    glueMethodHead(line, ");");
    glueCode("){");
    glueLine();
}

static void genTic(){
    glueCodeLine("tic();");
}

static void genToc(){
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

static void genListEach(List *list, C sep){
    if(list){
        genListEach(list->next, sep);
        glueAny("%c ",sep);
        scanNode(list->val);
    }
}

// (V []){ x,y,... }
static void genVarArray(List *list, C sep){
    glueCode("(V []){");
    genList(list, sep);
    glueCode("}");
}

static void genVarStoreDep(List *list, I dep){
    if(list){
        genVarStoreDep(list->next, dep+1);
        if(dep > 0) glueCode(" ");
        scanNode(list->val);
        glueAny("=tempV[%d];", dep);
    }
}

static void genVarStore(List *list){
    glueAny("{");
    genVarStoreDep(list, 0);
    glueAny("}");
}


static void genVarList(List *list, C sep){
    if(list){
        genVarList(list->next, sep);
        if(list->next) glueAny("%c ",sep);
        glueCode("V "); 
        scanNode(list->val);
    }
}

static void genVarSingle(List *list){
    if(!list->next){
        scanNode(list->val);
    }
    else EP("Single var expected.");
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

static void genLocalVar(SymbolName *sn, I k){
    if(k == 0) genIndent();
    else if(k % NUM_VAR_ROW == 0){
        glueLine(); genIndent();
    }
    //P("V %s = incV(); \n", sn->name);
    glueAny("V %s = incV(); ", sn->name);
}

static void genLocalVars(Node *block){
    SymbolTable *st = block->val.block.st;
    I k=0;
    DOI(SymbolTableSize, { SymbolName *sn = st->table[i];
            while(sn){genLocalVar(sn, k++); sn=sn->next;}} )
    glueLine();
    glueIndent(); glueAny("V tempV[10]; // temporary return vars"); glueLine();
    //P("total decl vars = %d\n", k); getchar();
}

static void genProfileStmt(B f, I c, S macro){
    if(f) {
        genIndent(); glueAny("%s(%d, ",macro,c);
    }
    else {
        glueCode(");"); glueLine();
    }
}

static void genPrefixMacro(Node *n, B f, I c){
    if(isNodeMethod(n)){
        genProfileStmt(f,c,C_MACRO_UDF);
    }
    else {
        genProfileStmt(f,c,C_MACRO_TIME);
    }
}

static void genCodeExtra(ChainExtra *extra, I lineno){
    glueMethodHead(extra->funcDecl, "");
    genProfileStmt(1,lineno,C_MACRO_TIME);
    glueCode(extra->funcInvc);
    genProfileStmt(0,-1,C_MACRO_TIME);
    glueMethodFunc(extra->funcFunc);
}

static void genStatement(Node *n, B f){
    switch(n->kind){
        case   callK: if(f){ if(needInd) genIndent(); }
                      else { if(needInd) { glueCode(";"); glueLine(); }  } break;
        case   stmtK: genPrefixMacro(n,f,n->lineno); break;
        case     ifK:
        case  whileK:
        case repeatK:
        case returnK: if(f) genIndent(); break;
    }
}

static void genEntry(){
    resetCode();
    I numRtns = totalInfoNodeList(entryMain->val.method.meta->returnTypes);
    glueCodeLine("E horse_entry(){");
    depth++;
    if(numRtns > 0){
        glueCodeLine("V rtns[99];");
        glueCodeLine("tic();");
        glueAnyLine("%s(0, horse_main(rtns), {});",C_MACRO_UDF);
        glueCodeLine("E elapsed = calc_toc();");
        glueCodeLine("P(\"The elapsed time (ms): %g\\n\", elapsed);");
        glueCodeLine("P(\"Output:\\n\");");
        glueAnyLine("DOI(%d, printV(rtns[i]))", numRtns);
    }
    glueCodeLine("return elapsed;");
    depth--;
    glueCodeLine("}");
}

/* ------ gen functions defined above ------ */

static void scanExprStmt(Node *n){
    scanNode(n->val.exprStmt.expr);
}

static void scanFuncBuiltin(S func){
    glueCode(obtainBuiltinName(func));
}

static void scanFuncMethod(S func){
    glueAny("horse_%s", func);
}

static SymbolKind scanFuncName(Node *n){
    S moduleName = nodeName1(n);
    S methodName = nodeName2(n);
    SymbolKind sk = nodeNameKind(n);
    switch(sk){
        case builtinS: scanFuncBuiltin(methodName); break;
        case  methodS: scanFuncMethod (methodName); break;
        default:
        TODO("Add support for symbol kind: %d", sk); break;
    }
    return sk;
}

static Node* getFirstNodeFromList(List *list){
    if(list){
        while(list->next) list=list->next;
        R list->val;
    }
    else R NULL;
}

static void scanCall(Node *n){
    SymbolKind sk = scanFuncName(nodeCallFunc(n));
    glueCode("(");
    if(sk == methodS) {
        //genVarArray(lhsVars, comma);
        glueAny("tempV%c ", comma);
        genList(nodeList(nodeCallParam(n)), comma);
    }
    else {
        S funcName = nodeName2(nodeCallFunc(n));
        genList(lhsVars, comma);
        List *args = nodeList(nodeCallParam(n));
        I numArg = totalList(args);
        if(sEQ(funcName, "list")){
            glueAny(", %d, ",numArg);
            genVarArray(args, comma);
        }
        else {
            Node *first = getFirstNodeFromList(args);
            if(first && instanceOf(first, funcK)){
                scanList1(args, first);
            }
            else genListEach(args, comma);
        }
    }
    glueCode(")");
    if(sk == methodS){
        glueCode(", ");
        genVarStore(lhsVars);
    }
}

static void scanVar(Node *n){
    glueCode(n->val.param.id);
    // n->val.param.id
    // n->val.param.typ
}

static void scanName(Node *n){
    //if(numArg == 0) numArg = 1;
    //else if(numArg > 0) { glueAny("%c ", comma); numArg++; }
    glueCode(nodeName2(n));
}

static void scanStmtCopy(Node *n){
    List *lhs = nodeStmtVars(n);
    Node *rhs = nodeStmtExpr(n);
    glueCode("copyV(");
    genVarSingle(lhs); glueChar(comma); scanNode(rhs); // lhs: single var
    glueCode(")");
}

static void scanAssignStmt(Node *n){
    B prevNeedInd= needInd;
    needInd = false;
    List *prevVars = lhsVars;
    lhsVars = nodeStmtVars(n); //n->val.assignStmt.vars;
    Node *rhsExpr = nodeStmtExpr(n);
    if(instanceOf(rhsExpr, callK) || instanceOf(rhsExpr,castK))
        scanNode(rhsExpr);
    else
        scanStmtCopy(n);
    needInd = prevNeedInd;
    lhsVars = prevVars;
}

static void scanVector(Node *n){
    //if(numArg == 0) numArg = 1;
    //else if(numArg > 0) {
    //    glueAny("%c ", comma);
    //    numArg++;
    //}
    Node *typeNode = n->val.vec.typ;
    I c = totalList(n->val.vec.val);
    glueCode(obtainLiteralFunc(typeNode, c==1));
    if(c == 1){
        glueCode("(");
        genList(n->val.vec.val, comma);
        glueCode(")");
    }
    else if(c > 1){
        resetCode(); SP(ptr, "(%d, (%c []){", c, obtainNodeTypeAlias(typeNode));
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
        case   charC: SP(temp, "'%c'", v->valS[0]); break;
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

static B checkSimpleHash(Node *n){
    L x = lookupSimpleHash(hashOpt,(L)n);
    if(x){
        ChainExtra *extra = (ChainExtra*)x;
        //P("kind = %s\n", getExtraKind(extra->kind)); getchar();
        switch(extra->kind){
            case NativeG: R 0;
            case   SkipG: R 1;
            case    OptG: genCodeExtra(extra, n->lineno); R 1;
            default: EP("Unknown kind: %d\n", extra->kind);
        }
    }
    R 0;
}

static void scanNode(Node *n){
    if(hashOpt && checkSimpleHash(n)) R ; // if mode for optimization
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
        if(list->next){
            glueAny("%c ", comma);
            scanNode(list->val);
        }
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

static void compileCode(List *list){
    if(list){ compileCode(list->next); compileMethod(list->val); }
}

static void saveToFile(S path, S head, S func, S code){
    printBanner("Generated Code Below");
    P("%s\n%s\n%s\n", head,func,code);
    return ;
    //FILE *fp = fopen(path, "w");
    //FP(fp, "%s%s", head,code);
    //fclose(fp);
}

static void dumpCode(){
    //if(H_DEBUG) printChainList(); // display visited chains
    genEntry();
    dispStats();
    saveToFile("out/gen.h", head_code, func_code, code);
}

static void init(){
    depth    = 0;
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
    compileCode(compiledMethodList->next);
    dumpCode();
    R 0;
}

I HorseCompilerNaive(){
    printBanner("Compiling without Optimizations");
    init();
    hashOpt = NULL; // disable opt
    compileCode(compiledMethodList->next);
    dumpCode();
    R 0;
}

