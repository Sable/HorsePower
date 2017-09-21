
L simulateQ12(){
	L e = 0;
	V a0 = allocNode(); V a1 = allocNode();
	V t0 = allocNode(); V t1 = allocNode(); V t2 = allocNode(); V t3 = allocNode();
	V t4 = allocNode(); V t5 = allocNode(); V t6 = allocNode();
	V j0 = allocNode();
	V w0 = allocNode(); V w1 = allocNode(); V w2 = allocNode(); V w3 = allocNode();
	V w4 = allocNode(); V w5 = allocNode(); V w6 = allocNode(); V w7 = allocNode();
	V w8 = allocNode(); V w9 = allocNode(); V w10= allocNode(); V w11= allocNode();
	V w12= allocNode(); V w13= allocNode(); V w14= allocNode(); V w15= allocNode();
	V g0 = allocNode();
	V s0 = allocNode(); V s1 = allocNode(); V s2 = allocNode(); V s3 = allocNode();
	V s4 = allocNode(); V s5 = allocNode(); V s6 = allocNode(); V s7 = allocNode();
	V s8 = allocNode(); V s9 = allocNode(); V s10= allocNode(); V s11= allocNode();
	V s12= allocNode(); V s13= allocNode(); V s14= allocNode();
	V r0 = allocNode(); V r1 = allocNode();
	V m0 = allocNode(); V m1 = allocNode(); V m2 = allocNode();
	V z0 = allocNode(); V z1 = allocNode(); V z2 = allocNode(); V z = allocNode();
	struct timeval tv0, tv1;
    gettimeofday(&tv0, NULL);

    PROFILE( 1,pfnLoadTable(a0, literalSym((S)"orders")));
    PROFILE( 2,pfnLoadTable(a1, literalSym((S)"lineitem")));
    PROFILE( 3,pfnColumnValue(t0, a0, literalSym((S)"o_orderkey")));
    PROFILE( 4,pfnColumnValue(t1, a0, literalSym((S)"o_orderpriority")));
    PROFILE( 5,pfnColumnValue(t2, a1, literalSym((S)"l_orderkey")));
    PROFILE( 6,pfnColumnValue(t3, a1, literalSym((S)"l_shipmode")));
    PROFILE( 7,pfnColumnValue(t4, a1, literalSym((S)"l_commitdate")));
    PROFILE( 8,pfnColumnValue(t5, a1, literalSym((S)"l_receiptdate")));
    PROFILE( 9,pfnColumnValue(t6, a1, literalSym((S)"l_shipdate")));

    PROFILE(10,pfnEnum(j0, t0, t2));
    S literStr1[] = {(S)"MAIL", (S)"SHIP"};
    V liter1 = literalSymVector(2, literStr1);
    PROFILE(11,pfnMember(w0,liter1,t3));
    PROFILE(12,pfnLt(w1,t4,t5));
    PROFILE(13,pfnLt(w2,t6,t4));
    PROFILE(14,pfnGeq(w3,t5,literalDate(19940101)));
    PROFILE(15,pfnDatetimeAdd(w4,literalDate(19940101),literalI64(1),literalSym((S)"year")));
    PROFILE(16,pfnLt(w5,t5,w4));
    PROFILE(17,pfnAnd(w6,w0,w1));
    PROFILE(18,pfnAnd(w7,w6,w2));
    PROFILE(19,pfnAnd(w8,w7,w3));
    PROFILE(20,pfnAnd(w9,w8,w5));
    PROFILE(21,pfnCompress(w10,w9,j0));
    PROFILE(22,pfnCompress(w11,w9,t3));
    PROFILE(23,pfnToIndex(w12,j0));    /* key step 1 */
    PROFILE(24,pfnWhere(w13,w9));      /* key step 2 */
    PROFILE(25,pfnIndex(w14,w12,w13)); /* key step 3 */

    // step 2: group by
    PROFILE(26,pfnGroup(g0,w11));

    // step 3: select
    PROFILE(27,pfnKeys(s0,g0));
    PROFILE(28,pfnValues(s1,g0));
    PROFILE(30,pfnRaze(s2,s1));
    PROFILE(31,pfnIndex(s3,t3,s2));
    PROFILE(32,pfnEachRight(w15,w14,s1,pfnIndex));
    PROFILE(33,pfnEachRight(s4,t1,w15,pfnIndex)); //w12->s1
    PROFILE(34,pfnEachLeft(s5,s4,literalSym((S)"1-URGENT"),pfnEq));
    PROFILE(36,pfnEachLeft(s6,s4,literalSym((S)"2-HIGH")  ,pfnEq));
    PROFILE(38,pfnEachItem(s7,s5,s6,pfnOr));
    PROFILE(39,pfnEach(s8,s7,pfnSum));
    PROFILE(40,pfnEachLeft(s9 ,s4,literalSym((S)"1-URGENT"),pfnNeq));
    PROFILE(41,pfnEachLeft(s10,s4,literalSym((S)"2-HIGH")  ,pfnNeq));
    PROFILE(42,pfnEachItem(s11,s9,s10,pfnAnd));
    PROFILE(43,pfnEach(s12,s11,pfnSum));
    PROFILE(44,pfnRaze(s13,s8));
    PROFILE(45,pfnRaze(s14,s12));

    // step 4: order by
    PROFILE(46,pfnRaze(r1,s0));
    PROFILE(47,pfnOrderBy(r0,r1,literalBool(1)));

    // step 5: materialization
    PROFILE(48,pfnIndex(m0,r1 ,r0));
    PROFILE(49,pfnIndex(m1,s13,r0));
    PROFILE(50,pfnIndex(m2,s14,r0));

    S literStr2[] = {(S)"l_shipmode", (S)"high_line_count", (S)"low_line_count"};
    V liter2 = literalSymVector(3, literStr2);
    PROFILE(51,copyV(z0,liter2));
    PROFILE(52,pfnToList(z1,z0));
    V rn[] = {m0,m1,m2};
    PROFILE(53,pfnList(z2,3,rn));
    PROFILE(54,pfnTable(z,z1,z2));

    gettimeofday(&tv1, NULL);
    P("Result of the Query 12: (elapsed time %g ms)\n\n", calcInterval(tv0,tv1)/1000.0);
    printV(z);
	R 0;
}

L testTPCHQ12(){
    P("** Start simulation for TPC-H Query 12\n");
    initTableByName((S)"orders");
    initTableByName((S)"lineitem");
    simulateQ12();
    P("** End Query 12\n");
    R 0;
}