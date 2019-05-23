#include "global.h"

Prog *root;
int yyparse(); /* see y.tab.c */
extern FILE *yyin;
extern int yylineno;

//static void runInterpreterCore(){
//    tic();
//    HorseInterpreter(root);
//    toc("Interpreter time (ms): %g ms\n", elapsed);
//}

static void parseInput(char *file_path){
    yylineno = 1;
    if(!(yyin=fopen(file_path, "r"))){
        EP("File not found: %s\n", file_path);
    }
    WP("parsing ....\n");
    tic();
    int ret = yyparse();
    fclose(yyin);
    if(ret == 0) WP("Successfully parsed: %s!\n",file_path);
    else { EP("Parsing failed: %s\n",file_path);}
    toc("Parsing time (ms): %g\n", elapsed);
}

int main(int argc, char *argv[]){
    if(argc != 2){
        WP("Usage: %s <file_path>\n", argv[0]);
        exit(1);
    }
    char *qfile = argv[1];
    parseInput(qfile);
    // do sth with root
    initGlobal();
    //weedProg(root);
    createSymbolTable(root);
    //printProg(root);
    //buildUDChain(root);
    //runInterpreterCore();
    return 0;
}

