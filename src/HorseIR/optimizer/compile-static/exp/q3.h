/*==== FE: Loop fusion with elementwise functions ====*/
/* num_func = 2, targ = t108 */
L q3_loopfusion_0(V z, V *x){
    // z -> t108
    V x0 = x[0]; // t84
    V x1 = x[1]; // t85
    initV(z,H_E,vn(x0));
    DOP(vn(x0), vE(z,i)=MUL(vE(x0,i),MINUS(1,vE(x1,i)))) R 0;
}
/*==== FP: Loop fusion with patterns ====*/
L q3_peephole_0(V *z, V y, V *x){
    V z0 = z[0]; // t79
    V z1 = z[1]; // t84
    V z2 = z[2]; // t85
    V x0 = x[0]; // k0
    V x1 = x[1]; // t67
    V x2 = x[2]; // t68
    L len = vn(y), k = 0;
    L lenZ = 0, parZ[H_CORE], offset[H_CORE];
    DOI(H_CORE, parZ[i]=offset[i]=0)
    CHECKE(getNumOfNonZero(y,parZ));
    DOI(H_CORE, lenZ += parZ[i])
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    initV(z0,vp(x0),lenZ);
    initV(z1,vp(x1),lenZ);
    initV(z2,vp(x2),lenZ);
    DOT(len, if(vB(y,i)){L c=offset[tid]++;vL(z0,c)=vL(x0,i);vE(z1,c)=vE(x1,i);vE(z2,c)=vE(x2,i);}) R 0;
}
L q3_peephole_1(V *z, V *x, V y){
    L r0 = vn(y);// y -> t112
    V z0 = z[0]; // t115
    V x0 = x[0]; // t108
    initV(z0, H_E, r0);
    /* test
    L v_min = 9999999, v_max = -1;
    DOI(r0, {V t=vV(y,i); \
            if(vn(t)<v_min)v_min=vn(t); \
            if(vn(t)>v_max)v_max=vn(t);})
    P("total = %lld, max = %lld, min = %lld\n", r0, v_max, v_min); getchar(); */
    /* slow: max=7, min=1
    DOJ(r0, {V t=vV(y,j);  E c0=0; \
        DOP(vn(t), {L k=vL(t,i); c0+=vE(x0,k);},\
        reduction(+:c0)) \
        vE(z0,j)=c0; \
    }) */
    // fast: simple addition
    DOP(r0, {V t=vV(y,i);  E c0=0; \
        DOJ(vn(t), c0+=vE(x0,vL(t,j));) vE(z0,i)=c0; })
    R 0;
}
L q3_peephole_2(V z, V x, V y){
    // z -> t115, x -> t108, y -> t112
    L r0 = vn(y);
    initV(z, H_L, r0);
    DOP(r0, vL(z,i) = vn(vV(y,i))) R 0;
}
// Pattern FP3
// Pattern FP4
// Pattern FP5
E compiled_main(){
    E elapsed=0;
    V t0   = allocNode(); V t7   = allocNode(); V t9   = allocNode(); V t18  = allocNode(); 
    V t19  = allocNode(); V t20  = allocNode(); V t23  = allocNode(); V t26  = allocNode(); 
    V t28  = allocNode(); V t38  = allocNode(); V t39  = allocNode(); V t40  = allocNode(); 
    V t42  = allocNode(); V t43  = allocNode(); V t52  = allocNode(); V t56  = allocNode(); 
    V t59  = allocNode(); V t61  = allocNode(); V t62  = allocNode(); V t67  = allocNode(); 
    V t68  = allocNode(); V t72  = allocNode(); V t78  = allocNode(); V k0   = allocNode(); 
    V t79  = allocNode(); V t84  = allocNode(); V t85  = allocNode(); V t98  = allocNode(); 
    V t99  = allocNode(); V t100 = allocNode(); V t101 = allocNode(); V t102 = allocNode(); 
    V t103 = allocNode(); V t104 = allocNode(); V t105 = allocNode(); V t106 = allocNode(); 
    V t107 = allocNode(); V t108 = allocNode(); V t109 = allocNode(); V t110 = allocNode(); 
    V t111 = allocNode(); V t112 = allocNode(); V t113 = allocNode(); V t114 = allocNode(); 
    V t115 = allocNode(); V t118 = allocNode(); V t119 = allocNode(); V t120 = allocNode(); 
    V t121 = allocNode(); V t122 = allocNode(); V t123 = allocNode(); V t124 = allocNode(); 
    V t125 = allocNode(); V t127 = allocNode(); V t131 = allocNode(); V t132 = allocNode(); 
    V t133 = allocNode(); V t134 = allocNode(); V t135 = allocNode(); V t136 = allocNode(); 
    V t137 = allocNode(); V t138 = allocNode(); V t139 = allocNode(); V t140 = allocNode(); 
    tic;
    PROFILE(  0, t0 , pfnLoadTable(t0, initLiteralSym((S)"customer")));
    PROFILE(  1, t7 , pfnColumnValue(t7, t0, initLiteralSym((S)"c_mktsegment")));
    PROFILE(  2, t9 , pfnEq(t9, t7, initLiteralSym((S)"BUILDING")));
    PROFILE(  3, t18, pfnLoadTable(t18, initLiteralSym((S)"orders")));
    PROFILE(  4, t19, pfnColumnValue(t19, t18, initLiteralSym((S)"o_orderkey")));
    PROFILE(  5, t20, pfnColumnValue(t20, t18, initLiteralSym((S)"o_custkey")));
    PROFILE(  6, t23, pfnColumnValue(t23, t18, initLiteralSym((S)"o_orderdate")));
    PROFILE(  7, t26, pfnColumnValue(t26, t18, initLiteralSym((S)"o_shippriority")));
    PROFILE(  8, t28, pfnLt(t28, t23, initLiteralDate(19950315)));
    PROFILE(  9, t38, pfnValues(t38, t20));
    PROFILE( 10, t39, pfnCompress(t39, t28, t38));
    PROFILE( 11, t40, pfnIndex(t40, t9, t39));
    PROFILE( 12, t42, pfnWhere(t42, t28));
    PROFILE( 13, t43, pfnCompress(t43, t40, t42));
    PROFILE( 14, t52, pfnIndex(t52, t19, t43));
    PROFILE( 15, t56, pfnIndex(t56, t23, t43));
    PROFILE( 16, t59, pfnIndex(t59, t26, t43));
    PROFILE( 17, t61, pfnLoadTable(t61, initLiteralSym((S)"lineitem")));
    PROFILE( 18, t62, pfnColumnValue(t62, t61, initLiteralSym((S)"l_orderkey")));
    PROFILE( 19, t67, pfnColumnValue(t67, t61, initLiteralSym((S)"l_extendedprice")));
    PROFILE( 20, t68, pfnColumnValue(t68, t61, initLiteralSym((S)"l_discount")));
    PROFILE( 21, t72, pfnColumnValue(t72, t61, initLiteralSym((S)"l_shipdate")));
    PROFILE( 22, t78, pfnGt(t78, t72, initLiteralDate(19950315)));
    PROFILE( 23, k0 , pfnFetch(k0, t62));
    PROFILE( 24, t85, q3_peephole_0((V []){t79,t84,t85},t78,(V []){k0,t67,t68}));
    PROFILE( 25, t98, pfnList(t98, 3, (V []){t52 ,t56 ,t59}));
    // LINE 26: size 3x147126
    P("LINE 26: size 3x%lld\n", vn(t52));
    PROFILE( 26, t99, pfnGroup(t99, t98));  // <---
    PROFILE( 27, t100, pfnKeys(t100, t99));
    PROFILE( 28, t101, pfnValues(t101, t99));
    PROFILE( 29, t102, pfnIndex(t102, t52, t100));
    PROFILE( 30, t103, pfnIndex(t103, t56, t100));
    PROFILE( 31, t104, pfnIndex(t104, t59, t100));
    PROFILE( 32, t105, pfnEach(t105,t101,pfnLen));
    PROFILE( 33, t106, pfnRaze(t106, t105));
    PROFILE( 34, t108, q3_loopfusion_0(t108,(V []){t84,t85}));
    PROFILE( 35, t109, pfnList(t109, 1, (V []){t79}));
    // LINE 36: size 1x3241776
    P("LINE 36: size 1x%lld\n", vn(t79));
    PROFILE( 36, t110, pfnGroup(t110, t109));  // <---
    PROFILE( 37, t111, pfnKeys(t111, t110));
    PROFILE( 38, t112, pfnValues(t112, t110));
    PROFILE( 39, t115, q3_peephole_1((V []){t115},(V []){t108},t112));
    PROFILE( 40, t118, pfnIndex(t118, t79, t111));
    // LINE 41: join index (147126, 829958)
    P("LINE 41: join index (%lld, %lld)\n", vn(t102),vn(t118));
    PROFILE( 41, t119, pfnJoinIndex(t119,t102,t118,initLiteralSym((S)"eq"))); // <---
    PROFILE( 42, t120, pfnIndex(t120, t119, initLiteralI64(0)));
    PROFILE( 43, t121, pfnIndex(t121, t119, initLiteralI64(1)));
    PROFILE( 44, t122, pfnIndex(t122, t106, t120));
    PROFILE( 45, t123, pfnIndex(t123, t102, t120));
    PROFILE( 46, t124, pfnIndex(t124, t103, t120));
    PROFILE( 47, t125, pfnIndex(t125, t104, t120));
    PROFILE( 48, t127, pfnIndex(t127, t115, t121));
    PROFILE( 49, t131, pfnMul(t131, t127, t122));
    PROFILE( 50, t132, pfnList(t132, 2, (V []){t131 ,t124}));
    PROFILE( 51, t133, pfnOrderBy(t133, t132, initLiteralBoolVector(2, (B []){0,1})));
    PROFILE( 52, t134, pfnIndex(t134, t123, t133));
    PROFILE( 53, t135, pfnIndex(t135, t131, t133));
    PROFILE( 54, t136, pfnIndex(t136, t124, t133));
    PROFILE( 55, t137, pfnIndex(t137, t125, t133));
    PROFILE( 56, t138, copyV(t138, initLiteralSymVector(4, (S []){"l_orderkey","revenue","o_orderdate","o_shippriority"})));
    PROFILE( 57, t139, pfnList(t139, 4, (V []){t134 ,t135 ,t136 ,t137}));
    PROFILE( 58, t140, pfnTable(t140, t138, t139));
    toc;
    printV2(t140,20);
    R elapsed;
}
