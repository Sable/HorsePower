
V simulateQ16_sub(){
	V a0 = allocNode();
	V t0 = allocNode();  V t1 = allocNode();
	V w0 = allocNode();  V w1 = allocNode();
	PROFILE(1, pfnLoadTable(a0, literalSym((S)"supplier")));
	PROFILE(2, pfnColumnValue(t0, a0, literalSym((S)"s_comment")));
	PROFILE(3, pfnColumnValue(t1, a0, literalSym((S)"s_suppkey")));
	PROFILE(4, pfnLike(w0, t0, literalString((S)"\%Customer\%Complaints\%")));
	PROFILE(5, pfnCompress(w1, w0, t1));
	R w1;
}

E simulateQ16(){
	V a0 = allocNode();  V a1 = allocNode();
	V t0 = allocNode();  V t1 = allocNode();  V t2 = allocNode();  V t3 = allocNode();
	V t4 = allocNode();  V t5 = allocNode();
	V w0 = allocNode();  V w1 = allocNode();  V w2 = allocNode();  V w3 = allocNode();
	V w4 = allocNode();  V w5 = allocNode();  V w6 = allocNode();  V w7 = allocNode();
	V w8 = allocNode();  V w9 = allocNode();  V w10= allocNode();  V w11= allocNode();
	V w12= allocNode();  V w13= allocNode();
	V g0 = allocNode();  V g1 = allocNode();  V g2 = allocNode();  V g3 = allocNode();
	V g4 = allocNode();  V g5 = allocNode();  V g6 = allocNode();  V g7 = allocNode();
	V g8 = allocNode();  V g9 = allocNode();  V g10= allocNode();  V g11= allocNode();
	V g12= allocNode();
	V d0 = allocNode();  V d1 = allocNode();
	V m0 = allocNode();  V m1 = allocNode();  V m2 = allocNode();  V m3 = allocNode();
	V z0 = allocNode();  V z1 = allocNode();  V z  = allocNode();
	V f0 = allocNode();  V f1 = allocNode();

	struct timeval tv0, tv1;
    gettimeofday(&tv0, NULL);

	PROFILE( 1, pfnLoadTable(a0, literalSym((S)"partsupp")));
	PROFILE( 2, pfnLoadTable(a1, literalSym((S)"part")));
	PROFILE( 3, pfnColumnValue(t0, a1, literalSym((S)"p_brand")));
	PROFILE( 4, pfnColumnValue(t1, a1, literalSym((S)"p_type")));
	PROFILE( 5, pfnColumnValue(t2, a1, literalSym((S)"p_size")));
	PROFILE( 6, pfnColumnValue(t3, a1, literalSym((S)"p_partkey")));
	PROFILE( 7, pfnColumnValue(t4, a0, literalSym((S)"ps_partkey")));  // fkey
	PROFILE( 8, pfnColumnValue(t5, a0, literalSym((S)"ps_suppkey")));
	PROFILE( 9, pfnNeq(w0, t0, literalSym((S)"Brand#45")));
	PROFILE(10, pfnLike(w1, t1, literalString((S)"MEDIUM POLISHED\%")));
	PROFILE(11, pfnNot(w2, w1));
	L group1[] = {49, 14, 23, 45, 19, 3, 36, 9};
	PROFILE(12, pfnMember(w3, literalI64Vector(8, group1), t2));
	V sub = simulateQ16_sub();
	PROFILE(13, pfnMember(w4, sub, t5));
	PROFILE(14, pfnNot(w5, w4));     // ps

	// and
	PROFILE(15, pfnAnd(w6, w0, w2));
	PROFILE(16, pfnAnd(w7, w6, w3)); // p

	// join
	PROFILE(17, pfnValues(w8, t4));
	PROFILE(18, pfnCompress(w9, w5, w8));
	PROFILE(19, pfnIndex(w10, w7, w9));
	PROFILE(20, pfnCompress(w11, w10, w9));  // index for part
	PROFILE(21, pfnWhere(w12, w5));
	PROFILE(22, pfnCompress(w13, w10, w12)); // index for partsupp

	// P("len of w11: %lld\n", vn(w11));  // 118274
	// group by
	PROFILE(23, pfnIndex(g0, t0, w11)); // p_brand
	PROFILE(24, pfnIndex(g1, t1, w11)); // p_type
	PROFILE(25, pfnIndex(g2, t2, w11)); // p_size
	PROFILE(26, pfnIndex(g3, t5, w13)); // ps_suppkey

	V group2[] = {g0, g1, g2};
	PROFILE(27, pfnList(g4, 3, group2));
	PROFILE(28, pfnGroup(g5, g4));   // <-- slow
	PROFILE(29, pfnValues(g6, g5));  // index
	PROFILE(30, pfnKeys(g7, g5));    // index
	// P("len of g7: %lld\n", vn(g7));  // should be 18314

	if(!isOptimized){
		PROFILE(99, pfnEachRight(f0, g3, g6, pfnIndex));  // <-- very slow
		PROFILE(99, pfnEach(f1, f0, pfnUnique));          // <-- very slow
		PROFILE(31, pfnEach(g8, f1, pfnLen)); // unique count
		PROFILE(32, pfnRaze(g9, g8));         // supplier_cnt
	}
	else {
		PROFILE(31, optLoopFusionQ16_1(g9, vn(g6), g3, g6));
	}
	PROFILE(33, pfnIndex(g10, g0, g7));   // p_brand
	PROFILE(34, pfnIndex(g11, g1, g7));   // p_type
	PROFILE(35, pfnIndex(g12, g2, g7));   // p_size

	// order by	
	V group3[] = {g9, g10, g11, g12};
	PROFILE(36, pfnList(d0, 4, group3));
	B group4[] = {0 ,  1 ,  1 ,  1};
	PROFILE(37, pfnOrderBy(d1, d0, literalBoolVector(4, group4))); // <-- slow
	

	// materialization
	PROFILE(38, pfnIndex(m0, g10, d1));
	PROFILE(39, pfnIndex(m1, g11, d1));
	PROFILE(40, pfnIndex(m2, g12, d1));
	PROFILE(41, pfnIndex(m3, g9 , d1));

	// return
	S strs[] = {(S)"p_brand", (S)"p_type", (S)"p_size", (S)"supplier_cnt" };
    PROFILE(42, copyV(z0,literalSymVector(4,strs)));
    V group5[] = {m0,m1,m2,m3};
    PROFILE(43, pfnList(z1,4,group5));
    PROFILE(44, pfnTable(z,z0,z1));

	gettimeofday(&tv1, NULL);
    E elapsed = calcInterval(tv0,tv1)/1000.0;
    P("The elapsed time (ms): %g\n\n", elapsed);
    printTablePretty(z, 10);  // limit 35
    P("size of z: row = %lld, col = %lld\n", tableRow(z), tableCol(z));
    R elapsed;
}

L testTPCHQ16(){
    P("** Start simulation for TPC-H Query 16\n");
    initTableByName((S)"partsupp");
    initTableByName((S)"part");
    initTableByName((S)"supplier");
    PROFILE(91, pfnAddFKey(literalSym((S)"part")    , literalSym((S)"p_partkey"),\
    	                   literalSym((S)"partsupp"), literalSym((S)"ps_partkey")));
    L cur = getHeapOffset();
    DOI(TEST_RUNS, {setHeapOffset(cur); times[i]=simulateQ16();})
    P("** End Query 16\n");
    R 0;
}