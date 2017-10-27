

V udfExists() {
    L e = 0;
    V a0 = allocNode(); V a1 = allocNode();
    V t0 = allocNode(); V t1 = allocNode(); V t2 = allocNode(); V t3 = allocNode();
    V j0 = allocNode();
    V w0 = allocNode(); V w1 = allocNode(); V w2 = allocNode(); V w3 = allocNode();
    V w4 = allocNode(); V w5 = allocNode();
    FS("Entering: udfExists\n");
    PROFILE( 1,pfnLoadTable(a0, literalSym((S)"orders")));
    PROFILE( 2,pfnLoadTable(a1, literalSym((S)"lineitem")));

    PROFILE( 3,pfnColumnValue(t0, a0, literalSym((S)"o_orderkey")));
    PROFILE( 4,pfnColumnValue(t1, a1, literalSym((S)"l_orderkey")));    //fkey
    PROFILE( 5,pfnColumnValue(t2, a1, literalSym((S)"l_commitdate")));
    PROFILE( 6,pfnColumnValue(t3, a1, literalSym((S)"l_receiptdate")));

    // PROFILE( 7,pfnEnum(j0, t0, t1));     // <--- slow
    PROFILE( 8,pfnLt      (w0, t2, t3));
    PROFILE( 9,pfnCompress(w1, w0, t1));
    PROFILE(10,pfnValues  (w2, w1));
    PROFILE(11,pfnKeys    (w3, w1));
    PROFILE(12,pfnLen     (w4, w3));
    PROFILE(13,pfnVector  (w5, w4, literalBool(0)));
    PROFILE(14,pfnIndexA  (w5, w2, literalBool(1)));
    FS("Leaving: udfExists\n");
    R w5;
}

E simulateQ4(){
    L e = 0;
    V a0 = allocNode();
    V t0 = allocNode(); V t1 = allocNode();
    V w0 = allocNode(); V w1 = allocNode(); V w2 = allocNode(); V w3 = allocNode();
    V w4 = allocNode(); V w5 = allocNode();
    V g0 = allocNode(); V g1 = allocNode();
    V s0 = allocNode(); V s1 = allocNode(); V s2 = allocNode(); V s3 = allocNode();
    V s4 = allocNode();
    V r0 = allocNode();
    V m0 = allocNode(); V m1 = allocNode(); V m2 = allocNode(); V m3 = allocNode();
    V z  = allocNode();
    struct timeval tv0, tv1;
    gettimeofday(&tv0, NULL);

    PROFILE( 1,pfnLoadTable(a0, literalSym((S)"orders")));
    PROFILE( 2,pfnColumnValue(t0, a0, literalSym((S)"o_orderdate")));
    PROFILE( 3,pfnColumnValue(t1, a0, literalSym((S)"o_orderpriority")));

    // step 1: where clause
    PROFILE( 4,pfnGeq(w0, t0, literalDate(19930701)));
    PROFILE( 5,pfnDatetimeAdd(w1, literalDate(19930701), literalI64(3), literalSym((S)"month")));
    PROFILE( 6,pfnLt(w2, t0, w1));
    PROFILE( 7,pfnAnd(w3, w0, w2));

    PROFILE( 8,(w4=udfExists(),0));
    PROFILE( 9,pfnAnd(w5,w3,w4));

    // step 2: group by
    PROFILE(10,pfnCompress(g0, w5, t1));
    PROFILE(11,pfnGroup(g1, g0));

    // step 3: select
    PROFILE(12,pfnKeys  (s0, g1));
    PROFILE(13,pfnValues(s1, g1));
    PROFILE(14,pfnEach  (s2, s1, pfnLen));
    PROFILE(15,pfnRaze  (s3, s2));
    // PROFILE(16,pfnRaze  (s4, s0));
    PROFILE(16,pfnIndex (s4, g0, s0));

    // step 4: order by
    PROFILE(17,pfnOrderBy(r0, s4, literalBool(1)));

    // step 5: materialization
    S literStr[] = {(S)"o_orderpriority", (S)"order_count"};
    V liter2 = literalSymVector(2, literStr);
    PROFILE(18, copyV(m0,liter2));
    // PROFILE(19,pfnToList(m1, m0));
    PROFILE(20,pfnIndex(m1, s4, r0));
    PROFILE(21,pfnIndex(m2, s3, r0));
    V rn[] = {m1,m2};
    PROFILE(22,pfnList(m3, 2, rn));

    // finally, return the table
    PROFILE(23,pfnTable(z, m0, m3));

    gettimeofday(&tv1, NULL);
    E elapsed = calcInterval(tv0,tv1)/1000.0;
    P("The elapsed time (ms): %g\n\n", elapsed);
    printV(z);
    R elapsed;
}

L testTPCHQ4(){
    P("** Start simulation for TPC-H Query 4\n");
    initTableByName((S)"orders");
    initTableByName((S)"lineitem");
    PROFILE(91, pfnAddFKey(literalSym((S)"orders"),   literalSym((S)"o_orderkey"),\
                           literalSym((S)"lineitem"), literalSym((S)"l_orderkey")));
    L cur = getHeapOffset();
    DOI(TEST_RUNS, {setHeapOffset(cur); times[i]=simulateQ4();})
    P("** End Query 4\n");
    R 0;
}
