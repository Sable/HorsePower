#include "global.h"

Prog *root;
Node *entryMain;
int yyparse(); /* see y.tab.c */
extern FILE *yyin;
extern int yylineno;

static void runInterpreterCore(){
    tic();
    HorseInterpreter();
    time_toc("Interpretation time (ms): %g\n", elapsed);
}

static void runCompilerCore(){
    tic();
    if(numOpts < 1)
        HorseCompilerNaive();     // compiler.c
    else
        HorseCompilerOptimized(); // optimizer.c
    time_toc("Compile time (ms): %g\n", elapsed);
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
    envInit(file);
    buildSymbolTable(root);
    propagateTypeShape(root, false);
    runCompilerCore();
}

static void envVersion(){
    P("HorseIR version: 0.2.1\n");
}

static void utlPrinter(char *qItem, char *file){
    envInit(file);
    if(sEQ(qItem, "pretty")){
        printProg(root);
    }
    else if(sEQ(qItem, "dot")){
        buildSymbolTable(root);
        propagateTypeShape(root, false);
        buildUDChain(root);
        dotProg(root);
    }
    else if(sEQ(qItem, "symboltable")){
        buildSymbolTable(root);
    }
    else if(sEQ(qItem, "typeshape")){
        buildSymbolTable(root);
        propagateTypeShape(root, true);
    }
    else EP("Unknown item for printer: %s", qItem);
}

static void utlStats(char *qStats){
    if(sEQ(qStats, "dump")) dumpStats();
    else if(sEQ(qStats, "load")) loadStats();
    else EP("Unknown cmd for stats printer: %s", qStats);
}

static void envUtility(OptionUtility opt){
    switch(opt){
        case   StatsU: utlStats   (qCmd); break;
        case PrinterU: utlPrinter (qCmd, qPath); break;
        default: EP("Utility function unknown");
    }
}

int main(int argc, char *argv[]){
    int r = getLongOption(argc, argv);
    if(r) BAD_TRY();
    else {
        switch(optMode){
            case InterpNaiveM: envInterpreter(qPath); break;
            case    CompilerM: envCompiler(qPath);    break;
            case   InterpJITM: TODO("interpreter jit mode"); break;
            case  ExperimentM: TODO("experiment");  break;
            case     VersionM: envVersion();break;
            case     UtilityM: envUtility(optUtl);   break;
            case      HelperM: GOOD_TRY();  break;
            case     UnknownM:  BAD_TRY();  break;
            default: EP("Option mode not supported: %d", optMode);
        }
    }
    return 0;
}

