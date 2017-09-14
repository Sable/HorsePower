
L simulateQ4(){
	struct timeval tv0, tv1;
    gettimeofday(&tv0, NULL);

    PROFILE(1,pfnLoadTable(a0, initSymbol(allocNode(),getSymbol((S)"orders"))));
    PROFILE(2,pfnLoadTable(a1, initSymbol(allocNode(),getSymbol((S)"lineitem"))));

    PROFILE(3,pfnColumnValue(t0, a0, initSymbol(allocNode(), getSymbol((S)"o_orderdate"))));
    PROFILE(4,pfnColumnValue(t1, a0, initSymbol(allocNode(), getSymbol((S)"o_orderkey"))));
    PROFILE(5,pfnColumnValue(t2, a0, initSymbol(allocNode(), getSymbol((S)"o_orderpriority"))));
    PROFILE(6,pfnColumnValue(t3, a1, initSymbol(allocNode(), getSymbol((S)"l_orderkey"))));
    PROFILE(7,pfnColumnValue(t4, a1, initSymbol(allocNode(), getSymbol((S)"l_commitdate"))));
    PROFILE(8,pfnColumnValue(t5, a1, initSymbol(allocNode(), getSymbol((S)"l_receiptdate"))));

    // step 1: where clause
    PROFILE(9 ,pfnGeq(w0, literalDate(19930701)));
    PROFILE(10,pfnDatetimeAdd(w1, literalDate(19930701), literalI64(3),literalSym((S)"month")));
    PROFILE(11,pfnLt(w2, t0, w1));
    PROFILE(12,pfnAnd(w3, w1, w2));
    PROFILE(13,pfnIndexOf(w4, t1, t3));
    PROFILE(14,pfnLen(w5, t1));
    PROFILE(15,pfnLt(w6, w4, w5));
    PROFILE(16,pfnLt(w7, t4, t5));
    PROFILE(17,pfnAnd(w8, w6, w7));
    PROFILE(18,pfnCompress(w9, w6, w8));
    PROFILE(19,pfnUnique(w10, w9));
    PROFILE(20,pfnLen(w11, t3));
    PROFILE(21,pfnVector(w12, w11, literalBool(0)));
    PROFILE(22,pfnIndexA(w12, w10, literalBool(1)));
    PROFILE(23,pfnAnd(w13, w3, w12));

    // step 2: group by
    PROFILE(24,pfnCompress(g0, w13, t2));
    PROFILE(25,pfnGroup(g1, g0));

    // step 3: select
    PROFILE(26,pfnCopy(s0, g0));
    PROFILE(27,pfnEachItem(s1, g1, pfnCount));
    PROFILE(28,pfnRaze(s2, s1));

    // step 4: order by
    PROFILE(29,pfnOrder(r0, s0));

    // step 5: materialization
    PROFILE(30,pfnLiteralSymbol(m0, {"o_orderpriority", "order_count"}));
    PROFILE(31,pfnTolist(m1, m0));
    PROFILE(32,pfnIndex(m2, s0, r0));
    PROFILE(33,pfnIndex(m3, s2, r0));
    PROFILE(34,pfnList(m4, m2, m3));

    // finally, return the table
    PROFILE(35,pfnTable(z, m1, m4));

    P("Result of the Query 4: (elapsed time %g ms)\n\n", calcInterval(tv0,tv1)/1000.0);
    printV(z);
	R 0;
}

L testTPCHQ4(){
    P("** Start simulation for TPC-H Query 4\n");
    initTableByName((S)"orders");
    initTableByName((S)"lineitem");
    simulateQ4();
    P("** End Query 4\n");
    R 0;
}
