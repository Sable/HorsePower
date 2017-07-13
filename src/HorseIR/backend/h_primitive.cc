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
			caseL lib_index_of_i32(vL(z),vL(x),vn(x),vL(y),vn(y)); break;
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
		DOI(leny, {L t=vL(y)[i]; lenz+=(t>=0&&t<lenx);})
		initV(z, typz, lenz);
		DOI(leny, {L t=vL(y)[i]; if(t>=0 && t<lenx)vL(z)[cnt++]=i;})
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
		DOI(leny, {L t=vL(y)[i]; lenz+=(t>=0&&t<lenx);})
		initV(z, typz, lenz);
		DOI(leny, {L t=vL(y)[i]; if(t>=0 && t<lenx)vL(z)[cnt++]=t;})
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
			DOI(lenz, if(lenx <= vL(y)[i])R E_INDEX)
			initV(z, typz, lenz);
			switch(typz){
				caseS DOI(lenz, vS(z)[i] = vS(x)[vL(y)[i]]) break;
				caseL DOI(lenz, vL(z)[i] = vL(x)[vL(y)[i]]) break;
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
	DOI(n, {V x=va_arg(args,V);*(vV(z)+i)=*x;})
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
	DOI(vn(x), *(vV(z)+i)=*(vV(x)+i))
	R 0;
}






