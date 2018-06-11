#include "global.h"

void *newM(int n){
    void *x = malloc(n);
    memset(x, 0, n);
    if(!x){ error("Memory allocation fails"); }
    return x;
}

void error(const char *s){
    fprintf(stderr,"%s\n", s);
    exit(1);
}

/* initialization */

/*
 * if FunctionType in typerule.h is changed,
 *   the list below may be updated as well
 */
const pFunc ElementWiseFunc[] = {
    /* unary 30 */
    absF, negF, ceilF, floorF, roundF, piF, notF,
    logF, expF, cosF, sinF, tanF, acosF, asinF, atanF, coshF, sinhF,
    tanhF, acoshF, asinhF, atanhF,
    dateF, yearF, monthF, dayF,
    timeF, hourF, minuteF, secondF, millF,
    /* binary 17 */
    ltF, gtF, leqF, geqF, eqF, neqF, plusF, minusF, mulF, divF,
    powerF, modF, andF, orF, nandF, norF, xorF
};

bool ElementFuncMap[999]; /* max # of functions, must > totalFunc */

static void initFuncKind(){
    if(totalFunc >= 999) error("Size of FuncMap is not enough");
    memset(ElementFuncMap, 0, sizeof(bool)*999);
    DOI(sizeof(ElementWiseFunc)/sizeof(pFunc), ElementFuncMap[ElementWiseFunc[i]]=1)
}

void initBackend(){
    initNameTable(); // util/nametable.c
    initMemory();    // h_memory.c
    initSym();       // h_symbol.c
    initSys();       // h_system.c
}

#define initQ1 initQ6 
#define addFKey(t0,c0,t1,c1) \
    pfnAddFKey(initLiteralSym((S)t0), initLiteralSym((S)c0),\
               initLiteralSym((S)t1), initLiteralSym((S)c1))

static void initQ6(){
    initTableByName((S)"lineitem");
    /* no fkey */
}

static void initQ4(){
    initTableByName((S)"lineitem");
    initTableByName((S)"orders");
    addFKey("orders", "o_orderkey", "lineitem", "l_orderkey");
}

static void initQ14(){
    initTableByName((S)"lineitem");
    initTableByName((S)"part");
    /* no fkey */
}

static void initQ16(){
    initTableByName((S)"partsupp");
    initTableByName((S)"part");
    initTableByName((S)"supplier");
    addFKey("part", "p_partkey", "partsupp", "ps_partkey");
}

static void initQ18(){
    initTableByName((S)"customer");
    initTableByName((S)"orders");
    initTableByName((S)"lineitem");
    addFKey("customer", "c_custkey", "orders", "o_custkey");
    addFKey("orders", "o_orderkey", "lineitem", "l_orderkey");
}

static void initQ19(){
    initTableByName((S)"lineitem");
    initTableByName((S)"part");
}

static void initQ22(){
    initTableByName((S)"customer");
    initTableByName((S)"orders");
    addFKey("customer", "c_custkey", "orders", "o_custkey");
}

void initTablesByQid(I id){
    if(id>=0 && id<=22){
        switch(id){
            case  1: initQ1 (); break;
            case  4: initQ4 (); break;
            case  6: initQ6 (); break;
            case 14: initQ14(); break;
            case 16: initQ16(); break;
            case 18: initQ18(); break;
            case 19: initQ19(); break;
            case 22: initQ22(); break;
            default: EP("Pending initTablesByQid: %d\n",id);
        }
    }
    else { // init all tables
        EP("loading all tables\n");
        initTableByName((S)"region");
        initTableByName((S)"nation");
        initTableByName((S)"customer");
        initTableByName((S)"orders");
        initTableByName((S)"lineitem");
        initTableByName((S)"part");
        initTableByName((S)"supplier");
        initTableByName((S)"partsupp");
    }
}

/* entry */
void initGlobal(){
    initTrie();
    initUDChain();
    initFuncKind();
    initBackend();
}


