#include "../global.h"
 
static void scanNode(Node *n);
static void scanList(List *list);

#define scanModule(n)  scanList(n->val.module.body)
#define scanMethod(n)  scanNode(n->val.method.block)
#define scanBlock(n)   scanList(n->val.block.stmts)
#define scanArgExpr(n) scanList(n->val.listS);
#define scanCast(n)    scanNode(n->val.cast.exp)

/* --------------- above declarations --------------- */

static bool isFuncName(Node *n, char *name){
    SymbolName *sn = n->val.name.sn;
    return sn->kind == builtinS && !strcmp(n->val.name.id2, name);
}

static void updateFuncName(Node *n, char *newName){
    n->val.name.id2 = strdup(newName); // Think: Free old?
}

static bool isIntegerNumber(V x, int n){
    if(xn == 1){
        switch(xt){
            caseB return xb == n;
            caseJ return xj == n;
            caseH return xh == n;
            caseI return xi == n;
            caseL return xl == n;
            caseF return xf == n;
            caseE return xe == n;
        }
    }
    return false;
}

static List *dupParam(List *list){
    List *x = NEW(List);
    x->val = list->val;
    return x;
}

/*
 * case 1: power(x, 2) ==> x * x
 * case 2: plus (x, 2) ==> x + x
 * case 3: plus (2, x) ==> x + x
 */
static bool patternPower(Node *n){
    Node *funcName = n->val.call.func;
    Node *funcParam= n->val.call.param;
    List *p1 = funcParam->val.listS; // 2nd param
    int numParam = totalElement(p1);
    if(isFuncName(funcName, "power") && numParam == 2){
        List *p0 = p1->next; // 1st param
        if(instanceOf(p1->val, vectorK) \
            && isIntegerNumber(fetchVector(p1->val), 2)){
            updateFuncName(funcName, "mul");
            List *x = dupParam(p0);
            x->next = p0;
            funcParam->val.listS = x; // Think: Free p1?
            return true;
        }
    }
    else if(isFuncName(funcName, "mul") && numParam == 2){
        List *p0 = p1->next;
        if(instanceOf(p1->val, vectorK) \
            && isIntegerNumber(fetchVector(p1->val), 2)){
            updateFuncName(funcName, "plus");
            List *x = dupParam(p0);
            x->next = p0;
            funcParam->val.listS = x; // Think: Free p1?
            return true;
        }
        else if(instanceOf(p0->val, vectorK) \
            && isIntegerNumber(fetchVector(p0->val), 2)){
            updateFuncName(funcName, "plus");
            List *x = dupParam(p1);
            p1->next = x;
            return true;
        }
    }
    return false;
}


// scan ...

static void scanAssignStmt(Node *n){
    scanNode(n->val.assignStmt.expr); 
}

static void scanCall(Node *n){
    if(patternPower(n)){
        printNode(n); //getchar();
    }
    // TODO: add more patterns
}

static void scanNode(Node *n){
    switch(n->kind){
        case    moduleK: scanModule    (n); break;
        case    methodK: scanMethod    (n); break;
        case     blockK: scanBlock     (n); break;
        case      stmtK: scanAssignStmt(n); break;
        case      callK: scanCall      (n); break;
        case      castK: scanCast      (n); break;
        //case   argExprK: scanArgExpr   (n); break;
        //case      nameK: scanName      (n); break;
        //case    vectorK: scanVector    (n); break;
    }
}

static void scanList(List *list){
    if(list){ scanList(list->next); scanNode(list->val); }
}

static void init(){ }

/* entry */
void optSReduction(Prog *root){
    printBanner("Strength Reduction");
    init();
    scanList(root->module_list);
}
