#include "../global.h"

extern ChainList *chain_list;
extern I qid;
I PhTotal = 0;
PeepholeNode PhList[99];
S RazeNode[99][3];
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

static int countCompress(ChainList *list, char *mask){
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
                setVisited(chain, true);
                return 1 + countCompress(list->next, mask);
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
        case   i64T: strcpy(rtnShort, "vL"); break;
        case   f32T: strcpy(rtnShort, "vF"); break;
        case   f64T: strcpy(rtnShort, "vE"); break;
        case monthT: strcpy(rtnShort, "vM"); break;
        case  charT: strcpy(rtnShort, "vC"); break;
        case   symT: strcpy(rtnShort, "vQ"); break;
        case   strT: strcpy(rtnShort, "vS"); break;
        default: printInfoNode(in); EP("type %d not supported yet.\n", k);
    }
    return 0;
}

static char getTypeCodeByName(char *name){
    InfoNode *in = getInfoNode(name);
    pType k = in->type;
    ShapeNode *sn = in->shape;
    switch(k){
        case  boolT: return 'B'; break;
        case   i64T: return 'L'; break;
        case   f32T: return 'F'; break;
        case   f64T: return 'E'; break;
        case monthT: return 'M'; break;
        case  charT: return 'C'; break;
        case   symT: return 'Q'; break;
        case   strT: return 'S'; break;
        default: printInfoNode(in); EP("[getTypeCodeByName] type %d not supported yet.\n", k);
    }
    return 0;
}

static S phNameString(char *invc, char **targ, char *lval, char **rval, int n){
    char tmp[99]; SP(tmp, "%s((V []){", invc);
    DOI(n, {if(i>0)strcat(tmp,","); strcat(tmp,targ[i]);})
    SP(tmp+strlen(tmp),"},%s,(V []){",lval);
    DOI(n, {if(i>0)strcat(tmp,","); strcat(tmp,rval[i]);})
    strcat(tmp, "})");
    return strdup(tmp);
}

static void genCompressCode(char **targ, char *lval, char **rval, int n){
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
            targ[i] = fetchName(name);       // targ name
            lval    = getParamsName(params); // 1st arg
            rval[i] = getParamsName(params->next); //2nd arg
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
            int count = countCompress(p, NULL);
            if(count > 1){
                genCompress(p, count);
            }
        }
        p = p->next;
    }
}


/* Pattern: Raze */


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

static void genRazeCode(){
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
    PhTotal++;
    RazeTotal = 0; varIndex = NULL; // clean
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
    PhTotal++;
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
    //P("targ = %s, %s, p0 = %s, p1 = %s\n", targ,func2,p0,p1);
    if(!strcmp(func3, "index")){
        if(!strcmp(func2, "len")){
            //P("RazeTotal = %d\n", RazeTotal);
            genRazeCode(); // output all cached
            genRazeCodeLen(targ, func2, p0, p1);
            return ;
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
        RazeTotal++; varIndex = p1;
    }
    else EP("3rd stmt must has pfnIndex\n");
}

static void findPatternRaze(ChainList *list){
    ChainList *p = list->next;
    while(p){
        if(!isVisited(p->chain)){
            int count = countRaze(p, NULL);
            if(count ==3){
                genRaze(p, count);
                if(p->next && matchChain(p->next->chain, "list") && RazeTotal > 0){
                    //P("RazeTotal = %d\n", RazeTotal); //q12
                    genRazeCode();
                }
            }
        }
        p = p->next;
    }
}

static void analyzeChainList(ChainList *list){
    findPatternCompress(list);
    findPatternRaze(list);
}

/* entry */
void analyzePeephole(){
    printBanner("Peephole");
    //printChainList();
    analyzeChainList(chain_list);
}
