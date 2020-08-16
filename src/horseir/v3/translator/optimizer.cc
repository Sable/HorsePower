#include "../global.h"

#define AllOptSize 2
#define BasicOptSize 2
#define hashOptSize (1<<12)  // 4K
#define hashDelSize (1<<10)

static const OC ListOfAllOpt[AllOptSize] = {OPT_FA, OPT_FP};
static const OC ListOfBasicOpt[BasicOptSize] = {OPT_FE, OPT_FP_DLS18};

extern Prog *root;
extern sHashTable *hashOpt;
extern sHashTable *hashDel;

I qid, phTotal;

/* ---------- Above declarations ----------  */

static void printBannerLocal(const char *msg){
    WP("/*==== ");
    WP("Optimizing %s", msg);
    WP(" ====*/\n");
}

static void optimizerMain(OC opt){
    printBannerLocal(obtainOptStr(opt));
    switch(opt){
        case OPT_FE : optElementwise(); break; // compiledMethodList
        case OPT_FP : optPattern(0);    break; // compiledMethodList
        case OPT_FP1: optPattern(1);    break; // compiledMethodList
        case OPT_FP2: optPattern(2);    break; // compiledMethodList
        case OPT_FP_DLS18:
                      optPattern(5);    break; // for dls18: 4 patterns
        case OPT_FA : optAutoFusion();  break; // compiledMethodList
        case OPT_IL : optInlining();    break; // compiledMethodList
        default: TODO("Add impl. %s", obtainOptStr(opt));
    }
}

#define isOptimizeAll()   isOptimizeGroup(OPT_ALL)
#define isOptimizeBasic() isOptimizeGroup(OPT_BASIC)
static B isOptimizeGroup(OC x){
    DOI(numOpts, if(qOpts[i] == x) R true) R false;
}

static void init(){
    buildUDChain(root);
    hashOpt = initSimpleHash(hashOptSize);
    hashDel = initSimpleHash(hashDelSize);
    qid     = qIsTpch?qTpchId:99;
    phTotal = 0;
}

I HorseCompilerOptimized(){
    printBanner("Compiling with Optimizations");
    init();
    simplifyUDChain();
    if(isOptimizeAll()){
        DOI(AllOptSize, optimizerMain(ListOfAllOpt[i]))  // enumerate all opts
    }
    else if(isOptimizeBasic()){
        DOI(BasicOptSize, optimizerMain(ListOfBasicOpt[i]))
    }
    else {
        DOI(numOpts, optimizerMain(qOpts[i]))
    }
    profileSimpleHash(hashOpt);
    genOptimizedCode();
    R 0;
}

