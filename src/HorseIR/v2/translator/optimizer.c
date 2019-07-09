#include "../global.h"

extern Prog *root;

#define hashOptSize (1<<12)  // 4K
sHashTable *hashOpt;

static void printBannerLocal(S msg){
    WP("/*==== ");
    WP("Optimizing %s", msg);
    WP(" ====*/\n");
}

static void optimizerMain(OC opt){
    printBannerLocal(getOptStr(opt));
    switch(opt){
        case OPT_FE: optElementwise();    break; // compiledMethodList
        case OPT_FP: optPattern();        break; // compiledMethodList
        case OPT_FD: optDeep();           break; // compiledMethodList
        case OPT_SR: optSReduction(root); break;
        default: TODO("Add impl. %s",  getOptStr(opt));
    }
}

static bool isOptimizeAll(){
    DOI(numOpts, if(qOpts[i] == OPT_ALL) R true) R false;
}

static void init(){
    buildUDChain(root);
    hashOpt = initSimpleHash(hashOptSize);
}

I HorseCompilerOptimized(){
    printBanner("Start Optimizing");
    init();
    if(isOptimizeAll()){
        TODO("Add impl.");
    }
    else {
        DOI(numOpts, optimizerMain(qOpts[i]));
    }
    profileSimpleHash(hashOpt);
    genOptimizedCode();
    R 0;
}

