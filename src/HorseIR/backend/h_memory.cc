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
	if(H_DEBUG)
		P("-> [Info heap] Successfully initialized\n");
}

G getHeapMem(L typ, L len){
	return allocMem(gHeap, &gHeapCur, INIT_HEAP_SIZE, typ, len);
}

G getCacheMem(L typ, L len){
	return allocMem(gCache, &gCacheCur, INIT_CACHE_SIZE, typ, len);
}

void printHeapInfo(){
	if(H_DEBUG)
		P("-> [Info heap] Init. %lld, used %lld (%lf%%)\n", \
			INIT_HEAP_SIZE, \
			gHeapCur, \
			gHeapCur*100.0/INIT_HEAP_SIZE);
}

G allocMem(G heap, L *cur, L top, L typ, L len){
	L size = getTypeSize(typ) * len;
	G g = NULL;
	if((*cur)+size < top){
		if(H_DEBUG){
			P("-> [Info heap] Allocated %lld for info (", size);
			printType(typ);
			P(", %2lld)\n",len);
		}
		g = heap + (*cur);
		*cur = (*cur) + size;
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
		case H_Q  : r = sizeof(L);     break;
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
	V x = allocNode();
	initV(x, typ, len);
	R x;
}

V allocNode(){
	R (V)getHeapMem(H_V, 1); //malloc(sizeof(V0));
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

/* initialization */

V initV(V x, L typ, L len){
	xp = typ;
	xn = len;
	xg = (len>1)?getHeapMem(typ, len):(G)&xb;
	R x;
}

V initDict(V x){
	R initV(x, H_N, 2);
}

V initSymbol(V x, L val){
	initV(x, H_Q, 1);
	xq = val;
	R x;
}

V initList(V x, L numItems){
	R initV(x, H_G, numItems);
}

V initValue(V x, L typ, L len){
	R initV(x,typ,len);
}

V initTable(V x, L numCols){
	R initV(x, H_A, numCols);
}


void printType(L x){
	C buff[128];
	printTypeStr(x, buff);
	P("%s",buff);
}

void printTypeStr(L x, S buff){
	switch(x){
		caseB SP(buff, "t::bool");    break;
		caseL SP(buff, "t::long");    break;
		caseQ SP(buff, "t::symbol");  break;
		caseA SP(buff, "t::table");   break;
		caseN SP(buff, "t::dict");    break;
		caseV SP(buff, "value_item"); break;
		default: SP(buff, "<unknown::%lld>",x); break;
	}
}

/* helper functions */

V getDictKey(V x) { R xV(0); }
V getDictVal(V x) { R xV(1); }
V getTableDict(V x, L k) { R xV(k); }


