
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

L testPfns(){
	testMember();
    testGroupBy();
	R 0;
}
