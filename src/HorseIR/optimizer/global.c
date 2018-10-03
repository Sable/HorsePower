#include "global.h"

/* helper functions */

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

static L calcIntervalPrecise(struct timeval t0, struct timeval t1){
    return (t1.tv_sec-t0.tv_sec)*1000000 + t1.tv_usec-t0.tv_usec;
}

E calcInterval(struct timeval t0, struct timeval t1){
    return calcIntervalPrecise(t0, t1) / 1000.0;
}

/* initialization */

/*
 * if FunctionType in typerule.h is changed,
 *   the list below may be updated as well
 */
const pFunc ElementWiseFunc[] = {
    /* unary 32 */
    absF, negF, ceilF, floorF, roundF, piF, notF,
    logF, log2F, log10F, expF, cosF, sinF, tanF, acosF, asinF,
    atanF, coshF, sinhF, tanhF, acoshF, asinhF, atanhF,
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
#define initQ12 initQ4
#define initQ17 initQ14
#define addFKey(t0,c0,t1,c1) \
    pfnAddFKey(initLiteralSym((S)t0), initLiteralSym((S)c0),\
               initLiteralSym((S)t1), initLiteralSym((S)c1))
#define addFKey2(t0,c0,t1,c1,n) \
    pfnAddFKey(initLiteralSym((S)t0), initLiteralSymVector(n, c0),\
               initLiteralSym((S)t1), initLiteralSymVector(n, c1))
// addFKey(key,fkey)

static void initQ2(){
    initTableByName((S)"part");
    initTableByName((S)"supplier");
    initTableByName((S)"partsupp");
    initTableByName((S)"nation");
    initTableByName((S)"region");
    addFKey("part"    , "p_partkey"  , "partsupp", "ps_partkey" );
    addFKey("supplier", "s_suppkey"  , "partsupp", "ps_suppkey" );
    addFKey("nation"  , "n_nationkey", "supplier", "s_nationkey");
    addFKey("region"  , "r_regionkey", "nation"  , "n_regionkey");
}


static void initQ3(){
    initTableByName((S)"customer");
    initTableByName((S)"orders");
    initTableByName((S)"lineitem");
    addFKey("customer", "c_custkey" , "orders"  , "o_custkey" );
    addFKey("orders"  , "o_orderkey", "lineitem", "l_orderkey");
}

static void initQ5(){
    initTableByName((S)"customer");
    initTableByName((S)"orders");
    initTableByName((S)"lineitem");
    initTableByName((S)"supplier");
    initTableByName((S)"nation");
    initTableByName((S)"region");
    addFKey("region"  , "r_regionkey", "nation"  , "n_regionkey");
    addFKey("nation"  , "n_nationkey", "customer", "c_nationkey");
    addFKey("customer", "c_custkey"  , "orders"  , "o_custkey"  );
    addFKey("orders"  , "o_orderkey" , "lineitem", "l_orderkey" );
}

static void initQ7(){
    initTableByName((S)"supplier");
    initTableByName((S)"lineitem");
    initTableByName((S)"orders");
    initTableByName((S)"customer");
    initTableByName((S)"nation");
    addFKey("nation"  , "n_nationkey", "customer", "c_nationkey");
    addFKey("customer", "c_custkey"  , "orders"  , "o_custkey"  );
    addFKey("orders"  , "o_orderkey" , "lineitem", "l_orderkey" );
}

static void initQ8(){
    initTableByName((S)"part");
    initTableByName((S)"supplier");
    initTableByName((S)"lineitem");
    initTableByName((S)"orders");
    initTableByName((S)"customer");
    initTableByName((S)"nation");
    initTableByName((S)"region");
    addFKey("region"  , "r_regionkey", "nation"  , "n_regionkey");
    addFKey("nation"  , "n_nationkey", "customer", "c_nationkey");
    addFKey("customer", "c_custkey"  , "orders"  , "o_custkey"  );
    addFKey("orders"  , "o_orderkey" , "lineitem", "l_orderkey" );
}

static void initQ9(){
    initTableByName((S)"part");
    initTableByName((S)"supplier");
    initTableByName((S)"lineitem");
    initTableByName((S)"partsupp");
    initTableByName((S)"orders");
    initTableByName((S)"nation");
    addFKey("nation"  , "n_nationkey", "supplier", "s_nationkey");
    addFKey("part"    , "p_partkey"  , "partsupp", "ps_partkey");
    addFKey("supplier", "s_suppkey"  , "partsupp", "ps_suppkey");
    addFKey("orders"  , "o_orderkey" , "lineitem", "l_orderkey" );
    //addFKey2("partsupp", ((S []){"ps_partkey", "ps_suppkey"}),
    //         "lineitem", ((S []){"l_partkey" , "l_suppkey" }), 2);
}

static void initQ10(){
    initTableByName((S)"customer");
    initTableByName((S)"orders");
    initTableByName((S)"lineitem");
    initTableByName((S)"nation");
    addFKey("nation"  , "n_nationkey", "customer", "c_nationkey");
    addFKey("customer", "c_custkey"  , "orders"  , "o_custkey"  );
    addFKey("orders"  , "o_orderkey" , "lineitem", "l_orderkey" );
}

static void initQ11(){
    initTableByName((S)"partsupp");
    initTableByName((S)"supplier");
    initTableByName((S)"nation");
    addFKey("supplier", "s_suppkey"   , "partsupp" , "ps_suppkey" );
    addFKey("nation"  , "n_nationkey" , "supplier" , "s_nationkey");
}

static void initQ6(){
    initTableByName((S)"lineitem");
    /* no fkey */
}

static void initQ4(){
    initTableByName((S)"lineitem");
    initTableByName((S)"orders");
    addFKey("orders", "o_orderkey", "lineitem", "l_orderkey");
}

static void initQ13(){
    initTableByName((S)"customer");
    initTableByName((S)"orders");
    addFKey("customer", "c_custkey", "orders", "o_custkey");
}

static void initQ14(){
    initTableByName((S)"lineitem");
    initTableByName((S)"part");
    /* no fkey */
}

static void initQ15(){
    initTableByName((S)"lineitem");
    initTableByName((S)"supplier");
}

static void initQ16(){
    initTableByName((S)"partsupp");
    initTableByName((S)"part");
    initTableByName((S)"supplier");
    addFKey("part"    , "p_partkey", "partsupp", "ps_partkey");
    addFKey("supplier", "s_suppkey", "partsupp", "ps_suppkey");
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

static void initQ21(){
    initTableByName((S)"supplier");
    initTableByName((S)"orders");
    initTableByName((S)"nation");
    initTableByName((S)"lineitem");
    addFKey("nation", "n_nationkey", "supplier", "s_nationkey");
    addFKey("orders", "o_orderkey" , "lineitem", "l_orderkey" );
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
            case  2: initQ2 (); break;
            case  3: initQ3 (); break;
            case  4: initQ4 (); break;
            case  5: initQ5 (); break;
            case  6: initQ6 (); break;
            case  7: initQ7 (); break;
            case  8: initQ8 (); break;
            case  9: initQ9 (); break; /* working */
            case 10: initQ10(); break;
            case 11: initQ11(); break;
            case 12: initQ12(); break;
            case 13: initQ13(); break;
            case 14: initQ14(); break;
            case 15: initQ15(); break;
            case 16: initQ16(); break;
            case 17: initQ17(); break;
            case 18: initQ18(); break;
            case 19: initQ19(); break;
            case 21: initQ21(); break;
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


