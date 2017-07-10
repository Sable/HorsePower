#include "h_global.h"

const L INIT_HEAP_SIZE  = 65536; //64MB
const L INIT_CACHE_SIZE = 65536; //64MB

/* global vars */

G gHeap = NULL; L gHeapCur;  //global heap and cursor
G gCache= NULL; L gCacheCur; //global cache and cursor


/* methods */

void initMain(){
	gHeap = (G)malloc(INIT_HEAP_SIZE);
	gCache= (G)malloc(INIT_CACHE_SIZE);
	gHeapCur = gCacheCur = 0;
	P("Successfully initialized\n");
}

G getHeapMem(L typ, L len){
	return allocMem(gHeap, &gHeapCur, INIT_HEAP_SIZE, typ, len);
}

G getCacheMem(L typ, L len){
	return allocMem(gCache, &gCacheCur, INIT_CACHE_SIZE, typ, len);
}

G allocMem(G heap, L *cur, L top, L typ, L len){
	L size = getTypeSize(typ) * len;
	G g = NULL;
	if((*cur)+size < top){
		*cur = (*cur) + size;
		g = heap + (*cur);
	}
	else {
		P("Heap full!!\n");
		exit(99);
	}
	P("cur = %lld\n", *cur);
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
		case H_G  : r = sizeof(V0);    break;
		case H_N  : r = sizeof(V0);    break;
		case H_A  : r = sizeof(V0);    break;
		case H_K  : r = sizeof(V0);    break;
		case H_V  : r = sizeof(V0);    break;
	}
	return r;
}


/* allocate */

V allocV(L typ, L len){
	V x = (V)getHeapMem(H_V, 1); //malloc(sizeof(V0));
	initV(x, typ, len);
	R x;
}

V allocDict(){
	V x = allocV(H_N, 2);
	P("-> dict: %lld\n",x);
	R x;
}

V allocList(L numItems){
	V x = allocV(H_G, numItems);
	P("-> list: %lld\n",x);
	R x;
}

V allocTable(L numCols){
	V x = allocV(H_A, numCols);
	P("-> table: %lld\n",x);
	R x;
}

V allocKTable(){
	V x = allocV(H_K, 2);
	P("-> ktable: %lld\n",x);
	R x;
}

/* initialization */

void initV(V x, L typ, L len){
	xp = typ;
	xn = len;
	xg = (len>1)?getHeapMem(typ, len):NULL;
}

void initDict(V x){
	initV(x, H_N, 2);
}

void initSymbol(V x, L val){
	initV(x, H_S, 1);
	xs = val;
}

void initList(V x, L numItems){
	initV(x, H_G, numItems);
}

void initValue(V x, L typ, L len){
	initV(x,typ,len);
}


void printType(L x){
	C buff[128];
	printTypeStr(x, buff);
	P("%s",buff);
}

void printTypeStr(L x, S buff){
	switch(x){
		caseB SP(buff, "t::bool");   break;
		caseL SP(buff, "t::long");   break;
		caseS SP(buff, "t::symbol"); break;
		caseA SP(buff, "t::table");  break;
		caseN SP(buff, "t::dict");   break;
		default: SP(buff, "<unknown::%lld>",x); break;
	}
}

/* helper functions */

V getDictKey(V x) { R xV(0); }
V getDictVal(V x) { R xV(1); }
V getTableDict(V x, L k) { R xV(k); }



