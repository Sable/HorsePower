/* num_func = 2, targ = t140 */
L q19_loopfusion_0(V z, V *x){
    // z -> t140
    V x0 = x[0]; // t128
    V x1 = x[1]; // t129
    initV(z,H_E,vn(x0));
    DOP(vn(x0), vE(z,i)=MUL(vE(x0,i),MINUS(1,vE(x1,i)))) R 0;
}
/* num_func = 14, targ = k10 */
L q19_loopfusion_1(V z, V *x){
    // z -> k10
    V x0 = x[0]; // k3
    V x1 = x[1]; // t35
    V x2 = x[2]; // k4
    V x3 = x[3]; // k5
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
    Q q0 = getSymbol("Brand#12");
    DOP(vn(x0), vB(z,i)=AND(AND(AND(GEQ(vL(x0,i),1),LEQ(vL(x0,i),5)),EQ(vQ(x1,i),q0)),vB(x2,i))) R 0;
}
/* num_func = 2, targ = t30 */
L q19_loopfusion_3(V z, V *x){
    // z -> t30
    V x0 = x[0]; // t24
    V x1 = x[1]; // t28
    initV(z,H_B,vn(x0));
    Q q0 = getSymbol("DELIVER IN PERSON");
    DOP(vn(x0), vB(z,i)=AND(EQ(vQ(x0,i),q0),vB(x1,i))) R 0;
}
/* num_func = 6, targ = t67 */
L q19_loopfusion_4(V z, V *x){
    // z -> t67
    V x0 = x[0]; // t6
    V x1 = x[1]; // t4
    V x2 = x[2]; // t63
    initV(z,H_B,vn(x0));
    Q q0 = getSymbol("Brand#23");
    DOP(vn(x0), vB(z,i)=AND(AND(AND(GEQ(vL(x0,i),1),LEQ(vL(x0,i),10)),EQ(vQ(x1,i),q0)),vB(x2,i))) R 0;
}
/* num_func = 6, targ = t78 */
L q19_loopfusion_5(V z, V *x){
    // z -> t78
    V x0 = x[0]; // t6
    V x1 = x[1]; // t4
    V x2 = x[2]; // t74
    initV(z,H_B,vn(x0));
    Q q0 = getSymbol("Brand#34");
    DOP(vn(x0), vB(z,i)=AND(AND(AND(GEQ(vL(x0,i),1),LEQ(vL(x0,i),15)),EQ(vQ(x1,i),q0)),vB(x2,i))) R 0;
}
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
    V z0 = z[0]; // t128
    V z1 = z[1]; // t129
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
E compiledQ19(){
    E elapsed=0;
    V t0  = allocNode(); V t1  = allocNode(); V t4  = allocNode(); V t6  = allocNode();
    V t7  = allocNode(); V t10 = allocNode(); V t12 = allocNode(); V t15 = allocNode();
    V t16 = allocNode(); V t17 = allocNode(); V t24 = allocNode(); V t25 = allocNode();
    V t27 = allocNode(); V t28 = allocNode(); V t30 = allocNode(); V t37 = allocNode();
    V t51 = allocNode(); V t52 = allocNode(); V t56 = allocNode(); V t62 = allocNode();
    V t63 = allocNode(); V t67 = allocNode(); V t73 = allocNode(); V t74 = allocNode();
    V t78 = allocNode(); V k3  = allocNode(); V k4  = allocNode(); V k5  = allocNode();
    V k10 = allocNode(); V t129= allocNode(); V t140= allocNode(); V t141= allocNode();
    V t142= allocNode(); V t143= allocNode(); V t144= allocNode();
    V t32 = allocNode(); V t35 = allocNode(); V t36 = allocNode();
    V t128= allocNode();
    V k0  = allocNode(); V k1  = allocNode(); V k2  = allocNode();
    V t29 = allocNode(); V t47 = allocNode(); V t48 = allocNode(); V t49 = allocNode();
    V t50 = allocNode(); V t53 = allocNode(); V t54 = allocNode(); V t55 = allocNode();
    V t57 = allocNode(); V t58 = allocNode(); V t59 = allocNode(); V t60 = allocNode();
    V t61 = allocNode(); V t64 = allocNode(); V t65 = allocNode(); V t66 = allocNode();
    V t68 = allocNode(); V t69 = allocNode(); V t70 = allocNode(); V t71 = allocNode();
    V t72 = allocNode(); V t75 = allocNode(); V t76 = allocNode(); V t77 = allocNode();
    V t79 = allocNode(); V k6  = allocNode(); V k7  = allocNode(); V k8  = allocNode();
    V k9  = allocNode(); V t139 = allocNode();
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
    if(OPT_LF){
        PROFILE( 14, t30, q19_loopfusion_3(t30,(V []){t24,t28}));
    }
    else {
        PROFILE( 14, t29, pfnEq(t29, t24, initLiteralSym((S)"DELIVER IN PERSON")));
        PROFILE( 15, t30, pfnAnd(t30, t29, t28));
    }
    if(OPT_PH){
        PROFILE( 15, t37, q19_peephole_0((V []){t32,t35,t36,t37},t30,(V []){t12,t15,t16,t17}));
    }
    else {
        PROFILE( 16, t32, pfnCompress(t32, t30, t12));
        PROFILE( 17, t35, pfnCompress(t35, t30, t15));
        PROFILE( 18, t36, pfnCompress(t36, t30, t16));
        PROFILE( 19, t37, pfnCompress(t37, t30, t17));
    }
    PROFILE( 16, t51, copyV(t51, initLiteralSymVector(4, (S []){"SM CASE","SM BOX","SM PACK","SM PKG"})));
    PROFILE( 17, t52, pfnMember(t52, t51, t7));
    if(OPT_LF){
        PROFILE( 18, t56, q19_loopfusion_2(t56,(V []){t6,t4,t52}));
    }
    else {
        PROFILE( 20, t47, pfnGeq(t47, t6, initLiteralI64(1)));
        PROFILE( 21, t48, pfnLeq(t48, t6, initLiteralI64(5)));
        PROFILE( 22, t49, pfnAnd(t49, t47, t48));
        PROFILE( 23, t50, pfnEq(t50, t4, initLiteralSym((S)"Brand#12")));

        PROFILE( 28, t55, pfnAnd(t55, t49, t50));
        PROFILE( 29, t56, pfnAnd(t56, t55, t52));
    }
    PROFILE( 19, t62, copyV(t62, initLiteralSymVector(4, (S []){"MED BAG","MED BOX","MED PKG","MED PACK"})));
    PROFILE( 20, t63, pfnMember(t63, t62, t7));
    if(OPT_LF){
        PROFILE( 21, t67, q19_loopfusion_4(t67,(V []){t6,t4,t63}));
    }
    else {
        PROFILE( 31, t58, pfnGeq(t58, t6, initLiteralI64(1)));
        PROFILE( 32, t59, pfnLeq(t59, t6, initLiteralI64(10)));
        PROFILE( 33, t60, pfnAnd(t60, t58, t59));
        PROFILE( 34, t61, pfnEq(t61, t4, initLiteralSym((S)"Brand#23")));
        PROFILE( 39, t66, pfnAnd(t66, t60, t61));
        PROFILE( 40, t67, pfnAnd(t67, t66, t63));
    }
    PROFILE( 22, t73, copyV(t73, initLiteralSymVector(4, (S []){"LG CASE","LG BOX","LG PACK","LG PKG"})));
    PROFILE( 23, t74, pfnMember(t74, t73, t7));
    if(OPT_LF){
        PROFILE( 24, t78, q19_loopfusion_5(t78,(V []){t6,t4,t74}));
    }
    else {
        PROFILE( 42, t69, pfnGeq(t69, t6, initLiteralI64(1)));
        PROFILE( 43, t70, pfnLeq(t70, t6, initLiteralI64(15)));
        PROFILE( 44, t71, pfnAnd(t71, t69, t70));
        PROFILE( 45, t72, pfnEq(t72, t4, initLiteralSym((S)"Brand#34")));
        PROFILE( 50, t77, pfnAnd(t77, t71, t72));
        PROFILE( 51, t78, pfnAnd(t78, t77, t74));
    }
    PROFILE( 99, k0 , pfnCompress(k0, t56, t1)); //
    PROFILE( 99, k1 , pfnCompress(k1, t67, t1)); //
    PROFILE( 99, k2 , pfnCompress(k2, t78, t1)); // can peephole
    PROFILE( 25, k3 , pfnMember(k3, k0, t32));
    PROFILE( 26, k4 , pfnMember(k4, k1, t32));
    PROFILE( 27, k5 , pfnMember(k5, k2, t32));
    if(OPT_LF){
        PROFILE( 28, k10, q19_loopfusion_1(k10,(V []){k3,t35,k4,k5}));
    }
    else {
        PROFILE( 26, t53, pfnGeq(t53, t35, initLiteralF64(1)));
        PROFILE( 27, t54, pfnLeq(t54, t35, initLiteralF64(11)));
        PROFILE( 30, t57, pfnAnd(t57, t53, t54));
        PROFILE( 37, t64, pfnGeq(t64, t35, initLiteralF64(10)));
        PROFILE( 38, t65, pfnLeq(t65, t35, initLiteralF64(20)));
        PROFILE( 41, t68, pfnAnd(t68, t64, t65));
        PROFILE( 48, t75, pfnGeq(t75, t35, initLiteralF64(20)));
        PROFILE( 49, t76, pfnLeq(t76, t35, initLiteralF64(30)));
        PROFILE( 52, t79, pfnAnd(t79, t75, t76));

        PROFILE( 59, k6 , pfnAnd(k6, k3, t57));
        PROFILE( 60, k7 , pfnAnd(k7, k4, t68));
        PROFILE( 61, k8 , pfnAnd(k8, k5, t79));
        PROFILE( 62, k9 , pfnOr(k9, k6, k7));
        PROFILE( 63, k10, pfnOr(k10, k9, k8));
    }
    if(OPT_PH){
        PROFILE( 29, t129, q19_peephole_1((V []){t128,t129},k10,(V []){t36,t37}));
    }
    else {
        PROFILE( 64, t128, pfnCompress(t128, k10, t36));
        PROFILE( 65, t129, pfnCompress(t129, k10, t37));
    }
    if(OPT_LF){
        PROFILE( 30, t140, q19_loopfusion_0(t140,(V []){t128,t129}));
    }
    else {
        PROFILE( 66, t139, pfnMinus(t139, initLiteralI64(1), t129));
        PROFILE( 67, t140, pfnMul(t140, t128, t139));
    }
    PROFILE( 31, t141, pfnSum(t141, t140));
    PROFILE( 32, t142, copyV(t142, initLiteralSym((S)"revenue")));
    PROFILE( 33, t143, pfnEnlist(t143, t141));
    PROFILE( 34, t144, pfnTable(t144, t142, t143));
    toc;
    printV(t144);
    R elapsed;
}
