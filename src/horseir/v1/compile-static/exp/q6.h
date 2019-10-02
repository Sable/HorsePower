/*==== FE: Loop fusion with elementwise functions ====*/
/* num_func = 9, targ = t25 */
L q6_loopfusion_0(V z, V *x){
    E elapsed=0;
    // z -> t25
    V x0 = x[0]; // t11
    V x1 = x[1]; // t5
    V x2 = x[2]; // t7
    initV(z,H_B,vn(x0));
    tic;
    DOP(vn(x0), vB(z,i)=AND(AND(AND(GEQ(vD(x0,i),19940101),LT(vD(x0,i),19950101)),LT(vE(x1,i),24)),AND(GEQ(vE(x2,i),0.05),LEQ(vE(x2,i),0.07)))) // 15.7 ms
    toc;
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
    DOI(H_CORE, parZ[i]=offset[i]=0)
    CHECKE(getNumOfNonZero(y,parZ));
    DOI(H_CORE, lenZ += parZ[i])
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    initV(z0,vp(x0),lenZ);
    initV(z1,vp(x1),lenZ);
    DOT(len, if(vB(y,i)){L c=offset[tid]++;vE(z0,c)=vE(x0,i);vE(z1,c)=vE(x1,i);}) R 0;
}

// serial code instead of parallel code
L q6_peephole_0_serial(V *z, V y, V *x){
    V z0 = z[0]; // t31
    V z1 = z[1]; // t32
    V x0 = x[0]; // t6
    V x1 = x[1]; // t7
    L len = vn(y), k = 0;
    L lenZ, offset, c;
    lenZ = offset = c = 0;
    DOI(vn(y), if(vB(y,i))lenZ++)
    initV(z0,vp(x0),lenZ);
    initV(z1,vp(x1),lenZ);
    DOI(len, if(vB(y,i)){vE(z0,c)=vE(x0,i);vE(z1,c)=vE(x1,i);c++;}) R 0;
}


// fuse more operations
L q6_peephole_1(V *z, V y, V *x){
    E elapsed=0;
    V z0 = z[0]; // t42
    V x0 = x[0]; // t6
    V x1 = x[1]; // t7
    L len = vn(y), k = 0;
    L lenZ, offset, c;
    lenZ = offset = c = 0;
    P("size of y: %lld\n", vn(y));
    //DOI(vn(y), if(vB(y,i))lenZ++)
    DOI(vn(y), lenZ+=vB(y,i))
    initV(z0,H_E,lenZ);
    DOI(len, if(vB(y,i)){vE(z0,c)=vE(x0,i)*vE(x1,i);c++;}) // 7.012 ms
    /*
    V t0 = allocNode();
    V t1 = allocNode();
    V t2 = allocNode();
    initV(t0,H_I,len);
    initV(t1,H_I,len);
    initV(t2,H_I,lenZ);
    DOI(len, vI(t0,i)=(int)100*vE(x0,i))
    DOI(len, vI(t1,i)=(int)100*vE(x1,i))
    c = 0;
    tic;
    //DOI(len, if(vB(y,i)){vF(t2,c)=vF(t0,i)*vF(t1,i);c++;}) // 5.789 ms
    //DOI(len, if(vB(y,i)){vI(t2,c)=vI(t0,i)*vI(t1,i);c++;}) // 5.783 ms
    toc;
    */
    R 0;
}

F double2float(E x){
    char tmp[20];
    sprintf(tmp, "%.2lf",x);
    if(x != (E)atof(tmp)){
        P("x = %lf, tmp = %s, value = %f\n", x, tmp, atof(tmp));
        getchar();
    }
    return atof(tmp);
}

// version: testing float
L q6_peephole_2(V *z, V *x){
    E elapsed=0;
    V z0 = z[0]; // t43
    V x0 = x[0]; // t11
    V x1 = x[1]; // t5
    V x2 = x[2]; // t7
    V x3 = x[3]; // t6
    initV(z0, H_E, 1);
    E total = 0;
    V t1 = allocNode();
    V t2 = allocNode();
    V t3 = allocNode();
    L len = vn(x0);
    // (F)E -> F, may generate different result
    // (I)(E * 100) -> I, may generate unexpected results
    initV(t1, H_F, len); DOI(len, vF(t1,i)=(F)(vE(x1,i)))
    initV(t2, H_F, len); DOI(len, vF(t2,i)=(F)(vE(x2,i)))
    initV(t3, H_F, len); DOI(len, vF(t3,i)=(F)(vE(x3,i)))
    //DOI(20, P("t2[%lld] = %d, x2[%lld] = %lf\n",i,vI(t2,i),i,vE(x2,i)));
    //DOI(20, P("t3[%lld] = %lld, x3[%lld] = %lf\n",i,vL(t3,i),i,vE(x3,i)));
    //DOI(len, if(vL(t3,i)/100.0 != vE(x3,i)){\
             P("t3[%lld] = %lld, x3[%lld] = %lf\n",i,vL(t3,i),i,vE(x3,i));});
    //DOI(len, if((E)vF(t3,i) != vE(x3,i)){\
             P("t3[%lld] = %f, x3[%lld] = %lf\n",i,vF(t3,i),i,vE(x3,i));});
    //L c = 0; F tt = 0;
    tic;
    //DOI(vn(x0), \
            if(AND(AND(AND(GEQ(vD(x0,i),19940101),LT(vD(x0,i),19950101)),LT(vE(x1,i),24)),AND(GEQ(vE(x2,i),0.05),LEQ(vE(x2,i),0.07)))) \
            {total+=vE(x3,i)*vE(x2,i);}) // 15.154 ms
    DOI(vn(x0), \
            if(AND(AND(AND(GEQ(vD(x0,i),19940101),LT(vD(x0,i),19950101)),LT(vF(t1,i),24.0f)),AND(GEQ(vF(t2,i),0.05f),LEQ(vF(t2,i),0.07f)))) \
            {total+=vF(t3,i)*vF(t2,i);}) // precision problem
    //DOI(vn(x0), \
            if(AND(AND(AND(GEQ(vD(x0,i),19940101),LT(vD(x0,i),19950101)),LT(vL(t1,i),2400)),AND(GEQ(vL(t2,i),5),LEQ(vL(t2,i),7)))) \
            {total+=(vL(t3,i)/100.0)*(vL(t2,i)/100.0);}) // 15.154 ms
    toc;
    //P("total = %lf\n",total);
    vE(z0, 0) = total;
    R 0;
}

L q6_peephole_3(V *z, V *x){
    E elapsed=0;
    V z0 = z[0]; // t43
    V x0 = x[0]; // t11
    V x1 = x[1]; // t5
    V x2 = x[2]; // t7
    V x3 = x[3]; // t6
    initV(z0, H_F, 1);
    P("type z0 = %s\n", getTypeName(vp(z0)));
    P("type x0 = %s\n", getTypeName(vp(x0)));
    P("type x1 = %s\n", getTypeName(vp(x1)));
    P("type x2 = %s\n", getTypeName(vp(x2)));
    P("type x3 = %s\n", getTypeName(vp(x3)));
    F total = 0;
    DOI(vn(x0), \
            if(AND(AND(AND(GEQ(vD(x0,i),19940101),LT(vD(x0,i),19950101)),LT(vF(x1,i),24.0f)),AND(GEQ(vF(x2,i),0.05f),LEQ(vF(x2,i),0.07f)))) \
            {total+=vF(x3,i)*vF(x2,i);}) // precision problem
    vf(z0) = total;
    return 0;
}

// version: testing i64
// case 1: if read f64 from files and later convert to i64, then it may suffer from a preceision issue
// case 2: the data must be read from files to i64 directly, but with a mask (e.g. showing the scale 2)
L q6_peephole_4(V *z, V *x){
    E elapsed=0;
    V z0 = z[0]; // t43
    V x0 = x[0]; // t11
    V x1 = x[1]; // t5
    V x2 = x[2]; // t7
    V x3 = x[3]; // t6
    initV(z0, H_L, 1);
    L total = 0;
    V t1 = allocNode();
    V t2 = allocNode();
    V t3 = allocNode();
    L len = vn(x0);
    initV(t1, H_L, len); DOI(len, vL(t1,i)=(100*vE(x1,i)))
    initV(t2, H_L, len); DOI(len, vL(t2,i)=(100*vE(x2,i)))
    initV(t3, H_L, len); DOI(len, vL(t3,i)=(100*vE(x3,i)))
    DOI(10, P("%lf " ,vE(x2,i))) P("\n");
    DOI(10, P("%lld ",vL(t2,i))) P("\n");
    tic;
    DOI(vn(x0), \
            if(AND(AND(AND(GEQ(vD(x0,i),19940101),LT(vD(x0,i),19950101)),LT(vL(t1,i),2400)),AND(GEQ(vL(t2,i),5),LEQ(vL(t2,i),7)))) \
            {total+=vL(t3,i)*vL(t2,i);}) // precision problem
    toc;
    vL(z0, 0) = total;
    R 0;
}

L q6_peephole_main(V *z, V *x){
    E elapsed=0;
    V z0 = z[0]; // t43
    V x0 = x[0]; // t11
    V x1 = x[1]; // t5
    V x2 = x[2]; // t7
    V x3 = x[3]; // t6
    //L c;
    //c=0; DOI(vn(x0), if(GEQ(vD(x0,i),19940101)) c++) P("c0=%lld\n",c); //c0=4336142
    //c=0; DOI(vn(x0), if(LT(vD(x0,i),19950101))  c++) P("c1=%lld\n",c); //c1=2574528  <-- 1
    //c=0; DOI(vn(x0), if(LT(vE(x1,i),24))        c++) P("c2=%lld\n",c); //c2=2758822
    //c=0; DOI(vn(x0), if(GEQ(vE(x2,i),0.05))     c++) P("c3=%lld\n",c); //c3=3273484
    //c=0; DOI(vn(x0), if(LEQ(vE(x2,i),0.07))     c++) P("c4=%lld\n",c); //c4=4365288
    //c=0; DOI(vn(x0), if(LT(vD(x0,i),19950101) && GEQ(vD(x0,i),19940101)) c++) P("c10=%lld\n",c); //c10=909455   <-- 0
    //c=0; DOI(vn(x0), if(LT(vD(x0,i),19950101) && LT(vE(x1,i),24))        c++) P("c12=%lld\n",c); //c12=1183087
    //c=0; DOI(vn(x0), if(LT(vD(x0,i),19950101) && GEQ(vE(x2,i),0.05))     c++) P("c13=%lld\n",c); //c13=1404720
    //c=0; DOI(vn(x0), if(LT(vD(x0,i),19950101) && LEQ(vE(x2,i),0.07))     c++) P("c14=%lld\n",c); //c14=1872688
    //c=0; DOI(vn(x0), if(LT(vD(x0,i),19950101) && GEQ(vD(x0,i),19940101) && LT(vE(x1,i),24))        c++) P("c22=%lld\n",c); //c22=417809 <-- 2
    //c=0; DOI(vn(x0), if(LT(vD(x0,i),19950101) && GEQ(vD(x0,i),19940101) && GEQ(vE(x2,i),0.05))     c++) P("c23=%lld\n",c); //c23=495300
    //c=0; DOI(vn(x0), if(LT(vD(x0,i),19950101) && GEQ(vD(x0,i),19940101) && LEQ(vE(x2,i),0.07))     c++) P("c24=%lld\n",c); //c24=662233
    //c=0; DOI(vn(x0), if(LT(vD(x0,i),19950101) && GEQ(vD(x0,i),19940101) && LT(vE(x1,i),24) && GEQ(vE(x2,i),0.05))     c++) P("c33=%lld\n",c); //c33=227979 <-- 3
    //c=0; DOI(vn(x0), if(LT(vD(x0,i),19950101) && GEQ(vD(x0,i),19940101) && LT(vE(x1,i),24) && LEQ(vE(x2,i),0.07))     c++) P("c34=%lld\n",c); //c34=303990 <-- 4
    //getchar();

    initV(z0, H_E, 1);
    E total = 0;
    //DOI(vn(x0), \
            if(AND(AND(AND(GEQ(vD(x0,i),19940101),LT(vD(x0,i),19950101)),LT(vE(x1,i),24)),AND(GEQ(vE(x2,i),0.05),LEQ(vE(x2,i),0.07)))) \
            {total+=vE(x3,i)*vE(x2,i);}) // soln 0, 15 ms (original)
    //DOI(vn(x0), \
            if(AND(AND(AND(AND(GEQ(vD(x0,i),19940101),LT(vD(x0,i),19950101)),LT(vE(x1,i),24)),GEQ(vE(x2,i),0.05)),LEQ(vE(x2,i),0.07))) \
            {total+=vE(x3,i)*vE(x2,i);}) // soln 1
    //DOI(vn(x0), \
            if(AND(AND(AND(AND(GEQ(vD(x0,i),19940101),LT(vD(x0,i),19950101)),GEQ(vE(x2,i),0.05)),LEQ(vE(x2,i),0.07)),LT(vE(x1,i),24))) \
            {total+=vE(x3,i)*vE(x2,i);}) // soln 2, 52
    //DOI(vn(x0), \
            if(AND(AND(AND(AND(GEQ(vD(x0,i),19940101),GEQ(vE(x2,i),0.05)),LT(vD(x0,i),19950101)),LEQ(vE(x2,i),0.07)),LT(vE(x1,i),24))) \
            {total+=vE(x3,i)*vE(x2,i);}) // soln 3, 55
    //DOI(vn(x0), \
            if(AND(AND(AND(AND(GEQ(vD(x0,i),19940101),GEQ(vE(x2,i),0.05)),LEQ(vE(x2,i),0.07)),LT(vD(x0,i),19950101)),LT(vE(x1,i),24))) \
            {total+=vE(x3,i)*vE(x2,i);}) // soln 4, 73
    DOI(vn(x0), \
            if(AND(AND(AND(LT(vD(x0,i),19950101),GEQ(vD(x0,i),19940101)),LT(vE(x1,i),24)),AND(GEQ(vE(x2,i),0.05),LEQ(vE(x2,i),0.07)))) \
            {total+=vE(x3,i)*vE(x2,i);}) // soln 5, 49 (best)
    vE(z0, 0) = total;
    R 0;
}

// Pattern FP3
// Pattern FP4
// Pattern FP5
E compiled_main(){
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
    //PROFILE(  5, t25, q6_loopfusion_0(t25,(V []){t11,t5,t7}));
    //PROFILE(  6, t32, q6_peephole_0((V []){t31,t32},t25,(V []){t6,t7}));
    //PROFILE(  6, t32, q6_peephole_0_serial((V []){t31,t32},t25,(V []){t6,t7}));    
    //PROFILE(  7, t42, pfnMul(t42, t31, t32));
    //PROFILE(99, t32, q6_peephole_1((V []){t42},t25,(V []){t6,t7}));    
    //PROFILE(  8, t43, pfnSum(t43, t42));
    //PROFILE(99, t43, q6_peephole_2((V []){t43},(V []){t11,t5,t7,t6}));    
    //PROFILE(99, t43, q6_peephole_3((V []){t43},(V []){t11,t5,t7,t6}));    
    //PROFILE(99, t43, q6_peephole_4((V []){t43},(V []){t11,t5,t7,t6}));    
    PROFILE( 99, t43, q6_peephole_main((V []){t43},(V []){t11,t5,t7,t6}));    
    PROFILE(  9, t44, copyV(t44, initLiteralSym((S)"revenue")));
    PROFILE( 10, t45, pfnList(t45, 1, (V []){t43}));
    PROFILE( 11, t46, pfnTable(t46, t44, t45));
    metaTable(t46, "result");
    toc;
    printV2(t46,20);
    R elapsed;
}
