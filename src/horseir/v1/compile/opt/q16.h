/*==== FE: Loop fusion with elementwise functions ====*/
/* num_func = 4, targ = t32 */
L q16_loopfusion_0(V z, V *x){
    // z -> t32
    V x0 = x[0]; // t27
    V x1 = x[1]; // t20
    V x2 = x[2]; // t29
    initV(z,H_B,vn(x0));
    Q id0 = getSymbol("Brand#45");
    DOP(vn(x0), vB(z,i)=AND(AND(vB(x0,i),NEQ(vQ(x1,i),id0)),NOT(vB(x2,i)))) R 0;
}
/*==== FP: Loop fusion with patterns ====*/
L q16_peephole_0(V *z, V y, V *x){
    V z0 = z[0]; // t73
    V z1 = z[1]; // t74
    V z2 = z[2]; // t75
    V z3 = z[3]; // t80
    V x0 = x[0]; // t55
    V x1 = x[1]; // t56
    V x2 = x[2]; // t57
    V x3 = x[3]; // t64
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
    DOT(len, if(vB(y,i)){L c=offset[tid]++;vQ(z0,c)=vQ(x0,i);vQ(z1,c)=vQ(x1,i);vL(z2,c)=vL(x2,i);vL(z3,c)=vL(x3,i);}) R 0;
}
// Pattern FP3
// Pattern FP4
L q16_peephole_1(V z, V x, V y){
    // z -> t94, x -> t80, y -> t87
    L r0 = vn(y);
    initV(z, H_L, r0);
    DOP(r0, {V t=vV(y,i); L len=vn(t); L tot=0; B f[199]={0};\
        DOJ(len, if(!f[j]){ \
        DOK(len, if(k!=j && vL(x,vL(t,j))==vL(x,vL(t,k))) f[k]=1)\
        f[j]=1; tot++;}) vL(z,i)=tot; }) R 0;
}
// Pattern FP5
E compiledQ16(){
    E elapsed=0;
    V t0   = allocNode(); V t1   = allocNode(); V t7   = allocNode(); V t8   = allocNode(); 
    V t9   = allocNode(); V t16  = allocNode(); V t20  = allocNode(); V t21  = allocNode(); 
    V t22  = allocNode(); V t26  = allocNode(); V t27  = allocNode(); V t28  = allocNode(); 
    V t29  = allocNode(); V t30  = allocNode(); V t31  = allocNode(); V t32  = allocNode(); 
    V t42  = allocNode(); V t43  = allocNode(); V t44  = allocNode(); V t48  = allocNode(); 
    V t49  = allocNode(); V t50  = allocNode(); V t51  = allocNode(); V t55  = allocNode(); 
    V t56  = allocNode(); V t57  = allocNode(); V t63  = allocNode(); V t64  = allocNode(); 
    V t68  = allocNode(); V t69  = allocNode(); V t73  = allocNode(); V t74  = allocNode(); 
    V t75  = allocNode(); V t80  = allocNode(); V t84  = allocNode(); V t85  = allocNode(); 
    V t86  = allocNode(); V t87  = allocNode(); V t88  = allocNode(); V t89  = allocNode(); 
    V t90  = allocNode(); V t91  = allocNode(); V t92  = allocNode(); V t93  = allocNode(); 
    V t94  = allocNode(); V t95  = allocNode(); V t96  = allocNode(); V t97  = allocNode(); 
    V t98  = allocNode(); V t99  = allocNode(); V t100 = allocNode(); V t101 = allocNode(); 
    V t102 = allocNode(); V t103 = allocNode(); 
    tic;
    PROFILE(  0, t0 , pfnLoadTable(t0, initLiteralSym((S)"supplier")));
    PROFILE(  1, t1 , pfnColumnValue(t1, t0, initLiteralSym((S)"s_suppkey")));
    PROFILE(  2, t7 , pfnColumnValue(t7, t0, initLiteralSym((S)"s_comment")));
    PROFILE(  3, t8 , pfnLike(t8, t7, initLiteralSym((S)"%Customer%Complaints%")));
    PROFILE(  4, t9 , pfnCompress(t9, t8, t1));
    PROFILE(  5, t16, pfnLoadTable(t16, initLiteralSym((S)"part")));
    PROFILE(  6, t20, pfnColumnValue(t20, t16, initLiteralSym((S)"p_brand")));
    PROFILE(  7, t21, pfnColumnValue(t21, t16, initLiteralSym((S)"p_type")));
    PROFILE(  8, t22, pfnColumnValue(t22, t16, initLiteralSym((S)"p_size")));
    PROFILE(  9, t26, copyV(t26, initLiteralI64Vector(8, (L []){49,14,23,45,19,3,36,9})));
    PROFILE( 10, t27, pfnMember(t27, t26, t22));
    PROFILE( 11, t29, pfnLike(t29, t21, initLiteralSym((S)"MEDIUM POLISHED%")));
    PROFILE( 12, t32, q16_loopfusion_0(t32,(V []){t27,t20,t29}));
    PROFILE( 13, t42, pfnLoadTable(t42, initLiteralSym((S)"partsupp")));
    PROFILE( 14, t43, pfnColumnValue(t43, t42, initLiteralSym((S)"ps_partkey")));
    PROFILE( 15, t44, pfnColumnValue(t44, t42, initLiteralSym((S)"ps_suppkey")));
    PROFILE( 16, t48, pfnValues(t48, t43));
    PROFILE( 17, t49, pfnIndex(t49, t32, t48));
    PROFILE( 18, t50, pfnCompress(t50, t49, t48));
    PROFILE( 19, t51, pfnWhere(t51, t49));
    PROFILE( 20, t55, pfnIndex(t55, t20, t50));
    PROFILE( 21, t56, pfnIndex(t56, t21, t50));
    PROFILE( 22, t57, pfnIndex(t57, t22, t50));
    PROFILE( 23, t63, pfnFetch(t63, t44));
    PROFILE( 24, t64, pfnIndex(t64, t63, t51));
    PROFILE( 25, t68, pfnMember(t68, t9, t64));
    PROFILE( 26, t69, pfnNot(t69, t68));
    PROFILE( 27, t80, q16_peephole_0((V []){t73,t74,t75,t80},t69,(V []){t55,t56,t57,t64}));
    PROFILE( 28, t84, pfnList(t84, 3, (V []){t73 ,t74 ,t75}));
    PROFILE( 29, t85, pfnGroup(t85, t84));
    PROFILE( 30, t86, pfnKeys(t86, t85));
    PROFILE( 31, t87, pfnValues(t87, t85));
    PROFILE( 32, t88, pfnIndex(t88, t73, t86));
    PROFILE( 33, t89, pfnIndex(t89, t74, t86));
    PROFILE( 34, t90, pfnIndex(t90, t75, t86));
    PROFILE( 35, t94, q16_peephole_1(t94,t80,t87));
    PROFILE( 36, t95, pfnList(t95, 4, (V []){t94 ,t88 ,t89 ,t90}));
    PROFILE( 37, t96, pfnOrderBy(t96, t95, initLiteralBoolVector(4, (B []){0,1,1,1})));
    PROFILE( 38, t97, pfnIndex(t97, t88, t96));
    PROFILE( 39, t98, pfnIndex(t98, t89, t96));
    PROFILE( 40, t99, pfnIndex(t99, t90, t96));
    PROFILE( 41, t100, pfnIndex(t100, t94, t96));
    PROFILE( 42, t101, copyV(t101, initLiteralSymVector(4, (S []){"p_brand","p_type","p_size","supplier_cnt"})));
    PROFILE( 43, t102, pfnList(t102, 4, (V []){t97 ,t98 ,t99 ,t100}));
    PROFILE( 44, t103, pfnTable(t103, t101, t102));
    toc;
    printV2(t103,20);
    R elapsed;
}
