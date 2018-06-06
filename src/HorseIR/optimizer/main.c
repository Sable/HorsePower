#include "global.h"

Prog *root;
int yyparse(); /* see y.tab.c */
extern FILE *yyin;
char file_path[128];
#define isCharNumber(c) ((c)>='0'&&(c)<='9')

/*
 * ./program filename
*/
int main(int argc, char *argv[]){
    char *flagOpt=NULL;
    I qid = -1;
    if(argc == 2 || argc == 3){
        if(isCharNumber(argv[1][0])) { qid=atoi(argv[1]);  SP(file_path, "data/q%d.hir", qid);}
        else strcpy(file_path, argv[1]);
        if(!(yyin=fopen(file_path, "r"))){
            EP("File %s not found\n", file_path);
        }
        if(argc == 3) flagOpt = argv[2];
    }
    else { EP("Usage: %s file [sr/lf]\n",argv[0]); }
    double parseTime = 0, exeTime = 0, optTime = 0;
    struct timeval tv0, tv1;
    gettimeofday(&tv0, NULL);
    int ret = yyparse();
    fclose(yyin);
    if(ret == 0) P("%s is parsed successfully!\n",argv[1]);
    else { EP("%s fails in parsing\n",argv[1]);}
    gettimeofday(&tv1, NULL);
    parseTime = calcInterval(tv0, tv1)/1000.0;
    if(flagOpt){
        /* basics */
        prettyProg(root);
        initGlobal();
        gettimeofday(&tv0, NULL);
        buildUDChain(root); /* include type and shape propagation */
        if(H_DEBUG) printTypeShape();
        /* optimizations */
        if(!strcmp(flagOpt, "sr")){
            analyzeSR(root);
            prettyProg(root); /* verify */
        }
        else if(!strcmp(flagOpt, "lf")){
            analyzeLF();
        }
        else { EP("opt %s is not supported\n",flagOpt); }
        gettimeofday(&tv1, NULL);
        optTime = calcInterval(tv0, tv1)/1000.0;
        P("Optimizing time: %g ms\n", optTime);
    }
    else {
        initBackend();
        initTablesByQid(qid);
        gettimeofday(&tv0, NULL);
        HorseInterpreter(root);
        gettimeofday(&tv1, NULL);
        exeTime = calcInterval(tv0, tv1)/1000.0;
        P("Parsing time: %g ms; Execution time: %g ms\n", parseTime, exeTime);
    }
    //P("Complete time: %g ms\n", calcInterval(tv0, tv1)/1000.0);
    return 0;
}

