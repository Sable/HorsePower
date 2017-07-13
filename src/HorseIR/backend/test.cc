#include "h_global.h"

C CSV_EMP[] = "data/simple-join/employee.csv";
C CSV_DEP[] = "data/simple-join/department.csv";
L TYPE_EMP[]  = {H_S, H_L};
L TYPE_DEP[]  = {H_L, H_S};
const L NUM_COL_EMP = 2;
const L NUM_COL_DEP = 2;
L SYM_LIST_EMP[2];
L SYM_LIST_DEP[2];

L testInputFile(S filePath){
	if(!filePath){
		fprintf(stderr, "Correct path must be set (-p path)\n");
		exit(99);
	}
	P("path = %s\n", filePath);
	// readFile(filePath, 1);
	R 0;
}

void initTable(){
	const C* PRE_DEFINED[] = {
		"LastName", "DepartmentID",
		"DepartmentID", "DepartmentName"
	};
	DOI(4, insertSym(createSymbol((S)PRE_DEFINED[i])));
	printAllSymol();
	SYM_LIST_EMP[0] = getSymbol((S)"LastName");
	SYM_LIST_EMP[1] = getSymbol((S)"DepartmentID");
	SYM_LIST_DEP[0] = getSymbol((S)"DepartmentID");
	SYM_LIST_DEP[1] = getSymbol((S)"DepartmentName");
}

/* return -1, if too large */
L getNiceNumber(L n){
	L k = 1, c = 62;
	while(c>0 && k<n){c--;k<<=1;}
	R (k<n?-1:k);
}

#define CHECK(e, x) { if(e!=0) { P("Error at line %d, (err=%lld)\n",x,e); exit(99); } P("Pass line %d\n",x); }

L simulateSimple(){
	L e;
	V s0 = allocNode();  V t0 = allocNode();  V r0 = allocNode();  V d0 = allocNode();
	V s1 = allocNode();  V t1 = allocNode();  V r1 = allocNode();  V d1 = allocNode();
	V s2 = allocNode();  V t2 = allocNode();  V r2 = allocNode();  V d2 = allocNode();
	V s3 = allocNode();
	V z0 = allocNode();  V z = allocNode();
	P("** Starting simulation\n");
	e = pfnColumnValue(s0, \
		  initSymbol(allocNode(),getSymbol((S)"Employee")), \
		  initSymbol(allocNode(),getSymbol((S)"LastName")));       CHECK(e,1);
	e = pfnColumnValue(s1, \
		  initSymbol(allocNode(),getSymbol((S)"Employee")), \
		  initSymbol(allocNode(),getSymbol((S)"DepartmentID")));   CHECK(e,2);
	e = pfnColumnValue(s2, \
		  initSymbol(allocNode(),getSymbol((S)"Department")), \
		  initSymbol(allocNode(),getSymbol((S)"DepartmentID")));   CHECK(e,3);
	e = pfnColumnValue(s3, \
		  initSymbol(allocNode(),getSymbol((S)"Department")), \
		  initSymbol(allocNode(),getSymbol((S)"DepartmentName"))); CHECK(e,4);

	e = pfnIndexOf       (t0, s2, s1);     CHECK(e,5);
	e = pfnFindValidIndex(t1, s2, t0);     CHECK(e,6);
	e = pfnFindValidItem (t2, s2, t0);     CHECK(e,6);

	e = pfnIndex    (r0, s0, t1);     CHECK(e,7);
	e = pfnIndex    (r1, s1, t1);     CHECK(e,8);
	e = pfnIndex    (r2, s3, t2);     CHECK(e,9);

	e = pfnDict     (d0, \
		  initSymbol(allocNode(),getSymbol((S)"LastName")),      r0);  CHECK(e,10);
	e = pfnDict     (d1, \
		  initSymbol(allocNode(),getSymbol((S)"DepartmentID")),  r1);  CHECK(e,11);
	e = pfnDict     (d2, \
		  initSymbol(allocNode(),getSymbol((S)"DepartmentName")),r2);  CHECK(e,12);

	e = pfnList     (z0, 3, d0, d1, d2); CHECK(e,14);
	e = pfnTable    (z, z0);             CHECK(e,15);
	P("\n");
	printTablePretty(z);
	R 0;
}

L testMain(){
	initMain();  // memory
	initSym();   // symbol
	initSys();
	initTable(); // table
	P("Reading table employee\n");
	V tableEmp = readCSV(CSV_EMP, NUM_COL_EMP, TYPE_EMP, SYM_LIST_EMP);
	registerTable((S)"Employee", tableEmp);
	P("Reading table department\n");
	V tableDep = readCSV(CSV_DEP, NUM_COL_DEP, TYPE_DEP, SYM_LIST_DEP);
	registerTable((S)"Department", tableDep);
	/* Simulation */
	simulateSimple();
	/* Print info */
	printSymInfo();
	printHeapInfo();
	getchar();
	R 0;
}

// L loadCSV(S filePath, L types, L size){
// 	R readFile((S)CSV_EMP, 1);
// }

