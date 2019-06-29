#include "../global.h"

#ifdef H_LINUX
#include <getopt.h>
#endif

OptionMode optMode;
int   qRun;
char *qPath;
TC    qTarg;
OC    qOpts[99];
int numOpts;
bool qIsTpch;
int qTpchId;

static int longFlag; // set by '--verbose'
static char *qOpt[99];

#define INDENT 29
//#define usage_q() dispLine(1, INDENT, "-q <qid>", "TPC-H query id `data/` (-q or -n)")
#define usage_n() dispLine(1, INDENT, "-f, --file <filename>", "Set a query file")
#define usage_o() dispLine(2, INDENT, "-o, --opt <opt>", "Query optimizations (default: all): fe or fp");

void version(){
    P("HorseIR version: 0.2.0\n");
}

static void dispLine(int level, int left, char *shortMsg, char *longMsg){
    int indents[] = { 2, 4, 6, 8 };
    if(level < 0 || level >  3) EP("lead must be between [0, 3]\n");
    int leads = indents[level];
    DOI(leads, WP(" ")); WP("%s", shortMsg);
    DOI(left-strlen(shortMsg)-leads, WP(" "));
    WP("%s\n", longMsg);
}

static void usageInterp(){
    WP("\nRun with an interpreter:\n");
    dispLine(0, INDENT, "-t, --interpreter", "Enable interpreter");
    usage_n();
    dispLine(2, INDENT, "-r, --run <runs>", "Number of runs (default 1)");
}

static void usageCompiler(){
    WP("\nRun with a compiler:\n");
    dispLine(0, INDENT, "-c, --compiler <target>", "Enable compiler with target c/llvm/openacc/cuda/opencl");
    usage_n(); usage_o();
    WP("\nRun with a pretty printer:\n");
}

static void usagePrettyPrinter(){
    dispLine(0, INDENT, "-p, --pretty", "Enable pretty printer");
    usage_n();
}

static void usageDotPrinter(){
    WP("\nRun with a dot printer:\n");
    dispLine(0, INDENT, "--dot", "Enable dot printer");
}

void usage(int e){
    WP("Usage: ./horse <option> [parameter]  \n");
    usageInterp();
    usageCompiler();
    usagePrettyPrinter();
    usageDotPrinter();
    WP("\nOthers:\n");
    dispLine(0, INDENT, "-h, --help", "Print this information");
    dispLine(0, INDENT, "-v, --version", "Print HorseIR version");
    exit(e);
}

/* no_argument, required_argument and optional_argument */
static struct option long_options[] = {
    {"dot" , no_argument, &longFlag, 0},
    {"tpch", required_argument, &longFlag, 1},
    {"help"        , no_argument      , 0, 'h'},
    {"version"     , no_argument      , 0, 'v'},
    {"compiler"    , no_argument      , 0, 'c'},
    {"interpreter" , no_argument      , 0, 't'},
    {"pretty"      , no_argument      , 0, 'p'},
    {"runs"        , required_argument, 0, 'r'},
    {"file"        , required_argument, 0, 'f'},
    {"opt"         , required_argument, 0, 'o'},
    {0, 0, 0, 0}
};

static char *strMode(OptionMode mode){
    switch(mode){
        case     UnknownM: return "Unknown";
        case InterpNaiveM: return "Interpreter Naive";
        case    CompilerM: return "Compiler";
        case   InterpJITM: return "Interpreter JIT";
        case PrettyPrintM: return "Pretty Printer";
        case    DotPrintM: return "Dot Printer";
        case     VersionM: return "Version";
        case      HelperM: return "Helper";
        case  ExperimentM: return "Experiment";
        default: EP("Unknown mode: %d\n", mode);
    }
}

static TC getTargetCode(S target){
    if(!strcmp(target, "cpu")) R TARGET_C;
    else if(!strcmp(target, "llvm")) R TARGET_LLVM;
    else if(!strcmp(target, "openacc")) R TARGET_ACC;
    else if(!strcmp(target, "opencl")) R TARGET_CL;
    else R TARGET_NA;
}

static OC getOptCode(S opt){
    if(!strcmp(opt, "fe")) R OPT_FE;
    else if(!strcmp(opt, "fp")) R OPT_FP;
    else if(!strcmp(opt, "sr")) R OPT_SR;
    else if(!strcmp(opt, "all")) R OPT_ALL;
    else R OPT_NA;
}

const S getOptStr(OC x){
    switch(x){
        case OPT_FE : R "Fusion Elementwise";
        case OPT_FP : R "Fusion with Patterns";
        case OPT_SR : R "Strength Reduction";
        case OPT_ALL: R "All Optimizations On";
        default: R "Unknown Optimizations";
    }
}

static int validateOptimization(){
    if(numOpts > 0){
        DOI(numOpts, \
            if(OPT_NA == (qOpts[i]=getOptCode(qOpt[i]))) \
                EP("Unknown optimizaiton: %s\n", qOpt[i]))
    }
    return 0;
}

#define validateInterpJIT validateInterp
#define validateDot       validatePretty

static int validateInterp(){
    if(!qPath || qRun <= 0) BAD_TRY();
    return 0;
}

static int validateCompiler(){
    if(TARGET_NA == qTarg)
        EP("Compiler target must be provided correctly: cpu or gpu\n");
    //P("qPath = %s, qRun = %d\n", qPath,qRun);
    if(!qPath || qRun <= 0) BAD_TRY();
    return validateOptimization();
}

static int validatePretty(){
    if(!qPath) BAD_TRY();
    return 0;
}

static int validateOptions(){
    // TODO: add validation rules
    switch(optMode){
        case InterpNaiveM: return validateInterp();
        case    CompilerM: return validateCompiler();
        case   InterpJITM: return validateInterpJIT();
        case PrettyPrintM: return validatePretty();
        case    DotPrintM: return validateDot();
        case     VersionM: break;
        case      HelperM: break;
        case  ExperimentM: break;
        case     UnknownM: break;
        default: TODO("Add support for validating %s\n", strMode(optMode));
    }
    return 0;
}

static OptionMode setMode(OptionMode opt, OptionMode mode){
    if(opt == UnknownM || opt == mode) return mode;
    P("Please choose one mode: %s or %s\n", strMode(opt), strMode(mode));
    exit(1);
}

static void getLongOptionVerbose(int x){
    if(long_options[x].flag != 0){
        int index = long_options[x].val;
        switch(index){
            case 0: optMode = setMode(optMode, DotPrintM); break;
            case 1: qTpchId = atoi(optarg);
                    qIsTpch = (qTpchId>0 && qTpchId<=22);  break;
            default: EP("Option index unknown: %d\n", index);
        }
    }
}

static void init(){
    longFlag = 0;
    optMode  = UnknownM;
    qRun     = 1;
    qPath    = NULL;
    qTarg    = TARGET_NA;
    qTpchId  = 0;
    qIsTpch  = false;
    numOpts  = 0;
}

int getLongOption(int argc, char *argv[]){
    int c, option_index  = 0;
    init();
    while((c=getopt_long(argc, argv,"hvtpr:c:f:o:x:", long_options, &option_index)) != -1){
        if(numOpts >= 99) EP("Buffer overflow\n");
        switch(c){
            case 0: getLongOptionVerbose(option_index); break;
            case 't': optMode = setMode(optMode, InterpNaiveM); break;
            case 'c': optMode = setMode(optMode,    CompilerM);\
                      qTarg   = getTargetCode(optarg);          break;
            case 'v': optMode = setMode(optMode,     VersionM); break;
            case 'h': optMode = setMode(optMode,      HelperM); break;
            case 'p': optMode = setMode(optMode, PrettyPrintM); break;
            case 'x': optMode = setMode(optMode,  ExperimentM); break;
            case 'r': qRun    = atoi(optarg);   break;
            case 'f': qPath   = strdup(optarg); break;
            case 'o': qOpt[numOpts++] = strdup(optarg); break;
            default: return 1;
        }
    }
    return validateOptions();
}

