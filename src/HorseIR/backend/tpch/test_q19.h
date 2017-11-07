
E simulateQ19(){
    V a0 = allocNode();  V a1 = allocNode();
    V t0 = allocNode();  V t1 = allocNode();  V t2 = allocNode();  V t3 = allocNode();
    V t4 = allocNode();  V t5 = allocNode();  V t6 = allocNode();  V t7 = allocNode();
    V t8 = allocNode();  V t9 = allocNode();
    V w0 = allocNode();  V w1 = allocNode();  V w2 = allocNode();  V w3 = allocNode();
    V w4 = allocNode();  V w5 = allocNode();  V w6 = allocNode();  V w7 = allocNode();
    V w8 = allocNode();  V w9 = allocNode();  V w10= allocNode();

    V p0 = allocNode();  V p1 = allocNode();  V p2 = allocNode();

    V m0 = allocNode();  V m1 = allocNode();  V m2 = allocNode();  V m3 = allocNode();
    V m4 = allocNode();  V m5 = allocNode();  V m6 = allocNode();

    V z0 = allocNode();  V z1 = allocNode();  V z  = allocNode();

    V d0 = allocNode();  V d1 = allocNode();  V d2 = allocNode();  V d3 = allocNode();
    V d4 = allocNode();  V d5 = allocNode();  V d6 = allocNode();  V d7 = allocNode();
    V d8 = allocNode();  V d9 = allocNode();  V d10= allocNode();  V d11= allocNode();
    V d12= allocNode();  V d13= allocNode();  V d14= allocNode();  V d15= allocNode();
    V d16= allocNode();

    V r0 = allocNode();  V r1 = allocNode();  V r2 = allocNode();  V r3 = allocNode();
    V r4 = allocNode();  V r5 = allocNode();  V r6 = allocNode();  V r7 = allocNode();
    V r8 = allocNode();  V r9 = allocNode();  V r10= allocNode();

    V k0 = allocNode();  V k1 = allocNode();  V k2 = allocNode();  V k3 = allocNode();
    V k4 = allocNode();  V k5 = allocNode();  V k6 = allocNode();  V k7 = allocNode();
    V k8 = allocNode();  V k9 = allocNode();  V k10= allocNode();

    V h0 = allocNode();  V h1 = allocNode();  V h2 = allocNode();  V h3 = allocNode();
    V h4 = allocNode();  V h5 = allocNode();  V h6 = allocNode();  V h7 = allocNode();
    V h8 = allocNode();  V h9 = allocNode();  V h10= allocNode();

    V f0 = allocNode();  V f1 = allocNode();

    struct timeval tv0, tv1;
    gettimeofday(&tv0, NULL);

    PROFILE( 1, pfnLoadTable(a0, literalSym((S)"lineitem")));
    PROFILE( 2, pfnLoadTable(a1, literalSym((S)"part")));

    PROFILE( 3, pfnColumnValue(t0, a1, literalSym((S)"p_partkey")));
    PROFILE( 4, pfnColumnValue(t1, a1, literalSym((S)"p_brand")));
    PROFILE( 5, pfnColumnValue(t2, a1, literalSym((S)"p_container")));
    PROFILE( 6, pfnColumnValue(t3, a1, literalSym((S)"p_size")));

    PROFILE( 7, pfnColumnValue(t4, a0, literalSym((S)"l_partkey")));
    PROFILE( 8, pfnColumnValue(t5, a0, literalSym((S)"l_quantity")));
    PROFILE( 9, pfnColumnValue(t6, a0, literalSym((S)"l_shipmode")));
    PROFILE(10, pfnColumnValue(t7, a0, literalSym((S)"l_shipinstruct")));
    PROFILE(11, pfnColumnValue(t8, a0, literalSym((S)"l_extendedprice")));
    PROFILE(12, pfnColumnValue(t9, a0, literalSym((S)"l_discount")));

    // select 1 - part - w
    S group1[] = {(S)"Brand#12", (S)"Brand#23", (S)"Brand#34"};
    PROFILE(20, pfnMember(w0, literalSymVector(3, group1), t1));
    S group2[] = {(S)"SM CASE", (S)"SM BOX", (S)"SM PACK", (S)"SM PKG", \
                  (S)"MED BAG", (S)"MED BOX", (S)"MED PKG", (S)"MED PACK", \
                  (S)"LG CASE", (S)"LG BOX", (S)"LG PACK", (S)"LG PKG"};
    PROFILE(21, pfnMember(w1, literalSymVector(12, group2), t2));
    if(!isOptimized){
        L group_const1[] = {1, 1, 1};
        L group_const2[] = {5, 10, 15};
        // PROFILE(22, pfnMin(w2, literalI64(1) , literalI64(1)));
        PROFILE(22, pfnMin(w3, literalI64Vector(3, group_const1)));
        // PROFILE(22, pfnMax(w4, literalI64(10), literalI64(15)));
        PROFILE(22, pfnMax(w5, literalI64Vector(3, group_const2)));
        // P("min %lld, max %lld\n", vl(w3), vl(w5));
        PROFILE(23, pfnGeq(w6, t3, w3));
        PROFILE(23, pfnLeq(w7, t3, w5));
        PROFILE(24, pfnAnd(w8, w6, w7));                                   // p_size
        PROFILE(25, pfnAnd(w9, w0, w1));
        PROFILE(26, pfnAnd(w10, w9, w8));
    }
    else {
        PROFILE(22, optLoopFusionQ19_1(w10, vn(t3), t3, w0, w1));
    }

    // select 2 - lineitem - p
    S group3[] = {(S)"AIR", (S)"AIR REG"};
    PROFILE(30, pfnMember(p0, literalSymVector(2, group3), t6));       // l_shipmode
    PROFILE(31, pfnEq(p1, t7, literalSym((S)"DELIVER IN PERSON")));    // l_shipinstruct
    PROFILE(32, pfnAnd(p2, p0, p1));

    // join - d
    // P("type: %lld\n", vp(t1)); DOI(vn(t1), if(vB(w10,i))P(" %s", getSymbolStr(vQ(t1 ,i)))) getchar(); P("\n");
    PROFILE(40, pfnCompress(d0, w10, t0)); // p_
    if(!isOptimized){
        PROFILE(41, pfnCompress(d1, p2, t4)); // l_
        PROFILE(45, pfnWhere(d7, p2));
    }
    else {
        PROFILE(45, optLoopFusionQ19_5(d1, d7, p2, t4));
    }
    PROFILE(42, pfnIndexOf(d2, d0, d1));
    PROFILE(43, pfnLen(d3, d0));
    PROFILE(44, pfnLt(d4, d2, d3));
    PROFILE(46, pfnCompress(d5, d4, d7)); // l_
    PROFILE(47, pfnWhere(f0, w10));
    PROFILE(48, pfnCompress(d6, d4, d2)); // p_
    PROFILE(49, pfnIndex(f1, f0, d6));

    PROFILE(50, pfnIndex(d9 , t1, f1)); // p_brand
    PROFILE(51, pfnIndex(d10, t2, f1)); // p_container
    PROFILE(52, pfnIndex(d11, t3, f1)); // p_size

    PROFILE(53, pfnIndex(d12, t5, d5)); // l_quantity
    PROFILE(54, pfnIndex(d13, t6, d5)); // l_shipmode
    PROFILE(55, pfnIndex(d14, t7, d5)); // l_shipinstruct
    PROFILE(56, pfnIndex(d15, t8, d5)); // l_extendedprice
    PROFILE(57, pfnIndex(d16, t9, d5)); // l_discount

    // P("len of d5: %lld\n", vn(d5)); // 2283

    // where 1 - r
    // PROFILE(60, pfnEq(r0, d9, literalSym((S)"Brand#12")));  pfnSum(m6, r0); printV(m6);
    S group4[] = {(S)"SM CASE", (S)"SM BOX", (S)"SM PACK", (S)"SM PKG"};
    PROFILE(61, pfnMember(r1, literalSymVector(4, group4), d10));
    if(!isOptimized){
        PROFILE(60, pfnEq(r0, d9, literalSym((S)"Brand#12")));
        PROFILE(62, pfnGeq(r2, d12, literalI64(1)));
        PROFILE(63, pfnLeq(r3, d12, literalI64(11)));
        PROFILE(64, pfnGeq(r4, d11, literalI64(1)));
        PROFILE(65, pfnLeq(r5, d11, literalI64(5)));
        PROFILE(66, pfnAnd(r6, r0, r1));
        PROFILE(67, pfnAnd(r7, r2, r3));
        PROFILE(68, pfnAnd(r8, r4, r5));
        PROFILE(69, pfnAnd(r9, r6, r7));
        PROFILE(70, pfnAnd(r10, r9, r8));
    }
    else {
        PROFILE(62, optLoopFusionQ19_2(r10, vn(r1), d9, literalSym((S)"Brand#12"), d11, d12, r1));
    }

    // where 2 - k
    // PROFILE(71, pfnEq(k0, d9, literalSym((S)"Brand#23")));
    S group5[] = {(S)"MED BAG", (S)"MED BOX", (S)"MED PKG", (S)"MED PACK"};
    PROFILE(72, pfnMember(k1, literalSymVector(4, group5), d10));
    if(isOptimized){
        PROFILE(71, pfnEq(k0, d9, literalSym((S)"Brand#23")));
        PROFILE(73, pfnGeq(k2, d12, literalI64(10))); 
        PROFILE(74, pfnLeq(k3, d12, literalI64(20)));
        PROFILE(75, pfnGeq(k4, d11, literalI64(1)));
        PROFILE(76, pfnLeq(k5, d11, literalI64(10)));
        PROFILE(77, pfnAnd(k6, k0, k1));
        PROFILE(78, pfnAnd(k7, k2, k3));
        PROFILE(79, pfnAnd(k8, k4, k5));
        PROFILE(80, pfnAnd(k9, k6, k7));
        PROFILE(81, pfnAnd(k10, k9, k8));
    }
    else {
        PROFILE(73, optLoopFusionQ19_3(k10, vn(k1), d9, literalSym((S)"Brand#23"), d11, d12, k1));
    }

    // where 3 - h
    // PROFILE(82, pfnEq(h0, d9, literalSym((S)"Brand#34")));
    S group6[] = {(S)"LG CASE", (S)"LG BOX", (S)"LG PACK", (S)"LG PKG"};
    PROFILE(83, pfnMember(h1, literalSymVector(4, group6), d10));
    if(isOptimized){
        PROFILE(82, pfnEq(h0, d9, literalSym((S)"Brand#34")));
        PROFILE(84, pfnGeq(h2, d12, literalI64(20))); 
        PROFILE(85, pfnLeq(h3, d12, literalI64(30)));
        PROFILE(86, pfnGeq(h4, d11, literalI64(1)));
        PROFILE(87, pfnLeq(h5, d11, literalI64(15)));
        PROFILE(88, pfnAnd(h6, h0, h1));
        PROFILE(89, pfnAnd(h7, h2, h3));
        PROFILE(90, pfnAnd(h8, h4, h5));
        PROFILE(91, pfnAnd(h9, h6, h7));
        PROFILE(92, pfnAnd(h10, h9, h8));
    }
    else {
        PROFILE(84, optLoopFusionQ19_4(h10, vn(h1), d9, literalSym((S)"Brand#34"), d11, d12, h1));
    }

    PROFILE(93, pfnOr(m0, r10, k10));
    PROFILE(94, pfnOr(m1, m0 , h10));
    PROFILE(95, pfnCompress(m2, m1, d15));
    PROFILE(96, pfnCompress(m3, m1, d16));
    PROFILE(97, pfnMinus(m4, literalF64(1), m3));
    PROFILE(98, pfnMul(m5, m2, m4));
    PROFILE(99, pfnSum(m6, m5));

    // return
    PROFILE(100, copyV(z0,literalSym((S)"revenue")));
    PROFILE(101, pfnEnlist(z1,m6));
    PROFILE(102, pfnTable(z,z0,z1));

    gettimeofday(&tv1, NULL);
    E elapsed = calcInterval(tv0,tv1)/1000.0;
    P("The elapsed time (ms): %g\n\n", elapsed);
    printV(z);
    R elapsed;
}

L testTPCHQ19(){
    P("** Start simulation for TPC-H Query 19\n");
    initTableByName((S)"lineitem");
    initTableByName((S)"part");
    L cur = getHeapOffset();
    DOI(TEST_RUNS, {setHeapOffset(cur); times[i]=simulateQ19();})
    P("** End Query 19\n");
    R 0;
}