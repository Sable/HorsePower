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

L testMain(){
	initMain();  // memory
	initSym();   // symbol
	initSys();
	initTable(); // table
	P("Reading table employee\n");
	V tableEmp = readCSV(CSV_EMP, NUM_COL_EMP, TYPE_EMP, SYM_LIST_EMP);
	registerTable((S)"employee", tableEmp);
	P("Reading table department\n");
	V tableDep = readCSV(CSV_DEP, NUM_COL_DEP, TYPE_DEP, SYM_LIST_DEP);
	registerTable((S)"department", tableDep);
	printSymInfo();
	printHeapInfo();
	getchar();
	R 0;
}

// L loadCSV(S filePath, L types, L size){
// 	R readFile((S)CSV_EMP, 1);
// }

