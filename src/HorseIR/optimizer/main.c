#include "global.h"

Prog *root;
int yyparse(); /* see y.tab.c */
extern FILE *yyin;
extern ChainList *chain_list;
#define isCharNumber(c) ((c)>='0'&&(c)<='9')
#define version() P("HorseIR version: 0.1.1\n")

B isUsage   = false;
B isVersion = false;
B isQuery   = false;
B isOpt     = false;
B isCompiler= false;
B isInterp  = false;
B isExp     = false;
I runs      = 1;
I qid       = -1;
I qscale    = 1;
C delimiter = '|';
S optName   = NULL;

static void usage();
static int  getOption(int argc, char *argv[]);
static void parseInput(I qid);
static void runCompileWithOpt(char *optName);
static void runInterpreter(I qid);
static void runExperiment();
static void validateParameters();

/*
 * Compile with all optimizations on:
 *   ./a.out -c -q 6
 *
 * Compile with a specific optimization
 *   ./a.out -c -o lf -q 6
 *
 * Run with the interpreter
 *   ./a.out -t -q 6
*/
int main(int argc, char *argv[]){
    char *flagOpt=NULL;
    int r = getOption(argc, argv);
    if(r) {
        usage(); exit(1);
    }
    else if(isCompiler) {
        validateParameters();
        parseInput(qid);
        runCompileWithOpt(isOpt?optName:NULL);
    }
    else if(isInterp){
        validateParameters();
        parseInput(qid);
        runInterpreter(qid);
    }
    else if(isExp){
        runExperiment();
    }
    else if(isUsage)   { usage();   exit(1); }
    else if(isVersion) { version(); exit(1); }
    else {
        usage(); exit(1);
    }
    return 0;
}

static int getOption(int argc, char *argv[]){
    int c;
    while((c = getopt(argc, argv, "xhvtq:s:d:r:o:")) != -1){
        switch(c){
            case 'h': isUsage   = true; break;
            case 'v': isVersion = true; break;
            case 't': isInterp  = true; break;
            case 'q': isQuery   = true; \
                      qid       = atoi(optarg); break;
            case 's': qscale    = atoi(optarg); break;
            case 'c': isCompiler= true;         break;
            case 'o': isCompiler= true; \
                      isOpt     = true; \
                      optName   = strdup(optarg);break;
            case 'd': delimiter = optarg[0];     break;
            case 'r': runs      = atoi(optarg);  break;
            case 'x': isExp     = true;          break;
            default : return 1;
        }
    }
    return 0;
}

static void usage(){
    WP("Usage: ./horse <option> [parameter]  \n");
    WP("  -h           Print this information\n");
    WP("  -v           Print HorseIR version\n" );
    WP("  -t           Enable interpreter\n"    );
    WP("  -c           Enable compiler\n"       );
    WP("  -q <qid>     TPC-H query id\n"        );
    WP("  -s <sid>     TPC-H query scale\n"     );
    WP("  -o <name>    Query opt. [fe/fp]\n"    );
    WP("  -d 'del'     Set delimiter to del\n"  );
    WP("  -r <runs>    Number of runs\n"        );
}

static void parseInput(I qid){
    char file_path[128];
    struct timeval tv0, tv1;
    SP(file_path, "data/q%d.hir", qid);
    if(!(yyin=fopen(file_path, "r"))){
        EP("File not found: %s\n", file_path);
    }
    gettimeofday(&tv0, NULL);
    int ret = yyparse();
    fclose(yyin);
    if(ret == 0) P("Successfully parsed: %s!\n",file_path);
    else { EP("Parsing failed: %s\n",file_path);}
    gettimeofday(&tv1, NULL);
    WP("Parsing time: %s\n", calcInterval(tv0, tv1));
}

static void runCompileWithOpt(char *optName){
    struct timeval tv0, tv1;
    /* basics */
    prettyProg(root);
    initGlobal();
    gettimeofday(&tv0, NULL);
    buildUDChain(root); /* include type and shape propagation */
    if(H_DEBUG) printTypeShape();
    /* optimizations */
    if(optName){ /* check a single optimization */
        if(!strcmp(optName, "sr")){
            analyzeSR(root);
            prettyProg(root); /* verify */
        }
        else if(!strcmp(optName, "lf")){
            analyzeLF();
        }
        else if(!strcmp(optName, "ph")){
            analyzePeephole();
        }
        else { EP("opt %s is not supported\n",optName); }
    }
    else { /* all optimizations */
        analyzeLF();
        analyzePeephole();
        HorseCompiler(chain_list);
    }
    gettimeofday(&tv1, NULL);
    P("Compile time: %g ms\n", calcInterval(tv0, tv1));
}

static void runInterpreter(I qid){
    struct timeval tv0, tv1;
    initBackend();
    initTablesByQid(qid);
    gettimeofday(&tv0, NULL);
    HorseInterpreter(root);
    gettimeofday(&tv1, NULL);
    P("Interpreter time: %g ms\n", calcInterval(tv0, tv1));
}

static void validateParameters(){
    if(qid < 0 || qscale < 0){
        EP("Error found: qid %d, qscale %d\n", qid, qscale);
    }
}

static void serializeToFile(char *name){
    char temp[99];
    SP(temp, "temp/%s.bin",name);
    FILE *fp = fopen(temp, "w");
    V x = allocNode();
    pfnLoadTable(x, initLiteralSym(name));
    serializeV(x, fp);
    fclose(fp);
    P("done\n");
}

/*
 * ./a.out -x
 */
static void runExperiment(){
    P(">> Hello experiments\n");
    B isReadFromText = true;
    initBackend();
    V t0 = allocNode();
    struct timeval tv0, tv1;
    gettimeofday(&tv0, NULL);
    if(isReadFromText){
        initTableByName((S)"nation");
        initTableByName((S)"region");
        initTableByName((S)"supplier");
        initTableByName((S)"customer");
        initTableByName((S)"lineitem");
    }
    gettimeofday(&tv1, NULL);
    P("Reading time: %g ms\n", calcInterval(tv0, tv1));
    pfnLoadTable(t0, initLiteralSym((S)"customer"));
    printTablePretty(t0, 20);
    serializeToFile("customer");
    serializeToFile("region");
    serializeToFile("supplier");
    serializeToFile("lineitem");
}

