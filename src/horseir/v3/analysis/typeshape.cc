#include "../global.h"

typedef InfoNode* (*NilFunc)();
typedef InfoNode* (*MonFunc)(InfoNode*);
typedef InfoNode* (*DyaFunc)(InfoNode*, InfoNode*);
typedef InfoNode* (*AllFunc)(InfoNodeList*);
// InfoNodeList has a leading dummy node
#define isSingleType(n) (!(n)->next && !(n)->subInfo)

static void scanList(List *list);
static void scanNode(Node *n);
static void scanMethod(Node *n);
static InfoNode *scanType(Node *n);
static void copyInfoNodeList(InfoNodeList *list, InfoNodeList *vals);
static bool infoCompatible(InfoNode *x, InfoNode *y);

static bool H_SHOW;
InfoNodeList *currentInList;

static Node *currentMethod;
extern Node *entryMain;
List *compiledMethodList;

sHashTable *hashScan, *hashMeta;

/*  ---- above declarations ---- */

static InfoNode *getNode(InfoNodeList *list, int k){
    while(list->next && k>=0){ list=list->next; k--; }
    return list->in;
}

static InfoNode *propagateBuiltin(char *funcName, InfoNodeList *list){
    int numArg = totalInfoNodeList1(list);
    int valence = -2;
    InfoNode *newNode = NULL;
    void* funcRtn = getTypeRules(funcName, &valence);  /* entry */
    if(!funcRtn) {
        EP("Error type rules found in *%s*", funcName);
    }
    if(numArg == valence){
        if(valence == 2){ // binary
            DyaFunc func = (DyaFunc)funcRtn;
            newNode = func(getNode(list,1), getNode(list,0));
        }
        else if(valence == 1){
            MonFunc func = (MonFunc)funcRtn;
            newNode = func(getNode(list,0));
        }
        else if(valence > 2){
            AllFunc func = (AllFunc)funcRtn;
            newNode = func(list);
        }
        else if(valence == 0){
            NilFunc func = (NilFunc)funcRtn;
            newNode = func();
        }
        else EP("Valence must >= 0");
    }
    else if(valence == -1){ // match anything
        AllFunc func = (AllFunc)funcRtn;
        newNode = func(list);
    }
    else EP("Builtin function valence mismatch (%s): %d vs. %d", funcName, numArg, valence);
    // return
    if(newNode) return newNode;
    else {
        DOIr(numArg, printInfoNode(getNode(list,i)))
        EP("NULL type rules found for function : %s",funcName);
    }
}

static bool checkParamCompatible(SymbolNameList *args, InfoNodeList *list){
    if(list){
        bool r = checkParamCompatible(args->next, list->next);
        if(!r) return false;
        Node *var = args->symName->val.local;
        Node *typ = var->val.param.typ;
        // printInfoNode(typ->val.type.in);
        // printInfoNode(list->in); getchar();
        return infoCompatible(typ->val.type.in, list->in);
    }
    return true;
}

static InfoNodeList *propagateMethod(Node *n, InfoNodeList *inputList){
    int numArg = totalInfoNodeList1(inputList);
    MetaMethod *meta = n->val.method.meta;
    int valence = totalSymbolNameList(meta->paramVars);
    if(numArg == valence){
        SymbolNameList *argList = meta->paramVars;
        if(H_SHOW) printInfoNode(inputList->next->in);
        if(!checkParamCompatible(argList, inputList->next)){
            EP("Type not compatible in parameters");
        }
        if(meta->isCompiling){
            meta->isCalled = true;
            return meta->returnTypes;
        }
        else {
            scanMethod(n);
            return meta->returnTypes;
        }
    }
    else EP("Method valence mismatch: %d vs. %d", numArg, valence);
}

InfoNodeList *propagateType(Node *func, InfoNodeList *list){
    //printNode(func);
    char *funcName = func->val.name.id2;
    SymbolName *sn = getSymbolName(func->val.name.st, funcName);
    switch(sn->kind){
        case builtinS:
            {
                InfoNode *rtn = propagateBuiltin(funcName, list);
                cleanInfoList(currentInList);
                addToInfoList(currentInList, rtn);
            } break;
        case methodS:
            {
                InfoNodeList *rtns = propagateMethod(sn->val.method, list);
                cleanInfoList(currentInList);
                copyInfoNodeList(currentInList, rtns);
            } break;
        default: TODO("Support kind: %d\n", sn->kind);
    }
    return currentInList;
}

// --- below workspace

InfoNode *addToInfoList(InfoNodeList *list, InfoNode *in){
    InfoNodeList *x = NEW(InfoNodeList);
    x->in   = in;
    x->next = list->next;
    list->next = x;
    return in;
}

void cleanInfoList(InfoNodeList *in_list){
    InfoNodeList *x = in_list->next;
    while(x){ InfoNodeList *t = x; x=x->next; free(t);}
    in_list->next = NULL;
}

static void copyInfoNodeList(InfoNodeList *list, InfoNodeList *vals){
    if(vals){
        copyInfoNodeList(list, vals->next);
        addToInfoList(list, vals->in);
    }
}

#define copyShapeNode(x, y) *(x)=*(y)  // a duplicate macro in typerule.c
static InfoNode *dupInfoNode(InfoNode *x){
    InfoNode *in = NEW(InfoNode);
    in->type  = x->type;
    in->subInfo = x->subInfo?dupInfoNode(x->subInfo):NULL;
    in->next  = x->next?dupInfoNode(x->next):NULL;
    in->shape = NEW(ShapeNode);
    copyShapeNode(in->shape, x->shape);
    return in;
}

static InfoNode *deepAddToInfoList(InfoNodeList *list, InfoNode *in){
    InfoNodeList *x = NEW(InfoNodeList);
    x->in = dupInfoNode(in);
    x->next = list->next;
    list->next = x;
    return in;
}

static void deepCopyInfoNodeList(InfoNodeList *list, InfoNodeList *vals){
    if(vals){
        deepCopyInfoNodeList(list, vals->next);
        deepAddToInfoList(list, vals->in);
    }
}

static InfoNode *getInfoVector(Node *n){
    int num = totalList(n->val.vec.val);
    InfoNode *in = scanType(n->val.vec.typ);  // because of you?
    ShapeNode *sn = newShapeNode(vectorH, SN_CONST, num);
    sn->consts = n;  // <-- pass constant values
    in->shape  = sn;
    //P("consts = %lld", (L)(in->consts));
    //printNode(n); printInfoNode(in); getchar();
    return in;
}

static InfoNode *getInfoFunc(Node *n){
    int num = totalList(n->val.listS);
    InfoNode *in = NEW(InfoNode);
    in->type  = funcT;
    in->shape = newShapeNode(vectorH, SN_CONST, num);
    in->funcs = n; // <---
    return in;
}

InfoNode* getInfoFromNode(Node *n){
    if(instanceOf(n, typeK)){
        return n->val.type.in;
    }
    else if(instanceOf(n, nameK)){
        SymbolName *x = getSymbolName(n->val.name.st, n->val.name.id2);
        Node *t = NULL;
        switch(x->kind){
            case globalS: t = x->val.global; break;
            case localS : t = x->val.local ; break;
            default: EP("Kind not supported:%d", x->kind);
        }
        return getInfoFromNode(t->val.param.typ);
    }
    else if(instanceOf(n, vectorK)){
        return getInfoVector(n);
    }
    else if(instanceOf(n, varK)){
        return getInfoFromNode(n->val.param.typ);
    }
    else if(instanceOf(n, funcK)){
        return getInfoFunc(n);
    }
    else EP("Not supported: %s", getNodeTypeStr(n));
    return NULL;
}

static void copyInfoNode(InfoNode *x, InfoNode* y){
    if(x && y){
        // ?? free old x->shape/subInfo/next
        x->type    = y->type;
        x->shape   = y->shape;
        x->subInfo = y->subInfo;
        x->next    = y->next;
    }
    else EP("x or y is NULL");
}

// true : exactly same
// false: not same
static bool infoCompatible(InfoNode *x, InfoNode *y){
    if(isW(y) || (x->shape && !y->shape)){
        // TODO: Is a shape info necessary?
        printInfoNode(x);
        printInfoNode(y);
        EP("Right hand side type or shape unknown");
    }
    if(isW(x)) {
        copyInfoNode(x, y);
        return true;
    }
    else {
        if(checkType(x,y) && checkShape(x,y)){ // shape not necessary
        //if(checkType(x,y)){
            bool partSub = false, partNext = false;
            // sub field
            if(!x->subInfo && !y->subInfo){
                partSub = true;
            }
            else if(x->subInfo && y->subInfo){
                partSub = infoCompatible(x->subInfo, y->subInfo);
            }
            if(!partSub) return false;
            // next field
            if(!x->next && !y->next) {
                partNext = true;
            }
            else if(x->next && y->next){
                partNext = infoCompatible(x->next, y->next);
            }
            return partNext; //  partSub && partNext
        }
        else {
            P("------------\n");
            printInfoNode(x);
            printInfoNode(y);
            EP("Shape incompatible: upon two types");
            return false;
        }
    }
}

static bool typeCompatible(Node *x, InfoNode *in){
    // x: varK
    InfoNode *ix = getInfoFromNode(x);
    return infoCompatible(ix, in);
}

static bool typelistCompatible(int num, List *x, InfoNodeList *list){
    //    x:  no dummy
    // list: has dummy
    InfoNodeList *p = list->next;
    for(int i=0;i<num;i++){
        if(!typeCompatible(x->val, p->in)) return false;
        x = x->next;
        p = p->next;
    }
    return true;
}

static bool typeCastable(InfoNode *curIn, InfoNode *castIn){
    if(isW(castIn))
        EP("Cast type must be a specific type (not wildcard)");
    if(isW(curIn)){
        castIn->shape = curIn->shape;
        return true;
    }
    else {
        bool partSub = false, partNext = false;
        if(checkType(curIn, castIn)){
            // sub field
            if(!curIn->subInfo && !castIn->subInfo){
                partSub = true;
            }
            else if(curIn->subInfo && castIn->subInfo){
                partSub = typeCastable(curIn->subInfo, castIn->subInfo);
            }
            if(!partSub) return false;
            // next field
            if(!curIn->next && !castIn->next){
                partNext = true;
            }
            else if(curIn->next && castIn->next){
                partNext = typeCastable(curIn->next, castIn->next);
            }
            castIn->shape = curIn->shape;
            return partNext;
        }
        return false;
    }
}

// both lists no dummy
static bool typeInfoNodeListCompatible(InfoNodeList *list_x, InfoNodeList *list_y){
    if(list_x){
        bool t = typeInfoNodeListCompatible(list_x->next, list_y->next);
        return t?infoCompatible(list_x->in, list_y->in):false;
    }
    return true;
}

static void printInfoVars(List *list){
    if(list){
        printInfoVars(list->next);
        InfoNode *x = getInfoFromNode(list->val);
        printInfoNode(x);
    }
}

static void scanModule(Node *n){
    scanList(n->val.module.body);
}

static bool compatibleReturns(InfoNodeList *oldIn, InfoNodeList *newIn){
    if(oldIn){
        bool r = compatibleReturns(oldIn->next, newIn->next);
        return r?infoCompatible(oldIn->in, newIn->in):false;
    }
    return true;
}

static void addMethodToList(List *list, Node *n){
    List *t = NEW(List);
    t->val = n;
    t->next = list->next;
    list->next = t;
}

static void scanMethod(Node *n){
    if(H_DEBUG)
        WP("Scanning method %s\n", n->val.method.fname);
    Node *prevNode = currentMethod;
    currentMethod = n;
    MetaMethod *meta = n->val.method.meta;
    meta->isCompiling = true;
    meta->isCalled    = false;
    if(!meta->isCompiled){
        meta->isCompiled = true;
        addMethodToList(compiledMethodList, n);
    }
    InfoNodeList *rtns = NEW(InfoNodeList);
    // scanNode(n->val.method.param); // TODO: main method has no params?
    int c = 1;
    while(c<3){
        cleanInfoList(rtns);
        deepCopyInfoNodeList(rtns, meta->returnTypes);
        //printInfoNode(rtns->next->in); getchar(); P("after copy ...\n");
        scanNode(n->val.method.block);
        //P("-----\n");
        //printInfoNode(rtns->next->in);
        //printInfoNode(meta->returnTypes->in);
        if(c == 1 && meta->isCalled) c = 2;
        else break;
    } // loop body executed 1 or 2 times
    if(H_SHOW) P("Method scanned round(s): %d\n", c); //getchar();
    meta->isCompiling = false;
    meta->isCalled    = false;
    currentMethod = prevNode;
    cleanInfoList(rtns); free(rtns);
}

#define scanListNode(n)   scanList(n->val.listS)
#define scanParams(n)     scanListNode(n)
#define scanArgExpr(n)    scanListNode(n)

// TODO: a,_ = ...
static void scanAssignStmt(Node *n){
    if(H_SHOW) printNode(n);
    List *vars = n->val.assignStmt.vars;
    Node *expr = n->val.assignStmt.expr;
    scanNode(expr);
    /* handle returns */
    int numVar = totalList(vars);
    if(instanceOf(expr, callK)){
        int numExpr = totalInfoNodeList1(currentInList);
        if(numVar == numExpr){
            // <---- type copied here if type compatible  ....
            if(typelistCompatible(numVar, vars, currentInList));
            else EP("Type error in assignment");
        }
        else EP("Assignment need %d, but %d returned", numVar, numExpr);
    }
    else {
        //printInfoNode(currentInList->next->in); getchar();
        if(numVar == 1 && typelistCompatible(1, vars, currentInList));
        else {
            printNode(n);
            EP("Type error in assignment");
        }
    }
    //printInfoNode(currentInList->next->in);
    if(H_SHOW) printInfoVars(vars);
    cleanInfoList(currentInList);
}

static void scanGlobal(Node *n){
    scanNode(n->val.global.op);
    /* handle returns */
    int numExpr = totalInfoNodeList1(currentInList);
    InfoNode *lhs = getInfoFromNode(n->val.global.typ);  // don't use scanType(..)
    InfoNode *rhs = currentInList->next?currentInList->next->in:NULL;
    if(numExpr == 1 && infoCompatible(lhs, rhs));
    else {
        printNode(n);
        EP("Type error in assignment");
    }
    cleanInfoList(currentInList);
}

static void scanCast(Node *n){
    scanNode(nodeCastExpr(n));
    InfoNode *castIn = getInfoFromNode(nodeCastType(n));
    InfoNode *curIn  = currentInList->next->in;
    if(typeCastable(curIn, castIn)){ // if ok, copy shape
        currentInList->next->in = castIn;
    }
}

static void scanIfStmt(Node *n){
    scanNode(n->val.ifStmt.condExpr );
    scanNode(n->val.ifStmt.thenBlock);
    scanNode(n->val.ifStmt.elseBlock);
}

static void scanWhileStmt(Node *n){
    scanNode(n->val.whileStmt.condExpr);
    scanNode(n->val.whileStmt.bodyBlock);
}

static InfoNodeList *scanRepeatStmt(Node *n){
    scanNode(n->val.repeatStmt.condExpr);
    scanNode(n->val.repeatStmt.bodyBlock);
    return NULL;
}

static void scanName(Node *n){
    SymbolName *x = getSymbolName(n->val.name.st, n->val.name.id2);
    switch(x->kind){
        case globalS: {Node *t=x->val.global; scanType(t->val.global.typ); } break;
        case localS : {Node *t=x->val.local;  scanType(t->val.param.typ);  } break;
        default: EP("Kind not supported: %d", x->kind);
    }
}

static void scanVector(Node *n){
    getInfoVector(n); // 'scanType' inside has added the type
}

static void scanFunc(Node *n){
    addToInfoList(currentInList, getInfoFunc(n));
}

static void scanCall(Node *n){
    cleanInfoList(currentInList);
    Node *funcName = n->val.call.func;
    scanNode(n->val.call.param);
    propagateType(funcName, currentInList);
}

static InfoNode *scanType(Node *n){
    return addToInfoList(currentInList, n->val.type.in);
}

static void scanExprStmt(Node *n){
    scanNode(n->val.exprStmt.expr);
}

static void scanBlockStmt(Node *n){
    scanList(n->val.block.stmts);
}

static void scanReturnStmt(Node *n){
    cleanInfoList(currentInList);
    scanListNode(n);
    MetaMethod *meta = currentMethod->val.method.meta;
    int numExpr = totalInfoNodeList1(currentInList);
    int numRtns = totalInfoNodeList(meta->returnTypes);
    if(numExpr == numRtns){
        if(numRtns > 0){
            if(typeInfoNodeListCompatible(meta->returnTypes, currentInList->next));
            else EP("Return type mismatch in method: %s", currentMethod->val.method.fname);
        }
    }
    else EP("Method expects %d returns, but gets %d", numRtns, numExpr);
}


static void scanNode(Node *n){
    if(!n) R;
    switch(n->kind){
        case    moduleK: scanModule      (n); break;
        case    methodK: scanMethod      (n); break;
        case      stmtK: scanAssignStmt  (n); break;
        case      castK: scanCast        (n); break;
        case  exprstmtK: scanExprStmt    (n); break;
        case paramExprK: scanParams      (n); break;
        case      callK: scanCall        (n); break;
        case      nameK: scanName        (n); break;
        case   argExprK: scanArgExpr     (n); break;
        case    returnK: scanReturnStmt  (n); break;
        //case  continueK: scanContinueStmt(n); break;
        //case     breakK: scanBreakStmt   (n); break;
        case        ifK: scanIfStmt      (n); break;
        case     whileK: scanWhileStmt   (n); break;
        case    repeatK: scanRepeatStmt  (n); break;
        case      typeK: scanType        (n); break;
        case     blockK: scanBlockStmt   (n); break;
        case    vectorK: scanVector      (n); break;
        case      funcK: scanFunc        (n); break;
        case    globalK: scanGlobal      (n); break;
        default: TODO("Add more type: %s\n", getNodeTypeStr(n));
    }
}

static void scanList(List *list){
    if(list){
        scanList(list->next);
        scanNode(list->val);
    }
}

static void init(){
    checkFuncNumber();
    currentMethod = NULL;
    currentInList = NEW(InfoNodeList);
    compiledMethodList = NEW(List);
    hashScan = initSimpleHash(512); // a magic number
    hashMeta = initSimpleHash(512); // a magic number
}

/*
 * entry:
 *   if(isShow) display type shape info
 */
O propagateTypeShape(Prog *root, B isShow){
    printBanner("Type Shape Propagation"); // after constructing symbol tables
    init();
    H_SHOW = isShow;
    scanNode(entryMain);
}

