#include "../global.h"

// The two numbers below must be the same as intp.c's
#define MonFuncSize 60
#define DyaFuncSize 33
#define SpeFuncSize 14

extern I qid; // ../main.c
extern Chain *exitChain;
extern FuseNode FuseList[99];
extern I        FuseTotal;
extern PeepholeNode PhList[99];
extern I            PhTotal;
static I lineNo = 0;
static I depth = 0;
static I totalDefs = 0;

char *monFnName[] = {
    "pfnAbs", "pfnNeg", "pfnCeil", "pfnFloor", "pfnRound", "pfnConj", "pfnRecip", "pfnSignum", "pfnPi"  , "pfnNot" ,
    "pfnLog", "pfnLog2", "pfnLog10", "pfnExp", "pfnTrigCos", "pfnTrigSin", "pfnTrigTan", "pfnTrigAcos", "pfnTrigAsin", "pfnTrigAtan", "pfnHyperCosh", "pfnHyperSinh",
    "pfnHyperTanh", "pfnHyperAcosh", "pfnHyperAsinh", "pfnHyperAtanh",
    "pfnDate", "pfnDateYear", "pfnDateMonth", "pfnDateDay",
    "pfnTime", "pfnTimeHour", "pfnTimeMinute", "pfnTimeSecond", "pfnTimeMill",
    "pfnUnique", NULL, "pfnLen", "pfnRange", "pfnFact", NULL, NULL, "pfnFlip", "pfnReverse",
    "pfnWhere", "pfnGroup", NULL, "pfnSum", "pfnAvg", "pfnMin", "pfnMax", "pfnRaze", "pfnEnlist", "pfnToList",
    NULL, "pfnKeys", "pfnValues", "pfnMeta", "pfnLoadTable", "pfnFetch"
};

char *dyaFnName[] = {
    "pfnLt", "pfnGt", "pfnLeq", "pfnGeq", "pfnEq", "pfnNeq", "pfnPlus", "pfnMinus", "pfnMul", "pfnDiv",
    "pfnPower", "pfnLog2", "pfnMod", "pfnAnd", "pfnOr", "pfnNand", "pfnNor", "pfnXor",
    NULL, 
    "pfnAppend", "pfnLike", "pfnCompress", NULL, "pfnIndexOf", NULL, NULL, "pfnOrderBy",
    "pfnMember", "pfnVector", "pfnMatch", "pfnIndex", "pfnColumnValue", "pfnSubString"
};

char *specialFnName[] = {
    "pfnEach", "pfnEachItem", "pfnEachLeft", "pfnEachRight", "pfnEnum", "pfnDict", "pfnTable",
    "pfnKTable", "pfnIndexA", "pfnList", "pfnOuter", "pfnJoinIndex", "pfnDatetimeAdd", "pfnDatetimeSub"
};

#define outF stdout
//FILE* outF;
#define genIndent() DOI(depth*4, FP(outF, " "))
#define genStmt(n, stmt) {genIndent(); FP(outF,"PROFILE(%3d, %-3s, ",lineNo++,n); stmt; FP(outF,");\n");}
#define genOther(stmt) {genIndent(); stmt;}

static void genMon(S f, S n, S *p){
    genStmt(n, FP(outF, "%s(%s, %s)",f,n,p[0]))
}
static void genDya(S f, S n, S *p){
    genStmt(n, FP(outF, "%s(%s, %s, %s)",f,n,p[0],p[1]))
}

static void genAny(S f, S n, S *p, L tot){
    genStmt(n, {FP(outF, "%s(%s, %lld, (V []){",f,n,tot); DOI(tot, {if(i>0)FP(outF," ,");FP(outF, "%s", p[i]);}) FP(outF, "})");})
}

static void genIndexA(S n, S *p){
    genStmt(n, FP(outF, "pfnIndexA(%s, %s, %s, %s)",p[0],p[1],p[2],p[3]))
}

static void fetchFuncName(S s, S func){
    S s0=NULL, s1=NULL;
    while(s[0]!=0 && s[0]!='"') s++;
    if(s[0]=='"') {
        s0 = ++s;
        while(s[0]!=0 && s[0]!='"') s++;
        if(s[0]=='"') s1=s;
    }
    if(s0==NULL || s1==NULL) func[0]=0;
    else { L n=s1-s0; strncpy(func,s0,n); func[n]=0; }
}

void getRealFuncName(S func, S x){
    I k = getFuncIndexByName(func);
    //P("input: %s", func);
    if(k>=0){
        if(k < MonFuncSize) strcpy(func, monFnName[k]);
        else if(k < MonFuncSize + DyaFuncSize) strcpy(func, dyaFnName[k-MonFuncSize]);
        else if(k < MonFuncSize + DyaFuncSize + SpeFuncSize) strcpy(func, specialFnName[k-MonFuncSize-DyaFuncSize]);
        else EP("func is not found with outbounded index: %d\n", k);
    }
    else EP("func not found: %s\n",func);
    //P(", output: %s", func); getchar();
    // if(!strcmp(x, "index"))      strcpy(func, "pfnIndex");
    // else if(!strcmp(x, "mul"))   strcpy(func, "pfnMul");
    // else if(!strcmp(x, "minus")) strcpy(func, "pfnMinus");
    // else if(!strcmp(x, "plus"))  strcpy(func, "pfnPlus");
    // else if(!strcmp(x, "sum"))   strcpy(func, "pfnSum");
    // else if(!strcmp(x, "avg"))   strcpy(func, "pfnAvg");
    // else if(!strcmp(x, "len"))   strcpy(func, "pfnLen");
    // else if(!strcmp(x, "unique"))strcpy(func, "pfnUnique");
    // else if(!strcmp(x, "eq"))    strcpy(func, "pfnEq");
    // else if(!strcmp(x, "min"))   strcpy(func, "pfnMin");
    // else { EP("Func. not supported: %s\n", x); }
}

static void genStmtArg3(S f, S n, S *p){
    char func[99];
    fetchFuncName(p[0], func);
    getRealFuncName(func, func);
    genStmt(n, FP(outF, "%s(%s,%s,%s,%s)",f,n,p[1],p[2],func))
}

static void genStmtArg2(S f,S n, S *p){
    char func[99];
    fetchFuncName(p[0], func);
    getRealFuncName(func, func);
    genStmt(n, FP(outF, "%s(%s,%s,%s)",f,n,p[1],func))
}

static void genJoinIndex(S f, S n, S *p){
    genStmt(n, FP(outF, "%s(%s,%s,%s,%s)",f,n,p[1],p[2],p[0]))
}

#define genEachMon genStmtArg2
#define genEachDya genStmtArg3

static void genAssignment(S n, S p){
    genStmt(n, FP(outF, "copyV(%s, %s)",n,p))
}

static char *stringifyLiteral(V x){
    char t[999];
    if(xn == 1){
        switch(xp){
            caseB SP(t,"initLiteralBool(%d)", xb); break;
            caseL SP(t,"initLiteralI64(%lld)", xl); break;
            caseS SP(t,"initLiteralString((S)\"%s\")", xs); break;
            caseQ SP(t,"initLiteralSym((S)\"%s\")",getSymbolStr(xq)); break;
            caseD {I d=xd;SP(t,"initLiteralDate(%d%02d%02d)", d/10000,d%10000/100,d%100);} break;
            caseF SP(t,"initLiteralF32(%g)", xf); break;
            caseE SP(t,"initLiteralF64(%g)", xe); break;
            caseC SP(t,"initLiteralChar('%c')", xc); break;
            default: EP("Adding more types: %s\n",getpTypeName(xp));
        }
    }
    else {
        S str = t;
        switch(xp){
            caseB str+=SP(str,"initLiteralBoolVector(%lld, (B []){",xn);
                  DOI(xn, {if(i>0){str[0]=',';str++;} str+=SP(str,"%d",xB(i));})
                  str+=SP(str, "})"); break;
            caseQ str+=SP(str,"initLiteralSymVector(%lld, (S []){",xn);
                  DOI(xn, {if(i>0){str[0]=',';str++;} str+=SP(str,"\"%s\"",getSymbolStr(xQ(i)));})
                  str+=SP(str, "})"); break;
            caseL str+=SP(str,"initLiteralI64Vector(%lld, (L []){",xn);
                  DOI(xn, {if(i>0){str[0]=',';str++;} str+=SP(str,"%lld",xL(i));})
                  str+=SP(str, "})"); break;
            caseS str+=SP(str,"initLiteralStrVector(%lld, (S []){",xn);
                  DOI(xn, {if(i>0){str[0]=',';str++;} str+=SP(str,"\"%s\"",xS(i));})
                  str+=SP(str, "})"); break;
            default: EP("Adding more types: %s (%lld)\n",getpTypeName(xp),xn);
        }
    }
    return strdup(t);
}

static S loadParam2V(Node *param){
    Node *n = param->val.nodeS;
    //printNodeKind(n);
    if(instanceOf(n, idK)){
        return fetchName(n);
    }
    else {
        V literal = getLiteralFromNode(param);
        return stringifyLiteral(literal);
    }
}

static S *getParamsFromExpr(Node *expr, I *nParam){
    Node *param = expr->val.expr.param;
    List *ptr = param->val.listS;
    I tot = 0;
    while(ptr){ tot++; ptr = ptr->next; }
    char **vals = NEW2(char, tot); //
    ptr = param->val.listS;
    tot=0; while(ptr){ vals[tot]=loadParam2V(ptr->val); ptr = ptr->next; tot++; }
    *nParam = tot;
    //P("tot = %d\n", tot);
    //DOI(tot, P("string [%lld] %s\n",i,vals[i]))
    return vals;
}

static void processStmtCommon(S writeName, Node *expr){
    I np = 0;
    S funcName = fetchName(expr->val.expr.func);
    //P("funcName: %s\n", funcName);
    S *params  = getParamsFromExpr(expr, &np);
    pFunc fIndex = getFuncIndexByName(funcName);
    I valence = getValence(fIndex);
    //P("valence = %d, np = %d\n", valence, np);
    if(np == valence || valence == -1){ //-1: any
        if(fIndex < eachF) {
            if(valence == 1){
                //P("monadic %d\n", fIndex);
                return genMon(monFnName[fIndex], writeName, params);
            }
            else if(valence == 2){
                //P("dyadic %d\n", fIndex-ltF);
                return genDya(dyaFnName[fIndex-ltF], writeName, params);
            }
            else EP("valence == %d not expected\n", valence);
        }
        else {
            //if(fIndex == joinIndexF){
            //    P("three: %s, %s, %s\n", params[0],params[1],params[2]);
            //    getchar();
            //}
            switch(fIndex){
                /* monadic */
                case        eachF: return genEachMon("pfnEach"   , writeName, params);
                /* dyadic */
                case        enumF: return genDya("pfnEnum"       , writeName, params);
                case       tableF: return genDya("pfnTable"      , writeName, params);
                case   eachRightF: return genEachDya("pfnEachRight" , writeName, params);
                case    eachLeftF: return genEachDya("pfnEachLeft"  , writeName, params);
                case    eachItemF: return genEachDya("pfnEachItem"  , writeName, params);
                case      indexAF: return genIndexA(writeName, params);
                /* anyadic */
                case        listF: return genAny("pfnList"       , writeName, params, np);
                case   joinIndexF: return genJoinIndex("pfnJoinIndex", writeName, params);
                default: EP("pending ... for %s\n", getpFuncName(fIndex));
            }
        }
    }
    else EP("valence error: %d expected, but %d found\n", valence, np);
}

static void compileSimpleStmt(Node *stmt){
    S writeName = fetchName(stmt->val.simpleStmt.name);
    Node *expr = stmt->val.simpleStmt.expr;
    if(expr->val.expr.func){
        processStmtCommon(writeName, expr);
        //saveToNameTable(writeName, processStmtCommon(expr));
    }
    else { // assignment
        Node *param = expr->val.expr.param;
        genAssignment(writeName, loadParam2V(param));
    }
}

static void compileCastStmt(Node *stmt){
    S writeName = fetchName(stmt->val.castStmt.name);
    Node *expr = stmt->val.castStmt.expr;
    processStmtCommon(writeName, expr);
}

static void compileReturnStmt(Node *stmt){
    Node *param = stmt->val.nodeS;
    S rtn = loadParam2V(param);
    genOther(FP(outF, "toc;\n"));
    genOther(FP(outF, "printV2(%s,20);\n",rtn));
    genOther(FP(outF, "R elapsed;\n"));
}

static I compileSingleStmt(Node *stmt){
    switch(stmt->kind){
        case simpleStmtK: compileSimpleStmt(stmt); break;
        case   castStmtK: compileCastStmt  (stmt); break;
        case     returnK: compileReturnStmt(stmt); break;
        default: printNodeKind(stmt);
                 EP("[runSingleStmt: kind %d not supported yet.\n", stmt->kind);
    }
    return 0;
}

static void genMethodHead(){ FP(outF, "E compiledQ%d(){\n",qid); }
static void genMethodTail(){ FP(outF, "}\n"); }

/* optional */
static void compileMethod(Node *method){
    List *stmts = method->val.method.list;
    depth++; lineNo = 0; 
    genMethodHead();
    genOther(FP(outF, "E elapsed=0;\n"));
    genOther(FP(outF, "tic;\n"));
    while(stmts){
        P("[%3d]-> ",stmts->val->lineno); prettyNode(stmts->val);
        compileSingleStmt(stmts->val);
        if(instanceOf(stmts->val, returnK)) break;
        stmts = stmts->next;
    }
    genMethodTail();
    depth--;
}

static I findTargInList(OptNode *list, S s, I n){
    DOI(n, if(!strcmp(s, list[i].targ)) return i) R -1;
}

static void collectName(S name){
    if(totalDefs==0) genIndent();
    P("V %-4s = allocNode(); ", name);
    totalDefs++;
    if(totalDefs == 4){ totalDefs = 0; FP(outF,"\n"); }
}

static I collectSingleDef(Node *stmt){
    switch(stmt->kind){
        case simpleStmtK: collectName(fetchName(stmt->val.simpleStmt.name)); break;
        case   castStmtK: collectName(fetchName(stmt->val.castStmt.name)); break;
    }
    return 0;
}

static void collectDefs(ChainList *list){
    totalDefs = 0;
    ChainList *p = list;
    while(p->next){
        p = p->next;
        Node *stmt = p->chain->cur;
        collectSingleDef(stmt);
        if(instanceOf(stmt, returnK)) break;
    }
    if(totalDefs!=0) FP(outF,"\n");
}

static bool identifyStmt(Node *stmt, OptNode *OptList, I OptTotal){
    S writeName = NULL;
    if(instanceOf(stmt, simpleStmtK))
        writeName = fetchName(stmt->val.simpleStmt.name);
    else if(instanceOf(stmt, castStmtK))
        writeName = fetchName(stmt->val.castStmt.name);
    else return false;
    //P("writeName = %s\n", writeName);
    //if(!strcmp(writeName, "t107")){
    //    int xxxxx = 0;
    //}
    L x = findTargInList(OptList,writeName,OptTotal);
    if(x>=0){
        genStmt(writeName, FP(outF, "%s", OptList[x].invc));
        return true;
    }
    return false;
}

static void identifyOptimization(Node *stmt){
    if(!identifyStmt(stmt, FuseList, FuseTotal))
        identifyStmt(stmt, PhList  , PhTotal);
}

static void compileChain(ChainList *list){
    ChainList *p = list;
    depth++; lineNo = 0; 
    genMethodHead();
    genOther(FP(outF, "E elapsed=0;\n"));
    collectDefs(list);
    genOther(FP(outF, "tic;\n"));
    while(p->next){
        p = p->next;
        Node *stmt = p->chain->cur;
        //prettyNode(stmt);
        //P("(isvisited: %d)\n", p->chain->isVisited);
        if(p->chain->isVisited){
            identifyOptimization(stmt);
        }
        else compileSingleStmt(stmt);
        if(instanceOf(stmt, returnK)) break;
    }
    genMethodTail();
    depth--;
}

FILE *writeFile(S s){
    FILE *fp = fopen(s, "w");
    if(NULL == fp){
        FP(stderr, "%s\n", strerror(errno));
        exit(1);
    } 
    return fp;
}

I HorseCompiler(ChainList *rt){
    //char outPath[128]; SP(outPath, "./compile/q%d.out", qid);
    //outF = writeFile(outPath);
    //Node *method = findMethod(findModule(rt->module_list, "default")->val.module.body, "main");
    //compileMethod(method);
    //printChainList();
    compileChain(rt);
    if(H_DEBUG) printChainList(); // display visited chains
    R 0;
}
