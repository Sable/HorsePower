#include "global.h"

Prog *root;
Node *entryMain;
I yyparse(); /* see y.tab.c */
extern FILE *yyin;
extern I yylineno;

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
    time_toc("Compiling time (ms): %g\n", elapsed);
}

static void runCompilerJITCore(){
    tic();
    if(numOpts < 1)
        HorseCompilerJITNaive();
    else
        HorseCompilerJITOptimized();
    time_toc("LLVM JIT time (ms): %g\n", elapsed);
}

static void parseInput(S file_path){
    yylineno = 1;
    if(!(yyin=fopen(file_path, "r"))){
        EP("File not found: %s", file_path);
    }
    tic();
    I ret = yyparse();
    fclose(yyin);
    if(ret == 0)
        WP("Successfully parsed: %s!\n",file_path);
    else
        EP("Parsing failed: %s",file_path);
    time_toc("Parsing time (ms): %g\n", elapsed);
}

static void envInit(S file){
    initGlobal();
    parseInput(file);
    weedProg(root);  // weed types/main
}

static void envInterpreter(S file){
    envInit(file);
    buildSymbolTable(root);
    runInterpreterCore();
}

static void envCompiler(S file){
    envInit(file);
    buildSymbolTable(root);
    propagateTypeShape(root, false);
    switch(qTarg){
        case TARGET_C   : runCompilerCore();    break;
        case TARGET_LLVM: runCompilerJITCore(); break;
        default: TODO("Add support: %d", qTarg);
    }
}

static void envInitDot(){
    buildSymbolTable(root);
    propagateTypeShape(root, false);
    buildUDChain(root);
}

static void envVersion(){
    P("HorseIR version: 0.3.0\n");
}

static void utlPrinter(S qItem, S file){
    envInit(file);
    if(sEQ(qItem, "pretty")){
        printProg(root);
    }
    else if(sEQ(qItem, "dot")){
        envInitDot();
        dotProg();
    }
    else if(sEQ(qItem, "mermaid")){
        envInitDot();
        dotProgMermaid();
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

static void utlStats(S qStats){
    if(sEQ(qStats, "dump"))
        dumpStats();
    else if(sEQ(qStats, "load"))
        loadStats();
    else
        EP("Unknown cmd for stats printer: %s", qStats);
}

static void envUtility(OptionUtility opt){
    switch(opt){
        case   StatsU: utlStats(qCmd); break;
        case PrinterU: utlPrinter(qCmd, qPath); break;
        default: EP("Utility function unknown");
    }
}

I main(I argc, S argv[]){
    I r = getLongOption(argc, argv);
    if(r) BAD_TRY();
    else {
        switch(optMode){
            case InterpNaiveM: envInterpreter(qPath);break;
            case    CompilerM: envCompiler(qPath);   break;
            case     VersionM: envVersion();         break;
            case     UtilityM: envUtility(optUtl);   break;
            case      HelperM: GOOD_TRY();           break;
            case     UnknownM:  BAD_TRY();           break;
            default: EP("Option mode not supported: %d", optMode);
        }
    }
    R 0;
}

