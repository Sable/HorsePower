
/*
 * where
 *   s_suppkey  = l_suppkey
 *   ps_suppkey = l_suppkey
 *   ps_partkey = l_partkey
 *   p_partkey = l_partkey
 *   o_orderkey = l_orderkey
 *   s_nationkey = n_nationkey
 *   p_name like '%green%'
 */
L createTableProfit(V* z){
    L e;
    V a0 = allocNode(); V a1 = allocNode(); V a2 = allocNode();
    V a3 = allocNode(); V a4 = allocNode(); V a5 = allocNode();
    V t0 = allocNode(); V t1 = allocNode(); V t2 = allocNode(); V t3 = allocNode();
    V t4 = allocNode(); V t5 = allocNode(); V t6 = allocNode(); V t7 = allocNode();
    V t8 = allocNode(); V t9 = allocNode(); V t10= allocNode(); V t11= allocNode();
    V t12= allocNode(); V t13= allocNode(); V t14= allocNode(); V t15= allocNode();
    V j0 = allocNode(); V j1 = allocNode(); V j2 = allocNode();
    V j3 = allocNode(); V j4 = allocNode();
    V w0 = allocNode(); V w1 = allocNode(); V w2 = allocNode(); V w3 = allocNode();
    V w4 = allocNode(); V w5 = allocNode(); V w6 = allocNode(); V w7 = allocNode();
    V w8 = allocNode(); /*V w9 */           V w10= allocNode(); V w11 = allocNode();
    V w12= allocNode(); V w13= allocNode(); /*V w14 */          V w15 = allocNode();
    V w16 = allocNode(); V w17 = allocNode(); V w18 = allocNode();
    V s0 = allocNode(); V s1 = allocNode(); V s2 = allocNode(); V s3 = allocNode();
    V s4 = allocNode();
    V k0 = allocNode(); V k1 = allocNode();
    V m0 = allocNode(); V m1 = allocNode(); V m2 = allocNode();
    V z0 = allocNode(); V z1 = allocNode(); V z2 = allocNode();

    PROFILE( 1, pfnLoadTable(a0, literalSym((S)"part")));
    PROFILE( 2, pfnLoadTable(a1, literalSym((S)"supplier")));
    PROFILE( 3, pfnLoadTable(a2, literalSym((S)"lineitem")));
    PROFILE( 4, pfnLoadTable(a3, literalSym((S)"partsupp")));
    PROFILE( 5, pfnLoadTable(a4, literalSym((S)"orders")));
    PROFILE( 6, pfnLoadTable(a5, literalSym((S)"nation")));

    PROFILE( 7, pfnColumnValue(t0 , a0, literalSym((S)"p_partkey")));
    PROFILE( 8, pfnColumnValue(t1 , a0, literalSym((S)"p_name")));
    PROFILE( 9, pfnColumnValue(t2 , a1, literalSym((S)"s_nationkey")));
    PROFILE(10, pfnColumnValue(t3 , a1, literalSym((S)"s_suppkey")));
    PROFILE(11, pfnColumnValue(t4 , a2, literalSym((S)"l_partkey")));
    PROFILE(12, pfnColumnValue(t5 , a2, literalSym((S)"l_suppkey")));
    PROFILE(13, pfnColumnValue(t6 , a2, literalSym((S)"l_orderkey")));
    PROFILE(14, pfnColumnValue(t7 , a2, literalSym((S)"l_extendedprice")));
    PROFILE(15, pfnColumnValue(t8 , a2, literalSym((S)"l_discount")));
    PROFILE(16, pfnColumnValue(t9 , a2, literalSym((S)"l_quantity")));
    PROFILE(17, pfnColumnValue(t10, a3, literalSym((S)"ps_partkey")));
    PROFILE(18, pfnColumnValue(t11, a3, literalSym((S)"ps_suppkey")));
    PROFILE(19, pfnColumnValue(t12, a3, literalSym((S)"ps_supplycost")));
    PROFILE(20, pfnColumnValue(t13, a4, literalSym((S)"o_orderkey")));
    PROFILE(21, pfnColumnValue(t14, a4, literalSym((S)"o_orderdate")));
    PROFILE(22, pfnColumnValue(t15, a5, literalSym((S)"n_nationkey")));

    // PROFILE(23, pfnLike(w0,t1,literalString((S)"%%green%%")));
    // PROFILE(24, pfnSum(w1,w0));
    // printV(w1);
    // R 0;

    // step 0: joins
    PROFILE(23, pfnEnum(j0,t0,t1));
    PROFILE(24, pfnEnum(j1,t2,t3));
    PROFILE(25, pfnEnum(j2,t4,t5));
    V key0[] = {t6,t7};  CHECKE(pfnList(k0,2,key0));
    V key1[] = {t3,t5};  CHECKE(pfnList(k1,2,key1));
    PROFILE(26, pfnEnum(j3,k0,k1));
    PROFILE(27, pfnEnum(j4,t8,t9));

    PROFILE(28, pfnLike(w0,t10,literalString((S)"%%green%%")));
    PROFILE(29, pfnToIndex(w1,j1));
    PROFILE(30, pfnIndex(w2,w0,w1));
    PROFILE(31, pfnToIndex(w3,j3));
    PROFILE(32, pfnIndex(w4,w2,w3));
    PROFILE(33, pfnToIndex(w5,j4));
    PROFILE(34, pfnCompress(w6,w4,w5));
    PROFILE(35, pfnIndex(w7,t15,w6));
    PROFILE(36, pfnCompress(w8,w4,w3));
    PROFILE(37, pfnIndex(w10,w3,w8));
    PROFILE(38, pfnToIndex(w11,j0));
    PROFILE(39, pfnIndex(w12,w11,w10));
    PROFILE(40, pfnIndex(w13,t0,w12));    //n_name
    PROFILE(41, pfnCompress(w15,w4,w11)); //l_extendedprice
    PROFILE(42, pfnCompress(w16,w4,w12)); //l_discount
    PROFILE(43, pfnCompress(w17,w4,w13)); //l_quantity
    PROFILE(44, pfnIndex(w18,t14,w8));    //ps_supplycost

    PROFILE(45, pfnDateYear(s0,w7));
    PROFILE(46, pfnMinus(s1,literalF64(1),w16));
    PROFILE(47, pfnMul(s2,w15,s1));
    PROFILE(48, pfnMul(s3,w18,w17));
    PROFILE(49, pfnMinus(s4,s2,s3));

    PROFILE(50, copyV(m0,w13));
    PROFILE(51, copyV(m1,s0));
    PROFILE(52, copyV(m2,s4));

    S strs2[] = {(S)"nation", (S)"o_year", (S)"amount"};
    PROFILE(53, copyV(z0, literalSymVector(3,strs2)));
    PROFILE(54, pfnToList(z1,z0));
    V list0[] = {m0,m1,m2};
    PROFILE(55, pfnList(z2,3,list0));
    PROFILE(56, pfnTable(*z,z1,z2));
    R 0;
}

L simulateQ9(){
    L e;
    V a0 = allocNode();
    V t0 = allocNode(); V t1 = allocNode(); V t2 = allocNode();
    V g0 = allocNode(); V g1 = allocNode();
    V s0 = allocNode(); V s1 = allocNode(); V s2 = allocNode(); V s3 = allocNode();
    V s4 = allocNode(); V s5 = allocNode(); V s6 = allocNode();
    V r0 = allocNode(); V r1 = allocNode(); V r2 = allocNode();
    V m0 = allocNode(); V m1 = allocNode(); V m2 = allocNode();
    V z0 = allocNode(); V z1 = allocNode(); V z2 = allocNode(); V z = allocNode();
    struct timeval tv0, tv1;
    gettimeofday(&tv0, NULL);

    PROFILE( 1, createTableProfit(&a0));
    R 0;
    PROFILE( 2, pfnColumnValue(t0, a0, literalSym((S)"nation")));
    PROFILE( 3, pfnColumnValue(t1, a0, literalSym((S)"o_year")));
    PROFILE( 4, pfnColumnValue(t2, a0, literalSym((S)"amount")));

    V list2[] = {t0,t1};
    PROFILE( 5, pfnList(g0,2,list2));
    PROFILE( 6, pfnGroup(g1,g0));

    PROFILE( 7, pfnKeys(s0,g1));
    PROFILE( 8, pfnIndex(s1,s0,literalI64(0)));
    PROFILE( 9, pfnIndex(s1,s1,literalI64(1)));
    PROFILE(10, pfnValues(s3,g1));
    PROFILE(11, pfnEachRight(s4,t2,s3,pfnIndex));
    PROFILE(12, pfnEach(s5,s4,pfnSum));
    PROFILE(13, pfnRaze(s6,s5));

    V list0[] = {s1,s2};
    PROFILE(14, pfnList(r0,2,list0));
    PROFILE(15, (initV(r1,H_B,2),vB(r1,0)=1,vB(r1,1)=0,0));
    PROFILE(16, pfnOrderBy(r2,r0,r1));

    PROFILE(17, pfnIndex(m0,s1,r2));
    PROFILE(18, pfnIndex(m1,s2,r2));
    PROFILE(19, pfnIndex(m2,s3,r2));

    S strs[] = {(S)"nation", (S)"o_year", (S)"sum_profit"};
    PROFILE(20, copyV(z0,literalSymVector(3,strs)));
    PROFILE(21, pfnToList(z1,z0));
    V list1[] = {m0,m1,m2};
    PROFILE(22, pfnList(z2,3,list1));
    PROFILE(23, pfnTable(z,z1,z2));

    gettimeofday(&tv1, NULL);
    P("Result of the Query 9: (elapsed time %g ms)\n\n", calcInterval(tv0,tv1)/1000.0);
    printV(z);
    R 0;
}

L testTPCHQ9(){
    P("** Start simulation for TPC-H Query 9\n");
    initTableByName((S)"part");
    initTableByName((S)"supplier");
    initTableByName((S)"lineitem");
    initTableByName((S)"partsupp");
    initTableByName((S)"orders");
    initTableByName((S)"nation");
    // PROFILE(91, pfnAddFKey(literalSym((S)"customer"), literalSym((S)"c_custkey"),\
    //                        literalSym((S)"orders"),   literalSym((S)"o_custkey")));
    simulateQ9();
    P("** End Query 9\n");
    R 0;
}

