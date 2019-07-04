#include "../global.h"

extern Prog *root;
extern List *compiledMethodList;
extern bool *ElementwiseUnaryMap;
extern bool *ElementwiseBinaryMap;

static Node *currentMethod;
static I qid;
static I phTotal;

#define isChainVisited(c) (c->isVisited)
#define chainNode(c)   (c)->cur
#define getNameKind(n) (n)->val.name.sn->kind
#define getName1(n)    (n)->val.name.id1
#define getName2(n)    (n)->val.name.id2

#define getVarKind(n)  (n)->val.param.sn->kind
#define getVarName(n)  (n)->val.param.id

/* ------ declaration above ------ */

bool isElementwise(char *funcName){
    FuncUnit x;
    getFuncIndexByName(funcName, &x);
    switch(x.kind){
        case 1: return ElementwiseUnaryMap [x.u];
        case 2: return ElementwiseBinaryMap[x.b];
    }
    return false;
}

static void printChainUses(Chain *p){
    DOI(p->useSize, {P(" --> "); printChain(p->chain_uses[i]); P("\n"); })
}

static void printChainDefs(Chain *p){
    DOI(p->defSize, {P(" <-- "); printChain(p->chain_defs[i]); P("\n"); })
}

static bool isMatchedDef(Chain *p, char *name){
    List *vars = chainNode(p)->val.assignStmt.vars;
    while(vars){
        Node *n = vars->val;
        if(instanceOf(n, nameK) && getNameKind(n) == localS){
            if(!strcmp(name, getName2(n))) return true;
        }
        else if(instanceOf(n, varK) && getVarKind(n) == localS){
            if(!strcmp(name, getVarName(n))) return true;
        }
        vars = vars->next;
    }
    return false;
}

static int findDefByName(Chain *p, char *name){
    int c = 0, x = -1;
    DOI(p->defSize, if(isMatchedDef(p->chain_defs[i], name)){c++; x=i;})
    return c==1?x:-1;
}

static bool isMatchedUse(Chain *p, char *name){
    if(!instanceOf(chainNode(p), stmtK)) return false;
    Node *expr  = chainNode(p)->val.assignStmt.expr;
    List *param = expr->val.call.param->val.listS;
    while(param){
        Node *n = param->val;
        if(instanceOf(n, nameK) && getNameKind(n) == localS){
            if(!strcmp(name, getName2(n))) return true;
        }
        param = param->next;
    }
    return false;
}

static int findUseByName(Chain *p, char *name){
    int c = 0, x = -1;
    DOI(p->useSize, if(isMatchedUse(p->chain_uses[i], name)){c++; x=i;})
    return c==1?x:-1;
}

typedef struct codegen_node{
    Node *n;
    int pnum; // # of parameter
    struct codegen_node *pnode[5]; // parameter nodes
}gNode;

gNode *initgNode(Node *n){
    gNode *x = NEW(gNode);
    x->n = n;
    x->pnum = totalElement(fetchParams(n));
    if(x->pnum >= 5)
        EP("Not enough space");
    return x;
}

static gNode *findFusionUp(Chain *chain){
    if(chain->isVisited) return NULL;
    else chain->isVisited = true;
    Node *n = chain->cur;
    if(instanceOf(n, stmtK)){
        List *vars = n->val.assignStmt.vars;
        Node *expr = n->val.assignStmt.expr;
        if(instanceOf(expr, callK));
        else if(instanceOf(expr, castK)){
            expr = expr->val.cast.exp;
        }
        else return NULL;
        Node *func = expr->val.call.func;
        SymbolKind sk = getNameKind(func);
        if(!(sk == builtinS && isElementwise(getName2(func))))
            return NULL; // if not an elemetnwsie func
        //List *param = expr->val.call.param->val.listS;
        List *param = fetchParams(n);
        //printChainUses(chain); getchar();
        // -- useful debugging
        //printBanner("Gotcha");
        //printChain(chain); P("\n");
        gNode *rt = initgNode(chainNode(chain));
        bool isOK = true;
        int cnt = 0;
        while(param){
            Node *p = param->val;
            if(instanceOf(p, nameK)){
                SymbolKind sk = getNameKind(p);
                if(sk == localS){
                    int c = findDefByName(chain, getName2(p));
                    //P("c4 = %d, s = %s\n", c, getName2(p)); getchar();
                    if(c < 0) isOK = false;
                    else {
                        rt->pnode[cnt] = findFusionUp(chain->chain_defs[c]);
                    }
                }
                else isOK = false;
            }
            else rt->pnode[cnt] = NULL;
            if(!isOK) {free(rt); return NULL;}  // stop and exit
            param = param->next;
            cnt++;
        }
        return rt;
        // isOK == true
        // isOK == true, good ==> save
    }
    return NULL;
}

static Chain *findFusionDown(Chain *chain){
    Node *n = chain->cur;
    if(instanceOf(n, stmtK)){
        List *vars = n->val.assignStmt.vars;
        Node *expr = n->val.assignStmt.expr;
        if(instanceOf(expr, callK)) ;
        else if(instanceOf(expr, castK)){
            expr = expr->val.cast.exp;
        }
        else return NULL;
        Node *func = expr->val.call.func;
        SymbolKind sk = getNameKind(func);
        if(!(sk == builtinS && isElementwise(getName2(func))))
            return NULL; // if not an elemetnwsie func
        bool isOK = true;
        while(vars){
            Node *p = vars->val;
            if(instanceOf(p, varK)){
                if(p->val.param.sn->kind == localS){
                    int c = findUseByName(chain, getVarName(p));
                    //P("c1 = %d, s = %s\n", c, getVarName(p)); getchar();
                    if(c < 0) isOK = false;
                    else {
                        Chain *foundChain = findFusionDown(chain->chain_uses[c]);
                        if(foundChain) return foundChain;
                    }
                }
                else isOK = false;
            }
            else if(instanceOf(p, nameK)){
                if(getNameKind(p) == localS){
                    int c = findUseByName(chain, getName2(p));
                    //P("c2 = %d, s = %s\n", c, getName2(p)); getchar();
                    if(c < 0) isOK = false;
                    else {
                        Chain *foundChain = findFusionDown(chain->chain_uses[c]);
                        if(foundChain) return foundChain;
                    }
                }
                else isOK = false;
            }
            else isOK = false;
            //printChain(chain);
            //P("isOK = %d\n", isOK); getchar();
            if(!isOK) return chain;
            vars = vars->next;
        }
        return chain;
    }
    return NULL;
}

static B isOK2Fuse(gNode *rt){
    // condition: more than 1 stmt
    DOI(rt->pnum, if(rt->pnode[i])R 1) R 0;
}

static void genCodeNode(Node *n);
static void genCodeList(List *list);


static void genCodeList(List *list){
    if(list){ genCodeList(list->next); genCodeNode(list->val); }
}

// copy from: compiler.c/scanConst
static void genCodeConst(Node *n){
    ConstValue *v = n->val.nodeC;
    switch(v->type){
        case    symC:
        case    strC: P("%s",v->valS); break;
        case   dateC:
        case  monthC:
        case   timeC:
        case minuteC:
        case secondC:
        case    intC: P("%d"  , v->valI); break;
        case  floatC: P("%g"  , v->valF); break;
        case     dtC:
        case   longC: P("%lld", v->valL); break;
        case   clexC: P(v->valX[1]>=0?"%g+%g":"%g%g", \
                              v->valX[0], v->valX[1]); break;
        default: EP("Add more constant types: %d\n", v->type);
    }
}

static void genCodeVector(Node *n){
    genCodeList(n->val.vec.val);
}

static void genCodeName(Node *n, I id){
    C code = getTypeCodeByName(n);
    P("v%c(x%d,i)", code, id);
}

static void genCodeNode(Node *n){
    switch(n->kind){
        case vectorK: genCodeVector(n); break;
        case  constK: genCodeConst (n); break;
        default: TODO("Add impl. for %s", getNodeTypeStr(n));
    }
}

static I varNum;
static S varNames[99];
static S fuseInvc;

static L searchName(S *names, S s){
    DOI(varNum, if(!strcmp(names[i],s))R i) R -1;
}

static B isDuplicated(S *names, S s){
    return searchName(names, s) >= 0;
}

static S genInvocation(S targ, S func, S *names, I num){
    C temp[199]; S ptr = temp;
    ptr += SP(ptr, "%s(%s, (V[]){",func,targ);
    DOI(num, ptr+=SP(ptr,(i>0?",%s":"%s"),names[i]))
    SP(ptr, "})");
    return strdup(temp);
}

// TODO: remove duplicated items (only distinct values wanted)
static void totalInputs(gNode *rt, S *names){
    List *params = fetchParams(rt->n);
    DOI(rt->pnum, {I k=i2-i-1; gNode *t=rt->pnode[k]; \
            if(t) totalInputs(t,names); \
            else {Node *p = fetchParamsIndex(params,k)->val; \
                if(instanceOf(p,nameK)){ \
                  if(!isDuplicated(names,getName2(p))) \
                    names[varNum++]=getName2(p);}} })
}

static void genCodeElem(gNode *rt, B isRT){
    Node *n  = rt->n;
    C temp[99];
    if(isRT){
        Node *z0 = getParamFromNode(n,0); S z0s = getNameStr(z0);
        C z0c = getTypeCodeByName(z0);
        SP(temp, "q%d_elementwise_%d",qid,phTotal++);
        P("I %s(V z, V *x){\n",temp);
        varNum = 0;
        totalInputs(rt, varNames);
        DOI(varNum, P(indent "V x%lld = x[%lld]; // %s\n",i,i,varNames[i]))
        P(indent "DOP(vn(z), v%c(z,i)=",z0c);
        // setup invocation for final fusion
        fuseInvc = genInvocation(z0s, temp, varNames, varNum);
    }
    Node *fn = fetchFuncNode(n);
    P("%s(", genFuncNameC(getName2(fn)));
    List *params = fetchParams(n);
    DOI(rt->pnum, {if(i>0)P(","); I k=i2-i-1; gNode *t=rt->pnode[k]; \
            if(t) genCodeElem(t,0); \
            else {Node *p = fetchParamsIndex(params,k)->val; \
                if(instanceOf(p,nameK)) genCodeName(p,searchName(varNames,getName2(p))); \
                else genCodeNode(p);} })
    P(")");
    if(isRT){
        P(") R 0;\n");
        P("}\n");
    }
}

static void findFusionSub(Chain *chain){
    Chain *bottom = findFusionDown(chain);
    if(bottom){
        // if num of chains > 1, likely fusion
        gNode *rt = findFusionUp(bottom);
        if(rt && isOK2Fuse(rt)){
            P("bottom chain found:\n");
            //printChain(bottom); getchar();
            genCodeElem(rt,1);
            P("Fusion invocation: %s\n", fuseInvc);
            getchar();
        }
    }
    else {
        chain->isVisited = true;
        //WP("bottom chain not found at: \n\t");
        //printChain(chain); P("\n");
    }
}

static void findFusion(Chain *chain){
    Node *n = chain->cur;
    //printChain(chain); P("\n");
    if(instanceOf(n, stmtK)){
        findFusionSub(chain);
    }
}

static void analyzeChain(ChainList *list){
    if(list){
        analyzeChain(list->next);
        if(!isChainVisited(list->chain))
            findFusion(list->chain);
    }
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

static void init(){
    qid = qIsTpch?qTpchId:99;
}

// entry: fuse elementwise
void optElementwise(){
    init();
    scanMethodList(compiledMethodList->next);
}

