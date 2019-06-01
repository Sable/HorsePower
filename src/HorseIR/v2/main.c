#include "global.h"

Prog *root;
Node *entryMain;
int yyparse(); /* see y.tab.c */
extern FILE *yyin;
extern int yylineno;
B isReadBin  = false;

static void runInterpreterCore(){
    tic();
    HorseInterpreter(root);
    time_toc("Interpreter time (ms): %g ms\n", elapsed);
}

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
    time_toc("Parsing time (ms): %g\n", elapsed);
}

static void envInit(char *file){
    parseInput(file);
    initGlobal();
    weedProg(root);  // weed types/main
}

static void envInterpreter(char *file){
    envInit(file);
    buildSymbolTable(root);
    runInterpreterCore();
}

static void envCompiler(char *file){
    //envInit(file);
    //buildSymbolTable(root);
    //propagateTypeShape(root);
    ////buildUDChain(root);
    //runInterpreterCore();
}

static void envPrettyPrint(char *file){
    envInit(file);
    printProg(root);
}


int main(int argc, char *argv[]){
    if(argc != 2)
        EP("Usage: %s <file_path>\n", argv[0]);
    char *qfile = argv[1];
    //envPrettyPrint(qfile);
    envInterpreter(qfile);
    return 0;
}

