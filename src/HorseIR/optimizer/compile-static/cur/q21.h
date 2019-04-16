/*==== FE: Loop fusion with elementwise functions ====*/
/*==== FP: Loop fusion with patterns ====*/
L q21_peephole_0(V *z, V y, V *x){
    V z0 = z[0]; // t52
    V z1 = z[1]; // t54
    V x0 = x[0]; // k0
    V x1 = x[1]; // t37
    L len = vn(y), k = 0;
    L lenZ = 0, parZ[H_CORE], offset[H_CORE];
    DOI(H_CORE, parZ[i]=offset[i]=0)
    CHECKE(getNumOfNonZero(y,parZ));
    DOI(H_CORE, lenZ += parZ[i])
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    initV(z0,vp(x0),lenZ);
    initV(z1,vp(x1),lenZ);
    DOT(len, if(vB(y,i)){L c=offset[tid]++;vL(z0,c)=vL(x0,i);vL(z1,c)=vL(x1,i);}) R 0;
}
L q21_peephole_1(V *z, V y, V *x){
    V z0 = z[0]; // t178
    V z1 = z[1]; // t180
    V x0 = x[0]; // k1
    V x1 = x[1]; // t163
    L len = vn(y), k = 0;
    L lenZ = 0, parZ[H_CORE], offset[H_CORE];
    DOI(H_CORE, parZ[i]=offset[i]=0)
    CHECKE(getNumOfNonZero(y,parZ));
    DOI(H_CORE, lenZ += parZ[i])
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    initV(z0,vp(x0),lenZ);
    initV(z1,vp(x1),lenZ);
    DOT(len, if(vB(y,i)){L c=offset[tid]++;vL(z0,c)=vL(x0,i);vL(z1,c)=vL(x1,i);}) R 0;
}
L q21_peephole_2(V *z, V y, V *x){
    V z0 = z[0]; // t207
    V z1 = z[1]; // t213
    V z2 = z[2]; // t215
    V x0 = x[0]; // t128
    V x1 = x[1]; // t134
    V x2 = x[2]; // t136
    L len = vn(y), k = 0;
    L lenZ = 0, parZ[H_CORE], offset[H_CORE];
    DOI(H_CORE, parZ[i]=offset[i]=0)
    CHECKE(getNumOfNonZero(y,parZ));
    DOI(H_CORE, lenZ += parZ[i])
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    initV(z0,vp(x0),lenZ);
    initV(z1,vp(x1),lenZ);
    initV(z2,vp(x2),lenZ);
    DOT(len, if(vB(y,i)){L c=offset[tid]++;vQ(z0,c)=vQ(x0,i);vL(z1,c)=vL(x1,i);vL(z2,c)=vL(x2,i);}) R 0;
}
// Pattern FP3
// Pattern FP4
// Pattern FP5
E compiled_main(){
    E elapsed=0;
    V t0   = allocNode(); V t2   = allocNode(); V t5   = allocNode(); V t10  = allocNode(); 
    V t11  = allocNode(); V t12  = allocNode(); V t14  = allocNode(); V t18  = allocNode(); 
    V t19  = allocNode(); V t21  = allocNode(); V t26  = allocNode(); V t27  = allocNode(); 
    V t34  = allocNode(); V t35  = allocNode(); V t37  = allocNode(); V t46  = allocNode(); 
    V t47  = allocNode(); V k0   = allocNode(); V t51  = allocNode(); V t52  = allocNode(); 
    V t54  = allocNode(); V t68  = allocNode(); V t69  = allocNode(); V t70  = allocNode(); 
    V t77  = allocNode(); V t84  = allocNode(); V t86  = allocNode(); V t100 = allocNode(); 
    V t101 = allocNode(); V t103 = allocNode(); V t110 = allocNode(); V t111 = allocNode(); 
    V t120 = allocNode(); V t122 = allocNode(); V t128 = allocNode(); V t134 = allocNode(); 
    V t136 = allocNode(); V t160 = allocNode(); V t161 = allocNode(); V t163 = allocNode(); 
    V t172 = allocNode(); V t173 = allocNode(); V k1   = allocNode(); V t177 = allocNode(); 
    V t178 = allocNode(); V t180 = allocNode(); V t195 = allocNode(); V t196 = allocNode(); 
    V t197 = allocNode(); V t198 = allocNode(); V t199 = allocNode(); V t200 = allocNode(); 
    V t201 = allocNode(); V t207 = allocNode(); V t213 = allocNode(); V t215 = allocNode(); 
    V t238 = allocNode(); V t239 = allocNode(); V t241 = allocNode(); V t255 = allocNode(); 
    V t256 = allocNode(); V t257 = allocNode(); V t258 = allocNode(); V t259 = allocNode(); 
    V t260 = allocNode(); V t261 = allocNode(); V t267 = allocNode(); V t298 = allocNode(); 
    V t299 = allocNode(); V t300 = allocNode(); V t301 = allocNode(); V t302 = allocNode(); 
    V t303 = allocNode(); V t304 = allocNode(); V t305 = allocNode(); V t306 = allocNode(); 
    V t307 = allocNode(); V t308 = allocNode(); V t309 = allocNode(); V t310 = allocNode(); 
    V t311 = allocNode(); 
    tic;
    PROFILE(  0, t0 , pfnLoadTable(t0, initLiteralSym((S)"nation")));
    PROFILE(  1, t2 , pfnColumnValue(t2, t0, initLiteralSym((S)"n_name")));
    PROFILE(  2, t5 , pfnEq(t5, t2, initLiteralSym((S)"SAUDI ARABIA")));
    PROFILE(  3, t10, pfnLoadTable(t10, initLiteralSym((S)"supplier")));
    PROFILE(  4, t11, pfnColumnValue(t11, t10, initLiteralSym((S)"s_suppkey")));
    PROFILE(  5, t12, pfnColumnValue(t12, t10, initLiteralSym((S)"s_name")));
    PROFILE(  6, t14, pfnColumnValue(t14, t10, initLiteralSym((S)"s_nationkey")));
    PROFILE(  7, t18, pfnValues(t18, t14));
    PROFILE(  8, t19, pfnIndex(t19, t5, t18));
    PROFILE(  9, t21, pfnWhere(t21, t19));
    PROFILE( 10, t26, pfnIndex(t26, t11, t21));
    PROFILE( 11, t27, pfnIndex(t27, t12, t21));
    PROFILE( 12, t34, pfnLoadTable(t34, initLiteralSym((S)"lineitem")));
    PROFILE( 13, t35, pfnColumnValue(t35, t34, initLiteralSym((S)"l_orderkey")));
    PROFILE( 14, t37, pfnColumnValue(t37, t34, initLiteralSym((S)"l_suppkey")));
    PROFILE( 15, t46, pfnColumnValue(t46, t34, initLiteralSym((S)"l_commitdate")));
    PROFILE( 16, t47, pfnColumnValue(t47, t34, initLiteralSym((S)"l_receiptdate")));
    PROFILE( 17, k0 , pfnFetch(k0, t35));
    PROFILE( 18, t51, pfnGt(t51, t47, t46));
    PROFILE( 19, t54, q21_peephole_0((V []){t52,t54},t51,(V []){k0,t37}));
    PROFILE( 20, t68, pfnJoinIndex(t68,t26,t54,initLiteralSym((S)"eq")));
    PROFILE( 21, t69, pfnIndex(t69, t68, initLiteralI64(0)));
    PROFILE( 22, t70, pfnIndex(t70, t68, initLiteralI64(1)));
    PROFILE( 23, t77, pfnIndex(t77, t27, t69));
    PROFILE( 24, t84, pfnIndex(t84, t52, t70));
    PROFILE( 25, t86, pfnIndex(t86, t54, t70));
    PROFILE( 26, t100, pfnLoadTable(t100, initLiteralSym((S)"orders")));
    PROFILE( 27, t101, pfnColumnValue(t101, t100, initLiteralSym((S)"o_orderkey")));
    PROFILE( 28, t103, pfnColumnValue(t103, t100, initLiteralSym((S)"o_orderstatus")));
    PROFILE( 29, t110, pfnEq(t110, t103, initLiteralChar('F')));
    PROFILE( 30, t111, pfnCompress(t111, t110, t101));
    PROFILE( 31, t120, pfnJoinIndex(t120,t111,t84,initLiteralSym((S)"eq")));
    PROFILE( 32, t122, pfnIndex(t122, t120, initLiteralI64(1)));
    PROFILE( 33, t128, pfnIndex(t128, t77, t122));
    PROFILE( 34, t134, pfnIndex(t134, t84, t122));
    PROFILE( 35, t136, pfnIndex(t136, t86, t122));
    PROFILE( 36, t160, pfnLoadTable(t160, initLiteralSym((S)"lineitem")));
    PROFILE( 37, t161, pfnColumnValue(t161, t160, initLiteralSym((S)"l_orderkey")));
    PROFILE( 38, t163, pfnColumnValue(t163, t160, initLiteralSym((S)"l_suppkey")));
    PROFILE( 39, t172, pfnColumnValue(t172, t160, initLiteralSym((S)"l_commitdate")));
    PROFILE( 40, t173, pfnColumnValue(t173, t160, initLiteralSym((S)"l_receiptdate")));
    PROFILE( 41, k1 , pfnFetch(k1, t161));
    PROFILE( 42, t177, pfnGt(t177, t173, t172));
    PROFILE( 43, t180, q21_peephole_1((V []){t178,t180},t177,(V []){k1,t163}));
    PROFILE( 44, t195, pfnList(t195, 2, (V []){t134 ,t136}));
    PROFILE( 45, t196, pfnList(t196, 2, (V []){t178 ,t180}));
    PROFILE( 46, t197, pfnJoinIndex(t197,t195,t196,initLiteralSymVector(2, (S []){"eq","neq"})));
    PROFILE( 47, t198, pfnIndex(t198, t197, initLiteralI64(0)));
    PROFILE( 48, t199, pfnLen(t199, t134));
    PROFILE( 49, t200, pfnVector(t200, t199, initLiteralBool(1)));
    //PROFILE( 50, t201, pfnIndexA(t200, t198, initLiteralBool(0)));
    PROFILE( 50, t201, pfnIndexA(t201, t200, t198, initLiteralBool(0)));
    PROFILE( 51, t215, q21_peephole_2((V []){t207,t213,t215},t201,(V []){t128,t134,t136}));
    PROFILE( 52, t238, pfnLoadTable(t238, initLiteralSym((S)"lineitem")));
    PROFILE( 53, t239, pfnColumnValue(t239, t238, initLiteralSym((S)"l_orderkey")));
    PROFILE( 54, t241, pfnColumnValue(t241, t238, initLiteralSym((S)"l_suppkey")));
    PROFILE( 55, t255, pfnFetch(t255, t239));
    PROFILE( 56, t256, pfnList(t256, 2, (V []){t215 ,t213}));
    PROFILE( 57, t257, pfnList(t257, 2, (V []){t241 ,t255}));
    PROFILE( 58, t258, pfnJoinIndex(t258,t256,t257,initLiteralSymVector(2, (S []){"neq","eq"})));
    PROFILE( 59, t259, pfnIndex(t259, t258, initLiteralI64(0)));
    PROFILE( 60, t260, pfnUnique(t260, t259));
    PROFILE( 61, t261, pfnIndex(t261, t259, t260));
    PROFILE( 62, t267, pfnIndex(t267, t207, t261));
    PROFILE( 63, t298, pfnList(t298, 1, (V []){t267}));
    PROFILE( 64, t299, pfnGroup(t299, t298));
    PROFILE( 65, t300, pfnKeys(t300, t299));
    PROFILE( 66, t301, pfnValues(t301, t299));
    PROFILE( 67, t302, pfnIndex(t302, t267, t300));
    PROFILE( 68, t303, pfnEach(t303,t301,pfnLen));
    PROFILE( 69, t304, pfnRaze(t304, t303));
    PROFILE( 70, t305, pfnList(t305, 2, (V []){t304 ,t302}));
    PROFILE( 71, t306, pfnOrderBy(t306, t305, initLiteralBoolVector(2, (B []){0,1})));
    PROFILE( 72, t307, pfnIndex(t307, t302, t306));
    PROFILE( 73, t308, pfnIndex(t308, t304, t306));
    PROFILE( 74, t309, copyV(t309, initLiteralSymVector(2, (S []){"s_name","numwait"})));
    PROFILE( 75, t310, pfnList(t310, 2, (V []){t307 ,t308}));
    PROFILE( 76, t311, pfnTable(t311, t309, t310));
    toc;
    printV2(t311,20);
    R elapsed;
}
