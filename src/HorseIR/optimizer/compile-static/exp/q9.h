/*==== FE: Loop fusion with elementwise functions ====*/
/* num_func = 4, targ = t226 */
L q9_loopfusion_0(V z, V *x){
    // z -> t226
    V x0 = x[0]; // t201
    V x1 = x[1]; // t202
    V x2 = x[2]; // t194
    V x3 = x[3]; // t200
    initV(z,H_E,vn(x0));
    DOP(vn(x0), vE(z,i)=MINUS(MUL(vE(x0,i),MINUS(1,vE(x1,i))),MUL(vE(x2,i),vE(x3,i)))) R 0;
}
/*==== FP: Loop fusion with patterns ====*/
L q9_peephole_0(V *z, V *x, V y){
    L r0 = vn(y);// y -> t230
    V z0 = z[0]; // t235
    V x0 = x[0]; // t226
    initV(z0, H_E, r0);
    DOJ(r0, {V t=vV(y,j);  E c0=0; \
        DOP(vn(t), {L k=vL(t,i); c0+=vE(x0,k);},\
        reduction(+:c0)) \
        vE(z0,j)=c0; \
    })
    R 0;
}
// Pattern FP3
// Pattern FP4
// Pattern FP5
E compiled_main(){
    E elapsed=0;
    V t0   = allocNode(); V t2   = allocNode(); V t5   = allocNode(); V t6   = allocNode(); 
    V t9   = allocNode(); V t13  = allocNode(); V t15  = allocNode(); V t19  = allocNode(); 
    V t20  = allocNode(); V t21  = allocNode(); V t29  = allocNode(); V t39  = allocNode(); 
    V t40  = allocNode(); V t41  = allocNode(); V t43  = allocNode(); V t45  = allocNode(); 
    V t46  = allocNode(); V t47  = allocNode(); V t48  = allocNode(); V t49  = allocNode(); 
    V t58  = allocNode(); V t59  = allocNode(); V t60  = allocNode(); V t61  = allocNode(); 
    V t63  = allocNode(); V t65  = allocNode(); V t66  = allocNode(); V t67  = allocNode(); 
    V t69  = allocNode(); V t72  = allocNode(); V t80  = allocNode(); V t89  = allocNode(); 
    V t90  = allocNode(); V t92  = allocNode(); V t94  = allocNode(); V t95  = allocNode(); 
    V t96  = allocNode(); V t97  = allocNode(); V t99  = allocNode(); V t100 = allocNode(); 
    V t101 = allocNode(); V t111 = allocNode(); V t112 = allocNode(); V t113 = allocNode(); 
    V t114 = allocNode(); V t115 = allocNode(); V t117 = allocNode(); V t139 = allocNode(); 
    V t141 = allocNode(); V t142 = allocNode(); V t146 = allocNode(); V t147 = allocNode(); 
    V t148 = allocNode(); V t158 = allocNode(); V t159 = allocNode(); V t163 = allocNode(); 
    V t168 = allocNode(); V t169 = allocNode(); V t170 = allocNode(); V t172 = allocNode(); 
    V t194 = allocNode(); V t200 = allocNode(); V t201 = allocNode(); V t202 = allocNode(); 
    V t217 = allocNode(); V t222 = allocNode(); V t223 = allocNode(); V t224 = allocNode(); 
    V t225 = allocNode(); V t226 = allocNode(); V t227 = allocNode(); V t228 = allocNode(); 
    V t229 = allocNode(); V t230 = allocNode(); V t231 = allocNode(); V t232 = allocNode(); 
    V t233 = allocNode(); V t234 = allocNode(); V t235 = allocNode(); V t236 = allocNode(); 
    V t237 = allocNode(); V t238 = allocNode(); V t239 = allocNode(); V t240 = allocNode(); 
    V t241 = allocNode(); V t242 = allocNode(); V t243 = allocNode(); 
    tic;
    PROFILE(  0, t0 , pfnLoadTable(t0, initLiteralSym((S)"nation")));
    PROFILE(  1, t2 , pfnColumnValue(t2, t0, initLiteralSym((S)"n_name")));
    PROFILE(  2, t5 , pfnLoadTable(t5, initLiteralSym((S)"supplier")));
    PROFILE(  3, t6 , pfnColumnValue(t6, t5, initLiteralSym((S)"s_suppkey")));
    PROFILE(  4, t9 , pfnColumnValue(t9, t5, initLiteralSym((S)"s_nationkey")));
    PROFILE(  5, t13, pfnValues(t13, t9));
    PROFILE(  6, t15, pfnIndex(t15, t2, t13));
    PROFILE(  7, t19, pfnLoadTable(t19, initLiteralSym((S)"part")));
    PROFILE(  8, t20, pfnColumnValue(t20, t19, initLiteralSym((S)"p_partkey")));
    PROFILE(  9, t21, pfnColumnValue(t21, t19, initLiteralSym((S)"p_name")));
    PROFILE( 10, t29, pfnLike(t29, t21, initLiteralSym((S)"%green%")));
    PROFILE( 11, t39, pfnLoadTable(t39, initLiteralSym((S)"partsupp")));
    PROFILE( 12, t40, pfnColumnValue(t40, t39, initLiteralSym((S)"ps_partkey")));
    PROFILE( 13, t41, pfnColumnValue(t41, t39, initLiteralSym((S)"ps_suppkey")));
    PROFILE( 14, t43, pfnColumnValue(t43, t39, initLiteralSym((S)"ps_supplycost")));
    PROFILE( 15, t45, pfnValues(t45, t40));
    PROFILE( 16, t46, pfnIndex(t46, t29, t45));
    PROFILE( 17, t47, pfnCompress(t47, t46, t45));
    PROFILE( 18, t48, pfnWhere(t48, t46));
    PROFILE( 19, t49, pfnIndex(t49, t20, t47));
    PROFILE( 20, t58, pfnFetch(t58, t40));
    PROFILE( 21, t59, pfnIndex(t59, t58, t48));
    PROFILE( 22, t60, pfnFetch(t60, t41));
    PROFILE( 23, t61, pfnIndex(t61, t60, t48));
    PROFILE( 24, t63, pfnIndex(t63, t43, t48));
    PROFILE( 25, t65, pfnJoinIndex(t65,t6,t61,initLiteralSym((S)"eq")));
    PROFILE( 26, t66, pfnIndex(t66, t65, initLiteralI64(0)));
    PROFILE( 27, t67, pfnIndex(t67, t65, initLiteralI64(1)));
    PROFILE( 28, t69, pfnIndex(t69, t15, t66));
    PROFILE( 29, t72, pfnIndex(t72, t6, t66));
    PROFILE( 30, t80, pfnIndex(t80, t49, t67));
    PROFILE( 31, t89, pfnIndex(t89, t59, t67));
    PROFILE( 32, t90, pfnIndex(t90, t61, t67));
    PROFILE( 33, t92, pfnIndex(t92, t63, t67));
    PROFILE( 34, t94, pfnLoadTable(t94, initLiteralSym((S)"lineitem")));
    PROFILE( 35, t95, pfnColumnValue(t95, t94, initLiteralSym((S)"l_orderkey")));
    PROFILE( 36, t96, pfnColumnValue(t96, t94, initLiteralSym((S)"l_partkey")));
    PROFILE( 37, t97, pfnColumnValue(t97, t94, initLiteralSym((S)"l_suppkey")));
    PROFILE( 38, t99, pfnColumnValue(t99, t94, initLiteralSym((S)"l_quantity")));
    PROFILE( 39, t100, pfnColumnValue(t100, t94, initLiteralSym((S)"l_extendedprice")));
    PROFILE( 40, t101, pfnColumnValue(t101, t94, initLiteralSym((S)"l_discount")));
    //getInfoVar(t90); getInfoVar(t97); getchar();
    // Variable t90 has type H_L and len 42656
    // Variable t97 has type H_L and len 6001215
    PROFILE( 41, t111, pfnList(t111, 4, (V []){t90 ,t89 ,t80 ,t72}));
    PROFILE( 42, t112, pfnList(t112, 4, (V []){t97 ,t96 ,t96 ,t97}));
    //getInfoVar(t111); getInfoVar(t112); getchar();
    // Variable t111 has type H_G and len 4
    // Variable t112 has type H_G and len 4
    PROFILE( 43, t113, pfnJoinIndex(t113,t111,t112,initLiteralSym((S)"eq")));
    PROFILE( 44, t114, pfnIndex(t114, t113, initLiteralI64(0)));
    PROFILE( 45, t115, pfnIndex(t115, t113, initLiteralI64(1)));
    PROFILE( 46, t117, pfnIndex(t117, t69, t114));
    PROFILE( 47, t139, pfnIndex(t139, t92, t114));
    PROFILE( 48, t141, pfnFetch(t141, t95));
    PROFILE( 49, t142, pfnIndex(t142, t141, t115));
    PROFILE( 50, t146, pfnIndex(t146, t99, t115));
    PROFILE( 51, t147, pfnIndex(t147, t100, t115));
    PROFILE( 52, t148, pfnIndex(t148, t101, t115));
    PROFILE( 53, t158, pfnLoadTable(t158, initLiteralSym((S)"orders")));
    PROFILE( 54, t159, pfnColumnValue(t159, t158, initLiteralSym((S)"o_orderkey")));
    PROFILE( 55, t163, pfnColumnValue(t163, t158, initLiteralSym((S)"o_orderdate")));
    //getInfoVar(t159); getInfoVar(t142); getchar();
    // Variable t159 has type H_L and len 1500000
    // Variable t142 has type H_L and len 319404
    //PROFILE( 56, t168, pfnJoinIndex(t168,t159,t142,initLiteralSym((S)"eq")));
    //PROFILE( 57, t169, pfnIndex(t169, t168, initLiteralI64(0)));
    //PROFILE( 58, t170, pfnIndex(t170, t168, initLiteralI64(1)));
    PROFILE( 56, t168, pfnJoinIndex(t168,t142,t159,initLiteralSym((S)"eq")));
    PROFILE( 57, t169, pfnIndex(t169, t168, initLiteralI64(1)));
    PROFILE( 58, t170, pfnIndex(t170, t168, initLiteralI64(0)));
    PROFILE( 59, t172, pfnIndex(t172, t117, t170));
    PROFILE( 60, t194, pfnIndex(t194, t139, t170));
    PROFILE( 61, t200, pfnIndex(t200, t146, t170));
    PROFILE( 62, t201, pfnIndex(t201, t147, t170));
    PROFILE( 63, t202, pfnIndex(t202, t148, t170));
    PROFILE( 64, t217, pfnIndex(t217, t163, t169));
    PROFILE( 65, t222, pfnDateYear(t222, t217));
    PROFILE( 66, t226, q9_loopfusion_0(t226,(V []){t201,t202,t194,t200}));
    PROFILE( 67, t227, pfnList(t227, 2, (V []){t172 ,t222}));
    PROFILE( 68, t228, pfnGroup(t228, t227));
    PROFILE( 69, t229, pfnKeys(t229, t228));
    PROFILE( 70, t230, pfnValues(t230, t228));
    PROFILE( 71, t231, pfnIndex(t231, t172, t229));
    PROFILE( 72, t232, pfnIndex(t232, t222, t229));
    PROFILE( 73, t235, q9_peephole_0((V []){t235},(V []){t226},t230));
    PROFILE( 74, t236, pfnList(t236, 2, (V []){t231 ,t232}));
    PROFILE( 75, t237, pfnOrderBy(t237, t236, initLiteralBoolVector(2, (B []){1,0})));
    PROFILE( 76, t238, pfnIndex(t238, t231, t237));
    PROFILE( 77, t239, pfnIndex(t239, t232, t237));
    PROFILE( 78, t240, pfnIndex(t240, t235, t237));
    PROFILE( 79, t241, copyV(t241, initLiteralSymVector(3, (S []){"nation","o_year","sum_profit"})));
    PROFILE( 80, t242, pfnList(t242, 3, (V []){t238 ,t239 ,t240}));
    PROFILE( 81, t243, pfnTable(t243, t241, t242));
    toc;
    printV2(t243,20);
    R elapsed;
}
