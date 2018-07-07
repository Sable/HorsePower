#ifndef __H_COMPILE__
#define __H_COMPILE__

//#define PROFILE(c, n, stmt) P("Line: %d\n",c); V n = allocNode(); stmt
#define PROFILE(c, n, stmt) stmt
//#define PROFILE(c,n,stmt) { struct timeval tt_0, tt_1; \
//            gettimeofday(&tt_0, NULL); stmt; gettimeofday(&tt_1, NULL); \
//            P("[Profiling] Line %d: %g ms\n", c,calcInterval(tt_0,tt_1)); }

#define tic struct timeval tv0, tv1; gettimeofday(&tv0, NULL)
#define toc gettimeofday(&tv1, NULL); elapsed = calcInterval(tv0,tv1); P("The elapsed time (ms): %g\n\n", elapsed)

#endif
