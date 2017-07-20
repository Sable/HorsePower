#include "h_global.h"

/* temp */
L lib_index_of_i32(L *indx, L *src, L slen, L *targ, L tlen){
	DOI(tlen, {B f=1;\
		DOJ(slen, if(targ[i]==src[j]){indx[i]=j;f=0;break;})\
		if(f)indx[i]=slen;})
	R 0;
}

/*
 * primitive function -> pfn
 * input  parameters  -> x, y, m ...
 * return parameters  -> z
 * return value       ->  0 (normal)
 *                    -> >0 (error)
 * opt. idea
 *    in-lining + CSE
 */

/* x.y */
L pfnColumnValue(V z, V x, V y){
	P("-> Entering column_value\n");
	if(isOneSymbol(x) && isOneSymbol(y)){
		V t = findTableByName(vs(x));
		if(!t) R E_TABLE_NOT_FOUND;
		else {
			L colId = vs(y);
			L colIndex = findColFromTable(t,colId);
			if(colIndex < 0) R E_COL_NOT_FOUND;
			else {
				R copyColumnValue(z, getDictVal(getTableDict(t,colIndex)));
			}
		}
	}
	else R E_DOMAIN;
}

L pfnIndexOf(V z, V x, V y){
	P("-> Entering index_of\n");
	if(vp(x) == vp(y)){
		initV(z, H_L, vn(y));
		switch(vp(y)){
			caseL lib_index_of_i32(sL(z),sL(x),vn(x),sL(y),vn(y)); break;
		}
		R 0;
	}
	else R E_DOMAIN;
}

// example: (21 45 7 9), (45 36 9)

/*
 * find_valid_index: (y<#x)/!#y
 */
L pfnFindValidIndex(V z, V x, V y){
	P("-> Entering find_valid_index\n");
	if(vp(x) == vp(y) && isInteger(x)){
		L lenz = 0, typz = H_L;
		L leny = vn(y), lenx = vn(x), cnt = 0;
		DOI(leny, {L t=vL(y,i); lenz+=(t>=0&&t<lenx);})
		initV(z, typz, lenz);
		DOI(leny, {L t=vL(y,i); if(t>=0 && t<lenx)vL(z,cnt++)=i;})
		R 0;
	}
	else R E_DOMAIN;
}

/*
 * find_valid_item: (y<#x)/y
 */
L pfnFindValidItem(V z, V x, V y){
	P("-> Entering find_valid_item\n");
	if(vp(x) == vp(y) && isInteger(x)){
		L lenz = 0, typz = H_L;
		L leny = vn(y), lenx = vn(x), cnt = 0;
		DOI(leny, {L t=vL(y,i); lenz+=(t>=0&&t<lenx);})
		initV(z, typz, lenz);
		DOI(leny, {L t=vL(y,i); if(t>=0 && t<lenx)vL(z,cnt++)=t;})
		R 0;
	}
	else R E_DOMAIN;
}

/*
 * indexing: x[y]
 */
L pfnIndex(V z, V x, V y){
	P("-> Entering index\n");
	if(isInteger(y)){
		L typz = vp(x), lenz = vn(y), lenx = vn(x);
		if(isSymbol(x) || isInteger(x)){ // basic types, non-list
			DOI(lenz, if(lenx <= vL(y,i))R E_INDEX)
			initV(z, typz, lenz);
			switch(typz){
				caseS DOI(lenz, vS(z,i) = vS(x,vL(y,i))) break;
				caseL DOI(lenz, vL(z,i) = vL(x,vL(y,i))) break;
			}
			R 0;
		}
		else R E_DOMAIN;
	}
	else R E_DOMAIN;
}

/* list, dict, table */

/* copy alias */
L pfnList(V z, L n, ...){
	va_list args;
	initList(z, n);
	va_start(args, n);
	DOI(n, {V x=va_arg(args,V);*(sV(z)+i)=*x;})
	va_end  (args);
	R 0;
}

/* copy alias */
L pfnDict(V z, V x, V y){
	initDict(z);
	V key = getDictKey(z);
	V val = getDictVal(z);
	*key = *x;
	*val = *y;
	R 0;
}

/* copy alias */
L pfnTable(V z, V x){
	initTable(z, vn(x));
	/* need to check dict */
	DOI(vn(x), *(sV(z)+i)=*(sV(x)+i))
	R 0;
}


/* Implement in order */

const E PI = acos(-1);

#define ABS  (x) ((x)<0?(-x):x)
#define NEG  (x) -(x)
#define CEIL (x) ceil(x)
#define FLOOR(x) floor(x)
#define ROUND(x) round(x)

/* abs */
L pfnAbs(V z, V x){
	if(isTypeGroupReal(vp(x))){
		initV(z,vp(x),vn(x));  // Step 1: initialize z
		switch(vp(x)){         // Step 2: based on x
			caseB DOI(vn(x), vB(z,i)=ABS(vB(x,i))) break;
			caseH DOI(vn(x), vH(z,i)=ABS(vH(x,i))) break;
			caseI DOI(vn(x), vI(z,i)=ABS(vI(x,i))) break;
			caseL DOI(vn(x), vL(z,i)=ABS(vL(x,i))) break;
			caseF DOI(vn(x), vF(z,i)=ABS(vF(x,i))) break;
			caseE DOI(vn(x), vE(z,i)=ABS(vE(x,i))) break;
		}
		R 0;
	}
	else R E_DOMAIN;
}

/* neg */
L pfnNeg(V z, V x){
	if(isTypeGroupReal(vp(x))){
		L typZ = inferNeg(vp(x));
		L lenZ = vn(x);
		initV(z,typZ,lenZ);
		switch(vp(x)){       
			caseB DOI(lenZ, vB(z,i)=NEG(vB(x,i))) break;
			caseH DOI(lenZ, vH(z,i)=NEG(vH(x,i))) break;
			caseI DOI(lenZ, vI(z,i)=NEG(vI(x,i))) break;
			caseL DOI(lenZ, vL(z,i)=NEG(vL(x,i))) break;
			caseF DOI(lenZ, vF(z,i)=NEG(vF(x,i))) break;
			caseE DOI(lenZ, vE(z,i)=NEG(vE(x,i))) break;
		}
		R 0;
	}
	else R E_DOMAIN;
}

L pfnCeil(V z, V x){
	if(isTypeGroupReal(vp(x))){
		L typZ = inferReal2Int(vp(x));
		L lenZ = vn(x);
		initV(z,typZ,lenZ);
		switch(vp(x)){
			caseB DOI(lenZ, vB(z,i)=vB(x,i)) break; //merely copy (opt)
			caseH DOI(lenZ, vH(z,i)=vH(x,i)) break;
			caseI DOI(lenZ, vI(z,i)=vI(x,i)) break;
			caseL DOI(lenZ, vL(z,i)=vL(x,i)) break;
			caseF DOI(lenZ, vL(z,i)=CEIL(vF(x,i))) break;
			caseE DOI(lenZ, vL(z,i)=CEIL(vE(x,i))) break;
		}
		R 0;
	}
	else R E_DOMAIN;
}

L pfnFloor(V z, V x){
	if(isTypeGroupReal(vp(x))){
		L typZ = inferReal2Int(vp(x));
		L lenZ = vn(x);
		initV(z,typZ,lenZ);
		switch(vp(x)){
			caseB DOI(lenZ, vB(z,i)=vB(x,i)) break; //merely copy (opt)
			caseH DOI(lenZ, vH(z,i)=vH(x,i)) break;
			caseI DOI(lenZ, vI(z,i)=vI(x,i)) break;
			caseL DOI(lenZ, vL(z,i)=vL(x,i)) break;
			caseF DOI(lenZ, vL(z,i)=FLOOR(vF(x,i))) break;
			caseE DOI(lenZ, vL(z,i)=FLOOR(vE(x,i))) break;
		}
		R 0;
	}
	else R E_DOMAIN;
}

L pfnRound(V z, V x){
	if(isTypeGroupReal(vp(x))){
		L typZ = inferReal2Int(vp(x));
		L lenZ = vn(x);
		initV(z,typZ,lenZ);
		switch(vp(x)){
			caseB DOI(lenZ, vB(z,i)=vB(x,i)) break; //merely copy (opt)
			caseH DOI(lenZ, vH(z,i)=vH(x,i)) break;
			caseI DOI(lenZ, vI(z,i)=vI(x,i)) break;
			caseL DOI(lenZ, vL(z,i)=vL(x,i)) break;
			caseF DOI(lenZ, vL(z,i)=ROUND(vF(x,i))) break;
			caseE DOI(lenZ, vL(z,i)=ROUND(vE(x,i))) break;
		}
		R 0;
	}
	else R E_DOMAIN;
}

L pfnConj(V z, V x){
	if(isComplex(x)){
		L typZ = H_X;
		L lenZ = vn(x);
		initV(z,typZ,lenZ);
		DOI(lenZ, {xReal(vX(z,i))=xReal(vX(x,i));xImag(vX(z,i))=-xImag(vX(x,i));})
		R 0;
	}
	else R E_DOMAIN;
}

#define recipSum(t, ptr) { DOI(lenZ, t+=ptr(x,i)); if(0==t) R E_DIV_ZERO; }
L pfnRecip(V z, V x){
	if(isTypeGroupReal(vp(x))){
		L typZ = inferRecip(vp(x));
		L lenZ = vn(x);
		initV(z,typZ,lenZ);
		switch(vp(x)){
			caseB {E t=0; recipSum(t,vB); DOI(lenZ, vE(z,i)=vB(x,i)/t); } break;
			caseH {E t=0; recipSum(t,vH); DOI(lenZ, vE(z,i)=vH(x,i)/t); } break;
			caseI {E t=0; recipSum(t,vI); DOI(lenZ, vE(z,i)=vI(x,i)/t); } break;
			caseL {E t=0; recipSum(t,vL); DOI(lenZ, vE(z,i)=vL(x,i)/t); } break;
			caseF {F t=0; recipSum(t,vF); DOI(lenZ, vF(z,i)=vF(x,i)/t); } break; //F
			caseE {E t=0; recipSum(t,vE); DOI(lenZ, vE(z,i)=vE(x,i)/t); } break;
		}
		R 0;
	}
	else R E_DOMAIN;
}

/* 1 -1 0 */
#define signumCmp(z,x) (0<(x)?1:0>(x)?-1:0)
L pfnSignum(V z, V x){
	if(isTypeGroupReal(vp(x))){
		L typZ = inferSignum(vp(x));
		L lenZ = vn(x);
		initV(z,typZ,lenZ);
		switch(vp(x)){
			caseB DOI(lenZ, vL(z,i)=signumCmp(vB(x,i))) break;
			caseH DOI(lenZ, vH(z,i)=signumCmp(vH(x,i))) break;
			caseI DOI(lenZ, vI(z,i)=signumCmp(vI(x,i))) break;
			caseL DOI(lenZ, vL(z,i)=signumCmp(vL(x,i))) break;
			caseF DOI(lenZ, vL(z,i)=signumCmp(vF(x,i))) break;
			caseE DOI(lenZ, vL(z,i)=signumCmp(vE(x,i))) break;
		}
		R 0;
	}
	else R E_DOMAIN;
}

#define piMulReal(x) (PI*x)
L pfnPi(V z, V x){
	if(isTypeGroupRealX(vp(x))){
		L typZ = inferPi(vp(x));
		L lenZ = vn(x);
		initV(z,typZ,lenZ);
		switch(vp(x)){
			caseB DOI(lenZ, vE(z,i)=piMulReal(vB(x,i))) break;
			caseH DOI(lenZ, vE(z,i)=piMulReal(vH(x,i))) break;
			caseI DOI(lenZ, vE(z,i)=piMulReal(vI(x,i))) break;
			caseL DOI(lenZ, vE(z,i)=piMulReal(vL(x,i))) break;
			caseF DOI(lenZ, vF(z,i)=piMulReal(vF(x,i))) break; //F
			caseE DOI(lenZ, vE(z,i)=piMulReal(vE(x,i))) break;
			caseX R E_NOT_IMPL;
		}
		R 0;
	}
	else R E_DOMAIN;
}

#define notBool(x) (~x)
L pfnNot(V z, V x){
	if(isBool(x)){
		L typZ = H_B;
		L lenZ = vn(x);
		initV(z,typZ,lenZ);
		DOI(lenZ, vB(z,i)=notBool(vB(x,i)));
		R 0;
	}
	else R E_DOMAIN;
}



