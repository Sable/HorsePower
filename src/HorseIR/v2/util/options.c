#include "../global.h"

#ifdef H_LINUX
#include <getopt.h>
#endif

OptionMode optMode;
OptionUtility optUtl;
int   qRun;
char *qPath;
TC    qTarg;
OC    qOpts[99];
int numOpts;
bool qIsTpch;
int qTpchId;
char *qStats;

static int longFlag; // set by '--verbose'
static char *qOpt[99];

#define INDENT 29
//#define usage_q() dispLine(1, INDENT, "-q <qid>", "TPC-H query id `data/` (-q or -n)")
#define usage_n(x) dispLine(x, INDENT, "-f, --file <filename>", "Set a query file")
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
    usage_n(1);
    dispLine(2, INDENT, "-r, --run <runs>", "Number of runs (default 1)");
}

static void usageCompiler(){
    WP("\nRun with a compiler:\n");
    dispLine(0, INDENT, "-c, --compiler <target>", "Enable compiler with target c/llvm/openacc/cuda/opencl");
    usage_n(1); usage_o();
}

static void usagePrettyPrinter(){
    //WP("\nRun with a pretty printer:\n");
    //WP("\n");
    dispLine(1, INDENT, "--pretty", "Enable pretty printer");
    usage_n(2);
}

static void usageDotPrinter(){
    //WP("\nRun with a dot printer:\n");
    //WP("\n");
    dispLine(1, INDENT, "--dot", "Enable dot printer");
}

static void usageStats(){
    //WP("\nRun with a stats manager:\n");
    //WP("\n");
    dispLine(1, INDENT, "--stats <load/dump>", "Load/dump stats");
}

static void usageUtility(){
    WP("\nRun with a utility manager:\n");
    dispLine(0, INDENT, "-u, --utility", "Enable utility manager (pretty/dot/stats)");
    usagePrettyPrinter();
    usageDotPrinter();
    usageStats();
}

void usage(int e){
    WP("Usage: ./horse <option> [parameter]  \n");
    usageInterp();
    usageCompiler();
    usageUtility();
    //usagePrettyPrinter();
    //usageDotPrinter();
    //usageStats();
    WP("\nOthers:\n");
    dispLine(0, INDENT, "-h, --help", "Print this information");
    dispLine(0, INDENT, "-v, --version", "Print HorseIR version");
    exit(e);
}

/* no_argument, required_argument and optional_argument */
static struct option long_options[] = {
    {"dot"         , no_argument      , &longFlag, 0},
    {"tpch"        , required_argument, &longFlag, 1},
    {"stats"       , required_argument, &longFlag, 2},
    {"pretty"      , no_argument      , &longFlag, 3},
    {"meta"        , no_argument      , &longFlag, 4},
    {"help"        , no_argument      , 0, 'h'},
    {"version"     , no_argument      , 0, 'v'},
    {"compiler"    , no_argument      , 0, 'c'},
    {"interpreter" , no_argument      , 0, 't'},
    {"runs"        , required_argument, 0, 'r'},
    {"file"        , required_argument, 0, 'f'},
    {"opt"         , required_argument, 0, 'o'},
    {"utility"     , required_argument, 0, 'u'},
    {0, 0, 0, 0}
};

static char *strMode(OptionMode mode){
    switch(mode){
        case     UnknownM: return "Unknown";
        case InterpNaiveM: return "Interpreter Naive";
        case    CompilerM: return "Compiler";
        case   InterpJITM: return "Interpreter JIT";
        case     VersionM: return "Version";
        case      HelperM: return "Helper";
        case  ExperimentM: return "Experiment";
        case     UtilityM: return "Utility";
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

static int validateStats(){
    if(qStats && (!strcmp(qStats, "dump") || !strcmp(qStats, "load")));
    else BAD_TRY();
    return 0;
}

static int validateUtility(){
    switch(optUtl){
        case PrettyPrintU: validatePretty(); break;
        case         DotU: validateDot();    break;
        case       StatsU: validateStats();  break;
        default: EP("Unknown utility funciton");
    }
}

static int validateOptions(){
    // TODO: add validation rules
    switch(optMode){
        case InterpNaiveM: return validateInterp();
        case    CompilerM: return validateCompiler();
        case   InterpJITM: return validateInterpJIT();
        case     UtilityM: return validateUtility();
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

static void setUtility(OptionUtility u){
    if(optUtl == UnknownU)
        optUtl = u;
    else if(optUtl != u)
        EP("Utility function has been chosen");
}

static void getLongOptionVerbose(int x){
    if(long_options[x].flag != 0){
        int index = long_options[x].val;
        switch(index){
            case 0: setUtility(DotU);                      break;
            case 1: qTpchId = atoi(optarg);
                    qIsTpch = (qTpchId>0 && qTpchId<=22);  break;
            case 2: setUtility(StatsU);
                    qStats  = strdup(optarg);              break;
            case 3: setUtility(PrettyPrintU);              break;
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
    qStats   = NULL;
    optUtl   = UnknownU;
}

int getLongOption(int argc, char *argv[]){
    int c, option_index  = 0;
    init();
    while((c=getopt_long(argc, argv,"hvtur:c:f:o:x:", long_options, &option_index)) != -1){
        if(numOpts >= 99) EP("Buffer overflow\n");
        switch(c){
            case 0: getLongOptionVerbose(option_index); break;
            case 't': optMode = setMode(optMode, InterpNaiveM); break;
            case 'c': optMode = setMode(optMode,    CompilerM);\
                      qTarg   = getTargetCode(optarg);          break;
            case 'u': optMode = setMode(optMode,     UtilityM); break;
            case 'v': optMode = setMode(optMode,     VersionM); break;
            case 'h': optMode = setMode(optMode,      HelperM); break;
            case 'x': optMode = setMode(optMode,  ExperimentM); break;
            case 'r': qRun    = atoi(optarg);   break;
            case 'f': qPath   = strdup(optarg); break;
            case 'o': qOpt[numOpts++] = strdup(optarg); break;
            default: P("c = %d\n",c); return 1;
        }
    }
    return validateOptions();
}

