
// where 1: return 5003.6858
V simulateQ22_sub2(){
    P("Enteirng sub2\n");
    V a0 = allocNode();
    V t0 = allocNode();  V t1 = allocNode();
    V w0 = allocNode();  V w1 = allocNode();  V w2 = allocNode();  V w3 = allocNode();
    V w4 = allocNode();  V w5 = allocNode();
    PROFILE( 1, pfnLoadTable(a0, literalSym((S)"customer")));
    PROFILE( 2, pfnColumnValue(t0, a0, literalSym((S)"c_acctbal")));
    PROFILE( 3, pfnColumnValue(t1, a0, literalSym((S)"c_phone")));
    PROFILE( 4, pfnGt(w0, t0, literalF64(0)));
    L group1[] = {1, 2};
    PROFILE( 5, pfnSubString(w1, t1, literalI64Vector(2, group1)));
    S group2[] = {(S)"13", (S)"31", (S)"23", (S)"29", (S)"30", (S)"18", (S)"17"};
    PROFILE( 6, pfnMember(w2, literalStrVector(7, group2), w1));
    PROFILE( 7, pfnAnd(w3, w0, w2));
    PROFILE( 8, pfnCompress(w4, w3, t0));
    // P("len of w4: %lld\n", vn(w4));
    PROFILE( 9, pfnAvg(w5, w4));
    // printV(w5);
    P("Leaving sub2\n");
    R w5;
}

// return custsale
V simulateQ22_sub1(){
    P("Enteirng sub1\n");
    V a0 = allocNode();  V a1 = allocNode();
    V t0 = allocNode();  V t1 = allocNode();  V t2 = allocNode();
    V w0 = allocNode();  V w1 = allocNode();  V w2 = allocNode();  V w3 = allocNode();
    V w4 = allocNode();  V w5 = allocNode();
    V m0 = allocNode();  V m1 = allocNode();
    V z0 = allocNode();  V z1 = allocNode();  V z  = allocNode();
    PROFILE( 1, pfnLoadTable(a0, literalSym((S)"customer")));
    PROFILE( 2, pfnLoadTable(a1, literalSym((S)"orders")));
    PROFILE( 3, pfnColumnValue(t0, a0, literalSym((S)"c_acctbal")));
    PROFILE( 3, pfnColumnValue(t1, a0, literalSym((S)"c_phone")));

    L group1[] = {1, 2};
    PROFILE( 4, pfnSubString(w0, t1, literalI64Vector(2, group1)));
    // P("type of t1: %lld\n", vp(t1)); DOI(10, P("[%lld] %s\n",i, vS(t1,i))) P("\n");
    // P("type of w0: %lld\n", vp(w0)); DOI(10, P("[%lld] %s\n",i, vS(w0,i))) P("\n");
    S group2[] = {(S)"13", (S)"31", (S)"23", (S)"29", (S)"30", (S)"18", (S)"17"};
    PROFILE( 5, pfnMember(w1, literalStrVector(7, group2), w0));
    V sub2 = simulateQ22_sub2();
    if(!isOptimized){
        PROFILE( 6, pfnGt(w2, t0, sub2));
        PROFILE( 7, pfnAnd(w3, w1, w2)); // boolean
    }
    else {
        PROFILE( 6, optLoopFusionQ22_1(w3, vn(w1), w1, t0, sub2));
    }

    PROFILE( 8, pfnColumnValue(t2, a1, literalSym((S)"o_custkey")));
    PROFILE( 9, pfnValues     (w4, t2));
    PROFILE(10, pfnIndexA     (w3, w4, literalBool(0)));

    PROFILE(11, pfnCompress(m0, w3, w0));
    PROFILE(12, pfnCompress(m1, w3, t0));

    S group3[] = {(S)"cntrycode", (S)"c_acctbal"};
    PROFILE(13, copyV(z0, literalSymVector(2, group3)));
    V group4[] = {m0, m1};
    PROFILE(14, pfnList(z1, 2, group4));
    PROFILE(15, pfnTable(z, z0, z1));
    // printV(z);  // too many records
    P("Leaving sub1\n");
    R z;
}

E simulateQ22(){
    V t0 = allocNode();  V t1 = allocNode();
    V w0 = allocNode();  V w1 = allocNode();  V w2 = allocNode();  V w3 = allocNode();
    V w4 = allocNode();  V w5 = allocNode();
    V m0 = allocNode();  V m1 = allocNode();  V m2 = allocNode();
    V z0 = allocNode();  V z1 = allocNode();  V z  = allocNode();
    
    struct timeval tv0, tv1;
    gettimeofday(&tv0, NULL);

    V sub1 = simulateQ22_sub1(); //custsale
    PROFILE( 1, pfnColumnValue(t0, sub1, literalSym((S)"cntrycode")));
    PROFILE( 2, pfnColumnValue(t1, sub1, literalSym((S)"c_acctbal")));
    PROFILE( 3, pfnGroup(w0, t0));
    PROFILE( 4, pfnValues(w1, w0));
    PROFILE( 5, pfnKeys  (w2, w0));
    PROFILE( 6, pfnEach(w3, w1, pfnLen));
    PROFILE( 7, pfnEachRight(w4, t1, w1, pfnIndex));
    PROFILE( 8, pfnEach(w5, w4, pfnSum));

    PROFILE( 9, pfnIndex(m0, t0, w2)); // cntrycode
    PROFILE(10, pfnRaze(m1, w3));      // numcust
    PROFILE(11, pfnRaze(m2, w5));      // totacctbal

    S group1[] = {(S)"cntrycode", (S)"numcust", (S)"totacctbal"};
    PROFILE(13, copyV(z0, literalSymVector(3, group1)));
    V group2[] = {m0, m1, m2};
    PROFILE(14, pfnList(z1, 3, group2));
    PROFILE(15, pfnTable(z, z0, z1));

    gettimeofday(&tv1, NULL);
    E elapsed = calcInterval(tv0,tv1)/1000.0;
    P("The elapsed time (ms): %g\n\n", elapsed);
    printV(z);
    R elapsed;
}

L testTPCHQ22(){
    P("** Start simulation for TPC-H Query 22\n");
    initTableByName((S)"customer");
    initTableByName((S)"orders");
    PROFILE(91, pfnAddFKey(literalSym((S)"customer"), literalSym((S)"c_custkey"),\
                           literalSym((S)"orders"),   literalSym((S)"o_custkey")));
    L cur = getHeapOffset();
    DOI(TEST_RUNS, {setHeapOffset(cur); times[i]=simulateQ22();})
    P("** End Query 22\n");
    R 0;
}