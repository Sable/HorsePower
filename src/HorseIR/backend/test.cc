#include "h_global.h"

const C CSV_EMP[] = "data/simple-join/employee.csv";
const C CSV_DEP[] = "data/simple-join/department.csv";
const C* COLS_EMP[] = {"LastName", "DepartmentID"};
const C* COLS_DEP[] = {"DepartmentID", "DepartmentName"};
// const L[] TYPE_EMP  = {H_S, H_L};
// const L[] TYPE_DEP  = {H_L, H_S};

L testInputFile(S filePath){
	if(!filePath){
		fprintf(stderr, "Correct path must be set (-p path)\n");
		exit(99);
	}
	P("path = %s\n", filePath);
	readFile(filePath, 1);
	R 0;
}


L testMain(){
	hInit(); 
	// if(loadCSV(CSV_EMP) && loadCSV(CSV_DEP)){
	// 	P("Two csv files have been loaded successfully");
	// }
	getchar();
	R 0;
}

