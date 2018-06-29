//=====Loop Fusion=====
//=====Peephole=====
L q13_peephole_0(V z, V x, V y){
    // z -> t44, x -> k7, y -> t40
    L r0 = vn(y);
    initV(z, H_L, r0);
    DOP(r0, vL(z,i)=vn(vV(y,i))); R 0;
}
E compiledQ13(){
    E elapsed=0;
    V a0  = allocNode(); V a1  = allocNode(); V t9  = allocNode(); V t11 = allocNode();
    V t18 = allocNode(); V t19 = allocNode(); V t20 = allocNode(); V t30 = allocNode();
    V k0  = allocNode(); V k1  = allocNode(); V k2  = allocNode(); V k3  = allocNode();
    V k4  = allocNode(); V t32 = allocNode(); V t33 = allocNode(); V t34 = allocNode();
    V t36 = allocNode(); V k5  = allocNode(); V k6  = allocNode(); V k7  = allocNode();
    V t37 = allocNode(); V t38 = allocNode(); V t39 = allocNode(); V t40 = allocNode();
    V t41 = allocNode(); V t42 = allocNode(); V t43 = allocNode(); V t44 = allocNode();
    V t45 = allocNode(); V t46 = allocNode(); V t47 = allocNode(); V t48 = allocNode();
    V t49 = allocNode(); V t50 = allocNode(); V t51 = allocNode();
    V t35 = allocNode(); V t31 = allocNode();
    //V w0[] = {k7};
    //V w1[] = {t44,t41};
    //B w2[] = {0,0};
    //S w3[] = {"c_count","custdist"};
    //V w4[] = {t47,t48};
    tic;
    PROFILE(  0, a0 , pfnLoadTable(a0, initLiteralSym((S)"customer")));
    PROFILE(  1, a1 , pfnColumnValue(a1, a0, initLiteralSym((S)"c_custkey")));
    PROFILE(  2, t9 , pfnLoadTable(t9, initLiteralSym((S)"orders")));
    PROFILE(  3, t11, pfnColumnValue(t11, t9, initLiteralSym((S)"o_custkey")));
    PROFILE(  4, t18, pfnColumnValue(t18, t9, initLiteralSym((S)"o_comment")));
    PROFILE(  5, t19, pfnLike(t19, t18, initLiteralString((S)"%special%requests%"))); // very slow!!
    PROFILE(  6, t20, pfnNot(t20, t19));
    PROFILE(  7, t30, pfnValues(t30, t11));
    PROFILE( 99, t31, pfnCompress(t31, t20, t30)); //
    PROFILE(  8, k0 , pfnKeys(k0, t11));
    PROFILE(  9, k1 , pfnLen(k1, k0));
    PROFILE( 10, k2 , pfnVector(k2, k1, initLiteralBool(1)));
    PROFILE( 11, k3 , pfnIndexA(k2, t31, initLiteralBool(0)));
    PROFILE( 12, k4 , pfnWhere(k4, k2));
    PROFILE( 13, t32, pfnGroup(t32, t31));
    PROFILE( 14, t33, pfnKeys(t33, t32));
    PROFILE( 15, t34, pfnValues(t34, t32));
    PROFILE( 16, t35, pfnEach(t35,t34,pfnLen));
    PROFILE( 17, t36, pfnRaze(t36, t35));
    PROFILE( 18, k5 , pfnLen(k5, k4));
    PROFILE( 19, k6 , pfnVector(k6, k5, initLiteralI64(0)));
    PROFILE( 20, k7 , pfnAppend(k7, k6, t36));
    PROFILE( 21, t37, pfnList(t37, 1, (V []){k7}));
    PROFILE( 22, t38, pfnGroup(t38, t37));
    PROFILE( 23, t39, pfnKeys(t39, t38));
    PROFILE( 24, t40, pfnValues(t40, t38));
    PROFILE( 25, t41, pfnIndex(t41, k7, t39));
    if(OPT_PH){
        PROFILE( 28, t44, q13_peephole_0(t44, k7, t40));
    }
    else {
        PROFILE( 26, t42, pfnEachRight(t42,k7,t40,pfnIndex));
        PROFILE( 27, t43, pfnEach(t43,t42,pfnLen));
        PROFILE( 28, t44, pfnRaze(t44, t43));
    }
    PROFILE( 29, t45, pfnList(t45, 2, (V []){t44,t41}));
    PROFILE( 30, t46, pfnOrderBy(t46, t45, initLiteralBoolVector(2, (B []){0,0})));
    PROFILE( 31, t47, pfnIndex(t47, t41, t46));
    PROFILE( 32, t48, pfnIndex(t48, t44, t46));
    PROFILE( 33, t49, copyV(t49, initLiteralSymVector(2, (S []){"c_count","custdist"})));
    PROFILE( 34, t50, pfnList(t50, 2, (V []){t47,t48}));
    PROFILE( 35, t51, pfnTable(t51, t49, t50));
    toc;
    printV(t51);
    R elapsed;
}
