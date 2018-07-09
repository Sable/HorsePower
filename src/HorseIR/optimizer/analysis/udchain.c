#include "../global.h"

/* signatures */
void scanNode(Node *n);
void scanList(List *list);
void addToDUChain(Chain *defChain, Chain *useChain);
void addToUDChain(Chain *useChain, Chain *defChain);
Chain *initChainWithNode(Node *val);
void addDefNameToChain(Chain *chain, char *defName);
void addUseNameToChain(Chain *chain, char *useName);
void printChainList();
//void printChainInfo(Chain *chain, char opt);
void printUDChain(Chain *chain);

#define scanModule(n)    scanList(n->val.module.body)
#define scanMethod(n)    scanList(n->val.method.list)
#define scanParamExpr(n) scanList(n->val.listS)
#define scanParam(n)     scanNode(n->val.nodeS)
#define isFuncCall(n)    (n->val.expr.func)

Chain *currentChain, *exitChain;
ChainList *chain_list, *chain_rt;
InfoNode *currentIn;

void addToChainList(Chain *c){
    chain_rt->next = NEW(ChainList);
    chain_rt = chain_rt->next;
    chain_rt->chain = c;
}

void addToNameList(NameList *n, char *str){
    n->next = NEW(NameList);
    n = n->next;
    n->name = strdup(str);
}

char *fetchName(Node *n){
    if(n->kind == idK) return n->val.idS;
    else if(n->kind == funcK) return n->val.idS;
    else if(n->kind == literalFuncK) return fetchName(n->val.nodeS); // next: idK
    else {
        P("id found: %d not %d\n", n->kind, idK);
        error("Kind must be idK (0) or funcK(14)");
    }
}

/* build def-use chains */
void scanID(Node *n){
    if(currentChain){
        char *useName = fetchName(n);
        addUseNameToChain(currentChain, useName);
        Chain *chain = getChain(useName);
        //P("use: %-3s -> ", useName);
        //  if(chain == NULL) P("not found\n");
        //  else prettyNode(chain->cur);
        addToUDChain(currentChain, chain); // use-def chain
        addToDUChain(chain, currentChain); // def-use chain
    }
}

static void scanExpr(Node *n){
    if(isFuncCall(n)){
        char *funcName = fetchName(n->val.expr.func);
        currentIn = propagateType(funcName, n->val.expr.param);
    }
    else currentIn = propagateTypeCopy(n->val.expr.param);
    scanNode(n->val.expr.param);
}

void scanSimpleStmt(Node *n){
    char *defName = fetchName(n->val.simpleStmt.name);
    addDefNameToChain(currentChain, defName);
    addToChainList(currentChain);
    /* continue scan */
    scanNode(n->val.simpleStmt.expr); 
    insertString(defName, currentChain, currentIn);
}

void scanCastStmt(Node *n){
    char *defName = fetchName(n->val.castStmt.name);
    addDefNameToChain(currentChain, defName);
    addToChainList(currentChain);
    /* continue scan */
    scanNode(n->val.castStmt.expr); 
    propagateTypeCast(currentIn, n->val.castStmt.cast);
    insertString(defName, currentChain, currentIn);
}

void scanReturn(Node *n){
    exitChain = currentChain;
    addToChainList(currentChain);
    /* continue scan */
    scanNode(n->val.nodeS);
}

void setCurrentNode(Node *n, Node *val){
    switch(n->kind){
        case simpleStmtK:
        case   castStmtK:
        case     returnK: currentChain = initChainWithNode(val); break;
    }
}

void scanNode(Node *n){
    setCurrentNode(n, n);
    switch(n->kind){
        case       moduleK: scanModule    (n); break;
        case       methodK: scanMethod    (n); break;
        case   simpleStmtK: scanSimpleStmt(n); break;
        case     castStmtK: scanCastStmt  (n); break;
        case         exprK: scanExpr      (n); break;
        case    paramExprK: scanParamExpr (n); break;
        case literalParamK: scanParam     (n); break;
        case           idK: scanID        (n); break;
        case       returnK: scanReturn    (n); break;
    }
    setCurrentNode(n, NULL);
}

void scanList(List *list){
    if(list){
        List *ptr = list;
        while(ptr){
            if(ptr->val) scanNode(ptr->val);
            ptr = ptr->next;
        }
    }
}

void initUDChain(){
    chain_list = chain_rt = NEW(ChainList);
}

void printChainList(){
    printBanner("Chain list");
    ChainList *p = chain_list;
    int lineno = 0;
    while(p->next){
        p = p->next;
        P("[%2d] %d: ",lineno++,p->chain->isVisited); prettyNode(p->chain->cur);
    }
}

void addDefNameToChain(Chain *chain, char *defName){
    if(!chain->defs){ chain->defs = NEW(NameList); }
    NameList *defs = chain->defs;
    while(defs->next) defs = defs->next;
    addToNameList(defs, defName);
}
void addUseNameToChain(Chain *chain, char *useName){
    if(!chain->uses){ chain->uses = NEW(NameList); }
    NameList *uses = chain->uses;
    while(uses->next) uses = uses->next;
    addToNameList(uses, useName);
}


Chain *initChainWithNode(Node *val){
    Chain *chain = NEW(Chain);
    chain->cur = val;
    int initSize = 2;
    chain->defSize = 0; chain->maxDSize = initSize;
    chain->useSize = 0; chain->maxUSize = initSize;
    chain->chain_uses = NEW2(Chain, initSize);
    chain->chain_defs = NEW2(Chain, initSize);
    return chain;
}

void addToUDChain(Chain *useChain, Chain *defChain){
    if(useChain->defSize + 1 >= useChain->maxDSize){
        int maxSize = useChain->maxDSize;
        int maxSize2= maxSize << 1;
        Chain **temp = NEW2(Chain, maxSize2);
        memcpy(temp, useChain->chain_defs, sizeof(Chain*)*maxSize);
        free(useChain->chain_defs);
        useChain->chain_defs = temp;
        useChain->maxDSize   = maxSize2;
    }
    useChain->chain_defs[useChain->defSize++] = defChain;
}

void addToDUChain(Chain *defChain, Chain *useChain){
    if(defChain->useSize + 1 >= defChain->maxUSize){
        int maxSize = defChain->maxUSize;
        int maxSize2= maxSize << 1;
        Chain **temp = NEW2(Chain, maxSize2);
        memcpy(temp, defChain->chain_uses, sizeof(Chain*)*maxSize);
        free(defChain->chain_uses);
        defChain->chain_uses = temp;
        defChain->maxUSize   = maxSize2;
    }
    defChain->chain_uses[defChain->useSize++] = useChain;
}


void printNameList(NameList *list){
    NameList *ptr = list;
    int c = 0;
    P("(");
    while(ptr->next){ ptr=ptr->next; if(c>0) P(","); P("%s", ptr->name); c++; }
    P(")\n");
}

/*
opt:
  'd': only defs
  'u': only uses
  'a': both
*/
void printChainInfo(Chain *chain, char opt){
    P("Chain    : "); if(chain->cur) prettyNode(chain->cur);     else P("Error\n");
    P("Name defs: "); if(chain->defs)printNameList(chain->defs); else P("None\n");
    P("Name uses: "); if(chain->uses)printNameList(chain->uses); else P("None\n");
    if(opt == 'd')
        P("# of defs: %d (cap %d)\n\n", chain->defSize, chain->maxDSize);
    else if(opt == 'u')
        P("# of uses: %d (cap %d)\n\n", chain->useSize, chain->maxUSize);
    else {
        P("# of defs: %d (cap %d)\n"  , chain->defSize, chain->maxDSize);
        P("# of uses: %d (cap %d)\n\n", chain->useSize, chain->maxUSize);
    }
}

void printUDChain(Chain *chain){
    printChainInfo(chain, 'd');
    DOI(chain->defSize, printUDChain(chain->chain_defs[i]))
}

/* entry */
void buildUDChain(Prog *root){
    scanList(root->module_list);
    //printChainList();
    //printUDChain(exitChain);
}

