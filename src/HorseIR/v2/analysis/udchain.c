#include "../global.h"

static void scanNode(Node *n   , ChainList *flow);
static void scanList(List *list, ChainList *flow);

static void   addDefNameToChain (Chain *chain, char *defName);
static void   addUseNameToChain (Chain *chain, char *useName);
static void   setCurrentNode    (Node *n, Node *val);
static void   addToUDChain      (Chain *useChain, Chain *defChain);
static void   addToDUChain      (Chain *defChain, Chain *useChain);
static Chain *initChainWithNode (Node *val);

#define scanBlock(n,f)     scanList(n->val.listS,f)
#define scanCast(n,f)      scanNode(n->val.cast.exp,f)
#define scanExprStmt(n,f)  scanNode(n->val.exprStmt.expr,f)
#define scanParamExpr(n,f) scanList(n->val.listS,f)
#define scanArgExpr(n,f)   scanList(n->val.listS,f)
#define newChain(n)        initChainWithNode(n)

Chain *currentChain, *exitChain;
ChainList *chain_list, *chain_rt;
char *currentModuleName;
bool isLHS;
//InfoNode *currentIn;

void initUDChain(){
    chain_list = chain_rt = NEW(ChainList);
}

static bool isInNameList(NameList *x, char *s){
    if(x){
        while(x->next){
            x = x->next;
            if(!strcmp(x->name, s)) return 1;
        }
    }
    return 0;
}

static void printChainDefNames(Chain *c){
    NameList *n = c->defs;
    if(n){
        while(n->next){
            n = n->next;
            P("name = %s\n", n->name);
        }
    }
    P("---list---done----\n");
}

static void addToFlow(ChainList *flow, Chain *c){
    //printChain(c); P("--added--\n"); getchar();
    ChainList *p = NEW(ChainList);
    p->chain = c;
    p->next = flow->next;
    flow->next = p;
}

static void markInFlow(ChainList *flow, char *s){
    // kill all defs before
    while(flow->next){
        Chain *x = flow->next->chain;
        if(isInNameList(x->defs,s)){
            ChainList *t = flow->next;
            flow->next = flow->next->next; 
            free(t); // clean flow->next ?
        }
        flow = flow->next;
    }
}

static ChainList *copyFlow(ChainList *flow){
    ChainList *newFlow = NEW(ChainList);
    ChainList *p = newFlow;
    while(flow->next){
        Chain *c = flow->next->chain;
        ChainList *t = NEW(ChainList);
        t->chain = c;
        p->next = t;
        p = p->next;
        flow = flow->next;
    }
    return newFlow;
}

static void cleanFlow(ChainList *p){
    while(p->next){
        ChainList *t = p->next;
        p = p->next;
        free(t);
    }
}

static bool hasDupChain(ChainList *p, Chain *t){
    while(p->next){
        Chain *c = p->next->chain;
        if(c == t) R 1; // is it ok?
        p = p->next;
    }
    R 0;
}

static ChainList *mergeFlow(ChainList *left, ChainList *right){
    ChainList *newFlow = copyFlow(left);
    ChainList *p = newFlow; while(p->next){ p = p->next; }
    ChainList *t = right;
    while(t->next){
        Chain *c = t->next->chain;
        if(!hasDupChain(newFlow,c)){
            ChainList *t = NEW(ChainList);
            t->chain = c;
            p->next = t;
            p = p->next;
        }
        t = t->next;
    }
    //printFlow(left);
    //printFlow(right);
    //printFlow(newFlow);
    //getchar();
    return newFlow;
}

/* UD Chain */

static void addToNameList(NameList *n, char *name){
    n->next = NEW(NameList);
    n = n->next;
    n->name = name;
}

static void addDefNameToChain(Chain *chain, char *defName){
    if(!chain->defs){ chain->defs = NEW(NameList); }
    NameList *defs = chain->defs;
    while(defs->next) defs = defs->next;
    addToNameList(defs, defName);
}

static void addUseNameToChain(Chain *chain, char *useName){
    if(!chain->uses){ chain->uses = NEW(NameList); }
    NameList *uses = chain->uses;
    while(uses->next) uses = uses->next;
    addToNameList(uses, useName);
}

static Chain *initChainWithNode(Node *val){
    Chain *chain = NEW(Chain);
    chain->cur = val;
    int initSize = 2;
    chain->defSize = 0; chain->maxDSize = initSize;
    chain->useSize = 0; chain->maxUSize = initSize;
    chain->chain_uses = NEW2(Chain, initSize);
    chain->chain_defs = NEW2(Chain, initSize);
    chain->defs = NULL;
    chain->uses = NULL;
    chain->isVisited = false;
    return chain;
}

bool hasDupDef(Chain *p, Chain *def){
    DOI(p->defSize, if(p->chain_defs[i] == def) R 1) R 0;
}

bool hasDupUse(Chain *p, Chain *use){
    DOI(p->useSize, if(p->chain_uses[i] == use) R 1) R 0;
}

static void addToUDChain(Chain *useChain, Chain *defChain){
    if(useChain->defSize + 1 >= useChain->maxDSize){
        int maxSize = useChain->maxDSize;
        int maxSize2= maxSize << 1;
        Chain **temp = NEW2(Chain, maxSize2);
        memcpy(temp, useChain->chain_defs, sizeof(Chain*)*maxSize);
        free(useChain->chain_defs);
        useChain->chain_defs = temp;
        useChain->maxDSize   = maxSize2;
    }
    if(!hasDupDef(useChain, defChain))
        useChain->chain_defs[useChain->defSize++] = defChain;
}

static void addToDUChain(Chain *defChain, Chain *useChain){
    if(defChain->useSize + 1 >= defChain->maxUSize){
        int maxSize = defChain->maxUSize;
        int maxSize2= maxSize << 1;
        Chain **temp = NEW2(Chain, maxSize2);
        memcpy(temp, defChain->chain_uses, sizeof(Chain*)*maxSize);
        free(defChain->chain_uses);
        defChain->chain_uses = temp;
        defChain->maxUSize   = maxSize2;
    }
    if(!hasDupUse(defChain, useChain))
        defChain->chain_uses[defChain->useSize++] = useChain;
}

void printChainInfo(Chain *p){
    printNode(p->cur);
    P("  defSize = %d, maxDSize = %d\n",p->defSize,p->maxDSize);
    P("  useSize = %d, maxUSize = %d\n",p->useSize,p->maxUSize);
    P("  %s visited\n", p->isVisited?"Is":"Not");
}

static void printChainUses(Chain *p){
    DOI(p->useSize, {P(" --> "); printChain(p->chain_uses[i]); P("\n"); })
}

static void printChainDefs(Chain *p){
    DOI(p->defSize, {P(" <-- "); printChain(p->chain_defs[i]); P("\n"); })
}

void printFlow(ChainList *chainList){
    printBanner("Chain list");
    ChainList *p = chainList;
    int lineno = 0;
    while(p->next){
        p = p->next;
        P("[%3d] %d: ",lineno++, p->chain->isVisited);
        printChain(p->chain); P("\n");
        printChainUses(p->chain);
        printChainDefs(p->chain);
    }
}

static void scanModule(Node *n){
    currentModuleName = n->val.module.id;
    scanList(n->val.module.body, NULL);
}

static void scanAssignStmt(Node *n, ChainList *flow){
    isLHS = true;
    scanList(n->val.assignStmt.vars, flow);
    isLHS = false;
    /* continue scan */
    scanNode(n->val.assignStmt.expr, flow); 
    //insertString(defName, currentChain, currentIn); //?
}

static void scanVar(Node *n, ChainList *flow){
    char *name = n->val.param.id;
    P("var = %s\n", name);
    // udchain: add defs
    //P("1. addDefNameToChain\n");
    addDefNameToChain(currentChain, name);
    //P("2. markInFlow\n");
    markInFlow(flow, name);
    //P("3. addToFlow\n");
    addToFlow(flow, currentChain);
}

void findInFlow(ChainList *flow, char *s){
    // kill all defs before
    while(flow->next){
        Chain *x = flow->next->chain;
        //P("looking for %s\n", s);
        //printChainDefNames(x);
        if(isInNameList(x->defs,s)){
            //P(">> found!!!\n"); printChain(x);
            addToUDChain(currentChain, x); // use-def chain
            addToDUChain(x, currentChain); // def-use chain
        }
        flow = flow->next;
    }
}

static void scanName(Node *n, ChainList *flow){
    //printFlow(flow); getchar();
    char *name = strName(n);
    if(isLHS){ // udchain: add defs
        addDefNameToChain(currentChain, name);
        markInFlow(flow, name);
        addToFlow(flow, currentChain);
    }
    else { // udchain: add uses
        if(currentChain){
            char *useName = name;
            addUseNameToChain(currentChain, useName);
            findInFlow(flow, useName);
        }
    }
}

static void scanCall(Node *n, ChainList *flow){
    if(instanceOf(n, callK)){
        Node *funcName = n->val.call.func;
        //currentIn = propagateType(funcName, n->val.call.param);
    }
    //else currentIn = propagateTypeCopy(n->val.call.param);
    scanNode(n->val.call.param, flow);
}

static void scanReturn(Node *n, ChainList *flow){
    TODO("Handle return\n");
    exitChain = currentChain;
    /* continue scan */
    scanList(n->val.listS, flow);
}

static void scanIf(Node *n, ChainList *flow){
    ChainList *newFlow = NULL;
    scanNode(n->val.ifStmt.condExpr, flow);
    ChainList *flow_then = copyFlow(flow);
    //printFlow(flow_then); getchar();
    scanNode(n->val.ifStmt.thenBlock, flow_then);
    if(n->val.ifStmt.elseBlock) {
        ChainList *flow_else = copyFlow(flow);
        scanNode(n->val.ifStmt.elseBlock, flow_else);
        newFlow = mergeFlow(flow_then, flow_else);
    }
    else {
        newFlow = mergeFlow(flow, flow_then);
    }
    // clean flow?
    cleanFlow(flow);  // dummy node remained
    flow->next = newFlow->next; // set flow
}

static void scanGlobal(Node *n){
    char *name2 = strName2(currentModuleName, n->val.global.id);
    addDefNameToChain(currentChain, name2);
}

static void scanMethod(Node *n){
    ChainList *flow = NEW(ChainList);
    scanNode(n->val.method.block, flow);
    printFlow(flow);
}

#define scanContinue(n) TODO("continue")
#define scanBreak(n)    TODO("break")
#define scanWhile(n)    TODO("while")
#define scanRepeat(n)   TODO("repeat")

static void setCurrentNode(Node *n, Node *val){
    switch(n->kind){
        case     stmtK:
        case     castK:
        case exprstmtK:
        case   returnK:
        case       ifK:
        case    whileK:
        case   repeatK:
        case   globalK:
             currentChain = val?initChainWithNode(val):NULL; break;
    }
}

static void scanNode(Node *n, ChainList *flow){
    if(!n) return ;
    printNodeType(n);
    P("flow is NULL: %d\n", NULL == flow);
    setCurrentNode(n, n);
    switch(n->kind){
        case    moduleK: scanModule    (n); break; //
        case    methodK: scanMethod    (n); break; // entry
        case      stmtK: scanAssignStmt(n,flow); break; //
        case      castK: scanCast      (n,flow); break; //
        case  exprstmtK: scanExprStmt  (n,flow); break; //
        case paramExprK: scanParamExpr (n,flow); break; //
        case      callK: scanCall      (n,flow); break; //
        case      nameK: scanName      (n,flow); break; //
        case   argExprK: scanArgExpr   (n,flow); break; //
        case    returnK: scanReturn    (n,flow); break; //
        case  continueK: scanContinue  (n); break;
        case     breakK: scanBreak     (n); break;
        case        ifK: scanIf        (n,flow); break;
        case     whileK: scanWhile     (n); break;
        case    repeatK: scanRepeat    (n); break;
        case     blockK: scanBlock     (n,flow); break; //
        case       varK: scanVar       (n,flow); break; //
        case    globalK: scanGlobal    (n); break; //
        /* TODO: add more kinds if possible */
    }
    setCurrentNode(n, NULL);
}

static void scanList(List *list, ChainList *flow){
    if(list){
        scanList(list->next, flow);
        if(list->val) scanNode(list->val, flow);
    }
}

/* entry */
void buildUDChain(Prog *root){
    currentModuleName = NULL;
    scanList(root->module_list, NULL);
    //printChainList();
}

