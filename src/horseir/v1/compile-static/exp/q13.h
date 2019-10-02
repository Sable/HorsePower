#define USE_Q13_V2

/*==== FE: Loop fusion with elementwise functions ====*/
/*==== FP: Loop fusion with patterns ====*/
// Pattern FP3
// Pattern FP4
// Pattern FP5
#ifdef USE_Q13_V1
E compiled_main(){
    printBanner("q13 version 1");
    E elapsed=0;
    V t0   = allocNode(); V t1   = allocNode(); V t9   = allocNode(); V t11  = allocNode(); 
    V t18  = allocNode(); V t19  = allocNode(); V t20  = allocNode(); V t22  = allocNode(); 
    V t30  = allocNode(); V t31  = allocNode(); V t32  = allocNode(); V t33  = allocNode(); 
    V t34  = allocNode(); V t35  = allocNode(); V t36  = allocNode(); V t37  = allocNode(); 
    V t38  = allocNode(); V t39  = allocNode(); V t40  = allocNode(); V t41  = allocNode(); 
    V t42  = allocNode(); V t43  = allocNode(); V t44  = allocNode(); V t45  = allocNode(); 
    V t46  = allocNode(); V t47  = allocNode(); V t51  = allocNode(); V t52  = allocNode(); 
    V t53  = allocNode(); V t54  = allocNode(); V t55  = allocNode(); V t56  = allocNode(); 
    V t57  = allocNode(); V t58  = allocNode(); V t59  = allocNode(); V t60  = allocNode(); 
    V t61  = allocNode(); V t62  = allocNode(); V t63  = allocNode(); V t64  = allocNode(); 
    V t65  = allocNode(); V t66  = allocNode(); V t67  = allocNode(); V t68  = allocNode(); 
    tic;
    PROFILE(  0, t0 , pfnLoadTable(t0, initLiteralSym((S)"customer")));
    PROFILE(  1, t1 , pfnColumnValue(t1, t0, initLiteralSym((S)"c_custkey")));
    PROFILE(  2, t9 , pfnLoadTable(t9, initLiteralSym((S)"orders")));
    PROFILE(  3, t11, pfnColumnValue(t11, t9, initLiteralSym((S)"o_custkey")));
    PROFILE(  4, t18, pfnColumnValue(t18, t9, initLiteralSym((S)"o_comment")));
    PROFILE(  5, t19, pfnLike(t19, t18, initLiteralSym((S)"%special%requests%")));
    // 16082 / 1500000
    //L tot=0; DOI(vn(t19), tot+=vB(t19,i)) P("pfnLike result: %lld / %lld\n",tot,vn(t19)); getchar();
    //getchar(); // stop check
    profileV(t19); getchar();
    PROFILE(  6, t20, pfnNot(t20, t19));
    PROFILE(  7, t22, pfnCompress(t22, t20, t11));
    PROFILE(  8, t30, pfnFetch(t30, t22));
    PROFILE(  9, t31, pfnGroup(t31, t1));
    PROFILE( 10, t32, pfnKeys(t32, t31));
    PROFILE( 11, t33, pfnValues(t33, t31));
    PROFILE( 12, t34, pfnEach(t34,t33,pfnLen));
    PROFILE( 13, t35, pfnRaze(t35, t34));
    PROFILE( 14, t36, pfnIndex(t36, t1, t32));
    PROFILE( 15, t37, pfnGroup(t37, t30));
    PROFILE( 16, t38, pfnKeys(t38, t37));
    PROFILE( 17, t39, pfnValues(t39, t37));
    PROFILE( 18, t40, pfnEach(t40,t39,pfnLen));
    PROFILE( 19, t41, pfnRaze(t41, t40));
    PROFILE( 20, t42, pfnIndex(t42, t30, t38));
    PROFILE( 21, t43, pfnJoinIndex(t43,t36,t42,initLiteralSym((S)"eq")));
    PROFILE( 22, t44, pfnIndex(t44, t43, initLiteralI64(0)));
    PROFILE( 23, t45, pfnIndex(t45, t43, initLiteralI64(1)));
    PROFILE( 24, t46, pfnIndex(t46, t35, t44));
    PROFILE( 25, t47, pfnIndex(t47, t41, t45));
    PROFILE( 26, t51, pfnMul(t51, t46, t47));
    PROFILE( 27, t52, pfnLen(t52, t32));
    PROFILE( 28, t53, pfnVector(t53, t52, initLiteralI64(0)));
    //PROFILE( 29, t54, pfnIndexA(t54, t44, t51));
    PROFILE( 29, t54, pfnIndexA(t54, t53, t44, t51));
    //printV2(t54, 20); getchar();
    PROFILE( 30, t55, pfnList(t55, 1, (V []){t54}));
    PROFILE( 31, t56, pfnGroup(t56, t55));
    PROFILE( 32, t57, pfnKeys(t57, t56));
    PROFILE( 33, t58, pfnValues(t58, t56));
    PROFILE( 34, t59, pfnIndex(t59, t54, t57));
    PROFILE( 35, t60, pfnEach(t60,t58,pfnLen));
    PROFILE( 36, t61, pfnRaze(t61, t60));
    //printV2(t61, 20); printV2(t59, 20); getchar();
    PROFILE( 37, t62, pfnList(t62, 2, (V []){t61 ,t59}));
    PROFILE( 38, t63, pfnOrderBy(t63, t62, initLiteralBoolVector(2, (B []){0,0})));
    PROFILE( 39, t64, pfnIndex(t64, t59, t63));
    PROFILE( 40, t65, pfnIndex(t65, t61, t63));
    PROFILE( 41, t66, copyV(t66, initLiteralSymVector(2, (S []){"c_count","custdist"})));
    PROFILE( 42, t67, pfnList(t67, 2, (V []){t64 ,t65}));
    PROFILE( 43, t68, pfnTable(t68, t66, t67));
    toc;
    printV2(t68,20);
    R elapsed;
}
#endif

#ifdef USE_Q13_V2
E compiled_main(){
    printBanner("q13 version 2");
    E elapsed=0;
    V t0   = allocNode(); V t1   = allocNode(); V t9   = allocNode(); V t11  = allocNode(); 
    V t18  = allocNode(); V t19  = allocNode(); V t20  = allocNode(); V t22  = allocNode(); 
    V t30  = allocNode(); V t31  = allocNode(); V t32  = allocNode(); V t33  = allocNode(); 
    V t34  = allocNode(); V t35  = allocNode(); V t36  = allocNode(); V t37  = allocNode(); 
    V t38  = allocNode(); V t39  = allocNode(); V t40  = allocNode(); V t41  = allocNode(); 
    V t42  = allocNode(); V t43  = allocNode(); V t44  = allocNode(); V t45  = allocNode(); 
    V t46  = allocNode(); V t47  = allocNode(); V t51  = allocNode(); V t52  = allocNode(); 
    V t53  = allocNode(); V t54  = allocNode(); V t55  = allocNode(); V t56  = allocNode(); 
    V t57  = allocNode(); V t58  = allocNode(); V t59  = allocNode(); V t60  = allocNode(); 
    V t61  = allocNode(); V t62  = allocNode(); V t63  = allocNode(); V t64  = allocNode(); 
    V t65  = allocNode(); V t66  = allocNode(); V t67  = allocNode(); V t68  = allocNode(); 
    tic;
    PROFILE(  0, t0 , pfnLoadTable(t0, initLiteralSym((S)"customer")));
    PROFILE(  1, t1 , pfnColumnValue(t1, t0, initLiteralSym((S)"c_custkey")));    // I
    PROFILE(  2, t9 , pfnLoadTable(t9, initLiteralSym((S)"orders")));
    PROFILE(  3, t11, pfnColumnValue(t11, t9, initLiteralSym((S)"o_custkey")));   // I
    PROFILE(  4, t18, pfnColumnValue(t18, t9, initLiteralSym((S)"o_comment")));   // S
    PROFILE(  5, t19, pfnLike(t19, t18, initLiteralSym((S)"%special%requests%")));
    // 16082 / 1500000
    //L tot=0; DOI(vn(t19), tot+=vB(t19,i)) P("pfnLike result: %lld / %lld\n",tot,vn(t19)); getchar();
    //getchar(); // stop check
    //profileV(t19); getchar();
    PROFILE(  6, t20, pfnNot(t20, t19));
    if(false){
        // maybe cause errors
        PROFILE(  7, t22, pfnCompress(t22, t20, t11)); /* TODO: fix a bug identified as compress/enum in pfncompress */
        PROFILE(  8, t30, pfnFetch(t30, t22));
    }
    else {
        // fetch before compress
        PROFILE(  7, t22, pfnFetch(t22, t11));
        PROFILE(  8, t22, pfnCompress(t30, t20, t22));
    }
    //getInfoVar(t1); getchar();
    PROFILE(  9, t31, pfnGroup(t31, t1));
    PROFILE( 10, t32, pfnKeys(t32, t31));
    PROFILE( 11, t33, pfnValues(t33, t31));
    PROFILE( 12, t34, pfnEach(t34,t33,pfnLen));
    PROFILE( 13, t35, pfnRaze(t35, t34));
    PROFILE( 14, t36, pfnIndex(t36, t1, t32));
    //L k0 = 0; DOI(vn(t30), if(vI(t30,i)==148561){k0++;}) P("k0 = %lld\n",k0); getchar(); // 28 expected
    //getInfoVar(t30); getchar();
    PROFILE( 15, t37, pfnGroup(t37, t30));
    PROFILE( 16, t38, pfnKeys(t38, t37));
    PROFILE( 17, t39, pfnValues(t39, t37));
    //getInfoVar(t38); getchar();
    PROFILE( 18, t40, pfnEach(t40,t39,pfnLen));
    PROFILE( 19, t41, pfnRaze(t41, t40));
    PROFILE( 20, t42, pfnIndex(t42, t30, t38));
    PROFILE( 21, t43, pfnJoinIndex(t43,t36,t42,initLiteralSym((S)"eq")));
    PROFILE( 22, t44, pfnIndex(t44, t43, initLiteralI64(0)));
    PROFILE( 23, t45, pfnIndex(t45, t43, initLiteralI64(1)));
    //L kk = -1; DOI(vn(t44), if(vI(t36,vL(t44,i))==148561){kk=i;break;})
    //if(kk==-1)P("not found\n"); else P("kk = %lld\n", kk); getchar();
    //getInfoVar(t44); getchar();
    PROFILE( 24, t46, pfnIndex(t46, t35, t44));
    PROFILE( 25, t47, pfnIndex(t47, t41, t45));
    PROFILE( 26, t51, pfnMul(t51, t46, t47));
    //P("vL(t46,%lld) [%lld] * vL(t47,%lld) [%lld] = %lld\n", kk,vL(t46,kk),kk,vL(t47,kk),vL(t51,kk)); getchar();
    // vL(t46,99036) [1] * vL(t47,99036) [29] = 29 --> expect (28)
    PROFILE( 27, t52, pfnLen(t52, t32));
    PROFILE( 28, t53, pfnVector(t53, t52, initLiteralI64(0)));
    //PROFILE( 29, t54, pfnIndexA(t54, t44, t51));
    //getInfoVar(t53); getInfoVar(t44); getInfoVar(t51); getchar();
    PROFILE( 29, t54, pfnIndexA(t54, t53, t44, t51));
    //getInfoVar(t54);
    //P("vL(t54, %lld) = %lld\n", vL(t44,kk), vL(t54,vL(t44,kk))); getchar();
    // vL(t54, 148560) = 29  --> 28 (expected)
    //getInfoVar(t54); printV2(t54, 20); getchar();
    PROFILE( 30, t55, pfnList(t55, 1, (V []){t54}));
    PROFILE( 31, t56, pfnGroup(t56, t55));
    PROFILE( 32, t57, pfnKeys(t57, t56));
    PROFILE( 33, t58, pfnValues(t58, t56));
    //getInfoVar(t57); getchar(); // expect: 42
    PROFILE( 34, t59, pfnIndex(t59, t54, t57));
    PROFILE( 35, t60, pfnEach(t60,t58,pfnLen));
    PROFILE( 36, t61, pfnRaze(t61, t60));
    //printV2(t61, 20); printV2(t59, 20); getchar();
    PROFILE( 37, t62, pfnList(t62, 2, (V []){t61 ,t59}));
    PROFILE( 38, t63, pfnOrderBy(t63, t62, initLiteralBoolVector(2, (B []){0,0})));
    PROFILE( 39, t64, pfnIndex(t64, t59, t63));
    PROFILE( 40, t65, pfnIndex(t65, t61, t63));
    PROFILE( 41, t66, copyV(t66, initLiteralSymVector(2, (S []){"c_count","custdist"})));
    PROFILE( 42, t67, pfnList(t67, 2, (V []){t64 ,t65}));
    PROFILE( 43, t68, pfnTable(t68, t66, t67));
    toc;
    printV2(t68,20);
    R elapsed;
}
#endif

