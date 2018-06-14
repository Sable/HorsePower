#include "../global.h"

// The two numbers below must be the same as intp.c's
#define MonFuncSize 58
#define DyaFuncSize 35

extern I qid; // ../main.c
extern Chain *exitChain;
extern FuseNode FuseList[99];
extern I        FuseTotal;
static I lineNo = 0;
static I depth = 0;

char *monFnName[] = {
    "pfnAbs", "pfnNeg", "pfnCeil", "pfnFloor", "pfnRound", "pfnConj", "pfnRecip", "pfnSignum", "pfnPi"  , "pfnNot" ,
    "pfnLog", "pfnExp", "pfnTrigCos", "pfnTrigSin", "pfnTrigTan", "pfnTrigAcos", "pfnTrigAsin", "pfnTrigAtan", "pfnHyperCosh", "pfnHyperSinh",
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
    NULL, NULL, NULL,
    "pfnAppend", "pfnLike", "pfnCompress", NULL, "pfnIndexOf", NULL, NULL, "pfnOrderBy",
    "pfnMember", "pfnVector", "pfnMatch", "pfnIndex", "pfnColumnValue", "pfnSubString"
};

//#define outF stdout
FILE* outF;
#define genIndent() DOI(depth*4, FP(outF, " "))
#define genStmt(n, stmt) {genIndent(); FP(outF,"PROFILE(%3d, %-3s, ",lineNo++,n); stmt; FP(outF,");\n");}
#define genOther(stmt) {genIndent(); stmt;}

static void compileMon(S f, S n, S *p){
    genStmt(n, FP(outF, "%s(%s, %s)",f,n,p[0]))
}
static void compileDya(S f, S n, S *p){
    genStmt(n, FP(outF, "%s(%s, %s, %s)",f,n,p[0],p[1]))
}

static void compileAny(S f, S n, S *p, L tot){
    genStmt(n, {FP(outF, "%s(%s, %s",f,n); DOI(tot, FP(outF, ", %s", p[i])) FP(outF, ")");})
}

static void compileIndexA(S n, S *p){
    genStmt(n, FP(outF, "pfnIndexA(%s, %s, %s)",p[0],p[1],p[2]))
}

static void compileEachDya(S f, S n, S *p){
    genStmt(n, FP(outF, "%s(%s,%s,%s,@%s)",f,n,p[1],p[2],p[0]))
}

static void compileEachMon(S f,S n, S *p){
    genStmt(n, FP(outF, "%s(%s,%s,@%s)",f,n,p[1],p[0]))
}

static void genAssignment(S n, S p){
    genStmt(n, FP(outF, "copyV(%s, %s)",n,p))
}

static char *stringifyLiteral(V x){
    char t[99];
    if(xn == 1){
        switch(xp){
            caseL SP(t,"literalI64(\"%lld\")", xl); break;
            caseQ SP(t,"literalSym((S)\"%s\")",getSymbolStr(xq)); break;
            caseD {I d=xd;SP(t,"%d.%02d.%02d:d", d/10000,d%10000/100,d%100);} break;
            caseF SP(t,"literalF32(%g:f32)", xf); break;
            caseE SP(t,"literalF64(%g:f64)", xe); break;
            default: EP("adding more types: %d\n",xp);
        }
        return strdup(t);
    }
    else EP("adding xn > 1\n");
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

static S *getParamsFromExpr(Node *expr, int *nParam){
    Node *param = expr->val.expr.param;
    List *ptr = param->val.listS;
    int tot = 0;
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
    int np = 0;
    S funcName = fetchName(expr->val.expr.func);
    //P("funcName: %s\n", funcName);
    S *params  = getParamsFromExpr(expr, &np);
    pFunc fIndex = getFuncIndexByName(funcName);
    int valence = getValence(fIndex);
    //P("valence = %d, np = %d\n", valence, np);
    if(np == valence || valence == -1){ //-1: any
        if(fIndex < eachF) {
            if(valence == 1){
                //P("monadic %d\n", fIndex);
                return compileMon(monFnName[fIndex], writeName, params);
            }
            else if(valence == 2){
                //P("dyadic %d\n", fIndex-ltF);
                return compileDya(dyaFnName[fIndex-ltF], writeName, params);
            }
            else EP("valence == %d not expected\n", valence);
        }
        else {
            switch(fIndex){
                /* monadic */
                case        eachF: return compileEachMon("pfnEach"   , writeName, params); break;
                /* dyadic */
                case        enumF: return compileDya("pfnEnum"       , writeName, params); break;
                case       tableF: return compileDya("pfnTable"      , writeName, params); break;
                case   eachRightF: return compileEachDya("pfnEachRight" , writeName, params); break;
                case    eachLeftF: return compileEachDya("pfnEachLeft"  , writeName, params); break;
                case    eachItemF: return compileEachDya("pfnEachItem"  , writeName, params); break;
                case      indexAF: return compileIndexA(writeName, params); break;
                /* anyadic */
                case        listF: return compileAny("pfnList"       , writeName, params, np); break;
                default: EP("pending ... for %d\n", fIndex);
            }
        }
    }
    else EP("valence error: %d expected, but %d found\n", valence, np);
}

static void compileSimpleStmt(Node *stmt){
    char *writeName = fetchName(stmt->val.simpleStmt.name);
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
    char *writeName = fetchName(stmt->val.simpleStmt.name);
    Node *expr = stmt->val.castStmt.expr;
    processStmtCommon(writeName, expr);
}

static void compileReturnStmt(Node *stmt){
    Node *param = stmt->val.nodeS;
    S rtn = loadParam2V(param);
    genOther(FP(outF, "toc;\n"));
    genOther(FP(outF, "printV(%s);\n",rtn));
    genOther(FP(outF, "R elapsed;\n"));
}

static int compileSingleStmt(Node *stmt){
    switch(stmt->kind){
        case simpleStmtK: compileSimpleStmt(stmt); break;
        case   castStmtK: compileCastStmt  (stmt); break;
        case     returnK: compileReturnStmt(stmt); break;
        default: printNodeKind(stmt);
                 EP("[runSingleStmt: kind %d not supported yet.\n", stmt->kind);
    }
    return 0;
}

static void genMethodHead(){ FP(outF, "E simulateQ%d(){\n",qid); }
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

static int findTargInFuseList(S n){
    DOI(FuseTotal, if(!strcmp(n, FuseList[i].targ)) return i) R -1;
}

static void identifyLoopFusion(Node *stmt){
    S writeName = NULL;
    if(instanceOf(stmt, simpleStmtK))
        writeName = fetchName(stmt->val.simpleStmt.name);
    else if(instanceOf(stmt, castStmtK))
        writeName = fetchName(stmt->val.castStmt.name);
    else return ;
    L x = findTargInFuseList(writeName);
    if(x>=0){
        genStmt(writeName, FP(outF, "%s", FuseList[x].invc));
    }
}

static void identifyOptimization(Node *stmt){
    identifyLoopFusion(stmt);
}

static void compileChain(ChainList *list){
    ChainList *p = list;
    depth++; lineNo = 0; 
    genMethodHead();
    genOther(FP(outF, "E elapsed=0;\n"));
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

int HorseCompiler(ChainList *rt){
    char outPath[128]; SP(outPath, "./compile/q%d.out", qid);
    outF = writeFile(outPath);
    //Node *method = findMethod(findModule(rt->module_list, "default")->val.module.body, "main");
    //compileMethod(method);
    //printChainList();
    compileChain(rt);
}