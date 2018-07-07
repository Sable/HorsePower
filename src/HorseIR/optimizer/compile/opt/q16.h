/* num_func = 4, targ = t32 */
L q16_loopfusion_0(V z, V *x){
    // z -> t32
    V x0 = x[0]; // t27
    V x1 = x[1]; // t20
    V x2 = x[2]; // t29
    initV(z,H_B,vn(x0));
    DOP(vn(x0), vB(z,i)=AND(AND(vB(x0,i),NEQ(vQ(x1,i),getSymbol("Brand#45"))),NOT(vB(x2,i)))) R 0;
}
L q16_peephole_0(V *z, V y, V *x){
    V z0 = z[0]; // w1
    V z1 = z[1]; // w2
    V x0 = x[0]; // p0
    V x1 = x[1]; // p1
    L len = vn(y), k = 0;
    L lenZ = 0, parZ[H_CORE], offset[H_CORE];
    DOI(H_CORE, parZ[i]=offset[i]=0)
    CHECKE(getNumOfNonZero(y,parZ));
    DOI(H_CORE, lenZ += parZ[i])
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    initV(z0,vp(x0),lenZ);
    initV(z1,vp(x1),lenZ);
    DOT(len, if(vB(y,i)){L c=offset[tid]++;vL(z0,c)=vL(x0,i);vL(z1,c)=vL(x1,i);})
}

/* added */
L q16_peephole_1(V z, V x, V y){
    // x -> t87
    // y -> u0
    L r0 = vn(y);
    initV(z,H_L,r0);
    DOP(r0, {V t=vV(y,i); L len=vn(t); L tot=0; B f[199]={0}; \
            DOJ(len, if(!f[j]){ \
                DOK(len, if(k!=j && vL(x,vL(t,j)) == vL(x,vL(t,k))) f[k]=1) \
                f[j]=1; tot++;}) vL(z,i)=tot; })
    R 0;
}

E compiledQ16(){
    E elapsed=0;
    V t0  = allocNode(); V t1  = allocNode(); V t7  = allocNode(); V t8  = allocNode();
    V t16 = allocNode(); V t17 = allocNode(); V t20 = allocNode(); V t21 = allocNode();
    V t22 = allocNode(); V t26 = allocNode(); V t27 = allocNode(); V t29 = allocNode();
    V t32 = allocNode(); V t42 = allocNode(); V t43 = allocNode(); V t44 = allocNode();
    V k0  = allocNode(); V k1  = allocNode(); V p1  = allocNode(); V t64 = allocNode();
    V t68 = allocNode(); V w0  = allocNode(); V w1  = allocNode(); V w2 = allocNode();
    V t84 = allocNode(); V t85 = allocNode(); V t86 = allocNode(); V t87 = allocNode();
    V t95 = allocNode(); V t96 = allocNode(); V u0  = allocNode(); V u1  = allocNode();
    V u2  = allocNode(); V u3  = allocNode(); V u4  = allocNode(); V u5  = allocNode();
    V u6  = allocNode(); V u7  = allocNode(); V u8  = allocNode(); V t99  = allocNode();
    V t100= allocNode(); V t101= allocNode(); V t102= allocNode(); V t103= allocNode();
    V t104= allocNode(); V t105= allocNode(); V t106= allocNode(); V t107= allocNode();
    V p0  = allocNode(); V t9  = allocNode();
    V t28 = allocNode(); V t30 = allocNode(); V t31 = allocNode();
    L x0[] = {49,14,23,45,19,3,36,9};
    V x1[] = {t27,t20,t29};
    V x2[] = {w1,w2};
    V x3[] = {p0,p1};
    V x4[] = {t84 ,t85 ,t86};
    V x5[] = {u5 ,u6 ,u7 ,u8};
    B x6[] = {0,1,1,1};
    S x7[] = {"p_brand","p_type","p_size","supplier_cnt"};
    V x8[] = {t101 ,t102 ,t103 ,t104};
    tic;
    PROFILE(  0, t0 , pfnLoadTable(t0, initLiteralSym((S)"supplier")));
    PROFILE(  1, t1 , pfnColumnValue(t1, t0, initLiteralSym((S)"s_suppkey")));
    PROFILE(  2, t7 , pfnColumnValue(t7, t0, initLiteralSym((S)"s_comment")));
    PROFILE(  3, t8 , pfnLike(t8, t7, initLiteralString((S)"%Customer%Complaints%")));
    PROFILE( 99, t9 , pfnCompress(t9, t8, t1));  //
    PROFILE(  4, t16, pfnLoadTable(t16, initLiteralSym((S)"part")));
    PROFILE(  5, t17, pfnColumnValue(t17, t16, initLiteralSym((S)"p_partkey")));
    PROFILE(  6, t20, pfnColumnValue(t20, t16, initLiteralSym((S)"p_brand")));
    PROFILE(  7, t21, pfnColumnValue(t21, t16, initLiteralSym((S)"p_type")));
    PROFILE(  8, t22, pfnColumnValue(t22, t16, initLiteralSym((S)"p_size")));
    PROFILE(  9, t26, copyV(t26, initLiteralI64Vector(8, x0)));
    PROFILE( 10, t27, pfnMember(t27, t26, t22));
    PROFILE( 11, t29, pfnLike(t29, t21, initLiteralString((S)"MEDIUM POLISHED%")));
    if(OPT_LF){
        PROFILE( 12, t32, q16_loopfusion_0(t32,x1));
    }
    else {
        PROFILE( 12, t28, pfnNeq(t28, t20, initLiteralSym((S)"Brand#45")));
        PROFILE( 14, t30, pfnNot(t30, t29));
        PROFILE( 15, t31, pfnAnd(t31, t27, t28));
        PROFILE( 16, t32, pfnAnd(t32, t31, t30));
    }
    PROFILE( 13, t42, pfnLoadTable(t42, initLiteralSym((S)"partsupp")));
    PROFILE( 14, t43, pfnColumnValue(t43, t42, initLiteralSym((S)"ps_partkey")));
    PROFILE( 15, t44, pfnColumnValue(t44, t42, initLiteralSym((S)"ps_suppkey")));
    PROFILE( 16, k0 , pfnValues(k0, t43));
    PROFILE( 17, k1 , pfnIndex(k1, t32, k0));
    PROFILE( 99, p0 , pfnCompress(p0, k1, k0)); //
    PROFILE( 18, p1 , pfnWhere(p1, k1));
    PROFILE( 19, t64, pfnIndex(t64, t44, p1));
    PROFILE( 20, t68, pfnMember(t68, t9, t64));
    PROFILE( 21, w0 , pfnNot(w0, t68));
    if(OPT_PH){
        PROFILE( 22, w2 , q16_peephole_0(x2,w0,x3));
    }
    else {
        PROFILE( 27, w1 , pfnCompress(w1, w0, p0));
        PROFILE( 28, w2 , pfnCompress(w2, w0, p1));
    }
    PROFILE( 23, t84, pfnIndex(t84, t20, w1));
    PROFILE( 24, t85, pfnIndex(t85, t21, w1));
    PROFILE( 25, t86, pfnIndex(t86, t22, w1));
    PROFILE( 26, t87, pfnIndex(t87, t44, w2));
    PROFILE( 27, t95, pfnList(t95, 3, x4));
    PROFILE( 28, t96, pfnGroup(t96, t95));
    PROFILE( 29, u0 , pfnValues(u0, t96));
    PROFILE( 30, u1 , pfnKeys(u1, t96));
    if(OPT_PH){
        PROFILE( 99, u5 , q16_peephole_1(u5, t87, u0)); // <--
    }
    else {
        PROFILE( 31, u2 , pfnEachRight(u2,t87,u0,pfnIndex));
        PROFILE( 32, u3 , pfnEach(u3,u2,pfnUnique));
        PROFILE( 33, u4 , pfnEach(u4,u3,pfnLen));
        PROFILE( 34, u5 , pfnRaze(u5, u4));
    }
    PROFILE( 35, u6 , pfnIndex(u6, t84, u1));
    PROFILE( 36, u7 , pfnIndex(u7, t85, u1));
    PROFILE( 37, u8 , pfnIndex(u8, t86, u1));
    PROFILE( 38, t99, pfnList(t99, 4, x5));
    PROFILE( 39, t100, pfnOrderBy(t100, t99, initLiteralBoolVector(4, x6)));
    PROFILE( 40, t101, pfnIndex(t101, u6, t100));
    PROFILE( 41, t102, pfnIndex(t102, u7, t100));
    PROFILE( 42, t103, pfnIndex(t103, u8, t100));
    PROFILE( 43, t104, pfnIndex(t104, u5, t100));
    PROFILE( 44, t105, copyV(t105, initLiteralSymVector(4, x7)));
    PROFILE( 45, t106, pfnList(t106, 4, x8));
    PROFILE( 46, t107, pfnTable(t107, t105, t106));
    toc;
    printTablePretty(t107, 10);
    R elapsed;
}
