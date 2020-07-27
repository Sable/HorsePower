#ifndef __B_COMMON__
#define __B_COMMON__

#ifdef  __cplusplus
extern "C" {
#endif

/* all types */ 
typedef bool       B;
typedef char       C,*S,*G;
typedef int8_t     J;
typedef int16_t    H;
typedef int32_t    I,M,D,U,W,T;
typedef long long  L,Q,Z;
typedef float      F;
typedef double     E;
typedef void       O;
typedef struct { F real,imag; } X;
typedef struct { Q source; G ek,ev; } Y0, *Y;  //key,value: V // Q source
typedef struct ListY { G y,cn;  struct ListY *next; } ListY; // cn: column name 
typedef struct { L row, col; ListY *fkey; } A0,*A;
typedef const char *CS;  // constant string

/* unsigned types */
typedef uint64_t UL;
typedef uint32_t UI;

#define LL I

/* size: 4 + 2 * 8 + 16 = 36 */
typedef struct NodeValue {
    L len; G g; I typ;
    union{
        B b;   //boolean   1  byte
        J j;   //small     1  byte
        H h;   //short     2  bytes
        I i32; //int       4  bytes
        L i64; //long      8  bytes
        F f32; //float     4  bytes
        E f64; //double    8  bytes
        C c;   //char      1  byte
        Q q;   //symbol    8  bytes
        S s;   //string    8  bytes
        M m;   //month     4  bytes
        D d;   //date      4  bytes
        Z z;   //date time 8  bytes
        U u;   //minute    4  bytes
        W w;   //second    4  bytes
        T t;   //time      4  bytes
        X x;   //complex   8  bytes
        Y y;   //enum      8  bytes
        A a;   //table     16 bytes
        struct NodeValue *g2;
    };
}V0,*V;

typedef struct ValueList{
    V v;
    struct ValueList *next;
}VList;

typedef struct list_value_node{
    V value;
    struct list_value_node *next;
}ListV0,*ListV;

typedef struct list_table{
    L sid;
    V table;
}ListT0, *ListT;

typedef enum TargetCode{
    TARGET_NA,
    TARGET_C, TARGET_LLVM,
    TARGET_ACC, TARGET_CUDA, TARGET_CL
}TC;

#include "h_type.h"

/* macros */ 
#define DOI(n, x) {for(L i=0,i2=n;i<i2;i++)x;}
#define DOJ(n, y) {for(L j=0,j2=n;j<j2;j++)y;}
#define DOK(n, z) {for(L k=0,k2=n;k<k2;k++)z;} 
#define DOI3(m, n, x) {for(L i=m,i2=n;i<i2;i++)x;}
#define DOJ3(m, n, x) {for(L j=m,j2=n;j<j2;j++)x;}

#define DOIr(n, x) {for(L i=n;i>=0;i--)x;}
#define DOIa(n, x) {for(L i=1,i2=n;i<i2;i++)x;}
#define DOJa(n, x) {for(L j=1,j2=n;j<j2;j++)x;}

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

#define DOTa(n, x, ...) {L seg=(n)/H_CORE; \
    _Pragma(STRINGIFY(omp parallel __VA_ARGS__)) \
    { \
        L tid = omp_get_thread_num(); \
        L sid = tid * seg; \
        L slen= (tid!=H_CORE-1)?seg:(n-sid); \
        x; \
    } \
}  

#define DOTb(n, x, ...) {L seg=(n)/H_CORE; \
    _Pragma(STRINGIFY(omp parallel __VA_ARGS__)) \
    { \
        L tid = omp_get_thread_num(); \
        for(L i=tid*seg+1,i2=(tid!=H_CORE-1?((tid+1)*seg):(n));i<i2;i++) x;\
    } \
}

#define DOTc(n, x, y, ...) {L seg=(n)/H_CORE; \
    _Pragma(STRINGIFY(omp parallel __VA_ARGS__)) \
    { \
        L tid = omp_get_thread_num(); \
        L pos = tid*seg; \
        x; \
        for(L i=pos+1,i2=(tid!=H_CORE-1?(pos+seg):(n));i<i2;i++) y;\
    } \
}

#define STRING_EMPTY(s) ((s)[0]!=0)
#define STRING_NONEMPTY(s) ((s)[0]!=0)

/* constant */

#define PCRE2_CODE_UNIT_WIDTH 8  // for pcre2.h
#define H_DEBUG false


/* extern */

extern G  H_heap;
extern E  H_EPSILON;
extern L  H_CORE;
extern C  LINE_SEP;
extern B  LINE_HEADER;
extern TC H_TARGET;
extern I  join_id, group_id; // for debugging

#ifdef __cplusplus
}
#endif

/* include .h files */
#include <pcre2.h>
#include <omp.h>
#include "h_memory.h"
#include "h_symbol.h"
#include "h_libs.h"
#include "h_io.h"
#include "h_system.h"
#include "h_hash.h"
#include "h_primitive.h"
#include "load_db.h"

/* common.c */
V initLiteralDate   (L x);
V initLiteralBool   (L x);
V initLiteralI64    (L x);
V initLiteralString (S s);
V initLiteralSym    (S str);
V initLiteralF64    (E x);
V initLiteralChar   (C x);
V initLiteralSymVector (L n, S strs[]);
V initLiteralBoolVector(L n, B b[]);
V initLiteralI32Vector (L n, I b[]);
V initLiteralI64Vector (L n, L b[]);
V initLiteralStrVector (L n, S b[]);

/* heuristics */
#define NUM_JOIN_LINEAR 10   // linear scan (<) or hash (>=)

#endif

