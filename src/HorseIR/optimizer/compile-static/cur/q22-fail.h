/*==== FE: Loop fusion with elementwise functions ====*/
/* num_func = 2, targ = t13 */
L q22_loopfusion_0(V z, V *x){
    // z -> t13
    V x0 = x[0]; // t6
    V x1 = x[1]; // t11
    initV(z,H_B,vn(x0));
    DOP(vn(x0), vB(z,i)=AND(GT(vE(x0,i),0),vB(x1,i))) R 0;
}
/*==== FP: Loop fusion with patterns ====*/
L q22_peephole_0(V *z, V y, V *x){
    V z0 = z[0]; // t37
    V z1 = z[1]; // t41
    V z2 = z[2]; // t42
    V x0 = x[0]; // t26
    V x1 = x[1]; // t30
    V x2 = x[2]; // t31
    L len = vn(y), k = 0;
    L lenZ = 0, parZ[H_CORE], offset[H_CORE];
    DOI(H_CORE, parZ[i]=offset[i]=0)
    CHECKE(getNumOfNonZero(y,parZ));
    DOI(H_CORE, lenZ += parZ[i])
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    initV(z0,vp(x0),lenZ);
    initV(z1,vp(x1),lenZ);
    initV(z2,vp(x2),lenZ);
    DOT(len, if(vB(y,i)){L c=offset[tid]++;vL(z0,c)=vL(x0,i);vS(z1,c)=vS(x1,i);vE(z2,c)=vE(x2,i);}) R 0;
}
L q22_peephole_1(V *z, V y, V *x){
    V z0 = z[0]; // t51
    V z1 = z[1]; // t56
    V z2 = z[2]; // t57
    V x0 = x[0]; // t37
    V x1 = x[1]; // t41
    V x2 = x[2]; // t42
    L len = vn(y), k = 0;
    L lenZ = 0, parZ[H_CORE], offset[H_CORE];
    DOI(H_CORE, parZ[i]=offset[i]=0)
    CHECKE(getNumOfNonZero(y,parZ));
    DOI(H_CORE, lenZ += parZ[i])
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    initV(z0,vp(x0),lenZ);
    initV(z1,vp(x1),lenZ);
    initV(z2,vp(x2),lenZ);
    DOT(len, if(vB(y,i)){L c=offset[tid]++;vL(z0,c)=vL(x0,i);vS(z1,c)=vS(x1,i);vE(z2,c)=vE(x2,i);}) R 0;
}
L q22_peephole_2(V *z, V y, V *x){
    V z0 = z[0]; // t80
    V z1 = z[1]; // t81
    V x0 = x[0]; // t56
    V x1 = x[1]; // t57
    L len = vn(y), k = 0;
    L lenZ = 0, parZ[H_CORE], offset[H_CORE];
    DOI(H_CORE, parZ[i]=offset[i]=0)
    CHECKE(getNumOfNonZero(y,parZ));
    DOI(H_CORE, lenZ += parZ[i])
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    initV(z0,vp(x0),lenZ);
    initV(z1,vp(x1),lenZ);
    DOT(len, if(vB(y,i)){L c=offset[tid]++;vS(z0,c)=vS(x0,i);vE(z1,c)=vE(x1,i);}) R 0;
}
// Pattern FP3
// Pattern FP4
// Pattern FP5
E compiledQ22(){
    E elapsed=0;
    V t0   = allocNode(); V t5   = allocNode(); V t6   = allocNode(); V t9   = allocNode(); 
    V t10  = allocNode(); V t11  = allocNode(); V t12  = allocNode(); V t13  = allocNode(); 
    V t19  = allocNode(); V t24  = allocNode(); V t25  = allocNode(); V t26  = allocNode(); 
    V t30  = allocNode(); V t31  = allocNode(); V t34  = allocNode(); V t35  = allocNode(); 
    V t36  = allocNode(); V t37  = allocNode(); V t41  = allocNode(); V t42  = allocNode(); 
    V t45  = allocNode(); V t51  = allocNode(); V t56  = allocNode(); V t57  = allocNode(); 
    V t60  = allocNode(); V t62  = allocNode(); V t70  = allocNode(); V t71  = allocNode(); 
    V t72  = allocNode(); V t80  = allocNode(); V t81  = allocNode(); V t84  = allocNode(); 
    V t85  = allocNode(); V t86  = allocNode(); V t87  = allocNode(); V t88  = allocNode(); 
    V t89  = allocNode(); V t90  = allocNode(); V t91  = allocNode(); V t92  = allocNode(); 
    V t93  = allocNode(); V t94  = allocNode(); V t95  = allocNode(); V t96  = allocNode(); 
    V t97  = allocNode(); V t98  = allocNode(); V t99  = allocNode(); V t100 = allocNode(); 
    V t101 = allocNode(); V t102 = allocNode(); 
    tic;
    PROFILE(  0, t0 , pfnLoadTable(t0, initLiteralSym((S)"customer")));
    PROFILE(  1, t5 , pfnColumnValue(t5, t0, initLiteralSym((S)"c_phone")));
    PROFILE(  2, t6 , pfnColumnValue(t6, t0, initLiteralSym((S)"c_acctbal")));
    PROFILE(  3, t9 , copyV(t9, initLiteralStrVector(7, (S []){"13","31","23","29","30","18","17"})));
    PROFILE(  4, t10, pfnSubString(t10, t5, initLiteralI64Vector(2, (L []){1,2})));
    PROFILE(  5, t11, pfnMember(t11, t9, t10));
    PROFILE(  6, t13, q22_loopfusion_0(t13,(V []){t6,t11}));
    PROFILE(  7, t19, pfnCompress(t19, t13, t6));
    PROFILE(  8, t24, pfnAvg(t24, t19));
    PROFILE(  9, t25, pfnLoadTable(t25, initLiteralSym((S)"customer")));
    PROFILE( 10, t26, pfnColumnValue(t26, t25, initLiteralSym((S)"c_custkey")));
    PROFILE( 11, t30, pfnColumnValue(t30, t25, initLiteralSym((S)"c_phone")));
    PROFILE( 12, t31, pfnColumnValue(t31, t25, initLiteralSym((S)"c_acctbal")));
    PROFILE( 13, t34, copyV(t34, initLiteralStrVector(7, (S []){"13","31","23","29","30","18","17"})));
    PROFILE( 14, t35, pfnSubString(t35, t30, initLiteralI64Vector(2, (L []){1,2})));
    PROFILE( 15, t36, pfnMember(t36, t34, t35));
    PROFILE( 16, t42, q22_peephole_0((V []){t37,t41,t42},t36,(V []){t26,t30,t31}));
    PROFILE( 17, t45, pfnLt(t45, t24, t42));
    PROFILE( 18, t57, q22_peephole_1((V []){t51,t56,t57},t45,(V []){t37,t41,t42}));
    PROFILE( 19, t60, pfnLoadTable(t60, initLiteralSym((S)"orders")));
    PROFILE( 20, t62, pfnColumnValue(t62, t60, initLiteralSym((S)"o_custkey")));
    PROFILE( 21, t70, pfnFetch(t70, t62));
    //getInfoVar(t70); getInfoVar(t51); getchar();
    // Variable t70 has type H_L and len 1500000
    // Variable t51 has type H_L and len 19000
    PROFILE( 22, t71, pfnMember(t71, t70, t51)); // most expensive
    PROFILE( 23, t72, pfnNot(t72, t71));
    PROFILE( 24, t81, q22_peephole_2((V []){t80,t81},t72,(V []){t56,t57}));
    PROFILE( 25, t84, pfnSubString(t84, t80, initLiteralI64Vector(2, (L []){1,2})));
    PROFILE( 26, t85, pfnList(t85, 1, (V []){t84}));
    PROFILE( 27, t86, pfnGroup(t86, t85));
    PROFILE( 28, t87, pfnKeys(t87, t86));
    PROFILE( 29, t88, pfnValues(t88, t86));
    PROFILE( 30, t89, pfnIndex(t89, t84, t87));
    PROFILE( 31, t90, pfnEachRight(t90,t81,t88,pfnIndex));
    PROFILE( 32, t91, pfnEach(t91,t90,pfnSum));
    PROFILE( 33, t92, pfnRaze(t92, t91));
    PROFILE( 34, t93, pfnEach(t93,t88,pfnLen));
    PROFILE( 35, t94, pfnRaze(t94, t93));
    PROFILE( 36, t95, pfnList(t95, 1, (V []){t89}));
    PROFILE( 37, t96, pfnOrderBy(t96, t95, initLiteralBool(1)));
    PROFILE( 38, t97, pfnIndex(t97, t89, t96));
    PROFILE( 39, t98, pfnIndex(t98, t94, t96));
    PROFILE( 40, t99, pfnIndex(t99, t92, t96));
    PROFILE( 41, t100, copyV(t100, initLiteralSymVector(3, (S []){"cntrycode","numcust","totacctbal"})));
    PROFILE( 42, t101, pfnList(t101, 3, (V []){t97 ,t98 ,t99}));
    PROFILE( 43, t102, pfnTable(t102, t100, t101));
    toc;
    printV2(t102,20);
    R elapsed;
}
