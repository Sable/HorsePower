#include "../global.h"
#include "compile.h"
#include "fuse.h"

L opt = 0;
I qid = -1;

#define OPT_NONE (0==opt)
#define OPT_LF   (1==opt || 3==opt)
#define OPT_PH   (2==opt || 3==opt)

#include "loadcode.h" 

static E runOpt(int id){
    switch(id){
        case  1: R compiledQ1(); break;
        case  4: R compiledQ4(); break;
        case  6: R compiledQ6(); break;
        case 12: R compiledQ12(); break;
        case 13: R compiledQ13(); break;
        case 14: R compiledQ14(); break;
        case 16: R compiledQ16(); break;
        case 19: R compiledQ19(); break;
        case 22: R compiledQ22(); break;
        default: R -1;
    }
}

void run(int n, int id){
    E *record=(E*)malloc(sizeof(E)*n), total=0;
    initBackend();
    initTablesByQid(id);
    L cur = getHeapOffset();
    DOI(n, {setHeapOffset(cur); record[i]=runOpt(id);})
    DOI(n, total += record[i])
    P(">> Run with %d times, average (ms): %g |",n,total/n);
    DOI(n, P(" %g",record[i])) P("\n");
    free(record);
}

int main(int argc, char** argv){
    if(argc!=4){
        printf("[Usage]: %s <opt> <n> <id>\n",argv[0]);
        exit(1);
    }
    opt = atoi(argv[1]);
    int n  = atoi(argv[2]);
    int id = atoi(argv[3]);
    if(id<0 || id>22) { printf("[id]: must be >=0 and <=22\n"); exit(2); }
    if(n<=0){ printf("[n]: must be >0\n"); exit(3); }
    if(opt<0 || opt>3) { printf("0: not optimized; 1: lf; 2: peephole; 3: all\n"); exit(4); }
    P(">> Optimized HorseIR?: %s\n", 0==opt?"no":1==opt?"lf":2==opt?"peephole":"all");
    run(n,id);
    return 0;
}

