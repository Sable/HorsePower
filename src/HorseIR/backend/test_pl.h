
#include "pl/blackscholes.h"
#include "pl/pagerank.h"

void testPL(L x, L sid){
    switch(x){
        case 0: runBS(sid); break;
        case 1: runPR(sid); break;
        default: P("No such test for pl %lld yet\n",x); break;
    }
}
