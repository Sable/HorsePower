#include "global.h"

Prog *root;
int yyparse(); /* see y.tab.c */
extern FILE *yyin;
extern int yylineno;

int main(int argc, char *argv[]){
    if(argc != 2){
        WP("Usage: %s <file_path>\n", argv[0]);
        exit(1);
    }
    char *file_path = argv[1];
    yylineno = 1;
    if(!(yyin=fopen(file_path, "r"))){
        EP("File not found: %s\n", file_path);
    }
    int ret = yyparse();
    fclose(yyin);
    if(ret == 0) P("Successfully parsed: %s!\n", file_path);
    else EP("Parsing failed: %s\n", file_path);
    // do sth with root
    //prettyProg(root);
    weedProg(root);
    return 0;
}

