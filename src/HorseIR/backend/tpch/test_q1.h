
L simulateQ1(){
    L e;
    V a0 = allocNode();
    V t0 = allocNode();  V t1 = allocNode();  V t2 = allocNode();  V t3 = allocNode();
    V t4 = allocNode();  V t5 = allocNode();  V t6 = allocNode();
    V w0 = allocNode();  V w1 = allocNode();  V w2 = allocNode();
    V g0 = allocNode();  V g1 = allocNode();  V g2 = allocNode();  V g3 = allocNode();
    V g4 = allocNode();  V g5 = allocNode();  V g6 = allocNode();  V g7 = allocNode();
    V s0 = allocNode();  V s1 = allocNode();  V s2 = allocNode();  V s3 = allocNode();
    V s4 = allocNode();  V s5 = allocNode();  V s6 = allocNode();  V s7 = allocNode();
    V s8 = allocNode();  V s9 = allocNode();  V s10= allocNode();  V s11= allocNode();
    V s12= allocNode();  V s13= allocNode();  V s14= allocNode();  V s15= allocNode();
    V s16= allocNode();  V s17= allocNode();  V s18= allocNode();  V s19= allocNode();
    V s20= allocNode();  V s21= allocNode();  V s22= allocNode();  V s23= allocNode();
    V s24= allocNode();  V s25= allocNode();  V s26= allocNode();
    V r0 = allocNode();  V b0 = allocNode();
    V m0 = allocNode();  V m1 = allocNode();  V m2 = allocNode();  V m3 = allocNode();
    V m4 = allocNode();  V m5 = allocNode();  V m6 = allocNode();  V m7 = allocNode();
    V m8 = allocNode();  V m9 = allocNode();
    V z0 = allocNode();  V z1 = allocNode();  V z2 = allocNode();  V z  = allocNode();

    struct timeval tv0, tv1;
    gettimeofday(&tv0, NULL);

    PROFILE( 1, pfnLoadTable(a0, literalSym((S)"lineitem")));
    PROFILE( 2, pfnColumnValue(t0, a0, literalSym((S)"l_extendedprice")));
    PROFILE( 3, pfnColumnValue(t1, a0, literalSym((S)"l_discount")));
    PROFILE( 4, pfnColumnValue(t2, a0, literalSym((S)"l_shipdate")));
    PROFILE( 5, pfnColumnValue(t3, a0, literalSym((S)"l_quantity")));
    PROFILE( 6, pfnColumnValue(t4, a0, literalSym((S)"l_returnflag")));
    PROFILE( 7, pfnColumnValue(t5, a0, literalSym((S)"l_linestatus")));
    PROFILE( 8, pfnColumnValue(t6, a0, literalSym((S)"l_tax")));

    // where
    PROFILE( 9, pfnLen(w0,a0));
    PROFILE(10, pfnDatetimeSub(w1,literalDate(19981201),literalI64(90),literalSym((S)"day")));
    PROFILE(11, pfnLeq(w2,t2,w1));

    // group by
    PROFILE(12, pfnCompress(g0,w2,t4));
    PROFILE(13, pfnCompress(g1,w2,t5));
    V group1[] = {g0,g1};

    PROFILE(14, pfnList(g2,2,group1));
    PROFILE(15, pfnGroup(g3,g2));
    PROFILE(16, pfnKeys(g4,g3));
    PROFILE(16, pfnValues(g5,g3));
    PROFILE(991, pfnWhere(g6,w2));
    PROFILE(992, pfnEachRight(g7,g6,g5,pfnIndex));
    // select
    PROFILE(17, pfnEachRight(s0, t3, g7, pfnIndex)); //l_quantity
    PROFILE(18, pfnEachRight(s1, t0, g7, pfnIndex)); //l_extendedprice
    PROFILE(19, pfnEachRight(s2, t1, g7, pfnIndex)); //l_discount
    PROFILE(20, pfnEachRight(s3, t6, g7, pfnIndex)); //l_tax
    PROFILE(21, pfnEach(s4, s0, pfnSum));            //sum_qty
    PROFILE(22, pfnEach(s5, s1, pfnSum));            //sum_base_price
    PROFILE(23, pfnEachItem(s6, literalI64(1), s2, pfnMinus));
    PROFILE(24, pfnEachItem(s7, s1, s6, pfnMul));
    PROFILE(25, pfnEach(s8, s7, pfnSum));            //sum_disc_price

    PROFILE(26, pfnEachItem(s9, literalI64(1), s2, pfnMinus));
    PROFILE(27, pfnEachItem(s10, s1, s9, pfnMul));
    PROFILE(28, pfnEachItem(s11, literalI64(1), s3, pfnPlus));
    PROFILE(29, pfnEachItem(s12, s10, s11, pfnMul)); // sum_charge

    PROFILE(30, pfnEach(s13, s0, pfnAvg));           //avg_qty
    PROFILE(31, pfnEach(s14, s1, pfnAvg));           //avg_price
    PROFILE(32, pfnEach(s15, s2, pfnAvg));           //avg_disc
    PROFILE(33, pfnEach(s16, g7, pfnLen));           //count_order
    PROFILE(34, pfnIndex(s17,g4,literalI64(0)));     //l_returnflag
    PROFILE(35, pfnIndex(s18,g4,literalI64(1)));     //l_linestatus
    PROFILE(36, pfnRaze(s19,s4));
    PROFILE(37, pfnRaze(s20,s5));
    PROFILE(38, pfnRaze(s21,s8));
    PROFILE(39, pfnRaze(s22,s12));
    PROFILE(40, pfnRaze(s23,s13));
    PROFILE(41, pfnRaze(s24,s14));
    PROFILE(42, pfnRaze(s25,s15));
    PROFILE(43, pfnRaze(s26,s16));
    // order by
    PROFILE(44, (initV(b0,H_B,2),vB(b0,0)=1,vB(b0,1)=1,0));
    PROFILE(45, pfnOrderBy(r0,g4,b0));
    // materialization
    PROFILE(46, pfnIndex(m0,s17,r0));
    PROFILE(47, pfnIndex(m1,s18,r0));
    PROFILE(48, pfnIndex(m2,s19,r0));
    PROFILE(49, pfnIndex(m3,s20,r0));
    PROFILE(50, pfnIndex(m4,s21,r0));
    PROFILE(51, pfnIndex(m5,s22,r0));
    PROFILE(52, pfnIndex(m6,s23,r0));
    PROFILE(53, pfnIndex(m7,s24,r0));
    PROFILE(54, pfnIndex(m8,s25,r0));
    PROFILE(55, pfnIndex(m9,s26,r0));

    // printV(m0);
    // printV(m1);
    // printV(m2);
    // printV(m3);
    // printV(m4);
    // printV(m5);
    // printV(m6);
    // printV(m7);
    // printV(m8);
    // printV(m9);

    S strs[] = {(S)"l_returnflag", (S)"l_linestatus", (S)"sum_qty", (S)"sum_base_price",\
                (S)"sum_disc_price", (S)"sum_charge", (S)"avg_qty", (S)"avg_price"     ,\
                (S)"avg_disc",     (S)"count_order"};
    PROFILE(56, copyV(z0,literalSymVector(10,strs)));
    // PROFILE(57, pfnToList(z1,z0));
    V group2[] = {m0,m1,m2,m3,m4,m5,m6,m7,m8,m9};
    PROFILE(58, pfnList(z1,10,group2));
    PROFILE(59, pfnTable(z,z0,z1));

    gettimeofday(&tv1, NULL);
    P("Result (elapsed time %g ms)\n\n", calcInterval(tv0,tv1)/1000.0);
    printV(z);
    R 0;
}

L testTPCHQ1(){
    P("** Start simulation for TPC-H Query 1\n");
    initTableByName((S)"lineitem");
    simulateQ1();
    P("** End Query 1\n");
    R 0;
}



