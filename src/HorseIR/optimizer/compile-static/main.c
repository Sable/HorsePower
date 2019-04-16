#include "../global.h"
#include "compile.h"
#include "fuse.h"

L opt = 0;
I qid = -1;
B isReadBin = true; /* reading from binary or text files */
extern L CSV_FILE_SCALE;

#define OPT_NONE (0==opt)
#define OPT_FE   (1==opt || 3==opt)
#define OPT_FP   (2==opt || 3==opt)

#define OPT_LF   OPT_FE
#define OPT_PH   OPT_FP

#include "loadcode.h" 

void run(int n, int id){
    E *record=(E*)malloc(sizeof(E)*n), total=0;
    initBackend();
    initTablesByQid(id); // load all tables
    L cur = getHeapOffset();
    DOI(n, {setHeapOffset(cur); time_clear(); record[i]=compiled_main();})
    DOI(n, total += record[i])
    P("q%02d>> Run with %d times, average (ms): %g |",id,n,total/n);
    DOI(n, P(" %g",record[i])) P("\n");
    free(record);
}

int getId(char* str){
    I len=strlen(str), x=len-1;
    while(x>=0 && str[x]!='q') x--;
    if(x>=0) return atoi(str+x+1);
    else {
        EP("Not a valid executable: %s, but horse-compile-q<id> is expected\n", str);
    }
}

int main(int argc, char** argv){
    if(argc!=4){
        printf("[Usage]: %s <scale> <opt> <n>\n",argv[0]);
        exit(1);
    }
    L scale= atoi(argv[1]);
    opt    = atoi(argv[2]);
    int n  = atoi(argv[3]);
    int id = getId(argv[0]);
    if(n<=0){ EP("[n]: must be >0\n"); }
    if(opt<0 || opt>3) { EP("0: not optimized; 1: lf; 2: peephole; 3: all\n"); }
    if(scale != 1 && scale != 2 && scale!=4 && scale!=8 && scale != 16) { EP("scale must be one of 1/2/4/8/16\n"); }
    P(">> Optimized HorseIR?: %s\n", 0==opt?"no":1==opt?"lf":2==opt?"peephole":"all");
    CSV_FILE_SCALE = scale;
    run(n, id);
    return 0;
}

