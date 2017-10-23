
V simulateQ18_sub(){
	V a0 = allocNode();
	V t0 = allocNode();  V t1 = allocNode();
	V w0 = allocNode();  V w1 = allocNode();  V w2 = allocNode();  V w3 = allocNode();
	V w4 = allocNode();  V w5 = allocNode();  V w6 = allocNode();  V w7 = allocNode();
	V f0 = allocNode();
	PROFILE(1, pfnLoadTable(a0, literalSym((S)"lineitem")));
	PROFILE(2, pfnColumnValue(t0, a0, literalSym((S)"l_orderkey"))); // fkey range [1, 6000000]
	PROFILE(3, pfnColumnValue(t1, a0, literalSym((S)"l_quantity")));
	if(!isOptimized){
		PROFILE(99, pfnFetch(f0, t0));
		PROFILE(4, pfnGroupBucket(w0, f0));
		PROFILE(5, pfnValues(w1, w0));      // size range [1, 7]
		PROFILE(6, pfnKeys(w2, w0));
		PROFILE(7, pfnEachRight(w3, t1, w1, pfnIndex));  // <-- slow
		PROFILE(8, pfnEach(w4, w3, pfnSum));             // <-- slow
		PROFILE(9, pfnRaze(w5, w4));
	}
	else {
		V f1 = allocNode();  V f2 = allocNode();
		PROFILE(99, pfnValues(f0, t0));
		PROFILE( 7, optLoopFusionQ18_1(w0, f0, t1));  // avoid a large group with small cells
		PROFILE( 8, pfnKeys           (f1, w0));
		PROFILE( 9, pfnValues         (w5, w0));
		PROFILE(10, pfnKeys           (f2, t0));
		PROFILE(11, pfnIndex          (w2, f2, f1));
		// P("keys w2."); DOI(20, P(" %lld",vL(w2,i)))   P("\n");
	}
	PROFILE(10, pfnGt(w6, w5, literalF64(300)));
	PROFILE(11, pfnCompress(w7, w6, w2));
	R w7;
}

L simulateQ18(){
	V a0 = allocNode();  V a1 = allocNode();  V a2 = allocNode();
	V t0 = allocNode();  V t1 = allocNode();  V t2 = allocNode();  V t3 = allocNode();
	V t4 = allocNode();  V t5 = allocNode();  V t6 = allocNode();
	V w0 = allocNode();  V w1 = allocNode();  V w2 = allocNode();  V w3 = allocNode();
	V w4 = allocNode();  V w5 = allocNode();  V w6 = allocNode();  V w7 = allocNode();
	V w8 = allocNode();  V w9 = allocNode();  V w10= allocNode();  V w11= allocNode();
	V w12= allocNode();  V w13= allocNode();
	V d0 = allocNode();  V d1 = allocNode();  V d2 = allocNode();  V d3 = allocNode();
	V d4 = allocNode();  V d5 = allocNode();  V d6 = allocNode();  V d7 = allocNode();
	V d8 = allocNode();  V d9 = allocNode();  V d10= allocNode();  V d11= allocNode();
	V d12= allocNode();
	V g0 = allocNode();  V g1 = allocNode();
	V m0 = allocNode();  V m1 = allocNode();  V m2 = allocNode();  V m3 = allocNode();
	V m4 = allocNode();  V m5 = allocNode();
	V z0 = allocNode();  V z1 = allocNode();  V z  = allocNode();
	
	struct timeval tv0, tv1;
    gettimeofday(&tv0, NULL);

	PROFILE( 1, pfnLoadTable(a0, literalSym((S)"customer")));
	PROFILE( 2, pfnLoadTable(a1, literalSym((S)"orders")));
	PROFILE( 3, pfnLoadTable(a2, literalSym((S)"lineitem")));
	PROFILE( 4, pfnColumnValue(t0, a1, literalSym((S)"o_orderkey")));
	V sub = simulateQ18_sub();  //printV(sub);  P("len of sub: %lld\n", vn(sub));
	PROFILE( 5, pfnMember(w0, sub, t0));
	// L group9[] = {6882,29158,502886,551136,565574,735366,857959,967334,983201,1263015,1436544,1474818,1481925,1544643,1742403,1845057,1971680,2096705,2199712,2232932,2329187,2366755,2662214,2745894,2761378,2806245,2869152,2942469,2995076,3037414,3043270,3342468,3767271,3861123,3883783,4134341,4259524,4267751,4290656,4439686,4478371,4527553,4702759,4720454,4722021,4739650,4745607,4806726,5007490,5156581,5200102,5296167,5489475,5746311,5832321,5849444,5984582};
	// PROFILE( 5, pfnMember(w0, literalI64Vector(57, group9), t0));

	// join 1
	PROFILE( 6, pfnColumnValue(t1, a2, literalSym((S)"l_orderkey")));
	PROFILE( 7, pfnValues(w1, t1));
	PROFILE( 8, pfnKeys(w2, t1));
	PROFILE( 9, pfnIndex(w3, w0, w1));
	// PROFILE(10, pfnCompress(w4, w3, t1)); // valid l_orderkey
	PROFILE(11, pfnCompress(w5, w3, w1)); // valid o_orderkey (index)

	// join 2
	PROFILE(12, pfnColumnValue(t2, a1, literalSym((S)"o_custkey")));
	PROFILE(13, pfnValues(w6, t2));
	PROFILE(14, pfnKeys(w7, t2));
	PROFILE(15, pfnIndex(w8, w6, w5));
	PROFILE(16, pfnIndex(w9, w7, w8));    // c_custkey

	// project
	PROFILE(17, pfnColumnValue(t3, a0, literalSym((S)"c_name")));
	PROFILE(18, pfnColumnValue(t4, a1, literalSym((S)"o_orderdate")));
	PROFILE(19, pfnColumnValue(t5, a1, literalSym((S)"o_totalprice")));
	PROFILE(20, pfnIndex(w10, w2, w5));   // o_orderkey
	PROFILE(21, pfnIndex(w11, t3, w8));   // c_name
	PROFILE(22, pfnIndex(w12, t4, w5));   // o_orderdate
	PROFILE(23, pfnIndex(w13, t5, w5));   // o_totalprice

	// group by
	V group1[] = {w11, w9, w10, w12, w13};
	PROFILE(24, pfnList(d0, 5, group1));
	PROFILE(25, pfnGroup(d1, d0));
	PROFILE(26, pfnValues(d2, d1));
	PROFILE(27, pfnKeys(d3, d1));
	PROFILE(28, pfnColumnValue(t6, a2, literalSym((S)"l_quantity")));
	PROFILE(29, pfnCompress(d4, w3, t6));
	PROFILE(30, pfnEachRight(d5, d4, d2, pfnIndex));
	PROFILE(31, pfnEach(d6, d5, pfnSum));
	PROFILE(32, pfnRaze(d7, d6));         // sum(l_quantity)
	PROFILE(33, pfnIndex(d8, w13, d3));   // o_totalprice
	PROFILE(34, pfnIndex(d9, w12, d3));   // o_orderdate
	PROFILE(35, pfnIndex(d10, w10, d3));  // o_orderkey
	PROFILE(36, pfnIndex(d11, w9 , d3));  // c_custkey
	PROFILE(37, pfnIndex(d12, w11, d3));  // c_name
	
	// order by
	V group2[] = {d8, d9};
	PROFILE(38, pfnList(g0, 2 , group2));
	B group3[] = {0, 1};
	PROFILE(39, pfnOrderBy(g1, g0, literalBoolVector(2, group3)));

	// materialization
	PROFILE(40, pfnIndex(m0, d12, g1));
	PROFILE(41, pfnIndex(m1, d11, g1));
	PROFILE(42, pfnIndex(m2, d10, g1));
	PROFILE(43, pfnIndex(m3, d9 , g1));
	PROFILE(44, pfnIndex(m4, d8 , g1));
	PROFILE(45, pfnIndex(m5, d7 , g1));
	
	// return
	S strs[] = {(S)"c_name", (S)"c_custkey", (S)"o_orderkey", (S)"o_orderdate", (S)"o_totalprice", (S)"sum" };
    PROFILE(46, copyV(z0,literalSymVector(6,strs)));
    V group4[] = {m0,m1,m2,m3,m4,m5};
    PROFILE(47, pfnList(z1,6,group4));
    PROFILE(48, pfnTable(z,z0,z1));

	gettimeofday(&tv1, NULL);
    P("The elapsed time (ms): %g\n\n", calcInterval(tv0,tv1)/1000.0);
    printTablePretty(z, 10);  // limit 10
    P("size of z: row = %lld, col = %lld\n", tableRow(z), tableCol(z));
	R 0;
}

L testTPCHQ18(){
    P("** Start simulation for TPC-H Query 18\n");
    initTableByName((S)"customer");
    initTableByName((S)"orders");
    initTableByName((S)"lineitem");
    PROFILE(91, pfnAddFKey(literalSym((S)"customer"), literalSym((S)"c_custkey"),\
    	                  literalSym((S)"orders"),   literalSym((S)"o_custkey")));
    PROFILE(92, pfnAddFKey(literalSym((S)"orders"),   literalSym((S)"o_orderkey"),\
    	                  literalSym((S)"lineitem"), literalSym((S)"l_orderkey")));
    simulateQ18();
    P("** End Query 18\n");
    R 0;
}