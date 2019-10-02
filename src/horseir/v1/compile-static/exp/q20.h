/*==== FE: Loop fusion with elementwise functions ====*/
/* num_func = 3, targ = t130 */
L q20_loopfusion_0(V z, V *x){
    // z -> t130
    V x0 = x[0]; // t122
    initV(z,H_B,vn(x0));
    DOP(vn(x0), vB(z,i)=AND(GEQ(vD(x0,i),19940101),LT(vD(x0,i),19950101))) R 0;
}
/*==== FP: Loop fusion with patterns ====*/
L q20_peephole_0(V *z, V y, V *x){
    V z0 = z[0]; // t71
    V z1 = z[1]; // t72
    V x0 = x[0]; // t70
    V x1 = x[1]; // t63
    //getInfoVar(x0); getInfoVar(x1); getchar();
    //Variable x0 has type H_I and len 800000
    //Variable x1 has type H_I and len 800000
    L len = vn(y), k = 0;
    L lenZ = 0, parZ[H_CORE], offset[H_CORE];
    DOI(H_CORE, parZ[i]=offset[i]=0)
    CHECKE(getNumOfNonZero(y,parZ));
    DOI(H_CORE, lenZ += parZ[i])
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    initV(z0,vp(x0),lenZ);
    initV(z1,vp(x1),lenZ);
    DOT(len, if(vB(y,i)){L c=offset[tid]++;vLL(z0,c)=vLL(x0,i);vLL(z1,c)=vLL(x1,i);}) R 0;
}
L q20_peephole_1(V *z, V y, V *x){
    V z0 = z[0]; // t132
    V z1 = z[1]; // t133
    V z2 = z[2]; // t135
    V x0 = x[0]; // t113
    V x1 = x[1]; // t114
    V x2 = x[2]; // t116
    //getInfoVar(x0); getInfoVar(x1); getInfoVar(x2); getchar();
    //Variable x0 has type H_I and len 6001215
    //Variable x1 has type H_I and len 6001215
    //Variable x2 has type H_E and len 6001215
    L len = vn(y), k = 0;
    L lenZ = 0, parZ[H_CORE], offset[H_CORE];
    DOI(H_CORE, parZ[i]=offset[i]=0)
    CHECKE(getNumOfNonZero(y,parZ));
    DOI(H_CORE, lenZ += parZ[i])
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    initV(z0,vp(x0),lenZ);
    initV(z1,vp(x1),lenZ);
    initV(z2,vp(x2),lenZ);
    DOT(len, if(vB(y,i)){L c=offset[tid]++;vLL(z0,c)=vLL(x0,i);vLL(z1,c)=vLL(x1,i);vE(z2,c)=vE(x2,i);}) R 0;
}

L q20_peephole_2(V *z, V y, V *x){
    V z0 = z[0]; // t179
    V z1 = z[1]; // t180
    V x0 = x[0]; // t27
    V x1 = x[1]; // t28
    L len = vn(y), k = 0;
    L lenZ = 0, parZ[H_CORE], offset[H_CORE];
    DOI(H_CORE, parZ[i]=offset[i]=0)
    CHECKE(getNumOfNonZero(y,parZ));
    DOI(H_CORE, lenZ += parZ[i])
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    initV(z0,vp(x0),lenZ);
    initV(z1,vp(x1),lenZ);
    DOT(len, if(vB(y,i)){L c=offset[tid]++;vS(z0,c)=vS(x0,i);vS(z1,c)=vS(x1,i);}) R 0;
}

// input type t27 and t28 changed from Q to S
//L q20_peephole_2(V *z, V y, V *x){
//    V z0 = z[0]; // t179
//    V z1 = z[1]; // t180
//    V x0 = x[0]; // t27
//    V x1 = x[1]; // t28
//    L len = vn(y), k = 0;
//    L lenZ = 0, parZ[H_CORE], offset[H_CORE];
//    DOI(H_CORE, parZ[i]=offset[i]=0)
//    CHECKE(getNumOfNonZero(y,parZ));
//    DOI(H_CORE, lenZ += parZ[i])
//    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
//    initV(z0,vp(x0),lenZ);
//    initV(z1,vp(x1),lenZ);
//    DOT(len, if(vB(y,i)){L c=offset[tid]++;vQ(z0,c)=vQ(x0,i);vQ(z1,c)=vQ(x1,i);}) R 0; // vQ -> vS
//}

L q20_peephole_3(V *z, V *x, V y){
    L r0 = vn(y);// y -> t150
    V z0 = z[0]; // t153
    V x0 = x[0]; // t135
    initV(z0, H_E, r0);
    /* slow: total = 543210, max = 8, min = 1
    DOJ(r0, {V t=vV(y,j);  E c0=0; \
        DOP(vn(t), {L k=vL(t,i); c0+=vE(x0,k);},\
        reduction(+:c0)) \
        vE(z0,j)=c0; \
    })*/
    DOP(r0, {V t=vV(y,i);  E c0=0; \
        DOJ(vn(t), c0+=vE(x0,vL(t,j));) vE(z0,i)=c0; })
    R 0;
}
L q20_peephole_4(V z, V x, V y){
    // z -> t153, x -> t135, y -> t150
    L r0 = vn(y);
    initV(z, H_L, r0);
    DOP(r0, vL(z,i) = vn(vV(y,i))) R 0;
}
// Pattern FP3
// Pattern FP4
// Pattern FP5
E compiled_main(){
    E elapsed=0;
    V t0   = allocNode(); V t2   = allocNode(); V t5   = allocNode(); V t10  = allocNode(); 
    V t11  = allocNode(); V t12  = allocNode(); V t13  = allocNode(); V t14  = allocNode(); 
    V t18  = allocNode(); V t19  = allocNode(); V t21  = allocNode(); V t26  = allocNode(); 
    V t27  = allocNode(); V t28  = allocNode(); V t34  = allocNode(); V t36  = allocNode(); 
    V t44  = allocNode(); V t60  = allocNode(); V t61  = allocNode(); V t62  = allocNode(); 
    V t63  = allocNode(); V t66  = allocNode(); V t67  = allocNode(); V t68  = allocNode(); 
    V t69  = allocNode(); V t70  = allocNode(); V t71  = allocNode(); V t72  = allocNode(); 
    V t111 = allocNode(); V t113 = allocNode(); V t114 = allocNode(); V t116 = allocNode(); 
    V t122 = allocNode(); V t128 = allocNode(); V t129 = allocNode(); V t130 = allocNode(); 
    V t132 = allocNode(); V t133 = allocNode(); V t135 = allocNode(); V t147 = allocNode(); 
    V t148 = allocNode(); V t149 = allocNode(); V t150 = allocNode(); V t151 = allocNode(); 
    V t152 = allocNode(); V t153 = allocNode(); V t154 = allocNode(); V t155 = allocNode(); 
    V t156 = allocNode(); V t157 = allocNode(); V t158 = allocNode(); V t159 = allocNode(); 
    V t160 = allocNode(); V t163 = allocNode(); V t172 = allocNode(); V t179 = allocNode(); 
    V t180 = allocNode(); V t185 = allocNode(); V t186 = allocNode(); V t187 = allocNode(); 
    V t188 = allocNode(); V t189 = allocNode(); V t190 = allocNode(); V t191 = allocNode(); 
    tic;
    PROFILE(  0, t0 , pfnLoadTable(t0, initLiteralSym((S)"nation")));
    PROFILE(  1, t2 , pfnColumnValue(t2, t0, initLiteralSym((S)"n_name")));
    PROFILE(  2, t5 , pfnEq(t5, t2, initLiteralSym((S)"CANADA")));
    PROFILE(  3, t10, pfnLoadTable(t10, initLiteralSym((S)"supplier")));
    PROFILE(  4, t11, pfnColumnValue(t11, t10, initLiteralSym((S)"s_suppkey")));
    PROFILE(  5, t12, pfnColumnValue(t12, t10, initLiteralSym((S)"s_name")));
    PROFILE(  6, t13, pfnColumnValue(t13, t10, initLiteralSym((S)"s_address")));
    PROFILE(  7, t14, pfnColumnValue(t14, t10, initLiteralSym((S)"s_nationkey")));
    PROFILE(  8, t18, pfnValues(t18, t14));
    PROFILE(  9, t19, pfnIndex(t19, t5, t18));
    PROFILE( 10, t21, pfnWhere(t21, t19));
    PROFILE( 11, t26, pfnIndex(t26, t11, t21));
    PROFILE( 12, t27, pfnIndex(t27, t12, t21));
    PROFILE( 13, t28, pfnIndex(t28, t13, t21));
    //P(">> join1: nation + supplier => %d\n",vn(t21)); getchar();  // expect 412
    PROFILE( 14, t34, pfnLoadTable(t34, initLiteralSym((S)"part")));
    PROFILE( 15, t36, pfnColumnValue(t36, t34, initLiteralSym((S)"p_name")));
    PROFILE( 16, t44, pfnLike(t44, t36, initLiteralSym((S)"forest%")));
    PROFILE( 17, t60, pfnLoadTable(t60, initLiteralSym((S)"partsupp")));
    PROFILE( 18, t61, pfnColumnValue(t61, t60, initLiteralSym((S)"ps_partkey")));
    PROFILE( 19, t62, pfnColumnValue(t62, t60, initLiteralSym((S)"ps_suppkey")));
    PROFILE( 20, t63, pfnColumnValue(t63, t60, initLiteralSym((S)"ps_availqty")));
    PROFILE( 21, t66, pfnValues(t66, t61));
    PROFILE( 22, t67, pfnIndex(t67, t44, t66));
    PROFILE( 23, t68, pfnFetch(t68, t61));
    PROFILE( 24, t69, pfnCompress(t69, t67, t68));
    PROFILE( 25, t70, pfnFetch(t70, t62));
    PROFILE( 26, t72, q20_peephole_0((V []){t71,t72},t67,(V []){t70,t63}));
    //P(">> join2: part + partsupp => %d\n",vn(t66)); getchar();
    PROFILE( 27, t111, pfnLoadTable(t111, initLiteralSym((S)"lineitem")));
    PROFILE( 28, t113, pfnColumnValue(t113, t111, initLiteralSym((S)"l_partkey")));
    PROFILE( 29, t114, pfnColumnValue(t114, t111, initLiteralSym((S)"l_suppkey")));
    PROFILE( 30, t116, pfnColumnValue(t116, t111, initLiteralSym((S)"l_quantity")));
    PROFILE( 31, t122, pfnColumnValue(t122, t111, initLiteralSym((S)"l_shipdate")));
    PROFILE( 32, t130, q20_loopfusion_0(t130,(V []){t122}));
    PROFILE( 33, t135, q20_peephole_1((V []){t132,t133,t135},t130,(V []){t113,t114,t116}));
    PROFILE( 34, t147, pfnList(t147, 2, (V []){t133 ,t132}));
    //getInfoVar(t133); getInfoVar(t132); getchar();
    // Variable t133 has type H_L and len 909455 (correct)
    // Variable t132 has type H_L and len 909455
    //P("input size for groupby\n"); getInfoVar(t147); getchar();
    PROFILE( 35, t148, pfnGroup(t148, t147)); // most expensive
    PROFILE( 36, t149, pfnKeys(t149, t148));
    //P("output key size\n"); getInfoVar(t149); getchar();
    // Variable t149 has type H_L and len 180600  (exepct 543210)
    PROFILE( 37, t150, pfnValues(t150, t148));
    PROFILE( 38, t153, q20_peephole_3((V []){t153},(V []){t135},t150));
    PROFILE( 39, t154, pfnIndex(t154, t133, t149));
    PROFILE( 40, t155, pfnIndex(t155, t132, t149));
    PROFILE( 41, t156, pfnMul(t156, initLiteralF64(0.5), t153));
    PROFILE( 42, t157, pfnList(t157, 3, (V []){t72 ,t69 ,t71}));
    PROFILE( 43, t158, pfnList(t158, 3, (V []){t156 ,t155 ,t154}));
    //getInfoVar(t72); getInfoVar(t69); getInfoVar(t71); getchar();
    // Variable t72 has type H_I and len 8508 (correct)
    // Variable t69 has type H_I and len 8508
    // Variable t71 has type H_I and len 8508
    //getInfoVar(t156); getInfoVar(t155); getInfoVar(t154); getchar();
    // Variable t156 has type H_E and len 543210 (correct)
    // Variable t155 has type H_I and len 543210
    // Variable t154 has type H_I and len 543210
    //PROFILE( 44, t159, pfnJoinIndex(t160,t157,t158,initLiteralSymVector(3, (S []){"gt","eq","eq"}))); // why t160???
    PROFILE( 44, t159, pfnJoinIndex(t159,t157,t158,initLiteralSymVector(3, (S []){"gt","eq","eq"})));
    PROFILE( 45, t160, pfnIndex(t160, t159, initLiteralI64(0)));
    //P(">> join4: result (3 columns) => %d\n",vn(t160)); getchar();
    // >> join4: result (3 columns) => 5833 (correct)
    PROFILE( 46, t163, pfnIndex(t163, t71, t160));
    //PROFILE( 47, t172, pfnMember(t172, t163, t26));
    PROFILE( 47, t172, pfnMember(t172, t26, t163));
    PROFILE( 48, t180, q20_peephole_2((V []){t179,t180},t172,(V []){t27,t28}));
    PROFILE( 49, t185, pfnList(t185, 1, (V []){t179}));
    //getInfoVar(t179); getchar();  // expect 186
    // Variable t179 has type H_S and len 89
    PROFILE( 50, t186, pfnOrderBy(t186, t185, initLiteralBool(1)));
    PROFILE( 51, t187, pfnIndex(t187, t179, t186));
    PROFILE( 52, t188, pfnIndex(t188, t180, t186));
    PROFILE( 53, t189, copyV(t189, initLiteralSymVector(2, (S []){"s_name","s_address"})));
    PROFILE( 54, t190, pfnList(t190, 2, (V []){t187 ,t188}));
    PROFILE( 55, t191, pfnTable(t191, t189, t190));
    toc;
    printV2(t191,20);
    R elapsed;
}
