
/* Should be included after tpch */

L readTableBS(){
    C CSV_LINE[] = "data/pl/in_1M.tbl";
    L TYPE_LINE[]  = {H_E, H_E, H_E, H_E,
                      H_E, H_E, H_C, H_E, H_E};
    const L NUM_COL_LINE = 9;
    Q SYM_LIST_LINE[NUM_COL_LINE];
    const C* PRE_DEFINED[] = {
        "sptprice", "strike", "rate", "divq",
        "volatility", "time", "optiontype", "divs", "dgrefval"
    };

    initTable(NUM_COL_LINE, PRE_DEFINED, SYM_LIST_LINE);
    V tableRegion = readCSV(CSV_LINE, NUM_COL_LINE, TYPE_LINE, SYM_LIST_LINE);
    registerTable((S)"blackscholes", tableRegion);
    R 0;
}

L CNDF(V OutputX, V InputX, V invs2xPI){
    V sign = allocNode(); V xInput = allocNode(); V expValues = allocNode(); V xNPrimeofX = allocNode();
    V xK2  = allocNode(); V xK2_2  = allocNode(); V xK2_3 = allocNode(); V xK2_4 = allocNode(); V xK2_5 = allocNode();
    V xLocal = allocNode(); V xLocal_1 = allocNode(); V xLocal_2 = allocNode(); V xLogTerm = allocNode();
    V t0 = allocNode(); V t1 = allocNode(); V t2 = allocNode(); V t3 = allocNode();
    V t4 = allocNode(); V t5 = allocNode(); V t6 = allocNode(); V t7 = allocNode();
    V t8 = allocNode(); V t9 = allocNode(); V t10 = allocNode(); V t11 = allocNode();
    V t12 = allocNode(); V t13 = allocNode(); V t14 = allocNode(); V t15 = allocNode(); V t16 = allocNode();

    PROFILE(1, pfnLt(sign, InputX, literalF64(0)));
    PROFILE(2, pfnAbs(xInput, InputX));
    PROFILE(3, pfnMul(t0, InputX, InputX));
    PROFILE(3, pfnMul(t1, literalF64(0.5), t0));
    PROFILE(3, pfnNeg(t2, t1));
    PROFILE(3, pfnExp(expValues, t2));
    PROFILE(4, pfnMul(xNPrimeofX, invs2xPI, expValues));
    PROFILE(5, pfnMul(t3, literalF64(0.2316419), xInput));
    PROFILE(5, pfnPlus(t4, literalF64(1), t3));
    PROFILE(5, pfnDiv(xK2, literalF64(1), t4));
    PROFILE(6, pfnMul(xK2_2, xK2, xK2));
    PROFILE(7, pfnMul(xK2_3, xK2, xK2_2));
    PROFILE(8, pfnMul(xK2_4, xK2, xK2_3));
    PROFILE(9, pfnMul(xK2_5, xK2, xK2_4));
    PROFILE(10, pfnMul(xLocal_1, xK2, literalF64(0.319381530)));
    PROFILE(11,pfnMul(t5, xK2_2, literalF64(-0.356563782)));
    PROFILE(11,pfnMul(t6, xK2_3, literalF64(1.781477937)));
    PROFILE(11,pfnMul(t7, xK2_4, literalF64(-1.821255978)));
    PROFILE(11,pfnMul(t8, xK2_5, literalF64(1.330274429)));
    PROFILE(11,pfnPlus(t9, t5, t6));
    PROFILE(11,pfnPlus(t10, t9, t7));
    PROFILE(11,pfnPlus(xLocal_2, t10, t8));
    PROFILE(12,pfnPlus(t11,xLocal_2,xLocal_1));
    PROFILE(12,pfnMul(t12,xNPrimeofX,t11));
    PROFILE(12,pfnMinus(xLocal, literalF64(1.0),t12));
    PROFILE(13,pfnNot(t13,sign));
    PROFILE(13,pfnMul(t14,t13,xLocal));
    PROFILE(13,pfnMinus(t15,literalF64(1.0),xLocal));
    PROFILE(13,pfnMul(t16,sign,t15));
    PROFILE(13,pfnPlus(OutputX,t16,t14));
    R 0;
}

V BlkSchls(V sptprice, V strike, V rate, V volatility, V time, V otypeC){
    V xSqrtTime = allocNode(); V xLogTerm = allocNode(); V xRiskFreeRate = allocNode(); V otype = allocNode();
    V xDen = allocNode(); V xPowerTerm = allocNode();  V xD1 = allocNode(); V d1 = allocNode();
    V xD2 = allocNode(); V d2 = allocNode(); V invs2xPI = allocNode(); V NofXd1 = allocNode(); V NofXd2 = allocNode();
    V FutureValueX = allocNode(); V OptionPrice = allocNode();
    V t0 = allocNode(); V t1 = allocNode(); V t2 = allocNode(); V t3 = allocNode(); V t4 = allocNode();
    V t5 = allocNode(); V t6 = allocNode(); V t7 = allocNode(); V t8 = allocNode(); V t9 = allocNode();
    V t10 = allocNode(); V t11 = allocNode(); V t12 = allocNode(); V t13 = allocNode(); V t14 = allocNode();
    V t15 = allocNode(); V t16 = allocNode(); V t17 = allocNode();
    V z0 = allocNode(); V z1 = allocNode(); V z = allocNode(); 
    
    PROFILE(1, pfnPower(xSqrtTime, time, literalF64(0.5)));
    PROFILE(2, pfnDiv(t0, sptprice, strike));
    PROFILE(2, pfnLog(xLogTerm, t0));
    PROFILE(3, copyV(xRiskFreeRate, rate));
    PROFILE(4, pfnEq(otype, otypeC, literalChar('P'))); //
    PROFILE(5, pfnMul(xDen, volatility, xSqrtTime));
    // PROFILE(91, optLoopFusionBS_1(xDen, vn(volatility), volatility, time)); /* LINE 1 and 5 */
    PROFILE(6, pfnMul(t1, volatility, literalF64(0.5)));
    PROFILE(6, pfnMul(xPowerTerm, volatility, t1));
    PROFILE(7, pfnPlus(t2, xRiskFreeRate, xPowerTerm));
    PROFILE(7, pfnMul(t3, time, t2));
    PROFILE(7, pfnPlus(t4, xLogTerm, t3));
    // PROFILE(92, optLoopFusionBS_2(t4, vn(sptprice), sptprice, strike, time, rate, volatility));
    PROFILE(7, pfnDiv(xD1, t4, xDen));
    PROFILE(7, copyV(d1, xD1));
    PROFILE(8, pfnMinus(xD2, xD1, xDen));
    PROFILE(8, copyV(d2,xD2));
    PROFILE(9, copyV(invs2xPI, literalF64(0.39894228040143270286)));
    PROFILE(10,CNDF(NofXd1, d1, invs2xPI));
    PROFILE(11,CNDF(NofXd2, d2, invs2xPI));
    PROFILE(12,pfnMul(t5,rate,time));
    PROFILE(12,pfnNeg(t6,t5));
    PROFILE(12,pfnExp(t7,t6));
    PROFILE(12,pfnMul(FutureValueX,strike,t7));
    PROFILE(13,pfnMinus(t8,literalF64(1.0), NofXd1));
    PROFILE(13,pfnMul(t9,sptprice,t8));
    PROFILE(13,pfnMinus(t10,literalF64(1.0), NofXd2));
    PROFILE(13,pfnMul(t11,FutureValueX,t10));
    PROFILE(13,pfnMinus(t12,t11,t9));
    PROFILE(13,pfnMul(OptionPrice, otype, t12));
    PROFILE(14,pfnMul(t13,FutureValueX,NofXd2));
    PROFILE(14,pfnMul(t14,sptprice,NofXd1));
    PROFILE(14,pfnMinus(t15,t14,t13));
    PROFILE(14,pfnNot(t16,otype));
    PROFILE(14,pfnMul(t17,t16,t15));
    PROFILE(14,pfnPlus(OptionPrice,OptionPrice,t17));
    R OptionPrice;
}

/*
 optionprice: [0, 28.643646769343334]
 #(> 5)     : 562036

*/

/*
    -- base 0
    SELECT
        *
    FROM
        myudf((select * from blackscholes))
 */
V mybs_udf0(V sptprice, V strike, V rate, V divq, V volatility, V time, V optiontype, V divs, V dgrefval){
    P("Entering mybs_udf0\n");
    V z0 = allocNode(); V z1 = allocNode(); V z = allocNode();
    V optionprice = BlkSchls(sptprice, strike, rate, volatility, time, optiontype);
    S group1[] = {(S)"optiontype", (S)"sptprice", (S)"optionprice"};
    PROFILE(1, copyV(z0, literalSymVector(3, group1)));
    V group2[] = {optiontype, sptprice, optionprice};
    PROFILE(2, pfnList(z1, 3, group2));
    PROFILE(3, pfnTable(z, z0, z1));
    R z;
}

/*
    -- query 2: filter
    SELECT
        *
    FROM
        myudf((select * from blackscholes))
    WHERE
        sptprice BETWEEN 51 AND 100
 */

V mybs_udf1(L id, V sptprice, V strike, V rate, V divq, V volatility, V time, V optiontype, V divs, V dgrefval){
    P("Entering mybs_udf1\n");
    V z0 = allocNode(); V z1 = allocNode(); V z  = allocNode();
    V w0 = allocNode(); V w1 = allocNode(); V w2 = allocNode();
    V m0 = allocNode(); V m1 = allocNode(); V m2 = allocNode();
    if(!isOptimized){
        V optionprice = BlkSchls(sptprice, strike, rate, volatility, time, optiontype);
        if(id == 1){
            PROFILE(10, pfnGeq(w0, sptprice, literalF64(50)));
            PROFILE(11, pfnLeq(w1, sptprice, literalF64(100)));
            PROFILE(12, pfnAnd(w2, w0, w1));
        }
        else if(id == 4){
            PROFILE(10, pfnLt(w0, sptprice, literalF64(50)));
            PROFILE(11, pfnGt(w1, sptprice, literalF64(100)));
            PROFILE(12, pfnOr(w2, w0, w1));
        }
        else if(id == 7){
            PROFILE(10, pfnGeq(w0, sptprice, literalF64(51)));
            PROFILE(11, pfnLeq(w1, sptprice, literalF64(100)));
            PROFILE(12, pfnAnd(w2, w0, w1));
        }
        PROFILE(12, pfnCompress(m0, w2, optiontype));
        PROFILE(13, pfnCompress(m1, w2, sptprice));
        PROFILE(14, pfnCompress(m2, w2, optionprice));
    }
    else {
        V w1 = allocNode();  V w2 = allocNode();  V w3 = allocNode();
        V w4 = allocNode();  V w5 = allocNode();  V w6 = allocNode();
        PROFILE(10, optLoopFusionBS_3(w0, vn(sptprice), sptprice, id));
        PROFILE(11, pfnCompress(w1, w0, sptprice));
        PROFILE(12, pfnCompress(w2, w0, strike));
        PROFILE(13, pfnCompress(w3, w0, rate));
        PROFILE(14, pfnCompress(w4, w0, volatility));
        PROFILE(15, pfnCompress(w5, w0, time));
        PROFILE(16, pfnCompress(w6, w0, optiontype));
        V optionprice = BlkSchls(w1,w2,w3,w4,w5,w6);
        copyV(m0, w6);
        copyV(m1, w1);
        copyV(m2, optionprice);
        P("udf1 selectivity %lld: %lf%%\n", id,(vn(w1)*100.0)/vn(w0));
    }
    S group1[] = {(S)"optiontype", (S)"sptprice", (S)"optionprice"};
    PROFILE(1, copyV(z0, literalSymVector(3, group1)));
    V group2[] = {m0, m1, m2};
    PROFILE(2, pfnList(z1, 3, group2));
    PROFILE(3, pfnTable(z, z0, z1));
    R z;
}

/*
    -- query 2: return all //change to query 1
    SELECT
        optiontype, sptprice
    FROM
        myudf((select * from blackscholes))
    WHERE
        sptprice BETWEEN 51 AND 100
 */
V mybs_udf2(L id, V sptprice, V strike, V rate, V divq, V volatility, V time, V optiontype, V divs, V dgrefval){
    P("Entering mybs_udf2\n");
    V z0 = allocNode(); V z1 = allocNode(); V z  = allocNode();
    V w0 = allocNode();
    V m0 = allocNode(); V m1 = allocNode();
    if(!isOptimized){
        V w1 = allocNode();  V w2 = allocNode();
        V optionprice = BlkSchls(sptprice, strike, rate, volatility, time, optiontype);
        if(id == 2){
            PROFILE(10, pfnGeq(w0, sptprice, literalF64(50)));
            PROFILE(11, pfnLeq(w1, sptprice, literalF64(100)));
            PROFILE(12, pfnAnd(w2, w0, w1));
        }
        else if(id == 5){
            PROFILE(10, pfnLt(w0, sptprice, literalF64(50)));
            PROFILE(11, pfnGt(w1, sptprice, literalF64(100)));
            PROFILE(12, pfnOr(w2, w0, w1));
        }
        else if(id == 8){
            PROFILE(10, pfnGeq(w0, sptprice, literalF64(51)));
            PROFILE(11, pfnLeq(w1, sptprice, literalF64(100)));
            PROFILE(12, pfnAnd(w2, w0, w1));
        }
        PROFILE(13, pfnCompress(m0, w2, optiontype));
        PROFILE(14, pfnCompress(m1, w2, sptprice));
    }
    else {
        PROFILE(10, optLoopFusionBS_3(w0, vn(sptprice), sptprice, id));
        PROFILE(16, pfnCompress(m0, w0, optiontype));
        PROFILE(16, pfnCompress(m1, w0, sptprice));
        P("udf2 selectivity %lld: %lf%%\n", id,(vn(m0)*100.0)/vn(w0));
    }
    S group1[] = {(S)"optiontype", (S)"sptprice"};
    PROFILE(1, copyV(z0, literalSymVector(2, group1)));
    V group2[] = {m0, m1};
    PROFILE(2, pfnList(z1, 2, group2));
    PROFILE(3, pfnTable(z, z0, z1));
    R z;
}

// optionprice after sptprice between 50 and 100 : [5.0388  48434816362, 28.643646769343334]

/*
    -- query 3:
    SELECT
        optiontype, sptprice
    FROM
        myudf((select * from blackscholes))
    WHERE
    -- 1st version
        sptprice BETWEEN 51 AND 100   ==>  sptprice < 51 OR sptprice > 100   //no records returned
        AND optionprice > 15          ==>  OR optionprice <= 15
    -- 2nd version
        optionprice > 15              ==>  optionprice <= 15   ==> optionprice > 7
 */
V mybs_udf3(L id, V sptprice, V strike, V rate, V divq, V volatility, V time, V optiontype, V divs, V dgrefval){
    P("Entering mybs_udf3\n");  
    V w0 = allocNode(); V w1 = allocNode(); V w2 = allocNode(); V w3 = allocNode();
    V w4 = allocNode();
    V m0 = allocNode(); V m1 = allocNode();
    V z0 = allocNode(); V z1 = allocNode(); V z = allocNode();
    if(!isOptimized){
        V optionprice = BlkSchls(sptprice, strike, rate, volatility, time, optiontype);
        if(id == 3){
            PROFILE(13, pfnGt(w4, optionprice, literalF64(15)));
        }
        else if(id == 6) {
            PROFILE(13, pfnLeq(w4, optionprice, literalF64(15)));
        }
        else if(id == 9){
            PROFILE(13, pfnGt(w4, optionprice, literalF64(6)));
        }
        PROFILE(15, pfnCompress(m0, w4, optiontype));
        PROFILE(16, pfnCompress(m1, w4, sptprice));
    }
    else { // id == 3 || id == 6
        V w7 = allocNode();
        V optionprice = BlkSchls(sptprice, strike, rate, volatility, time, optiontype);
        if(3==id){
            PROFILE(17, pfnGt(w7, optionprice, literalF64(15)));
        }
        else if(6==id){
            PROFILE(17, pfnLeq(w7, optionprice, literalF64(15)));
        }
        else if(9==id){
            PROFILE(17, pfnGt(w7, optionprice, literalF64(6)));
        }
        PROFILE(18, pfnCompress(m0, w7, optiontype));
        PROFILE(19, pfnCompress(m1, w7, sptprice));
        P("udf3 selectivity %lld: %lf%%\n", id,(vn(m0)*100.0)/vn(w0));
    }
    S group1[] = {(S)"optiontype", (S)"sptprice"};
    PROFILE(1, copyV(z0, literalSymVector(2, group1)));
    V group2[] = {m0, m1};
    PROFILE(2, pfnList(z1, 2, group2));
    PROFILE(3, pfnTable(z, z0, z1));
    R z;
}


E runBSQuery(L id){
    V a0 = allocNode(); V sptprice = allocNode(); V strike = allocNode(); V rate = allocNode();
    V volatility = allocNode(); V time = allocNode(); V optiontype = allocNode();
    V divq = allocNode();  V divs = allocNode();  V dgrefval = allocNode();
    struct timeval tv0, tv1;
    gettimeofday(&tv0, NULL);
    PROFILE( 1, pfnLoadTable(a0, literalSym((S)"blackscholes")));
    PROFILE( 2, pfnColumnValue(sptprice  , a0, literalSym((S)"sptprice")));
    PROFILE( 3, pfnColumnValue(strike    , a0, literalSym((S)"strike")));
    PROFILE( 4, pfnColumnValue(rate      , a0, literalSym((S)"rate")));
    PROFILE( 5, pfnColumnValue(divq      , a0, literalSym((S)"divq")));
    PROFILE( 6, pfnColumnValue(volatility, a0, literalSym((S)"volatility")));
    PROFILE( 7, pfnColumnValue(time      , a0, literalSym((S)"time")));
    PROFILE( 8, pfnColumnValue(optiontype, a0, literalSym((S)"optiontype")));
    PROFILE( 9, pfnColumnValue(divs      , a0, literalSym((S)"divs")));
    PROFILE(10, pfnColumnValue(dgrefval  , a0, literalSym((S)"dgrefval")));
    V z;
    switch(id){
        // base 0
        case 0: z = mybs_udf0(sptprice, strike, rate, divq, volatility, time, optiontype, divs, dgrefval); break;
        // query 1
        case 1: z = mybs_udf1(id, sptprice, strike, rate, divq, volatility, time, optiontype, divs, dgrefval); break;
        // query 2
        case 2: z = mybs_udf2(id, sptprice, strike, rate, divq, volatility, time, optiontype, divs, dgrefval); break;
        // query 3
        case 3: z = mybs_udf3(id, sptprice, strike, rate, divq, volatility, time, optiontype, divs, dgrefval); break;
        // query 4
        case 4: z = mybs_udf1(id, sptprice, strike, rate, divq, volatility, time, optiontype, divs, dgrefval); break;
        // query 5
        case 5: z = mybs_udf2(id, sptprice, strike, rate, divq, volatility, time, optiontype, divs, dgrefval); break;
        // query 6
        case 6: z = mybs_udf3(id, sptprice, strike, rate, divq, volatility, time, optiontype, divs, dgrefval); break;
        // query 7
        case 7: z = mybs_udf1(id, sptprice, strike, rate, divq, volatility, time, optiontype, divs, dgrefval); break;
        // query 8
        case 8: z = mybs_udf2(id, sptprice, strike, rate, divq, volatility, time, optiontype, divs, dgrefval); break;
        // query 9
        case 9: z = mybs_udf3(id, sptprice, strike, rate, divq, volatility, time, optiontype, divs, dgrefval); break;
        default: P("Invalid query id (%lld) in blackscholes\n", id); exit(99);
    }
    gettimeofday(&tv1, NULL);
    E elapsed = calcInterval(tv0,tv1)/1000.0;
    P("The elapsed time (ms): %g\n", elapsed);
    printTablePretty(z, 10);
    R elapsed;
}

L runBS(L sid){
    P("** Start simulation for Black-Scholes\n");
    readTableBS();
    L cur = getHeapOffset();
    DOI(TEST_RUNS, {setHeapOffset(cur); times[i]=runBSQuery(sid);})
    P("** End Black-Scholes\n");
}



















