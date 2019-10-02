/*==== FE: Loop fusion with elementwise functions ====*/
/* num_func = 3, targ = t133 */
L q7_loopfusion_0(V z, V *x){
    // z -> t133
    V x0 = x[0]; // t125
    initV(z,H_B,vn(x0));
    DOP(vn(x0), vB(z,i)=AND(GEQ(vD(x0,i),19950101),LEQ(vD(x0,i),19961231))) R 0;
}
/* num_func = 2, targ = t251 */
L q7_loopfusion_1(V z, V *x){
    // z -> t251
    V x0 = x[0]; // t238
    V x1 = x[1]; // t239
    initV(z,H_E,vn(x0));
    DOP(vn(x0), vE(z,i)=MUL(vE(x0,i),MINUS(1,vE(x1,i)))) R 0;
}
/*==== FP: Loop fusion with patterns ====*/
L q7_peephole_0(V *z, V y, V *x){
    V z0 = z[0]; // t134
    V z1 = z[1]; // t136
    V z2 = z[2]; // t139
    V z3 = z[3]; // t140
    V z4 = z[4]; // t144
    V x0 = x[0]; // k0
    V x1 = x[1]; // t117
    V x2 = x[2]; // t120
    V x3 = x[3]; // t121
    V x4 = x[4]; // t125
    //getInfoVar(x0); getInfoVar(x1); getInfoVar(x2); getInfoVar(x3); getInfoVar(x4); getchar();
    //Variable x0 has type H_I and len 6001215
    //Variable x1 has type H_I and len 6001215
    //Variable x2 has type H_E and len 6001215
    //Variable x3 has type H_E and len 6001215
    //Variable x4 has type H_D and len 6001215
    L len = vn(y), k = 0;
    L lenZ = 0, parZ[H_CORE], offset[H_CORE];
    DOI(H_CORE, parZ[i]=offset[i]=0)
    CHECKE(getNumOfNonZero(y,parZ));
    DOI(H_CORE, lenZ += parZ[i])
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    initV(z0,vp(x0),lenZ);
    initV(z1,vp(x1),lenZ);
    initV(z2,vp(x2),lenZ);
    initV(z3,vp(x3),lenZ);
    initV(z4,vp(x4),lenZ);
    DOT(len, if(vB(y,i)){L c=offset[tid]++;vLL(z0,c)=vLL(x0,i);vLL(z1,c)=vLL(x1,i);vE(z2,c)=vE(x2,i);vE(z3,c)=vE(x3,i);vD(z4,c)=vD(x4,i);}) R 0;
}
L q7_peephole_1(V *z, V *x, V y){
    L r0 = vn(y);// y -> t255
    V z0 = z[0]; // t261
    V x0 = x[0]; // t251
    initV(z0, H_E, r0);
    DOJ(r0, {V t=vV(y,j);  E c0=0; \
        DOP(vn(t), {L k=vL(t,i); c0+=vE(x0,k);},\
        reduction(+:c0)) \
        vE(z0,j)=c0; \
    })
    R 0;
}
// Pattern FP3
// Pattern FP4
// Pattern FP5
E compiled_main(){
    E elapsed=0;
    V t0   = allocNode(); V t1   = allocNode(); V t4   = allocNode(); V t8   = allocNode(); 
    V t9   = allocNode(); V t10  = allocNode(); V t13  = allocNode(); V t14  = allocNode(); 
    V t15  = allocNode(); V t18  = allocNode(); V t19  = allocNode(); V t20  = allocNode(); 
    V t21  = allocNode(); V t22  = allocNode(); V t23  = allocNode(); V t24  = allocNode(); 
    V t25  = allocNode(); V t26  = allocNode(); V t27  = allocNode(); V t28  = allocNode(); 
    V t29  = allocNode(); V t30  = allocNode(); V t31  = allocNode(); V t32  = allocNode(); 
    V t33  = allocNode(); V t34  = allocNode(); V t35  = allocNode(); V t36  = allocNode(); 
    V t37  = allocNode(); V t41  = allocNode(); V t42  = allocNode(); V t46  = allocNode(); 
    V t47  = allocNode(); V t50  = allocNode(); V t55  = allocNode(); V t56  = allocNode(); 
    V t57  = allocNode(); V t58  = allocNode(); V t60  = allocNode(); V t63  = allocNode(); 
    V t64  = allocNode(); V t67  = allocNode(); V t75  = allocNode(); V t76  = allocNode(); 
    V t77  = allocNode(); V t85  = allocNode(); V t86  = allocNode(); V t87  = allocNode(); 
    V t88  = allocNode(); V t90  = allocNode(); V t93  = allocNode(); V t94  = allocNode(); 
    V t105 = allocNode(); V t114 = allocNode(); V t115 = allocNode(); V t117 = allocNode(); 
    V t120 = allocNode(); V t121 = allocNode(); V t125 = allocNode(); V t131 = allocNode(); 
    V t132 = allocNode(); V t133 = allocNode(); V k0   = allocNode(); V t134 = allocNode(); 
    V t136 = allocNode(); V t139 = allocNode(); V t140 = allocNode(); V t144 = allocNode(); 
    V t151 = allocNode(); V t152 = allocNode(); V t153 = allocNode(); V t155 = allocNode(); 
    V t158 = allocNode(); V t159 = allocNode(); V t181 = allocNode(); V t184 = allocNode(); 
    V t185 = allocNode(); V t189 = allocNode(); V t195 = allocNode(); V t196 = allocNode(); 
    V t197 = allocNode(); V t198 = allocNode(); V t200 = allocNode(); V t209 = allocNode(); 
    V t213 = allocNode(); V t238 = allocNode(); V t239 = allocNode(); V t243 = allocNode(); 
    V t249 = allocNode(); V t250 = allocNode(); V t251 = allocNode(); V t252 = allocNode(); 
    V t253 = allocNode(); V t254 = allocNode(); V t255 = allocNode(); V t256 = allocNode(); 
    V t257 = allocNode(); V t258 = allocNode(); V t259 = allocNode(); V t260 = allocNode(); 
    V t261 = allocNode(); V t262 = allocNode(); V t263 = allocNode(); V t264 = allocNode(); 
    V t265 = allocNode(); V t266 = allocNode(); V t267 = allocNode(); V t268 = allocNode(); 
    V t269 = allocNode(); V t270 = allocNode(); 
    tic;
    PROFILE(  0, t0 , pfnLoadTable(t0, initLiteralSym((S)"supplier")));
    PROFILE(  1, t1 , pfnColumnValue(t1, t0, initLiteralSym((S)"s_suppkey")));
    PROFILE(  2, t4 , pfnColumnValue(t4, t0, initLiteralSym((S)"s_nationkey")));
    PROFILE(  3, t8 , pfnLoadTable(t8, initLiteralSym((S)"nation")));
    PROFILE(  4, t9 , pfnColumnValue(t9, t8, initLiteralSym((S)"n_nationkey")));
    PROFILE(  5, t10, pfnColumnValue(t10, t8, initLiteralSym((S)"n_name")));
    PROFILE(  6, t13, pfnLoadTable(t13, initLiteralSym((S)"nation")));
    PROFILE(  7, t14, pfnColumnValue(t14, t13, initLiteralSym((S)"n_nationkey")));
    PROFILE(  8, t15, pfnColumnValue(t15, t13, initLiteralSym((S)"n_name")));
    PROFILE(  9, t18, pfnEq(t18, t10, initLiteralSym((S)"GERMANY")));
    PROFILE( 10, t19, pfnEq(t19, t15, initLiteralSym((S)"FRANCE")));
    PROFILE( 11, t20, pfnWhere(t20, t18));
    PROFILE( 12, t21, pfnWhere(t21, t19));
    PROFILE( 13, t22, pfnLen(t22, t20));
    PROFILE( 14, t23, pfnLen(t23, t21));
    PROFILE( 15, t24, pfnVector(t24, t23, t20));
    PROFILE( 16, t25, pfnVector(t25, t22, t21));
    PROFILE( 17, t26, pfnEq(t26, t10, initLiteralSym((S)"FRANCE")));
    PROFILE( 18, t27, pfnEq(t27, t15, initLiteralSym((S)"GERMANY")));
    PROFILE( 19, t28, pfnWhere(t28, t26));
    PROFILE( 20, t29, pfnWhere(t29, t27));
    PROFILE( 21, t30, pfnLen(t30, t28));
    PROFILE( 22, t31, pfnLen(t31, t29));
    PROFILE( 23, t32, pfnVector(t32, t31, t28));
    PROFILE( 24, t33, pfnVector(t33, t30, t29));
    PROFILE( 25, t34, pfnAppend(t34, t24, t32));
    PROFILE( 26, t35, pfnAppend(t35, t25, t33));
    PROFILE( 27, t36, pfnIndex(t36, t9, t34));
    PROFILE( 28, t37, pfnIndex(t37, t10, t34));
    PROFILE( 29, t41, pfnIndex(t41, t14, t35));
    PROFILE( 30, t42, pfnIndex(t42, t15, t35));
    PROFILE( 31, t46, pfnLoadTable(t46, initLiteralSym((S)"customer")));
    PROFILE( 32, t47, pfnColumnValue(t47, t46, initLiteralSym((S)"c_custkey")));
    PROFILE( 33, t50, pfnColumnValue(t50, t46, initLiteralSym((S)"c_nationkey")));
    PROFILE( 34, t55, pfnFetch(t55, t50));
    PROFILE( 35, t56, pfnJoinIndex(t56,t36,t55,initLiteralSym((S)"eq")));
    PROFILE( 36, t57, pfnIndex(t57, t56, initLiteralI64(0)));
    PROFILE( 37, t58, pfnIndex(t58, t56, initLiteralI64(1)));
    PROFILE( 38, t60, pfnIndex(t60, t37, t57));
    PROFILE( 39, t63, pfnIndex(t63, t41, t57));
    PROFILE( 40, t64, pfnIndex(t64, t42, t57));
    PROFILE( 41, t67, pfnIndex(t67, t47, t58));
    PROFILE( 42, t75, pfnLoadTable(t75, initLiteralSym((S)"orders")));
    PROFILE( 43, t76, pfnColumnValue(t76, t75, initLiteralSym((S)"o_orderkey")));
    PROFILE( 44, t77, pfnColumnValue(t77, t75, initLiteralSym((S)"o_custkey")));
    PROFILE( 45, t85, pfnFetch(t85, t77));
    //getInfoVar(t67); getInfoVar(t85); getchar();
    // Variable t67 has type H_L and len 12008
    // Variable t85 has type H_L and len 1500000
    PROFILE( 46, t86, pfnJoinIndex(t86,t67,t85,initLiteralSym((S)"eq"))); // <-- expensive
    PROFILE( 47, t87, pfnIndex(t87, t86, initLiteralI64(0)));
    PROFILE( 48, t88, pfnIndex(t88, t86, initLiteralI64(1)));
    PROFILE( 49, t90, pfnIndex(t90, t60, t87));
    PROFILE( 50, t93, pfnIndex(t93, t63, t87));
    PROFILE( 51, t94, pfnIndex(t94, t64, t87));
    PROFILE( 52, t105, pfnIndex(t105, t76, t88));
    PROFILE( 53, t114, pfnLoadTable(t114, initLiteralSym((S)"lineitem")));
    PROFILE( 54, t115, pfnColumnValue(t115, t114, initLiteralSym((S)"l_orderkey")));
    PROFILE( 55, t117, pfnColumnValue(t117, t114, initLiteralSym((S)"l_suppkey")));
    PROFILE( 56, t120, pfnColumnValue(t120, t114, initLiteralSym((S)"l_extendedprice")));
    PROFILE( 57, t121, pfnColumnValue(t121, t114, initLiteralSym((S)"l_discount")));
    PROFILE( 58, t125, pfnColumnValue(t125, t114, initLiteralSym((S)"l_shipdate")));
    PROFILE( 59, t133, q7_loopfusion_0(t133,(V []){t125}));
    PROFILE( 60, k0 , pfnFetch(k0, t115));
    PROFILE( 61, t144, q7_peephole_0((V []){t134,t136,t139,t140,t144},t133,(V []){k0,t117,t120,t121,t125}));
    //getInfoVar(t105); getInfoVar(t134); getchar();
    // Variable t105 has type H_L and len 121324  (correct)
    // Variable t134 has type H_L and len 1828450
    PROFILE( 62, t151, pfnJoinIndex(t151,t105,t134,initLiteralSym((S)"eq"))); // <-- expensive
    PROFILE( 63, t152, pfnIndex(t152, t151, initLiteralI64(0)));
    PROFILE( 64, t153, pfnIndex(t153, t151, initLiteralI64(1)));
    PROFILE( 65, t155, pfnIndex(t155, t90, t152));
    PROFILE( 66, t158, pfnIndex(t158, t93, t152));
    PROFILE( 67, t159, pfnIndex(t159, t94, t152));
    PROFILE( 68, t181, pfnIndex(t181, t136, t153));
    PROFILE( 69, t184, pfnIndex(t184, t139, t153));
    PROFILE( 70, t185, pfnIndex(t185, t140, t153));
    PROFILE( 71, t189, pfnIndex(t189, t144, t153));
    PROFILE( 72, t195, pfnFetch(t195, t4));
    PROFILE( 73, t196, pfnList(t196, 2, (V []){t195 ,t1}));
    PROFILE( 74, t197, pfnList(t197, 2, (V []){t158 ,t181}));
    //getInfoVar(t195); getInfoVar(t1);
    // Variable t195 has type H_I and len 10000
    // Variable t1 has type H_I and len 10000
    //getInfoVar(t158); getInfoVar(t181); getchar();
    // Variable t158 has type H_I and len 148370 (correct)
    // Variable t181 has type H_I and len 148370
    PROFILE( 75, t198, pfnJoinIndex(t198,t196,t197,initLiteralSym((S)"eq")));
    PROFILE( 76, t200, pfnIndex(t200, t198, initLiteralI64(1)));
    //getInfoVar(t200); getchar(); // 5924
    PROFILE( 77, t209, pfnIndex(t209, t155, t200));
    PROFILE( 78, t213, pfnIndex(t213, t159, t200));
    PROFILE( 79, t238, pfnIndex(t238, t184, t200));
    PROFILE( 80, t239, pfnIndex(t239, t185, t200));
    PROFILE( 81, t243, pfnIndex(t243, t189, t200));
    PROFILE( 82, t249, pfnDateYear(t249, t243));
    PROFILE( 83, t251, q7_loopfusion_1(t251,(V []){t238,t239}));
    PROFILE( 84, t252, pfnList(t252, 3, (V []){t213 ,t209 ,t249}));
    //getInfoVar(t213); getInfoVar(t209); getInfoVar(t249); getchar(); // expect size: 5924
    // Variable t213 has type H_Q and len 5924
    // Variable t209 has type H_Q and len 5924
    // Variable t249 has type H_H and len 5924
    PROFILE( 85, t253, pfnGroup(t253, t252));
    PROFILE( 86, t254, pfnKeys(t254, t253));
    PROFILE( 87, t255, pfnValues(t255, t253));
    PROFILE( 88, t256, pfnIndex(t256, t213, t254));
    PROFILE( 89, t257, pfnIndex(t257, t209, t254));
    PROFILE( 90, t258, pfnIndex(t258, t249, t254));
    //getInfoVar(t251); getchar();
    PROFILE( 91, t261, q7_peephole_1((V []){t261},(V []){t251},t255));
    PROFILE( 92, t262, pfnList(t262, 3, (V []){t256 ,t257 ,t258}));
    PROFILE( 93, t263, pfnOrderBy(t263, t262, initLiteralBoolVector(3, (B []){1,1,1})));
    PROFILE( 94, t264, pfnIndex(t264, t256, t263));
    PROFILE( 95, t265, pfnIndex(t265, t257, t263));
    PROFILE( 96, t266, pfnIndex(t266, t258, t263));
    PROFILE( 97, t267, pfnIndex(t267, t261, t263));
    PROFILE( 98, t268, copyV(t268, initLiteralSymVector(4, (S []){"supp_nation","cust_nation","l_year","revenue"})));
    PROFILE( 99, t269, pfnList(t269, 4, (V []){t264 ,t265 ,t266 ,t267}));
    PROFILE(100, t270, pfnTable(t270, t268, t269));
    toc;
    printV2(t270,20);
    R elapsed;
}
