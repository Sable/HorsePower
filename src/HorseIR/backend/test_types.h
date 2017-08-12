
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
	testG();
	testN();
	testA(tableEmp);
	testA(tableDep);
	testK(tableEmp, tableDep);
}
