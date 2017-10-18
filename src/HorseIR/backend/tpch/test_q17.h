
L simulateQ17(){
	V a0 = allocNode();  V a1 = allocNode();
	V t0 = allocNode();  V t1 = allocNode();  V t2 = allocNode();  V t3 = allocNode();
	V t4 = allocNode();  V t5 = allocNode();
	V w0 = allocNode();  V w1 = allocNode();  V w2 = allocNode();  V w3 = allocNode();
	V w4 = allocNode();  V w5 = allocNode();  V w6 = allocNode();  V w7 = allocNode();
	V w8 = allocNode();  V w9 = allocNode();  V w10= allocNode();  V w11= allocNode();
	V w12= allocNode();  V w13= allocNode();  V w14= allocNode();  V w15= allocNode(); /* Not use: w5 and w14 */
	V p0 = allocNode();  V p1 = allocNode();
	V z0 = allocNode();  V z1 = allocNode();  V z  = allocNode();

	V f0 = allocNode();  V f1 = allocNode();  V f2 = allocNode();  V f3 = allocNode();
	V f4 = allocNode();  V f5 = allocNode();  V f6 = allocNode();  V f7 = allocNode();
	V f8 = allocNode();  V f9 = allocNode();  V f10= allocNode();  V f11= allocNode();
	V f12= allocNode();
	
	struct timeval tv0, tv1;
    gettimeofday(&tv0, NULL);

	PROFILE(1, pfnLoadTable(a0, literalSym((S)"lineitem")));
	PROFILE(2, pfnLoadTable(a1, literalSym((S)"part")));

	PROFILE(3, pfnColumnValue(t0, a0, literalSym((S)"l_partkey")));
	PROFILE(4, pfnColumnValue(t1, a1, literalSym((S)"p_partkey")));
	PROFILE(5, pfnColumnValue(t2, a1, literalSym((S)"p_brand")));
	PROFILE(6, pfnColumnValue(t3, a1, literalSym((S)"p_container")));
	PROFILE(7, pfnEq(w0, t2, literalSym((S)"Brand#23")));
	PROFILE(8, pfnEq(w1, t3, literalSym((S)"MED BOX")));
	PROFILE(9, pfnAnd(w2, w0, w1));
	PROFILE(10, pfnCompress(w3, w2, t1));  // small p_partkey
	// P("len of w3: %lld\n", vn(w3));
	PROFILE(11, pfnMember(w4, w3, t0));    // find l_partkey in small p_partkey
	PROFILE(12, pfnColumnValue(t4, a0, literalSym((S)"l_quantity")));
	PROFILE(13, pfnCompress(w6, w4, t4)); P("size of w6: %lld\n", vn(w6));

	/* sub query */
	PROFILE(14, pfnCompress(f0, w4, t0));
	PROFILE(15, pfnGroup(f1, f0));
	PROFILE(16, pfnKeys(f2, f1));
	PROFILE(17, pfnIndex(f3, f0, f2));
	PROFILE(18, pfnIndexOf(f4, f3, f0));          // index
	PROFILE(19, pfnValues(f5, f1));
	PROFILE(20, pfnEachRight(f12, w6, f5, pfnIndex))
	PROFILE(21, pfnEach(f6, f12, pfnAvg));
	PROFILE(22, pfnRaze(f7, f6));
	PROFILE(23, pfnMul(f8, f7, literalF64(0.2))); // * 0.2
	PROFILE(24, pfnIndex(f9, f8, f4));
	PROFILE(25, pfnLt(f10, w6, f9));              // <

	PROFILE(26, pfnColumnValue(t5, a0, literalSym((S)"l_extendedprice")));
	PROFILE(27, pfnCompress(w13, w4, t5));
	PROFILE(28, pfnCompress(w15, f10, w13));
	P("size of w15: %lld\n", vn(w15));

	// project 
	PROFILE(29, pfnSum(p0, w15));
	PROFILE(30, pfnDiv(p1, p0, literalF64(7)));

	// return
	PROFILE(31, copyV(z0, literalSym((S)"avg_yearly")));
	PROFILE(32, pfnEnlist(z1, p1));
	PROFILE(33, pfnTable(z, z0, z1));

	gettimeofday(&tv1, NULL);
    P("Result (elapsed time %g ms)\n\n", calcInterval(tv0,tv1)/1000.0);
	printV(z);
	R 0;

}

L testTPCHQ17(){
    P("** Start simulation for TPC-H Query 17\n");
    initTableByName((S)"lineitem");
    initTableByName((S)"part");
    simulateQ17();
    P("** End Query 17\n");
    R 0;
}