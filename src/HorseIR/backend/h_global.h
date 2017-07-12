#pragma once

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdbool.h>

/* all types */

typedef bool       B;
typedef char       C,*S,*G;
typedef short      H;
typedef int        I;
typedef long long  L,Y,M,D,Z,U,W,T;
typedef float      F;
typedef double     E;
typedef struct { E real,imag; } X;

typedef struct node_value{
	L typ,len;
	union{
		B b;   //bool
		H h;   //short
		I i32; //int
		L i64; //long
		F f32; //float
		E f64; //double
		C c;   //char
		L s;   //symbol
		Y y;   //year
		M m;   //month
		D d;   //date
		Z z;   //date time
		U u;   //minute
		W w;   //second
		T t;   //time
		X x;   //complex
		S g;
	};
}V0,*V;

typedef struct list_value_node{
	V value;
	struct list_value_node *next;
}ListV0,*ListV;

typedef struct list_table{
	L sid;
	V table;
}ListT0, *ListT;

#include "h_global_type.h"

/* macros */

#define P printf
#define R return
#define FP fprintf
#define SP sprintf

#define DOI(n, x) {for(L i=0,i2=n;i<i2;i++)x;}
#define DOJ(n, y) {for(L j=0,j2=n;j<j2;j++)y;}
#define DOK(n, z) {for(L k=0,k2=n;k<k2;k++)z;}

#define STRING_EMPTY(s) ((s)[0]!=0)
#define STRING_NONEMPTY(s) ((s)[0]!=0)

/* constant code */
#define ERROR_CODE 99

/* extern */

extern G H_heap;

#ifdef	__cplusplus
}
#endif

/* include .h files */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include "h_memory.h"
#include "h_symbol.h"
#include "h_io.h"
#include "h_system.h"
#include "h_primitive.h"
#include "test.h"
