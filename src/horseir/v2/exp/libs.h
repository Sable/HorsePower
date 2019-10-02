#ifndef __H_C_LIB__
#define __H_C_LIB__

#include <float.h>  // DBL_MAX

//#define NO_PROFILE_INFO

//#define PROFILE(c, n, stmt) P("Line: %d\n",c); V n = allocNode(); stmt
#ifdef NO_PROFILE_INFO
    #define PROFILE(c, stmt) stmt
#else
    #define PROFILE(c,stmt) { my_tic(); \
        I e=stmt; if(e) printErrMsg(e); \
        E t0=my_toc(0); \
        if(t0>=0.1) P("[Profiling] Line %d: %g ms\n", c,t0); \
        else P("[Profiling] Line %d:\n", c); }
#endif

#define DOP_ACC(n, x, ...) {L i2=n; \
    _Pragma(STRINGIFY(acc parallel __VA_ARGS__)) \
    for(L i=0;i<i2;i++) x;}


static V incV(){
    if(buffS < MAX_V) return buffV[buffS++];
    else EP("Buff not available: %d\n", buffS);
}

#define HORSE_UDF(x) do{L cur=buffS; x; buffS=cur;}while(0)

#define LiteralSingle(T,t) {V z=incV(); initV(z, H_##T, 1); v##t(z)=x; R z;}
static V LiteralBool(B x) { LiteralSingle(B, b); }
static V LiteralI8  (J x) { LiteralSingle(J, j); }
static V LiteralI16 (H x) { LiteralSingle(H, h); }
static V LiteralI32 (I x) { LiteralSingle(I, i); }
static V LiteralI64 (L x) { LiteralSingle(L, l); }
static V LiteralF32 (F x) { LiteralSingle(F, f); }
static V LiteralF64 (E x) { LiteralSingle(E, e); }
static V LiteralSymbol(S s) {Q x=getSymbol(s); LiteralSingle(Q, q);}

#define LiteralVector(T) {V z=incV(); initV(z, H_##T, n); DOI(n, v##T(z,i) = x[i]) R z;}
static V LiteralVectorBool(I n, B *x) { LiteralVector(B); }
static V LiteralVectorI8  (I n, J *x) { LiteralVector(J); }
static V LiteralVectorI16 (I n, H *x) { LiteralVector(H); }
static V LiteralVectorI32 (I n, I *x) { LiteralVector(I); }
static V LiteralVectorI64 (I n, L *x) { LiteralVector(L); }
static V LiteralVectorF32 (I n, F *x) { LiteralVector(F); }
static V LiteralVectorF64 (I n, E *x) { LiteralVector(E); }


/* macros for fusion */

#ifdef LT
#undef LT
#endif

#ifdef EQ
#undef EQ
#endif

#define AND(x,y) (x)&&(y)
#define OR(x,y)  (x)||(y)
#define GEQ(x,y) (x)>=(y)
#define LEQ(x,y) (x)<=(y)
#define LT(x,y)  (x)<(y)
#define GT(x,y)  (x)>(y)
#define EQ(x,y)  (x)==(y)
#define NEQ(x,y) (x)!=(y)

#define MUL(x,y)   (x)*(y)
#define DIV(x,y)   (x)/(y)
#define MINUS(x,y) (x)-(y)
#define PLUS(x,y)  (x)+(y)
#define NOT(x)     !(x)

#endif
