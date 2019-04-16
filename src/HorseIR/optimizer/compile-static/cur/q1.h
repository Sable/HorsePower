/*==== FE: Loop fusion with elementwise functions ====*/
/* num_func = 2, targ = t35 */
L q1_loopfusion_0(V z, V *x){
    // z -> t35
    V x0 = x[0]; // t23
    V x1 = x[1]; // t24
    initV(z,H_E,vn(x0));
    DOP(vn(x0), vE(z,i)=MUL(vE(x0,i),MINUS(1,vE(x1,i)))) R 0;
}
/* num_func = 4, targ = t39 */
L q1_loopfusion_1(V z, V *x){
    // z -> t39
    V x0 = x[0]; // t23
    V x1 = x[1]; // t24
    V x2 = x[2]; // t25
    initV(z,H_E,vn(x0));
    DOP(vn(x0), vE(z,i)=MUL(MUL(vE(x0,i),MINUS(1,vE(x1,i))),PLUS(1,vE(x2,i)))) R 0;
}
/*==== FP: Loop fusion with patterns ====*/
L q1_peephole_0(V *z, V y, V *x){
    V z0 = z[0]; // t22
    V z1 = z[1]; // t23
    V z2 = z[2]; // t24
    V z3 = z[3]; // t25
    V z4 = z[4]; // t26
    V z5 = z[5]; // t27
    V x0 = x[0]; // t5
    V x1 = x[1]; // t6
    V x2 = x[2]; // t7
    V x3 = x[3]; // t8
    V x4 = x[4]; // t9
    V x5 = x[5]; // t10
    L len = vn(y), k = 0;
    L lenZ = 0, parZ[H_CORE], offset[H_CORE];
    DOI(H_CORE, parZ[i]=offset[i]=0)
    CHECKE(getNumOfNonZero(y,parZ));
    DOI(H_CORE, lenZ += parZ[i])
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    /* 1.From beginning to here: 17 ms */
    //P("input: %lld, lenZ: %lld\n", len,lenZ); getchar();
    initV(z0,vp(x0),lenZ);
    initV(z1,vp(x1),lenZ);
    initV(z2,vp(x2),lenZ);
    initV(z3,vp(x3),lenZ);
    initV(z4,vp(x4),lenZ);
    initV(z5,vp(x5),lenZ);
    DOT(len, if(vB(y,i)){L c=offset[tid]++;vE(z0,c)=vE(x0,i);vE(z1,c)=vE(x1,i);vE(z2,c)=vE(x2,i);vE(z3,c)=vE(x3,i);vC(z4,c)=vC(x4,i);vC(z5,c)=vC(x5,i);}) R 0;
    /* 2.From 1 to here: 118 ms */
}

L q1_combine_0(V *z, V y, V *x){
    V z0 = z[0]; // t22
    V z1 = z[1]; // t23
    V z2 = z[2]; // t24
    V z3 = z[3]; // t26
    V z4 = z[4]; // t27
    V z5 = z[5]; // t35
    V z6 = z[6]; // t39
    V x0 = x[0]; // t5
    V x1 = x[1]; // t6
    V x2 = x[2]; // t7
    V x3 = x[3]; // t8
    V x4 = x[4]; // t9
    V x5 = x[5]; // t10
    struct timeval t0, t1;
    gettimeofday(&t0, NULL);
    L len = vn(y), k = 0;
    L lenZ = 0, parZ[H_CORE], offset[H_CORE];
    DOI(H_CORE, parZ[i]=offset[i]=0)
    CHECKE(getNumOfNonZero(y,parZ));
    DOI(H_CORE, lenZ += parZ[i])
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    gettimeofday(&t1, NULL);
    P("time 1: %g\n", calcInterval(t0,t1)); // 6 ms
    initV(z0,vp(x0),lenZ);
    initV(z1,vp(x1),lenZ);
    initV(z2,vp(x2),lenZ);
    initV(z3,vp(x4),lenZ);
    initV(z4,vp(x5),lenZ);
    initV(z5,H_E,lenZ);
    initV(z6,H_E,lenZ);
    gettimeofday(&t0, NULL);
    DOT(len, if(vB(y,i)){L c=offset[tid]++;\
        vE(z0,c)=vE(x0,i);\
        vE(z1,c)=vE(x1,i);\
        vE(z2,c)=vE(x2,i);\
        vC(z3,c)=vC(x4,i);\
        vC(z4,c)=vC(x5,i);\
        vE(z5,c)=MUL(vE(x2,i),MINUS(1, vE(x2,i)));\
        vE(z6,c)=MUL(MUL(vE(x1,i),MINUS(1,vE(x2,i))),PLUS(1,vE(x3,i)));})
    gettimeofday(&t1, NULL);
    P("time 3: %g\n", calcInterval(t0,t1)); // getchar(); // 73 ms
    R 0;
}

L q1_peephole_1(V *z, V *x, V y){
    L r0 = vn(y);// y -> t43
    V z0 = z[0]; // t48
    V z1 = z[1]; // t51
    V z2 = z[2]; // t54
    V z3 = z[3]; // t57
    V z4 = z[4]; // t60
    V z5 = z[5]; // t63
    V z6 = z[6]; // t69
    V x0 = x[0]; // t22
    V x1 = x[1]; // t22
    V x2 = x[2]; // t23
    V x3 = x[3]; // t23
    V x4 = x[4]; // t35
    V x5 = x[5]; // t39
    V x6 = x[6]; // t24
	/* total = 4, max = 2920374, min = 38854 */
    initV(z0, H_E, r0);
    initV(z1, H_E, r0);
    initV(z2, H_E, r0);
    initV(z3, H_E, r0);
    initV(z4, H_E, r0);
    initV(z5, H_E, r0);
    initV(z6, H_E, r0);
    DOJ(r0, {V t=vV(y,j);  E c0=0; E c1=0; E c2=0; E c3=0; E c4=0; E c5=0; E c6=0; \
    DOP(vn(t), {L k=vL(t,i); c0+=vE(x0,k); c1+=vE(x1,k); c2+=vE(x2,k); c3+=vE(x3,k); c4+=vE(x4,k); c5+=vE(x5,k); c6+=vE(x6,k);},reduction(+:c0,c1,c2,c3,c4,c5,c6)) \
        vE(z0,j)=c0; \
        vE(z1,j)=c1/vn(t); \
        vE(z2,j)=c2; \
        vE(z3,j)=c3/vn(t); \
        vE(z4,j)=c4; \
        vE(z5,j)=c5; \
        vE(z6,j)=c6/vn(t); \
    })
    R 0;
}
L q1_peephole_2(V z, V x){
    // z -> t71, x -> t43
    L r0 = vn(x);
    initV(z, H_L, r0);
    DOP(r0, vL(z,i)=vn(vV(x,i))); R 0;
}

// Pattern FP3
// Pattern FP4
// Pattern FP5
E compiled_main(){
    E elapsed=0;
    V t0   = allocNode(); V t5   = allocNode(); V t6   = allocNode(); V t7   = allocNode(); 
    V t8   = allocNode(); V t9   = allocNode(); V t10  = allocNode(); V t11  = allocNode(); 
    V t17  = allocNode(); V t22  = allocNode(); V t23  = allocNode(); V t24  = allocNode(); 
    V t25  = allocNode(); V t26  = allocNode(); V t27  = allocNode(); V t34  = allocNode(); 
    V t35  = allocNode(); V t36  = allocNode(); V t37  = allocNode(); V t38  = allocNode(); 
    V t39  = allocNode(); V t40  = allocNode(); V t41  = allocNode(); V t42  = allocNode(); 
    V t43  = allocNode(); V t44  = allocNode(); V t45  = allocNode(); V t46  = allocNode(); 
    V t47  = allocNode(); V t48  = allocNode(); V t49  = allocNode(); V t50  = allocNode(); 
    V t51  = allocNode(); V t52  = allocNode(); V t53  = allocNode(); V t54  = allocNode(); 
    V t55  = allocNode(); V t56  = allocNode(); V t57  = allocNode(); V t58  = allocNode(); 
    V t59  = allocNode(); V t60  = allocNode(); V t61  = allocNode(); V t62  = allocNode(); 
    V t63  = allocNode(); V t67  = allocNode(); V t68  = allocNode(); V t69  = allocNode(); 
    V t70  = allocNode(); V t71  = allocNode(); V t72  = allocNode(); V t73  = allocNode(); 
    V t74  = allocNode(); V t75  = allocNode(); V t76  = allocNode(); V t77  = allocNode(); 
    V t78  = allocNode(); V t79  = allocNode(); V t80  = allocNode(); V t81  = allocNode(); 
    V t82  = allocNode(); V t83  = allocNode(); V t84  = allocNode(); V t85  = allocNode(); 
    V t86  = allocNode(); 
    tic;
    PROFILE(  0, t0 , pfnLoadTable(t0, initLiteralSym((S)"lineitem")));
    PROFILE(  1, t5 , pfnColumnValue(t5, t0, initLiteralSym((S)"l_quantity")));
    PROFILE(  2, t6 , pfnColumnValue(t6, t0, initLiteralSym((S)"l_extendedprice")));
    PROFILE(  3, t7 , pfnColumnValue(t7, t0, initLiteralSym((S)"l_discount")));
    PROFILE(  4, t8 , pfnColumnValue(t8, t0, initLiteralSym((S)"l_tax")));
    PROFILE(  5, t9 , pfnColumnValue(t9, t0, initLiteralSym((S)"l_returnflag")));
    PROFILE(  6, t10, pfnColumnValue(t10, t0, initLiteralSym((S)"l_linestatus")));
    PROFILE(  7, t11, pfnColumnValue(t11, t0, initLiteralSym((S)"l_shipdate")));
    PROFILE(  8, t17, pfnLeq(t17, t11, initLiteralDate(19980902)));
    // PROFILE(  9, t27, q1_peephole_0((V []){t22,t23,t24,t25,t26,t27},t17,(V []){t5,t6,t7,t8,t9,t10}));
    // PROFILE( 10, t35, q1_loopfusion_0(t35,(V []){t23,t24}));
    // PROFILE( 11, t39, q1_loopfusion_1(t39,(V []){t23,t24,t25}));
    PROFILE( 11, t27, q1_combine_0((V []){t22,t23,t24,t26,t27,t35,t39},t17,(V []){t5,t6,t7,t8,t9,t10}));
    PROFILE( 12, t40, pfnList(t40, 2, (V []){t26 ,t27}));
    //PROFILE( 13, t41, pfnGroup(t41, t40));
    PROFILE( 13, t41, pfnGroupTrie(t41, t40));
    PROFILE( 14, t42, pfnKeys(t42, t41));
    PROFILE( 15, t43, pfnValues(t43, t41));
    PROFILE( 16, t44, pfnIndex(t44, t26, t42));
    PROFILE( 17, t45, pfnIndex(t45, t27, t42));
    PROFILE( 38, t69, q1_peephole_1((V []){t48,t51,t54,t57,t60,t63,t69}, (V []){t22,t22,t23,t23,t35,t39,t24}, t43));
    // PROFILE( 18, t46, pfnEachRight(t46,t22,t43,pfnIndex));
    // PROFILE( 19, t47, pfnEach(t47,t46,pfnSum));
    // PROFILE( 20, t48, pfnRaze(t48, t47));
    // PROFILE( 21, t49, pfnEachRight(t49,t22,t43,pfnIndex));
    // PROFILE( 22, t50, pfnEach(t50,t49,pfnAvg));
    // PROFILE( 23, t51, pfnRaze(t51, t50));
    // PROFILE( 24, t52, pfnEachRight(t52,t23,t43,pfnIndex));
    // PROFILE( 25, t53, pfnEach(t53,t52,pfnSum));
    // PROFILE( 26, t54, pfnRaze(t54, t53));
    // PROFILE( 27, t55, pfnEachRight(t55,t23,t43,pfnIndex));
    // PROFILE( 28, t56, pfnEach(t56,t55,pfnAvg));
    // PROFILE( 29, t57, pfnRaze(t57, t56));
    // PROFILE( 30, t58, pfnEachRight(t58,t35,t43,pfnIndex));
    // PROFILE( 31, t59, pfnEach(t59,t58,pfnSum));
    // PROFILE( 32, t60, pfnRaze(t60, t59));
    // PROFILE( 33, t61, pfnEachRight(t61,t39,t43,pfnIndex));
    // PROFILE( 34, t62, pfnEach(t62,t61,pfnSum));
    // PROFILE( 35, t63, pfnRaze(t63, t62));
    // PROFILE( 36, t67, pfnEachRight(t67,t24,t43,pfnIndex));
    // PROFILE( 37, t68, pfnEach(t68,t67,pfnAvg));
    // PROFILE( 38, t69, pfnRaze(t69, t68));
    PROFILE( 40, t71, q1_peephole_2(t71, t43));
    // PROFILE( 39, t70, pfnEach(t70,t43,pfnLen));
    // PROFILE( 40, t71, pfnRaze(t71, t70));
    PROFILE( 41, t72, pfnList(t72, 2, (V []){t44 ,t45}));
    PROFILE( 42, t73, pfnOrderBy(t73, t72, initLiteralBoolVector(2, (B []){1,1})));
    PROFILE( 43, t74, pfnIndex(t74, t44, t73));
    PROFILE( 44, t75, pfnIndex(t75, t45, t73));
    PROFILE( 45, t76, pfnIndex(t76, t48, t73));
    PROFILE( 46, t77, pfnIndex(t77, t54, t73));
    PROFILE( 47, t78, pfnIndex(t78, t60, t73));
    PROFILE( 48, t79, pfnIndex(t79, t63, t73));
    PROFILE( 49, t80, pfnIndex(t80, t51, t73));
    PROFILE( 50, t81, pfnIndex(t81, t57, t73));
    PROFILE( 51, t82, pfnIndex(t82, t69, t73));
    PROFILE( 52, t83, pfnIndex(t83, t71, t73));
    PROFILE( 53, t84, copyV(t84, initLiteralSymVector(10, (S []){"l_returnflag","l_linestatus","sum_qty","sum_base_price","sum_disc_price","sum_charge","avg_qty","avg_price","avg_disc","count_order"})));
    PROFILE( 54, t85, pfnList(t85, 10, (V []){t74 ,t75 ,t76 ,t77 ,t78 ,t79 ,t80 ,t81 ,t82 ,t83}));
    PROFILE( 55, t86, pfnTable(t86, t84, t85));
    toc;
    printV2(t86,20);
    R elapsed;
}
