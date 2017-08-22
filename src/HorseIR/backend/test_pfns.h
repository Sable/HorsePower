

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
	R 0;
}
