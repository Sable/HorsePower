/*==== FE: Loop fusion with elementwise functions ====*/
/* num_func = 2, targ = t87 */
L q14_loopfusion_0(V z, V *x){
    // z -> t87
    V x0 = x[0]; // t84
    V x1 = x[1]; // t85
    initV(z,H_E,1);
    L i=0; ve(z)=DIV(MUL(100,ve(x0)),ve(x1)); R 0;
}
/* num_func = 6, targ = t81 */
L q14_loopfusion_1(V z, V *x){
    // z -> t81
    V x0 = x[0]; // t75
    V x1 = x[1]; // t55
    V x2 = x[2]; // t56
    initV(z,H_E,vn(x0));
    DOP(vn(x0), vE(z,i)=PLUS(MUL(vB(x0,i),MUL(vE(x1,i),MINUS(1,vE(x2,i)))),MUL(NOT(vB(x0,i)),0))) R 0;
}
/* num_func = 3, targ = t19 */
L q14_loopfusion_2(V z, V *x){
    // z -> t19
    V x0 = x[0]; // t11
    initV(z,H_B,vn(x0));
    DOP(vn(x0), vB(z,i)=AND(GEQ(vD(x0,i),19950901),LT(vD(x0,i),19951001))) R 0;
    //DOP(vn(x0), vB(z,i)=AND(LT(vD(x0,i),19951001), GEQ(vD(x0,i),19950901))) R 0;
}

/*
L q14_loopfusion_2_exp(V *x){
    V z = allocNode();
    V x0 = x[0]; // t11
    L tot = 0;
    DOI(vn(x0), if(AND(GEQ(vD(x0,i),19950901),LT(vD(x0,i),19951001)))tot++);
    initV(z,H_L,tot);
    DOI(vn(x0), if(AND(GEQ(vD(x0,i),19950901),LT(vD(x0,i),19951001))){vL(z,i)=i;}) R 0;
}*/

/* num_func = 2, targ = t83 */
L q14_loopfusion_3(V z, V *x){
    // z -> t83
    V x0 = x[0]; // t55
    V x1 = x[1]; // t56
    initV(z,H_E,vn(x0));
    DOP(vn(x0), vE(z,i)=MUL(vE(x0,i),MINUS(1,vE(x1,i)))) R 0;
}
/*==== FP: Loop fusion with patterns ====*/
L q14_peephole_0(V *z, V y, V *x){
    E elapsed=0;
    V z0 = z[0]; // t21
    V z1 = z[1]; // t25
    V z2 = z[2]; // t26
    V x0 = x[0]; // t2
    V x1 = x[1]; // t6
    V x2 = x[2]; // t7
    V tt = allocNode();
    L len = vn(y), k = 0;
    L lenZ = 0, parZ[H_CORE], offset[H_CORE];
    DOI(H_CORE, parZ[i]=offset[i]=0)
    CHECKE(getNumOfNonZero(y,parZ));
    DOI(H_CORE, lenZ += parZ[i])
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    initV(z0,vp(x0),lenZ);
    initV(z1,vp(x1),lenZ);
    initV(z2,vp(x2),lenZ);
    P("compress rate: %g%% = %lld/%lld\n", lenZ*100.0/len,lenZ,len);
    initV(tt,H_L,lenZ);
    tic;
    switch(0){
        case 0: {
                    DOT(len, if(vB(y,i)){L c=offset[tid]++;vL(z0,c)=vL(x0,i);vE(z1,c)=vE(x1,i);vE(z2,c)=vE(x2,i);})
                } break;
        case 1: {
                    DOT(len, if(vB(y,i)){L c=offset[tid]++; vL(tt,c)=i;})
                    DOP(lenZ, {L k=vL(tt,i); vL(z0,i)=vL(x0,k); vE(z1,i)=vE(x1,k); vE(z2,i)=vE(x2,k);})
                    P("main computation done.\n");
                } break;
        default: EP("......\n");
    }
    toc;
    /*
    // reset offset for experiment
    DOI(H_CORE, offset[i]=0)
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    tic;
    DOT(len, if(vB(y,i)){L c=offset[tid]++;vL(z0,c)=vL(x0,i);})
    //DOT(len, if(vB(y,i)){L c=offset[tid]++;vE(z1,c)=vE(x1,i);})
    //DOT(len, if(vB(y,i)){L c=offset[tid]++;vE(z2,c)=vE(x2,i);})
    toc;
    P("ending ....\n");
    */
    R 0;
}


L q14_peephole_0_exp(V *z, V y, V *x){
    E elapsed=0;
    V z0 = z[0]; // t21
    //V z1 = z[1]; // t25
    //V z2 = z[2]; // t26
    V tt = z[3]; // tt
    V x0 = x[0]; // t2
    V x1 = x[1]; // t6
    V x2 = x[2]; // t7
    L len = vn(y), k = 0;
    L lenZ = 0, parZ[H_CORE], offset[H_CORE];
    DOI(H_CORE, parZ[i]=offset[i]=0)
    CHECKE(getNumOfNonZero(y,parZ));
    DOI(H_CORE, lenZ += parZ[i])
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    initV(z0,vp(x0),lenZ);
    //initV(z1,vp(x1),lenZ);
    //initV(z2,vp(x2),lenZ);
    P("compress rate: %g%% = %lld/%lld\n", lenZ*100.0/len,lenZ,len);
    initV(tt,H_L,lenZ);
    tic;
    DOT(len, if(vB(y,i)){L c=offset[tid]++; vL(tt,c)=i;})
    DOP(lenZ, {L k=vL(tt,i); vL(z0,i)=vL(x0,k);})
    P("main computation done.\n");
    toc;
    R 0;
}

L q14_new_0(V *z, V *x){
    V z0 = z[0]; // t55
    V z1 = z[1]; // t56
    V x0 = x[0]; // t6
    V x1 = x[1]; // t7
    V x2 = x[2]; // tt
    V x3 = x[3]; // t48
    initV(z0, vp(x0), vn(x3));
    initV(z1, vp(x1), vn(x3));
    DOP(vn(x3), {L k=vL(x2,vL(x3,i)); vE(z0,i)=vE(x0,k); vE(z1,i)=vE(x1,k);}) R 0;
}


// Pattern FP3
// Pattern FP4
// Pattern FP5
E compiledQ14(){
    E elapsed=0;
    V t0   = allocNode(); V t2   = allocNode(); V t6   = allocNode(); V t7   = allocNode(); 
    V t11  = allocNode(); V t17  = allocNode(); V t18  = allocNode(); V t19  = allocNode(); 
    V t21  = allocNode(); V t25  = allocNode(); V t26  = allocNode(); V t36  = allocNode(); 
    V t37  = allocNode(); V t41  = allocNode(); V t46  = allocNode(); V t47  = allocNode(); 
    V t48  = allocNode(); V t55  = allocNode(); V t56  = allocNode(); V t70  = allocNode(); 
    V t75  = allocNode(); V t76  = allocNode(); V t77  = allocNode(); V t78  = allocNode(); 
    V t79  = allocNode(); V t80  = allocNode(); V t81  = allocNode(); V t82  = allocNode(); 
    V t83  = allocNode(); V t84  = allocNode(); V t85  = allocNode(); V t86  = allocNode(); 
    V t87  = allocNode(); V t88  = allocNode(); V t89  = allocNode(); V t90  = allocNode(); 
    V tt = allocNode();
    tic;
    PROFILE(  0, t0 , pfnLoadTable(t0, initLiteralSym((S)"lineitem")));
    PROFILE(  1, t2 , pfnColumnValue(t2, t0, initLiteralSym((S)"l_partkey")));
    PROFILE(  2, t6 , pfnColumnValue(t6, t0, initLiteralSym((S)"l_extendedprice")));
    PROFILE(  3, t7 , pfnColumnValue(t7, t0, initLiteralSym((S)"l_discount")));
    PROFILE(  4, t11, pfnColumnValue(t11, t0, initLiteralSym((S)"l_shipdate")));
    PROFILE(  5, t19, q14_loopfusion_2(t19,(V []){t11}));
    //PROFILE( 99, t19, q14_loopfusion_2_exp((V []){t11}));
    //PROFILE(  6, t26, q14_peephole_0((V []){t21,t25,t26},t19,(V []){t2,t6,t7}));
    PROFILE( 99, t26, q14_peephole_0_exp((V []){t21,t25,t26,tt},t19,(V []){t2,t6,t7}));
    PROFILE(  7, t36, pfnLoadTable(t36, initLiteralSym((S)"part")));
    PROFILE(  8, t37, pfnColumnValue(t37, t36, initLiteralSym((S)"p_partkey")));
    PROFILE(  9, t41, pfnColumnValue(t41, t36, initLiteralSym((S)"p_type")));
    // original code
    PROFILE( 10, t46, pfnJoinIndex(t46,t37,t21,initLiteralSym((S)"eq")));
    PROFILE( 11, t47, pfnIndex(t47, t46, initLiteralI64(0)));
    PROFILE( 12, t48, pfnIndex(t48, t46, initLiteralI64(1)));
    // swap the left and right sides
    //PROFILE( 10, t46, pfnJoinIndex(t46,t21,t37,initLiteralSym((S)"eq")));
    //PROFILE( 11, t47, pfnIndex(t47, t46, initLiteralI64(1)));
    //PROFILE( 12, t48, pfnIndex(t48, t46, initLiteralI64(0)));
    // 
    P("size: t47=%lld, t48=%lld, t37=%lld, t21=%lld\n", vn(t47),vn(t48),vn(t37),vn(t21));
    //PROFILE( 13, t55, pfnIndex(t55, t25, t48));
    //PROFILE( 14, t56, pfnIndex(t56, t26, t48));
    PROFILE( 90, t56, q14_new_0((V []){t55,t56}, (V []){t6,t7,tt,t48}));
    PROFILE( 15, t70, pfnIndex(t70, t41, t47));
    PROFILE( 16, t75, pfnLike(t75, t70, initLiteralSym((S)"PROMO%")));
    PROFILE( 17, t81, q14_loopfusion_1(t81,(V []){t75,t55,t56}));
    PROFILE( 18, t83, q14_loopfusion_3(t83,(V []){t55,t56}));
    PROFILE( 19, t84, pfnSum(t84, t81));
    PROFILE( 20, t85, pfnSum(t85, t83));
    PROFILE( 21, t87, q14_loopfusion_0(t87,(V []){t84,t85}));
    PROFILE( 22, t88, copyV(t88, initLiteralSym((S)"promo_revenue")));
    PROFILE( 23, t89, pfnList(t89, 1, (V []){t87}));
    PROFILE( 24, t90, pfnTable(t90, t88, t89));
    toc;
    printV2(t90,20);
    R elapsed;
}
