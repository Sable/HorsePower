/* num_func = 8, targ = t26 */
L q12_loopfusion_0(V z, V *x){
    // z -> t26
    V x0 = x[0]; // t13
    V x1 = x[1]; // t18
    V x2 = x[2]; // t12
    V x3 = x[3]; // t11
    initV(z,H_B,vn(x0));
    DOP(vn(x0), vB(z,i)=AND(AND(GEQ(vD(x0,i),19940101),LT(vD(x0,i),19950101)),AND(AND(vB(x1,i),LT(vD(x2,i),vD(x0,i))),LT(vD(x3,i),vD(x2,i))))) R 0;
}
/* num_func = 10, targ = t88 */
L q12_loopfusion_1(V z, V *x){
    // z -> t88
    V x0 = x[0]; // t78
    L s0 = getSymbol("1-URGENT");
    L s1 = getSymbol("2-HIGH");
    initV(z,H_L,vn(x0));
    DOP(vn(x0), vL(z,i)=PLUS(MUL(OR(EQ(vQ(x0,i),s0),EQ(vQ(x0,i),s1)),1),MUL(NOT(OR(EQ(vQ(x0,i),s0),EQ(vQ(x0,i),s1))),0))) R 0;
}
/* num_func = 10, targ = t95 */
L q12_loopfusion_2(V z, V *x){
    // z -> t95
    V x0 = x[0]; // t78
    L s0 = getSymbol("1-URGENT");
    L s1 = getSymbol("2-HIGH");
    initV(z,H_L,vn(x0));
    DOP(vn(x0), vL(z,i)=PLUS(MUL(AND(NEQ(vQ(x0,i),s0),NEQ(vQ(x0,i),s1)),1),MUL(NOT(AND(NEQ(vQ(x0,i),s0),NEQ(vQ(x0,i),s1))),0))) R 0;
}
L q12_peephole_0(V *z, V *x, V y){
    L r0 = vn(y);// y -> t99
    V z0 = z[0]; // t103
    V z1 = z[1]; // t106
    V x0 = x[0]; // t88
    V x1 = x[1]; // t95
    initV(z0, H_L, r0);
    initV(z1, H_L, r0);
    DOJ(r0, {V t=vV(y,j);  L c0=0; L c1=0; \
        DOI(vn(t), {L k=vL(t,i); c0+=vL(x0,k); c1+=vL(x1,k);})\
        vL(z0,j)=c0; \
        vL(z1,j)=c1; \
    })
    R 0;
}
E compiledQ12(){
    E elapsed=0;
    V t0  = allocNode(); V t1  = allocNode(); V t11 = allocNode(); V t12 = allocNode();
    V t13 = allocNode(); V t15 = allocNode(); V t17 = allocNode(); V t18 = allocNode();
    V t26 = allocNode(); V t43 = allocNode(); V t49 = allocNode(); V t53 = allocNode();
    V t56 = allocNode(); V t71 = allocNode(); V t78 = allocNode(); V t88 = allocNode();
    V t95 = allocNode(); V t96 = allocNode(); V t97 = allocNode(); V t98 = allocNode();
    V t99 = allocNode(); V t100= allocNode(); V t101= allocNode(); V t102= allocNode();
    V t103= allocNode(); V t104= allocNode(); V t105= allocNode(); V t106= allocNode();
    V t107= allocNode(); V t108= allocNode(); V t109= allocNode(); V t110= allocNode();
    V t111= allocNode(); V t112= allocNode(); V t113= allocNode(); V t114= allocNode();
    V k1  = allocNode();
    V t19 = allocNode(); V t20 = allocNode(); V t21 = allocNode(); V t23 = allocNode();
    V t24 = allocNode(); V t25 = allocNode(); V t82 = allocNode(); V t83 = allocNode();
    V t84 = allocNode(); V t85 = allocNode(); V t86 = allocNode(); V t87 = allocNode();
    V t89 = allocNode(); V t90 = allocNode(); V t91 = allocNode(); V t92 = allocNode();
    V t93 = allocNode(); V t94 = allocNode(); V t22 = allocNode();
    //S w0[] = {"MAIL","SHIP"};
    //V w1[] = {t13,t18,t12,t11};
    //V w2[] = {t78};
    //V w3[] = {t71};
    //V w4[] = {t100};
    //S w5[] = {"l_shipmode","high_line_count","low_line_count"};
    //V w6[] = {t109 ,t110 ,t111};
    tic;
    PROFILE(  0, t0 , pfnLoadTable(t0, initLiteralSym((S)"lineitem")));
    PROFILE(  1, t1 , pfnColumnValue(t1, t0, initLiteralSym((S)"l_orderkey")));
    PROFILE(  2, t11, pfnColumnValue(t11, t0, initLiteralSym((S)"l_shipdate")));
    PROFILE(  3, t12, pfnColumnValue(t12, t0, initLiteralSym((S)"l_commitdate")));
    PROFILE(  4, t13, pfnColumnValue(t13, t0, initLiteralSym((S)"l_receiptdate")));
    PROFILE(  5, t15, pfnColumnValue(t15, t0, initLiteralSym((S)"l_shipmode")));
    PROFILE(  6, t17, copyV(t17, initLiteralSymVector(2, (S []){"MAIL","SHIP"})));
    PROFILE(  7, t18, pfnMember(t18, t17, t15));  // expensive
    if(OPT_LF){
        PROFILE(  8, t26, q12_loopfusion_0(t26,(V []){t13,t18,t12,t11}));  // expensive
    }
    else {
        PROFILE(  8, t19, pfnLt(t19, t12, t13));
        PROFILE(  9, t20, pfnLt(t20, t11, t12));
        PROFILE( 10, t21, pfnAnd(t21, t18, t19));
        PROFILE( 11, t22, pfnAnd(t22, t21, t20));
        PROFILE( 12, t23, pfnGeq(t23, t13, initLiteralDate(19940101)));
        PROFILE( 13, t24, pfnLt(t24, t13, initLiteralDate(19950101)));
        PROFILE( 14, t25, pfnAnd(t25, t23, t24));
        PROFILE( 15, t26, pfnAnd(t26, t25, t22));
    }
    PROFILE(  9, t43, pfnLoadTable(t43, initLiteralSym((S)"orders")));
    PROFILE( 10, t49, pfnColumnValue(t49, t43, initLiteralSym((S)"o_orderpriority")));
    PROFILE( 11, t53, pfnValues(t53, t1));
    PROFILE( 99, t53, pfnCompress(k1, t26, t53)); //
    PROFILE( 12, t56, pfnWhere(t56, t26));
    PROFILE( 13, t71, pfnIndex(t71, t15, t56));
    PROFILE( 14, t78, pfnIndex(t78, t49, k1));
    if(OPT_LF){
        PROFILE( 15, t88, q12_loopfusion_1(t88,(V []){t78}));
        PROFILE( 16, t95, q12_loopfusion_2(t95,(V []){t78}));
    }
    else {
        PROFILE( 23, t82, pfnEq(t82, t78, initLiteralSym((S)"1-URGENT")));
        PROFILE( 24, t83, pfnEq(t83, t78, initLiteralSym((S)"2-HIGH")));
        PROFILE( 25, t84, pfnOr(t84, t82, t83));
        PROFILE( 26, t85, pfnMul(t85, t84, initLiteralI64(1)));
        PROFILE( 27, t86, pfnNot(t86, t84));
        PROFILE( 28, t87, pfnMul(t87, t86, initLiteralI64(0)));
        PROFILE( 29, t88, pfnPlus(t88, t85, t87));
        PROFILE( 30, t89, pfnNeq(t89, t78, initLiteralSym((S)"1-URGENT")));
        PROFILE( 31, t90, pfnNeq(t90, t78, initLiteralSym((S)"2-HIGH")));
        PROFILE( 32, t91, pfnAnd(t91, t89, t90));
        PROFILE( 33, t92, pfnMul(t92, t91, initLiteralI64(1)));
        PROFILE( 34, t93, pfnNot(t93, t91));
        PROFILE( 35, t94, pfnMul(t94, t93, initLiteralI64(0)));
        PROFILE( 36, t95, pfnPlus(t95, t92, t94));
    }
    PROFILE( 17, t96, pfnList(t96, 1, (V []){t71}));
    PROFILE( 18, t97, pfnGroup(t97, t96));
    PROFILE( 19, t98, pfnKeys(t98, t97));
    PROFILE( 20, t99, pfnValues(t99, t97));
    PROFILE( 21, t100, pfnIndex(t100, t71, t98));
    if(OPT_PH){
        PROFILE(27, t106, q12_peephole_0((V []){t103,t106}, (V []){t88,t95}, t99));
    }
    else {
        PROFILE( 22, t101, pfnEachRight(t101,t88,t99,pfnIndex));
        PROFILE( 23, t102, pfnEach(t102,t101,pfnSum));
        PROFILE( 24, t103, pfnRaze(t103, t102));
        PROFILE( 25, t104, pfnEachRight(t104,t95,t99,pfnIndex));
        PROFILE( 26, t105, pfnEach(t105,t104,pfnSum));
        PROFILE( 27, t106, pfnRaze(t106, t105));
    }
    PROFILE( 28, t107, pfnList(t107, 1, (V []){t100}));
    PROFILE( 29, t108, pfnOrderBy(t108, t107, initLiteralBool(1)));
    PROFILE( 30, t109, pfnIndex(t109, t100, t108));
    PROFILE( 31, t110, pfnIndex(t110, t103, t108));
    PROFILE( 32, t111, pfnIndex(t111, t106, t108));
    PROFILE( 33, t112, copyV(t112, initLiteralSymVector(3, (S []){"l_shipmode","high_line_count","low_line_count"})));
    PROFILE( 34, t113, pfnList(t113, 3, (V []){t109 ,t110 ,t111}));
    PROFILE( 35, t114, pfnTable(t114, t112, t113));
    toc;
    printV(t114);
    R elapsed;
}
