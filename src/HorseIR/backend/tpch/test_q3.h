
L simulateQ3(){
	/* declaration */
	V a0 = allocNode();  V a1 = allocNode();  V a2 = allocNode();
	V t0 = allocNode();  V t1 = allocNode();  V t2 = allocNode();  V t3 = allocNode();
	V t4 = allocNode();  V t5 = allocNode();  V t10= allocNode();  V t11= allocNode();
	V w0 = allocNode();  V w1 = allocNode();  V w2 = allocNode();  V w3 = allocNode();
	V w4 = allocNode();  V w5 = allocNode();  V w6 = allocNode();  V w7 = allocNode();
	V w8 = allocNode();  V w9 = allocNode();  V w10= allocNode();
	V p1 = allocNode();  V p2 = allocNode();  V p3 = allocNode();  V p4 = allocNode();
	V p5 = allocNode();  V p6 = allocNode();  V p7 = allocNode();  V p8 = allocNode();
	V g1 = allocNode();  V g2 = allocNode();  V g3 = allocNode();  V g4 = allocNode();
	V g5 = allocNode();  V g6 = allocNode();  V g7 = allocNode();  V g8 = allocNode();
	V g9 = allocNode();  V g10= allocNode();
	V d0 = allocNode();  V d1 = allocNode();
	V m0 = allocNode();  V m1 = allocNode();  V m2 = allocNode();  V m3 = allocNode();
	V z0 = allocNode();  V z1 = allocNode();  V z  = allocNode();

	struct timeval tv0, tv1;
    gettimeofday(&tv0, NULL);

	PROFILE(1, pfnLoadTable(a0, literalSym((S)"customer")));
	PROFILE(2, pfnLoadTable(a1, literalSym((S)"orders")));
	PROFILE(3, pfnLoadTable(a2, literalSym((S)"lineitem")));

	// select 1
	PROFILE(4, pfnColumnValue(t0, a1, literalSym((S)"o_orderdate")));
	PROFILE(5, pfnColumnValue(t1, a1, literalSym((S)"o_shippriority")));
	PROFILE(6, pfnColumnValue(t2, a1, literalSym((S)"o_custkey")));     //fkey
	PROFILE(7, pfnLt(w0, t0, literalDate(19950315)));

	// select 2
	PROFILE(8, pfnColumnValue(t3, a0, literalSym((S)"c_mktsegment")));
	PROFILE(9, pfnEq(w1, t3, literalSym((S)"BUILDING")));

	// join 1
	PROFILE(10, pfnValues(w2, t2));     //index of order
	PROFILE(11, pfnIndex(w3, w1, w2));
	PROFILE(12, pfnAnd(w4, w0, w3));

	// select 3
	PROFILE(13, pfnColumnValue(t4, a2, literalSym((S)"l_shipdate")));
	PROFILE(14, pfnColumnValue(t5, a2, literalSym((S)"l_orderkey")));  //fkey
	PROFILE(15, pfnColumnValue(t10,a2, literalSym((S)"l_extendedprice")));
	PROFILE(16, pfnColumnValue(t11,a2, literalSym((S)"l_discount")));
	PROFILE(17, pfnGt(w5, t4, literalDate(19950315)));

	// join 2
	PROFILE(18, pfnValues(w6, t5));
	PROFILE(19, pfnIndex(w7, w4, w6));
	PROFILE(20, pfnAnd(w8, w5, w7));

	// ...
	PROFILE(21, pfnCompress(w9, w8, w6));
	PROFILE(22, pfnWhere(w10, w8));
	
	// project 1
	PROFILE(23, pfnKeys(p1, t5));
	// PROFILE(99, pfnIndex(p1, p0, w9));
	PROFILE(24, pfnIndex(p2, p1, w9));   // l_orderkey
	PROFILE(25, pfnIndex(p3, t0, w9));   // o_orderdate
	PROFILE(26, pfnIndex(p4, t1, w9));   // o_shippriority
	PROFILE(27, pfnIndex(p5, t10, w10)); // l_extendedprice
	PROFILE(28, pfnIndex(p6, t11, w10)); // l_discount
	PROFILE(29, pfnMinus(p7, literalF64(1.0), p6));
	PROFILE(30, pfnMul(p8, p5, p7));     // revenue
	P("done: project 1\n");

	// group by
	V group1[] = {p2, p3, p4};
	PROFILE(31, pfnList(g1, 3, group1));
	PROFILE(32, pfnGroup(g2, g1));                    // <--- slow, fixed
	PROFILE(33, pfnValues(g3, g2)); // values (index)
	PROFILE(34, pfnKeys(g4, g2));   // keys   (index)
	PROFILE(35, pfnEachRight(g5, p8, g3, pfnIndex));  // <--- slow
	PROFILE(36, pfnEach(g6, g5, pfnSum));             // <--- slow
	PROFILE(37, pfnIndex(g7, p2, g4));    // l_orderkey
	PROFILE(38, pfnIndex(g8, p3, g4));    // o_orderdate
	PROFILE(39, pfnIndex(g9, p4, g4));    // o_shippriority
	PROFILE(40, pfnRaze(g10, g6));        // revenue

	// order
	V group2[] = {g10, g7};
	PROFILE(44, pfnList(d0, 2, group2));
	B group3[] = {0, 1};
	PROFILE(45, pfnOrderBy(d1, d0, literalBoolVector(2,group3)));

	// materialization
	PROFILE(46, pfnIndex(m0, g7,  d1));
	PROFILE(47, pfnIndex(m1, g10, d1));
	PROFILE(48, pfnIndex(m2, g8,  d1));
	PROFILE(49, pfnIndex(m3, g9,  d1));

	// return
	S strs[] = {(S)"l_orderkey", (S)"revenue", (S)"o_orderdate", (S)"o_shippriority" };
    PROFILE(50, copyV(z0,literalSymVector(4,strs)));
    V group4[] = {m0,m1,m2,m3};
    PROFILE(51, pfnList(z1,4,group4));
    PROFILE(52, pfnTable(z,z0,z1));

	gettimeofday(&tv1, NULL);
    P("Result (elapsed time %g ms)\n\n", calcInterval(tv0,tv1)/1000.0);
    printTablePretty(z, 10);  // limit 10
    P("size of z: row = %lld, col = %lld\n", tableRow(z), tableCol(z));
    R 0;
}

L testTPCHQ3(){
    P("** Start simulation for TPC-H Query 3\n");
    initTableByName((S)"customer");
    initTableByName((S)"orders");
    initTableByName((S)"lineitem");
    PROFILE(91, pfnAddFKey(literalSym((S)"customer"), literalSym((S)"c_custkey"),\
    	                  literalSym((S)"orders"),   literalSym((S)"o_custkey")));
    PROFILE(92, pfnAddFKey(literalSym((S)"orders"),   literalSym((S)"o_orderkey"),\
    	                  literalSym((S)"lineitem"), literalSym((S)"l_orderkey")));
    simulateQ3();
    P("** End Query 3\n");
    R 0;
}
