
L simulateQ12(){
	L e = 0;
	V a0 = allocNode(); V a1 = allocNode();
	V t0 = allocNode(); V t1 = allocNode(); V t2 = allocNode(); V t3 = allocNode();
	V t4 = allocNode(); V t5 = allocNode(); V t6 = allocNode();
	V j0 = allocNode();
	V w0 = allocNode(); V w1 = allocNode(); V w2 = allocNode(); V w3 = allocNode();
	V w4 = allocNode(); V w5 = allocNode(); V w6 = allocNode(); V w7 = allocNode();
	V w8 = allocNode(); V w9 = allocNode(); V w10= allocNode(); V w11= allocNode();
	V w12= allocNode();
	V g0 = allocNode();
	V s0 = allocNode(); V s1 = allocNode(); V s2 = allocNode(); V s3 = allocNode();
	V s4 = allocNode(); V s5 = allocNode(); V s6 = allocNode(); V s7 = allocNode();
	V s8 = allocNode(); V s9 = allocNode(); V s10= allocNode(); V s11= allocNode();
	V s12= allocNode();
	V r0 = allocNode();
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
    PROFILE(11,pfnMember(w0,t0,liter1));
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
    PROFILE(22,pfnValues(w11,w10));
    PROFILE(23,pfnToIndex(w12,w10));

    // step 2: group by
    PROFILE(24,pfnGroup(g0,w11));

    // step 3: select
    PROFILE(25,pfnKeys(s0,g0));
    PROFILE(26,pfnValues(s1,g0));
    PROFILE(27,pfnRaze(s2,s1));
    PROFILE(28,pfnIndex(s3,t3,s2));
    PROFILE(29,pfnIndex(s4,t1,w12));
    PROFILE(30,pfnEq(s5,s4,literalSym((S)"1_URGENT")));
    PROFILE(31,pfnEq(s6,s4,literalSym((S)"2_HIGH")));
    PROFILE(32,pfnOr(s7,s5,s6));
    PROFILE(33,pfnSum(s8,s7));
    PROFILE(34,pfnNeq(s9,s4,literalSym((S)"1_URGENT")));
    PROFILE(35,pfnNeq(s10,s4,literalSym((S)"2_HIGH")));
    PROFILE(36,pfnAnd(s11,s9,s10));
    PROFILE(37,pfnSum(s12,s11));

    // step 4: order by
    PROFILE(38,pfnOrderBy(r0,s3,literalBool(1)));

    // step 5: materialization
    PROFILE(39,pfnIndex(m0,s2,r0));
    PROFILE(40,pfnIndex(m1,s8,r0));
    PROFILE(41,pfnIndex(m2,s12,r0));

    S literStr2[] = {(S)"l_shipmode", (S)"high_line_count", (S)"low_line_count"};
    V liter2 = literalSymVector(3, literStr2);
    PROFILE(42,copyV(z0,liter2));
    PROFILE(43,pfnToList(z1,z0));
    V rn[] = {m0,m1,m2};
    PROFILE(44,pfnList(z2,3,rn));
    PROFILE(45,pfnTable(z,z1,z2));

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