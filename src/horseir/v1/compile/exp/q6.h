/*==== FE: Loop fusion with elementwise functions ====*/
/* num_func = 9, targ = t25 */
L q6_loopfusion_0(V z, V *x){
    // z -> t25
    V x0 = x[0]; // t11
    V x1 = x[1]; // t5
    V x2 = x[2]; // t7
    /*
     * I: 29 ms (boolean expression)
     * L: 35 ms
     * F: 31 ms
     * E: 36 ms
     */
    //V tt = allocNode(); initV(tt,H_L,vn(x2));
    //DOI(vn(x2), vL(tt,i)=100*vE(x2,i))
    PROFILE(20, t0, initV(z,H_B,vn(x0)));
#ifdef SELECT_C
    // strategy 1 (65 ms)
    //PROFILE(21, t0, DOI(vn(x0), vB(z,i)=AND(AND(AND(GEQ(vD(x0,i),19940101),LT(vD(x0,i),19950101)),LT(vE(x1,i),24)),AND(GEQ(vE(x2,i),0.05),LEQ(vE(x2,i),0.07)))))
    // strategy 2 (62 ms)
    PROFILE(22, t0, DOP(vn(x0), vB(z,i)=AND(AND(AND(GEQ(vD(x0,i),19940101),LT(vD(x0,i),19950101)),AND(GEQ(vE(x2,i),0.05),LEQ(vE(x2,i),0.07))),LT(vE(x1,i),24))))
    //PROFILE(22, t0, DOI(vn(x0), vB(z,i)=AND(AND(AND(GEQ(vD(x0,i),19940101),LT(vD(x0,i),19950101)),AND(GEQ(vL(tt,i),5),LEQ(vL(tt,i),7))),LT(vE(x1,i),24))))
    // strategy 3 (92 ms)
    //PROFILE(23, t0, DOI(vn(x0), vB(z,i)=AND(AND(AND(GEQ(vE(x2,i),0.05),LEQ(vE(x2,i),0.07)),AND(GEQ(vD(x0,i),19940101),LT(vD(x0,i),19950101))),LT(vE(x1,i),24))))
    // strategy 4 (82 ms)
    //PROFILE(24, t0, DOI(vn(x0), vB(z,i)=AND(AND(AND(GEQ(vE(x2,i),0.05),GEQ(vD(x0,i),19940101)),AND(LEQ(vE(x2,i),0.07),LT(vD(x0,i),19950101))),LT(vE(x1,i),24))))
#endif
#ifdef SELECT_ACC
    /*
     * Experiment for OpenACC
     */
    int size = vn(x0);
    D *in0 = sD(x0);
    E *in1 = sE(x1);
    E *in2 = sE(x2);
    C *rtn = sC(z); // because B is not supported in GPU
#pragma acc data copy(in0[0:size],in1[0:size],in2[0:size]) copy(rtn[0:size])
{
#pragma acc parallel loop
    for(L i=0; i<size; i++){
        rtn[i]=AND(AND(AND(GEQ(in0[i],19940101),LT(in0[i],19950101)),AND(GEQ(in2[i],0.05),LEQ(in2[i],0.07))),LT(in1[i],24));
    }
}
#endif
    //DOI(size, vB(z,i)=rtn[i]==1)
//#pragma acc parallel loop present(in0) present(in1) present(in2) copyout(targ[0:size])
    //for(L i=0; i<size; i++){
    //    targ[i]=AND(AND(AND(GEQ(in0[i],19940101),LT(in0[i],19950101)),AND(GEQ(in2[i],0.05),LEQ(in2[i],0.07))),LT(in1[i],24));
    //}
    R 0;
}
/*==== FP: Loop fusion with patterns ====*/
L q6_peephole_0(V *z, V y, V *x){
    V z0 = z[0]; // t31
    V z1 = z[1]; // t32
    V x0 = x[0]; // t6
    V x1 = x[1]; // t7
    L len = vn(y), k = 0;
    L lenZ = 0, parZ[H_CORE], offset[H_CORE];
    PROFILE(10, t0, DOI(H_CORE, parZ[i]=offset[i]=0));
    PROFILE(11, t0, CHECKE(getNumOfNonZero(y,parZ)));
    PROFILE(12, t0, DOI(H_CORE, lenZ += parZ[i]));
    PROFILE(13, t0, DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1]));
    PROFILE(14, t0, initV(z0,vp(x0),lenZ));
    PROFILE(15, t0, initV(z1,vp(x1),lenZ));
    PROFILE(16, t0, DOT(len, if(vB(y,i)){L c=offset[tid]++;vE(z0,c)=vE(x0,i);vE(z1,c)=vE(x1,i);})); R 0;
}
// Pattern FP3
// Pattern FP4
// Pattern FP5
E compiledQ6(){
    E elapsed=0;
    V t0   = allocNode(); V t5   = allocNode(); V t6   = allocNode(); V t7   = allocNode(); 
    V t11  = allocNode(); V t17  = allocNode(); V t18  = allocNode(); V t19  = allocNode(); 
    V t20  = allocNode(); V t21  = allocNode(); V t22  = allocNode(); V t23  = allocNode(); 
    V t24  = allocNode(); V t25  = allocNode(); V t31  = allocNode(); V t32  = allocNode(); 
    V t42  = allocNode(); V t43  = allocNode(); V t44  = allocNode(); V t45  = allocNode(); 
    V t46  = allocNode(); 
    tic;
    PROFILE(  0, t0 , pfnLoadTable(t0, initLiteralSym((S)"lineitem")));
    PROFILE(  1, t5 , pfnColumnValue(t5, t0, initLiteralSym((S)"l_quantity")));
    PROFILE(  2, t6 , pfnColumnValue(t6, t0, initLiteralSym((S)"l_extendedprice")));
    PROFILE(  3, t7 , pfnColumnValue(t7, t0, initLiteralSym((S)"l_discount")));
    PROFILE(  4, t11, pfnColumnValue(t11, t0, initLiteralSym((S)"l_shipdate")));
    PROFILE(  5, t25, q6_loopfusion_0(t25,(V []){t11,t5,t7}));
    PROFILE(  6, t32, q6_peephole_0((V []){t31,t32},t25,(V []){t6,t7}));
    PROFILE(  7, t42, pfnMul(t42, t31, t32));
    PROFILE(  8, t43, pfnSum(t43, t42));
    PROFILE(  9, t44, copyV(t44, initLiteralSym((S)"revenue")));
    PROFILE( 10, t45, pfnList(t45, 1, (V []){t43}));
    PROFILE( 11, t46, pfnTable(t46, t44, t45));
    toc;
    printV2(t46,20);
    R elapsed;
}
