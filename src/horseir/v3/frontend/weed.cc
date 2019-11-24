#include "../global.h"

/* declarations */

#define weedParamExpr weedListNode
#define weedExpr      weedListNode
#define weedReturn    weedListNode
#define weedVarDecl   weedListNode

static void weedNode    (Node *x);
static bool weedDate    (int x);
static bool weedMonth   (int x);
static bool weedMinute  (int x);
static bool weedSecond  (int x);
static bool weedTime    (int x);
static bool weedDT      (long long x);
static bool weedClex    (float *x);
static bool weedString  (char *x);
static void weedList    (List *x);
static void weedListNode(Node *x);

static int cntMain = 0;
extern Node *entryMain;

/* code blocks */

/*
 * bool: 0 or 1
 *   i8: [-128, 127]
 *  i16: [-2^15, 2^15-1]
 *  i32: [-2^31, 2^31-1]
 *  i64: [-2^63, 2^63-1]
 */

#define checkDay(x, n) ((x)>=1 && (x)<=(n))
#define isNotLeap(yy)  (yy%4!=0 || yy%400!=0)

#define checkBool(x)   (0==x || 1==x)
#define checkSmall(x)  (x>=-128 || x<=127);
#define checkShort(x)  (x>=-32768 || x<=32767)
#define checkInt(x)    (x>=INT_MIN || x<=INT_MAX);
#define checkFloat(x)  (x>=FLT_MIN || x<=FLT_MAX)
#define checkDouble(x) (x>=DBL_MIN || x<=DBL_MAX)

static bool weedInt(int x, HorseType t){
    switch(t){
        case   boolT: R checkBool(x);
        case     i8T: R checkSmall(x);
        case    i16T: R checkShort(x);
        case    i32T: R 1;
        case    f32T: R checkFloat(x);
        case    f64T: R checkDouble(x);
        default: R 0;
    }
}

static bool weedLong(long long x, HorseType t){
    switch(t){
        case boolT: R checkBool(x);
        case   i8T: R checkSmall(x);
        case  i16T: R checkShort(x);
        case  i32T: R checkInt(x);
        case  i64T: R 1;
        case  f32T: R checkFloat(x);
        case  f64T: R checkDouble(x);
        default: R 0;
    }
}

static bool weedFloat(double x, HorseType t){
    switch(t){
        case  f32T: R (x>=FLT_MIN || x<=FLT_MAX);
        case  f64T: R 1;
        default: R 0;
    }
}

static bool weedConst(Node *x, HorseType t){
    ConstValue *p = x->val.nodeC;
    switch(p->type){
        case    intC: R weedInt  (p->valI, t);
        case  floatC: R weedFloat(p->valF, t);
        case   longC: R weedLong (p->valL, t);
        case   clexC: R   clexT == t && weedClex  (p->valX);
        case   charC: R   charT == t && weedString(p->valS);
        case    symC: R    symT == t && weedString(p->valS);
        case    strC: R    strT == t && weedString(p->valS);
        case   dateC: R   dateT == t && weedDate  (p->valI);
        case  monthC: R  monthT == t && weedMonth (p->valI);
        case     dtC: R     dtT == t && weedDT    (p->valL);
        case minuteC: R minuteT == t && weedMinute(p->valI);
        case secondC: R secondT == t && weedSecond(p->valI);
        case   timeC: R   timeT == t && weedTime  (p->valI);
        default: EP("Constant type not supported: %d", p->type);
    }
}

#define weedYY(x) (x>=1000 && x<=9999)
#define weedMM(x) (x>=1 && x<=12)
#define weedHH(x) (x>=0 && x<24)
#define weedM2(x) (x>=0 && x<60)
#define weedSS(x) (x>=0 && x<60)
#define weedLL(x) (x>=0 && x<1000)

static bool weedDate(int x){
    int yy=x/10000,mm=x/100%100,dd=x%100;
    if(weedYY(yy) && weedMM(mm)){
        switch(mm){
            case  1: R checkDay(dd, 31); break; //
            case  2: R checkDay(dd, isNotLeap(yy)?28:29); break;
            case  3: R checkDay(dd, 31); break; //
            case  4: R checkDay(dd, 30); break;
            case  5: R checkDay(dd, 31); break; //
            case  6: R checkDay(dd, 30); break;
            case  7: R checkDay(dd, 31); break; //
            case  8: R checkDay(dd, 31); break; //
            case  9: R checkDay(dd, 30); break;
            case 10: R checkDay(dd, 31); break; //
            case 11: R checkDay(dd, 30); break;
            case 12: R checkDay(dd, 31); break; //
            default: R 0;
        }
    }
    else R 0;
}

static bool weedMonth(int x){
    int yy=x/100,mm=x%100;
    R weedYY(yy) && weedMM(mm);
}

static bool weedMinute(int x){
    int hh=x/60, mm=x%60;
    R weedHH(hh) && weedM2(mm);
}

static bool weedSecond(int x){
    int ss=x%60;
    R weedMinute(x/60) && weedSS(ss);
}

static bool weedTime(int x){
    int ll=x%1000;
    R weedSecond(x/1000) && weedLL(ll);
}

static bool weedDT(long long x){
    R weedDate(x/86400000) && weedTime(x%86400000);
}

static bool weedClex(float *x){
    float rel = x[0], img = x[1];
    R checkFloat(rel) && checkFloat(img);
}

static bool weedString(char *x){
    while(*x){
        char c = *x; if(c < 0) R 0; x++;
    }
    R 1;
}

static void printConst(Node *x){
    //printNodeType(x);
    ConstValue *p = x->val.nodeC;
    switch(p->type){
        case   intC: P("int: %d\n"   , p->valI); break;
        case floatC: P("float: %g\n" , p->valF); break;
        case  longC: P("long: %lld\n", p->valL); break;
        default: EP("Type not supported: %d", p->type);
    }
}

static HorseType getVectorType(Node *x){
    HorseType t = getType(x);
    switch(t){
        case   boolT: case     i8T: case   i16T: case  i32T: case  i64T:
        case    f32T: case    f64T: case  clexT: case charT: case  symT:
        case    strT: case   dateT: case monthT: case timeT: case   dtT:
        case minuteT: case secondT: return t;
        default: EP("Invalid type for constants: %s", x->val.type.typ);
    }
}

// boolT =?= H_B ...
static const char* getTypeStr2(HorseType t){
    switch(t){
        CASE_STR(boolT);
        CASE_STR(i8T);
        CASE_STR(i16T);
        CASE_STR(i32T);
        CASE_STR(i64T);
        CASE_STR(f32T);
        CASE_STR(f64T);
        CASE_STR(clexT);
        CASE_STR(charT);
        CASE_STR(symT);
        CASE_STR(strT);
        CASE_STR(dateT);
        CASE_STR(monthT);
        CASE_STR(minuteT);
        CASE_STR(secondT);
        CASE_STR(timeT);
        CASE_STR(dtT);
        default: TODO("Add more type: %d\n",t);
    }
}

static const char* getConstTypeStr(Node *x){
    ConstValue *p = x->val.nodeC;
    switch(p->type){
        CASE_STR(intC);
        CASE_STR(floatC);
        CASE_STR(longC);
        CASE_STR(clexC);
        CASE_STR(charC);
        CASE_STR(strC);
        CASE_STR(symC);
        CASE_STR(dateC);
        CASE_STR(monthC);
        CASE_STR(minuteC);
        CASE_STR(secondC);
        CASE_STR(timeC);
        CASE_STR(dtC);
        default: TODO("Add more type: %d\n",p->type);
    }
}

/*
 * a type indicator is a must
 */
static void weedVector(Node *x){
    //printNodeType(x);
    List *p = x->val.vec.val;
    HorseType t = getVectorType(x->val.vec.typ);
    while(p){
        if(!weedConst(p->val, t)){
            printNode(x);
            EP("Literal type or range error: (expect %s, find %s)", \
                    getTypeStr2(t),getConstTypeStr(p->val));
        }
        p = p->next;
    }
}

static void weedVar(Node *x){
    char *name = x->val.param.id;
    if(sEQ(name, uscore))
        EP("Underscore '%s' shouldn't have any type.", uscore);
}

static void weedStmt(Node *stmt){
    weedList(stmt->val.assignStmt.vars);
    weedNode(stmt->val.assignStmt.expr);
}

static void weedIf(Node *x){
    weedNode(x->val.ifStmt.condExpr);
    weedNode(x->val.ifStmt.thenBlock);
    weedNode(x->val.ifStmt.elseBlock);
}

static void weedWhile(Node *x){
    weedNode(x->val.whileStmt.condExpr);
    weedNode(x->val.whileStmt.bodyBlock);
}

static void weedRepeat(Node *x){
    weedNode(x->val.repeatStmt.condExpr);
    weedNode(x->val.repeatStmt.bodyBlock);
}

static void weedBlock(Node *x){
    weedList(x->val.block.stmts);
}

static void weedImport(Node *x){
    //printNodeType(x);
}

static void weedGlobal(Node *x){
    //printNodeType(x);
}

static void weedMethod(Node *x){
    //printNodeType(x);
    if(sEQ(x->val.method.fname, "main")){
        if(cntMain == 0){ entryMain = x; cntMain++; }
        else EP("Only one main method expected in modules.");
    }
    weedNode(x->val.method.block);
}

static void weedModule(Node *module){
    //printNodeType(module);
    weedList(module->val.module.body);
}

static void weedCall(Node *x){
    //printNodeType(x);
    Node *param = x->val.call.param;
    if(param) weedNode(param);
}

static void weedCast(Node *x){
    //printNodeType(x);
    weedNode(x->val.cast.exp);
}

static void weedList(List *x){
    if(x){
        weedList(x->next);
        weedNode(x->val);
    }
}

static void weedListNode(Node *x){
    weedList(x->val.listS);
}

static void weedName(Node *x){
    if(x->val.name.one) ;
    else {
        char *id1 = x->val.name.id1;
        char *id2 = x->val.name.id2;
        if(sEQ(id2, uscore) || sEQ(id1, uscore))
            EP("Underscore '%s' can't be used in a global name reference.", uscore);
    }
}

static void weedExprStmt(Node *x){
    weedNode(x->val.exprStmt.expr);
}

static void weedNode(Node *x){
    if(!x) R ;
    switch(x->kind){
        case    moduleK: weedModule(x);    break;
        case    importK: weedImport(x);    break;
        case    methodK: weedMethod(x);    break;
        case    globalK: weedGlobal(x);    break;
        case     blockK: weedBlock(x);     break;
        case       varK: weedVar(x);       break;
        case      callK: weedCall(x);      break;
        case      nameK: weedName(x);      break;
        case      funcK: break;
        case      castK: weedCast(x);      break;
        case    vectorK: weedVector(x);    break;
        case     constK: printConst(x);    break;
        case   argExprK: weedExpr(x);      break; /* list */
        case paramExprK: weedParamExpr(x); break; /* list */
        case      stmtK: weedStmt(x);      break;
        case        ifK: weedIf(x);        break;
        case     whileK: weedWhile(x);     break;
        case    repeatK: weedRepeat(x);    break;
        case    returnK: weedReturn(x);    break;
        case     breakK: break;
        case  continueK: break;
        case   varDeclK: weedVarDecl(x);   break;
        case  exprstmtK: weedExprStmt(x);  break;
        default: EP("Type unknown: %s", getNodeTypeStr(x));
    }
}

static void weedMainMethod(){
    if(cntMain == 1); // fine
    else if(cntMain == 0) EP("Main method not found");
    else EP("Only one main method expected, but %d found", cntMain);
}

static void init(){
    cntMain = 0;
    entryMain = NULL;
}

void weedProg(Prog *root){
    printBanner("Program Weeder");
    init();
    weedList(root->module_list);
    weedMainMethod();
}
