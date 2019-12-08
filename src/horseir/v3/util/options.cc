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
int   numOpts;
bool  qIsTpch;
int   qTpchId;
char *qCmd;

static int longFlag; // set by '--verbose'
static char *qOpt[99];

#define INDENT 31
//#define usage_q() dispLine(1, INDENT, "-q <qid>", "TPC-H query id `data/` (-q or -n)")
#define usage_n(x) dispLine(x, INDENT, "-f, --file <filename>", "Specify a query file")

static void dispLine(I level, I left, const char *shortMsg, const char *longMsg){
    I indentLevel[] = { 2, 4, 6, 8 };
    if(level < 0 || level >  3)
        EP("The given level must be between [0, 3]");
    I leads = indentLevel[level];
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
    dispLine(0, INDENT, "-c, --compiler <target>", "Enable compiler target (cpu/llvm)");
    usage_n(1);
    dispLine(2, INDENT, "-o, --opt <opt>", "Query optimizations:");
    dispLine(0, INDENT, "", "> fe  : elementwise fusion");
    dispLine(0, INDENT, "", "> fp  : fusion with patterns");
    dispLine(0, INDENT, "", "> fa  : automatic operator fusion");
    dispLine(0, INDENT, "", "> all : all above optimizations");
}

static void usagePrinter(){
    usage_n(1);
    dispLine(2, INDENT, "--print <item>", "Enable printer to print:");
    dispLine(0, INDENT, "", "> pretty      : pretty programs");
    dispLine(0, INDENT, "", "> dot         : dependence graphs in dot");
    dispLine(0, INDENT, "", "> mermaid     : mermaid dot");
    dispLine(0, INDENT, "", "> symboltable : symbol tables");
    dispLine(0, INDENT, "", "> typeshape   : types and shapes");
}

static void usageStats(){
    //WP("\nRun with a stats manager:\n");
    //WP("\n");
    dispLine(1, INDENT, "--stats <load/dump>", "Load/dump stats");
}

static void usageUtility(){
    WP("\nRun with a utility manager:\n");
    dispLine(0, INDENT, "-u, --utility", "Enable utility manager (printer/stats)");
    usagePrinter();
    usageStats();
}

void usage(I e){
    WP("Usage: ./horse <option> [parameter]  \n");
    usageInterp();
    usageCompiler();
    usageUtility();
    WP("\nOthers:\n");
    dispLine(0, INDENT, "-h, --help", "Print this information");
    dispLine(0, INDENT, "-v, --version", "Print HorseIR version");
    exit(e);
}

/* I/O redirect, need unistd.h */

#define fd_stdin  0
#define fd_stdout 1
#define fd_stderr 2

B isRedirected(){
    R !isatty(fd_stdin) || !isatty(fd_stdout) || !isatty(fd_stderr);
}

B isInputRedirected(){
    R !isatty(fd_stdin);
}

B isOutputRedirected(){
    R !isatty(fd_stdout) || !isatty(fd_stderr);
}

B isStdoutRedirected(){
    R !isatty(fd_stdout);
}

B isStderrRedirected(){
    R !isatty(fd_stderr);
}


/*
 * See discussion: https://stackoverflow.com/a/1052750/4111149
 *   no_argument       : space or '='
 *   required_argument : space or '='
 *   optional_argument : '='
 */
static struct option long_options[] = {
    {"tpch"        , required_argument, &longFlag, 0},
    {"stats"       , required_argument, &longFlag, 1},
    {"printer"     , required_argument, &longFlag, 2},
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

static const char *strMode(OptionMode mode){
    switch(mode){
        case     UnknownM: R "Unknown";
        case InterpNaiveM: R "Interpreter Naive";
        case    CompilerM: R "Compiler";
        case     VersionM: R "Version";
        case      HelperM: R "Helper";
        case     UtilityM: R "Utility";
        default: EP("Unknown mode: %d", mode);
    }
}

static TC getTargetCode(S target){
    if(sEQ(target, "cpu")) R TARGET_C;
    else if(sEQ(target, "llvm")) R TARGET_LLVM;
    else if(sEQ(target, "openacc")) R TARGET_ACC;
    else if(sEQ(target, "opencl")) R TARGET_CL;
    else R TARGET_NA;
}

static OC obtainOptCode(S opt){
    if(sEQ(opt, "fe")) R OPT_FE;
    else if(sEQ(opt, "fp")) R OPT_FP;
    else if(sEQ(opt, "fa")) R OPT_FA;
    else if(sEQ(opt, "all")) R OPT_ALL;
    else R OPT_NA;
}

const char *obtainOptStr(OC x){
    switch(x){
        case OPT_FE : R "Elementwise Fusion";
        case OPT_FP : R "Fusion with Patterns";
        case OPT_FA : R "Automatic Operator Fusion";
        case OPT_ALL: R "All Optimizations On";
        default: R "Unknown Optimizations";
    }
}

static I validateOptimization(){
    if(numOpts > 0){
        DOI(numOpts, \
            if(OPT_NA == (qOpts[i]=obtainOptCode(qOpt[i]))) \
                EP("Unknown optimizaiton: %s", qOpt[i]))
    }
    R 0;
}

static I validateInterp(){
    if(!qPath || qRun <= 0) BAD_TRY();
    R 0;
}

static I validateCompiler(){
    if(TARGET_NA == qTarg)
        EP("Compiler target must be provided correctly: cpu or gpu");
    //P("qPath = %s, qRun = %d\n", qPath,qRun);
    if(!qPath || qRun <= 0) BAD_TRY();
    R validateOptimization();
}

// qCmd can be checked later
static I validatePrinter(){
    if(!qPath) BAD_TRY();
    R 0;
}

static I validateStats(){
    if(qCmd && (sEQ(qCmd, "dump") || sEQ(qCmd, "load")));
    else BAD_TRY();
    R 0;
}

static I validateUtility(){
    switch(optUtl){
        case PrinterU: validatePrinter(); break;
        case   StatsU: validateStats();   break;
        default: EP("Unknown utility funciton");
    }
    R 0;
}

static I validateOptions(){
    // TODO: add validation rules
    switch(optMode){
        case InterpNaiveM: return validateInterp();
        case    CompilerM: return validateCompiler();
        case     UtilityM: return validateUtility();
        case     VersionM: break;
        case      HelperM: break;
        case     UnknownM: break;
        default: TODO("Add support for validating %s", strMode(optMode));
    }
    R 0;
}

static OptionMode setMode(OptionMode opt, OptionMode mode){
    if(opt == UnknownM || opt == mode) return mode;
    EP("Please choose one mode: %s or %s", strMode(opt), strMode(mode));
}

static void setUtility(OptionUtility u){
    if(optUtl == UnknownU)
        optUtl = u;
    else if(optUtl != u)
        EP("Utility function has been chosen");
}

static void getLongOptionVerbose(I x){
    if(long_options[x].flag != 0){
        I index = long_options[x].val;
        switch(index){
            case 0: qTpchId = atoi(optarg);
                    qIsTpch = (qTpchId>0 && qTpchId<=22);  break;
            case 1: setUtility(StatsU);
                    qCmd    = strdup(optarg);  break;
            case 2: setUtility(PrinterU);
                    qCmd    = strdup(optarg);  break;
            default: EP("Option index unknown: %d", index);
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
    qCmd     = NULL;
    optUtl   = UnknownU;
}

I getLongOption(I argc, C *argv[]){
    I c, option_index  = 0;
    init();
    while((c=getopt_long(argc, argv,"hvtur:c:f:o:", long_options, &option_index)) != -1){
        if(numOpts >= 99)
            EP("Buffer overflow: %d >= 99", numOpts);
        switch(c){
            case 0: getLongOptionVerbose(option_index); break;
            case 't': optMode = setMode(optMode, InterpNaiveM); break;
            case 'c': optMode = setMode(optMode,    CompilerM);\
                      qTarg   = getTargetCode(optarg);          break;
            case 'u': optMode = setMode(optMode,     UtilityM); break;
            case 'v': optMode = setMode(optMode,     VersionM); break;
            case 'h': optMode = setMode(optMode,      HelperM); break;
            case 'r': qRun    = atoi(optarg);   break;
            case 'f': qPath   = strdup(optarg); break;
            case 'o': qOpt[numOpts++] = strdup(optarg); break;
            default: WP("c = %d\n",c); R 1;
        }
    }
    R validateOptions();
}

