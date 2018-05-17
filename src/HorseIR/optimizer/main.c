#include "global.h"

Prog *root;
int yyparse(); /* see y.tab.c */
extern FILE *yyin;

// ./program filename
int main(int argc, char *argv[]){
    if(argc == 2){
        if(!(yyin=fopen(argv[1], "r"))){
            fprintf(stderr, "File %s not found\n", argv[1]);
            exit(1);
        }
    }
    else {
        fprintf(stderr, "Usage:./run filename\n");
        exit(1);
    }
    int ret = yyparse();
    fclose(yyin);
    if(ret == 0)
        printf("%s is parsed successfully!\n",argv[1]);
    else
        { printf("%s fails in parsing\n"); exit(1); }
    printf("** Pretty printer **\n");
    prettyProg(root);
    initGlobal();
    buildUDChain(root);
    //printTrieType();
    //analyzeSR(root);
    analyzeLF();
    return 0;
}
