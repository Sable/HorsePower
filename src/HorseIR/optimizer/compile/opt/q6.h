L q6_loopfusion_0(V z, V *x){
    // z -> t25
    V x0 = x[0]; // t11
    V x1 = x[1]; // t5
    V x2 = x[2]; // t7
    L r0 =vn(x0); initV(z,H_B,r0);
    // try 0
    DOP(vn(x0), vB(z,i)=AND(AND(AND(GEQ(vD(x0,i),19940101),LT(vD(x0,i),19950101)),LT(vE(x1,i),24)),AND(GEQ(vE(x2,i),0.05),LEQ(vE(x2,i),0.07)))) R 0;
    // try 1
    //DOP(vn(x0), vB(z,i)=AND(AND(AND(GEQ(vE(x2,i),0.05),LEQ(vE(x2,i),0.07)),AND(GEQ(vD(x0,i),19940101),LT(vD(x0,i),19950101))),LT(vE(x1,i),24))) R 0;
}

L q6_peephole_0(V *z, V y, V *x){
    // z -> t32
    V z0 = z[0]; // t31
    V z1 = z[1]; // t32
    V x0 = x[0]; // t6
    V x1 = x[1]; // t7
    L len = vn(y), k = 0;
    L lenZ = 0, parZ[H_CORE], offset[H_CORE];
    DOI(H_CORE, parZ[i]=offset[i]=0)
    CHECKE(getNumOfNonZero(y,parZ));
    DOI(H_CORE, lenZ += parZ[i])
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    initV(z0,vp(x0),lenZ); // H_E
    initV(z1,vp(x1),lenZ); // H_E
    DOT(len, if(vB(y,i)){L c=offset[tid]++; \
                              vE(z0,c)=vE(x0,i); vE(z1,c)=vE(x1,i);})
}

E compiledQ6(){
    E elapsed=0;
    V t0  = allocNode(); V t5  = allocNode(); V t6  = allocNode(); V t7  = allocNode();
    V t11 = allocNode(); V t17 = allocNode(); V t18 = allocNode(); V t19 = allocNode();
    V t20 = allocNode(); V t21 = allocNode(); V t22 = allocNode(); V t23 = allocNode();
    V t24 = allocNode(); V t25 = allocNode();
    V t31 = allocNode(); V t32 = allocNode();
    V t42 = allocNode(); V t43 = allocNode(); V t44 = allocNode(); V t45 = allocNode();
    V t46 = allocNode();
    tic;
    PROFILE(  0, t0 , pfnLoadTable(t0, initLiteralSym((S)"lineitem")));
    PROFILE(  1, t5 , pfnColumnValue(t5, t0, initLiteralSym((S)"l_quantity")));
    PROFILE(  2, t6 , pfnColumnValue(t6, t0, initLiteralSym((S)"l_extendedprice")));
    PROFILE(  3, t7 , pfnColumnValue(t7, t0, initLiteralSym((S)"l_discount")));
    PROFILE(  4, t11, pfnColumnValue(t11, t0, initLiteralSym((S)"l_shipdate")));
    //V tt1[] = {t11,t5,t7};
    if(OPT_LF){
        PROFILE(  5, t25, q6_loopfusion_0(t25,(V []){t11,t5,t7}));
    }
    else {
        PROFILE(  5, t17, pfnGeq(t17, t11, initLiteralDate(19940101)));
        PROFILE(  6, t18, pfnLt(t18, t11, initLiteralDate(19950101)));
        PROFILE(  7, t19, pfnAnd(t19, t17, t18));
        PROFILE(  8, t20, pfnLt(t20, t5, initLiteralF64(24)));
        PROFILE(  9, t21, pfnGeq(t21, t7, initLiteralF64(0.05)));
        PROFILE( 10, t22, pfnLeq(t22, t7, initLiteralF64(0.07)));
        PROFILE( 11, t23, pfnAnd(t23, t21, t22));
        PROFILE( 12, t24, pfnAnd(t24, t19, t20));
        PROFILE( 13, t25, pfnAnd(t25, t24, t23));
    }
    //PROFILE(  6, t31, pfnCompress(t31, t25, t6));
    //PROFILE(  7, t32, pfnCompress(t32, t25, t7));
    //V tt2[] = {t6,t7};
    //V tt3[] = {t31,t32};
    if(OPT_PH){
        PROFILE(  7, t99, q6_peephole_0((V []){t31,t32}, t25, (V []){t6,t7}));
    }
    else {
        PROFILE( 14, t31, pfnCompress(t31, t25, t6));
        PROFILE( 15, t32, pfnCompress(t32, t25, t7));
    }
    PROFILE(  8, t42, pfnMul(t42, t31, t32));
    PROFILE(  9, t43, pfnSum(t43, t42));
    PROFILE( 10, t44, copyV(t44, initLiteralSym((S)"revenue")));
    PROFILE( 11, t45, pfnEnlist(t45, t43));
    PROFILE( 12, t46, pfnTable(t46, t44, t45));
    toc;
    printV(t46);
    R elapsed;
}
