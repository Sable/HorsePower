/*
 * system functions
 */

#include "h_global.h"

const L NUM_LIST_TABLE = 100;

ListT listTable;
L listTableCur;

void initSys(){
	listTable = (ListT)malloc(sizeof(ListT0) * NUM_LIST_TABLE);
	listTableCur = 0;
}

void deleteSys(){
	listTableCur = 0;
	free(listTable);
}

L registerTable(S tableName, V tablePtr){
	listTable[listTableCur].sid   = getSymbol(tableName);
	listTable[listTableCur].table = tablePtr;
	R listTableCur++;
}

V findTableByName(L sid){
	DOI(listTableCur, if(listTable[i].sid == sid){R listTable[i].table;})
	R NULL;
}

/* copy y to x */
L copyTable(V x, V y){
	*x = *y; // alias
	R 0;
}

/* Error messages */

#define errCaseCell(label, msg) case label: P(msg); break

void printErrMsg(L eid){
	P("Error: ");
	switch(eid){
		errCaseCell(E_GENERAL,         "General error"  );
		errCaseCell(E_INDEX,           "Index error"    );
		errCaseCell(E_TABLE_NOT_FOUND, "Table not found");
	}
	P(".\n");
}
