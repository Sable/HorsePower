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
B isW       = false;
B isPretty  = false;
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
        parseInput(qid);
        runCompileWithOpt(isOpt?optName:NULL);
    }
    else if(isInterp){
        parseInput(qid);
        runInterpreter(qid);
    }
    else if(isExp){
        runExperiment();
    }
    else if(isPretty){
        parseInput(qid);
        prettyProg(root);
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
    while((c = getopt(argc, argv, "hvtq:s:co:d:r:x:p")) != -1){
        switch(c){
            case 'h': isUsage   = true;            break;
            case 'v': isVersion = true;            break;
            case 't': isInterp  = true;            break;
            case 'q': isQuery   = true; \
                      qid       = atoi(optarg);    break;
            case 's': qscale    = atoi(optarg);    break;
            case 'c': isCompiler= true;            break;
            case 'o': isCompiler= true; \
                      isOpt     = true; \
                      optName   = strdup(optarg);  break;
            case 'd': delimiter = optarg[0];       break;
            case 'r': runs      = atoi(optarg);    break;
            case 'x': isExp     = true; \
                      isW       = 0==atoi(optarg); break;
            case 'p': isPretty  = true;            break;
            default : return 1;
        }
    }
    validateParameters();
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
    WP("  -d 'del'     CSV del. (default '|')\n");
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
    WP("Parsing time (ms): %g\n", calcInterval(tv0, tv1));
}

static void runCompileWithOpt(char *optName){
    struct timeval tv0, tv1;
    /* basics */
    //prettyProg(root);
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
        else if(!strcmp(optName, "fe")){ //lf -> fe
            analyzeLF();
        }
        else if(!strcmp(optName, "fp")){ //ph -> fp
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
    P("Interpreter time (ms): %g ms\n", calcInterval(tv0, tv1));
}

static void validateParameters(){
    if(isQuery){
        if(qid<0 || qid>22)
            EP("> qid must be [1,22], but %d found.\n", qid);
        if(!isInterp && !isCompiler && !isPretty)
            EP("> choose interpreter (-t), compiler (-c) or pretty printer (-p).\n");
    }
    if(qscale < 0){
        EP("> qscale must > 0, but %d found.\n", qscale);
    }
    if(runs < 0){
        EP("> # of runs must > 0, but %d found.\n", runs);
    }
    if(isInterp && isCompiler){
        EP("> choose one mode: interpreter (-t) or compiler (-c), not both.\n");
    }
    else if(isInterp || isCompiler){
        if(!isQuery){
            EP("> query must be specified (-q <id>).\n");
        }
    }
    if(isPretty && !isQuery){
        EP("> pretty printer needs a query specified (-q <id>).\n");
    }
}

static void serializeToFile(char *name){
    char temp[99];
    SP(temp, "temp/%s.bin",name);
    FILE *fp = fopen(temp, "wb");
    V x = allocNode();
    pfnLoadTable(x, initLiteralSym(name));
    serializeV(x, fp);
    fclose(fp);
    P("done\n");
}

/*
 * ./a.out -x 0/1
 */
static void runExperiment(){
    P(">> Hello experiments\n");
    B isReadFromText = isW;
    initBackend();
    V t0 = allocNode();
    struct timeval tv0, tv1;
    gettimeofday(&tv0, NULL);
    if(isReadFromText){ // total: 29376.3 ms
        initTableByName((S)"region");
        initTableByName((S)"nation");
        initTableByName((S)"supplier");
        initTableByName((S)"part");
        initTableByName((S)"customer");
        initTableByName((S)"partsupp");
        initTableByName((S)"orders");
        initTableByName((S)"lineitem");
    }
    else { // total: 14142.6 ms
        initTableFromBin((S)"region");
        initTableFromBin((S)"nation");
        initTableFromBin((S)"supplier");
        initTableFromBin((S)"part");
        initTableFromBin((S)"customer");
        initTableFromBin((S)"partsupp");
        initTableFromBin((S)"orders");
        initTableFromBin((S)"lineitem");
    }
    gettimeofday(&tv1, NULL);
    P("Reading time (ms): %g ms\n", calcInterval(tv0, tv1));
    pfnLoadTable(t0, initLiteralSym((S)"lineitem"));
    printTablePretty(t0, 10);
    if(isReadFromText){
        serializeToFile("region");
        serializeToFile("nation");
        serializeToFile("supplier");
        serializeToFile("part");
        serializeToFile("customer");
        serializeToFile("partsupp");
        serializeToFile("orders");
        serializeToFile("lineitem");
    }
}

