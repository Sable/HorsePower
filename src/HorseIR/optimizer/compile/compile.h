#ifndef __H_COMPILE__
#define __H_COMPILE__

#include <float.h>  // DBL_MAX

//#define PROFILE(c, n, stmt) P("Line: %d\n",c); V n = allocNode(); stmt
//#define PROFILE(c, n, stmt) stmt
#define PROFILE(c,n,stmt) { struct timeval tt_0, tt_1; \
    gettimeofday(&tt_0, NULL); stmt; gettimeofday(&tt_1, NULL); \
    E t0=calcInterval(tt_0,tt_1); \
    P("[Profiling] Line %d: %g ms\n", c,t0>=0.1?t0:0); }

#define tic struct timeval tv0, tv1; gettimeofday(&tv0, NULL)
#define toc gettimeofday(&tv1, NULL); elapsed = calcInterval(tv0,tv1); P("The elapsed time (ms): %g\n\n", elapsed)

#define DOP_ACC(n, x, ...) {L i2=n; \
    _Pragma(STRINGIFY(acc parallel __VA_ARGS__)) \
    for(L i=0;i<i2;i++) x;}

#endif
