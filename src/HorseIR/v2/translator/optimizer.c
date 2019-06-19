#include "../global.h"

extern Prog *root;
extern List *compiledMethodList;

static Node *currentMethod;

#define isChainVisited(c) (c->isVisited)

/* ------ declaration above ------ */

static void findFusion(Chain *chain){
    Node *n = chain->cur;
    printChain(chain); P("\n");
    switch(n->kind){
        case stmtK: break;
    }
}

static void analyzeChain(ChainList *list){
    if(list){
        analyzeChain(list->next);
        if(!isChainVisited(list->chain))
            findFusion(list->chain);
    }
}

static void compileMethod(Node *n){
    Node *prevMethod = currentMethod;
    currentMethod = n;
    ChainList *chains = n->val.method.meta->chains;
    //printChainList(chains);
    analyzeChain(chains->next);
    currentMethod = prevMethod;
}

static void scanMethodList(List *list){
    if(list) { scanMethodList(list->next); compileMethod(list->val); }
}

static void printBannerLocal(S msg){
    WP("/*==== ");
    WP("Optimizing %s", msg);
    WP(" ====*/\n");
}

static void optimizerMain(OC opt){
    printBannerLocal(getOptStr(opt));
    scanMethodList(compiledMethodList->next);
}

static bool isOptimizeAll(){
    DOI(numOpts, if(qOpts[i] == OPT_ALL) R true) R false;
}

static void init(){
    buildUDChain(root);
    currentMethod = NULL;
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
    R 0;
}

