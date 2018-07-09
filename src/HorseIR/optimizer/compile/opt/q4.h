/* num_func = 4, targ = p0 */
L q4_loopfusion_0(V z, V *x){
    // z -> p0
    V x0 = x[0]; // t5
    V x1 = x[1]; // k4
    initV(z,H_B,vn(x0));
    DOP(vn(x0), vB(z,i)=AND(AND(GEQ(vD(x0,i),19930701),LT(vD(x0,i),19931001)),vB(x1,i))) R 0;
}
L q4_peephole_0(V z, V *x, V *y){
    // z -> k4
    V x0 = x[0]; // t34;
    V x1 = x[1]; // t35;
    V y0 = y[0]; // k0;
    V y1 = y[1]; // k1;
    L r0 = vn(y1);
    initV(z, H_B, r0);
    DOP(vn(y0), if(vD(x0,i)<vD(x1,i))vB(z,vL(y0,i))=1)
}
E compiledQ4(){
    E elapsed=0;
    V t0  = allocNode(); V t1  = allocNode(); V t5  = allocNode(); V t6  = allocNode(); 
    V t22 = allocNode(); V t23 = allocNode(); V t34 = allocNode(); V t35 = allocNode(); 
    V t39 = allocNode(); V k0  = allocNode(); V k1  = allocNode(); V k3  = allocNode(); 
    V k4  = allocNode(); V k5  = allocNode(); V p0  = allocNode(); V t79  = allocNode(); 
    V t80 = allocNode(); V t81 = allocNode(); V t82 = allocNode(); V t83  = allocNode(); 
    V t84 = allocNode(); V t85 = allocNode(); V t86 = allocNode(); V t87  = allocNode(); 
    V t88 = allocNode(); V t89 = allocNode(); V t90 = allocNode(); V t91  = allocNode(); 
    V t92 = allocNode(); V k2  = allocNode(); V t74 = allocNode();
    V t10 = allocNode(); V t11 = allocNode(); V t12 = allocNode();
    tic;
    PROFILE(  0, t0 , pfnLoadTable(t0, initLiteralSym((S)"orders")));
    PROFILE(  1, t1 , pfnColumnValue(t1, t0, initLiteralSym((S)"o_orderkey")));
    PROFILE(  2, t5 , pfnColumnValue(t5, t0, initLiteralSym((S)"o_orderdate")));
    PROFILE(  3, t6 , pfnColumnValue(t6, t0, initLiteralSym((S)"o_orderpriority")));
    PROFILE(  4, t22, pfnLoadTable(t22, initLiteralSym((S)"lineitem")));
    PROFILE(  5, t23, pfnColumnValue(t23, t22, initLiteralSym((S)"l_orderkey")));
    PROFILE(  6, t34, pfnColumnValue(t34, t22, initLiteralSym((S)"l_commitdate")));
    PROFILE(  7, t35, pfnColumnValue(t35, t22, initLiteralSym((S)"l_receiptdate")));
    PROFILE(  9, k0 , pfnValues(k0, t23));
    PROFILE( 10, k1 , pfnKeys(k1, t23));
    if(OPT_PH){
        PROFILE(11, k4, q4_peephole_0(k4,(V []){t34, t35}, (V []){k0,k1}));
    }
    else {
        PROFILE(  8, t39, pfnLt(t39, t34, t35));
        PROFILE( 99, k2 , pfnCompress(k2, t39, k0)); //
        PROFILE( 11, k3 , pfnLen(k3, k1));
        PROFILE( 12, k4 , pfnVector(k4, k3, initLiteralBool(0)));
        PROFILE( 13, k5 , pfnIndexA(k4, k2, initLiteralBool(1)));
    }
    if(OPT_LF){
        PROFILE( 14, p0 , q4_loopfusion_0(p0,(V []){t5,k4}));
    }
    else {
        PROFILE(  4, t10, pfnGeq(t10, t5, initLiteralDate(19930701)));
        PROFILE(  5, t11, pfnLt(t11, t5, initLiteralDate(19931001)));
        PROFILE(  6, t12, pfnAnd(t12, t10, t11));
        PROFILE( 18, p0 , pfnAnd(p0, t12, k4));
    }
    PROFILE( 99, t74, pfnCompress(t74, p0, t6)); //
    PROFILE( 15, t79, pfnGroup(t79, t74));
    PROFILE( 16, t80, pfnKeys(t80, t79));
    PROFILE( 17, t81, pfnValues(t81, t79));
    PROFILE( 18, t82, pfnIndex(t82, t74, t80));
    PROFILE( 19, t83, pfnEachRight(t83,t74,t81,pfnIndex));
    PROFILE( 20, t84, pfnEach(t84,t83,pfnLen));
    PROFILE( 21, t85, pfnRaze(t85, t84));
    PROFILE( 22, t86, pfnList(t86, 1, (V []){t82}));
    PROFILE( 23, t87, pfnOrderBy(t87, t86, initLiteralBool(1)));
    PROFILE( 24, t88, pfnIndex(t88, t82, t87));
    PROFILE( 25, t89, pfnIndex(t89, t85, t87));
    PROFILE( 26, t90, copyV(t90, initLiteralSymVector(2, (S []){"o_orderpriority","order_count"})));
    PROFILE( 27, t91, pfnList(t91, 2, (V []){t88, t89}));
    PROFILE( 28, t92, pfnTable(t92, t90, t91));
    toc;
    printV(t92);
    R elapsed;
}
