/* source
 *   /home/sable/hanfeng.c/Documents/GitHub/HorsePower/src/HorseIR/optimizer/compile/opt/q22.h
 */

/* num_func = 2, targ = t46 */
L q22_loopfusion_0(V z, V *x){
    // z -> t46
    V x0 = x[0]; // t31
    V x1 = x[1]; // t24 <--- scalar
    V x2 = x[2]; // t36
    initV(z,H_B,vn(x0));
    DOP(vn(x0), vB(z,i)=AND(GT(vE(x0,i),ve(x1)),vB(x2,i))) R 0;
}
/* num_func = 2, targ = t13 */
L q22_loopfusion_1(V z, V *x){
    // z -> t13
    V x0 = x[0]; // t6
    V x1 = x[1]; // t11
    initV(z,H_B,vn(x0));
    DOP(vn(x0), vB(z,i)=AND(GT(vE(x0,i),0),vB(x1,i))) R 0;
}
L q22_peephole_0(V *z, V y, V *x){
    V z0 = z[0]; // t63
    V z1 = z[1]; // t64
    V x0 = x[0]; // t30
    V x1 = x[1]; // t31
    L len = vn(y), k = 0;
    L lenZ = 0, parZ[H_CORE], offset[H_CORE];
    DOI(H_CORE, parZ[i]=offset[i]=0)
    CHECKE(getNumOfNonZero(y,parZ));
    DOI(H_CORE, lenZ += parZ[i])
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    initV(z0,vp(x0),lenZ);
    initV(z1,vp(x1),lenZ);
    DOT(len, if(vB(y,i)){L c=offset[tid]++;vS(z0,c)=vS(x0,i);vE(z1,c)=vE(x1,i);}) R 0;
}
E compiled_main(){
    E elapsed=0;
    V t0  = allocNode(); V t5  = allocNode(); V t6  = allocNode(); V t9  = allocNode();
    V t10 = allocNode(); V t11 = allocNode(); V t13 = allocNode(); V t24 = allocNode();
    V t25 = allocNode(); V t30 = allocNode(); V t31 = allocNode(); V t34 = allocNode();
    V t35 = allocNode(); V t36 = allocNode(); V t46 = allocNode(); V t47 = allocNode();
    V t49 = allocNode(); V t57 = allocNode(); V t58 = allocNode(); V t64 = allocNode();
    V t67 = allocNode(); V t68 = allocNode(); V t69 = allocNode(); V t70 = allocNode();
    V t71 = allocNode(); V t72 = allocNode(); V t73 = allocNode(); V t74 = allocNode();
    V t75 = allocNode(); V t76 = allocNode(); V t77 = allocNode(); V t78 = allocNode();
    V t79 = allocNode(); V t80 = allocNode(); V t81 = allocNode(); V t82 = allocNode();
    V t83 = allocNode(); V t84 = allocNode(); V t85 = allocNode(); V t86 = allocNode();
    V t63 = allocNode(); V t19 = allocNode();
    V t12 = allocNode(); V t45 = allocNode();
    tic;
    PROFILE(  0, t0 , pfnLoadTable(t0, initLiteralSym((S)"customer")));
    PROFILE(  1, t5 , pfnColumnValue(t5, t0, initLiteralSym((S)"c_phone")));
    PROFILE(  2, t6 , pfnColumnValue(t6, t0, initLiteralSym((S)"c_acctbal")));
    PROFILE(  3, t9 , copyV(t9, initLiteralStrVector(7, (S []){"13","31","23","29","30","18","17"})));
    PROFILE(  4, t10, pfnSubString(t10, t5, initLiteralI64Vector(2, (L []){1,2})));
    PROFILE(  5, t11, pfnMember(t11, t9, t10));
    if(OPT_LF){
        PROFILE(  6, t13, q22_loopfusion_1(t13,(V []){t6,t11}));
    }
    else {
        PROFILE(  6, t12, pfnGt(t12, t6, initLiteralF64(0)));
        PROFILE(  7, t13, pfnAnd(t13, t12, t11));
    }
    PROFILE( 99, t19, pfnCompress(t19, t13, t6)); //
    PROFILE(  7, t24, pfnAvg(t24, t19));
    PROFILE(  8, t25, pfnLoadTable(t25, initLiteralSym((S)"customer")));
    PROFILE(  9, t30, pfnColumnValue(t30, t25, initLiteralSym((S)"c_phone")));
    PROFILE( 10, t31, pfnColumnValue(t31, t25, initLiteralSym((S)"c_acctbal")));
    PROFILE( 11, t34, copyV(t34, initLiteralStrVector(7, (S []){"13","31","23","29","30","18","17"})));
    PROFILE( 12, t35, pfnSubString(t35, t30, initLiteralI64Vector(2, (L []){1,2})));
    PROFILE( 13, t36, pfnMember(t36, t34, t35));
    if(OPT_LF){
        PROFILE( 14, t46, q22_loopfusion_0(t46,(V []){t31,t24,t36}));
    }
    else {
        PROFILE( 16, t45, pfnGt(t45, t31, t24));
        PROFILE( 17, t46, pfnAnd(t46, t45, t36));
    }
    PROFILE( 15, t47, pfnLoadTable(t47, initLiteralSym((S)"orders")));
    PROFILE( 16, t49, pfnColumnValue(t49, t47, initLiteralSym((S)"o_custkey")));
    PROFILE( 17, t57, pfnValues(t57, t49));
    //PROFILE( 18, t58, pfnIndexA(t46, t57, initLiteralBool(0))); t58 = t46;
    PROFILE( 18, t58, pfnIndexA(t58, t46, t57, initLiteralBool(0)));
    if(OPT_PH){
        PROFILE( 19, t64, q22_peephole_0((V []){t63,t64},t58,(V []){t30,t31}));
    }
    else {
        PROFILE( 23, t63, pfnCompress(t63, t58, t30));
        PROFILE( 24, t64, pfnCompress(t64, t58, t31));
    }
    PROFILE( 20, t67, pfnSubString(t67, t63, initLiteralI64Vector(2, (L []){1,2})));
    PROFILE( 21, t68, pfnList(t68, 1, (V []){t67}));
    PROFILE( 22, t69, pfnGroup(t69, t68));
    PROFILE( 23, t70, pfnKeys(t70, t69));
    PROFILE( 24, t71, pfnValues(t71, t69));
    PROFILE( 25, t72, pfnIndex(t72, t67, t70));
    PROFILE( 26, t73, pfnEachRight(t73,t64,t71,pfnIndex));
    PROFILE( 27, t74, pfnEach(t74,t73,pfnSum));
    PROFILE( 28, t75, pfnRaze(t75, t74));
    PROFILE( 29, t76, pfnEachRight(t76,t67,t71,pfnIndex));
    PROFILE( 30, t77, pfnEach(t77,t76,pfnLen));
    PROFILE( 31, t78, pfnRaze(t78, t77));
    PROFILE( 32, t79, pfnList(t79, 1, (V []){t72}));
    PROFILE( 33, t80, pfnOrderBy(t80, t79, initLiteralBool(1)));
    PROFILE( 34, t81, pfnIndex(t81, t72, t80));
    PROFILE( 35, t82, pfnIndex(t82, t78, t80));
    PROFILE( 36, t83, pfnIndex(t83, t75, t80));
    PROFILE( 37, t84, copyV(t84, initLiteralSymVector(3, (S []){"cntrycode","numcust","totacctbal"})));
    PROFILE( 38, t85, pfnList(t85, 3, (V []){t81 ,t82 ,t83}));
    PROFILE( 39, t86, pfnTable(t86, t84, t85));
    toc;
    printV(t86);
    R elapsed;
}
