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

/* copy x to z */
L copyV(V z, V x){
	*z = *x; // alias
	R 0;
}


L findColFromTable(V x, L cId){
	DOI(vn(x), if(cId == vs(getDictKey(getTableDict(x,i))))R i)
	R -1;
}

/* Error messages */

#define errCaseCell(label, msg) case label: P(msg); break

void printErrMsg(L eid){
	P("Error: ");
	switch(eid){
		errCaseCell(E_GENERAL,         "General error"   );
		errCaseCell(E_INDEX,           "Index error"     );
		errCaseCell(E_TABLE_NOT_FOUND, "Table not found" );
		errCaseCell(E_COL_NOT_FOUND,   "Column not found");
	}
	P(".\n");
}
