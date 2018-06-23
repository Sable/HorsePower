#include "../global.h"

// The two numbers below must be the same as intp.c's
#define MonFuncSize 58
#define DyaFuncSize 35

extern I qid; // ../main.c
extern Chain *exitChain;
extern FuseNode FuseList[99];
extern I        FuseTotal;
extern PeepholeNode PhList[99];
extern I            PhTotal;
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

#define outF stdout
//FILE* outF;
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
    genStmt(n, {FP(outF, "%s(%s, %lld, {",f,n,tot); DOI(tot, {if(i>0)FP(outF," ,");FP(outF, "%s", p[i]);}) FP(outF, "})");})
}

static void compileIndexA(S n, S *p){
    genStmt(n, FP(outF, "pfnIndexA(%s, %s, %s)",p[0],p[1],p[2]))
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

static void compileEachDya(S f, S n, S *p){
    char func[99]; fetchFuncName(p[0], func);
    if(!strcmp(func, "index"))      strcpy(func, "pfnIndex");
    else if(!strcmp(func, "mul"))   strcpy(func, "pfnMul");
    else if(!strcmp(func, "minus")) strcpy(func, "pfnMinus");
    else if(!strcmp(func, "plus"))  strcpy(func, "pfnPlus");
    else EP("[compileEachDya] add more func %s\n", func);
    genStmt(n, FP(outF, "%s(%s,%s,%s,%s)",f,n,p[1],p[2],func))
}

static void compileEachMon(S f,S n, S *p){
    char func[99]; fetchFuncName(p[0], func);
    if(!strcmp(func, "sum"))         strcpy(func, "pfnSum");
    else if(!strcmp(func, "avg"))    strcpy(func, "pfnAvg");
    else if(!strcmp(func, "len"))    strcpy(func, "pfnLen");
    else if(!strcmp(func, "unique")) strcpy(func, "pfnUnique");
    else EP("[compileEachMon] add more func %s\n", func);
    genStmt(n, FP(outF, "%s(%s,%s,%s)",f,n,p[1],func))
}

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
            default: EP("adding more types: %d\n",xp);
        }
    }
    else {
        char *str = t;
        switch(xp){
            caseB str+=SP(str,"initLiteralBoolVector(%lld, (B []){",xn);
                  DOI(xn, {if(i>0){str[0]=',';str++;} str+=SP(str,"%lld",xB(i));})
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
            default: EP("adding more types: %d (%lld)\n",xp,xn);
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

static int findTargInList(OptNode *list, S s, I n){
    DOI(n, if(!strcmp(s, list[i].targ)) return i) R -1;
}

static void identifyStmt(Node *stmt, OptNode *OptList, I OptTotal){
    S writeName = NULL;
    if(instanceOf(stmt, simpleStmtK))
        writeName = fetchName(stmt->val.simpleStmt.name);
    else if(instanceOf(stmt, castStmtK))
        writeName = fetchName(stmt->val.castStmt.name);
    else return ;
    L x = findTargInList(OptList,writeName,OptTotal);
    if(x>=0){
        genStmt(writeName, FP(outF, "%s", OptList[x].invc));
    }
}

static void identifyOptimization(Node *stmt){
    identifyStmt(stmt, FuseList, FuseTotal);
    identifyStmt(stmt, PhList  , PhTotal);
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
    //char outPath[128]; SP(outPath, "./compile/q%d.out", qid);
    //outF = writeFile(outPath);
    //Node *method = findMethod(findModule(rt->module_list, "default")->val.module.body, "main");
    //compileMethod(method);
    //printChainList();
    compileChain(rt);
}
