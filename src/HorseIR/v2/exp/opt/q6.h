static I horse_main(V *h_rtn);
static I q6_deepfusion_0(V z, V *x);

static I q6_deepfusion_0(V z, V *x){
    V x0=x[0]; // t12
    V x1=x[1]; // t6
    V x2=x[2]; // t8
    V x3=x[3]; // t7
    E c = 0;
    initV(z, H_E, 1);
    //DOP(vn(z), if(AND(AND(AND(GEQ(vD(x0,i),19940101),LT(vD(x0,i),19950101)),LT(vE(x1,i),24)),AND(GEQ(vE(x2,i),0.05),LEQ(vE(x2,i),0.07)))){\
        c+=MUL(vE(x3,i),vE(x2,i)); ttt++;})
    /*
     * when generating parallel code, a reduction operation is needed
     */
    DOI(vn(x0), if(AND(AND(AND(GEQ(vD(x0,i),19940101),LT(vD(x0,i),19950101)),LT(vE(x1,i),24)),AND(GEQ(vE(x2,i),0.05),LEQ(vE(x2,i),0.07)))){\
        c+=MUL(vE(x3,i),vE(x2,i));})
    vE(z,0) = c;
    R 0;
}

static I horse_main(V *h_rtn){
    V t0 = incV(); V t6 = incV(); V t7 = incV(); V t8 = incV();
    V t20 = incV(); V t21 = incV(); V t22 = incV(); V t23 = incV();
    V t32 = incV(); V t33 = incV(); V t26 = incV(); V t43 = incV();
    V t44 = incV(); V t45 = incV(); V t46 = incV(); V t47 = incV();
    /* missing decls */
    V t12 = incV();
    PROFILE(4, pfnLoadTable(t0, LiteralSymbol("lineitem")));
    PROFILE(5, pfnColumnValue(t6, t0, LiteralSymbol("l_quantity")));
    PROFILE(6, pfnColumnValue(t7, t0, LiteralSymbol("l_extendedprice")));
    PROFILE(7, pfnColumnValue(t8, t0, LiteralSymbol("l_discount")));
    PROFILE(8, pfnColumnValue(t12, t0, LiteralSymbol("l_shipdate")));
    PROFILE(21, q6_deepfusion_0(t44, (V[]){t12,t6,t8,t7}));
    PROFILE(22, copyV(t45, LiteralSymbol("revenue")));
    PROFILE(23, pfnList(t46, 1, (V []){t44})); // fix code gen for pfnList's args
    PROFILE(24, pfnTable(t47, t45, t46));
    h_rtn[0] = t47;
    return 0;
}

E horse_entry(){
    V rtns[99];
    tic();
    HORSE_UDF(horse_main(rtns));
    E elapsed = calc_toc();
    P("The elapsed time (ms): %g\n", elapsed);
    P("Output:\n");
    DOI(1, printV(rtns[i]))
    return elapsed;
}
