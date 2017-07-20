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



/* Type checking */

B isTypeGroupInt(L t){
	R (H_B==t || H_H==t || H_I==t || H_L==t);
}

B isTypeGroupFloat(L t){
	R (H_F==t || H_E==t);
}

B isTypeGroupReal(L t){
	R (isTypeGroupInt(t) || isTypeGroupFloat(t));
}

B isTypeGroupRealX(L t){
	R (isTypeGroupReal(t) || H_X==t);
}

B isTypeGroupTime(L t){
	R (H_M==t || H_D==t || H_Z==t || H_U==t || H_W==t || H_T==t);
}

B isTypeGroupBasic(L t){
	R (isTypeGroupReal(t) || H_C==t || H_S==t || isTypeGroupTime(t));
}

/* Type inference */

L inferNeg(L t){
	R (H_B==t?H_L:t);
}

L inferReal2Int(L t){
	R isTypeGroupFloat(t)?H_L:t;
}

L inferRecip(L t){
	R (H_F==t?H_F:H_E);
}

L inferSignum(L t){
	R (H_H==t||H_I==t)?t:H_L;
}

L inferPi(L t){
	R (H_F==t||H_X==t)?t:H_E;
}

/* Error messages */

#define errCaseCell(label, msg) case label: P(msg); break

void printErrMsg(L eid){
	P("Error: ");
	switch(eid){
		errCaseCell(E_GENERAL,         "General error"      );
		errCaseCell(E_INDEX,           "Index error"        );
		errCaseCell(E_DIV_ZERO,        "Divide zero"        );
		errCaseCell(E_TABLE_NOT_FOUND, "Table not found"    );
		errCaseCell(E_COL_NOT_FOUND,   "Column not found"   );
		errCaseCell(E_NOT_IMPL,        "Not implement yet"  );
	}
	P(".\n");
}
