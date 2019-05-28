#include "../global.h"

typedef I (*NiladicFunc)(V);
typedef I (*MonadicFunc)(V,V);
typedef I (*DyadicFunc)(V,V,V);
typedef I (*AnyadicFunc)(V,L,V[]);
typedef I (*EachMonadic)(V,V,MonadicFunc);
typedef I (*EachDyadic)(V,V,V,DyadicFunc);
typedef I (*EachTriple)(V,V,V,V);

#define MonFuncSize 60
#define DyaFuncSize 33
#define OuterProduct  EachDyadic
#define JoinOperation EachTriple

#define VAR_MAX_SIZE 32

extern Node *entryMain;
static Node *currentMethod;

static O runList(List *list);
static O runNode(Node *n);
static O runMethod(Node *method, VList *param);

#define runListKind(n) runList(n->val.listS)
#define runBlock(n)    runListKind(n)
#define runArgExpr(n)  runListKind(n)

static VList *paramList, *rtnList;

/* ---- above declarations ---- */

/* pfnRand, pfnSeed, pfnCount */
MonadicFunc monFunc[MonFuncSize] = {
    pfnAbs, pfnNeg, pfnCeil, pfnFloor, pfnRound, pfnConj, pfnRecip, pfnSignum,
    pfnPi  , pfnNot , pfnLog, pfnLog2, pfnLog10, pfnExp, pfnTrigCos,
    pfnTrigSin, pfnTrigTan, pfnTrigAcos, pfnTrigAsin, pfnTrigAtan,
    pfnHyperCosh, pfnHyperSinh, pfnHyperTanh, pfnHyperAcosh, pfnHyperAsinh,
    pfnHyperAtanh, pfnDate, pfnDateYear, pfnDateMonth, pfnDateDay, pfnTime,
    pfnTimeHour, pfnTimeMinute, pfnTimeSecond, pfnTimeMill, pfnUnique, NULL,
    pfnLen, pfnRange, pfnFact, NULL, NULL, pfnFlip, pfnReverse, pfnWhere,
    pfnGroup, NULL, pfnSum, pfnAvg, pfnMin, pfnMax, pfnRaze, pfnEnlist,
    pfnToList, NULL, pfnKeys, pfnValues, pfnMeta, pfnLoadTable, pfnFetch
};

/* pfnRandk, pfnDrop, pfnTake, pfnDatetimeAdd, pfnDatetimeSub */
DyadicFunc  dyaFunc[DyaFuncSize] = {
    pfnLt, pfnGt, pfnLeq, pfnGeq, pfnEq, pfnNeq, pfnPlus, pfnMinus, pfnMul,
    pfnDiv, pfnPower, pfnLogBase, pfnMod, pfnAnd, pfnOr, pfnNand, pfnNor,
    pfnXor, NULL, pfnAppend, pfnLike, pfnCompress, NULL, pfnIndexOf, NULL,
    NULL, pfnOrderBy, pfnMember, pfnVector, pfnMatch, pfnIndex, pfnColumnValue,
    pfnSubString
};

static V executeMon(MonadicFunc f, V *p){
    V z = NEW(V0);
    I status = (*f)(z,p[0]);
    if(status==0) return z;
    else {P("[Monadic]"); printErrMsg(status);}
}

static V executeDya(DyadicFunc f, V *p){
    V z = NEW(V0);
    I status = (*f)(z, p[0], p[1]);
    if(status==0) return z;
    else {P("[Dyadic]"); printErrMsg(status);}
}

static V executeAny(AnyadicFunc f, V *p, L n){
    V z = NEW(V0);
    I status = (*f)(z, n, p);
    if(status==0) return z;
    else {P("[Anyadic]"); printErrMsg(status);}
}

static V executeIndexA(V *p){
    V z = NEW(V0);
    I status = pfnIndexA(z, p[0], p[1], p[2]);
    if(status==0) return z;
    else {P("[IndexA]"); printErrMsg(status);}
}

static V executeTriple(EachTriple f, V *p){
    if(H_DEBUG) P("executeTriple\n");
    V z = NEW(V0);
    I status = (*f)(z, p[0], p[1], p[2]);
    if(status==0) return z;
    else {printErrMsg(status);}
}

static V executeJoinIndex(JoinOperation f, V *p){
    if(H_DEBUG) P("executeJoinIndex\n");
    V z = NEW(V0);
    I status = (*f)(z, p[1], p[2], p[0]);
    if(status==0) return z;
    else {printErrMsg(status);}
}

static V executeEachDya(EachDyadic f, V *p){
    if(H_DEBUG) P("executeEachDya\n");
    V z = NEW(V0);
    S funcName = getSymbolStr(vq(p[0]));
    FuncUnit fu;
    getFuncIndexByName(funcName, &fu);
    if(fu.kind == 1){ // unary
        EP("[EachDya] (%s) not supported.\n", funcName);
    }
    else if(fu.kind == 2){ // binary
        I status = (*f)(z, p[1], p[2], dyaFunc[fu.b]);
        if(status==0) return z;
        else {P("[EachDyadic]"); printErrMsg(status);}
    }
    else {
        EP("dyadic op expected for each_left/right, not %s\n", funcName);
    } R 0;
}

static V executeEachMon(EachMonadic f, V *p){
    if(H_DEBUG) P("executeEachMon\n");
    V z = NEW(V0);
    S funcName = getSymbolStr(vq(p[0]));
    FuncUnit fu;
    getFuncIndexByName(funcName, &fu);
    if(fu.kind == 1){ // unary
        I status = (*f)(z, p[1], monFunc[fu.u]);
        if(status==0) return z;
        else {WP("[EachMonadic]"); printErrMsg(status);}
    }
    else if(fu.kind == 2){ // binary
        EP("Not supported: %s\n", funcName);
    }
    else {
        EP("Monadic op expected for each, not %s\n", funcName);
    } R 0;
}

V executeOther(TypeOther x, V *params, I numParams){
    switch(x){
        /* monadic */
        case        eachF: return executeEachMon(&pfnEach   , params); break;
        /* dyadic */
        case        enumF: return executeDya(&pfnEnum       , params); break;
        case       tableF: return executeDya(&pfnTable      , params); break;
        case   eachRightF: return executeEachDya(&pfnEachRight , params); break;
        case    eachLeftF: return executeEachDya(&pfnEachLeft  , params); break;
        case    eachItemF: return executeEachDya(&pfnEachItem  , params); break;
        case      indexAF: return executeIndexA(params); break;
        /* anyadic */
        case        listF: return executeAny(&pfnList, params, numParams); break;
        /* others */
        //case       outerF: return executeOuter(&pfnOuter, params); break;
        case   joinIndexF: return executeJoinIndex(&pfnJoinIndex, params); break;
        case       dtaddF: return executeTriple(&pfnDatetimeAdd, params); break;
        case       dtsubF: return executeTriple(&pfnDatetimeSub, params); break;
        /* dictF, ktableF */
        default: EP("pending ... for %d\n", x);
    }
    R NULL;
}


static I totalVList(VList *x){ // skip dummy
    I c=0; while(x->next){c++; x=x->next;} R c;
}

static O addParam(VList *list, V x){
    VList *t = NEW(VList);
    t->v = x;
    t->next = list->next;
    list->next = t;
}

static O cleanVList(VList *x){ // free x->v also?
    if(x && x->next){ cleanVList(x->next); free(x->next); x->next = NULL; }
}

static void printVList(VList *list){
    if(list){
        printVList(list->next);
        printV(list->v);
    }
}

static G runStack;
static L stackPtr;
#define STACK_SIZE 67108864  // 64MB

static L totalSymbolNames(SymbolNameList *list){
    L c=0; while(list){c++; list=list->next;} R c;
}

static O saveToStackSub(SymbolNameList *list){
    if(!list) R ;
    L size = totalSymbolNames(list) * sizeof(V);
    if(stackPtr + size < STACK_SIZE){
        V *x = (V*)(runStack + stackPtr);
        L i = 0;
        while(list){
            P("added: %s\n", list->symName->name);
            x[i++] = list->symName->value;
            list=list->next;
        }
        //P(">> save i = %lld\n", i);
        stackPtr += size;
    }
    else EP("Stack overflow\n");
}

static O saveToStack(Node *method){
    //P("save to stack\n");
    saveToStackSub(method->val.method.meta->paramVars);
    saveToStackSub(method->val.method.meta->localVars);
}

static O loadFromStackSub(SymbolNameList *list){
    L size = totalSymbolNames(list);
    stackPtr -= size *sizeof(V);
    V *x = (V*)(runStack + stackPtr);
    L i = 0;
    while(list){
        list->symName->value = x[i++];
        list=list->next;
    }
    //P(">> load i = %lld\n", i);
}

static O loadFromStack(Node *method){
    //P("load from stack\n");
    loadFromStackSub(method->val.method.meta->paramVars);
    loadFromStackSub(method->val.method.meta->localVars);
}

static O loadParams(Node *method, VList *param){
    SymbolNameList *list = method->val.method.meta->paramVars;
    param = param->next;
    while(list){
        //P("name = %s\n", list->symName->name);
        //P("value = \n"); printV(param->v);
        list->symName->value = param->v;
        list = list->next;
        param = param->next;
    }
}

static I getParamsSub(VList *list, V *rtn){
    if(list && list->next){
        I k = getParamsSub(list->next, rtn);
        rtn[k] = list->next->v;
        return k+1;
    }
    return 0;
}

// TODO: test a variable is used before assigned

static V *getParams(VList *list){
    I tot = totalVList(list);
    V *vals = NEW2(V0, tot);
    getParamsSub(list, vals);
    return vals;
}

static V invokeBuiltin(char *funcName, VList *list){
    FuncUnit fu;
    getFuncIndexByName(funcName, &fu);
    I valence = getValence(&fu);
    I numArg = totalVList(list);
    V *params = getParams(list);
    if(numArg == valence || valence == -1){ //-1: any
        if(valence == 1)
            return executeMon(monFunc[fu.u], params);
        else if(valence == 2)
            return executeDya(dyaFunc[fu.b], params);
        else
            return executeOther(fu.t, params, numArg);
    }
    else EP("valence error: %d expected, but %d found\n", valence, numArg);
    // TODO: free params
}

static O invokeFunction(Node *func, VList *param){
    char *funcName = func->val.name.id2;
    SymbolName *sn = getSymbolName(func->val.name.st, funcName);
    switch(sn->kind){
        case builtinS: {
              V rtn = invokeBuiltin(funcName, param);
              addParam(rtnList, rtn);
             } break;
        case  methodS: {
                runMethod(sn->val.method, param);
                rtnList->next = paramList->next; // save link
             } break;
        default: TODO("Support kind: %d\n", sn->kind);
    }
}

static O runCall(Node *n){
    Node *funcName = n->val.call.func;
    //char *funcName = n->val.call.func;
    cleanVList(rtnList);
    cleanVList(paramList);
    //P("size = %d\n", totalVList(rtnList)); getchar();
    runNode(n->val.call.param); 
    invokeFunction(funcName, paramList);
}

static L loadVector(List *list, V x, I t){
    if(list){
        L i = loadVector(list->next, x, t);
        loadConst(list->val, x, i, t);
        R i+1;
    }
    R 0;
}

static O runVector(Node *n){
    I t = getHType(getType(n->val.vec.typ));
    List *items = n->val.vec.val;
    V x = allocNode();
    L size = totalElement(items);
    initV(x, t, size);
    loadVector(items, x, t);
    addParam(paramList, x);
}


static O assignVars(List *vars, VList *list){
    if(vars){
        assignVars(vars->next, list->next);
        Node *n = vars->val;
        n->val.param.sn->value = list->v;
        //printV(list->v);
    }
}

static O runAssignStmt(Node *n){
    Node *expr = n->val.assignStmt.expr;
    List *vars = n->val.assignStmt.vars;
    runNode(expr);
    I numVars = totalVar(vars);
    I numRtns = totalVList(rtnList);
    if(numVars == numRtns){
        assignVars(vars, rtnList->next);
    }
    else EP("%d expects ,but only %d returned\n", numVars, numRtns);
}

static O runName(Node *n){
    SymbolName *sn = getSymbolNameFromName(n);
    addParam(paramList, sn->value);
}

static B checkInfoNodeWithValue(InfoNode *in, V v){
    // check main type and cell types with return types
    if(!in->subInfo && !in->next){
    }
    else TODO("Need to check cell types\n");
}

static InfoNode *getInfoNodeFromV(V x){
    InfoNode *in = NEW(InfoNode);
    Type t;
    switch(xp){
        caseB t = boolT; break;
        caseJ t =   i8T; break;
        caseH t =  i16T; break;
        caseI t =  i32T; break;
        caseL t =  i64T; break;
        caseF t =  f32T; break;
        caseE t =  f64T; break;
        default: TODO("Add more types\n");
    }
    in->type = t;
    return in;
}

static B checkReturnTypeSub(Node *n, V v){
    InfoNode *in = n->val.type.in;
    InfoNode *nv = getInfoNodeFromV(v);
    B rtn =  checkType(in, nv);
    freeInfoNode(nv);
    return rtn;
}

static O checkReturnType(List *nlist, VList *plist){
    if(nlist){
        checkReturnType(nlist->next, plist->next);
        if(!checkReturnTypeSub(nlist->val, plist->v))
            EP("Return type is not expected\n");
    }
}

static O runReturn(Node *n){
    cleanVList(paramList);
    runList(n->val.listS);
    checkReturnType(currentMethod->val.method.typ, paramList->next);
}

static O runNode(Node *n){
    switch(n->kind){
        case     stmtK: runAssignStmt(n); break;
        case   returnK: runReturn(n);     break;
        case    blockK: runBlock(n);      break;
        case     callK: runCall(n);       break;
        case   vectorK: runVector(n);     break;
        case  argExprK: runArgExpr(n);    break;
        case     nameK: runName(n);       break;
        case    breakK: TODO("break");    break;
        case continueK: TODO("continue"); break;
        case       ifK: TODO("if");       break;
        case    whileK: TODO("while");    break;
        case   repeatK: TODO("repeat");   break;
        default: EP("Kind not supported yet: %s\n", getNodeTypeStr(n));
    }
}


static void runList(List *list){
    if(list){
        runList(list->next);
        if(list->val) runNode(list->val);
    }
}

static O runMethod(Node *method, VList *param){
    P("method name %s\n", method->val.method.fname);
    Node *prevNode = currentMethod;
    if(prevNode != NULL)
        saveToStack(prevNode);
    if(param != NULL)
        loadParams(method, param);
    currentMethod = method;
    List *rtnTypes = method->val.method.typ;
    runNode(method->val.method.block);
    if(rtnTypes){
        // output - if it has returns
        if(method == entryMain){
            P("Output: \n");
            printVList(paramList->next);
        }
        else if(prevNode != NULL){
            loadFromStack(prevNode);
        }
    }
}

static O init(){
    paramList = NEW(VList);
    rtnList = NEW(VList);
    runStack = (G)malloc(STACK_SIZE);
    stackPtr = 0;
}

/* entry */
I HorseInterpreter(Prog *rt){
    printBanner("Interpretation");
    init();
    runMethod(entryMain, NULL);
    //printSymInfo();
    //printHeapInfo();
    R 0;
}


