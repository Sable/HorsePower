#pragma once

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

/* all types */

typedef bool       B;
typedef char       C,*S,*G;
typedef int16_t    H;
typedef int32_t    I,M,D,U,W,T;
typedef long long  L,Q,Z;
typedef float      F;
typedef double     E;
typedef struct { F real,imag; } X;
typedef struct { Q name; S target; } Y;  //target: V
typedef struct { I row, col; } A;

/* unsigned types */
typedef uint64_t UL;
typedef uint32_t UI;

/* size: 3 * 8 + 8 = 32 */
typedef struct node_value{
	L typ,len; G g;
	union{
		B b;   //boolean   1 byte
		H h;   //short     2 bytes
		I i32; //int       4 bytes
		L i64; //long      8 bytes
		F f32; //float     4 bytes
		E f64; //double    8 bytes
		C c;   //char      1 byte
		Q q;   //symbol    8 bytes
		S s;   //string    8 bytes
		M m;   //month     4 bytes
		D d;   //date      4 bytes
		Z z;   //date time 8 bytes
		U u;   //minute    4 bytes
		W w;   //second    4 bytes
		T t;   //time      4 bytes
		X x;   //complex   8 bytes
		Y y;   //enum      8 bytes
		A a;   //table     8 bytes
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

#define FT(s,...) FP(stdout,s,__VA_ARGS__)
#define FS(x)     FT("%s",x)

#define DOI(n, x) {for(L i=0,i2=n;i<i2;i++)x;}
#define DOJ(n, y) {for(L j=0,j2=n;j<j2;j++)y;}
#define DOK(n, z) {for(L k=0,k2=n;k<k2;k++)z;}

#define DOIa(n, x) {for(L i=1,i2=n;i<i2;i++)x;}

/* parallel DOI */
#define STRINGIFY(x) #x
#define DOP(n, x, ...) {L i2=n; \
	_Pragma(STRINGIFY(omp parallel for simd __VA_ARGS__)) \
	for(L i=0;i<i2;i++) x;}

#define DOT(n, x, ...) {L seg=(n)/H_CORE; \
	_Pragma(STRINGIFY(omp parallel __VA_ARGS__)) \
	{ \
		L tid = omp_get_thread_num(); \
		for(L i=tid*seg,i2=(tid!=H_CORE-1?(i+seg):(n));i<i2;i++) x; \
	} \
}


#define STRING_EMPTY(s) ((s)[0]!=0)
#define STRING_NONEMPTY(s) ((s)[0]!=0)


// #ifndef max
// #define max(a,b) ((a)>(b)?(a):(b))
// #endif

// #ifndef min
// #define min(a,b) ((a)>(b)?(b):(a))
// #endif

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)>(b)?(b):(a))

/* constant code */

#define ERROR_CODE 99
#define PCRE2_CODE_UNIT_WIDTH 8  // for pcre2.h
#define H_DEBUG false

/* extern */

extern G H_heap;
extern E H_EPSILON;
extern L H_CORE;
extern C LINE_SEP;

#ifdef	__cplusplus
}
#endif

/* include .h files */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <pcre2.h>
#include <sys/time.h>
#include <omp.h>
#include "h_memory.h"
#include "h_symbol.h"
#include "h_libs.h"
#include "h_io.h"
#include "h_system.h"
#include "h_primitive.h"
#include "test.h"
