/* num_func = 2, targ = t89 */
L q14_loopfusion_0(V z, V *x){
    // z -> t89
    V x0 = x[0]; // t86
    V x1 = x[1]; // t87
    initV(z,H_E,1);
    L i=0; vE(z,i)=DIV(MUL(100,vE(x0,i)),vE(x1,i)); R 0;
}
/* num_func = 6, targ = t83 */
L q14_loopfusion_1(V z, V *x){
    // z -> t83
    V x0 = x[0]; // t77
    V x1 = x[1]; // t57
    V x2 = x[2]; // t58
    initV(z,H_E,vn(x0));
    DOP(vn(x0), vE(z,i)=PLUS(MUL(vB(x0,i),MUL(vE(x1,i),MINUS(1,vE(x2,i)))),MUL(NOT(vB(x0,i)),0))) R 0;
}
/* num_func = 3, targ = t19 */
L q14_loopfusion_2(V z, V *x){
    // z -> t19
    V x0 = x[0]; // t11
    initV(z,H_B,vn(x0));
    DOP(vn(x0), vB(z,i)=AND(GEQ(vD(x0,i),19950901),LT(vD(x0,i),19951001))) R 0;
}
/* num_func = 2, targ = t85 */
L q14_loopfusion_3(V z, V *x){
    // z -> t85
    V x0 = x[0]; // t57
    V x1 = x[1]; // t58
    initV(z,H_E,vn(x0));
    DOP(vn(x0), vE(z,i)=MUL(vE(x0,i),MINUS(1,vE(x1,i)))) R 0;
}
L q14_peephole_0(V *z, V y, V *x){
    V z0 = z[0]; // t21
    V z1 = z[1]; // t25
    V z2 = z[2]; // t26
    V x0 = x[0]; // t2
    V x1 = x[1]; // t6
    V x2 = x[2]; // t7
    L len = vn(y), k = 0;
    L lenZ = 0, parZ[H_CORE], offset[H_CORE];
    DOI(H_CORE, parZ[i]=offset[i]=0)
    CHECKE(getNumOfNonZero(y,parZ));
    DOI(H_CORE, lenZ += parZ[i])
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    initV(z0,vp(x0),lenZ);
    initV(z1,vp(x1),lenZ);
    initV(z2,vp(x2),lenZ);
    DOT(len, if(vB(y,i)){L c=offset[tid]++;vL(z0,c)=vL(x0,i);vE(z1,c)=vE(x1,i);vE(z2,c)=vE(x2,i);})
}
E compiledQ14(){
    E elapsed=0;
    V t0  = allocNode(); V t2  = allocNode(); V t6  = allocNode(); V t7  = allocNode();
    V t11 = allocNode(); V t19 = allocNode(); V t26 = allocNode(); V t36 = allocNode();
    V t37 = allocNode(); V t41 = allocNode(); V t46 = allocNode(); V t47 = allocNode();
    V t48 = allocNode(); V t49 = allocNode(); V t50 = allocNode(); V t51 = allocNode();
    V t57 = allocNode(); V t58 = allocNode(); V t72 = allocNode(); V t77 = allocNode();
    V t83 = allocNode(); V t85 = allocNode(); V t86 = allocNode(); V t87 = allocNode();
    V t89 = allocNode(); V t90 = allocNode(); V t91 = allocNode(); V t92 = allocNode();
    V t21 = allocNode(); V t25 = allocNode(); 
    V t17 = allocNode(); V t18 = allocNode(); V t78 = allocNode(); V t79 = allocNode();
    V t80 = allocNode(); V t81 = allocNode(); V t82 = allocNode(); V t84 = allocNode();
    V t88 = allocNode();
    tic;
    PROFILE(  0, t0 , pfnLoadTable(t0, initLiteralSym((S)"lineitem")));
    PROFILE(  1, t2 , pfnColumnValue(t2, t0, initLiteralSym((S)"l_partkey")));
    PROFILE(  2, t6 , pfnColumnValue(t6, t0, initLiteralSym((S)"l_extendedprice")));
    PROFILE(  3, t7 , pfnColumnValue(t7, t0, initLiteralSym((S)"l_discount")));
    PROFILE(  4, t11, pfnColumnValue(t11, t0, initLiteralSym((S)"l_shipdate")));
    if(OPT_LF){
        PROFILE(  5, t19, q14_loopfusion_2(t19,(V []){t11}));
    }
    else {
        PROFILE(  5, t17, pfnGeq(t17, t11, initLiteralDate(19950901)));
        PROFILE(  6, t18, pfnLt(t18, t11, initLiteralDate(19951001)));
        PROFILE(  7, t19, pfnAnd(t19, t17, t18));
    }
    if(OPT_PH){
        PROFILE(  6, t26, q14_peephole_0((V []){t21,t25,t26},t19,(V []){t2,t6,t7}));
    }
    else {
        PROFILE(  8, t21, pfnCompress(t21, t19, t2));
        PROFILE(  9, t25, pfnCompress(t25, t19, t6));
        PROFILE( 10, t26, pfnCompress(t26, t19, t7));
    }
    PROFILE(  7, t36, pfnLoadTable(t36, initLiteralSym((S)"part")));
    PROFILE(  8, t37, pfnColumnValue(t37, t36, initLiteralSym((S)"p_partkey")));
    PROFILE(  9, t41, pfnColumnValue(t41, t36, initLiteralSym((S)"p_type")));
    PROFILE( 10, t46, pfnEnum(t46, t37, t21));
    PROFILE( 11, t47, pfnValues(t47, t46));
    PROFILE( 12, t48, pfnKeys(t48, t46));
    PROFILE( 13, t49, pfnLen(t49, t48));
    PROFILE( 14, t50, pfnLt(t50, t47, t49));
    PROFILE( 15, t51, pfnWhere(t51, t50));
    PROFILE( 16, t57, pfnIndex(t57, t25, t51));
    PROFILE( 17, t58, pfnIndex(t58, t26, t51));
    PROFILE( 18, t72, pfnIndex(t72, t41, t47));
    PROFILE( 19, t77, pfnLike(t77, t72, initLiteralString((S)"PROMO%")));
    if(OPT_LF){
        PROFILE( 20, t83, q14_loopfusion_1(t83,(V []){t77,t57,t58}));
        PROFILE( 21, t85, q14_loopfusion_3(t85,(V []){t57,t58}));
    }
    else {
        PROFILE( 24, t78, pfnMinus(t78, initLiteralI64(1), t58));
        PROFILE( 25, t79, pfnMul(t79, t57, t78));
        PROFILE( 26, t80, pfnMul(t80, t77, t79));
        PROFILE( 27, t81, pfnNot(t81, t77));
        PROFILE( 28, t82, pfnMul(t82, t81, initLiteralF64(0)));
        PROFILE( 29, t83, pfnPlus(t83, t80, t82));
        PROFILE( 30, t84, pfnMinus(t84, initLiteralI64(1), t58));
        PROFILE( 31, t85, pfnMul(t85, t57, t84));
    }
    PROFILE( 22, t86, pfnSum(t86, t83));
    PROFILE( 23, t87, pfnSum(t87, t85));
    if(OPT_LF){
        PROFILE( 24, t89, q14_loopfusion_0(t89,(V []){t86,t87}));
    }
    else {
        PROFILE( 34, t88, pfnMul(t88, initLiteralF64(100), t86));
        PROFILE( 35, t89, pfnDiv(t89, t88, t87));
    }
    PROFILE( 25, t90, copyV(t90, initLiteralSym((S)"promo_revenue")));
    PROFILE( 26, t91, pfnEnlist(t91, t89));
    PROFILE( 27, t92, pfnTable(t92, t90, t91));
    toc;
    printV(t92);
    R elapsed;
}
