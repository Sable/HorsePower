/*==== FE: Loop fusion with elementwise functions ====*/
/* num_func = 8, targ = t26 */
L q12_loopfusion_0(V z, V *x){
    // z -> t26
    V x0 = x[0]; // t13
    V x1 = x[1]; // t18
    V x2 = x[2]; // t12
    V x3 = x[3]; // t11
    initV(z,H_B,vn(x0));
    DOP(vn(x0), vB(z,i)=AND(AND(GEQ(vD(x0,i),19940101),LT(vD(x0,i),19950101)),AND(AND(vB(x1,i),LT(vD(x2,i),vD(x0,i))),LT(vD(x3,i),vD(x2,i))))) R 0;
    //DOP(vn(x0), vB(z,i)=AND(AND(AND(vB(x1,i),LT(vD(x2,i),vD(x0,i))),LT(vD(x3,i),vD(x2,i))),AND(GEQ(vD(x0,i),19940101),LT(vD(x0,i),19950101)))) R 0; //slow
}
/* num_func = 10, targ = t88 */
L q12_loopfusion_1(V z, V *x){
    // z -> t88
    V x0 = x[0]; // t78
    Q s0 = getSymbol("1-URGENT");
    Q s1 = getSymbol("2-HIGH");
    initV(z,H_L,vn(x0));
    //DOP(vn(x0), vL(z,i)=PLUS(MUL(OR(EQ(vQ(x0,i),getSymbol("1-URGENT")),EQ(vQ(x0,i),getSymbol("2-HIGH"))),1),MUL(NOT(OR(EQ(vQ(x0,i),getSymbol("1-URGENT")),EQ(vQ(x0,i),getSymbol("2-HIGH")))),0))) R 0;
    DOP(vn(x0), vL(z,i)=PLUS(MUL(OR(EQ(vQ(x0,i),s0),EQ(vQ(x0,i),s1)),1),MUL(NOT(OR(EQ(vQ(x0,i),s0),EQ(vQ(x0,i),s1))),0))) R 0;
}
/* num_func = 10, targ = t95 */
L q12_loopfusion_2(V z, V *x){
    // z -> t95
    V x0 = x[0]; // t78
    Q s0 = getSymbol("1-URGENT");
    Q s1 = getSymbol("2-HIGH");
    initV(z,H_L,vn(x0));
    //DOP(vn(x0), vL(z,i)=PLUS(MUL(AND(NEQ(vQ(x0,i),getSymbol("1-URGENT")),NEQ(vQ(x0,i),getSymbol("2-HIGH"))),1),MUL(NOT(AND(NEQ(vQ(x0,i),getSymbol("1-URGENT")),NEQ(vQ(x0,i),getSymbol("2-HIGH")))),0))) R 0;
    DOP(vn(x0), vL(z,i)=PLUS(MUL(AND(NEQ(vQ(x0,i),s0),NEQ(vQ(x0,i),s1)),1),MUL(NOT(AND(NEQ(vQ(x0,i),s0),NEQ(vQ(x0,i),s1))),0))) R 0;
}
/*==== FP: Loop fusion with patterns ====*/
L q12_peephole_0(V *z, V *x, V y){
    L r0 = vn(y);// y -> t99
    V z0 = z[0]; // t103
    V z1 = z[1]; // t106
    V x0 = x[0]; // t88
    V x1 = x[1]; // t95
    initV(z0, H_L, r0);
    initV(z1, H_L, r0);
    DOJ(r0, {V t=vV(y,j);  L c0=0; L c1=0; \
        DOP(vn(t), {L k=vL(t,i); c0+=vL(x0,k); c1+=vL(x1,k);},\
        reduction(+:c0,c1)) \
        vL(z0,j)=c0; \
        vL(z1,j)=c1; \
    })
    R 0;
}
// Pattern FP3
// Pattern FP4
// Pattern FP5
// new pattern 1
L q12_new_0(V *z, V *x){
    V z0 = z[0]; // t70
    V z1 = z[1]; // t78
    V x0 = x[0]; // t26
    V x1 = x[1]; // t53
    V x2 = x[2]; // t15
    V x3 = x[3]; // t49
    L lenX = vn(x0);
    L lenZ = 0, parZ[H_CORE], offset[H_CORE];
    memset(parZ  , 0, sizeof(L)*H_CORE);
    memset(offset, 0, sizeof(L)*H_CORE);
    CHECKE(getNumOfNonZero(x0,parZ));
    DOI(H_CORE, lenZ += parZ[i])
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    initV(z0,vp(x2),lenZ);
    initV(z1,vp(x3),lenZ);
    //P("vp(x1)=%s, vp(x2)=%s, vp(x3)=%s\n", getTypeName(vp(x1)),getTypeName(vp(x2)),getTypeName(vp(x3)));
    if(lenX != lenZ){
        DOT(lenX, if(vB(x0,i)){L t=offset[tid]++; \
                    vQ(z0,t)=vQ(x2,i); \
                    vQ(z1,t)=vQ(x3,vL(x1,i));})
    }
    else {
        DOP(lenX, { vQ(z0,i)=vQ(x2,i); \
                    vQ(z1,i)=vQ(x3,vL(x1,i));})
    }
    //getchar();
    R 0;
}
E compiled_main(){
    E elapsed=0;
    V t0   = allocNode(); V t1   = allocNode(); V t11  = allocNode(); V t12  = allocNode(); 
    V t13  = allocNode(); V t15  = allocNode(); V t17  = allocNode(); V t18  = allocNode(); 
    V t19  = allocNode(); V t20  = allocNode(); V t21  = allocNode(); V t22  = allocNode(); 
    V t23  = allocNode(); V t24  = allocNode(); V t25  = allocNode(); V t26  = allocNode(); 
    V t43  = allocNode(); V t49  = allocNode(); V t53  = allocNode(); V t54  = allocNode(); 
    V t55  = allocNode(); V t70  = allocNode(); V t78  = allocNode(); V t82  = allocNode(); 
    V t83  = allocNode(); V t84  = allocNode(); V t85  = allocNode(); V t86  = allocNode(); 
    V t87  = allocNode(); V t88  = allocNode(); V t89  = allocNode(); V t90  = allocNode(); 
    V t91  = allocNode(); V t92  = allocNode(); V t93  = allocNode(); V t94  = allocNode(); 
    V t95  = allocNode(); V t96  = allocNode(); V t97  = allocNode(); V t98  = allocNode(); 
    V t99  = allocNode(); V t100 = allocNode(); V t101 = allocNode(); V t102 = allocNode(); 
    V t103 = allocNode(); V t104 = allocNode(); V t105 = allocNode(); V t106 = allocNode(); 
    V t107 = allocNode(); V t108 = allocNode(); V t109 = allocNode(); V t110 = allocNode(); 
    V t111 = allocNode(); V t112 = allocNode(); V t113 = allocNode(); V t114 = allocNode(); 
    tic;
    PROFILE(  0, t0 , pfnLoadTable(t0, initLiteralSym((S)"lineitem")));
    PROFILE(  1, t1 , pfnColumnValue(t1, t0, initLiteralSym((S)"l_orderkey")));
    PROFILE(  2, t11, pfnColumnValue(t11, t0, initLiteralSym((S)"l_shipdate")));
    PROFILE(  3, t12, pfnColumnValue(t12, t0, initLiteralSym((S)"l_commitdate")));
    PROFILE(  4, t13, pfnColumnValue(t13, t0, initLiteralSym((S)"l_receiptdate")));
    PROFILE(  5, t15, pfnColumnValue(t15, t0, initLiteralSym((S)"l_shipmode")));
    PROFILE(  6, t17, copyV(t17, initLiteralSymVector(2, (S []){"MAIL","SHIP"})));
    PROFILE(  7, t18, pfnMember(t18, t17, t15));
    PROFILE(  8, t26, q12_loopfusion_0(t26,(V []){t13,t18,t12,t11}));
    PROFILE(  9, t43, pfnLoadTable(t43, initLiteralSym((S)"orders")));
    PROFILE( 10, t49, pfnColumnValue(t49, t43, initLiteralSym((S)"o_orderpriority")));
    PROFILE( 11, t53, pfnValues(t53, t1));
    // a pattern
    //PROFILE( 12, t54, pfnCompress(t54, t26, t53));
    //PROFILE( 13, t55, pfnWhere(t55, t26));
    //PROFILE( 14, t70, pfnIndex(t70, t15, t55));
    //PROFILE( 15, t78, pfnIndex(t78, t49, t54));
    PROFILE( 99, ttt, q12_new_0((V []){t70,t78}, (V []){t26,t53,t15,t49}));

    PROFILE( 16, t88, q12_loopfusion_1(t88,(V []){t78}));
    PROFILE( 17, t95, q12_loopfusion_2(t95,(V []){t78}));
    PROFILE( 18, t96, pfnList(t96, 1, (V []){t70}));
    PROFILE( 19, t97, pfnGroup(t97, t96));
    PROFILE( 20, t98, pfnKeys(t98, t97));
    PROFILE( 21, t99, pfnValues(t99, t97));
    PROFILE( 22, t100, pfnIndex(t100, t70, t98));
    PROFILE( 23, t106, q12_peephole_0((V []){t103,t106},(V []){t88,t95},t99));
    PROFILE( 24, t107, pfnList(t107, 1, (V []){t100}));
    PROFILE( 25, t108, pfnOrderBy(t108, t107, initLiteralBool(1)));
    PROFILE( 26, t109, pfnIndex(t109, t100, t108));
    PROFILE( 27, t110, pfnIndex(t110, t103, t108));
    PROFILE( 28, t111, pfnIndex(t111, t106, t108));
    PROFILE( 29, t112, copyV(t112, initLiteralSymVector(3, (S []){"l_shipmode","high_line_count","low_line_count"})));
    PROFILE( 30, t113, pfnList(t113, 3, (V []){t109 ,t110 ,t111}));
    PROFILE( 31, t114, pfnTable(t114, t112, t113));
    toc;
    printV2(t114,20);
    R elapsed;
}
