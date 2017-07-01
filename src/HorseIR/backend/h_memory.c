#include "h_global.h"

/* global vars */

G H_heap = NULL;

/* methods */

G hMalloc(H_T typ, L n){
	G r = (G)malloc(getTypeSize(typ) * n);
	if(!r) {
		P("error\n");
		exit(99);
	}
	return r;
}

L getTypeSize(H_T typ){
	L r = -1;
	switch(typ){
		case H_bool    : r = sizeof(B);     break;
		case H_char    : r = sizeof(C) + 1; break;
		case H_short   : r = sizeof(H);     break;
		case H_int     : r = sizeof(I);     break;
		case H_long    : r = sizeof(L);     break;
		case H_float   : r = sizeof(F);     break;
		case H_double  : r = sizeof(E);     break;
		case H_complex : r = sizeof(X);     break;
		case H_symbol  : r = sizeof(L);     break;
		case H_month   : r = sizeof(M);     break;
		case H_date    : r = sizeof(D);     break;
		case H_datetime: r = sizeof(Z);     break;
		case H_minute  : r = sizeof(U);     break;
		case H_second  : r = sizeof(W);     break;
		case H_time    : r = sizeof(T);     break;
	}
	return r;
}
