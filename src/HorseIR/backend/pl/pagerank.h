
L readTablePR(){
    C CSV_LINE[]  = "data/pl/pr_1M.tbl";
    L TYPE_LINE[] = {H_L, H_L, H_L};
    const L NUM_COL_LINE = 3;
    Q SYM_LIST_LINE[NUM_COL_LINE];
    const C* PRE_DEFINED[] = {
        "urlid", "outbound", "inbound"
    };

    initTable(NUM_COL_LINE, PRE_DEFINED, SYM_LIST_LINE);
    V tableRegion = readCSV(CSV_LINE, NUM_COL_LINE, TYPE_LINE, SYM_LIST_LINE);
    registerTable((S)"pagerank", tableRegion);
    R 0;
}

V mypr_compute(V inbound){
    V urlrank = allocNode();  V size = allocNode();
    V rankid  = allocNode();  V r = allocNode();
    PROFILE(1, pfnOrderBy(r, inbound, literalBool(0)));
    PROFILE(2, pfnLen(size, inbound));
    PROFILE(3, pfnRange(rankid , size));
    if(!isOptimized){
        PROFILE(4, pfnRange(urlrank, size));
        PROFILE(5, pfnIndexA(urlrank,r,rankid));
    }
    else {
        PROFILE(4, optLoopFusionPR_6(urlrank,vn(r),r,rankid));
    }
    R urlrank;
}

/*
    -- query 0: base
    SELECT
        *
    FROM
        mypr((select * from pagerank));
 */
V mypr_udf0(L id, V urlid, V outbound, V inbound){
    V z0 = allocNode();  V z1 = allocNode();  V z  = allocNode();
    V urlrank = mypr_compute(inbound);
    S group1[] = { (S)"urlid", (S)"outbound", (S)"inbound", (S)"urlrank" };
    PROFILE(1, copyV(z0, literalSymVector(4, group1)));
    V group2[] = { urlid, outbound, inbound, urlrank };
    PROFILE(2,pfnList(z1, 4, group2));
    PROFILE(3,pfnTable(z,z0,z1));
    R z;
}
/*
    -- query 1:
    SELECT
        *
    FROM
        mypr((select * from pagerank));
    WHERE
        inbound > 40;
 */
V mypr_udf1(L id, V urlid, V outbound, V inbound){
    V w0 = allocNode();
    V m0 = allocNode();  V m1 = allocNode();  V m2 = allocNode();  V m3 = allocNode();
    V z0 = allocNode();  V z1 = allocNode();  V z  = allocNode();
    V urlrank = mypr_compute(inbound);
    if(1==id){
        PROFILE(10, pfnGt(w0, inbound, literalI64(40)));
    }
    else if(2==id){
        PROFILE(10, pfnLeq(w0, inbound, literalI64(40)));
    }
    else if(3==id){
        PROFILE(10, pfnGt(w0, inbound, literalI64(30)));
    }
    PROFILE(11, pfnCompress(m0, w0, urlid));
    PROFILE(12, pfnCompress(m1, w0, outbound));
    PROFILE(13, pfnCompress(m2, w0, inbound));
    PROFILE(14, pfnCompress(m3, w0, urlrank));
    S group1[] = { (S)"urlid", (S)"outbound", (S)"inbound", (S)"urlrank" };
    PROFILE(1, copyV(z0, literalSymVector(4, group1)));
    V group2[] = { m0, m1, m2, m3 };
    PROFILE(2,pfnList(z1, 4, group2));
    PROFILE(3,pfnTable(z,z0,z1));
    P("pr1 selectivity %lld: %lf%%\n", id, (vn(m0)*100.0)/vn(w0));
    R z;
}

/*
    -- query 2:
    SELECT
        urlid, inbound
    FROM
        mypr((select * from pagerank));
    WHERE
        inbound > 40;
 */
V mypr_udf2(L id, V urlid, V outbound, V inbound){
    V w0 = allocNode();
    V m0 = allocNode();  V m1 = allocNode();
    V z0 = allocNode();  V z1 = allocNode();  V z  = allocNode();
    if(!isOptimized){
        V urlrank = mypr_compute(inbound);
    }
    if(1==id){
        PROFILE(10, pfnGt(w0, inbound, literalI64(40)));
    }
    else if(2==id){
        PROFILE(10, pfnLeq(w0, inbound, literalI64(40)));
    }
    else if(3==id){
        PROFILE(10, pfnGt(w0, inbound, literalI64(30)));
    }
    PROFILE(11, pfnCompress(m0, w0, urlid));
    PROFILE(13, pfnCompress(m1, w0, inbound));
    S group1[] = { (S)"urlid", (S)"inbound" };
    PROFILE(1, copyV(z0, literalSymVector(2, group1)));
    V group2[] = { m0, m1 };
    PROFILE(2,pfnList(z1, 2, group2));
    PROFILE(3,pfnTable(z,z0,z1));
    P("pr2 selectivity %lld: %lf%%\n", id, (vn(m0)*100.0)/vn(w0));
    R z;
}

/*
    -- query 3
    SELECT
        urlid, inbound
    FROM
        mypr((select * from pagerank))
    WHERE
        urlrank < 10000;      ==>  urlrank >= 10000;     ==>   urlrank <= 500000;
 */
V mypr_udf3(L id, V urlid, V outbound, V inbound){
    V w0 = allocNode();
    V m0 = allocNode();  V m1 = allocNode();
    V z0 = allocNode();  V z1 = allocNode();  V z  = allocNode();
    V urlrank = mypr_compute(inbound);
    if(1==id){
        PROFILE(10, pfnLt(w0, urlrank, literalI64(10000)));
    }
    else if(2==id){
        PROFILE(10, pfnGeq(w0, urlrank, literalI64(10000)));
    }
    else if(3==id){
        PROFILE(10, pfnLeq(w0, urlrank, literalI64(500000)));
    }
    PROFILE(11, pfnCompress(m0, w0, urlid));
    PROFILE(13, pfnCompress(m1, w0, inbound));
    S group1[] = { (S)"urlid", (S)"inbound" };
    PROFILE(1, copyV(z0, literalSymVector(2, group1)));
    V group2[] = { m0, m1 };
    PROFILE(2,pfnList(z1, 2, group2));
    PROFILE(3,pfnTable(z,z0,z1));
    P("pr3 selectivity %lld: %lf%%\n", id, (vn(m0)*100.0)/vn(w0));
    R z;
}

E runPRQuery(L id){
    V a0 = allocNode();
    V urlid = allocNode();  V outbound = allocNode();  V inbound = allocNode();
    struct timeval tv0, tv1;
    gettimeofday(&tv0, NULL);
    PROFILE( 1, pfnLoadTable(a0, literalSym((S)"pagerank")));
    PROFILE( 2, pfnColumnValue(urlid    , a0, literalSym((S)"urlid")));
    PROFILE( 3, pfnColumnValue(outbound , a0, literalSym((S)"outbound")));
    PROFILE( 4, pfnColumnValue(inbound  , a0, literalSym((S)"inbound")));
    V z;
    switch(id){
        case 0: z = mypr_udf0(0,urlid,outbound,inbound); break;
        case 1: z = mypr_udf1(1,urlid,outbound,inbound); break;
        case 2: z = mypr_udf2(1,urlid,outbound,inbound); break;
        case 3: z = mypr_udf3(1,urlid,outbound,inbound); break;
        case 4: z = mypr_udf1(2,urlid,outbound,inbound); break;
        case 5: z = mypr_udf2(2,urlid,outbound,inbound); break;
        case 6: z = mypr_udf3(2,urlid,outbound,inbound); break;
        case 7: z = mypr_udf1(3,urlid,outbound,inbound); break;
        case 8: z = mypr_udf2(3,urlid,outbound,inbound); break;
        case 9: z = mypr_udf3(3,urlid,outbound,inbound); break;
        default: P("Invalid query id (%lld) in Page-Rank\n", id); exit(99);
    }
    gettimeofday(&tv1, NULL);
    E elapsed = calcInterval(tv0,tv1)/1000.0;
    P("The elapsed time (ms): %g\n", elapsed);
    printTablePretty(z, 10);
    R elapsed;
}

L runPR(L sid){
    P("** Start simulation for Page-Rank\n");
    readTablePR();
    L cur = getHeapOffset();
    DOI(TEST_RUNS, {setHeapOffset(cur); times[i]=runPRQuery(sid);})
    P("** End Black-Scholes\n");
}
