
L simulateQ6(){
    L e;
    V t0 = allocNode();  V t5 = allocNode();  V t10 = allocNode();  V t15 = allocNode();
    V t1 = allocNode();  V t6 = allocNode();  V t11 = allocNode();  V t16 = allocNode();
    V t2 = allocNode();  V t7 = allocNode();  V t12 = allocNode();  V t17 = allocNode();
    V t3 = allocNode();  V t8 = allocNode();  V t13 = allocNode();  V t18 = allocNode();
    V t4 = allocNode();  V t9 = allocNode();  V t14 = allocNode();  V t19 = allocNode();

    V t20 = allocNode(); V t21 = allocNode(); V t22 = allocNode();  V a0 = allocNode();
    struct timeval tv0, tv1;
    gettimeofday(&tv0, NULL);
    e = pfnLoadTable(a0, \
          initSymbol(allocNode(),getSymbol((S)"lineitem")));          CHECK(e,1);

    e = pfnColumnValue(t0, \
          a0, \
          initSymbol(allocNode(),getSymbol((S)"l_extendedprice")));   CHECK(e,3);
    e = pfnColumnValue(t1, \
          a0, \
          initSymbol(allocNode(),getSymbol((S)"l_discount")));        CHECK(e,4);
    e = pfnColumnValue(t2, \
          a0, \
          initSymbol(allocNode(),getSymbol((S)"l_shipdate")));        CHECK(e,5);
    e = pfnColumnValue(t3, \
          a0, \
          initSymbol(allocNode(),getSymbol((S)"l_quantity")));        CHECK(e,6);

    e = pfnGeq(t4, t2, literalDate(19940101));                        CHECK(e,7);
    e = pfnDatetimeAdd(t5,\
          literalDate(19940101),literalI64(1),literalSym((S)"year")); CHECK(e,8);
    e = pfnLt(t6,t2,t5);                                              CHECK(e,9);
    e = pfnMinus(t7,literalF64(0.06),literalF64(0.01));               CHECK(e,10);
    e = pfnPlus(t8,literalF64(0.06),literalF64(0.01));                CHECK(e,11);

    e = pfnGeq(t9,t1,t7);                                             CHECK(e,12);
    e = pfnLeq(t10,t1,t8);                                            CHECK(e,13);
    e = pfnAnd(t11,t9,t10);                                           CHECK(e,14);
    e = pfnLt(t12,t3,literalI64(24));                                 CHECK(e,15);

    e = pfnAnd(t13,t4,t6);                                            CHECK(e,16);
    e = pfnAnd(t14,t13,t11);                                           CHECK(e,17);
    e = pfnAnd(t15,t14,t12);                                          CHECK(e,18);

    e = pfnCompress(t16,t15,t0);                                      CHECK(e,19);
    e = pfnCompress(t17,t15,t1);                                      CHECK(e,20);
    e = pfnMul(t18,t16,t17);                                          CHECK(e,21);
    e = pfnSum(t19,t18);                                              CHECK(e,22);

    e = pfnTolist(t20,literalSym((S)"revenue"));                      CHECK(e,23);
    e = pfnEnlist(t21,t19);                                           CHECK(e,24);
    e = pfnTable(t22,t20,t21);                                        CHECK(e,25);
    gettimeofday(&tv1, NULL);

    P("Result of the Query 6: (elapsed time %g ms)\n\n", calcInterval(tv0,tv1)/1000.0);
    printV(t22);
    R 0;
}

L testTPCHQ6(){
    P("** Start simulation for TPC-H Query 6\n");
    initTableByName((S)"lineitem");
    simulateQ6();
    P("** End Query 6\n");
    R 0;
}

