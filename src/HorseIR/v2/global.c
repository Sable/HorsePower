#include "global.h"

/* helper functions */

void *newM(int n){
    void *x = malloc(n);
    memset(x, 0, n);
    if(!x){ error("Memory allocation fails"); }
    return x;
}

void error(const char *s){
    fprintf(stderr,"%s\n", s);
    exit(1);
}

#ifndef my_tic

#define TIMEVAL_N 10
static int timeSlotId = 0;
struct timeval tv0a[TIMEVAL_N], tv0b[TIMEVAL_N];

static void time_check(){
    if(timeSlotId >= TIMEVAL_N || timeSlotId < 0){
        EP("No sufficient lots for timeval: %d >= %d or a negative num", \
                timeSlotId, TIMEVAL_N);
    }
}

static L calcIntervalPrecise(struct timeval t0, struct timeval t1){
    return (t1.tv_sec-t0.tv_sec)*1000000 + t1.tv_usec-t0.tv_usec;
}

// return: ms (Linux/MacOS)
static double calcInterval(struct timeval t0, struct timeval t1){
    return calcIntervalPrecise(t0, t1) / 1000.0;
}

void my_tic(){
    time_check();
    gettimeofday(&tv0a[timeSlotId++], NULL);
}

double my_toc(bool isPrint){
    timeSlotId--; time_check(); 
    gettimeofday(&tv0b[timeSlotId], NULL);
    double elapsed = calcInterval(tv0a[timeSlotId], tv0b[timeSlotId]);
    if(isPrint)
        P("[%d] The elapsed time (ms): %g\n\n", timeSlotId, elapsed);
    return elapsed;
}

void time_clear(){
    timeSlotId = 0;
}
#endif


void initGlobal(){
    initUDChain();
    checkFuncNumber();
}


