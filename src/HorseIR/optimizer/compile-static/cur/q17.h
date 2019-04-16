/*==== FE: Loop fusion with elementwise functions ====*/
/* num_func = 3, targ = t12 */
L q17_loopfusion_0(V z, V *x){
    // z -> t12
    V x0 = x[0]; // t7
    V x1 = x[1]; // t4
    initV(z,H_B,vn(x0));
    Q s0 = getSymbol("MED BOX");
    Q s1 = getSymbol("Brand#23");
    //DOP(vn(x0), vB(z,i)=AND(EQ(vQ(x0,i),getSymbol("MED BOX")),EQ(vQ(x1,i),getSymbol("Brand#23")))) R 0;
    DOP(vn(x0), vB(z,i)=AND(EQ(vQ(x0,i),s0),EQ(vQ(x1,i),s1))) R 0;
}
/* num_func = 4, targ = t111 */
L q17_loopfusion_1(V z, V *x){
    // z -> t111
    V x0 = x[0]; // t101
    V x1 = x[1]; // t105
    V x2 = x[2]; // t108
    initV(z,H_E,vn(x0));
    DOP(vn(x0), vE(z,i)=MUL(0.2,DIV(MUL(vL(x0,i),vL(x1,i)),MUL(vL(x0,i),vL(x2,i))))) R 0;
}
/*==== FP: Loop fusion with patterns ====*/
L q17_peephole_0(V *z, V *x, V y){
    L r0 = vn(y);// y -> t97
    V z0 = z[0]; // t105
    V x0 = x[0]; // t78
    initV(z0, H_E, r0);
    DOJ(r0, {V t=vV(y,j);  E c0=0; \
        DOP(vn(t), {L k=vL(t,i); c0+=vE(x0,k);},\
        reduction(+:c0)) \
        vE(z0,j)=c0; \
    })
    R 0;
}
L q17_peephole_1(V z, V x, V y){
    // z -> t108, x -> t78, y -> t97
    L r0 = vn(y);
    initV(z, H_L, r0);
    DOP(r0, vL(z,i) = vn(vV(y,i))) R 0;
}
// Pattern FP3
// Pattern FP4
// Pattern FP5
E compiled_main(){
    E elapsed=0;
    V t0   = allocNode(); V t1   = allocNode(); V t4   = allocNode(); V t7   = allocNode(); 
    V t10  = allocNode(); V t11  = allocNode(); V t12  = allocNode(); V t13  = allocNode(); 
    V t22  = allocNode(); V t24  = allocNode(); V t27  = allocNode(); V t28  = allocNode(); 
    V t39  = allocNode(); V t40  = allocNode(); V t41  = allocNode(); V t42  = allocNode(); 
    V t56  = allocNode(); V t57  = allocNode(); V t68  = allocNode(); V t69  = allocNode(); 
    V t70  = allocNode(); V t71  = allocNode(); V t72  = allocNode(); V t73  = allocNode(); 
    V t75  = allocNode(); V t78  = allocNode(); V t90  = allocNode(); V t91  = allocNode(); 
    V t92  = allocNode(); V t93  = allocNode(); V t94  = allocNode(); V t95  = allocNode(); 
    V t96  = allocNode(); V t97  = allocNode(); V t99  = allocNode(); V t100 = allocNode(); 
    V t101 = allocNode(); V t103 = allocNode(); V t104 = allocNode(); V t105 = allocNode(); 
    V t106 = allocNode(); V t107 = allocNode(); V t108 = allocNode(); V t109 = allocNode(); 
    V t110 = allocNode(); V t111 = allocNode(); V t112 = allocNode(); V t113 = allocNode(); 
    V t114 = allocNode(); V t115 = allocNode(); V t131 = allocNode(); V t147 = allocNode(); 
    V t148 = allocNode(); V t149 = allocNode(); V t150 = allocNode(); V t151 = allocNode(); 
    tic;
    PROFILE(  0, t0 , pfnLoadTable(t0, initLiteralSym((S)"part")));
    PROFILE(  1, t1 , pfnColumnValue(t1, t0, initLiteralSym((S)"p_partkey")));
    PROFILE(  2, t4 , pfnColumnValue(t4, t0, initLiteralSym((S)"p_brand")));
    PROFILE(  3, t7 , pfnColumnValue(t7, t0, initLiteralSym((S)"p_container")));
    PROFILE(  4, t12, q17_loopfusion_0(t12,(V []){t7,t4}));
    PROFILE(  5, t13, pfnCompress(t13, t12, t1));
    PROFILE(  6, t22, pfnLoadTable(t22, initLiteralSym((S)"lineitem")));
    PROFILE(  7, t24, pfnColumnValue(t24, t22, initLiteralSym((S)"l_partkey")));
    PROFILE(  8, t27, pfnColumnValue(t27, t22, initLiteralSym((S)"l_quantity")));
    PROFILE(  9, t28, pfnColumnValue(t28, t22, initLiteralSym((S)"l_extendedprice")));
    //getInfoVar(t13); getInfoVar(t24); getchar();
    // Variable t13 has type H_L and len 204
    // Variable t24 has type H_L and len 6001215
    PROFILE( 10, t39, pfnJoinIndex(t39,t13,t24,initLiteralSym((S)"eq")));
    PROFILE( 11, t40, pfnIndex(t40, t39, initLiteralI64(0)));
    PROFILE( 12, t41, pfnIndex(t41, t39, initLiteralI64(1)));
    PROFILE( 13, t42, pfnIndex(t42, t13, t40));
    PROFILE( 14, t56, pfnIndex(t56, t27, t41));
    PROFILE( 15, t57, pfnIndex(t57, t28, t41));
    PROFILE( 16, t68, pfnList(t68, 1, (V []){t42}));
    PROFILE( 17, t69, pfnGroup(t69, t68));
    PROFILE( 18, t70, pfnKeys(t70, t69));
    PROFILE( 19, t71, pfnValues(t71, t69));
    PROFILE( 20, t72, pfnIndex(t72, t42, t70));
    PROFILE( 21, t73, pfnLoadTable(t73, initLiteralSym((S)"lineitem")));
    PROFILE( 22, t75, pfnColumnValue(t75, t73, initLiteralSym((S)"l_partkey")));
    PROFILE( 23, t78, pfnColumnValue(t78, t73, initLiteralSym((S)"l_quantity")));
    //getInfoVar(t72); getInfoVar(t75); getchar();
    // Variable t72 has type H_L and len 204
    // Variable t75 has type H_L and len 6001215
    PROFILE( 24, t90, pfnJoinIndex(t90,t72,t75,initLiteralSym((S)"eq")));
    PROFILE( 25, t91, pfnIndex(t91, t90, initLiteralI64(0)));
    PROFILE( 26, t92, pfnIndex(t92, t90, initLiteralI64(1)));
    PROFILE( 27, t93, pfnGroup(t93, t91));
    PROFILE( 28, t94, pfnKeys(t94, t93));
    PROFILE( 29, t95, pfnValues(t95, t93));
    PROFILE( 30, t96, pfnIndex(t96, t91, t94));
    PROFILE( 31, t97, pfnEachRight(t97,t92,t95,pfnIndex));
    PROFILE( 32, t99, pfnEach(t99,t71,pfnLen));
    PROFILE( 33, t100, pfnRaze(t100, t99));
    PROFILE( 34, t101, pfnIndex(t101, t100, t96));
    PROFILE( 35, t105, q17_peephole_0((V []){t105},(V []){t78},t97));
    PROFILE( 36, t108, q17_peephole_1(t108,t78,t97));
    PROFILE( 37, t111, q17_loopfusion_1(t111,(V []){t101,t105,t108}));
    PROFILE( 38, t112, pfnList(t112, 2, (V []){t56 ,t42}));
    PROFILE( 39, t113, pfnList(t113, 2, (V []){t111 ,t72}));
    PROFILE( 40, t114, pfnJoinIndex(t114,t112,t113,initLiteralSymVector(2, (S []){"lt","eq"})));
    PROFILE( 41, t115, pfnIndex(t115, t114, initLiteralI64(0)));
    PROFILE( 42, t131, pfnIndex(t131, t57, t115));
    PROFILE( 43, t147, pfnSum(t147, t131));
    PROFILE( 44, t148, pfnDiv(t148, t147, initLiteralF64(7)));
    PROFILE( 45, t149, copyV(t149, initLiteralSym((S)"avg_yearly")));
    PROFILE( 46, t150, pfnList(t150, 1, (V []){t148}));
    PROFILE( 47, t151, pfnTable(t151, t149, t150));
    toc;
    printV2(t151,20);
    R elapsed;
}
