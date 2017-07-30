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
	DOI(vn(x), if(cId == vq(getDictKey(getTableDict(x,i))))R i)
	R -1;
}

V promoteValue(V x, L typMax){
	if(vp(x) == typMax) R x;
	else{
		V z = allocV(typMax, xn); //opt?
		switch(vp(z)){
			caseH {
				switch(vp(x)){
					caseB DOI(xn, vH(z,i)=vB(x,i)) break;
				}
			} break;
			caseI {
				switch(vp(x)){
					caseB DOI(xn, vI(z,i)=vB(x,i)) break;
					caseH DOI(xn, vI(z,i)=vH(x,i)) break;
				}
			} break;
			caseL {
				switch(vp(x)){
					caseB DOI(xn, vL(z,i)=vB(x,i)) break;
					caseH DOI(xn, vL(z,i)=vH(x,i)) break;
					caseI DOI(xn, vL(z,i)=vI(x,i)) break;
				}
			} break;
			caseF {
				switch(vp(x)){
					caseB DOI(xn, vF(z,i)=(F)vB(x,i)) break;
					caseH DOI(xn, vF(z,i)=(F)vH(x,i)) break;
					caseI DOI(xn, vF(z,i)=(F)vI(x,i)) break;
					caseL DOI(xn, vF(z,i)=(F)vL(x,i)) break;
				}
			} break;
			caseE {
				switch(vp(x)){
					caseB DOI(xn, vE(z,i)=(E)vB(x,i)) break;
					caseH DOI(xn, vE(z,i)=(E)vH(x,i)) break;
					caseI DOI(xn, vE(z,i)=(E)vI(x,i)) break;
					caseL DOI(xn, vE(z,i)=(E)vL(x,i)) break;
					caseF DOI(xn, vE(z,i)=(E)vF(x,i)) break;
				}
			} break;
			caseX {
				switch(vp(x)){
					caseB DOI(xn, xCopy(vX(z,i),vB(x,i),0)) break;
					caseH DOI(xn, xCopy(vX(z,i),vH(x,i),0)) break;
					caseI DOI(xn, xCopy(vX(z,i),vI(x,i),0)) break;
					caseL DOI(xn, xCopy(vX(z,i),vL(x,i),0)) break;
					caseF DOI(xn, xCopy(vX(z,i),vF(x,i),0)) break;
					caseE DOI(xn, xCopy(vX(z,i),vE(x,i),0)) break;
				}
			} break;
			default: R NULL;
		}
		R z;
	}
}

B checkZero(V x){
	switch(xp){
		caseB DOI(xn, if(0==xB(i)) R 0); break;
		caseH DOI(xn, if(0==xH(i)) R 0); break;
		caseI DOI(xn, if(0==xI(i)) R 0); break;
		caseL DOI(xn, if(0==xL(i)) R 0); break;
		caseF DOI(xn, if(0==xF(i)) R 0); break;
		caseE DOI(xn, if(0==xE(i)) R 0); break;
	}
	R 1;
}

L getSingleIntValue(V x){
	R (1==xn)?(H_B==xp?xb:H_H==xp?xh:H_I==xp?xi:H_L==xp?xl:-1):-1;
}

L calcFact(L n){
	if(n>=0){
		L z = 1; DOI(n, z*=(i+1)) R z;
	}
	else R -1;
}

L getNumOfNonZero(V x){
	if(H_B==xp){
		L z = 0; DOI(xn, z+=vB(x,i)) R z;
	}
	R -1;
}

L appendList(V z, V x, V y){R E_NOT_IMPL;}
/* x: enum, y: number */
L appendEnum(V z, V x, V y){R E_NOT_IMPL;}


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

B isTypeGroupDate(L t){
	R (H_M==t || H_D==t || H_Z==t);
}

B isTypeGroupTime(L t){
	R (H_U==t || H_W==t || H_T==t);
}

B isTypeGroupDTime(L t){
	R isTypeGroupDate(t) || isTypeGroupTime(t);
}

B isTypeGroupNumeric(L t){
	R isTypeGroupReal(t) || isTypeGroupDTime(t);
}

B isTypeGroupBasic(L t){
	R (isTypeGroupReal(t) || H_C==t || H_Q==t || isTypeGroupDTime(t));
}

B isTypeGroupAdvanced(L t){
	R 1;
}

B isTypeGroupAll(L t){
	R (isTypeGroupBasic(t) || isTypeGroupAdvanced(t));
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
		errCaseCell(E_LENGTH,          "Length error"       );
		errCaseCell(E_TYPE,            "Type error"         );
		errCaseCell(E_TABLE_NOT_FOUND, "Table not found"    );
		errCaseCell(E_COL_NOT_FOUND,   "Column not found"   );
		errCaseCell(E_NOT_IMPL,        "Not implement yet"  );
	}
	P(".\n");
}
