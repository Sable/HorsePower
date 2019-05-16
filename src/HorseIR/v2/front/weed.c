#include "../global.h"

/* declarations */

#define weedParamExpr weedList
#define weedExpr      weedList
#define weedReturn    weedList

static void weedNode(Node *x);

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

static bool weedDate(int x){
    int yy=x/10000,mm=x/100%100,dd=x%100;
    if(yy>=1000 && yy<=9999 && mm>=1 && mm<=12){
        switch(mm){
            case  1: R checkDay(dd, 31); break;
            case  2: R checkDay(dd, isNotLeap(yy)?28:29); break;
            case  3: R checkDay(dd, 31); break;
            case  4: R checkDay(dd, 30); break;
            case  5: R checkDay(dd, 31); break;
            case  6: R checkDay(dd, 30); break;
            case  7: R checkDay(dd, 31); break;
            case  8: R checkDay(dd, 31); break;
            case  9: R checkDay(dd, 30); break;
            case 10: R checkDay(dd, 31); break;
            case 11: R checkDay(dd, 30); break;
            case 12: R checkDay(dd, 31); break;
        }
    }
    else R 0;
}

static bool weedInt(int x, Type t){
    switch(t){
        case boolT: R (0==x || 1==x);
        case   i8T: R (x>=-128 || x<=127);
        case  i16T: R (x>=-32768 || x<=32767);
        case  i32T: R 1;
        case  f32T: R (x>=FLT_MIN || x<=FLT_MAX);
        case  f64T: R (x>=DBL_MIN || x<=DBL_MAX);
        case dateT: 
        default: R 0;
    }
}

static bool weedLong(long long x, Type t){
    switch(t){
        case boolT: R (0==x || 1==x);
        case   i8T: R (x>=-128 || x<=127);
        case  i16T: R (x>=-32768 || x<=32767);
        case  i32T: R (x>=INT_MIN || x<=INT_MAX);
        case  i64T: R 1;
        case  f32T: R (x>=FLT_MIN || x<=FLT_MAX);
        case  f64T: R (x>=DBL_MIN || x<=DBL_MAX);
        default: R 0;
    }
}

static bool weedFloat(double x, Type t){
    switch(t){
        case  f32T: R (x>=FLT_MIN || x<=FLT_MAX);
        case  f64T: R 1;
        default: R 0;
    }
}

static bool weedConst(Node *x, Type t){
    ConstValue *p = x->val.nodeC;
    switch(p->type){
        case    intC: R weedInt  (p->valI, t);
        case  floatC: R weedFloat(p->valF, t);
        case   longC: R weedLong (p->valL, t);
        case   clexC: R   clexT == t;
        case   charC: R   charT == t;
        case    symC: R    symT == t;
        case    strC: R    strT == t;
        case   dateC: R   dateT == t;
        case  monthC: R  monthT == t;
        case     dtC: R     dtT == t;
        case minuteC: R minuteT == t;
        case secondC: R secondT == t;
        case   timeC: R   timeT == t;
        default: EP("Constant type not supported: %d\n", p->type);
    }
}

static void printConst(Node *x){
    printNodeType(x);
    ConstValue *p = x->val.nodeC;
    switch(p->type){
        case   intC: P("int: %d\n"   , p->valI); break;
        case floatC: P("float: %g\n" , p->valF); break;
        case  longC: P("long: %lld\n", p->valL); break;
        default: EP("Type not supported: %d\n", p->type);
    }
}

static Type getType(Node *x){
    if(!x) R -1;
    if(x->val.type.isWild) R wildT;
    else if(x->val.type.cell) {
        TODO("Compound type\n");
    }
    else {
        char *typ = x->val.type.typ;
        if(!strcmp(typ, "bool")) R boolT;
        else if(!strcmp(typ, "i8"  )) R i8T;
        else if(!strcmp(typ, "i16" )) R i16T;
        else if(!strcmp(typ, "i32" )) R i32T;
        else if(!strcmp(typ, "i64" )) R i64T;
        else if(!strcmp(typ, "f32" )) R f32T;
        else if(!strcmp(typ, "f64" )) R f64T;
        else if(!strcmp(typ, "clex")) R clexT;
        else if(!strcmp(typ, "char")) R charT;
        else if(!strcmp(typ, "sym" )) R symT;
        else if(!strcmp(typ, "str" )) R strT;
        else if(!strcmp(typ, "d"   )) R dateT;
        else if(!strcmp(typ, "m"   )) R monthT;
        else if(!strcmp(typ, "z"   )) R dtT;
        else if(!strcmp(typ, "w"   )) R minuteT;
        else if(!strcmp(typ, "v"   )) R secondT;
        else if(!strcmp(typ, "t"   )) R timeT;
        else EP("Invalid typ for constants: %s\n", typ);
    }
    R 0;
}

#define CaseLine(x) case x: R #x

static char *getTypeStr(Type t){
    switch(t){
        CaseLine(boolT);
        CaseLine(i8T);
        CaseLine(i16T);
        CaseLine(i32T);
        CaseLine(i64T);
        CaseLine(f32T);
        CaseLine(f64T);
        CaseLine(clexT);
        CaseLine(charT);
        CaseLine(symT);
        CaseLine(strT);
        CaseLine(dateT);
        CaseLine(monthT);
        CaseLine(minuteT);
        CaseLine(secondT);
        CaseLine(timeT);
        CaseLine(dtT);
        default: TODO("Add more type: %d\n",t);
    }
}

static char *getConstTypeStr(Node *x){
    ConstValue *p = x->val.nodeC;
    switch(p->type){
        CaseLine(intC);
        CaseLine(floatC);
        CaseLine(longC);
        CaseLine(clexC);
        CaseLine(charC);
        CaseLine(strC);
        CaseLine(symC);
        CaseLine(dateC);
        CaseLine(monthC);
        CaseLine(minuteC);
        CaseLine(secondC);
        CaseLine(timeC);
        CaseLine(dtC);
        default: TODO("Add more type: %d\n",p->type);
    }
}

/*
 * a type indicator is a must
 */
static void weedVector(Node *x){
    printNodeType(x);
    List *p = x->val.vec.val;
    Type  t = getType(x->val.vec.typ);
    while(p){
        if(!weedConst(p->val, t)){
            EP("Literal type expects %s, but %s found\n", \
                    getTypeStr(t),getConstTypeStr(p->val));
        }
        p = p->next;
    }
}

//static void weedOperand(Node *x){
//    printNodeType(x);
//    //switch(x->kind){
//    //    case       idK: break;
//    //    case compoundK: break;
//    //    case     funcK: break;
//    //    case   vectorK: weedVector(x); break;
//    //    default: EP("type unknown: %s\n", printNodeTypeStr(x));
//    //}
//    weedNode(x->val.listS);
//}

static void weedStmt(Node *stmt){
    weedNode(stmt->val.assignStmt.expr);
}

static void weedBlock(Node *block){
    printNodeType(block);
    List *p = block->val.listS;
    while(p){
        Node *x = p->val;
        switch(x->kind){
            case     stmtK: weedStmt(x); break;
            case       ifK: break;
            case    whileK: break;
            case   repeatK: break;
            case   returnK: break;
            case     gotoK: break;
            case    breakK: break;
            case continueK: break;
        }
        p = p->next;
    }
}

static void weedImport(Node *x){
    printNodeType(x);
}

static void weedGlobal(Node *x){
    printNodeType(x);
}

static void weedMethod(Node *x){
    printNodeType(x);
    weedBlock(x->val.method.block);
}

static void weedModule(Node *module){
    printNodeType(module);
    List *p = module->val.module.body;
    while(p){
        Node *x = p->val;
        switch(x->kind){
            case importK: weedImport(x); break;
            case methodK: weedMethod(x); break;
            case globalK: weedGlobal(x); break;
            default: EP("type unknown: %s\n", printNodeTypeStr(x));
        }
        p = p->next;
    }
}

static void weedModuleList(List *modules){
    List *p = modules;
    while(p){
        weedModule(p->val);
        p = p->next;
    }
}

static void weedCall(Node *x){
    printNodeType(x);
    weedNode(x->val.call.param);
}

static void weedCast(Node *x){
    printNodeType(x);
    weedNode(x->val.cast.exp);
}

static void weedList(Node *x){
    printNodeType(x);
    List *p = x->val.listS;
    while(p){
        weedNode(p->val);
        p = p->next;
    }
}

static void weedNode(Node *x){
    switch(x->kind){
        case    importK: weedImport(x);    break;
        case    methodK: weedMethod(x);    break;
        case    globalK: weedGlobal(x);    break;
        case        idK: break;
        case      callK: weedCall(x);      break;
        case  compoundK: break;
        case      funcK: break;
        case      castK: weedCast(x);      break;
        case    vectorK: weedVector(x);    break;
        case     constK: printConst(x);    break;
        case      exprK: weedExpr(x);      break; /* list */
        case paramExprK: weedParamExpr(x); break; /* list */
        default: EP("type unknown: %s\n", printNodeTypeStr(x));
    }
}

void weedProg(Prog *root){
    printBanner("Program Weeder");
    weedModuleList(root->module_list);
}
