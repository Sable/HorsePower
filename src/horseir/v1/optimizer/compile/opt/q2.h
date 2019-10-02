/*==== FE: Loop fusion with elementwise functions ====*/
/* num_func = 2, targ = t63 */
L q2_loopfusion_0(V z, V *x){
    // z -> t63
    V x0 = x[0]; // t57
    V x1 = x[1]; // t61
    initV(z,H_B,vn(x0));
    DOP(vn(x0), vB(z,i)=AND(EQ(vL(x0,i),15),vB(x1,i))) R 0;
}
/*==== FP: Loop fusion with patterns ====*/
L q2_peephole_0(V *z, V y, V *x){
    V z0 = z[0]; // t64
    V z1 = z[1]; // t66
    V x0 = x[0]; // t52
    V x1 = x[1]; // t54
    L len = vn(y), k = 0;
    L lenZ = 0, parZ[H_CORE], offset[H_CORE];
    DOI(H_CORE, parZ[i]=offset[i]=0)
    CHECKE(getNumOfNonZero(y,parZ));
    DOI(H_CORE, lenZ += parZ[i])
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    initV(z0,vp(x0),lenZ);
    initV(z1,vp(x1),lenZ);
    DOT(len, if(vB(y,i)){L c=offset[tid]++;vL(z0,c)=vL(x0,i);vQ(z1,c)=vQ(x1,i);}) R 0;
}
/* calculate min */
L q2_peephole_1(V *z, V *x, V y){
    L r0 = vn(y);// y -> t163
    V z0 = z[0]; // t166
    V x0 = x[0]; // t158
    //P("type: x0=%lld, y=%lld, %d\n",vp(x0),vp(y), H_E); getchar();
    /* total = 117422, max = 4, min = 1 */
    initV(z0, H_E, r0);
    /* slow version */
    //DOJ(r0, {V t=vV(y,j); E c0=0;\
    //    DOP(vn(t), {L k=vL(t,i); if(c0>vE(x0,k)) c0=vE(x0,k);}, reduction(+:c0))\
    //    vE(z0,j)=c0;})
    /* fast version */
    DOP(r0, {V t=vV(y,i); E c0=DBL_MAX;\
        DOJ(vn(t), {L k=vL(t,j); if(c0>vE(x0,k))c0=vE(x0,k);}) vE(z0,i)=c0;})
    R 0;
}

// Pattern FP3
// Pattern FP4
// Pattern FP5
E compiledQ2(){
    E elapsed=0;
    V t0   = allocNode(); V t2   = allocNode(); V t4   = allocNode(); V t8   = allocNode(); 
    V t9   = allocNode(); V t10  = allocNode(); V t11  = allocNode(); V t13  = allocNode(); 
    V t14  = allocNode(); V t16  = allocNode(); V t20  = allocNode(); V t21  = allocNode(); 
    V t25  = allocNode(); V t26  = allocNode(); V t27  = allocNode(); V t28  = allocNode(); 
    V t29  = allocNode(); V t30  = allocNode(); V t31  = allocNode(); V t32  = allocNode(); 
    V t33  = allocNode(); V t34  = allocNode(); V t35  = allocNode(); V t36  = allocNode(); 
    V t41  = allocNode(); V t44  = allocNode(); V t45  = allocNode(); V t46  = allocNode(); 
    V t48  = allocNode(); V t49  = allocNode(); V t50  = allocNode(); V t51  = allocNode(); 
    V t52  = allocNode(); V t54  = allocNode(); V t56  = allocNode(); V t57  = allocNode(); 
    V t61  = allocNode(); V t62  = allocNode(); V t63  = allocNode(); V t64  = allocNode(); 
    V t66  = allocNode(); V t73  = allocNode(); V t75  = allocNode(); V t77  = allocNode(); 
    V t81  = allocNode(); V t82  = allocNode(); V t84  = allocNode(); V t86  = allocNode(); 
    V t87  = allocNode(); V t89  = allocNode(); V t93  = allocNode(); V t98  = allocNode(); 
    V t99  = allocNode(); V t102 = allocNode(); V t106 = allocNode(); V t107 = allocNode(); 
    V t109 = allocNode(); V t117 = allocNode(); V t130 = allocNode(); V t131 = allocNode(); 
    V t132 = allocNode(); V t134 = allocNode(); V t136 = allocNode(); V t137 = allocNode(); 
    V t139 = allocNode(); V t154 = allocNode(); V t155 = allocNode(); V t158 = allocNode(); 
    V t160 = allocNode(); V t161 = allocNode(); V t162 = allocNode(); V t163 = allocNode(); 
    V t164 = allocNode(); V t165 = allocNode(); V t166 = allocNode(); V t167 = allocNode(); 
    V t168 = allocNode(); V t169 = allocNode(); V t170 = allocNode(); V t171 = allocNode(); 
    V t173 = allocNode(); V t180 = allocNode(); V t183 = allocNode(); V t184 = allocNode(); 
    V t185 = allocNode(); V t187 = allocNode(); V t189 = allocNode(); V t190 = allocNode(); 
    V t191 = allocNode(); V t192 = allocNode(); V t193 = allocNode(); V t194 = allocNode(); 
    V t195 = allocNode(); V t197 = allocNode(); V t208 = allocNode(); V t209 = allocNode(); 
    V t213 = allocNode(); V t214 = allocNode(); V t215 = allocNode(); V t220 = allocNode(); 
    V t224 = allocNode(); V t225 = allocNode(); V t227 = allocNode(); V t228 = allocNode(); 
    V t229 = allocNode(); V t230 = allocNode(); V t232 = allocNode(); V t247 = allocNode(); 
    V t248 = allocNode(); V t249 = allocNode(); V t250 = allocNode(); V t251 = allocNode(); 
    V t252 = allocNode(); V t253 = allocNode(); V t254 = allocNode(); V t255 = allocNode(); 
    V t256 = allocNode(); V t257 = allocNode(); V t258 = allocNode(); V t259 = allocNode(); 
    tic;
    PROFILE(  0, t0 , pfnLoadTable(t0, initLiteralSym((S)"region")));
    PROFILE(  1, t2 , pfnColumnValue(t2, t0, initLiteralSym((S)"r_name")));
    PROFILE(  2, t4 , pfnEq(t4, t2, initLiteralSym((S)"EUROPE")));
    PROFILE(  3, t8 , pfnLoadTable(t8, initLiteralSym((S)"nation")));
    PROFILE(  4, t9 , pfnColumnValue(t9, t8, initLiteralSym((S)"n_nationkey")));
    PROFILE(  5, t10, pfnColumnValue(t10, t8, initLiteralSym((S)"n_name")));
    PROFILE(  6, t11, pfnColumnValue(t11, t8, initLiteralSym((S)"n_regionkey")));
    PROFILE(  7, t13, pfnValues(t13, t11));
    PROFILE(  8, t14, pfnIndex(t14, t4, t13));
    PROFILE(  9, t16, pfnWhere(t16, t14));
    PROFILE( 10, t20, pfnIndex(t20, t9, t16));
    PROFILE( 11, t21, pfnIndex(t21, t10, t16));
    PROFILE( 12, t25, pfnLoadTable(t25, initLiteralSym((S)"supplier")));
    PROFILE( 13, t26, pfnColumnValue(t26, t25, initLiteralSym((S)"s_suppkey")));
    PROFILE( 14, t27, pfnColumnValue(t27, t25, initLiteralSym((S)"s_name")));
    PROFILE( 15, t28, pfnColumnValue(t28, t25, initLiteralSym((S)"s_address")));
    PROFILE( 16, t29, pfnColumnValue(t29, t25, initLiteralSym((S)"s_nationkey")));
    PROFILE( 17, t30, pfnColumnValue(t30, t25, initLiteralSym((S)"s_phone")));
    PROFILE( 18, t31, pfnColumnValue(t31, t25, initLiteralSym((S)"s_acctbal")));
    PROFILE( 19, t32, pfnColumnValue(t32, t25, initLiteralSym((S)"s_comment")));
    PROFILE( 20, t33, pfnFetch(t33, t29));
    PROFILE( 21, t34, pfnJoinIndex(t34,t20,t33,initLiteralSym((S)"eq")));
    PROFILE( 22, t35, pfnIndex(t35, t34, initLiteralI64(0)));
    PROFILE( 23, t36, pfnIndex(t36, t34, initLiteralI64(1)));
    PROFILE( 24, t41, pfnIndex(t41, t21, t35));
    PROFILE( 25, t44, pfnIndex(t44, t26, t36));
    PROFILE( 26, t45, pfnIndex(t45, t27, t36));
    PROFILE( 27, t46, pfnIndex(t46, t28, t36));
    PROFILE( 28, t48, pfnIndex(t48, t30, t36));
    PROFILE( 29, t49, pfnIndex(t49, t31, t36));
    PROFILE( 30, t50, pfnIndex(t50, t32, t36));
    PROFILE( 31, t51, pfnLoadTable(t51, initLiteralSym((S)"part")));
    PROFILE( 32, t52, pfnColumnValue(t52, t51, initLiteralSym((S)"p_partkey")));
    PROFILE( 33, t54, pfnColumnValue(t54, t51, initLiteralSym((S)"p_mfgr")));
    PROFILE( 34, t56, pfnColumnValue(t56, t51, initLiteralSym((S)"p_type")));
    PROFILE( 35, t57, pfnColumnValue(t57, t51, initLiteralSym((S)"p_size")));
    PROFILE( 36, t61, pfnLike(t61, t56, initLiteralSym((S)"%BRASS")));
    PROFILE( 37, t63, q2_loopfusion_0(t63,(V []){t57,t61}));
    PROFILE( 38, t66, q2_peephole_0((V []){t64,t66},t63,(V []){t52,t54}));
    PROFILE( 39, t73, pfnLoadTable(t73, initLiteralSym((S)"region")));
    PROFILE( 40, t75, pfnColumnValue(t75, t73, initLiteralSym((S)"r_name")));
    PROFILE( 41, t77, pfnEq(t77, t75, initLiteralSym((S)"EUROPE")));
    PROFILE( 42, t81, pfnLoadTable(t81, initLiteralSym((S)"nation")));
    PROFILE( 43, t82, pfnColumnValue(t82, t81, initLiteralSym((S)"n_nationkey")));
    PROFILE( 44, t84, pfnColumnValue(t84, t81, initLiteralSym((S)"n_regionkey")));
    PROFILE( 45, t86, pfnValues(t86, t84));
    PROFILE( 46, t87, pfnIndex(t87, t77, t86));
    PROFILE( 47, t89, pfnWhere(t89, t87));
    PROFILE( 48, t93, pfnIndex(t93, t82, t89));
    PROFILE( 49, t98, pfnLoadTable(t98, initLiteralSym((S)"supplier")));
    PROFILE( 50, t99, pfnColumnValue(t99, t98, initLiteralSym((S)"s_suppkey")));
    PROFILE( 51, t102, pfnColumnValue(t102, t98, initLiteralSym((S)"s_nationkey")));
    PROFILE( 52, t106, pfnFetch(t106, t102));
    PROFILE( 53, t107, pfnJoinIndex(t107,t93,t106,initLiteralSym((S)"eq")));
    PROFILE( 54, t109, pfnIndex(t109, t107, initLiteralI64(1)));
    PROFILE( 55, t117, pfnIndex(t117, t99, t109));
    PROFILE( 56, t130, pfnLoadTable(t130, initLiteralSym((S)"partsupp")));
    PROFILE( 57, t131, pfnColumnValue(t131, t130, initLiteralSym((S)"ps_partkey")));
    PROFILE( 58, t132, pfnColumnValue(t132, t130, initLiteralSym((S)"ps_suppkey")));
    PROFILE( 59, t134, pfnColumnValue(t134, t130, initLiteralSym((S)"ps_supplycost")));
    PROFILE( 60, t136, pfnFetch(t136, t132));
    PROFILE( 61, t137, pfnJoinIndex(t137,t117,t136,initLiteralSym((S)"eq")));
    PROFILE( 62, t139, pfnIndex(t139, t137, initLiteralI64(1)));
    PROFILE( 63, t154, pfnFetch(t154, t131));
    PROFILE( 64, t155, pfnIndex(t155, t154, t139));
    PROFILE( 65, t158, pfnIndex(t158, t134, t139));
    PROFILE( 66, t160, pfnList(t160, 1, (V []){t155}));
    PROFILE( 67, t161, pfnGroup(t161, t160));
    PROFILE( 68, t162, pfnKeys(t162, t161));
    PROFILE( 69, t163, pfnValues(t163, t161));
    // PROFILE( 70, t164, pfnEachRight(t164,t158,t163,pfnIndex));
    // PROFILE( 71, t165, pfnEach(t165,t164,pfnMin));
    // PROFILE( 72, t166, pfnRaze(t166, t165));
    PROFILE( 99, t166, q2_peephole_1((V []){t166},(V []){t158},t163));
    PROFILE( 73, t167, pfnIndex(t167, t155, t162));
    PROFILE( 74, t168, pfnJoinIndex(t168,t64,t167,initLiteralSym((S)"eq")));
    PROFILE( 75, t169, pfnIndex(t169, t168, initLiteralI64(0)));
    PROFILE( 76, t170, pfnIndex(t170, t168, initLiteralI64(1)));
    PROFILE( 77, t171, pfnIndex(t171, t64, t169));
    PROFILE( 78, t173, pfnIndex(t173, t66, t169));
    PROFILE( 79, t180, pfnIndex(t180, t166, t170));
    PROFILE( 80, t183, pfnLoadTable(t183, initLiteralSym((S)"partsupp")));
    PROFILE( 81, t184, pfnColumnValue(t184, t183, initLiteralSym((S)"ps_partkey")));
    PROFILE( 82, t185, pfnColumnValue(t185, t183, initLiteralSym((S)"ps_suppkey")));
    PROFILE( 83, t187, pfnColumnValue(t187, t183, initLiteralSym((S)"ps_supplycost")));
    PROFILE( 84, t189, pfnFetch(t189, t184));
    PROFILE( 85, t190, pfnList(t190, 2, (V []){t180 ,t171}));
    PROFILE( 86, t191, pfnList(t191, 2, (V []){t187 ,t189}));
    PROFILE( 87, t192, pfnJoinIndex(t192,t190,t191,initLiteralSym((S)"eq")));
    PROFILE( 88, t193, pfnIndex(t193, t192, initLiteralI64(0)));
    PROFILE( 89, t194, pfnIndex(t194, t192, initLiteralI64(1)));
    PROFILE( 90, t195, pfnIndex(t195, t171, t193));
    PROFILE( 91, t197, pfnIndex(t197, t173, t193));
    PROFILE( 92, t208, pfnFetch(t208, t185));
    PROFILE( 93, t209, pfnIndex(t209, t208, t194));
    PROFILE( 94, t213, pfnJoinIndex(t213,t44,t209,initLiteralSym((S)"eq")));
    PROFILE( 95, t214, pfnIndex(t214, t213, initLiteralI64(0)));
    PROFILE( 96, t215, pfnIndex(t215, t213, initLiteralI64(1)));
    PROFILE( 97, t220, pfnIndex(t220, t41, t214));
    PROFILE( 98, t224, pfnIndex(t224, t45, t214));
    PROFILE( 99, t225, pfnIndex(t225, t46, t214));
    PROFILE(100, t227, pfnIndex(t227, t48, t214));
    PROFILE(101, t228, pfnIndex(t228, t49, t214));
    PROFILE(102, t229, pfnIndex(t229, t50, t214));
    PROFILE(103, t230, pfnIndex(t230, t195, t215));
    PROFILE(104, t232, pfnIndex(t232, t197, t215));
    PROFILE(105, t247, pfnList(t247, 4, (V []){t228 ,t220 ,t224 ,t230}));
    PROFILE(106, t248, pfnOrderBy(t248, t247, initLiteralBoolVector(4, (B []){0,1,1,1})));
    PROFILE(107, t249, pfnIndex(t249, t228, t248));
    PROFILE(108, t250, pfnIndex(t250, t224, t248));
    PROFILE(109, t251, pfnIndex(t251, t220, t248));
    PROFILE(110, t252, pfnIndex(t252, t230, t248));
    PROFILE(111, t253, pfnIndex(t253, t232, t248));
    PROFILE(112, t254, pfnIndex(t254, t225, t248));
    PROFILE(113, t255, pfnIndex(t255, t227, t248));
    PROFILE(114, t256, pfnIndex(t256, t229, t248));
    PROFILE(115, t257, copyV(t257, initLiteralSymVector(8, (S []){"s_acctbal","s_name","n_name","p_partkey","p_mfgr","s_address","s_phone","s_comment"})));
    PROFILE(116, t258, pfnList(t258, 8, (V []){t249 ,t250 ,t251 ,t252 ,t253 ,t254 ,t255 ,t256}));
    PROFILE(117, t259, pfnTable(t259, t257, t258));
    toc;
    printV2(t259,20);
    R elapsed;
}
