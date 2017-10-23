
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

L BlkSchls(V sptprice, V strike, V rate, V volatility, V time, V otypeC){
	V xSqrtTime = allocNode(); V xLogTerm = allocNode(); V xRiskFreeRate = allocNode(); V otype = allocNode();
	V xDen = allocNode(); V xPowerTerm = allocNode();  V xD1 = allocNode(); V d1 = allocNode();
	V xD2 = allocNode(); V d2 = allocNode(); V invs2xPI = allocNode(); V NofXd1 = allocNode(); V NofXd2 = allocNode();
	V FutureValueX = allocNode(); V OptionPrice = allocNode();
	V t0 = allocNode(); V t1 = allocNode(); V t2 = allocNode(); V t3 = allocNode(); V t4 = allocNode();
	V t5 = allocNode(); V t6 = allocNode(); V t7 = allocNode(); V t8 = allocNode(); V t9 = allocNode();
	V t10 = allocNode(); V t11 = allocNode(); V t12 = allocNode(); V t13 = allocNode(); V t14 = allocNode();
	V t15 = allocNode(); V t16 = allocNode(); V t17 = allocNode();
	
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
	P("OptionPrice: ");DOI(5, P("%lf ",vE(OptionPrice,i))) P("\n");
	R 0;
}

L runBS(){
	readTableBS();
	V a0 = allocNode(); V sptprice = allocNode(); V strike = allocNode(); V rate = allocNode();
	V volatility = allocNode(); V time = allocNode(); V otypeC = allocNode();
	struct timeval tv0, tv1;
    gettimeofday(&tv0, NULL);
	PROFILE( 1, pfnLoadTable(a0, literalSym((S)"blackscholes")));
	PROFILE( 2, pfnColumnValue(sptprice  , a0, literalSym((S)"sptprice")));
	PROFILE( 3, pfnColumnValue(strike    , a0, literalSym((S)"strike")));
	PROFILE( 4, pfnColumnValue(rate      , a0, literalSym((S)"rate")));
	PROFILE( 5, pfnColumnValue(volatility, a0, literalSym((S)"volatility")));
	PROFILE( 6, pfnColumnValue(time      , a0, literalSym((S)"time")));
	PROFILE( 7, pfnColumnValue(otypeC    , a0, literalSym((S)"optiontype")));
	PROFILE( 8, BlkSchls(sptprice, strike, rate, volatility, time, otypeC));
	gettimeofday(&tv1, NULL);
    P("Result (elapsed time %g ms)\n\n", calcInterval(tv0,tv1)/1000.0);
}



















