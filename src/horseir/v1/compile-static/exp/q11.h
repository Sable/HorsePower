/*==== FE: Loop fusion with elementwise functions ====*/
/* num_func = 2, targ = t136 */
L q11_loopfusion_0(V z, V *x){
    // z -> t136
    V x0 = x[0]; // t63
    V x1 = x[1]; // t135
    //getInfoVar(x0); getInfoVar(x1); getchar();
    //Variable x0 has type H_E and len 1
    //Variable x1 has type H_E and len 29818
    initV(z,H_B,vn(x1));
    DOP(vn(x1), vB(z,i)=LT(MUL(ve(x0),0.0001),vE(x1,i))) R 0;
}
/*==== FP: Loop fusion with patterns ====*/
L q11_peephole_0(V *z, V y, V *x){
    V z0 = z[0]; // t141
    V z1 = z[1]; // t142
    V x0 = x[0]; // t132
    V x1 = x[1]; // t135
    //getInfoVar(x0); getInfoVar(x1); getchar();
    //Variable x0 has type H_I and len 29818
    //Variable x1 has type H_E and len 29818
    L len = vn(y), k = 0;
    L lenZ = 0, parZ[H_CORE], offset[H_CORE];
    DOI(H_CORE, parZ[i]=offset[i]=0)
    CHECKE(getNumOfNonZero(y,parZ));
    DOI(H_CORE, lenZ += parZ[i])
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    initV(z0,vp(x0),lenZ);
    initV(z1,vp(x1),lenZ);
    DOT(len, if(vB(y,i)){L c=offset[tid]++;vLL(z0,c)=vLL(x0,i);vE(z1,c)=vE(x1,i);}) R 0;
}
L q11_peephole_1(V *z, V *x, V y){
    L r0 = vn(y);// y -> t131
    V z0 = z[0]; // t135
    V x0 = x[0]; // t127
    initV(z0, H_E, r0);
    /* slow: max=3, min=1
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
L q11_peephole_2(V z, V x, V y){
    // z -> t135, x -> t127, y -> t131
    L r0 = vn(y);
    initV(z, H_L, r0);
    DOP(r0, vL(z,i) = vn(vV(y,i))) R 0;
}
// Pattern FP3
// Pattern FP4
// Pattern FP5
E compiled_main(){
    E elapsed=0;
    V t0   = allocNode(); V t2   = allocNode(); V t5   = allocNode(); V t10  = allocNode(); 
    V t11  = allocNode(); V t14  = allocNode(); V t18  = allocNode(); V t19  = allocNode(); 
    V t21  = allocNode(); V t26  = allocNode(); V t34  = allocNode(); V t36  = allocNode(); 
    V t37  = allocNode(); V t38  = allocNode(); V t40  = allocNode(); V t41  = allocNode(); 
    V t43  = allocNode(); V t59  = allocNode(); V t60  = allocNode(); V t62  = allocNode(); 
    V t63  = allocNode(); V t64  = allocNode(); V t65  = allocNode(); V t67  = allocNode(); 
    V t70  = allocNode(); V t75  = allocNode(); V t76  = allocNode(); V t79  = allocNode(); 
    V t83  = allocNode(); V t84  = allocNode(); V t86  = allocNode(); V t91  = allocNode(); 
    V t99  = allocNode(); V t100 = allocNode(); V t101 = allocNode(); V t102 = allocNode(); 
    V t103 = allocNode(); V t105 = allocNode(); V t106 = allocNode(); V t108 = allocNode(); 
    V t121 = allocNode(); V t122 = allocNode(); V t124 = allocNode(); V t125 = allocNode(); 
    V t127 = allocNode(); V t128 = allocNode(); V t129 = allocNode(); V t130 = allocNode(); 
    V t131 = allocNode(); V t132 = allocNode(); V t133 = allocNode(); V t134 = allocNode(); 
    V t135 = allocNode(); V t136 = allocNode(); V t141 = allocNode(); V t142 = allocNode(); 
    V t144 = allocNode(); V t145 = allocNode(); V t146 = allocNode(); V t147 = allocNode(); 
    V t148 = allocNode(); V t149 = allocNode(); V t150 = allocNode(); 
    tic;
    PROFILE(  0, t0 , pfnLoadTable(t0, initLiteralSym((S)"nation")));
    PROFILE(  1, t2 , pfnColumnValue(t2, t0, initLiteralSym((S)"n_name")));
    PROFILE(  2, t5 , pfnEq(t5, t2, initLiteralSym((S)"GERMANY")));
    PROFILE(  3, t10, pfnLoadTable(t10, initLiteralSym((S)"supplier")));
    PROFILE(  4, t11, pfnColumnValue(t11, t10, initLiteralSym((S)"s_suppkey")));
    PROFILE(  5, t14, pfnColumnValue(t14, t10, initLiteralSym((S)"s_nationkey")));
    PROFILE(  6, t18, pfnValues(t18, t14));
    PROFILE(  7, t19, pfnIndex(t19, t5, t18));
    PROFILE(  8, t21, pfnWhere(t21, t19));
    PROFILE(  9, t26, pfnIndex(t26, t11, t21));
    PROFILE( 10, t34, pfnLoadTable(t34, initLiteralSym((S)"partsupp")));
    PROFILE( 11, t36, pfnColumnValue(t36, t34, initLiteralSym((S)"ps_suppkey")));
    PROFILE( 12, t37, pfnColumnValue(t37, t34, initLiteralSym((S)"ps_availqty")));
    PROFILE( 13, t38, pfnColumnValue(t38, t34, initLiteralSym((S)"ps_supplycost")));
    PROFILE( 14, t40, pfnFetch(t40, t36));
    PROFILE( 15, t41, pfnJoinIndex(t41,t26,t40,initLiteralSym((S)"eq")));
    PROFILE( 16, t43, pfnIndex(t43, t41, initLiteralI64(1)));
    PROFILE( 17, t59, pfnIndex(t59, t37, t43));
    PROFILE( 18, t60, pfnIndex(t60, t38, t43));
    PROFILE( 19, t62, pfnMul(t62, t60, t59));
    PROFILE( 20, t63, pfnSum(t63, t62));
    PROFILE( 21, t65, pfnLoadTable(t65, initLiteralSym((S)"nation")));
    PROFILE( 22, t67, pfnColumnValue(t67, t65, initLiteralSym((S)"n_name")));
    PROFILE( 23, t70, pfnEq(t70, t67, initLiteralSym((S)"GERMANY")));
    PROFILE( 24, t75, pfnLoadTable(t75, initLiteralSym((S)"supplier")));
    PROFILE( 25, t76, pfnColumnValue(t76, t75, initLiteralSym((S)"s_suppkey")));
    PROFILE( 26, t79, pfnColumnValue(t79, t75, initLiteralSym((S)"s_nationkey")));
    PROFILE( 27, t83, pfnValues(t83, t79));
    PROFILE( 28, t84, pfnIndex(t84, t70, t83));
    PROFILE( 29, t86, pfnWhere(t86, t84));
    PROFILE( 30, t91, pfnIndex(t91, t76, t86));
    PROFILE( 31, t99, pfnLoadTable(t99, initLiteralSym((S)"partsupp")));
    PROFILE( 32, t100, pfnColumnValue(t100, t99, initLiteralSym((S)"ps_partkey")));
    PROFILE( 33, t101, pfnColumnValue(t101, t99, initLiteralSym((S)"ps_suppkey")));
    PROFILE( 34, t102, pfnColumnValue(t102, t99, initLiteralSym((S)"ps_availqty")));
    PROFILE( 35, t103, pfnColumnValue(t103, t99, initLiteralSym((S)"ps_supplycost")));
    PROFILE( 36, t105, pfnFetch(t105, t101));
    PROFILE( 37, t106, pfnJoinIndex(t106,t91,t105,initLiteralSym((S)"eq")));
    PROFILE( 38, t108, pfnIndex(t108, t106, initLiteralI64(1)));
    PROFILE( 39, t121, pfnFetch(t121, t100));
    PROFILE( 40, t122, pfnIndex(t122, t121, t108));
    PROFILE( 41, t124, pfnIndex(t124, t102, t108));
    PROFILE( 42, t125, pfnIndex(t125, t103, t108));
    PROFILE( 43, t127, pfnMul(t127, t125, t124));
    PROFILE( 44, t128, pfnList(t128, 1, (V []){t122}));
    PROFILE( 45, t129, pfnGroup(t129, t128));
    PROFILE( 46, t130, pfnKeys(t130, t129));
    PROFILE( 47, t131, pfnValues(t131, t129));
    PROFILE( 48, t132, pfnIndex(t132, t122, t130));
    PROFILE( 49, t135, q11_peephole_1((V []){t135},(V []){t127},t131));
    PROFILE( 50, t136, q11_loopfusion_0(t136,(V []){t63,t135}));
    PROFILE( 51, t142, q11_peephole_0((V []){t141,t142},t136,(V []){t132,t135}));
    PROFILE( 52, t144, pfnList(t144, 1, (V []){t142}));
    PROFILE( 53, t145, pfnOrderBy(t145, t144, initLiteralBool(0)));
    PROFILE( 54, t146, pfnIndex(t146, t141, t145));
    PROFILE( 55, t147, pfnIndex(t147, t142, t145));
    PROFILE( 56, t148, copyV(t148, initLiteralSymVector(2, (S []){"ps_partkey","value"})));
    PROFILE( 57, t149, pfnList(t149, 2, (V []){t146 ,t147}));
    PROFILE( 58, t150, pfnTable(t150, t148, t149));
    toc;
    printV2(t150,20);
    R elapsed;
}
