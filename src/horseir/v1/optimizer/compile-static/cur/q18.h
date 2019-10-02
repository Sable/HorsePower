/*==== FE: Loop fusion with elementwise functions ====*/
/*==== FP: Loop fusion with patterns ====*/
L q18_peephole_0(V *z, V y, V *x){
    V z0 = z[0]; // t52
    V z1 = z[1]; // t54
    V z2 = z[2]; // t55
    V x0 = x[0]; // t51
    V x1 = x[1]; // t43
    V x2 = x[2]; // t44
    L len = vn(y), k = 0;
    L lenZ = 0, parZ[H_CORE], offset[H_CORE];
    DOI(H_CORE, parZ[i]=offset[i]=0)
    CHECKE(getNumOfNonZero(y,parZ));
    DOI(H_CORE, lenZ += parZ[i])
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    initV(z0,vp(x0),lenZ);
    initV(z1,vp(x1),lenZ);
    initV(z2,vp(x2),lenZ);
    DOT(len, if(vB(y,i)){L c=offset[tid]++;vL(z0,c)=vL(x0,i);vE(z1,c)=vE(x1,i);vD(z2,c)=vD(x2,i);}) R 0;
}
L q18_peephole_1(V *z, V *x, V y){
    L r0 = vn(y);// y -> t30
    V z0 = z[0]; // t34
    V x0 = x[0]; // t14
    initV(z0, H_E, r0);
    /* total = 1500000, max = 7, min = 1 */
    /* slow version */
    //DOJ(r0, {V t=vV(y,j);  E c0=0; \
    //    DOP(vn(t), {L k=vL(t,i); c0+=vE(x0,k);},\
    //    reduction(+:c0)) \
    //    vE(z0,j)=c0; \
    //})
    /* fast version */
    DOP(r0, {V t=vV(y,i); E c0=0; \
        DOJ(vn(t), {L k=vL(t,j); c0+=vE(x0,k);}) \
        vE(z0,i)=c0;})
    R 0;
}
L q18_peephole_2(V *z, V *x, V y){
    L r0 = vn(y);// y -> t138
    V z0 = z[0]; // t146
    V x0 = x[0]; // t123
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
    V t0   = allocNode(); V t1   = allocNode(); V t2   = allocNode(); V t9   = allocNode(); 
    V t10  = allocNode(); V t14  = allocNode(); V t26  = allocNode(); V t27  = allocNode(); 
    V t28  = allocNode(); V t29  = allocNode(); V t30  = allocNode(); V t31  = allocNode(); 
    V t32  = allocNode(); V t33  = allocNode(); V t34  = allocNode(); V t35  = allocNode(); 
    V t36  = allocNode(); V t39  = allocNode(); V t40  = allocNode(); V t41  = allocNode(); 
    V t43  = allocNode(); V t44  = allocNode(); V t49  = allocNode(); V t50  = allocNode(); 
    V t51  = allocNode(); V t52  = allocNode(); V t54  = allocNode(); V t55  = allocNode(); 
    V t60  = allocNode(); V t61  = allocNode(); V t62  = allocNode(); V t63  = allocNode(); 
    V t64  = allocNode(); V t72  = allocNode(); V t75  = allocNode(); V t76  = allocNode(); 
    V t81  = allocNode(); V t82  = allocNode(); V t86  = allocNode(); V t98  = allocNode(); 
    V t99  = allocNode(); V t100 = allocNode(); V t101 = allocNode(); V t102 = allocNode(); 
    V t103 = allocNode(); V t110 = allocNode(); V t113 = allocNode(); V t114 = allocNode(); 
    V t123 = allocNode(); V t135 = allocNode(); V t136 = allocNode(); V t137 = allocNode(); 
    V t138 = allocNode(); V t139 = allocNode(); V t140 = allocNode(); V t141 = allocNode(); 
    V t142 = allocNode(); V t143 = allocNode(); V t144 = allocNode(); V t145 = allocNode(); 
    V t146 = allocNode(); V t147 = allocNode(); V t148 = allocNode(); V t149 = allocNode(); 
    V t150 = allocNode(); V t151 = allocNode(); V t152 = allocNode(); V t153 = allocNode(); 
    V t154 = allocNode(); V t155 = allocNode(); V t156 = allocNode(); V t157 = allocNode(); 
    tic;
    PROFILE(  0, t0 , pfnLoadTable(t0, initLiteralSym((S)"customer")));
    PROFILE(  1, t1 , pfnColumnValue(t1, t0, initLiteralSym((S)"c_custkey")));
    PROFILE(  2, t2 , pfnColumnValue(t2, t0, initLiteralSym((S)"c_name")));
    PROFILE(  3, t9 , pfnLoadTable(t9, initLiteralSym((S)"lineitem")));
    PROFILE(  4, t10, pfnColumnValue(t10, t9, initLiteralSym((S)"l_orderkey")));
    PROFILE(  5, t14, pfnColumnValue(t14, t9, initLiteralSym((S)"l_quantity")));
    PROFILE(  6, t26, pfnFetch(t26, t10));
    PROFILE(  7, t27, pfnList(t27, 1, (V []){t26}));
    //getInfoVar(t26); getchar();
    // Variable t26 has type H_L and len 6001215
    PROFILE(  8, t28, pfnGroup(t28, t27));  // <-- expensive
    PROFILE(  9, t29, pfnKeys(t29, t28));
    PROFILE( 10, t30, pfnValues(t30, t28));
    PROFILE( 11, t31, pfnIndex(t31, t26, t29));
    //getInfoVar(t14); getInfoVar(t30); getchar();
    // Variable t14 has type H_E and len 6001215
    // Variable t30 has type H_G and len 1500000
    PROFILE( 12, t34, q18_peephole_1((V []){t34},(V []){t14},t30));
    PROFILE( 13, t35, pfnGt(t35, t34, initLiteralF64(300)));
    PROFILE( 14, t36, pfnCompress(t36, t35, t31));
    PROFILE( 15, t39, pfnLoadTable(t39, initLiteralSym((S)"orders")));
    PROFILE( 16, t40, pfnColumnValue(t40, t39, initLiteralSym((S)"o_orderkey")));
    PROFILE( 17, t41, pfnColumnValue(t41, t39, initLiteralSym((S)"o_custkey")));
    PROFILE( 18, t43, pfnColumnValue(t43, t39, initLiteralSym((S)"o_totalprice")));
    PROFILE( 19, t44, pfnColumnValue(t44, t39, initLiteralSym((S)"o_orderdate")));
    PROFILE( 20, t49, pfnMember(t49, t36, t40));
    PROFILE( 21, t50, pfnCompress(t50, t49, t40));
    PROFILE( 22, t51, pfnFetch(t51, t41));
    PROFILE( 23, t55, q18_peephole_0((V []){t52,t54,t55},t49,(V []){t51,t43,t44}));
    PROFILE( 24, t60, pfnJoinIndex(t60,t1,t52,initLiteralSym((S)"eq")));
    PROFILE( 25, t61, pfnIndex(t61, t60, initLiteralI64(0)));
    PROFILE( 26, t62, pfnIndex(t62, t60, initLiteralI64(1)));
    PROFILE( 27, t63, pfnIndex(t63, t1, t61));
    PROFILE( 28, t64, pfnIndex(t64, t2, t61));
    PROFILE( 29, t72, pfnIndex(t72, t50, t62));
    PROFILE( 30, t75, pfnIndex(t75, t54, t62));
    PROFILE( 31, t76, pfnIndex(t76, t55, t62));
    PROFILE( 32, t81, pfnLoadTable(t81, initLiteralSym((S)"lineitem")));
    PROFILE( 33, t82, pfnColumnValue(t82, t81, initLiteralSym((S)"l_orderkey")));
    PROFILE( 34, t86, pfnColumnValue(t86, t81, initLiteralSym((S)"l_quantity")));
    PROFILE( 35, t98, pfnFetch(t98, t82));
    //getInfoVar(t72); getInfoVar(t98); getchar();
    // Variable t72 has type H_L and len 57
    // Variable t98 has type H_L and len 6001215
    PROFILE( 36, t99, pfnJoinIndex(t99,t72,t98,initLiteralSym((S)"eq"))); // <-- expensive
    PROFILE( 37, t100, pfnIndex(t100, t99, initLiteralI64(0)));
    PROFILE( 38, t101, pfnIndex(t101, t99, initLiteralI64(1)));
    PROFILE( 39, t102, pfnIndex(t102, t63, t100));
    PROFILE( 40, t103, pfnIndex(t103, t64, t100));
    PROFILE( 41, t110, pfnIndex(t110, t72, t100));
    PROFILE( 42, t113, pfnIndex(t113, t75, t100));
    PROFILE( 43, t114, pfnIndex(t114, t76, t100));
    PROFILE( 44, t123, pfnIndex(t123, t86, t101));
    PROFILE( 45, t135, pfnList(t135, 5, (V []){t103 ,t102 ,t110 ,t114 ,t113}));
    PROFILE( 46, t136, pfnGroup(t136, t135));
    PROFILE( 47, t137, pfnKeys(t137, t136));
    PROFILE( 48, t138, pfnValues(t138, t136));
    PROFILE( 49, t139, pfnIndex(t139, t103, t137));
    PROFILE( 50, t140, pfnIndex(t140, t102, t137));
    PROFILE( 51, t141, pfnIndex(t141, t110, t137));
    PROFILE( 52, t142, pfnIndex(t142, t114, t137));
    PROFILE( 53, t143, pfnIndex(t143, t113, t137));
    PROFILE( 54, t146, q18_peephole_2((V []){t146},(V []){t123},t138));
    PROFILE( 55, t147, pfnList(t147, 2, (V []){t143 ,t142}));
    PROFILE( 56, t148, pfnOrderBy(t148, t147, initLiteralBoolVector(2, (B []){0,1})));
    PROFILE( 57, t149, pfnIndex(t149, t139, t148));
    PROFILE( 58, t150, pfnIndex(t150, t140, t148));
    PROFILE( 59, t151, pfnIndex(t151, t141, t148));
    PROFILE( 60, t152, pfnIndex(t152, t142, t148));
    PROFILE( 61, t153, pfnIndex(t153, t143, t148));
    PROFILE( 62, t154, pfnIndex(t154, t146, t148));
    PROFILE( 63, t155, copyV(t155, initLiteralSymVector(6, (S []){"c_name","c_custkey","o_orderkey","o_orderdate","o_totalprice","sum"})));
    PROFILE( 64, t156, pfnList(t156, 6, (V []){t149 ,t150 ,t151 ,t152 ,t153 ,t154}));
    PROFILE( 65, t157, pfnTable(t157, t155, t156));
    toc;
    printV2(t157,20);
    R elapsed;
}
