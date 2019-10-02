/*==== FE: Loop fusion with elementwise functions ====*/
/* num_func = 4, targ = t62 */
L q4_loopfusion_0(V z, V *x){
    // z -> t62
    V x0 = x[0]; // t5
    V x1 = x[1]; // t61
    initV(z,H_B,vn(x0));
    DOP(vn(x0), vB(z,i)=AND(AND(GEQ(vD(x0,i),19930701),LT(vD(x0,i),19931001)),vB(x1,i))) R 0;
}
/*==== FP: Loop fusion with patterns ====*/
// Pattern FP3
L q4_peephole_0(V z, V *x, V *y){
    // z -> t61
    V x0 = x[0]; // t34
    V x1 = x[1]; // t35
    V y0 = y[0]; // t56
    V y1 = y[1]; // t57
    L r0 = vn(y1);
    initV(z, H_B, r0);
    DOP(vn(y0), if(vD(x0,i)<vD(x1,i))vB(z,vL(y0,i))=1) R 0;
}
// Pattern FP4
// Pattern FP5
E compiled_main(){
    E elapsed=0;
    V t0   = allocNode(); V t5   = allocNode(); V t6   = allocNode(); V t10  = allocNode(); 
    V t11  = allocNode(); V t12  = allocNode(); V t22  = allocNode(); V t23  = allocNode(); 
    V t34  = allocNode(); V t35  = allocNode(); V t39  = allocNode(); V t56  = allocNode(); 
    V t57  = allocNode(); V t58  = allocNode(); V t59  = allocNode(); V t60  = allocNode(); 
    V t61  = allocNode(); V t62  = allocNode(); V t68  = allocNode(); V t72  = allocNode(); 
    V t73  = allocNode(); V t74  = allocNode(); V t75  = allocNode(); V t76  = allocNode(); 
    V t77  = allocNode(); V t78  = allocNode(); V t79  = allocNode(); V t80  = allocNode(); 
    V t81  = allocNode(); V t82  = allocNode(); V t83  = allocNode(); V t84  = allocNode(); 
    V t85  = allocNode(); 
    tic;
    PROFILE(  0, t0 , pfnLoadTable(t0, initLiteralSym((S)"orders")));
    PROFILE(  1, t5 , pfnColumnValue(t5, t0, initLiteralSym((S)"o_orderdate")));
    PROFILE(  2, t6 , pfnColumnValue(t6, t0, initLiteralSym((S)"o_orderpriority")));
    PROFILE(  3, t22, pfnLoadTable(t22, initLiteralSym((S)"lineitem")));
    PROFILE(  4, t23, pfnColumnValue(t23, t22, initLiteralSym((S)"l_orderkey")));
    PROFILE(  5, t34, pfnColumnValue(t34, t22, initLiteralSym((S)"l_commitdate")));
    PROFILE(  6, t35, pfnColumnValue(t35, t22, initLiteralSym((S)"l_receiptdate")));
    PROFILE(  7, t56, pfnValues(t56, t23));
    PROFILE(  8, t57, pfnKeys(t57, t23));
    PROFILE(  9, t61, q4_peephole_0(t61,(V []){t34,t35}, (V []){t56,t57}));
    PROFILE( 10, t62, q4_loopfusion_0(t62,(V []){t5,t61}));
    PROFILE( 11, t68, pfnCompress(t68, t62, t6));
    PROFILE( 12, t72, pfnList(t72, 1, (V []){t68}));
    PROFILE( 13, t73, pfnGroup(t73, t72));
    PROFILE( 14, t74, pfnKeys(t74, t73));
    PROFILE( 15, t75, pfnValues(t75, t73));
    PROFILE( 16, t76, pfnIndex(t76, t68, t74));
    PROFILE( 17, t77, pfnEach(t77,t75,pfnLen));
    PROFILE( 18, t78, pfnRaze(t78, t77));
    PROFILE( 19, t79, pfnList(t79, 1, (V []){t76}));
    PROFILE( 20, t80, pfnOrderBy(t80, t79, initLiteralBool(1)));
    PROFILE( 21, t81, pfnIndex(t81, t76, t80));
    PROFILE( 22, t82, pfnIndex(t82, t78, t80));
    PROFILE( 23, t83, copyV(t83, initLiteralSymVector(2, (S []){"o_orderpriority","order_count"})));
    PROFILE( 24, t84, pfnList(t84, 2, (V []){t81 ,t82}));
    PROFILE( 25, t85, pfnTable(t85, t83, t84));
    toc;
    printV2(t85,20);
    R elapsed;
}
