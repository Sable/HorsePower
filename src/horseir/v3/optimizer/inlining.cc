#include "../global.h"

#define SIMPLE_HASH_SIZE (1<<10)

extern Node *entryMain;
extern Prog *root;
sHashTable *visitMap, *hashInline, *hashMethod;

#define scanCast(n) scanNode(n->val.cast.exp)

static O scanNode(Node *n);
static O scanList(List *list);

static int countList(List *list){
    int tot = 0;
    while(list){ tot++; list = list->next; }
    return tot;
}

static Node* getReturnStmt(ChainList *list){
    while(list){
        Chain *chain = list->chain;
        if(instanceOf(chainNode(chain), returnK)){
            if(chain->defSize == 1){
                Node *targ = chainNode(chain->chain_defs[0]);
                // printNode(targ);
                addToSimpleHash(visitMap, (L)chainNode(chain), 1);
                addToSimpleHash(visitMap, (L)targ, 1);
                R targ;
            }
            else{
                TODO("Support when multiple defs: %d", chain->defSize);
            }
        }
        list = list->next;
    }
    R NULL;
}

static List *copyMethodStmt(List *list){
    if(list){
        List *newList = copyMethodStmt(list->next);
        if(!lookupSimpleHash(visitMap, (L)list->val)){
            return makeList(newList, dumplicateNode(list->val));
        }
        else {
            return newList;
        }
    }
    return NULL;
}

static O printLocalList(List *list){
    if(list){
        printLocalList(list->next);
        printNode(list->val);
    }
}

S strMethodParam[99], strParam[99];
static I sizeParam;

static I fetchParamName(Node *param, S *names){
    List *list = param->val.listS;
    I c = 0;
    while(list){
        names[c++] = getNameStr(list->val);
        list = list->next;
    }
    R c;
}

static O updateExpr(List *list){
    while(list){
        Node *n = list->val;
        if(instanceOf(n, nameK)){
            DOI(sizeParam, if(sEQ(getNameStr(n), strMethodParam[i])){
                    nodeName2(n) = strdup(strParam[i]);
                })
        }
        list = list->next;
    }
}

static O updateListWithParam(List *list){
    if(list){
        updateListWithParam(list->next);
        updateExpr(getNodeParams(list->val));
    }
}

static List* inlineMethod(Node *var, Node *callee, Node *param){
    WP("Start inlining method:\n\t"); printNode(callee);
    WP("Assign to variable: \n\t"); printNode(var);
    WP("With parameters:\n\t"); printNode(param);
    ChainList *chains = nodeMethodChainList(callee);
    // printChainList(chains); getchar();
    WP("step 1: get return stmt\n");
    Node *calleeReturn = getReturnStmt(chains);
    WP("step 2: create a new return stmt\n");
    Node *newNode = makeNodeAssignment(
            makeList(NULL, var),
            nodeStmtExpr(calleeReturn));
    // printNode(newNode);
    WP("step 3: copy all local stmts\n");
    List *newStmts = makeList(
            copyMethodStmt(callee->val.method.block->val.listS),
            newNode);
    // printLocalList(newStmts);
    I size1 = fetchParamName(nodeMethodParameters(callee), strMethodParam);
    I size2 = fetchParamName(param, strParam);
    if(size1 == size2){
        sizeParam = size1;
        updateListWithParam(newStmts);
    }
    else{
        EP("Unknown sizes: %d vs. %d", size1, size2);
    }
    // printLocalList(newStmts);
    return newStmts;
}

static Node* scanAssignStmt(Node *n){
    Node *rhsExpr = nodeStmtExpr(n);
    if(instanceOf(rhsExpr, callK)){
        Node *callFunc  = nodeCallFunc(rhsExpr);
        SymbolKind sk = nodeNameKind(callFunc);
        if(sk == methodS){
            // inline method
            printNode(n);
            // S methodName = nodeName2(callFunc);
            // P("method name = %s\n", methodName);
            Node *callee = nodeNameMethod(callFunc); // called method
            List *rhsVars = nodeStmtVars(n);
            if(countList(rhsVars) == 1){
                List *newList = inlineMethod(rhsVars->val, callee, nodeCallParam(rhsExpr));
                if(newList){
                    addToSimpleHash(hashInline, (L)n, (L)newList);
                }
            }
            else {
                TODO("Support multiple vars");
            }
        }
    }
    R NULL;
}

static O scanBlock(Node *n){
    scanList(n->val.listS);
}

static O scanNode(Node *n){
    switch(n->kind){
        case  stmtK: scanAssignStmt(n); break;
        case  castK: scanCast      (n); break;
        case blockK: scanBlock     (n); break;
        default: break;
    }
}

static void scanList(List *list){
    if(list){
        scanList(list->next);
        scanNode(list->val);
    }
}

static O scanMainMethod(Node *method){
    scanNode(method->val.method.block);
}

static O updateMethod(Node *method){
    addToSimpleHash(hashMethod, (L)method, 1);
    List *stmts = method->val.method.block->val.listS;
    List *newHead = NEW(List); newHead->next = NULL;
    List *t = newHead;
    while(stmts){
        Node *n = stmts->val;
        if(instanceOf(n, stmtK)){
            // updateAssignStmt(n);
            List *newList = (List*)lookupSimpleHash(hashInline, (L)n);
            if(newList)
                t->next = newList;
            else
                t->next = makeList(NULL, n);
        }
        else {
            t->next = makeList(NULL, n);
        }
        while(t->next) t=t->next;
        stmts = stmts->next;
    }
    method->val.method.block->val.listS = newHead->next;
}

static O init(){
    visitMap   = initSimpleHash(SIMPLE_HASH_SIZE);
    hashInline = initSimpleHash(SIMPLE_HASH_SIZE);
    hashMethod = initSimpleHash(SIMPLE_HASH_SIZE);
}

static O clean(){
}

O optInlining(){
    init();
    scanMainMethod(entryMain);
    updateMethod(entryMain);
    // printNode(entryMain); getchar();
    buildUDChain(root);  // rebuild
    clean();
}

