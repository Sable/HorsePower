#include "../global.h"
#include "compile.h"
#include "fuse.h"
#include "loadcode.h"

I qid = -1;

E runSingle(int id){
    switch(id){
        case 6: R compiledQ6(); break;
        default: R -1;
    }
}

void run(int n, int id){
    E *record=(E*)malloc(sizeof(E)*n), total=0;
    initBackend();
    initTablesByQid(id);
    DOI(n, record[i]=runSingle(id))
    DOI(n, total += record[i])
    P(">> Run with %d times, average (ms): %g |",n,total/n);
    DOI(n, P(" %g",record[i])) P("\n");
    free(record);
}

int main(int argc, char** argv){
    if(argc!=3){
        printf("[Usage]: ./run <n> <id>\n");
        exit(1);
    }
    int n = atoi(argv[1]);
    int id = atoi(argv[2]);
    if(id<0 || id>22) { printf("[id]: must be >=0 and <=22\n"); exit(2); }
    if(n<=0){ printf("[n]: must be >0\n"); exit(3); }
    run(n,id);
    return 0;
}

