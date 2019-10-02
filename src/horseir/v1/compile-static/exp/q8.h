/*==== FE: Loop fusion with elementwise functions ====*/
/* num_func = 3, targ = t108 */
L q8_loopfusion_0(V z, V *x){
    // z -> t108
    V x0 = x[0]; // t101
    initV(z,H_B,vn(x0));
    DOP(vn(x0), vB(z,i)=AND(GEQ(vD(x0,i),19950101),LEQ(vD(x0,i),19961231))) R 0;
}
/* num_func = 8, targ = t402 */
//L q8_loopfusion_1(V z, V *x){
//    // z -> t402
//    V x0 = x[0]; // t335
//    V x1 = x[1]; // t360
//    V x2 = x[2]; // t361
//    initV(z,H_E,vn(x0));
//    DOP(vn(x0), vE(z,i)=PLUS(MUL(EQ(vQ(x0,i),getSymbol("BRAZIL")),MUL(vE(x1,i),MINUS(1,vE(x2,i)))),MUL(NOT(EQ(vQ(x0,i),getSymbol("BRAZIL"))),0))) R 0;
//}
L q8_loopfusion_1(V *z, V *x){
    V z0 = z[0]; // t397
    V z1 = z[1]; // t402
    V x0 = x[0]; // t335
    V x1 = x[1]; // t360
    V x2 = x[2]; // t361
    initV(z0,H_E,vn(x0));
    initV(z1,H_E,vn(x0));
    Q id0 = getSymbol("BRAZIL");
    DOP(vn(x0), vE(z1,i)=PLUS(MUL(EQ(vQ(x0,i),id0),(vE(z0,i)=MUL(vE(x1,i),MINUS(1,vE(x2,i))))),MUL(NOT(EQ(vQ(x0,i),id0)),0))) R 0;
}
/*==== FP: Loop fusion with patterns ====*/
L q8_peephole_0(V *z, V y, V *x){
    V z0 = z[0]; // t109
    V z1 = z[1]; // t110
    V z2 = z[2]; // t113
    V x0 = x[0]; // t97
    V x1 = x[1]; // k0
    V x2 = x[2]; // t101
    //getInfoVar(x0); getInfoVar(x1); getInfoVar(x2); getchar();
    //Variable x0 has type H_I and len 1500000
    //Variable x1 has type H_I and len 1500000
    //Variable x2 has type H_D and len 1500000
    L len = vn(y), k = 0;
    L lenZ = 0, parZ[H_CORE], offset[H_CORE];
    DOI(H_CORE, parZ[i]=offset[i]=0)
    CHECKE(getNumOfNonZero(y,parZ));
    DOI(H_CORE, lenZ += parZ[i])
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    initV(z0,vp(x0),lenZ);
    initV(z1,vp(x1),lenZ);
    initV(z2,vp(x2),lenZ);
    DOT(len, if(vB(y,i)){L c=offset[tid]++;vLL(z0,c)=vLL(x0,i);vLL(z1,c)=vLL(x1,i);vD(z2,c)=vD(x2,i);}) R 0;
}
// Pattern FP3
// Pattern FP4
// Pattern FP5
E compiled_main(){
    E elapsed=0;
    V t0   = allocNode(); V t1   = allocNode(); V t2   = allocNode(); V t5   = allocNode(); 
    V t7   = allocNode(); V t9   = allocNode(); V t13  = allocNode(); V t14  = allocNode(); 
    V t16  = allocNode(); V t18  = allocNode(); V t19  = allocNode(); V t21  = allocNode(); 
    V t25  = allocNode(); V t30  = allocNode(); V t31  = allocNode(); V t35  = allocNode(); 
    V t40  = allocNode(); V t41  = allocNode(); V t50  = allocNode(); V t51  = allocNode(); 
    V t52  = allocNode(); V t53  = allocNode(); V t56  = allocNode(); V t57  = allocNode(); 
    V t67  = allocNode(); V t69  = allocNode(); V t79  = allocNode(); V t80  = allocNode(); 
    V t82  = allocNode(); V t85  = allocNode(); V t86  = allocNode(); V t96  = allocNode(); 
    V t97  = allocNode(); V t98  = allocNode(); V t101 = allocNode(); V k0   = allocNode(); 
    V t106 = allocNode(); V t107 = allocNode(); V t108 = allocNode(); V t109 = allocNode(); 
    V t110 = allocNode(); V t113 = allocNode(); V t118 = allocNode(); V t119 = allocNode(); 
    V t120 = allocNode(); V t132 = allocNode(); V t135 = allocNode(); V t136 = allocNode(); 
    V t148 = allocNode(); V t151 = allocNode(); V t156 = allocNode(); V t157 = allocNode(); 
    V t160 = allocNode(); V t165 = allocNode(); V t166 = allocNode(); V t167 = allocNode(); 
    V t179 = allocNode(); V t182 = allocNode(); V t183 = allocNode(); V t197 = allocNode(); 
    V t205 = allocNode(); V t206 = allocNode(); V t211 = allocNode(); V t213 = allocNode(); 
    V t232 = allocNode(); V t235 = allocNode(); V t236 = allocNode(); V t250 = allocNode(); 
    V t263 = allocNode(); V t264 = allocNode(); V t267 = allocNode(); V t271 = allocNode(); 
    V t272 = allocNode(); V t273 = allocNode(); V t295 = allocNode(); V t296 = allocNode(); 
    V t310 = allocNode(); V t326 = allocNode(); V t327 = allocNode(); V t331 = allocNode(); 
    V t332 = allocNode(); V t333 = allocNode(); V t335 = allocNode(); V t360 = allocNode(); 
    V t361 = allocNode(); V t375 = allocNode(); V t395 = allocNode(); V t396 = allocNode(); 
    V t397 = allocNode(); V t398 = allocNode(); V t399 = allocNode(); V t400 = allocNode(); 
    V t401 = allocNode(); V t402 = allocNode(); V t403 = allocNode(); V t404 = allocNode(); 
    V t405 = allocNode(); V t406 = allocNode(); V t407 = allocNode(); V t408 = allocNode(); 
    V t409 = allocNode(); V t410 = allocNode(); V t411 = allocNode(); V t412 = allocNode(); 
    V t413 = allocNode(); V t414 = allocNode(); V t415 = allocNode(); V t416 = allocNode(); 
    V t417 = allocNode(); V t418 = allocNode(); V t419 = allocNode(); V t420 = allocNode(); 
    V t421 = allocNode(); 
    tic;
    PROFILE(  0, t0 , pfnLoadTable(t0, initLiteralSym((S)"nation")));
    PROFILE(  1, t1 , pfnColumnValue(t1, t0, initLiteralSym((S)"n_nationkey")));
    PROFILE(  2, t2 , pfnColumnValue(t2, t0, initLiteralSym((S)"n_name")));
    PROFILE(  3, t5 , pfnLoadTable(t5, initLiteralSym((S)"region")));
    PROFILE(  4, t7 , pfnColumnValue(t7, t5, initLiteralSym((S)"r_name")));
    PROFILE(  5, t9 , pfnEq(t9, t7, initLiteralSym((S)"AMERICA")));
    PROFILE(  6, t13, pfnLoadTable(t13, initLiteralSym((S)"nation")));
    PROFILE(  7, t14, pfnColumnValue(t14, t13, initLiteralSym((S)"n_nationkey")));
    PROFILE(  8, t16, pfnColumnValue(t16, t13, initLiteralSym((S)"n_regionkey")));
    PROFILE(  9, t18, pfnValues(t18, t16));
    PROFILE( 10, t19, pfnIndex(t19, t9, t18));
    PROFILE( 11, t21, pfnWhere(t21, t19));
    PROFILE( 12, t25, pfnIndex(t25, t14, t21));
    PROFILE( 13, t30, pfnLoadTable(t30, initLiteralSym((S)"part")));
    PROFILE( 14, t31, pfnColumnValue(t31, t30, initLiteralSym((S)"p_partkey")));
    PROFILE( 15, t35, pfnColumnValue(t35, t30, initLiteralSym((S)"p_type")));
    PROFILE( 16, t40, pfnEq(t40, t35, initLiteralSym((S)"ECONOMY ANODIZED STEEL")));
    PROFILE( 17, t41, pfnCompress(t41, t40, t31));
    PROFILE( 18, t50, pfnLoadTable(t50, initLiteralSym((S)"lineitem")));
    PROFILE( 19, t51, pfnColumnValue(t51, t50, initLiteralSym((S)"l_orderkey")));
    PROFILE( 20, t52, pfnColumnValue(t52, t50, initLiteralSym((S)"l_partkey")));
    PROFILE( 21, t53, pfnColumnValue(t53, t50, initLiteralSym((S)"l_suppkey")));
    PROFILE( 22, t56, pfnColumnValue(t56, t50, initLiteralSym((S)"l_extendedprice")));
    PROFILE( 23, t57, pfnColumnValue(t57, t50, initLiteralSym((S)"l_discount")));
    //getInfoVar(t41); getInfoVar(t52); getchar();
    // Variable t41 has type H_L and len 1451
    // Variable t52 has type H_L and len 6001215
    PROFILE( 24, t67, pfnJoinIndex(t67,t41,t52,initLiteralSym((S)"eq"))); // <-- expensive
    PROFILE( 25, t69, pfnIndex(t69, t67, initLiteralI64(1)));
    PROFILE( 26, t79, pfnFetch(t79, t51)); // <-- expensive
    PROFILE( 27, t80, pfnIndex(t80, t79, t69));
    PROFILE( 28, t82, pfnIndex(t82, t53, t69));
    PROFILE( 29, t85, pfnIndex(t85, t56, t69));
    PROFILE( 30, t86, pfnIndex(t86, t57, t69));
    PROFILE( 31, t96, pfnLoadTable(t96, initLiteralSym((S)"orders")));
    PROFILE( 32, t97, pfnColumnValue(t97, t96, initLiteralSym((S)"o_orderkey")));
    PROFILE( 33, t98, pfnColumnValue(t98, t96, initLiteralSym((S)"o_custkey")));
    PROFILE( 34, t101, pfnColumnValue(t101, t96, initLiteralSym((S)"o_orderdate")));
    PROFILE( 35, k0 , pfnFetch(k0, t98));
    PROFILE( 36, t108, q8_loopfusion_0(t108,(V []){t101}));
    PROFILE( 37, t113, q8_peephole_0((V []){t109,t110,t113},t108,(V []){t97,k0,t101}));
    //getInfoVar(t109); getInfoVar(t80); getchar(); 
    // Variable t109 has type H_L and len 457263
    // Variable t80 has type H_L and len 43693
    PROFILE( 38, t118, pfnJoinIndex(t118,t109,t80,initLiteralSym((S)"eq"))); // <-- expensive
    PROFILE( 39, t119, pfnIndex(t119, t118, initLiteralI64(0)));
    PROFILE( 40, t120, pfnIndex(t120, t118, initLiteralI64(1)));
    PROFILE( 41, t132, pfnIndex(t132, t82, t120));
    PROFILE( 42, t135, pfnIndex(t135, t85, t120));
    PROFILE( 43, t136, pfnIndex(t136, t86, t120));
    PROFILE( 44, t148, pfnIndex(t148, t110, t119));
    PROFILE( 45, t151, pfnIndex(t151, t113, t119));
    PROFILE( 46, t156, pfnLoadTable(t156, initLiteralSym((S)"customer")));
    PROFILE( 47, t157, pfnColumnValue(t157, t156, initLiteralSym((S)"c_custkey")));
    PROFILE( 48, t160, pfnColumnValue(t160, t156, initLiteralSym((S)"c_nationkey")));
    //getInfoVar(t157); getInfoVar(t148); getchar();
    // Variable t157 has type H_L and len 150000
    // Variable t148 has type H_L and len 13389
    PROFILE( 49, t165, pfnJoinIndex(t165,t157,t148,initLiteralSym((S)"eq"))); // <-- expensive
    PROFILE( 50, t166, pfnIndex(t166, t165, initLiteralI64(0)));
    PROFILE( 51, t167, pfnIndex(t167, t165, initLiteralI64(1)));
    PROFILE( 52, t179, pfnIndex(t179, t132, t167));
    PROFILE( 53, t182, pfnIndex(t182, t135, t167));
    PROFILE( 54, t183, pfnIndex(t183, t136, t167));
    PROFILE( 55, t197, pfnIndex(t197, t151, t167));
    PROFILE( 56, t205, pfnFetch(t205, t160));
    PROFILE( 57, t206, pfnIndex(t206, t205, t166));
    PROFILE( 58, t211, pfnJoinIndex(t211,t25,t206,initLiteralSym((S)"eq")));
    PROFILE( 59, t213, pfnIndex(t213, t211, initLiteralI64(1)));
    PROFILE( 60, t232, pfnIndex(t232, t179, t213));
    PROFILE( 61, t235, pfnIndex(t235, t182, t213));
    PROFILE( 62, t236, pfnIndex(t236, t183, t213));
    PROFILE( 63, t250, pfnIndex(t250, t197, t213));
    PROFILE( 64, t263, pfnLoadTable(t263, initLiteralSym((S)"supplier")));
    PROFILE( 65, t264, pfnColumnValue(t264, t263, initLiteralSym((S)"s_suppkey")));
    PROFILE( 66, t267, pfnColumnValue(t267, t263, initLiteralSym((S)"s_nationkey")));
    PROFILE( 67, t271, pfnJoinIndex(t271,t264,t232,initLiteralSym((S)"eq")));
    PROFILE( 68, t272, pfnIndex(t272, t271, initLiteralI64(0)));
    PROFILE( 69, t273, pfnIndex(t273, t271, initLiteralI64(1)));
    PROFILE( 70, t295, pfnIndex(t295, t235, t273));
    PROFILE( 71, t296, pfnIndex(t296, t236, t273));
    PROFILE( 72, t310, pfnIndex(t310, t250, t273));
    PROFILE( 73, t326, pfnFetch(t326, t267));
    PROFILE( 74, t327, pfnIndex(t327, t326, t272));
    PROFILE( 75, t331, pfnJoinIndex(t331,t1,t327,initLiteralSym((S)"eq")));
    PROFILE( 76, t332, pfnIndex(t332, t331, initLiteralI64(0)));
    PROFILE( 77, t333, pfnIndex(t333, t331, initLiteralI64(1)));
    PROFILE( 78, t335, pfnIndex(t335, t2, t332));
    PROFILE( 79, t360, pfnIndex(t360, t295, t333));
    PROFILE( 80, t361, pfnIndex(t361, t296, t333));
    PROFILE( 81, t375, pfnIndex(t375, t310, t333));
    PROFILE( 82, t395, pfnDateYear(t395, t375));
    //PROFILE( 83, t402, q8_loopfusion_1(t402,(V []){t335,t360,t361}));
    PROFILE( 83, t402, q8_loopfusion_1((V []){t397, t402},(V []){t335,t360,t361}));
    PROFILE( 84, t403, pfnList(t403, 1, (V []){t395}));
    PROFILE( 85, t404, pfnGroup(t404, t403));
    PROFILE( 86, t405, pfnKeys(t405, t404));
    PROFILE( 87, t406, pfnValues(t406, t404));
    //printV2(t405,20); getchar();
    PROFILE( 88, t407, pfnIndex(t407, t395, t405));
    PROFILE( 89, t408, pfnEachRight(t408,t402,t406,pfnIndex));
    PROFILE( 90, t409, pfnEach(t409,t408,pfnSum));
    PROFILE( 91, t410, pfnRaze(t410, t409));
    PROFILE( 92, t411, pfnEachRight(t411,t397,t406,pfnIndex));
    PROFILE( 93, t412, pfnEach(t412,t411,pfnSum));
    PROFILE( 94, t413, pfnRaze(t413, t412));
    PROFILE( 95, t414, pfnDiv(t414, t410, t413));
    PROFILE( 96, t415, pfnList(t415, 1, (V []){t407}));
    PROFILE( 97, t416, pfnOrderBy(t416, t415, initLiteralBool(1)));
    PROFILE( 98, t417, pfnIndex(t417, t407, t416));
    PROFILE( 99, t418, pfnIndex(t418, t414, t416));
    PROFILE(100, t419, copyV(t419, initLiteralSymVector(2, (S []){"o_year","mkt_share"})));
    PROFILE(101, t420, pfnList(t420, 2, (V []){t417 ,t418}));
    PROFILE(102, t421, pfnTable(t421, t419, t420));
    toc;
    printV2(t421,20);
    R elapsed;
}
