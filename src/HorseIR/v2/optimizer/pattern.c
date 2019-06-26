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

#define isChainVisited(x) (x)->isVisited
#define chainNode(c)      (c->cur)
#define setVisited(c, v)  c->isVisited=v

#define indent "    "
#define indent2 indent indent

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

static PatternTree *createFP0(){
    return NULL;
}

static PatternTree *createFP1(){
    return NULL;
}

static PatternTree *createFP2(int op){
    if(op!=1&&op!=2){
        EP("pattern 2 must have an option 1 or 2\n");
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

static Node *fetchFuncNode(Node *n){
    if(instanceOf(n, stmtK)){
        Node *rhs = n->val.assignStmt.expr;
        if(instanceOf(rhs, callK)){
            return rhs->val.call.func;
        }
        else if(instanceOf(rhs, castK)){
            Node *t = rhs->val.cast.exp;
            return instanceOf(t, callK)?t->val.call.func:NULL;
        }
    }
    return NULL;
}

static List *fetchParams(Node *n){
    if(instanceOf(n, stmtK)){
        Node *rhs = n->val.assignStmt.expr;
        if(instanceOf(rhs, castK)){
            rhs = rhs->val.cast.exp;
        }
        if(instanceOf(rhs, callK)){
            return rhs->val.call.param->val.listS;
        }
        // TODO: any other cases?
    }
    return NULL;
}

static List *fetchParamsIndex(List *list, int pos){
    if(list){
        return (pos>0)?fetchParamsIndex(list->next, pos-1):list;
    }
    return NULL;
}

static char *fetchFuncName(Node *n){
    if(instanceOf(n, nameK)){
        SymbolName *sn = n->val.name.sn;
        if(sn->kind == builtinS){
            return n->val.name.id2;
        }
    }
    return NULL;
}

/*
 * 0: targ
 * 1: 1st param
 * 2: 2nd param
 */
static Node *getParamFromChain(Chain *chain, int pos){
    if(instanceOf(chainNode(chain), stmtK)){
        List *vars = chainNode(chain)->val.assignStmt.vars;
        Node *targ = NULL;
        if(1 == totalElement(vars)) targ = vars->val;
        else EP("only one targ expected");
        if(pos == 0) return targ;
        Node *func = fetchFuncNode(chainNode(chain));
        if(!func) return NULL;
        List *params = fetchParams(chainNode(chain));
        List *p0     = fetchParamsIndex(params, pos);
        if(p0 && instanceOf(p0->val, nameK))
            return p0->val;
    }
    return NULL;
}

static char *getNameStr(Node *n){
    // input nameK
    SymbolName *sn = n->val.name.sn;
    switch(sn->kind){
        case   localS: return n->val.name.id2;
        default: TODO("add impl.");
    }
}

static char getTypeCodeByIn(InfoNode *in){
#define isBasicType(x) (!(x)->subInfo && !(x)->next)
    if(isBasicType(in)){
        switch(in->type){
            case  boolT: return 'B';
            case   i32T: return 'I';
            case   i64T: return 'L';
            case   f32T: return 'F';
            case   f64T: return 'E';
            case monthT: return 'M';
            case  charT: return 'C';
            case   symT: return 'Q';
            case   strT: return 'S';
            case  dateT: return 'D';
            default: TODO("add type: %d\n", in->type);
        }
    }
}

static char getTypeCodeByName(Node *n){
    SymbolName *sn = n->val.name.sn;
    switch(sn->kind){
        case localS: {
                Node * typ = sn->val.local->val.param.typ;
                return getTypeCodeByIn(typ->val.type.in);
             }
        default: TODO("add impl.");
    }
}

static void setAllChainVisited(PatternTree *ptree){
    Chain *chain = ptree->chain;
    DOI(ptree->num, setAllChainVisited(ptree->child[i]))
    if(strcmp(ptree->func1, "?")) setVisited(chain, true);
    ptree->chain = NULL;
}

/* ------ find patterns below ------ */

static char *phNameFP3(char *z, char *invc, char *x0, char *x1, char *y0, char *y1){
    char tmp[99];
    SP(tmp, "%s(%s,(V []){%s,%s}, (V []){%s,%s})", invc,z,x0,x1,y0,y1);
    return strdup(tmp);
}

static char *phNameFP4(char *z, char *invc, char *x, char *y){
    char tmp[99]; SP(tmp, "%s(%s,%s,%s)", invc,z,x,y);
    return strdup(tmp);
}

static void genPattern3_C(PatternTree *ptree){
    char tmp[99], pfn[30];
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
    P("L %s(V z, V *x, V *y){\n", tmp);
    P(indent "// z -> %s\n", z0s);
    P(indent "V x0 = x[0]; // %s\n", x0s);
    P(indent "V x1 = x[1]; // %s\n", x1s);
    P(indent "V y0 = y[0]; // %s\n", y0s);
    P(indent "V y1 = y[1]; // %s\n", y1s);
    P(indent "L r0 = vn(y1);\n");
    P(indent "initV(z, H_B, r0);\n");
    P(indent "DOP(vn(y0), if(v%c(x0,i)<v%c(x1,i))v%c(z,vL(y0,i))=1) R 0;\n}\n",
            getTypeCodeByName(x0),getTypeCodeByName(x1),getTypeCodeByName(z0));
    PhList[phTotal].invc = phNameFP3(z0s, tmp, x0s, x1s, y0s, y1s);
    PhList[phTotal].targ = z0s;  phTotal++;
    setAllChainVisited(ptree);
}

static void genPattern4_C(PatternTree *ptree){
    char tmp[99], pfn[30];
    Chain *chain_x  = ptree->child[0]->child[0]->child[0]->chain;  // 2 params of each_right
    Chain *chain_z  = ptree->chain;  // 2 params of each_right
    Node *x0 = getParamFromChain(chain_x, 2); S x0s = getNameStr(x0);
    Node *y0 = getParamFromChain(chain_x, 3); S y0s = getNameStr(y0);
    Node *z0 = getParamFromChain(chain_z, 0); S z0s = getNameStr(z0);
    //P("x = %s, y = %s, z = %s\n", x,y,z);
    SP(tmp, "q%d_peephole_%d", qid, phTotal);
    P("L %s(V z, V x, V y){\n", tmp);
    P(indent "// z -> %s, x -> %s, y -> %s\n", z0s,x0s,y0s);
    P(indent "L r0 = vn(y);\n");
    P(indent "initV(z, H_L, r0);\n");
    P(indent "DOP(r0, {V t=vV(y,i); L len=vn(t); L tot=0; B f[199]={0};\\\n");
    P(indent2 "DOJ(len, if(!f[j]){ \\\n");
    char x0c = getTypeCodeByName(x0);
    P(indent2 "DOK(len, if(k!=j && v%c(x,vL(t,j))==v%c(x,vL(t,k))) f[k]=1)\\\n",x0c,x0c);
    P(indent2 "f[j]=1; tot++;}) vL(z,i)=tot; }) R 0;\n}\n");
    PhList[phTotal].invc = phNameFP4(z0s, tmp, x0s, y0s);
    PhList[phTotal].targ = z0s;  phTotal++;
    setAllChainVisited(ptree);
}

static void genPattern5_C(PatternTree *ptree){
    TODO("pattern 5");
}

static void genPattern_C(PatternTree *ptree, I pid){
    //P("gen pattern pid = %d\n", pid);
    switch(pid){
        //case 2: genPattern2(ptree); break; // q3
        case 3: genPattern3_C(ptree); break;
        case 4: genPattern4_C(ptree); break;
        case 5: genPattern5_C(ptree); break; // q2
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
    switch(qTarg){
        case TARGET_C    : genPattern_C(ptree, pid);     break;
        case TARGET_LLVM : TODO("Support for LLVM\n");   break;
        case TARGET_ACC  : genPattern_ACC(ptree, pid);   break;
        case TARGET_CL   : TODO("Support for OpenCL\n"); break;
        default: EP("TARGET platform is unknown: %d\n", qTarg);
    }
}

static char *getFirstParam(List *params){
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
            //prettyPatternTree(ptree->child[i]);
            //prettyNode(chainNode(chain->chain_defs[i]));
            DOI(numOfDefs, \
                    if(!matchPattern(chain->chain_defs[i], ptree->child[i]))R false)
            //P("return true\n");
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
                P("Pattern %d found\n", pid);
                genPattern(ptree, pid);
            }
        }
    }
}

static void analyzeChain(ChainList *list){
    DOI(numPattern, findPattern(list, allPattern[i], i+3));
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
    PatternTree *fp0 = createFP0();
    PatternTree *fp1 = createFP1();
    //PatternTree *fp2 = createFP2();
    PatternTree *fp3 = createFP3();
    PatternTree *fp4 = createFP4();
    PatternTree *fp5 = createFP5();
    //insertPattern(fp0);
    //insertPattern(fp1);
    //insertPattern(fp2);
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
    qid = qIsTpch?qTpchId:99;
    initPatterns();
    DOI(numPattern, {P("pattern %lld:\n",i+3);printPatternTree(allPattern[i]);})
}

void optPattern(){
    printBanner("Fusion with Patterns");
    init();
    scanMethodList(compiledMethodList->next);
    cleanPatterns();
}


