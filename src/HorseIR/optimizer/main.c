#include "global.h"

Prog *root;
int yyparse(); /* see y.tab.c */
extern FILE *yyin;

/*
 * ./program filename
*/
int main(int argc, char *argv[]){
    char *flagOpt;
    if(argc == 3){
        if(!(yyin=fopen(argv[1], "r"))){
            EP("File %s not found\n", argv[1]);
        }
        flagOpt = argv[2];
    }
    else { EP("Usage: %s file opt_flag\n",argv[0]); }
    int ret = yyparse();
    fclose(yyin);
    if(ret == 0) P("%s is parsed successfully!\n",argv[1]);
    else { EP("%s fails in parsing\n",argv[1]);}
    /* basics */
    prettyProg(root);
    initGlobal();
    buildUDChain(root); /* include type and shape propagation */
    printTypeShape();
    /* optimizations */
    if(!strcmp(flagOpt, "sr")){
        analyzeSR(root);
        prettyProg(root); /* verify */
    }
    else if(!strcmp(flagOpt, "lf")){
        analyzeLF();
    }
    else { EP("opt %s is not supported\n",flagOpt); }
    return 0;
}

