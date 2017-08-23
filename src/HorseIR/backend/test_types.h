
void testB(){
    P("\nTest bool:\n");
    V a = allocNode();
    initV(a, H_B, 1);
    vb(a) = 0;  
    printV(a);
    V b = allocNode();
    initV(a, H_B, 5);
    B t[] = {0,1,1,0,1};
    DOI(5,vB(a,i)=t[i])
    printV(a);
}

void testH(){
    P("\nTest short:\n");
    V a = allocNode();
    initV(a, H_H, 1);
    vh(a) = 99;
    printV(a);
    V b = allocNode();
    initV(a, H_H, 5);
    H t[] = {-2,1,3,72,25};
    DOI(5,vH(a,i)=t[i])
    printV(a);
}

void testI(){
    P("\nTest int:\n");
    V a = allocNode();
    initV(a, H_I, 1);
    vi(a) = -2;
    printV(a);
    vi(a) = 99;
    printV(a);
    vi(a) = 0;
    printV(a);
}

void testL(){
    P("\nTest long:\n");
    V a = allocNode();
    initV(a, H_L, 1);
    vl(a) = -2;
    printV(a);
    vl(a) = 99;
    printV(a);
    vl(a) = 0;
    printV(a);
}

void testX(){
    P("\nTest complex:\n");
    V a = allocNode();
    initV(a, H_X, 1);
    xCopy(vx(a),2.3,-0.5);
    printV(a);
    xCopy(vx(a),-2.5,0);
    printV(a);
    xCopy(vx(a),2.3,0.5);
    printV(a);
}

void testZ(){
    P("\nTest datetime:\n");
    V a = allocNode();
    initV(a, H_Z, 1);
    L d = 20170823LL;
    L t[] = {20,2,34,001};
    L v = (t[0]*3600+t[1]*60+t[2])*1000+t[3];
    vz(a) = (d*216000000LL+v);
    /* output: 2017.08.23T20:02:34.001:z */
    printV(a);
}

void testS(){
    P("\nTest string:\n");
    V a = allocNode();
    initV(a, H_S, 4);
    vS(a,0)=insertString((S)"Have");
    vS(a,1)=insertString((S)"a");
    vS(a,2)=insertString((S)"good");
    vS(a,3)=insertString((S)"day.");
    printV(a);
}

void testG(){
    P("\nTest list:\n");
    V a = allocNode();
    initV(a, H_L, 5);
    L t[] = {-2,1,3,72,25};
    DOI(5, vL(a,i)=t[i])
    V listA = allocNode();
    pfnTolist(listA,a);
    printV(listA);
}

void testY(){
    P("\nTest enum:\n");
    V a = allocNode();
    V b = allocNode();
    L ta[] = {-2,1,3,72,25};
    L tb[] = {72,72,1};
    initV(a, H_L, 5); DOI(5, vL(a,i)=ta[i])
    initV(b, H_L, 3); DOI(3, vL(b,i)=tb[i])
    V z = allocNode();
    pfnEnum(z,a,b);
    printV(z);
}

void testN(){
    P("\nTest dict:\n");
    V a = allocNode();
    V b = allocNode();
    initV(a, H_L, 1); vl(a) = -2;
    initV(b, H_Q, 1); vq(b) =  3;
    V listA = allocNode();
    V listB = allocNode();
    V z     = allocNode();
    pfnTolist(listA,a);
    pfnTolist(listB,b);
    pfnDict(z,listA,listB);
    printV(z);
}

void testA(V t1){
    P("\nTest table:\n");
    printV(t1);
}

void testK(V t1, V t2){
    P("\nTest ktable:\n");
    V z = allocNode();
    pfnKTable(z,t1,t2);
    printV(z);
}


void testTypes(){
    P("Reading table Employee\n");
    V tableEmp = readCSV(CSV_EMP, NUM_COL_EMP, TYPE_EMP, SYM_LIST_EMP);
    registerTable((S)"Employee", tableEmp);
    P("Reading table Department\n");
    V tableDep = readCSV(CSV_DEP, NUM_COL_DEP, TYPE_DEP, SYM_LIST_DEP);
    registerTable((S)"Department", tableDep);
    testB();
    testH();
    testI();
    testL();
    testX();
    testS();
    // testG();
    testY();
    testN();
    testA(tableEmp);
    testA(tableDep);
    testK(tableEmp, tableDep);
    testZ();
}

