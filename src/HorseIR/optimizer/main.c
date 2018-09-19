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
B isQFile   = false;
B isOpt     = false;
B isCompiler= false;
B isInterp  = false;
B isExp     = false;
B isFront   = false;
B isW       = false;
B isPretty  = false;
B isServer  = false;
B isClient  = false;
I runs      = 1;
I qid       = -1;
I qscale    = 1;
C delimiter = '|';
S optName   = NULL;
S qfile     = NULL;

static void usage();
static int  getOption(int argc, char *argv[]);
static void parseInputWithQid(I qid);
static void parseInput(S file_path);
static void runCompileWithOpt(char *optName);
static void runInterpreterWithQid(I qid);
static void runInterpreter();
static void runInterpreterCore();
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
        if(isQuery){
            parseInputWithQid(qid);
            runCompileWithOpt(isOpt?optName:NULL);
        }
        else if(isQFile){
            EP("TODO: -t + -n <filename>");
        }
    }
    else if(isInterp){
        if(isQuery){
            parseInputWithQid(qid);
            runInterpreterWithQid(qid);
        }
        else if(isQFile){
            parseInput(qfile);
            runInterpreter();
        }
    }
    else if(isExp){
        runExperiment();
    }
    else if(isFront){
        if(isQuery) parseInputWithQid(qid);
        else if(isQFile) parseInput(qfile);
    }
    else if(isPretty){
        parseInputWithQid(qid);
        prettyProg(root);
    }
    else if(isServer){
        runModeServer();
    }
    else if(isClient){
        runModeClient();
    }
    else if(isUsage)   { usage();   exit(1); }
    else if(isVersion) { version(); exit(1); }
    else {
        usage(); exit(1);
    }
    return 0;
}

// TODO: getopt_long
static int getOption(int argc, char *argv[]){
    int c;
    while((c = getopt(argc, argv, "hvtq:n:s:co:d:r:x:fpz:")) != -1){
        switch(c){
            case 'h': isUsage   = true;            break;
            case 'v': isVersion = true;            break;
            case 't': isInterp  = true;            break;
            case 'q': isQuery   = true; \
                      qid       = atoi(optarg);    break;
            case 'n': isQFile   = true; \
                      qfile     = strdup(optarg);  break;
            case 's': qscale    = atoi(optarg);    break;
            case 'c': isCompiler= true;            break;
            case 'o': isCompiler= true; \
                      isOpt     = true; \
                      optName   = strdup(optarg);  break;
            case 'd': delimiter = optarg[0];       break;
            case 'r': runs      = atoi(optarg);    break;
            case 'x': isExp     = true; \
                      isW       = 0==atoi(optarg); break;
            case 'f': isFront   = true;            break;
            case 'p': isPretty  = true;            break;
            case 'z': isServer  = 0==atoi(optarg);
                      isClient  = 1==atoi(optarg); break;
            default : return 1;
        }
    }
    validateParameters();
    return 0;
}

static void usage(){
#define usage_q() WP("  -q <qid>       TPC-H query id `data/` (-q or -n)\n")
#define usage_n() WP("  -n <filename>  Set a query file (-q or -n)\n"      )
    WP("Usage: ./horse <option> [parameter]  \n");
    WP("\nRun with an interpreter:\n");
    WP("  -t             Enable interpreter\n"  );
    usage_q();
    usage_n();
    WP("  -s <sid>       TPC-H query scale (default 1)\n"    );
    WP("  -r <runs>      Number of runs (default 1)\n"       );
    WP("\nRun with a compiler:\n");
    WP("  -c             Enable compiler\n"          );
    usage_q();
    usage_n();
    WP("  -o <name>      Query opt. (fe or fp)\n"    );
    WP("\nRun with a pretty printer:\n");
    WP("  -p             Enable pretty printer\n"    );
    usage_q();
    usage_n();
    WP("\nRun with server/client:\n");
    WP("  -z 0/1         Enable server/client mode\n");
    WP("\nOthers:\n");
    WP("  -h             Print this information\n");
    WP("  -v             Print HorseIR version\n" );
    WP("  -d 'del'       CSV del. (default '|')\n");
}

static void parseInputWithQid(I qid){
    char file_path[128];
    SP(file_path, "data/hand-q%d.hir", qid);
    //SP(file_path, "data/q%d.hir", qid);
    parseInput(file_path);
}

static void parseInput(S file_path){
    struct timeval tv0, tv1;
    if(!(yyin=fopen(file_path, "r"))){
        EP("File not found: %s\n", file_path);
    }
    WP("parsing ....\n");
    gettimeofday(&tv0, NULL);
    int ret = yyparse();
    fclose(yyin);
    if(ret == 0) WP("Successfully parsed: %s!\n",file_path);
    else { EP("Parsing failed: %s\n",file_path);}
    gettimeofday(&tv1, NULL);
    WP("Parsing time (ms): %g\n", calcInterval(tv0, tv1));
    weedProg(root);
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
    WP("Compile time: %g ms\n", calcInterval(tv0, tv1));
}

static void runInterpreterWithQid(I qid){
    initBackend();
    initTablesByQid(qid);
    runInterpreterCore();
}

static void runInterpreter(){
    initBackend();
    runInterpreterCore();
}

static void runInterpreterCore(){
    struct timeval tv0, tv1;
    gettimeofday(&tv0, NULL);
    HorseInterpreter(root);
    gettimeofday(&tv1, NULL);
    P("Interpreter time (ms): %g ms\n", calcInterval(tv0, tv1));
}

static void validateParameters(){
    if(isFront || isExp) return; //skip on tests
    if(isInterp && isCompiler){
        EP("> choose one mode: interpreter (-t) or compiler (-c), not both.\n");
    }
    else if(isInterp || isCompiler){
        if(!isQuery && !isQFile){
            EP("> query must be specified (-q <id> or -n <filename>).\n");
        }
    }
    if(isQuery){
        if(qid<0 || qid>22)
            EP("> qid must be [1,22], but %d found.\n", qid);
        if(isQFile)
            EP("> choose either '-q <qid>' or '-n <filename>'\n");
        if(!isInterp && !isCompiler && !isPretty)
            EP("> choose interpreter (-t), compiler (-c) or pretty printer (-p).\n");
        if(qscale < 0){
            EP("> qscale must > 0, but %d found.\n", qscale);
        }
    }
    else if(isQFile){
        if(!isInterp && !isCompiler && !isPretty)
            EP("> choose interpreter (-t), compiler (-c) or pretty printer (-p).\n");
    }
    else { // isQuery: false; isQFile: false
        if(isPretty) {
            EP("> pretty printer needs a query specified (-q <id> or -n <filename>).\n");
        }
    }
    if(runs < 0){
        EP("> # of runs must > 0, but %d found.\n", runs);
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

