#include "../global.h"

extern Prog *root;
extern List *compiledMethodList;
extern bool *ElementwiseUnaryMap;
extern bool *ElementwiseBinaryMap;

static Node *currentMethod;

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

static void findFusionUp(Chain *chain){
    if(chain->isVisited) return;
    else chain->isVisited = true;
    Node *n = chain->cur;
    if(instanceOf(n, stmtK)){
        List *vars = n->val.assignStmt.vars;
        Node *expr = n->val.assignStmt.expr;
        if(instanceOf(expr, callK));
        else if(instanceOf(expr, castK)){
            expr = expr->val.cast.exp;
        }
        else return ;
        Node *func = expr->val.call.func;
        SymbolKind sk = getNameKind(func);
        if(!(sk == builtinS && isElementwise(getName2(func))))
            return ; // if not an elemetnwsie func
        List *param = expr->val.call.param->val.listS;
        //printChainUses(chain); getchar();
        printBanner("Gotcha");
        printChain(chain); P("\n");
        bool isOK = true;
        while(param){
            Node *p = param->val;
            if(instanceOf(p, nameK)){
                SymbolKind sk = getNameKind(p);
                if(sk == localS){
                    int c = findDefByName(chain, getName2(p));
                    //P("c4 = %d, s = %s\n", c, getName2(p)); getchar();
                    if(c < 0) isOK = false;
                    else {
                        findFusionUp(chain->chain_defs[c]);
                    }
                }
                else isOK = false;
            }
            if(!isOK) return;  // stop and exit
            param = param->next;
        }
        // isOK == true
        // isOK == true, good ==> save
    }
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

static void findFusionSub(Chain *chain){
    Chain *bottom = findFusionDown(chain);
    if(bottom){
        // if num of chains > 1, likely fusion
        P("bottom chain found:\n\t");
        printChain(bottom); getchar();
        findFusionUp(bottom);
        P("------\n");
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

static void printBannerLocal(S msg){
    WP("/*==== ");
    WP("Optimizing %s", msg);
    WP(" ====*/\n");
}

static void optimizerMain(OC opt){
    printBannerLocal(getOptStr(opt));
    scanMethodList(compiledMethodList->next);
}

static bool isOptimizeAll(){
    DOI(numOpts, if(qOpts[i] == OPT_ALL) R true) R false;
}

static void init(){
    buildUDChain(root);
    currentMethod = NULL;
}

I HorseCompilerOptimized(){
    printBanner("Start Optimizing");
    init();
    if(isOptimizeAll()){
        TODO("Add impl.");
    }
    else {
        DOI(numOpts, optimizerMain(qOpts[i]));
    }
    R 0;
}

