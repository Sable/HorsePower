#include "../global.h"

/* Pattern: general */

typedef struct PatternTree{
    int num;                    // number of child nodes
    char *func1, *func2;        // functin names
    struct PatternTree **child; // child nodes
    Chain *chain;
}PatternTree;

static Node *currentMethod;
static PatternTree *allPattern[99];
static int numPattern;
static PeepholeNode PhList[99];
static int phTotal;
static int qid;

extern List *compiledMethodList;
extern sHashTable *hashOpt;

#define isChainVisited(x) (x)->isVisited
#define chainNode(c)      (c->cur)
#define setVisited(c, v)  c->isVisited=v

#define CODE_MAX_SIZE 10240
#define HEAD_MAX_SIZE 1024
static char code[CODE_MAX_SIZE], *ptr;
static int depth;

/* ------ declaration above ------ */

static PatternTree *initPatternTree(char *func1, char *func2, int n){
    PatternTree *x = (PatternTree*)malloc(sizeof(PatternTree));
    x->func1 = strdup(func1);
    x->func2 = func2?strdup(func2):NULL;
    x->num   = n;
    x->child = 0==n?NULL:(PatternTree**)malloc(sizeof(PatternTree*) * n);
    x->chain = NULL;
    return x;
}

static void deletePatternTree(PatternTree *ptree){
    DOI(ptree->num, deletePatternTree(ptree->child[i]))
    free(ptree->func1);
    if(ptree->func2) free(ptree->func2);
    free(ptree->child);
    free(ptree);
}

static void printPatternTreeCore(PatternTree *ptree, int dep){
    if(ptree){
        DOI(dep, P(" "))
        P("(%s, %s, %d)\n", \
                ptree->func1?ptree->func1:"NULL", \
                ptree->func2?ptree->func2:"NULL", ptree->num);
        DOI(ptree->num, printPatternTreeCore(ptree->child[i], dep+2))
    }
}

static void printPatternTree(PatternTree *ptree){
    printPatternTreeCore(ptree, 2);
}

static PatternTree *createFP1(){
    P("// Pattern FP0\n");
    return NULL;
}

static PatternTree *createFP2(int op){
    if(op!=1&&op!=2){
        EP("pattern 2 must have an option 1 or 2");
    }
    P("// Pattern FP2\n");
    PatternTree *rt = initPatternTree("raze", NULL, 1);
    rt->child[0] = initPatternTree("each", op==1?"sum":op==2?"avg":NULL, 1);
    rt->child[0]->child[0] = initPatternTree("each_right", "index", 0);
    return rt;
}

static PatternTree *createFP3(){
    P("// Pattern FP3\n");
    PatternTree *rt = initPatternTree("index_a", NULL, 2);
    rt->child[0] = initPatternTree("vector", NULL, 1);
    rt->child[0]->child[0] = initPatternTree("len", NULL, 0);
    rt->child[1] = initPatternTree("compress", NULL, 2);
    rt->child[1]->child[0] = initPatternTree("lt", NULL, 0);
    rt->child[1]->child[1] = initPatternTree("?", NULL, 0);
    return rt;
}

static PatternTree *createFP4(){
    P("// Pattern FP4\n");
    PatternTree *rt = initPatternTree("raze", NULL, 1);
    rt->child[0] = initPatternTree("each", "len", 1);
    rt->child[0]->child[0] = initPatternTree("each", "unique", 1);
    rt->child[0]->child[0]->child[0] = initPatternTree("each_right", "index", 0);
    return rt;
}

static PatternTree *createFP5(){
    P("// Pattern FP5\n");
    PatternTree *rt = initPatternTree("raze", NULL, 1);
    rt->child[0] = initPatternTree("each", "min", 1);
    rt->child[0]->child[0] = initPatternTree("each_right", "index", 0);
    return rt;
}

/*
 * 0: targ
 * 1: 1st param
 * 2: 2nd param
 */
static Node *getParamFromChain(Chain *chain, I pos){
    return getParamFromNode(chainNode(chain), pos);
}

static void setAllChainVisitedNonRT(PatternTree *ptree){
    Chain *chain = ptree->chain;
    DOI(ptree->num, setAllChainVisitedNonRT(ptree->child[i]))
    if(strcmp(ptree->func1, "?")) {
        setVisited(chain, true);
        ChainExtra *extra = NEW(ChainExtra);
        extra->kind = SkipG;
        addToSimpleHash(hashOpt, (L)chainNode(chain), (L)extra);
    }
    ptree->chain = NULL;
}


static void setAllChainVisited(PatternTree *ptree, S invc, S decl, S code){
    Chain *chain = ptree->chain;
    DOI(ptree->num, setAllChainVisitedNonRT(ptree->child[i]))
    ChainExtra *extra = NEW(ChainExtra);
    extra->kind = OptG;
    extra->funcInvc = invc;
    extra->funcDecl = decl;
    extra->funcFunc = code;
    addToSimpleHash(hashOpt, (L)chainNode(ptree->chain), (L)extra);
    ptree->chain = NULL;
}

/* ------ find patterns below ------ */

static char *phNameFP2(char *z, char *invc, char *x0, char *x1){
    char tmp[99];
    SP(tmp, "%s(%s,(V []){%s,%s})", invc,z,x0,x1);
    return strdup(tmp);
}

static char *phNameFP3(char *z, char *invc, char *x0, char *x1, char *y0, char *y1){
    char tmp[99];
    SP(tmp, "%s(%s,(V []){%s,%s}, (V []){%s,%s})", invc,z,x0,x1,y0,y1);
    return strdup(tmp);
}

static char *phNameFP4(char *z, char *invc, char *x, char *y){
    char tmp[99]; SP(tmp, "%s(%s,%s,%s)", invc,z,x,y);
    return strdup(tmp);
}

static S genDecl2(S func, C del){
    C temp[199]; 
    SP(temp, "static I %s(V z, V *x V *y)%c", func, del);
    return strdup(temp);
}

static S genDecl3(S func, C del){
    C temp[199]; 
    SP(temp, "static I %s(V z, V x V y)%c", func, del);
    return strdup(temp);
}

static void genIndent(){ DOI(depth, glueCode("    ")); }

static void genPattern2_C_Core(PatternTree *ptree, int op){
    char tmp[99];
    Chain *chain_x = ptree->child[0]->child[0]->chain;  // 
    Chain *chain_y = ptree->child[0]->chain;            // 
    Chain *chain_z = ptree->chain;                      //
    Node *x0 = getParamFromChain(chain_x, 2); S x0s = getNameStr(x0);
    Node *x1 = getParamFromChain(chain_x, 3); S x1s = getNameStr(x1);
    Node *z0 = getParamFromChain(chain_z, 0); S z0s = getNameStr(z0);
    C x0c = getTypeCodeByName(x0);
    C z0c = getTypeCodeByName(z0);
    //P("x0 = %s, x1 = %s, y0 = %s, y1 = %s, z = %s\n", x0,x1,y0,y1,z);
    SP(tmp, "q%d_peephole_fp%d_%d", qid, op, phTotal);
    glueCode(genDecl2(tmp, '{')); glueLine();
    depth++;
    glueAnyLine("// z -> %s", z0s);
    glueAnyLine("V x0 = x[0]; // %s", x0s);
    glueAnyLine("V x1 = x[1]; // %s", x1s);
    glueAnyLine("initV(z, H_%c, vn(x1));", z0c);
    if(op==1 || op==2){ // sum or avg
        glueAnyLine("DOP(vn(x1), {%c a=0; V t=vV(x1,i); DOJ(vn(t), a+=v%c(x0,vL(t,j))) v%c(z,i)=a%s;}) R 0;", x0c, x0c, z0c, op==2?"/vn(t)":"");
    }
    else if(op == 5 || op == 6){ // min or max
        glueAnyLine("DOP(vn(x1), {%c a=%s; V t=vV(x1,i); DOJ(vn(t), {%c t0=v%c(x0,vL(t,j)); if(t0%ca)a=t0;}) v%c(z,i)=a;}) R 0;", x0c, op==5?getMaxValue(x0c):getMinValue(x0c), x0c, x0c, op==5?'<':'>', z0c);
    }
    depth--;
    glueAnyLine("}");
    S invc = phNameFP2(z0s, tmp, x0s, x1s);
    S decl = genDecl2(tmp,';');
    //PhList[phTotal].invc = phNameFP2(z0s, tmp, x0s, x1s);
    //PhList[phTotal].targ = z0s;
    //phTotal++;
    setAllChainVisited(ptree, invc, decl, strdup(code));
}

static void genPattern1_C(PatternTree *ptree){
    // sum
    genPattern2_C_Core(ptree, 1);
}

static void genPattern2_C(PatternTree *ptree){
    // avg
    genPattern2_C_Core(ptree, 2);
}

static void genPattern3_C(PatternTree *ptree){
    char tmp[99];
    Chain *chain_x  = ptree->child[1]->child[0]->chain;  // 2 params of lt
    Chain *chain_y0 = ptree->child[1]->chain;            // 2nd param of compress
    Chain *chain_y1 = ptree->child[0]->child[0]->chain;  // 1st param of len
    Chain *chain_z  = ptree->chain;                      // targ of index_a
    Node *x0 = getParamFromChain(chain_x , 1); S x0s = getNameStr(x0);
    Node *x1 = getParamFromChain(chain_x , 2); S x1s = getNameStr(x1);
    Node *y0 = getParamFromChain(chain_y0, 2); S y0s = getNameStr(y0);
    Node *y1 = getParamFromChain(chain_y1, 1); S y1s = getNameStr(y1);
    Node *z0 = getParamFromChain(chain_z , 0); S z0s = getNameStr(z0);
    //P("x0 = %s, x1 = %s, y0 = %s, y1 = %s, z = %s\n", x0,x1,y0,y1,z);
    SP(tmp, "q%d_peephole_%d", qid, phTotal);
    glueCode(genDecl2(tmp,'{')); glueLine();
    depth++;
    glueAnyLine("// z -> %s", z0s);
    glueAnyLine("V x0 = x[0]; // %s", x0s);
    glueAnyLine("V x1 = x[1]; // %s", x1s);
    glueAnyLine("V y0 = y[0]; // %s", y0s);
    glueAnyLine("V y1 = y[1]; // %s", y1s);
    glueAnyLine("L r0 = vn(y1);");
    glueAnyLine("initV(z, H_B, r0);");
    glueAnyLine("DOP(vn(y0), if(v%c(x0,i)<v%c(x1,i))v%c(z,vL(y0,i))=1) R 0;",
            getTypeCodeByName(x0),getTypeCodeByName(x1),getTypeCodeByName(z0));
    depth--;
    glueAnyLine("}");
    S invc = phNameFP3(z0s, tmp, x0s, x1s, y0s, y1s);
    S decl = genDecl2(tmp,';');
    //PhList[phTotal].invc = phNameFP3(z0s, tmp, x0s, x1s, y0s, y1s);
    //PhList[phTotal].targ = z0s;
    //phTotal++;
    setAllChainVisited(ptree, invc, decl, strdup(code));
}

static void genPattern4_C(PatternTree *ptree){
    char tmp[99];
    Chain *chain_x  = ptree->child[0]->child[0]->child[0]->chain;  // 2 params of each_right
    Chain *chain_z  = ptree->chain;  // 2 params of each_right
    Node *x0 = getParamFromChain(chain_x, 2); S x0s = getNameStr(x0);
    Node *y0 = getParamFromChain(chain_x, 3); S y0s = getNameStr(y0);
    Node *z0 = getParamFromChain(chain_z, 0); S z0s = getNameStr(z0);
    //P("x = %s, y = %s, z = %s\n", x,y,z);
    SP(tmp, "q%d_peephole_%d", qid, phTotal);
    glueCode(genDecl3(tmp,'{')); glueLine();
    //glueAnyLine("L %s(V z, V x, V y){", tmp);
    depth++;
    glueAnyLine("// z -> %s, x -> %s, y -> %s", z0s,x0s,y0s);
    glueAnyLine("L r0 = vn(y);\n");
    glueAnyLine("initV(z, H_L, r0);\n");
    glueAnyLine("DOP(r0, {V t=vV(y,i); L len=vn(t); L tot=0; B f[199]={0};\\");
    depth++;
    glueAnyLine("DOJ(len, if(!f[j]){ \\");
    char x0c = getTypeCodeByName(x0);
    glueAnyLine("DOK(len, if(k!=j && v%c(x,vL(t,j))==v%c(x,vL(t,k))) f[k]=1)\\",x0c,x0c);
    glueAnyLine("f[j]=1; tot++;}) vL(z,i)=tot; }) R 0;");
    depth--; glueAnyLine("}");
    depth--; glueAnyLine("}");
    S invc = phNameFP4(z0s, tmp, x0s, y0s);
    S decl = genDecl3(tmp,';');
    //PhList[phTotal].invc = phNameFP4(z0s, tmp, x0s, y0s);
    //PhList[phTotal].targ = z0s;
    //phTotal++;
    setAllChainVisited(ptree, invc, decl, strdup(code));
}

static void genPattern5_C(PatternTree *ptree){
    // min
    genPattern2_C_Core(ptree, 5);
}

static void genPattern6_C(PatternTree *ptree){
    // max
    genPattern2_C_Core(ptree, 6);
}

static void genPattern_C(PatternTree *ptree, I pid){
    //P("gen pattern pid = %d\n", pid);
    switch(pid){
        case 1: genPattern1_C(ptree); break;
        case 2: genPattern2_C(ptree); break; // q3
        case 3: genPattern3_C(ptree); break;
        case 4: genPattern4_C(ptree); break;
        case 5: genPattern5_C(ptree); break; // q2
        case 6: genPattern6_C(ptree); break;
        default: EP("Pattern not supported yet: %d\n", pid);
    }
}

static void genPattern_ACC(PatternTree *ptree, I pid){
    switch(pid){
        case 3: TODO("OpenACC code gen for pattern 3\n");
        case 4: TODO("OpenACC code gen for pattern 4\n");
        case 5: TODO("OpenACC code gen for pattern 5\n");
        default: EP("Pattern not supported yet: %d\n", pid);
    }
}

static void genPattern(PatternTree *ptree, I pid){
    resetCode();
    switch(qTarg){
        case TARGET_C    : genPattern_C(ptree, pid);     break;
        case TARGET_LLVM : TODO("Support for LLVM\n");   break;
        case TARGET_ACC  : genPattern_ACC(ptree, pid);   break;
        case TARGET_CL   : TODO("Support for OpenCL\n"); break;
        default: EP("TARGET platform is unknown: %d\n", qTarg);
    }
}

static char *getFirstParam(List *params){
    // scan to the last list
    while(params && params->next) params = params->next;
    if(params){
        Node *n = params->val;
        if(instanceOf(n, funcK)){
            int tot = totalElement(n->val.listS);
            if(1 == tot){
                Node *func = n->val.listS->val;
                if(func->val.name.sn->kind == builtinS){
                    return func->val.name.id2;
                }
            }
        }
    }
    return NULL;
}

static bool matchChain2(Chain *chain, char *func1, char *func2){
    if(isChainVisited(chain)) return false;
    if(instanceOf(chainNode(chain), stmtK)){
        Node *expr = chainNode(chain)->val.assignStmt.expr;
        Node *rhs = NULL;
        if(instanceOf(expr,callK)) rhs = expr;
        else if(instanceOf(expr, castK)) rhs = expr->val.cast.exp;
        else return false;
        Node *func = rhs->val.call.func;
        SymbolName *sn = func->val.name.sn;
        if(sn->kind != builtinS) return false;
        char *funcName = func->val.name.id2;
        //P("Input: %s, %s\n", funcName, func1);
        if(!strcmp(funcName, func1) || !strcmp(func1, "?")) {
            // match func2
            if(func2){
                List *params = rhs->val.call.param->val.listS;
                char *firstP = getFirstParam(params);
                //P("--> matched?: %s, %s | %s, %s\n", funcName, func1, firstP, func2);
                return firstP?!strcmp(func2, firstP):false;
            }
            else return true;
        }
    }
    return false;
}

static bool matchPattern(Chain *chain, PatternTree* ptree){
    if(matchChain2(chain, ptree->func1, ptree->func2)){
        int numOfDefs = ptree->num==0?0:chain->defSize;
        //P("numOfDefs = %d, ptree->num = %d\n", numOfDefs, ptree->num);
        if(numOfDefs == ptree->num){
            //P("num = %d, ptree func = %s\n", numOfDefs, ptree->func1);
            //prettyPatternTree(ptree->child[0]);
            //printChain(chain->chain_defs[0]); P("\n");
            DOI(numOfDefs, \
                    if(!matchPattern(chain->chain_defs[i], ptree->child[i]))R false)
            //P("return true\n"); getchar();
            ptree->chain = chain; /* setup */
            return true;
        }
    }
    //P("func not matched: %s\n", ptree->func1);
    return false;
}

static void findPattern(ChainList *list, PatternTree* ptree, I pid){
    if(list){
        findPattern(list->next, ptree, pid);
        if(!isChainVisited(list->chain)){
            if(matchPattern(list->chain, ptree)){
                //P("Pattern %d found\n", pid); getchar();
                cleanCode(); ptr = code;
                genPattern(ptree, pid);
            }
        }
    }
}

static void analyzeChain(ChainList *list){
    // TODO: findPatternCompress
    DOI(numPattern, findPattern(list, allPattern[i], i+1));
}

static void compileMethod(Node *n){
    Node *prevMethod = currentMethod;
    currentMethod = n;
    ChainList *chains = n->val.method.meta->chains;
    //printChainList(chains);
    analyzeChain(chains->next);
    currentMethod = prevMethod;
}

static void scanMethodList(List *list){
    if(list) { scanMethodList(list->next); compileMethod(list->val); }
}

static void initPatterns(){
#define insertPattern(x) allPattern[numPattern++]=x
    numPattern = 0;
    PatternTree *fp1 = createFP2(1);
    PatternTree *fp2 = createFP2(2);
    PatternTree *fp3 = createFP3();
    PatternTree *fp4 = createFP4();
    PatternTree *fp5 = createFP5();
    insertPattern(fp1);
    insertPattern(fp2);
    insertPattern(fp3);
    insertPattern(fp4);
    insertPattern(fp5);
}

static void cleanPatterns(){
    DOI(numPattern, deletePatternTree(allPattern[i]))
}

static void init(){
    currentMethod = NULL;
    phTotal = 0;
    qid     = qIsTpch?qTpchId:99;
    depth   = 0;
    code[0] = 0;
    ptr     = code;
    initPatterns();
    DOI(numPattern, {P("pattern %lld:\n",i+1);printPatternTree(allPattern[i]);})
}

void optPattern(){
    printBanner("Fusion with Patterns");
    init();
    scanMethodList(compiledMethodList->next);
    cleanPatterns();
}


