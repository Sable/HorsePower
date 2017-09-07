
void testOrderBy(){
    P("Testing order by\n");
    V a = allocNode();
    initV(a,H_G,2);
    L value_A[] = {32,12,7,78,12,45};
    L value_B[] = {56,34,32,45,78,23};
    fillItem(vV(a,0),value_A,6);
    fillItem(vV(a,1),value_B,6);
    P("a = "); printList(a); P("\n");
    V b = allocNode();
    initV(b,H_B,2);
    vB(b,0) = true;  //ASC
    vB(b,1) = false; //DESC
    P("b = "); printList(b); P("\n");
    V z = allocNode();
    pfnOrderBy(z,a,b);
    printList(z);
}

L testGroupBy(){
    P("\nTest group by:\n");
    V a = allocNode();
    V b = allocNode();
    V c = allocNode();
    V z = allocNode();
    L ta[] = {32,12, 7,78,12,45};
    L tb[] = {56,34,32,45,78,23};
    initV(a, H_E, 6); DOI(6, vE(a,i)=ta[i])
    initV(b, H_E, 6); DOI(6, vE(b,i)=tb[i])
    V rn[] = {a,b};
    pfnList(c,2,rn);
    P("input a: \n"); printV(a);
    L e = pfnGroup(z,a);
    if(e!=0) printErrMsg(e);
    else printV(z);
    P("input c: \n"); printV(c);
    e = pfnGroup(z,c);
    if(e!=0) printErrMsg(e);
    else printV(z);
    R 0;
}

L testMember(){
	P("\nTest member:\n");
    V a = allocNode();
    V b = allocNode();
    V z = allocNode();
    L ta[] = {-2,1,3,72,25};
    L tb[] = {72,72,99};
    initV(a, H_E, 5); DOI(5, vE(a,i)=ta[i])
    initV(b, H_E, 3); DOI(3, vE(b,i)=tb[i])
    printV(a);
    printV(b);
    L e = pfnMember(z,a,b); /* pfnIndexOf */
    if(e!=0) printErrMsg(e);
    else printV(z);
    R 0;
}

L testSpecial(){
    P("\nTest special:\n");
    V a = allocNode();
    V b = allocNode();
    pfnPlus(a,literalF64(0.06),literalF64(0.01)); 
    pfnLeq(b,a,literalF64(0.07));
    printV(b);
    pfnEq(b,a,literalF64(0.07));
    printV(b);
    pfnGeq(b,a,literalF64(0.07));
    printV(b);
    R 0;
}

L testCompare(){
    P("\nTest < <= == >= >\n");
    V a = allocNode();
    V b = allocNode();
    V z = allocNode();
    L ta[] = {-2,1,3,72,25};
    L tb[] = {20,20,20,20,20};
    initV(a, H_E, 5); DOI(5, vE(a,i)=ta[i])
    initV(b, H_E, 5); DOI(5, vE(b,i)=tb[i])
    printV(a);
    printV(b);
    L e = pfnLt(z,a,b); /* less than */
    P("e = %lld\n",e);
    printV(z);
    R e;
}

L testSum(){
    P("\nTest sum\n");
    V a = allocNode();
    V z = allocNode();
    L ta[] = {-2,1,3,72,25};
    initV(a, H_L, 5); DOI(5, vL(a,i)=ta[i])
    printV(a);
    L e = pfnSum(z,a); /* less than */
    P("e = %lld\n",e);
    printV(z);
    R e;
}

L testPfns(){
    // testMember();
    // testGroupBy();
    // testSpecial();
    // testCompare();
    testSum();
	R 0;
}
