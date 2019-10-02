/*==== FE: Loop fusion with elementwise functions ====*/
/* num_func = 3, targ = t73 */
L q5_loopfusion_0(V z, V *x){
    // z -> t73
    V x0 = x[0]; // t66
    initV(z,H_B,vn(x0));
    DOP(vn(x0), vB(z,i)=AND(GEQ(vD(x0,i),19940101),LT(vD(x0,i),19950101))) R 0;
}
/* num_func = 2, targ = t226 */
L q5_loopfusion_1(V z, V *x){
    // z -> t226
    V x0 = x[0]; // t214
    V x1 = x[1]; // t215
    initV(z,H_E,vn(x0));
    DOP(vn(x0), vE(z,i)=MUL(vE(x0,i),MINUS(1,vE(x1,i)))) R 0;
}
/*==== FP: Loop fusion with patterns ====*/
L q5_peephole_0(V *z, V *x, V y){
    L r0 = vn(y);// y -> t230
    V z0 = z[0]; // t234
    V x0 = x[0]; // t226
    initV(z0, H_E, r0);
    /* L v_min = 9999999, v_max = -1;
    DOI(r0, {V t=vV(y,i); \
            if(vn(t)<v_min)v_min=vn(t); \
            if(vn(t)>v_max)v_max=vn(t);})
    P("max = %lld, min = %lld\n", v_max, v_min); getchar(); */
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
E compiledQ5(){
    E elapsed=0;
    V t0   = allocNode(); V t1   = allocNode(); V t4   = allocNode(); V t8   = allocNode(); 
    V t10  = allocNode(); V t12  = allocNode(); V t16  = allocNode(); V t17  = allocNode(); 
    V t18  = allocNode(); V t19  = allocNode(); V t21  = allocNode(); V t22  = allocNode(); 
    V t24  = allocNode(); V t28  = allocNode(); V t29  = allocNode(); V t33  = allocNode(); 
    V t34  = allocNode(); V t37  = allocNode(); V t42  = allocNode(); V t43  = allocNode(); 
    V t44  = allocNode(); V t45  = allocNode(); V t49  = allocNode(); V t50  = allocNode(); 
    V t53  = allocNode(); V t56  = allocNode(); V t61  = allocNode(); V t62  = allocNode(); 
    V t63  = allocNode(); V t66  = allocNode(); V t71  = allocNode(); V t72  = allocNode(); 
    V t73  = allocNode(); V t74  = allocNode(); V k0   = allocNode(); V t75  = allocNode(); 
    V t84  = allocNode(); V t85  = allocNode(); V t86  = allocNode(); V t90  = allocNode(); 
    V t91  = allocNode(); V t97  = allocNode(); V t102 = allocNode(); V t111 = allocNode(); 
    V t112 = allocNode(); V t114 = allocNode(); V t117 = allocNode(); V t118 = allocNode(); 
    V t128 = allocNode(); V t129 = allocNode(); V t130 = allocNode(); V t131 = allocNode(); 
    V t135 = allocNode(); V t136 = allocNode(); V t142 = allocNode(); V t158 = allocNode(); 
    V t161 = allocNode(); V t162 = allocNode(); V t172 = allocNode(); V t173 = allocNode(); 
    V t174 = allocNode(); V t175 = allocNode(); V t177 = allocNode(); V t189 = allocNode(); 
    V t214 = allocNode(); V t215 = allocNode(); V t225 = allocNode(); V t226 = allocNode(); 
    V t227 = allocNode(); V t228 = allocNode(); V t229 = allocNode(); V t230 = allocNode(); 
    V t231 = allocNode(); V t232 = allocNode(); V t233 = allocNode(); V t234 = allocNode(); 
    V t235 = allocNode(); V t236 = allocNode(); V t237 = allocNode(); V t238 = allocNode(); 
    V t239 = allocNode(); V t240 = allocNode(); V t241 = allocNode(); 
    tic;
    PROFILE(  0, t0 , pfnLoadTable(t0, initLiteralSym((S)"supplier")));
    PROFILE(  1, t1 , pfnColumnValue(t1, t0, initLiteralSym((S)"s_suppkey")));
    PROFILE(  2, t4 , pfnColumnValue(t4, t0, initLiteralSym((S)"s_nationkey")));
    PROFILE(  3, t8 , pfnLoadTable(t8, initLiteralSym((S)"region")));
    PROFILE(  4, t10, pfnColumnValue(t10, t8, initLiteralSym((S)"r_name")));
    PROFILE(  5, t12, pfnEq(t12, t10, initLiteralSym((S)"ASIA")));
    PROFILE(  6, t16, pfnLoadTable(t16, initLiteralSym((S)"nation")));
    PROFILE(  7, t17, pfnColumnValue(t17, t16, initLiteralSym((S)"n_nationkey")));
    PROFILE(  8, t18, pfnColumnValue(t18, t16, initLiteralSym((S)"n_name")));
    PROFILE(  9, t19, pfnColumnValue(t19, t16, initLiteralSym((S)"n_regionkey")));
    PROFILE( 10, t21, pfnValues(t21, t19));
    PROFILE( 11, t22, pfnIndex(t22, t12, t21));
    PROFILE( 12, t24, pfnWhere(t24, t22));
    PROFILE( 13, t28, pfnIndex(t28, t17, t24));
    PROFILE( 14, t29, pfnIndex(t29, t18, t24));
    PROFILE( 15, t33, pfnLoadTable(t33, initLiteralSym((S)"customer")));
    PROFILE( 16, t34, pfnColumnValue(t34, t33, initLiteralSym((S)"c_custkey")));
    PROFILE( 17, t37, pfnColumnValue(t37, t33, initLiteralSym((S)"c_nationkey")));
    PROFILE( 18, t42, pfnFetch(t42, t37));
    PROFILE( 19, t43, pfnJoinIndex(t43,t28,t42,initLiteralSym((S)"eq")));
    PROFILE( 20, t44, pfnIndex(t44, t43, initLiteralI64(0)));
    PROFILE( 21, t45, pfnIndex(t45, t43, initLiteralI64(1)));
    PROFILE( 22, t49, pfnIndex(t49, t28, t44));
    PROFILE( 23, t50, pfnIndex(t50, t29, t44));
    PROFILE( 24, t53, pfnIndex(t53, t34, t45));
    PROFILE( 25, t56, pfnIndex(t56, t42, t45));
    PROFILE( 26, t61, pfnLoadTable(t61, initLiteralSym((S)"orders")));
    PROFILE( 27, t62, pfnColumnValue(t62, t61, initLiteralSym((S)"o_orderkey")));
    PROFILE( 28, t63, pfnColumnValue(t63, t61, initLiteralSym((S)"o_custkey")));
    PROFILE( 29, t66, pfnColumnValue(t66, t61, initLiteralSym((S)"o_orderdate")));
    PROFILE( 30, t73, q5_loopfusion_0(t73,(V []){t66}));
    PROFILE( 31, t74, pfnCompress(t74, t73, t62));
    PROFILE( 32, k0 , pfnFetch(k0, t63));
    PROFILE( 33, t75, pfnCompress(t75, t73, k0));
    PROFILE( 34, t84, pfnJoinIndex(t84,t53,t75,initLiteralSym((S)"eq")));
    PROFILE( 35, t85, pfnIndex(t85, t84, initLiteralI64(0)));
    PROFILE( 36, t86, pfnIndex(t86, t84, initLiteralI64(1)));
    PROFILE( 37, t90, pfnIndex(t90, t49, t85));
    PROFILE( 38, t91, pfnIndex(t91, t50, t85));
    PROFILE( 39, t97, pfnIndex(t97, t56, t85));
    PROFILE( 40, t102, pfnIndex(t102, t74, t86));
    PROFILE( 41, t111, pfnLoadTable(t111, initLiteralSym((S)"lineitem")));
    PROFILE( 42, t112, pfnColumnValue(t112, t111, initLiteralSym((S)"l_orderkey")));
    PROFILE( 43, t114, pfnColumnValue(t114, t111, initLiteralSym((S)"l_suppkey")));
    PROFILE( 44, t117, pfnColumnValue(t117, t111, initLiteralSym((S)"l_extendedprice")));
    PROFILE( 45, t118, pfnColumnValue(t118, t111, initLiteralSym((S)"l_discount")));
    PROFILE( 46, t128, pfnFetch(t128, t112));
    //getInfoVar(t102); getInfoVar(t128); getchar();
    // Variable t102 has type H_L and len 46008
    // Variable t128 has type H_L and len 6001215
    PROFILE( 47, t129, pfnJoinIndex(t129,t102,t128,initLiteralSym((S)"eq"))); // <-- expensive
    PROFILE( 48, t130, pfnIndex(t130, t129, initLiteralI64(0)));
    PROFILE( 49, t131, pfnIndex(t131, t129, initLiteralI64(1)));
    PROFILE( 50, t135, pfnIndex(t135, t90, t130));
    PROFILE( 51, t136, pfnIndex(t136, t91, t130));
    PROFILE( 52, t142, pfnIndex(t142, t97, t130));
    PROFILE( 53, t158, pfnIndex(t158, t114, t131));
    PROFILE( 54, t161, pfnIndex(t161, t117, t131));
    PROFILE( 55, t162, pfnIndex(t162, t118, t131));
    PROFILE( 56, t172, pfnFetch(t172, t4));
    PROFILE( 57, t173, pfnList(t173, 3, (V []){t1 ,t172 ,t172}));
    PROFILE( 58, t174, pfnList(t174, 3, (V []){t158 ,t142 ,t135}));
    PROFILE( 59, t175, pfnJoinIndex(t175,t173,t174,initLiteralSym((S)"eq")));
    PROFILE( 60, t177, pfnIndex(t177, t175, initLiteralI64(1)));
    PROFILE( 61, t189, pfnIndex(t189, t136, t177));
    PROFILE( 62, t214, pfnIndex(t214, t161, t177));
    PROFILE( 63, t215, pfnIndex(t215, t162, t177));
    PROFILE( 64, t226, q5_loopfusion_1(t226,(V []){t214,t215}));
    PROFILE( 65, t227, pfnList(t227, 1, (V []){t189}));
    PROFILE( 66, t228, pfnGroup(t228, t227));
    PROFILE( 67, t229, pfnKeys(t229, t228));
    PROFILE( 68, t230, pfnValues(t230, t228));
    PROFILE( 69, t231, pfnIndex(t231, t189, t229));
    PROFILE( 70, t234, q5_peephole_0((V []){t234},(V []){t226},t230));
    PROFILE( 71, t235, pfnList(t235, 1, (V []){t234}));
    PROFILE( 72, t236, pfnOrderBy(t236, t235, initLiteralBool(0)));
    PROFILE( 73, t237, pfnIndex(t237, t231, t236));
    PROFILE( 74, t238, pfnIndex(t238, t234, t236));
    PROFILE( 75, t239, copyV(t239, initLiteralSymVector(2, (S []){"n_name","revenue"})));
    PROFILE( 76, t240, pfnList(t240, 2, (V []){t237 ,t238}));
    PROFILE( 77, t241, pfnTable(t241, t239, t240));
    toc;
    printV2(t241,20);
    R elapsed;
}
