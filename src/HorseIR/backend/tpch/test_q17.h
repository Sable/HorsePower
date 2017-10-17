
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
	// P("size: %lld, %lld, %lld\n", vn(w4), vn(w3), vn(t0));

	PROFILE(12, pfnColumnValue(t4, a0, literalSym((S)"l_quantity")));
	PROFILE(13, pfnCompress(w6, w4, t4)); P("size of w6: %lld\n", vn(w6));
	PROFILE(14, pfnAvg(w7, w6));
	PROFILE(15, pfnMul(w8, literalF64(0.2), w7));
	P("sub value:\n"); printV(w8);

	PROFILE(16, pfnLt(w9, t4, w8));
	PROFILE(17,  pfnAnd(w10, w4, w9));
	PROFILE(18, pfnCompress(w11, w10, t0));     // l_partkey (small)
	PROFILE(19, pfnMember(w12, w3, w11));

	PROFILE(20, pfnColumnValue(t5, a0, literalSym((S)"l_extendedprice")));
	PROFILE(21,  pfnCompress(w13, w10, t5));
	PROFILE(22,  pfnCompress(w15, w12, w13));
	P("size of w15: %lld\n", vn(w15));

	// project 
	PROFILE(23, pfnSum(p0, w15));
	PROFILE(24, pfnDiv(p1, p0, literalF64(7)));

	// return
	PROFILE(25, copyV(z0, literalSym((S)"avg_yearly")));
	PROFILE(26, pfnEnlist(z1, p1));
	PROFILE(27, pfnTable(z, z0, z1));

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