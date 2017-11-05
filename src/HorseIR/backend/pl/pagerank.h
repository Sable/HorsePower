
V readPRData(){
    C DATA_PATH[] = "data/pl/pr_in1K.tbl";
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
    V w4 = allocNode();  V w5 = allocNode();
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
    PROFILE(6, pfnLen(w3, m0));
    PROFILE(7, pfnRange(m3, w3));
    PROFILE(8, pfnOrderBy(w4, m1, literalBool(0)));
    if(!isOptimized){
        PROFILE(9, pfnLen(w5, w4));
        PROFILE(10,pfnRange(m2, w5));
        PROFILE(11,pfnIndexA(m2,w4,m3));
    }
    else {
        PROFILE(9, optLoopFusionPR_6(m2,vn(w4),w4,m3));
    }

    S group1[] = { (S)"urlid", (S)"outbound", (S)"inbound", (S)"urlrank" };
    PROFILE(12, copyV(z0, literalSymVector(4, group1)));
    V group2[] = { m3, m0, m1, m2 };
    PROFILE(13,pfnList(z1, 4, group2));
    PROFILE(14,pfnTable(z,z0,z1));
    R z;
}
/*
    -- query 1:
    SELECT
        urlid, outbound, inbound
    FROM
        myudf()
    WHERE
        inbound > 70;
 */

V mypr_udf1(V web){
    V w0 = allocNode();  V w1 = allocNode();  V w2 = allocNode();  V w3 = allocNode();
    V m0 = allocNode();  V m1 = allocNode();  V m2 = allocNode();
    V z0 = allocNode();  V z1 = allocNode();  V z  = allocNode();
    if(!isOptimized){
        PROFILE(1, optLoopFusionPR_1(w0,w1,vn(web),web));
        PROFILE(2, pfnOrderBy(w2, w1, literalBool(0)));
        PROFILE(3, pfnGt(w3, w1, literalI64(70)));
        PROFILE(4, pfnWhere(m0, w3));
        PROFILE(5, pfnCompress(m1, w3, w0));
        PROFILE(6, pfnCompress(m2, w3, w1));
    }
    else {
        PROFILE(1, optLoopFusionPR_2(m0,m1,m2,vn(web),web));
        /* skip w2 (rank) */
    }
    S group1[] = { (S)"urlid", (S)"outbound", (S)"inbound" };
    PROFILE(7, copyV(z0, literalSymVector(3, group1)));
    V group2[] = { m0, m1, m2 };
    PROFILE(8, pfnList(z1, 3, group2));
    PROFILE(9, pfnTable(z,z0,z1));
    R z;
}

/*
    -- query 2:
    SELECT
        urlid, urlrank
    FROM
        myudf()
    WHERE
        inbound > 70;
 */
V mypr_udf2(V web){
    V w0 = allocNode();  V w1 = allocNode();  V w2 = allocNode();  V w3 = allocNode();
    V w4 = allocNode();  V w5 = allocNode();  V w6 = allocNode();  V w7 = allocNode();
    V m0 = allocNode();  V m1 = allocNode();
    V z0 = allocNode();  V z1 = allocNode();  V z  = allocNode();
    PROFILE(1, optLoopFusionPR_1(w0,w1,vn(web),web));
    PROFILE(2, pfnOrderBy(w2, w1, literalBool(0)));
    PROFILE(3, pfnLen(w3, w1));
    PROFILE(4, pfnRange(w4, w3));
    if(!isOptimized){
        PROFILE(5, pfnLen(w5, w2));
        PROFILE(6, pfnRange(w6, w5));
        PROFILE(7, pfnIndexA(w6,w2,w4));
    }
    else {
        PROFILE(5, optLoopFusionPR_6(w6,vn(w2),w2,w4));
    }
    PROFILE(8, pfnGt(w7, w1, literalI64(70)));
    PROFILE(9, pfnCompress(m0, w7, w4));
    PROFILE(10,pfnCompress(m1, w7, w6));
    S group1[] = { (S)"urlid", (S)"urlrank" };
    PROFILE(11,copyV(z0, literalSymVector(2, group1)));
    V group2[] = { m0, m1 };
    PROFILE(12,pfnList(z1, 2, group2));
    PROFILE(13,pfnTable(z,z0,z1));
    R z;
}

/*
    -- query 3
    SELECT
        urlid, outbound, inbound
    FROM
        myudf()
    WHERE
        inbound > 70
        AND urlrank < 10;
 */
V mypr_udf3(V web){
    V w0 = allocNode();  V w1 = allocNode();  V w2 = allocNode();  V w3 = allocNode();
    V w4 = allocNode();  V w5 = allocNode();  V w6 = allocNode();  V w7 = allocNode();
    V w8 = allocNode();
    V m0 = allocNode();  V m1 = allocNode();  V m2 = allocNode();
    V z0 = allocNode();  V z1 = allocNode();  V z  = allocNode();
    PROFILE(1, optLoopFusionPR_1(w0,w1,vn(web),web));
    PROFILE(2, pfnOrderBy(w2, w1, literalBool(0)));
    if(!isOptimized){
        PROFILE(3, pfnLen(w3, w2));
        PROFILE(4, pfnRange(w4, w3));
        initV(w5, H_L, vn(w2));
        PROFILE(5, pfnIndexA(w5,w2,w4));
        PROFILE(6, pfnGt(w6, w1, literalI64(70)));
        PROFILE(7, pfnLt(w7, w5, literalI64(10)));
        PROFILE(8, pfnAnd(w8,w6,w7));
    }
    else {
        PROFILE(1, optLoopFusionPR_4(w5,vn(w2),w2));
        PROFILE(4, optLoopFusionPR_5(w8,vn(w1),w1,w5));
    }
    PROFILE(9, pfnWhere(m0, w8));
    PROFILE(10,pfnCompress(m1, w8, w0));
    PROFILE(10,pfnCompress(m2, w8, w1));
    // PROFILE(11,pfnCompress(m2, w8, w5));  // urlrank
    S group1[] = { (S)"urlid", (S)"outbound", (S)"inbound" };
    PROFILE(7, copyV(z0, literalSymVector(3, group1)));
    V group2[] = { m0, m1, m2};
    PROFILE(8, pfnList(z1, 3, group2));
    PROFILE(9, pfnTable(z,z0,z1));
    R z;
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
