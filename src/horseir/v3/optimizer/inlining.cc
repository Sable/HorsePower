#include "../global.h"

#define SIMPLE_HASH_SIZE (1<<10)

extern Node *entryMain;
extern Prog *root;
sHashTable *visitMap, *hashInline, *hashMethod;

#define scanCast(n) scanNode(n->val.cast.exp)

static O scanNode(Node *n);
static O scanList(List *list);
static List* scanAssignStmt(Node *n);
static Node* copyNode(Node *n);
static List* copyNodeList(List *list);

static I method_id;
static S allVars[999];
static I allVarsSize;

static I countList(List *list){
    I tot = 0;
    while(list){ tot++; list = list->next; }
    return tot;
}

static B isStmtMethod(Node *n){
    if(instanceOf(n, stmtK)){
        Node *rhs = nodeStmtExpr(n);
        if(instanceOf(rhs, callK)){
            SymbolKind sk = nodeNameKind(nodeCallFunc(rhs));
            R sk == methodS;
        }
    }
    R false;
}

static I saveAllVars(List *list, S *allVars){
    I allVarsSize = 0;
    while(list){
        Node *n = list->val;
        if(instanceOf(n, stmtK)){
            List *vars = nodeStmtVars(n);
            while(vars){
                Node *k = vars->val;
                if(instanceOf(k, nameK)){ // skip varK
                    allVars[allVarsSize++] = nodeName2(k);
                }
                vars = vars->next;
            }
        }
        list = list->next;
    }
    // WP("allVarsSize = %d\n", allVarsSize);
    R allVarsSize;
}

static I findModifiedVars(S *x, I sx, S *y, I sy){
    // DOI(sx, WP("x[%lld] = %s\n",i,x[i]))
    // DOI(sy, WP("y[%lld] = %s\n",i,y[i]))
    DOI(sx, DOJ(sy, if(sEQ(x[i], y[j])) R i)) R -1;
}

static Node* copyNodeStmt(Node *n){
    List *lhs = nodeStmtVars(n);
    Node *rhs = nodeStmtExpr(n);
    R makeNodeAssignment(copyNodeList(lhs), copyNode(rhs));
}

static S genNewName(S name){
    C nameBuff[99];
    SP(nameBuff, "in%d_%s", method_id, name);
    R strdup(nameBuff);
}

static Node* copyNodeName(Node *n){
    Node *rtn = duplicateNode(n);
    nodeName2(rtn) = genNewName(nodeName2(n));
    R rtn;
}

static Node* copyNodeVar(Node *n){
    Node *rtn = duplicateNode(n);
    nodeVarName(rtn) = genNewName(nodeVarName(n));
    R rtn;
}

static Node* copyNodeCall(Node *n){
    Node *rtn = duplicateNode(n);
    nodeCallParam(rtn) = copyNode(nodeCallParam(n));
    R rtn;
}

static Node* copyArgExpr(Node *n){
    Node *rtn = duplicateNode(n);
    nodeList(rtn) = copyNodeList(nodeList(n));
    R rtn;
}

static Node* copyNodeCast(Node *n){
    Node *rtn = duplicateNode(n);
    nodeCastExpr(rtn) = copyNode(nodeCastExpr(n));
    R rtn;
}

static Node* copyNode(Node *n){
    switch(n->kind){
        case    stmtK: R copyNodeStmt(n);
        case    nameK: R copyNodeName(n);
        case     varK: R copyNodeVar(n);
        case    callK: R copyNodeCall(n);
        case argExprK: R copyArgExpr(n);
        case    castK: R copyNodeCast(n);
        case  vectorK: R n;
        default:
            printNode(n);
            EP("Support more node");
    }
}

static List* copyNodeList(List *list){
    if(list){
        List *newList = copyNodeList(list->next);
        return makeList(newList, copyNode(list->val));
    }
    R NULL;
}

static List* copyMethodStmt(List *list){
    if(list){
        List *newList = copyMethodStmt(list->next);
        Node *n = list->val;
        if(isStmtMethod(n)){
            I local_id = method_id;
            List *nextList = scanAssignStmt(n);
            method_id = local_id;
            List *p = nextList;
            while(p->next) p = p->next;
            p->next = newList;
            return nextList;
        }
        else if(instanceOf(n, returnK)){
            return newList;
        }
        else{
            return makeList(newList, copyNode(n));
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


static I fetchParamName(Node *param, S *names){
    List *list = param->val.listS;
    I c = 0;
    while(list){
        names[c++] = getNameStr(list->val);
        list = list->next;
    }
    R c;
}

static O updateExpr(List *list, S *strMethodParam, S *strParam, I sizeParam){
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

static O updateListWithParam(List *list, S *strMethodParam, S *strParam, I size){
    if(list){
        updateListWithParam(list->next, strMethodParam, strParam, size);
        updateExpr(getNodeParams(list->val), strMethodParam, strParam, size);
    }
}

static O updateStrParam(I size, S *name, I id){
    C nameBuff[99];
    DOI(size, { \
            SP(nameBuff, "in%d_%s", id, name[i]);
            name[i] = strdup(nameBuff); })
}

static List* inlineMethod(Node *var, Node *callee, Node *param){
    WP("Start inlining method:\n\t"); printNode(callee);
    WP("Assign to variable: \n\t"); printNode(var);
    WP("With parameters:\n\t"); printNode(param);
    // ChainList *chains = nodeMethodChainList(callee);
    // printChainList(chains); getchar();
    S allVars[99];
    I allVarsSize = saveAllVars(callee->val.method.block->val.listS, allVars);
    List *content = copyMethodStmt(callee->val.method.block->val.listS);
    Node *newNode = makeNodeAssignment(
            makeList(NULL, var),
            nodeStmtExpr(content->val));
    // printNode(content->val); printNode(newNode); getchar();
    List *newStmts = makeList(NULL, newNode);
    newStmts->next = content->next;
    // todo: remove the 1st node of content
    // printLocalList(newStmts);
    S strMethodParam[99], strParam[99];
    I size1 = fetchParamName(nodeMethodParameters(callee), strMethodParam);
    I size2 = fetchParamName(param, strParam);
    if(size1 == size2){
        I indx = findModifiedVars(allVars, allVarsSize, strMethodParam, size1);
        if(indx >= 0){
            TODO("A modified variable found: %s\n", allVars[indx]);
        }
        if(method_id > 1){
            updateStrParam(size2, strParam, method_id - 1);
        }
        updateListWithParam(newStmts, strMethodParam, strParam, size1);
    }
    else{
        EP("Unknown sizes: %d vs. %d", size1, size2);
    }
    printLocalList(newStmts);
    getchar();
    return newStmts;
}

static List* scanAssignStmt(Node *n){
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
                method_id++;
                List *newList = inlineMethod(rhsVars->val, callee, nodeCallParam(rhsExpr));
                if(newList){
                    addToSimpleHash(hashInline, (L)n, (L)newList);
                }
                R newList;
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
    method_id  = 0;
}

static O clean(){
}

O optInlining(){
    init();
    scanMainMethod(entryMain);
    updateMethod(entryMain);
    // printNode(entryMain); getchar();
    // rebuild code below
    buildSymbolTable(root);
    buildUDChain(root);
    clean();
}

