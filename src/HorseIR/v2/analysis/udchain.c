#include "../global.h"

static void scanNode(Node *n   , ChainList *flow);
static void scanList(List *list, ChainList *flow);

static void   addDefNameToChain (Chain *chain, char *defName);
static void   addUseNameToChain (Chain *chain, char *useName);
static void   setCurrentNode    (Node *n, Node *val);
static void   addToUDChain      (Chain *useChain, Chain *defChain);
static void   addToDUChain      (Chain *defChain, Chain *useChain);
static Chain *initChainWithNode (Node *val);

#define scanBlock(n,f)     scanList(n->val.block.stmts,f)
#define scanCast(n,f)      scanNode(n->val.cast.exp,f)
#define scanExprStmt(n,f)  scanNode(n->val.exprStmt.expr,f)
#define scanParamExpr(n,f) scanList(n->val.listS,f)
#define scanArgExpr(n,f)   scanList(n->val.listS,f)
#define newChain(n)        initChainWithNode(n)

static Chain *currentChain;
static ChainList *flow_sink, *chain_list;
static FlowList *flow_list_exit, *flow_list_input;
static char *currentModuleName;
static bool isLHS;
static InfoNode *currentIn;

static void addToChainList(ChainList *chains, Chain *c){
    ChainList *x = NEW(ChainList);
    x->chain = c;
    x->next = chains->next;
    chains->next = x;
}

//static void printChainList(ChainList *chains){
//    if(chains && chains->next){
//        printChainList(chains->next);
//        printChainInfo(chains->next->chain);
//    }
//}


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

static void addToFlowList(FlowList *list, ChainList *flow){
    FlowList *p = NEW(FlowList);
    p->flow = flow;
    p->next = list->next;
    list->next = p;
}

static void cleanFlowList(FlowList *list){
    if(list){
        FlowList *p = list->next;
        while(p){FlowList *t = p; p = p->next; free(t);}
        list->next = NULL;
    }
}


static void addToFlow(ChainList *flow, Chain *c){
    //printChain(c); P("--added--\n"); getchar();
    ChainList *p = NEW(ChainList);
    p->chain = c;
    p->next = flow->next;
    flow->next = p;
}

static void removeKillSet(ChainList *flow, char *s){
    // kill all defs before
    //bool isFound = false; ChainList *src = flow;
    while(flow && flow->next){
        Chain *x = flow->next->chain;
        if(isInNameList(x->defs,s)){
            //isFound = true; P("found s = %s\n", s); printFlow(src); getchar();
            ChainList *t = flow->next;
            flow->next = flow->next->next; 
            free(t); // clean flow->next ?
        }
        else flow = flow->next;
    }
    //if(isFound){
        //P("----> after <-----\n"); printFlow(src); getchar();
    //}
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

// static bool hasDupChain(ChainList *p, Chain *t){
//     while(p->next){
//         Chain *c = p->next->chain;
//         if(c == t) R 1; // is it ok?
//         p = p->next;
//     }
//     R 0;
// }

#define sameChain(x,y) (x)->cur == (y)->cur

// list: no dummy
static bool isChainInList(ChainList *list, Chain *chain){
    if(list){
        bool t = isChainInList(list->next, chain);
        //P("%lld vs %lld\n", (L)list->chain->cur, (L)chain->cur);
        return t?true:sameChain(list->chain, chain);  // check Node cur
    }
    return false;
}

static Chain *findChainInList(ChainList *list, Node *n){
    if(list){
        Chain *chain = findChainInList(list->next, n);
        if(chain) return chain;
        else return list->chain->cur == n? list->chain: NULL;
    }
    return NULL;
}


static ChainList *mergeFlow(ChainList *left, ChainList *right){
    ChainList *newFlow = copyFlow(left);
    ChainList *p = newFlow; while(p->next){ p = p->next; }
    ChainList *t = right;
    while(t->next){
        Chain *c = t->next->chain;
        if(!isChainInList(newFlow->next, c)){
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

static bool sameFlowBody(ChainList *left, ChainList *right){
    if(left){
        bool t = sameFlowBody(left->next, right);
        //if(t) P("t = true\n"); else P("t = false\n");
        return t?isChainInList(right, left->chain):false;
    }
    return true;
}

// probably a duplicated method
static int countChainList(ChainList *list){
    if(list){ return 1 + countChainList(list->next); } return 0;
}

// assume: left contains distinct chains
static bool sameFlow(ChainList *left, ChainList *right){
    int  leftSize = countChainList(left);
    int rightSize = countChainList(right);
    //P("leftSize = %d, rightSize = %d\n", leftSize, rightSize);
    //printFlow(left); printFlow(right); getchar();
    if(left && right) {
        if(leftSize == rightSize){
            bool t = sameFlowBody(left->next, right->next);
            //P("t = %d\n", t); getchar();
            return t;
        }
        else return false;
    }
    else return false;
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

static int printFlowBody(ChainList *list){
    if(list){
        int lineno = printFlowBody(list->next);
        Chain *p = list->chain;
        P("[%3d] %d, %lld: ",lineno, p->isVisited, (L)list->chain->cur);
        printChain(p);     P("\n");
        printChainUses(p);
        printChainDefs(p); P("\n");
        return lineno + 1;
    }
    return 0;
}

void printFlow(ChainList *chainList){
    if(chainList)
        printFlowBody(chainList->next);
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
    //P("var = %s\n", name);
    // udchain: add defs
    //P("1. addDefNameToChain\n");
    addDefNameToChain(currentChain, name);
    //P("2. removeKillSet\n");
    removeKillSet(flow, name);
    //P("3. addToFlow\n");
    addToFlow(flow, currentChain);
}

static void findInFlowBody(ChainList *flow, char *s){
    if(flow){
        findInFlowBody(flow->next, s);
        Chain *x = flow->chain;
        if(isInNameList(x->defs,s)){  // TODO: can improve with symbol tables
            //P(">> found!!! (%s)\n", s); printChain(x); P("\n");
            addToUDChain(currentChain, x); // use-def chain
            addToDUChain(x, currentChain); // def-use chain
        }
    }
}

static void findInFlow(ChainList *flow, char *s){
    //printFlow(flow);
    findInFlowBody(flow->next, s);
}

// void findInFlow(ChainList *flow, char *s){
//     // kill all defs before
//     while(flow->next){
//         Chain *x = flow->next->chain;
//         //P("looking for %s\n", s);
//         //printChainDefNames(x);
//         if(isInNameList(x->defs,s)){  // TODO: can improve with symbol tables
//             //P(">> found!!!\n"); printChain(x); P("\n");
//             addToUDChain(currentChain, x); // use-def chain
//             addToDUChain(x, currentChain); // def-use chain
//         }
//         flow = flow->next;
//     }
// }

static void scanName(Node *n, ChainList *flow){
    //printFlow(flow); getchar();
    char *name = strName(n);
    if(isLHS){ // udchain: add defs
        addDefNameToChain(currentChain, name);
        removeKillSet(flow, name);
        addToFlow(flow, currentChain);
        //P("flow next = %lld\n", (L)flow->next); printFlow(flow); getchar();
    }
    else { // udchain: add uses
        //P("checking use: %s, currentChain: %lld\n", name, (L)currentChain); getchar();
        if(currentChain){
            char *useName = name;
            addUseNameToChain(currentChain, useName);
            findInFlow(flow, useName);
            // should not add current chain to flow
        }
    }
}

static void scanCall(Node *n, ChainList *flow){
    //if(instanceOf(n, callK)){
    //    Node *func = n->val.call.func;
    //    //InfoNodeList *rtns = propagateType(func, n->val.call.param);
    //    //if(totalInfo(rnts) == 1)
    //    //    currentIn = rtns->next->val;
    //    //else
    //    TODO("Add support for single-/multi-var return\n");
    //}
    scanNode(n->val.call.param, flow);
}

static void setFlow(ChainList *dest, ChainList *src){
    // clean flow?
    cleanFlow(dest); // dummy node remained
    dest->next = src->next; // set flow
}

static void scanReturn(Node *n, ChainList *flow){
    scanList(n->val.listS, flow);
    ChainList *newFlow = mergeFlow(flow_sink, flow);
    setFlow(flow_sink, newFlow);
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
    setFlow(flow, newFlow);
    //// clean flow?
    //cleanFlow(flow);  // dummy node remained
    //flow->next = newFlow->next; // set flow
}

// same as copyFlow
static ChainList *dupFlow(ChainList *flow){
    if(flow){
        ChainList *t = dupFlow(flow->next);
        ChainList *newChain = NEW(ChainList);
        newChain->chain = flow->chain;
        newChain->next  = t;
        return newChain;
    }
    return NULL;
}

static void freeFlow(ChainList *flow){
    if(flow){ freeFlow(flow->next); free(flow); }
}


static ChainList *mergeFlowListBody(FlowList *list){
    if(list){
        ChainList *flow = mergeFlowListBody(list->next);
        return flow?mergeFlow(flow, list->flow):list->flow;
    }
    return NULL;
}

// has dummy
static ChainList *mergeFlowList(FlowList *flowList){
    return mergeFlowListBody(flowList->next);
}

static void scanWhileRepeat(Node *n, ChainList *flow, bool isWhile){
    ChainList *entryFlow = copyFlow(flow);
    ChainList *mergedFlow = NULL, *newFlow = NULL;
    int cnt = 0;
    FlowList *prev_input = flow_list_input->next;
    FlowList *prev_exit  = flow_list_exit->next;
    flow_list_input->next = NULL;
    flow_list_exit->next  = NULL;
    //P("Starting while/repeat...");
    while(true){
        if(mergedFlow){
            addToFlowList(flow_list_input, entryFlow);
            newFlow = mergeFlowList(flow_list_input);
            cleanFlowList(flow_list_input);
            if(sameFlow(mergedFlow, newFlow)) {
                addToFlowList(flow_list_exit, newFlow); break;
            }
            else {setFlow(flow, newFlow);}
            freeFlow(mergedFlow);
            cleanFlowList(flow_list_input);
            cleanFlowList(flow_list_exit);
        }
        mergedFlow = copyFlow(flow);
        //P("Mergedflow: \n"); printFlow(mergedFlow); getchar();
        if(isWhile){
            scanNode(n->val.whileStmt.condExpr , flow);
            scanNode(n->val.whileStmt.bodyBlock, flow);
        }
        else {
            scanNode(n->val.repeatStmt.condExpr , flow);
            scanNode(n->val.repeatStmt.bodyBlock, flow);
        }
        addToFlowList(flow_list_input, flow);
        //P("Iteration: \n"); printFlow(flow); getchar();
        cnt++;
    }
    newFlow = mergeFlowList(flow_list_exit);
    setFlow(flow, newFlow);
    P("Scan while/repeat with %d loops to a fixed point\n", cnt); //getchar();
    cleanFlowList(flow_list_input);
    cleanFlowList(flow_list_exit);
    flow_list_input->next = prev_input;
    flow_list_exit->next  = prev_exit;
}

static void scanWhile(Node *n, ChainList *flow){
    scanWhileRepeat(n, flow, true);
}

static void scanRepeat(Node *n, ChainList *flow){
    scanWhileRepeat(n, flow, false);
}

static void scanContinue(Node *n, ChainList *flow){
    addToFlowList(flow_list_input, flow);
}

static void scanBreak(Node *n, ChainList *flow){
    addToFlowList(flow_list_exit, flow);
}

static void scanGlobal(Node *n){
    char *name2 = strName2(currentModuleName, n->val.global.id);
    addDefNameToChain(currentChain, name2);
}

static void scanMethod(Node *n){
    ChainList *flow = NEW(ChainList);
    chain_list->next = NULL;
    scanNode(n->val.method.block, flow);
    //printFlow(flow);
    //printFlow(flow_sink);
    printChainList(chain_list); // print all chains
    n->val.method.meta->chains = chain_list; // with dummy
    // TODO: assign chain_list to a method node
    // TODO: clean chain_list
}

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
             //currentChain = val?initChainWithNode(val):NULL;
            { Chain *chain = findChainInList(chain_list->next, val);
                if(chain) currentChain = chain;
                else {
                    currentChain = initChainWithNode(val);
                    addToChainList(chain_list, currentChain);
                }
            } break;
    }
}

static void scanNode(Node *n, ChainList *flow){
    if(!n) return ;
    //printNodeType(n);
    //if(!flow) WP("flow is NULL"); else WP("flow is NOT NULL");
    Chain *prevChain = currentChain;
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
        case  continueK: scanContinue  (n,flow); break;
        case     breakK: scanBreak     (n,flow); break;
        case        ifK: scanIf        (n,flow); break;
        case     whileK: scanWhile     (n,flow); break;
        case    repeatK: scanRepeat    (n,flow); break;
        case     blockK: scanBlock     (n,flow); break; //
        case       varK: scanVar       (n,flow); break; //
        case    globalK: scanGlobal    (n); break; //
        /* TODO: add more kinds if possible */
    }
    //setCurrentNode(n, NULL);
    currentChain = prevChain;
}

static void scanList(List *list, ChainList *flow){
    if(list){
        scanList(list->next, flow);
        if(list->val) scanNode(list->val, flow);
    }
}

static void init(){
    isLHS = false;
    currentModuleName = NULL;
    flow_sink = NEW(ChainList);
    chain_list = NEW(ChainList);
    flow_list_input = NEW(FlowList);
    flow_list_exit  = NEW(FlowList);
}

/* entry */
void buildUDChain(Prog *root){
    printBanner("UD Chaining");
    init();
    scanList(root->module_list, NULL);
}

