#include "global.h"

B isReadBin  = false;
extern B qIsTpch;
extern I qTpchId;

static void initElementwiseFuncMap();
static void initTablesAll();

/* helper functions */

void *newM(I n){
    void *x = malloc(n);
    memset(x, 0, n);
    if(!x){
        error("Memory allocation fails");
    }
    return x;
}

void error(const char *s){
    fprintf(stderr,"%s\n", s);
    exit(1);
}

void initBackend(){
    initMemory();    // h_memory.c
    initSym();       // h_symbol.c
    initSys();       // h_system.c
}

void initGlobal(){
    initElementwiseFuncMap();
    switch(optMode){
        case InterpNaiveM:
        case    CompilerM: initBackend(); break;
        default: break;
    }
    if(optMode == InterpNaiveM && qIsTpch)
        initTablesByQid(qTpchId);
}

void initStats(){
    initElementwiseFuncMap();
    initBackend();
    initTablesAll();
}

static void initTablesAll(){
    initTableByName((S)"region");
    initTableByName((S)"nation");
    initTableByName((S)"customer");
    initTableByName((S)"orders");
    initTableByName((S)"lineitem");
    initTableByName((S)"part");
    initTableByName((S)"supplier");
    initTableByName((S)"partsupp");
}

#ifndef my_tic

#define TIMEVAL_N 10
static I timeSlotId = 0;
struct timeval tv0a[TIMEVAL_N], tv0b[TIMEVAL_N];

static void time_check(){
    if(timeSlotId >= TIMEVAL_N || timeSlotId < 0){
        EP("No sufficient lots for timeval: %d >= %d or a negative num", \
                timeSlotId, TIMEVAL_N);
    }
}

static L calcIntervalPrecise(struct timeval t0, struct timeval t1){
    return (t1.tv_sec-t0.tv_sec)*1000000 + t1.tv_usec-t0.tv_usec;
}

// return: ms (Linux/MacOS)
static E calcInterval(struct timeval t0, struct timeval t1){
    return calcIntervalPrecise(t0, t1) / 1000.0;
}

void my_tic(){
    time_check();
    gettimeofday(&tv0a[timeSlotId++], NULL);
}

E my_toc(B isPrint){
    timeSlotId--; time_check(); 
    gettimeofday(&tv0b[timeSlotId], NULL);
    E elapsed = calcInterval(tv0a[timeSlotId], tv0b[timeSlotId]);
    if(isPrint)
        P("[%d] The elapsed time (ms): %g\n\n", timeSlotId, elapsed);
    return elapsed;
}

void time_clear(){
    timeSlotId = 0;
}

#endif

/* ---------------- Elementwise  -------------------- */
const TypeUnary GroupElementwiseU[] = {
    /* unary 32 */
    absF, negF, ceilF, floorF, roundF, piF, notF,
    logF, log2F, log10F, expF, cosF, sinF, tanF, acosF, asinF,
    atanF, coshF, sinhF, tanhF, acoshF, asinhF, atanhF,
    dateF, yearF, monthF, dayF,
    timeF, hourF, minuteF, secondF, millF
};

const TypeBinary GroupElementwiseB[] = {
    /* binary 17 */
    ltF, gtF, leqF, geqF, eqF, neqF, plusF, minusF, mulF, divF,
    powerF, modF, andF, orF, nandF, norF, xorF
};

B *ElementwiseUnaryMap;
B *ElementwiseBinaryMap;

// TODO: need to free two maps?
static void initElementwiseFuncMap(){
    ElementwiseUnaryMap  = NEWL(B, totalU);
    ElementwiseBinaryMap = NEWL(B, totalB);
    DOI(sizeof(GroupElementwiseU)/sizeof(TypeUnary), ElementwiseUnaryMap[GroupElementwiseU[i]]=true)
    DOI(sizeof(GroupElementwiseB)/sizeof(TypeBinary), ElementwiseBinaryMap[GroupElementwiseB[i]]=true)
}

/* ---------------- TPC-H related -------------------- */

#define initQ1  initQ6 
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
    addFKey("nation"  , "n_nationkey", "supplier", "s_nationkey");
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
    addFKey("nation"  , "n_nationkey", "supplier", "s_nationkey");
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
    addFKey("nation"  , "n_nationkey", "supplier", "s_nationkey");
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
    initTableByName((S)"part");  // extra
    addFKey("supplier", "s_suppkey"   , "partsupp" , "ps_suppkey" );
    addFKey("part"    , "p_partkey"   , "partsupp" , "ps_partkey" );
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

static void initQ20(){
    initTableByName((S)"supplier");
    initTableByName((S)"nation");
    initTableByName((S)"partsupp");
    initTableByName((S)"lineitem");
    initTableByName((S)"part");
    addFKey("nation"  , "n_nationkey", "supplier", "s_nationkey");
    addFKey("part"    , "p_partkey"  , "partsupp", "ps_partkey");
    addFKey("supplier", "s_suppkey"  , "partsupp", "ps_suppkey");
    //addFKey2("partsupp", ((S []){"ps_partkey", "ps_suppkey"}),
    //         "lineitem", ((S []){"l_partkey" , "l_suppkey" }), 2);
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
    tic();
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
            case  9: initQ9 (); break;
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
            case 20: initQ20(); break;
            case 21: initQ21(); break;
            case 22: initQ22(); break;
            default: EP("Add more tpch queries: %d",id);
        }
    }
    else if(id==99){ // init all tables
        // EP("loading all tables\n");
        initTablesAll();
        addFKey("part"    , "p_partkey"  , "partsupp", "ps_partkey" );
        addFKey("supplier", "s_suppkey"  , "partsupp", "ps_suppkey" );
        addFKey("region"  , "r_regionkey", "nation"  , "n_regionkey");
        addFKey("customer", "c_custkey"  , "orders"  , "o_custkey"  );
        addFKey("orders"  , "o_orderkey" , "lineitem", "l_orderkey" );
        addFKey("nation"  , "n_nationkey", "supplier", "s_nationkey");
        addFKey("nation"  , "n_nationkey", "customer", "c_nationkey");
    }
    else EP("qid must be [1,22] or 99 (all)");
    time_toc(">>> Loading data (ms): %g ms\n", elapsed);
}


