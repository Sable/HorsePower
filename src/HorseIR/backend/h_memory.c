#include "h_global.h"

/* global vars */

G H_heap = NULL;

/* methods */

G h_malloc(H_T typ, L n){
	G r = (G)malloc(h_sizeof(typ) * n);
	if(!r) {
		P("error\n");
		exit(99);
	}
	return r;
}

L h_sizeof(H_T typ){
	L r = -1;
	switch(typ){
		case H_bool   : r = sizeof(B);     break;
		case H_char   : r = sizeof(C) + 1; break;
		case H_int    : r = sizeof(I);     break;
		case H_float  : r = sizeof(F);     break;
		case H_double : r = sizeof(E);     break;
		case H_complex: r = sizeof(X) * 2; break;
	}
	return r;
}
