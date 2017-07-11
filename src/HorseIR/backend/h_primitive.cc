#include "h_global.h"

/*
 * primitive function -> pfn
 * input  parameters  -> x, y, m ...
 * return parameters  -> z
 * return value       ->  0 (normal)
 *                    -> >0 (error)
 * opt. idea
 *    in-lining + CSE
 */

L pfnColumnValue(V z, V x, V y){
	if(isOneSymbol(x) && isOneSymbol(y)){
		V t = findTableByName(vs(x));
		if(!t) R E_TABLE_NOT_FOUND;
		else {
			R copyTable(z, t);
		}
	}
	else R E_DOMAIN;
}

L pfnIndexOf(V z, V x, V y){
	if(vp(x) == vp(y)){
		setarg(z, H_L, vn(y));
		switch(vp(y)){
			caseL lib_index_of_i32(vL(z),vL(x),vn(x),vL(y),vn(y)); break;
		}
	}
	else R E_DOMAIN;
}

// example: (21 45 7 9), (45 36 9)

/*
 * find_left: x[(y<#x)/y]
 */
L pfnFindLeft(V z, V x, V y){
	if(vp(x) == vp(y) && isInteger(x)){
		L lenz = 0, typz = H_B;
		L leny = vn(y), lenx = vn(x), cnt = 0;
		DOI(leny, lenz+=vL(y)[i]<lenx)
		setarg(z, typz, lenz);
		DOI(leny, {L t=vL(y)[i]; if(t>=0 && t<lenx)vL(z)[cnt++]=vL(x)[t];})
	}
	else R E_DOMAIN;
}

/*
 * find_right: x[(y<#m)/!#y]
 */
L pfnFindRight(V x, V y, V m){
	if(vp(x) == vp(y) && vp(x) == vp(m) && vn(x) == vn(y) && isInteger(x)){
		L lenz = 0, typz = H_B;
		L lenm = vn(m), leny = vn(y), cnt = 0;
		DOI(leny, lenz+=vL(y)<lenm)
		setarg(z, typz, lenz);
		DOI(leny, {L t=vL(y)[i]; if(t<lenm)vL(z)[cnt++]=vL(x)[i];})
	}
	else R E_DOMAIN;
}

/*
 * indexing: x[y]
 */
L pfnIndex(V z, V x, V y){
	if(isInteger(y)){
		L typz = vp(x), lenz = vn(y), lenx = vn(x);
		if(isSymbol(x)){
			DOI(lenz, if(lenx <= vL(y)[i])R E_INDEX)
			setarg(z, typz, lenz);
			DOI(lenz, vS(z)[i] = vS(x)[vL(y)[i]])
			R 0;
		}
	}
	R E_DOMAIN;
}

/* list, dict, table */

L pfnList(ListV x){
}

L pfnDict(V x){
}

L pfnTable(V x){
}

