/*==== FE: Loop fusion with elementwise functions ====*/
/* num_func = 2, targ = t118 */
L q19_loopfusion_0(V z, V *x){
    // z -> t118
    V x0 = x[0]; // t106
    V x1 = x[1]; // t107
    initV(z,H_E,vn(x0));
    DOP(vn(x0), vE(z,i)=MUL(vE(x0,i),MINUS(1,vE(x1,i)))) R 0;
}
/* num_func = 14, targ = t90 */
L q19_loopfusion_1(V z, V *x){
    // z -> t90
    V x0 = x[0]; // t81
    V x1 = x[1]; // t35
    V x2 = x[2]; // t84
    V x3 = x[3]; // t87
    initV(z,H_B,vn(x0));
    DOP(vn(x0), vB(z,i)=OR(OR(AND(vB(x0,i),AND(GEQ(vE(x1,i),1),LEQ(vE(x1,i),11))),AND(vB(x2,i),AND(GEQ(vE(x1,i),10),LEQ(vE(x1,i),20)))),AND(vB(x3,i),AND(GEQ(vE(x1,i),20),LEQ(vE(x1,i),30))))) R 0;
}
/* num_func = 6, targ = t56 */
L q19_loopfusion_2(V z, V *x){
    // z -> t56
    V x0 = x[0]; // t6
    V x1 = x[1]; // t4
    V x2 = x[2]; // t52
    initV(z,H_B,vn(x0));
    //DOP(vn(x0), vB(z,i)=AND(AND(AND(GEQ(vL(x0,i),1),LEQ(vL(x0,i),5)),EQ(vQ(x1,i),getSymbol("Brand#12"))),vB(x2,i))) R 0;
    Q id0 = getSymbol("Brand#12");
    DOP(vn(x0), vB(z,i)=AND(AND(AND(GEQ(vL(x0,i),1),LEQ(vL(x0,i),5)),EQ(vQ(x1,i),id0)),vB(x2,i))) R 0;
}
/* num_func = 2, targ = t30 */
L q19_loopfusion_3(V z, V *x){
    // z -> t30
    V x0 = x[0]; // t24
    V x1 = x[1]; // t28
    initV(z,H_B,vn(x0));
    //DOP(vn(x0), vB(z,i)=AND(EQ(vQ(x0,i),getSymbol("DELIVER IN PERSON")),vB(x1,i))) R 0;
    Q id0 = getSymbol("DELIVER IN PERSON");
    DOP(vn(x0), vB(z,i)=AND(EQ(vQ(x0,i),id0),vB(x1,i))) R 0;
}
/* num_func = 6, targ = t67 */
L q19_loopfusion_4(V z, V *x){
    // z -> t67
    V x0 = x[0]; // t6
    V x1 = x[1]; // t4
    V x2 = x[2]; // t63
    initV(z,H_B,vn(x0));
    //DOP(vn(x0), vB(z,i)=AND(AND(AND(GEQ(vL(x0,i),1),LEQ(vL(x0,i),10)),EQ(vQ(x1,i),getSymbol("Brand#23"))),vB(x2,i))) R 0;
    Q id0 = getSymbol("Brand#23");
    DOP(vn(x0), vB(z,i)=AND(AND(AND(GEQ(vL(x0,i),1),LEQ(vL(x0,i),10)),EQ(vQ(x1,i),id0)),vB(x2,i))) R 0;
}
/* num_func = 6, targ = t78 */
L q19_loopfusion_5(V z, V *x){
    // z -> t78
    V x0 = x[0]; // t6
    V x1 = x[1]; // t4
    V x2 = x[2]; // t74
    initV(z,H_B,vn(x0));
    //DOP(vn(x0), vB(z,i)=AND(AND(AND(GEQ(vL(x0,i),1),LEQ(vL(x0,i),15)),EQ(vQ(x1,i),getSymbol("Brand#34"))),vB(x2,i))) R 0;
    Q id0 = getSymbol("Brand#34");
    DOP(vn(x0), vB(z,i)=AND(AND(AND(GEQ(vL(x0,i),1),LEQ(vL(x0,i),15)),EQ(vQ(x1,i),id0)),vB(x2,i))) R 0;
}
/*==== FP: Loop fusion with patterns ====*/
L q19_peephole_0(V *z, V y, V *x){
    V z0 = z[0]; // t32
    V z1 = z[1]; // t35
    V z2 = z[2]; // t36
    V z3 = z[3]; // t37
    V x0 = x[0]; // t12
    V x1 = x[1]; // t15
    V x2 = x[2]; // t16
    V x3 = x[3]; // t17
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
    DOT(len, if(vB(y,i)){L c=offset[tid]++;vL(z0,c)=vL(x0,i);vE(z1,c)=vE(x1,i);vE(z2,c)=vE(x2,i);vE(z3,c)=vE(x3,i);}) R 0;
}
L q19_peephole_1(V *z, V y, V *x){
    V z0 = z[0]; // t106
    V z1 = z[1]; // t107
    V x0 = x[0]; // t36
    V x1 = x[1]; // t37
    L len = vn(y), k = 0;
    L lenZ = 0, parZ[H_CORE], offset[H_CORE];
    DOI(H_CORE, parZ[i]=offset[i]=0)
    CHECKE(getNumOfNonZero(y,parZ));
    DOI(H_CORE, lenZ += parZ[i])
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    initV(z0,vp(x0),lenZ);
    initV(z1,vp(x1),lenZ);
    DOT(len, if(vB(y,i)){L c=offset[tid]++;vE(z0,c)=vE(x0,i);vE(z1,c)=vE(x1,i);}) R 0;
}
// Pattern FP3
// Pattern FP4
// Pattern FP5
E compiledQ19(){
    E elapsed=0;
    V t0   = allocNode(); V t1   = allocNode(); V t4   = allocNode(); V t6   = allocNode(); 
    V t7   = allocNode(); V t10  = allocNode(); V t12  = allocNode(); V t15  = allocNode(); 
    V t16  = allocNode(); V t17  = allocNode(); V t24  = allocNode(); V t25  = allocNode(); 
    V t27  = allocNode(); V t28  = allocNode(); V t29  = allocNode(); V t30  = allocNode(); 
    V t32  = allocNode(); V t35  = allocNode(); V t36  = allocNode(); V t37  = allocNode(); 
    V t47  = allocNode(); V t48  = allocNode(); V t49  = allocNode(); V t50  = allocNode(); 
    V t51  = allocNode(); V t52  = allocNode(); V t53  = allocNode(); V t54  = allocNode(); 
    V t55  = allocNode(); V t56  = allocNode(); V t57  = allocNode(); V t58  = allocNode(); 
    V t59  = allocNode(); V t60  = allocNode(); V t61  = allocNode(); V t62  = allocNode(); 
    V t63  = allocNode(); V t64  = allocNode(); V t65  = allocNode(); V t66  = allocNode(); 
    V t67  = allocNode(); V t68  = allocNode(); V t69  = allocNode(); V t70  = allocNode(); 
    V t71  = allocNode(); V t72  = allocNode(); V t73  = allocNode(); V t74  = allocNode(); 
    V t75  = allocNode(); V t76  = allocNode(); V t77  = allocNode(); V t78  = allocNode(); 
    V t79  = allocNode(); V t80  = allocNode(); V t81  = allocNode(); V t82  = allocNode(); 
    V t83  = allocNode(); V t84  = allocNode(); V t85  = allocNode(); V t86  = allocNode(); 
    V t87  = allocNode(); V t88  = allocNode(); V t89  = allocNode(); V t90  = allocNode(); 
    V t106 = allocNode(); V t107 = allocNode(); V t117 = allocNode(); V t118 = allocNode(); 
    V t119 = allocNode(); V t120 = allocNode(); V t121 = allocNode(); V t122 = allocNode(); 
    tic;
    PROFILE(  0, t0 , pfnLoadTable(t0, initLiteralSym((S)"part")));
    PROFILE(  1, t1 , pfnColumnValue(t1, t0, initLiteralSym((S)"p_partkey")));
    PROFILE(  2, t4 , pfnColumnValue(t4, t0, initLiteralSym((S)"p_brand")));
    PROFILE(  3, t6 , pfnColumnValue(t6, t0, initLiteralSym((S)"p_size")));
    PROFILE(  4, t7 , pfnColumnValue(t7, t0, initLiteralSym((S)"p_container")));
    PROFILE(  5, t10, pfnLoadTable(t10, initLiteralSym((S)"lineitem")));
    PROFILE(  6, t12, pfnColumnValue(t12, t10, initLiteralSym((S)"l_partkey")));
    PROFILE(  7, t15, pfnColumnValue(t15, t10, initLiteralSym((S)"l_quantity")));
    PROFILE(  8, t16, pfnColumnValue(t16, t10, initLiteralSym((S)"l_extendedprice")));
    PROFILE(  9, t17, pfnColumnValue(t17, t10, initLiteralSym((S)"l_discount")));
    PROFILE( 10, t24, pfnColumnValue(t24, t10, initLiteralSym((S)"l_shipinstruct")));
    PROFILE( 11, t25, pfnColumnValue(t25, t10, initLiteralSym((S)"l_shipmode")));
    PROFILE( 12, t27, copyV(t27, initLiteralSymVector(2, (S []){"AIR","AIR REG"})));
    PROFILE( 13, t28, pfnMember(t28, t27, t25));
    PROFILE( 14, t30, q19_loopfusion_3(t30,(V []){t24,t28}));
    PROFILE( 15, t37, q19_peephole_0((V []){t32,t35,t36,t37},t30,(V []){t12,t15,t16,t17}));
    PROFILE( 16, t51, copyV(t51, initLiteralSymVector(4, (S []){"SM CASE","SM BOX","SM PACK","SM PKG"})));
    PROFILE( 17, t52, pfnMember(t52, t51, t7));
    PROFILE( 18, t56, q19_loopfusion_2(t56,(V []){t6,t4,t52}));
    PROFILE( 19, t62, copyV(t62, initLiteralSymVector(4, (S []){"MED BAG","MED BOX","MED PKG","MED PACK"})));
    PROFILE( 20, t63, pfnMember(t63, t62, t7));
    PROFILE( 21, t67, q19_loopfusion_4(t67,(V []){t6,t4,t63}));
    PROFILE( 22, t73, copyV(t73, initLiteralSymVector(4, (S []){"LG CASE","LG BOX","LG PACK","LG PKG"})));
    PROFILE( 23, t74, pfnMember(t74, t73, t7));
    PROFILE( 24, t78, q19_loopfusion_5(t78,(V []){t6,t4,t74}));
    PROFILE( 25, t80, pfnCompress(t80, t56, t1));
    PROFILE( 26, t81, pfnMember(t81, t80, t32));
    PROFILE( 27, t83, pfnCompress(t83, t67, t1));
    PROFILE( 28, t84, pfnMember(t84, t83, t32));
    PROFILE( 29, t86, pfnCompress(t86, t78, t1));
    PROFILE( 30, t87, pfnMember(t87, t86, t32));
    PROFILE( 31, t90, q19_loopfusion_1(t90,(V []){t81,t35,t84,t87}));
    PROFILE( 32, t107, q19_peephole_1((V []){t106,t107},t90,(V []){t36,t37}));
    PROFILE( 33, t118, q19_loopfusion_0(t118,(V []){t106,t107}));
    PROFILE( 34, t119, pfnSum(t119, t118));
    PROFILE( 35, t120, copyV(t120, initLiteralSym((S)"revenue")));
    PROFILE( 36, t121, pfnList(t121, 1, (V []){t119}));
    PROFILE( 37, t122, pfnTable(t122, t120, t121));
    toc;
    printV2(t122,20);
    R elapsed;
}
