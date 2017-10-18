
V simulateQ18_sub(){
	V a0 = allocNode();
	V t0 = allocNode();  V t1 = allocNode();
	V w0 = allocNode();  V w1 = allocNode();  V w2 = allocNode();  V w3 = allocNode();
	V w4 = allocNode();  V w5 = allocNode();  V w6 = allocNode();  V w7 = allocNode();
	PROFILE(1, pfnLoadTable(a0, literalSym((S)"lineitem")));
	PROFILE(2, pfnColumnValue(t0, a0, literalSym((S)"l_orderkey")));
	PROFILE(3, pfnColumnValue(t1, a0, literalSym((S)"l_quantity")));
	P("type: %lld, len: %lld\n", vp(t0), vn(t0)); DOI(200, P(" %lld", vL(t0,i))) P("\n");
	pfnMax(w0, t0); printV(w0);
	pfnMin(w1, t0); printV(w1);
	getchar();
	PROFILE(4, pfnGroupBucket(w0, t0));
	PROFILE(5, pfnValues(w1, w0));
	PROFILE(6, pfnKeys(w2, w0));
	P("type: %lld, len: %lld\n", vp(w2), vn(w2)); DOI(200, P(" %lld", vL(w2,i))) P("\n");
	L maxS = 0, minS = 999999999999;
	DOI(vn(w1), {L tt=vn(vV(w1,i)); if(minS > tt) minS=tt; if(maxS < tt) maxS=tt; })
	P("minS = %lld, maxS = %lld\n", minS, maxS);    // [1, 7]
	PROFILE(7, pfnEachRight(w3, t1, w1, pfnIndex));
	PROFILE(8, pfnEach(w4, w3, pfnSum));
	PROFILE(9, pfnRaze(w5, w4));
	PROFILE(10, pfnGt(w6, w5, literalF64(300)));
	PROFILE(11, pfnCompress(w7, w6, w2));
	R w7;
}

L simulateQ18(){
	V sub = simulateQ18_sub();
	printV(sub);
	P("len of sub: %lld\n", vn(sub));
	R 0;
}

L testTPCHQ18(){
    P("** Start simulation for TPC-H Query 18\n");
    // initTableByName((S)"customer");
    // initTableByName((S)"orders");
    initTableByName((S)"lineitem");
    simulateQ18();
    P("** End Query 18\n");
    R 0;
}