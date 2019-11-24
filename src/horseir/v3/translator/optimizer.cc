#include "../global.h"

#define AllOptSize 2
#define hashOptSize (1<<12)  // 4K

static const OC ListOfAllOpt[AllOptSize] = {OPT_FA, OPT_FP};

extern Prog *root;
extern sHashTable *hashOpt;

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
        case OPT_FE: optElementwise(); break; // compiledMethodList
        case OPT_FP: optPattern();     break; // compiledMethodList
        case OPT_FA: optAuto();        break; // compiledMethodList
        default: TODO("Add impl. %s", obtainOptStr(opt));
    }
}

static B isOptimizeAll(){
    DOI(numOpts, if(qOpts[i] == OPT_ALL) R true) R false;
}

static void init(){
    buildUDChain(root);
    hashOpt = initSimpleHash(hashOptSize);
    qid     = qIsTpch?qTpchId:99;
    phTotal = 0;
}

I HorseCompilerOptimized(){
    printBanner("Compiling with Optimizations");
    init();
    if(isOptimizeAll()){
        DOI(AllOptSize, optimizerMain(ListOfAllOpt[i]))  // enumerate all opts
    }
    else {
        DOI(numOpts, optimizerMain(qOpts[i]))
    }
    profileSimpleHash(hashOpt);
    genOptimizedCode();
    R 0;
}

