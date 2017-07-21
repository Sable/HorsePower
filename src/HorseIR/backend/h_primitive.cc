#include "h_global.h"
#include <math.h>

#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif

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
		L lenZ = 0, typZ = H_L;
		L lenY = vn(y), lenX = vn(x), cnt = 0;
		DOI(lenY, {L t=vL(y,i); lenZ+=(t>=0&&t<lenX);})
		initV(z, typZ, lenZ);
		DOI(lenY, {L t=vL(y,i); if(t>=0 && t<lenX)vL(z,cnt++)=i;})
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
		L lenZ = 0, typZ = H_L;
		L lenY = vn(y), lenX = vn(x), cnt = 0;
		DOI(lenY, {L t=vL(y,i); lenZ+=(t>=0&&t<lenX);})
		initV(z, typZ, lenZ);
		DOI(lenY, {L t=vL(y,i); if(t>=0 && t<lenX)vL(z,cnt++)=t;})
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
		L typZ = vp(x), lenZ = vn(y), lenX = vn(x);
		if(isSymbol(x) || isInteger(x)){ // basic types, non-list
			DOI(lenZ, if(lenX <= vL(y,i))R E_INDEX)
			initV(z, typZ, lenZ);
			switch(typZ){
				caseS DOI(lenZ, vS(z,i) = vS(x,vL(y,i))) break;
				caseL DOI(lenZ, vL(z,i) = vL(x,vL(y,i))) break;
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

#define ABS(x) ((x)<0?(-x):x)
#define NEG(x) -(x)
#define CEIL(x) ceil(x)
#define FLOOR(x) floor(x)
#define ROUND(x) round(x)
#define DIVDE(x,t) (x/t)
#define SIGNUM(x) (0<(x)?1:0>(x)?-1:0)
#define PIMUL(x) (PI*x)
#define NOT(x) (~x)

/* abs */
L pfnAbs(V z, V x){
	if(isTypeGroupReal(vp(x))){
		initV(z,vp(x),vn(x));  // Step 1: initialize z
		switch(vp(x)){         // Step 2: based on x
			caseB DOI(vn(x), vB(z,i)=vB(x,i))      break; //opt
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
			caseB {E t=0; recipSum(t,vB); DOI(lenZ, vE(z,i)=DIVDE(vB(x,i),t)); } break;
			caseH {E t=0; recipSum(t,vH); DOI(lenZ, vE(z,i)=DIVDE(vH(x,i),t)); } break;
			caseI {E t=0; recipSum(t,vI); DOI(lenZ, vE(z,i)=DIVDE(vI(x,i),t)); } break;
			caseL {E t=0; recipSum(t,vL); DOI(lenZ, vE(z,i)=DIVDE(vL(x,i),t)); } break;
			caseF {F t=0; recipSum(t,vF); DOI(lenZ, vF(z,i)=DIVDE(vF(x,i),t)); } break; //F
			caseE {E t=0; recipSum(t,vE); DOI(lenZ, vE(z,i)=DIVDE(vE(x,i),t)); } break;
		}
		R 0;
	}
	else R E_DOMAIN;
}

/* 1 -1 0 */
L pfnSignum(V z, V x){
	if(isTypeGroupReal(vp(x))){
		L typZ = inferSignum(vp(x));
		L lenZ = vn(x);
		initV(z,typZ,lenZ);
		switch(vp(x)){
			caseB DOI(lenZ, vL(z,i)=vB(x,i))         break; //opt, no -1
			caseH DOI(lenZ, vH(z,i)=SIGNUM(vH(x,i))) break;
			caseI DOI(lenZ, vI(z,i)=SIGNUM(vI(x,i))) break;
			caseL DOI(lenZ, vL(z,i)=SIGNUM(vL(x,i))) break;
			caseF DOI(lenZ, vL(z,i)=SIGNUM(vF(x,i))) break;
			caseE DOI(lenZ, vL(z,i)=SIGNUM(vE(x,i))) break;
		}
		R 0;
	}
	else R E_DOMAIN;
}

L pfnPi(V z, V x){
	if(isTypeGroupRealX(vp(x))){
		L typZ = inferPi(vp(x));
		L lenZ = vn(x);
		initV(z,typZ,lenZ);
		switch(vp(x)){
			caseB DOI(lenZ, vE(z,i)=PIMUL(vB(x,i))) break;
			caseH DOI(lenZ, vE(z,i)=PIMUL(vH(x,i))) break;
			caseI DOI(lenZ, vE(z,i)=PIMUL(vI(x,i))) break;
			caseL DOI(lenZ, vE(z,i)=PIMUL(vL(x,i))) break;
			caseF DOI(lenZ, vF(z,i)=PIMUL(vF(x,i))) break; //F
			caseE DOI(lenZ, vE(z,i)=PIMUL(vE(x,i))) break;
			caseX R E_NOT_IMPL;
		}
		R 0;
	}
	else R E_DOMAIN;
}

L pfnNot(V z, V x){
	if(isBool(x)){
		L typZ = H_B;
		L lenZ = vn(x);
		initV(z,typZ,lenZ);
		DOI(lenZ, vB(z,i)=NOT(vB(x,i)));
		R 0;
	}
	else R E_DOMAIN;
}

L pfnLen(V z, V x){
	initV(z,H_L,1);
	vl(z) = vn(x);
	R 0;
}

L pfnRange(V z, V x){
	if(isTypeGroupInt(x)){
		L size = getSingleIntValue(x);
		initV(z,H_L,size);
		DOI(size, vL(z,i)=i) // opt, stride?
		R 0;
	}
	else R E_DOMAIN;
}

/* Binary */

#define COMP(op,x,y) (2>op?COMPLESS(op,x,y):4>op?COMPMORE(op,x,y):6>op?COMPEQ(op,x,y):0)
#define COMPLESS(op,x,y) (0==op?(x<y):x<=y)
#define COMPMORE(op,x,y) (2==op?(x>y):x>=y)
#define COMPEQ(op,x,y) (4==op?(x==y):x!=y)

/* Not impl.: C, S, Time  */
L pfnCompare(V z, V x, V y, L op){
	if(isTypeGroupBasic(vp(x)) && isTypeGroupBasic(vp(y))){
		if(!isValidType  (x,y)) R E_TYPE;
		if(!isValidLength(x,y)) R E_LENGTH;
		L lenZ   = isOne(x)?vn(y):vn(x), typZ = H_B;
		L typMax = max(vp(x),vp(y));
		V tempX  = promoteValue(x, typMax);
		V tempY  = promoteValue(y, typMax);
		initV(z,typZ,lenZ);
		if(isOne(x)) {
			switch(typMax){
				caseB DOI(lenZ, vB(z,i)=COMP(op,vB(tempX,0),vB(tempY,i))) break;
				caseH DOI(lenZ, vB(z,i)=COMP(op,vH(tempX,0),vH(tempY,i))) break;
				caseI DOI(lenZ, vB(z,i)=COMP(op,vI(tempX,0),vI(tempY,i))) break;
				caseL DOI(lenZ, vB(z,i)=COMP(op,vL(tempX,0),vL(tempY,i))) break;
				caseF DOI(lenZ, vB(z,i)=COMP(op,vF(tempX,0),vF(tempY,i))) break;
				caseE DOI(lenZ, vB(z,i)=COMP(op,vE(tempX,0),vE(tempY,i))) break;
				default: R E_NOT_IMPL;
			}
		}
		else if(isOne(y)) {
			switch(typMax){
				caseB DOI(lenZ, vB(z,i)=COMP(op,vB(tempX,i),vB(tempY,0))) break;
				caseH DOI(lenZ, vB(z,i)=COMP(op,vH(tempX,i),vH(tempY,0))) break;
				caseI DOI(lenZ, vB(z,i)=COMP(op,vI(tempX,i),vI(tempY,0))) break;
				caseL DOI(lenZ, vB(z,i)=COMP(op,vL(tempX,i),vL(tempY,0))) break;
				caseF DOI(lenZ, vB(z,i)=COMP(op,vF(tempX,i),vF(tempY,0))) break;
				caseE DOI(lenZ, vB(z,i)=COMP(op,vE(tempX,i),vE(tempY,0))) break;
				default: R E_NOT_IMPL;
			}
		}
		else {
			switch(typMax){
				caseB DOI(lenZ, vB(z,i)=COMP(op,vB(tempX,i),vB(tempY,i))) break;
				caseH DOI(lenZ, vB(z,i)=COMP(op,vH(tempX,i),vH(tempY,i))) break;
				caseI DOI(lenZ, vB(z,i)=COMP(op,vI(tempX,i),vI(tempY,i))) break;
				caseL DOI(lenZ, vB(z,i)=COMP(op,vL(tempX,i),vL(tempY,i))) break;
				caseF DOI(lenZ, vB(z,i)=COMP(op,vF(tempX,i),vF(tempY,i))) break;
				caseE DOI(lenZ, vB(z,i)=COMP(op,vE(tempX,i),vE(tempY,i))) break;
				default: R E_NOT_IMPL;
			}
		}
		R 0;
	}
	else R E_DOMAIN;
}

/* less than */
L pfnLt(V z, V x, V y){
	R pfnCompare(z,x,y,0);
}

/* less equal than */
L pfnLeq(V z, V x, V y){
	R pfnCompare(z,x,y,1);
}

/* greater than */
L pfnGt(V z, V x, V y){
	R pfnCompare(z,x,y,2);
}

/* greater equal than */
L pfnGeq(V z, V x, V y){
	R pfnCompare(z,x,y,3);
}

/* equal */
L pfnEq(V z, V x, V y){
	R pfnCompare(z,x,y,4);
}

/* not equal */
L pfnNeq(V z, V x, V y){
	R pfnCompare(z,x,y,5);
}

#define ARITH2(op,x,y) (0==op?(x+y):1==op?(x-y):2==op?(x*y):3==op?(x/y):0)

L pfnArith(V z, V x, V y, L op){
	if(isTypeGroupReal(vp(x)) && isTypeGroupReal(vp(y))){
		if(!isValidLength(x,y))   R E_LENGTH;
		if(3==op && checkZero(y)) R E_DIV_ZERO;
		L lenZ   = isOne(x)?vn(y):vn(x);
		L typMax = max(vp(x),vp(y));
		L typZ   = typMax;
		V tempX  = promoteValue(x, typMax);
		V tempY  = promoteValue(y, typMax);
		initV(z,typZ,lenZ);
		if(isOne(x)) {
			switch(typMax){
				caseB DOI(lenZ, vB(z,i)=ARITH2(op,vB(tempX,0),vB(tempY,i))) break;
				caseH DOI(lenZ, vH(z,i)=ARITH2(op,vH(tempX,0),vH(tempY,i))) break;
				caseI DOI(lenZ, vI(z,i)=ARITH2(op,vI(tempX,0),vI(tempY,i))) break;
				caseL DOI(lenZ, vL(z,i)=ARITH2(op,vL(tempX,0),vL(tempY,i))) break;
				caseF DOI(lenZ, vF(z,i)=ARITH2(op,vF(tempX,0),vF(tempY,i))) break;
				caseE DOI(lenZ, vE(z,i)=ARITH2(op,vE(tempX,0),vE(tempY,i))) break;
				default: R E_NOT_IMPL;
			}
		}
		else if(isOne(y)) {
			switch(typMax){
				caseB DOI(lenZ, vB(z,i)=ARITH2(op,vB(tempX,i),vB(tempY,0))) break;
				caseH DOI(lenZ, vH(z,i)=ARITH2(op,vH(tempX,i),vH(tempY,0))) break;
				caseI DOI(lenZ, vI(z,i)=ARITH2(op,vI(tempX,i),vI(tempY,0))) break;
				caseL DOI(lenZ, vL(z,i)=ARITH2(op,vL(tempX,i),vL(tempY,0))) break;
				caseF DOI(lenZ, vF(z,i)=ARITH2(op,vF(tempX,i),vF(tempY,0))) break;
				caseE DOI(lenZ, vE(z,i)=ARITH2(op,vE(tempX,i),vE(tempY,0))) break;
				default: R E_NOT_IMPL;
			}
		}
		else {
			switch(typMax){
				caseB DOI(lenZ, vB(z,i)=ARITH2(op,vB(tempX,i),vB(tempY,i))) break;
				caseH DOI(lenZ, vH(z,i)=ARITH2(op,vH(tempX,i),vH(tempY,i))) break;
				caseI DOI(lenZ, vI(z,i)=ARITH2(op,vI(tempX,i),vI(tempY,i))) break;
				caseL DOI(lenZ, vL(z,i)=ARITH2(op,vL(tempX,i),vL(tempY,i))) break;
				caseF DOI(lenZ, vF(z,i)=ARITH2(op,vF(tempX,i),vF(tempY,i))) break;
				caseE DOI(lenZ, vE(z,i)=ARITH2(op,vE(tempX,i),vE(tempY,i))) break;
				default: R E_NOT_IMPL;
			}
		}
		R 0;
	}
	else if(isTypeGroupTime(vp(x)) && isTypeGroupTime(vp(y))){
		switch(op){
			case 0:  break; //plus
			case 1:  break; //sub
			case 2:  break; //mul
			case 3:  break; //div
		}
		R E_NOT_IMPL;
	}
	else R E_DOMAIN;
}

/* plus: 0 */
L pfnPlus(V z, V x, V y){
	R pfnArith(z,x,y,0);
}

/* plus: 1 */
L pfnMinus(V z, V x, V y){
	R pfnArith(z,x,y,1);
}

/* plus: 2 */
L pfnMul(V z, V x, V y){
	R pfnArith(z,x,y,2);
}

/* plus: 3 */
L pfnDiv(V z, V x, V y){
	R pfnArith(z,x,y,3);
}

#define LOGIC(op,x,y) (2>op?LOGICAND(op,x,y):4>op?LOGICOR(op,x,y):(x^y))
#define LOGICAND(op,x,y) (0==op?(x&y):~(x&y))
#define LOGICOR(op,x,y)  (3==op?(x|y):~(x|y))

L pfnLogic(V z, V x, V y, L op){
	if(isBool(x) && isBool(y)){
		if(!isValidLength(x,y)) R E_LENGTH;
		L lenZ = isOne(x)?vn(y):vn(x), typZ = H_B;
		initV(z,typZ,lenZ);
		if(isOne(x)) {
			DOI(lenZ, vB(z,i)=LOGIC(op,vB(x,0),vB(y,i)))
		}
		else if(isOne(y)) {
			DOI(lenZ, vB(z,i)=LOGIC(op,vB(x,i),vB(y,0)))
		}
		else {
			DOI(lenZ, vB(z,i)=LOGIC(op,vB(x,i),vB(y,i)))
		}
		R 0;
	}
	else R E_DOMAIN;
}

/* and: 0 */
L pfnAnd(V z, V x, V y){
	R pfnLogic(z,x,y,0);
}

/* nand: 1 */
L pfnNand(V z, V x, V y){
	R pfnLogic(z,x,y,1);
}

/* or: 2 */
L pfnOr(V z, V x, V y){
	R pfnLogic(z,x,y,2);
}

/* nor: 3 */
L pfnNor(V z, V x, V y){
	R pfnLogic(z,x,y,3);
}

/* nor: 4 */
L pfnXor(V z, V x, V y){
	R pfnLogic(z,x,y,4);
}




