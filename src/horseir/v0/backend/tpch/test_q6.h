
E simulateQ6(){
    L e;
    V t0 = allocNode();  V t5 = allocNode();  V t10 = allocNode();  V t15 = allocNode();
    V t1 = allocNode();  V t6 = allocNode();  V t11 = allocNode();  V t16 = allocNode();
    V t2 = allocNode();  V t7 = allocNode();  V t12 = allocNode();  V t17 = allocNode();
    V t3 = allocNode();  V t8 = allocNode();  V t13 = allocNode();  V t18 = allocNode();
    V t4 = allocNode();  V t9 = allocNode();  V t14 = allocNode();  V t19 = allocNode();

    V t20 = allocNode(); V t21 = allocNode(); V t22 = allocNode();  V a0 = allocNode();
    struct timeval tv0, tv1;
    gettimeofday(&tv0, NULL);

    PROFILE(1,pfnLoadTable(a0, \
        initSymbol(allocNode(),getSymbol((S)"lineitem"))));
    PROFILE(2,pfnColumnValue(t0, a0, \
        initSymbol(allocNode(),getSymbol((S)"l_extendedprice"))));
    PROFILE(3,pfnColumnValue(t1, a0, \
        initSymbol(allocNode(),getSymbol((S)"l_discount"))));
    PROFILE(4,pfnColumnValue(t2, a0, \
        initSymbol(allocNode(),getSymbol((S)"l_shipdate"))));
    PROFILE(5,pfnColumnValue(t3, a0, \
        initSymbol(allocNode(),getSymbol((S)"l_quantity"))));

    if(!isOptimized){
        PROFILE(6,pfnGeq(t4, t2, literalDate(19940101)));
        PROFILE(7,pfnDatetimeAdd(t5,literalDate(19940101),literalI64(1),literalSym((S)"year")));
        PROFILE(8,pfnLt(t6,t2,t5));
        PROFILE(9,pfnMinus(t7,literalF64(0.06),literalF64(0.01)));
        PROFILE(10,pfnPlus(t8,literalF64(0.06),literalF64(0.01)));

        PROFILE(11,pfnGeq(t9,t1,t7));
        PROFILE(12,pfnLeq(t10,t1,t8));
        PROFILE(13,pfnAnd(t11,t9,t10));
        // PROFILE(13,pfnBetween(t11,t1,t7,t8));
        PROFILE(14,pfnLt(t12,t3,literalI64(24)));

        PROFILE(15,pfnAnd(t13,t4,t6));
        PROFILE(16,pfnAnd(t14,t13,t11));
        PROFILE(17,pfnAnd(t15,t14,t12));
    }
    else {
        PROFILE(91, optLoopFusionQ6_1(t15,tableRow(a0),t1,t2,t3));
    }

    if(!isOptimized){
        PROFILE(18,pfnCompress(t16,t15,t0));
        PROFILE(19,pfnCompress(t17,t15,t1));
    }
    else {
        PROFILE(18,optLoopFusionQ6_3(t16,t17,t15,t0,t1));
    }
    PROFILE(20,pfnMul(t18,t16,t17));
    // P("before %lld, after %lld\n", vn(t0),vn(t16));
    // before 6001215, after 114160  -->  selectivity 1.9%
    // PROFILE(92, optLoopFusionQ6_2(t18,tableRow(a0),t15,t0,t1));
    PROFILE(21,pfnSum(t19,t18));

    PROFILE(22,copyV(t20,literalSym((S)"revenue")));
    PROFILE(23,pfnEnlist(t21,t19));
    PROFILE(24,pfnTable(t22,t20,t21));

    gettimeofday(&tv1, NULL);

    E elapsed = calcInterval(tv0,tv1)/1000.0;
    P("The elapsed time (ms): %g\n\n", elapsed);
    printV(t22);
    R elapsed;
}

L testTPCHQ6(){
    P("** Start simulation for TPC-H Query 6\n");
    initTableByName((S)"lineitem");
    L cur = getHeapOffset();
    DOI(TEST_RUNS, {setHeapOffset(cur); times[i]=simulateQ6();})
    P("** End Query 6\n");
    R 0;
}

