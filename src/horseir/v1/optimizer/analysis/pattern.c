/*
 * Fusion patterns
 */

#include "../global.h"

extern ChainList *chain_list;
extern I qid;
I PhTotal = 0;
PeepholeNode PhList[99];
S RazeNode[99][4];
S varIndex = NULL;
I RazeTotal = 0;

// copied from lf.c (TODO: need to be removed)
#define chainNode(c) (c->cur)
#define isSimpleStmt(n) instanceOf(n,simpleStmtK)
#define isVisited(c) (c->isVisited)
#define setVisited(c, v) c->isVisited=v
#define indent "    "
#define indent2 indent indent

static char *getParamsName(List *params){
    Node *p = params->val->val.nodeS;
    if(instanceOf(p, idK)){
        return fetchName(p);
    }
    else if(instanceOf(p, literalFuncK)){
        return fetchName(p);
    }
    else EP("[getParamsName]: type %d not supported\n",p->kind);
    return NULL;
}

/* Pattern: Compress */

static int countCompress(ChainList *list, char *mask, bool isMark){
    if(!list) return 0;
    Chain *chain = list->chain;
    if(isVisited(chain)) return 0;
    if(isSimpleStmt(chainNode(chain))){
        Node *expr = chainNode(chain)->val.simpleStmt.expr;
        Node *func = expr->val.expr.func;
        if(!func) return 0;
        Node *param= expr->val.expr.param;
        char *funcName = fetchName(func);
        List *params   = param->val.listS;
        if(!strcmp(funcName, "compress")){
            char *pName = getParamsName(params); //first arg
            if(pName){
                if(!mask) mask = pName;
                else if(strcmp(mask, pName)) return 0;
                if(isMark) setVisited(chain, true);
                return 1 + countCompress(list->next, mask, isMark);
            }
            else EP("[countCompress]: a potential error here.\n");
        }
        else return 0;
    }
    return 0;
}

// see more: lf.c:fuseNameByStr
static int getVarShortByName(char *name, char *rtnShort){
    InfoNode *in = getInfoNode(name);
    pType k = in->type;
    ShapeNode *sn = in->shape;
    switch(k){
        case  boolT: strcpy(rtnShort, "vB"); break;
        case   i32T: strcpy(rtnShort, "vI"); break;
        case   i64T: strcpy(rtnShort, "vL"); break;
        case   f32T: strcpy(rtnShort, "vF"); break;
        case   f64T: strcpy(rtnShort, "vE"); break;
        case monthT: strcpy(rtnShort, "vM"); break;
        case  charT: strcpy(rtnShort, "vC"); break;
        case   symT: strcpy(rtnShort, "vQ"); break;
        case   strT: strcpy(rtnShort, "vS"); break;
        case  dateT: strcpy(rtnShort, "vD"); break;
        default: printInfoNode(in); EP("type %s not supported yet.\n", getpTypeName(k));
    }
    return 0;
}

static char getTypeCodeByName(char *name){
    InfoNode *in = getInfoNode(name);
    pType k = in->type;
    ShapeNode *sn = in->shape;
    switch(k){
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
        default:
              printInfoNode(in);
              EP("type %s not supported yet.\n", getpTypeName(k));
    }
    return 0;
}

static S phNameString(char *invc, char **targ, char *lval, char **rval, int n){
    char tmp[999]; SP(tmp, "%s((V []){", invc);
    DOI(n, {if(i>0)strcat(tmp,","); strcat(tmp,targ[i]);})
    SP(tmp+strlen(tmp),"},%s,(V []){",lval);
    DOI(n, {if(i>0)strcat(tmp,","); strcat(tmp,rval[i]);})
    strcat(tmp, "})");
    return strdup(tmp);
}

static void genCompressCode_C(char **targ, char *lval, char **rval, int n){
    char tmp[99], typ[20];
    SP(tmp, "q%d_peephole_%d", qid, PhTotal);
    P("L %s(V *z, V y, V *x){\n", tmp);
    DOI(n, P(indent "V z%lld = z[%lld]; // %s\n", i,i,targ[i]))
    DOI(n, P(indent "V x%lld = x[%lld]; // %s\n", i,i,rval[i]))
    P(indent "L len = vn(y), k = 0;\n");
    P(indent "L lenZ = 0, parZ[H_CORE], offset[H_CORE];\n");
    P(indent "DOI(H_CORE, parZ[i]=offset[i]=0)\n");
    P(indent "CHECKE(getNumOfNonZero(y,parZ));\n");
    P(indent "DOI(H_CORE, lenZ += parZ[i])\n");
    P(indent "DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])\n");
    DOI(n, P(indent "initV(z%lld,vp(x%lld),lenZ);\n",i,i))
    P(indent "DOT(len, if(vB(y,i)){L c=offset[tid]++;");
    DOI(n, {getVarShortByName(rval[i],typ); P("%s(z%lld,c)=%s(x%lld,i);",typ,i,typ,i);})
    P("}) R 0;\n}\n");
    PhList[PhTotal].invc = phNameString(tmp, targ, lval, rval, n);
    PhList[PhTotal].targ = targ[n-1]; PhTotal++;
}

static void genCompressCode_ACC(char **targ, char *lval, char **rval, int n){
    TODO("adding support for OpenACC\n");
}

static void genCompressCode(char **targ, char *lval, char **rval, int n){
    switch(H_TARGET){
        case TARGET_C   : genCompressCode_C(targ, lval, rval, n); break;
        case TARGET_ACC : genCompressCode_ACC(targ, lval, rval, n); break;
        case TARGET_LLVM: TODO("Support for LLVM\n"); break;
        case TARGET_CL  : TODO("Support for OpenCL\n"); break;
        default: EP("TARGET platform is unknown: %d\n", H_TARGET);
    }
}

static void genCompress(ChainList *list, int count){
    ChainList *p = list;
    char **targ = malloc(sizeof(char*)*count);
    char **rval = malloc(sizeof(char*)*count);
    char *lval = NULL;
    for(int i=0;i<count;i++){
        Chain *chain = p->chain;
        if(isSimpleStmt(chainNode(chain))){
            Node *name = chainNode(chain)->val.simpleStmt.name;
            Node *expr = chainNode(chain)->val.simpleStmt.expr;
            Node *func = expr->val.expr.func;
            if(!func) return ;
            Node *param  = expr->val.expr.param;
            List *params = param->val.listS;
            targ[i] = fetchName(name);             // targ name
            lval    = getParamsName(params);       // 1st arg
            rval[i] = getParamsName(params->next); // 2nd arg
        }
        p = p->next;
    }
    // start gen
    genCompressCode(targ, lval, rval, count);
    // free
    free(targ);
    free(rval);
}

static void findPatternCompress(ChainList *list){
    ChainList *p = list->next;
    while(p){
        if(!isVisited(p->chain)){
            int count = countCompress(p, NULL, false);
            if(count > 1){
                countCompress(p, NULL, true);
                genCompress(p, count);
            }
        }
        p = p->next;
    }
}


/* Pattern: Raze */

typedef struct raze_group{
    Chain *first, *second, *third;
}RazeGroup;

static bool matchChain2(Chain *chain, char *func1, char *func2){
    if(isVisited(chain)) return false;
    if(isSimpleStmt(chainNode(chain))){
        Node *expr = chainNode(chain)->val.simpleStmt.expr;
        Node *func = expr->val.expr.func;
        if(!func) return false;
        char *funcName = fetchName(func);
        //P("Input: %s, %s\n", funcName, func1);
        if(!strcmp(funcName, func1) || !strcmp(func1, "?")) {
            // match func2
            if(func2){
                List *params = expr->val.expr.param->val.listS;
                char *firstP = getParamsName(params);
                //P("--> matched?: %s, %s | %s, %s\n", funcName, func1, firstP, func2);
                return !strcmp(func2, firstP);
            }
            else return true;
        }
    }
    return false;
}

static bool matchChain(Chain *chain, char *cur){
    if(isVisited(chain)) return false;
    if(isSimpleStmt(chainNode(chain))){
        Node *expr = chainNode(chain)->val.simpleStmt.expr;
        Node *func = expr->val.expr.func;
        if(!func) return 0;
        char *funcName = fetchName(func);
        if(!strcmp(funcName, cur)) return true;
    }
    return false;
}

static int countRaze(ChainList *list, char *mask){
    if(!list) return 0;
    Chain *first= list->chain;
    if(isVisited(first)) return 0;
    if(matchChain(first, "raze")){
        Chain *second = first->chain_defs[0];
        if(matchChain(second, "each")){
            Chain *third = second->chain_defs[0];
            if(matchChain(third, "each_right")){
                return 3;
            }
            else return 2;
        }
        else return 1;
    }
    else return 0;
    return 0;
}

static void setRazeNodeVisited(){
    DOI(RazeTotal, {RazeGroup *rg=(RazeGroup*)RazeNode[i][3];\
            setVisited(rg->first , true); \
            setVisited(rg->second, true); \
            setVisited(rg->third , true); })
}

static char *phNameRazeCode(int z, char *invc, int x, char *y, int n){
    char tmp[99]; SP(tmp, "%s((V []){", invc);
    DOI(n, {if(i>0)strcat(tmp,",");strcat(tmp,RazeNode[i][z]);}) strcat(tmp, "},");
    strcat(tmp, "(V []){");
    DOI(n, {if(i>0)strcat(tmp,",");strcat(tmp,RazeNode[i][x]);}) strcat(tmp, "},");
    strcat(tmp, y); strcat(tmp, ")");
    return strdup(tmp);
}

static void genRazeCode_C(){
    char tmp[99], typeCode[99];
    DOI(RazeTotal, typeCode[i]=!strcmp(RazeNode[i][1],"sum")?getTypeCodeByName(RazeNode[i][2]):'E')
    SP(tmp, "q%d_peephole_%d", qid, PhTotal);
    P("L %s(V *z, V *x, V y){\n", tmp);
    P(indent "L r0 = vn(y);// y -> %s\n", varIndex);
    DOI(RazeTotal, P(indent "V z%lld = z[%lld]; // %s\n", i,i,RazeNode[i][0]))
    DOI(RazeTotal, P(indent "V x%lld = x[%lld]; // %s\n", i,i,RazeNode[i][2]))
    DOI(RazeTotal, P(indent "initV(z%lld, H_%c, r0);\n", i,typeCode[i]))
    P(indent "DOJ(r0, {V t=vV(y,j); ");
    DOI(RazeTotal, P(" %c c%lld=0;",typeCode[i],i)) P(" \\\n");
    P(indent2 "DOP(vn(t), {L k=vL(t,i);");
    DOI(RazeTotal, P(" c%lld+=v%c(x%lld,k);",i,typeCode[i],i)) P("},\\\n");
    P(indent2 "reduction(+:"); DOI(RazeTotal, {if(i>0)P(",");P("c%lld",i);})
    P(")) \\\n");
    DOI(RazeTotal, { char *func = RazeNode[i][1]; \
            if(!strcmp(func, "sum")) P(indent2 "v%c(z%lld,j)=c%lld;",typeCode[i],i,i); \
            else if(!strcmp(func, "avg")) P(indent2 "v%c(z%lld,j)=c%lld/vn(t);",typeCode[i],i,i); \
            else EP("special func not supported: %s\n", func); P(" \\\n");} \
        )
    P(indent "})\n" indent "R 0;\n}\n");
    PhList[PhTotal].invc = phNameRazeCode(0, tmp, 2, varIndex, RazeTotal);
    PhList[PhTotal].targ = RazeNode[RazeTotal-1][0];  PhTotal++;
    setRazeNodeVisited();
    RazeTotal = 0; varIndex = NULL; // clean
}

static void genRazeCode_ACC(){
    TODO("adding support for ACC\n");
}

static void genRazeCode(){
    switch(H_TARGET){
        case TARGET_C   : genRazeCode_C(); break;
        case TARGET_ACC : genRazeCode_ACC(); break;
        case TARGET_LLVM: TODO("Support for LLVM\n"); break;
        case TARGET_CL  : TODO("Support for OpenCL\n"); break;
        default: EP("TARGET platform is unknown: %d\n", H_TARGET);
    }
}

static char *phNameRazeCodeLen(char *z, char *invc, char *x, char *y){
    char tmp[99]; SP(tmp, "%s(%s,%s,%s)", invc, z, x, y);
    return strdup(tmp);
}

static void genRazeCodeLen(char *targ, char *func, char *p0, char *p1){
    char tmp[99], pfn[30];
    //getRealFuncName(pfn, func);
    //P("pfn = %s\n", pfn); // must be 'pfnLen'
    SP(tmp, "q%d_peephole_%d", qid, PhTotal);
    P("L %s(V z, V x, V y){\n", tmp);
    P(indent "// z -> %s, x -> %s, y -> %s\n", targ, p0, p1);
    P(indent "L r0 = vn(y);\n");
    P(indent "initV(z, H_L, r0);\n");
    P(indent "DOP(r0, vL(z,i) = vn(vV(y,i))) R 0;\n}\n");
    PhList[PhTotal].invc = phNameRazeCodeLen(targ, tmp, p0, p1);
    PhList[PhTotal].targ = targ;  PhTotal++;
    RazeTotal = 0; varIndex = NULL; // clean
}

/*
 * From q1.h
PROFILE( 18, t46, pfnEachRight(t46,t22,t43,pfnIndex));    //third
PROFILE( 19, t47, pfnEach(t47,t46,pfnSum));               //second
PROFILE( 20, t48, pfnRaze(t48, t47));                     //first
*/
static void genRaze(ChainList *list, int count){
    Chain *first= list->chain;
    char *targ    = fetchName(chainNode(first)->val.simpleStmt.name);
    Chain *second = first->chain_defs[0];
    Node *expr2   = chainNode(second)->val.simpleStmt.expr;
    Node *param2  = expr2->val.expr.param;
    List *params2 = param2->val.listS;
    char *func2   = getParamsName(params2);
    Chain *third  = second->chain_defs[0];
    Node *expr3   = chainNode(third)->val.simpleStmt.expr;
    Node *param3  = expr3->val.expr.param;
    List *params3 = param3->val.listS;
    char *func3   = getParamsName(params3);
    char *p0      = getParamsName(params3->next);
    char *p1      = getParamsName(params3->next->next);
    // debug
    //prettyNode(expr2);
    //prettyNode(expr3);
    //P("targ = %s, %s, p0 = %s, p1 = %s\n", targ,func2,p0,p1);
    //P("func2 = %s, func3 = %s\n", func2, func3);
    if(!strcmp(func3, "index")){
        if(!strcmp(func2, "len")){
            //P("RazeTotal = %d\n", RazeTotal);
            if(RazeTotal > 0) goto PRODUCE;
        }
        else if(varIndex == NULL){
            varIndex = p1;
        }
        else if(strcmp(varIndex, p1)){ // not equal
            genRazeCode();
            RazeTotal = 0;
        }
        RazeNode[RazeTotal][0] = targ;
        RazeNode[RazeTotal][1] = func2;
        RazeNode[RazeTotal][2] = p0;
        RazeGroup *rg = (RazeGroup*)malloc(sizeof(RazeGroup));
        rg->first  = first;
        rg->second = second;
        rg->third  = third;
        RazeNode[RazeTotal][3] = (char *)rg;
        RazeTotal++; varIndex = p1;
    }
    else EP("3rd stmt must has pfnIndex: %s\n", func3);
    if(RazeTotal > 0){
PRODUCE:
        genRazeCode(); // output all cached
        genRazeCodeLen(targ, func2, p0, p1);
        // below 3 lines for RazeCodeLen
        setVisited(first , true);
        setVisited(second, true);
        setVisited(third , true);
    }
    return;
}

static void findPatternRaze(ChainList *list){
    ChainList *p = list->next;
    while(p){
        if(!isVisited(p->chain)){
            int count = countRaze(p, NULL);
            if(count ==3){
                genRaze(p, count);
                if(p->next && matchChain(p->next->chain, "list") && RazeTotal > 0){
                    //P("RazeTotal = %d\n", RazeTotal); getchar(); //q12
                    genRazeCode();
                }
            }
        }
        p = p->next;
    }
}

/* Pattern: general */

typedef struct PatternTree{
    int num;                    // number of child
    char *func1, *func2;        // functin name
    struct PatternTree **child; // children
    Chain *chain;
}PatternTree;

static void prettyPatternTree(PatternTree *x){
    if(x){
        if(!(x->func2)) P("(%s, %d)", x->func1, x->num);
        else P("(%s+%s, %d)", x->func1, x->func2, x->num);
        DOI(x->num, prettyPatternTree(x->child[i]))
    }
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

/*
 * 0: targ
 * 1: 1st param
 * 2: 2nd param
 */
static char *getParamFromChain(Chain *chain, int pos){
    if(isSimpleStmt(chainNode(chain))){
        Node *targ = chainNode(chain)->val.simpleStmt.name;
        if(pos == 0) return fetchName(targ);
        Node *expr = chainNode(chain)->val.simpleStmt.expr;
        Node *func = expr->val.expr.func;
        if(!func) return false;
        char *funcName = fetchName(func);
        List *params = expr->val.expr.param->val.listS;
        while(pos>1){ params=params->next; pos--; }
        return getParamsName(params);
    }
    return NULL;
}

static void setAllChainVisited(PatternTree *ptree){
    Chain *chain = ptree->chain;
    DOI(ptree->num, setAllChainVisited(ptree->child[i]))
    if(strcmp(ptree->func1, "?")) setVisited(chain, true);
    ptree->chain = NULL;
}

static char *phNameFP3(char *z, char *invc, char *x0, char *x1, char *y0, char *y1){
    char tmp[99]; SP(tmp, "%s(%s,(V []){%s,%s}, (V []){%s,%s})", invc,z,x0,x1,y0,y1);
    return strdup(tmp);
}

static void genPattern2_C(PatternTree *ptree){
    char tmp[99];
    Chain *chain_x = ptree->child[0]->child[0]->chain; // each_right
    Chain *chain_y = ptree->child[0]->chain;           // each, sum/avg
    Chain *chain_z = ptree->chain;                     // raze
    char *x0 = getParamFromChain(chain_x, 1); // index
    char *y0 = getParamFromChain(chain_y, 1); // sum/avg
    char *z0 = getParamFromChain(chain_z, 0); // index
    P("x0 = %s, y0 = %s, z0 = %s\n", x0,y0,z0);
    SP(tmp, "q%d_peephole_%d", qid, PhTotal);
    //P("L %s(V z, V *x, V *y){\n", tmp);
    EP("pending ... \n");
}

static void genPattern3_C(PatternTree *ptree){
    char tmp[99], pfn[30];
    Chain *chain_x  = ptree->child[1]->child[0]->chain;  // 2 params of lt
    Chain *chain_y0 = ptree->child[1]->chain;            // 2nd param of compress
    Chain *chain_y1 = ptree->child[0]->child[0]->chain;  // 1st param of len
    Chain *chain_z  = ptree->chain;                      // targ of index_a
    char *x0 = getParamFromChain(chain_x , 1);
    char *x1 = getParamFromChain(chain_x , 2);
    char *y0 = getParamFromChain(chain_y0, 2);
    char *y1 = getParamFromChain(chain_y1, 1);
    char *z  = getParamFromChain(chain_z , 0);
    //P("x0 = %s, x1 = %s, y0 = %s, y1 = %s, z = %s\n", x0,x1,y0,y1,z);
    SP(tmp, "q%d_peephole_%d", qid, PhTotal);
    P("L %s(V z, V *x, V *y){\n", tmp);
    P(indent "// z -> %s\n", z);
    P(indent "V x0 = x[0]; // %s\n", x0);
    P(indent "V x1 = x[1]; // %s\n", x1);
    P(indent "V y0 = y[0]; // %s\n", y0);
    P(indent "V y1 = y[1]; // %s\n", y1);
    P(indent "L r0 = vn(y1);\n");
    P(indent "initV(z, H_B, r0);\n");
    P(indent "DOP(vn(y0), if(v%c(x0,i)<v%c(x1,i))v%c(z,vL(y0,i))=1) R 0;\n}\n",
            getTypeCodeByName(x0),getTypeCodeByName(x1),getTypeCodeByName(z));
    PhList[PhTotal].invc = phNameFP3(z, tmp, x0, x1, y0, y1);
    PhList[PhTotal].targ = z;  PhTotal++;
    setAllChainVisited(ptree);
}

static char *phNameFP4(char *z, char *invc, char *x, char *y){
    char tmp[99]; SP(tmp, "%s(%s,%s,%s)", invc,z,x,y);
    return strdup(tmp);
}

static void genPattern4_C(PatternTree *ptree){
    char tmp[99], pfn[30];
    Chain *chain_x  = ptree->child[0]->child[0]->child[0]->chain;  // 2 params of each_right
    Chain *chain_z  = ptree->chain;  // 2 params of each_right
    char *x = getParamFromChain(chain_x, 2);
    char *y = getParamFromChain(chain_x, 3);
    char *z = getParamFromChain(chain_z, 0);
    //P("x = %s, y = %s, z = %s\n", x,y,z);
    SP(tmp, "q%d_peephole_%d", qid, PhTotal);
    P("L %s(V z, V x, V y){\n", tmp);
    P(indent "// z -> %s, x -> %s, y -> %s\n", z,x,y);
    P(indent "L r0 = vn(y);\n");
    P(indent "initV(z, H_L, r0);\n");
    P(indent "DOP(r0, {V t=vV(y,i); L len=vn(t); L tot=0; B f[199]={0};\\\n");
    P(indent2 "DOJ(len, if(!f[j]){ \\\n");
    char x2c = getTypeCodeByName(x);
    P(indent2 "DOK(len, if(k!=j && v%c(x,vL(t,j))==v%c(x,vL(t,k))) f[k]=1)\\\n",x2c,x2c);
    P(indent2 "f[j]=1; tot++;}) vL(z,i)=tot; }) R 0;\n}\n");
    PhList[PhTotal].invc = phNameFP4(z, tmp, x, y);
    PhList[PhTotal].targ = z;  PhTotal++;
    setAllChainVisited(ptree);
}

static void genPattern5_C(PatternTree *ptree){
    // TODO: add code template for this pattern
}

/*
 * Planned
 *   P5 -> q2
 *   P6 -> q3
 */
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
    switch(H_TARGET){
        case TARGET_C    : genPattern_C(ptree, pid); break;
        case TARGET_LLVM : TODO("Support for LLVM\n"); break;
        case TARGET_ACC  : genPattern_ACC(ptree, pid); break;
        case TARGET_CL   : TODO("Support for OpenCL\n"); break;
        default: EP("TARGET platform is unknown: %d\n", H_TARGET);
    }
}

static void findPattern(ChainList *list, PatternTree* ptree, I pid){
    ChainList *p = list->next;
    while(p){
        if(!isVisited(p->chain)){
            if(matchPattern(p->chain, ptree)){
                genPattern(ptree, pid);
            }
        }
        p = p->next;
    }
}

static PatternTree *initPatternTree(char *func1, char *func2, int n){
    PatternTree *x = (PatternTree*)malloc(sizeof(PatternTree));
    x->func1 = strdup(func1);
    x->func2 = func2?strdup(func2):NULL;
    x->num   = n;
    x->child = 0==n?NULL:(PatternTree**)malloc(sizeof(PatternTree*) * n);
    x->chain = NULL;
    return x;
}

static void revokePatternTree(PatternTree *ptree){
    DOI(ptree->num, revokePatternTree(ptree->child[i]))
    free(ptree->func1);
    if(ptree->func2) free(ptree->func2);
    free(ptree->child);
    free(ptree);
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

/* main */

static void analyzeChainList(ChainList *list){
    findPatternCompress(list);
    findPatternRaze(list);
    //PatternTree *fp2_1 = createFP2(1);
    //prettyPatternTree(fp2_1); P("\n");
    //findPattern(list, fp2_1, 2);
    //PatternTree *fp2_2 = createFP2(2);
    //findPattern(list, fp2_2, 2);
    PatternTree *fp3 = createFP3(); /* q4 */
    //prettyPatternTree(fp3); P("\n");
    findPattern(list, fp3, 3);
    PatternTree *fp4 = createFP4();
    //prettyPatternTree(fp4); P("\n");
    findPattern(list, fp4, 4);
    PatternTree *fp5 = createFP5();
    findPattern(list, fp5, 5);
    // clean up
    revokePatternTree(fp3);
    revokePatternTree(fp4);
}

/* entry */
void analyzeFusePattern(){
    printBanner("FP: Loop fusion with patterns");
    //printChainList();
    analyzeChainList(chain_list);
    //printChainList();
}
