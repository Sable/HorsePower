
V readPRData(){
	C DATA_PATH[] = "data/pl/pr_in10K.tbl";
	R readMatrix(DATA_PATH);
}

/*
	-- query 0: base
	SELECT
	    *
	FROM
	    myudf();
 */
V mypr_udf0(V web){
	V w0 = allocNode();  V w1 = allocNode();  V w2 = allocNode();  V w3 = allocNode();
	V m0 = allocNode();  V m1 = allocNode();  V m2 = allocNode();  V m3 = allocNode();
	V z0 = allocNode();  V z1 = allocNode();  V z  = allocNode();
	if(!isOptimized){
	    PROFILE(1, pfnEach(w0, web, pfnSum));
	    PROFILE(2, pfnFlip(w1, web));
	    PROFILE(3, pfnEach(w2, w1, pfnSum));
	    PROFILE(4, pfnRaze(m0, w0));
	    PROFILE(5, pfnRaze(m1, w2));
	}
	else {
		PROFILE(1, optLoopFusionPR_1(m0,m1,vn(web),web));
	}
	PROFILE(6, pfnOrderBy(m2, m1, literalBool(1)));
	PROFILE(7, pfnLen(w3, m0));
	PROFILE(8, pfnRange(m3, w3));
	S group1[] = { (S)"Url", (S)"Outbound", (S)"Inbound", (S)"Rank" };
	PROFILE(9, copyV(z0, literalSymVector(4, group1)))
	V group2[] = { m3, m0, m1, m2};
	PROFILE(10, pfnList(z1, 4, group2));
	PROFILE(11, pfnTable(z,z0,z1));
	R z;
}

V mypr_udf1(V web){
	R 0;
}

V mypr_udf2(V web){
	R 0;
}

V mypr_udf3(V web){
	R 0;
}

E runPRQuery(L id, V web){
	struct timeval tv0, tv1;
    gettimeofday(&tv0, NULL);
    V z;
    switch(id){
    	case 0: z = mypr_udf0(web); break;
    	case 1: z = mypr_udf1(web); break;
    	case 2: z = mypr_udf2(web); break;
    	case 3: z = mypr_udf3(web); break;
	}
	gettimeofday(&tv1, NULL);
	E elapsed = calcInterval(tv0,tv1)/1000.0;
    P("The elapsed time (ms): %g\n", elapsed);
    printTablePretty(z, 10);
    R elapsed;
}

L runPR(L sid){
	P("** Start simulation for Page-Rank\n");
	V web = readPRData();
	L cur = getHeapOffset();
    DOI(TEST_RUNS, {setHeapOffset(cur); times[i]=runPRQuery(sid, web);})
    P("** End Black-Scholes\n");
}
