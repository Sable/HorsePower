#include "h_global.h"

const L INIT_HEAP_SIZE  = 65536; //64MB
const L INIT_CACHE_SIZE = 65536; //64MB

/* global vars */

G gHeap = NULL; L gHeapCur;  //global heap and cursor
G gCache= NULL; L gCacheCur; //global cache and cursor


/* methods */

void hInit(){
	gHeap = (G)malloc(INIT_HEAP_SIZE);
	gCache= (G)malloc(INIT_CACHE_SIZE);
	gHeapCur = gCacheCur = 0;
	P("Successfully initialized\n");
}

G getHeapMem(L typ, L len){
	return allocMem(gHeap, gHeapCur, INIT_HEAP_SIZE, typ, len);
}

G getCacheMem(L typ, L len){
	return allocMem(gCache, gCacheCur, INIT_CACHE_SIZE, typ, len);
}

G allocMem(G heap, L cur, L top, L typ, L len){
	L size = getTypeSize(typ) * len;
	G g = NULL;
	if(cur+size < top){
		cur += size;
		g = heap + cur;
	}
	else {
		P("Heap full!!\n");
		exit(99);
	}
	R g;
}

L getTypeSize(L typ){
	L r = -1;
	switch(typ){
		case H_B  : r = sizeof(B);     break;
		case H_C  : r = sizeof(C) + 1; break;
		case H_H  : r = sizeof(H);     break;
		case H_I  : r = sizeof(I);     break;
		case H_L  : r = sizeof(L);     break;
		case H_F  : r = sizeof(F);     break;
		case H_E  : r = sizeof(E);     break;
		case H_X  : r = sizeof(X);     break;
		case H_S  : r = sizeof(L);     break;
		case H_M  : r = sizeof(M);     break;
		case H_D  : r = sizeof(D);     break;
		case H_Z  : r = sizeof(Z);     break;
		case H_U  : r = sizeof(U);     break;
		case H_W  : r = sizeof(W);     break;
		case H_T  : r = sizeof(T);     break;
		case H_G  : r = sizeof(V);     break;
		case H_N  : r = sizeof(V);     break;
		case H_A  : r = sizeof(V);     break;
		case H_K  : r = sizeof(V);     break;
	}
	return r;
}


/* allocate */

V allocV(L typ, L len){
	V x = (V)malloc(sizeof(V0));
	x->typ = typ;
	x->len = len;
	x->g   = (len>1)?getHeapMem(typ, len):NULL;
	R x;
}

V allocDict(){
	R allocV(H_N, 2);
}

V allocList(L numItems){
	R allocV(H_G, numItems);
}

V allocTable(L numCols){
	R allocV(H_A, numCols);
}

V allocKTable(){
	R allocV(H_K, 2);
}







