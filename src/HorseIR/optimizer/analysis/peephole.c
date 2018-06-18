#include "../global.h"

extern ChainList *chain_list;
extern I qid;
I PeepholeTotal = 0;

// copied from lf.c (TODO: need to be removed)
#define chainNode(c) (c->cur)
#define isSimpleStmt(n) instanceOf(n,simpleStmtK)
#define isVisited(c) (c->isVisited)
#define setVisited(c, v) c->isVisited=v
#define indent "    "

static char *getParamsName(List *params){
    Node *p = params->val->val.nodeS;
    if(instanceOf(p, idK)){
        return fetchName(p);
    }
    return NULL;
}

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
        default: EP("type %d not supported yet.\n");
    }
    return 0;

}

static void genCompressCode(char **targ, char *lval, char **rval, int n){
    char tmp[99], typ[20];
    SP(tmp, "q%d_peephole_%d", qid, PeepholeTotal++);
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
    P("})\n}\n");
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
static void analyzeChainList(ChainList *list){
    findPatternCompress(list);
}

/* entry */
void analyzePeephole(){
    printBanner("Peephole");
    //printChainList();
    analyzeChainList(chain_list);
}
