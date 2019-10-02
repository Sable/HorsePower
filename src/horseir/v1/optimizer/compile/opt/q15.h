/*==== FE: Loop fusion with elementwise functions ====*/
/* num_func = 3, targ = t19 */
L q15_loopfusion_0(V z, V *x){
    // z -> t19
    V x0 = x[0]; // t11
    initV(z,H_B,vn(x0));
    DOP(vn(x0), vB(z,i)=AND(GEQ(vD(x0,i),19960101),LT(vD(x0,i),19960401))) R 0;
}
/* num_func = 2, targ = t37 */
L q15_loopfusion_1(V z, V *x){
    // z -> t37
    V x0 = x[0]; // t25
    V x1 = x[1]; // t26
    initV(z,H_E,vn(x0));
    DOP(vn(x0), vE(z,i)=MUL(vE(x0,i),MINUS(1,vE(x1,i)))) R 0;
}
/*==== FP: Loop fusion with patterns ====*/
L q15_peephole_0(V *z, V y, V *x){
    V z0 = z[0]; // t22
    V z1 = z[1]; // t25
    V z2 = z[2]; // t26
    V x0 = x[0]; // t3
    V x1 = x[1]; // t6
    V x2 = x[2]; // t7
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
L q15_peephole_1(V *z, V *x, V y){
    L r0 = vn(y);// y -> t41
    V z0 = z[0]; // t45
    V x0 = x[0]; // t37
    initV(z0, H_E, r0);
    /* slow: total = 10000, max = 42, min = 4
    DOJ(r0, {V t=vV(y,j);  E c0=0; \
        DOP(vn(t), {L k=vL(t,i); c0+=vE(x0,k);},\
        reduction(+:c0)) \
        vE(z0,j)=c0; \
    })*/
    DOP(r0, {V t=vV(y,i);  E c0=0; \
        DOJ(vn(t), c0+=vE(x0,vL(t,j));) vE(z0,i)=c0; })
    R 0;
}
L q15_peephole_2(V z, V x, V y){
    // z -> t45, x -> t37, y -> t41
    L r0 = vn(y);
    initV(z, H_L, r0);
    DOP(r0, vL(z,i) = vn(vV(y,i))) R 0;
}
// Pattern FP3
// Pattern FP4
// Pattern FP5
E compiledQ15(){
    E elapsed=0;
    V t0   = allocNode(); V t3   = allocNode(); V t6   = allocNode(); V t7   = allocNode(); 
    V t11  = allocNode(); V t17  = allocNode(); V t18  = allocNode(); V t19  = allocNode(); 
    V t22  = allocNode(); V t25  = allocNode(); V t26  = allocNode(); V t36  = allocNode(); 
    V t37  = allocNode(); V t38  = allocNode(); V t39  = allocNode(); V t40  = allocNode(); 
    V t41  = allocNode(); V t42  = allocNode(); V t43  = allocNode(); V t44  = allocNode(); 
    V t45  = allocNode(); V t46  = allocNode(); V t47  = allocNode(); V t49  = allocNode(); 
    V t54  = allocNode(); V t55  = allocNode(); V t56  = allocNode(); V t57  = allocNode(); 
    V t58  = allocNode(); V t59  = allocNode(); V t61  = allocNode(); V t64  = allocNode(); 
    V t65  = allocNode(); V t66  = allocNode(); V t71  = allocNode(); V t73  = allocNode(); 
    V t74  = allocNode(); V t75  = allocNode(); V t78  = allocNode(); V t81  = allocNode(); 
    V t82  = allocNode(); V t83  = allocNode(); V t84  = allocNode(); V t85  = allocNode(); 
    V t86  = allocNode(); V t87  = allocNode(); V t88  = allocNode(); V t89  = allocNode(); 
    V t90  = allocNode(); 
    tic;
    PROFILE(  0, t0 , pfnLoadTable(t0, initLiteralSym((S)"lineitem")));
    PROFILE(  1, t3 , pfnColumnValue(t3, t0, initLiteralSym((S)"l_suppkey")));
    PROFILE(  2, t6 , pfnColumnValue(t6, t0, initLiteralSym((S)"l_extendedprice")));
    PROFILE(  3, t7 , pfnColumnValue(t7, t0, initLiteralSym((S)"l_discount")));
    PROFILE(  4, t11, pfnColumnValue(t11, t0, initLiteralSym((S)"l_shipdate")));
    PROFILE(  5, t19, q15_loopfusion_0(t19,(V []){t11}));
    PROFILE(  6, t26, q15_peephole_0((V []){t22,t25,t26},t19,(V []){t3,t6,t7}));
    PROFILE(  7, t37, q15_loopfusion_1(t37,(V []){t25,t26}));
    PROFILE(  8, t38, pfnList(t38, 1, (V []){t22}));
    PROFILE(  9, t39, pfnGroup(t39, t38));
    PROFILE( 10, t40, pfnKeys(t40, t39));
    PROFILE( 11, t41, pfnValues(t41, t39));
    PROFILE( 12, t42, pfnIndex(t42, t22, t40));
    PROFILE( 13, t45, q15_peephole_1((V []){t45},(V []){t37},t41));
    PROFILE( 14, t46, pfnMax(t46, t45));
    PROFILE( 15, t47, pfnJoinIndex(t47,t46,t45,initLiteralSym((S)"eq")));
    PROFILE( 16, t49, pfnIndex(t49, t47, initLiteralI64(1)));
    PROFILE( 17, t54, pfnIndex(t54, t45, t49));
    PROFILE( 18, t55, pfnIndex(t55, t42, t49));
    PROFILE( 19, t56, pfnLoadTable(t56, initLiteralSym((S)"supplier")));
    PROFILE( 20, t57, pfnColumnValue(t57, t56, initLiteralSym((S)"s_suppkey")));
    PROFILE( 21, t58, pfnColumnValue(t58, t56, initLiteralSym((S)"s_name")));
    PROFILE( 22, t59, pfnColumnValue(t59, t56, initLiteralSym((S)"s_address")));
    PROFILE( 23, t61, pfnColumnValue(t61, t56, initLiteralSym((S)"s_phone")));
    PROFILE( 24, t64, pfnJoinIndex(t64,t57,t55,initLiteralSym((S)"eq")));
    PROFILE( 25, t65, pfnIndex(t65, t64, initLiteralI64(0)));
    PROFILE( 26, t66, pfnIndex(t66, t64, initLiteralI64(1)));
    PROFILE( 27, t71, pfnIndex(t71, t54, t66));
    PROFILE( 28, t73, pfnIndex(t73, t57, t65));
    PROFILE( 29, t74, pfnIndex(t74, t58, t65));
    PROFILE( 30, t75, pfnIndex(t75, t59, t65));
    PROFILE( 31, t78, pfnIndex(t78, t61, t65));
    PROFILE( 32, t81, pfnList(t81, 1, (V []){t73}));
    PROFILE( 33, t82, pfnOrderBy(t82, t81, initLiteralBool(1)));
    PROFILE( 34, t83, pfnIndex(t83, t73, t82));
    PROFILE( 35, t84, pfnIndex(t84, t74, t82));
    PROFILE( 36, t85, pfnIndex(t85, t75, t82));
    PROFILE( 37, t86, pfnIndex(t86, t78, t82));
    PROFILE( 38, t87, pfnIndex(t87, t71, t82));
    PROFILE( 39, t88, copyV(t88, initLiteralSymVector(5, (S []){"s_suppkey","s_name","s_address","s_phone","total_revenue"})));
    PROFILE( 40, t89, pfnList(t89, 5, (V []){t83 ,t84 ,t85 ,t86 ,t87}));
    PROFILE( 41, t90, pfnTable(t90, t88, t89));
    toc;
    printV2(t90,20);
    R elapsed;
}
