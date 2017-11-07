

E simulateQ14(){
    V a0 = allocNode();  V a1 = allocNode();
    V t0 = allocNode();  V t1 = allocNode();  V t2 = allocNode();  V t3 = allocNode();
    V t4 = allocNode();  V t5 = allocNode();
    V w0 = allocNode();  V w1 = allocNode();  V w2 = allocNode();  V w3 = allocNode();
    V w4 = allocNode();  V w5 = allocNode();  V w6 = allocNode();  V w7 = allocNode();
    V p0 = allocNode();  V p1 = allocNode();  V p2 = allocNode();  V p3 = allocNode();
    V p4 = allocNode();  V p5 = allocNode();  V p6 = allocNode();  V p7 = allocNode();
    V p8 = allocNode();  V p9 = allocNode();  V p10= allocNode();  V p11= allocNode();
    V p12= allocNode();  V p13= allocNode();
    V z0 = allocNode();  V z1 = allocNode();  V z  = allocNode();
    struct timeval tv0, tv1;
    gettimeofday(&tv0, NULL);

    PROFILE(1, pfnLoadTable(a0, literalSym((S)"lineitem")));
    PROFILE(2, pfnLoadTable(a1, literalSym((S)"part")));

    // select 1
    PROFILE(3, pfnColumnValue(t0, a0, literalSym((S)"l_shipdate")));
    if(!isOptimized){
        PROFILE(4, pfnGeq(w0, t0, literalDate(19950901)));
        PROFILE(5, pfnDatetimeAdd(w1, literalDate(19950901),literalI64(1),literalSym((S)"month")));
        PROFILE(6, pfnLt(w2, t0, w1));
        PROFILE(7, pfnAnd(w3, w0, w2));
    }
    else {
        PROFILE(99, optLoopFusionQ14_1(w3, vn(t0), t0));
    }

    if(!isOptimized){
        // join 1
        PROFILE(8, pfnColumnValue(t1, a0, literalSym((S)"l_partkey")));
        PROFILE(9, pfnColumnValue(t2, a1, literalSym((S)"p_partkey")));
        PROFILE(10, pfnCompress(w4, w3, t1));
        // P("size of w4: %lld\n", vn(w4));// 76K
        PROFILE(11, pfnEnum(w5, t2, w4));  // optimize?

        // project 1
        PROFILE(12, pfnColumnValue(t3, a1, literalSym((S)"p_type")));
        PROFILE(13, pfnColumnValue(t4, a0, literalSym((S)"l_extendedprice")));
        PROFILE(14, pfnColumnValue(t5, a0, literalSym((S)"l_discount")));
        PROFILE(15, pfnValues(p0, w5));
        PROFILE(16, pfnIndex(p1, t3, p0));     // p_type
        PROFILE(17, pfnCompress(p2, w3, t4));  // l_extendedprice
        PROFILE(18, pfnCompress(p3, w3, t5));  // l_discount
        // P("before: %lld, after: %lld\n", vn(t4), vn(p2));
    }
    else {
        PROFILE( 8, pfnColumnValue(t1, a0, literalSym((S)"l_partkey")));
        PROFILE( 9, pfnColumnValue(t2, a1, literalSym((S)"p_partkey")));
        PROFILE(10, pfnColumnValue(t3, a1, literalSym((S)"p_type")));
        PROFILE(11, pfnColumnValue(t4, a0, literalSym((S)"l_extendedprice")));
        PROFILE(12, pfnColumnValue(t5, a0, literalSym((S)"l_discount")));
        PROFILE(13, optLoopFusionQ14_3(w4, p2, p3, w3, t1, t4, t5));
        PROFILE(14, pfnEnum(w5, t2, w4));
        PROFILE(15, pfnValues(p0, w5));
        PROFILE(16, pfnIndex(p1, t3, p0));
    }
    PROFILE(19, pfnLike(p4, p1, literalString((S)"PROMO%")));
    
    PROFILE(20, pfnMinus(p5,literalF64(1), p3));
    if(!isOptimized){
        PROFILE(21, pfnMul(p6, p2, p5));
        PROFILE(22, pfnMul(p7, p4, p6));  // case
    }
    else {
        PROFILE(21, optLoopFusionQ14_2(p7, vn(p4), p2, p4, p5));
    }
    PROFILE(23, pfnSum(p8, p7));      // sum 1
    PROFILE(24, pfnMul(p9, literalF64(100), p8));

    if(!isOptimized){
        PROFILE(24, pfnMinus(p10,literalF64(1), p3));
        PROFILE(25, pfnMul(p11, p2, p10));
    }
    else {
        PROFILE(25, pfnMul(p11, p2, p5));  // p10 -> p5
    }

    PROFILE(26, pfnSum(p12, p11));    // sum 2
    PROFILE(27, pfnDiv(p13, p9, p12));

    // return
    PROFILE(28, copyV(z0, literalSym((S)"promo_revenue")));
    PROFILE(29, pfnEnlist(z1, p13));
    PROFILE(30, pfnTable(z, z0, z1));

    gettimeofday(&tv1, NULL);
    E elapsed = calcInterval(tv0,tv1)/1000.0;
    P("The elapsed time (ms): %g\n\n", elapsed);
    printV(z);
    R elapsed;
}


L testTPCHQ14(){
    P("** Start simulation for TPC-H Query 14\n");
    initTableByName((S)"lineitem");
    initTableByName((S)"part");
    L cur = getHeapOffset();
    DOI(TEST_RUNS, {setHeapOffset(cur); times[i]=simulateQ14();})
    P("** End Query 14\n");
    R 0;
}

