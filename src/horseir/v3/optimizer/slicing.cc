#include "../global.h"

extern List *compiledMethodList;
extern sHashTable *hashSlice, *hashSliceFlag;

// 1. delete chain nodes
// 2. add unvisited tree nodes to hashSliceFlag
static ChainList* analyzeChain(ChainList *list){
    ChainList *pre = NULL, *newChainList = list;
    while(list){
        Chain *chain = list->chain;
        Node *n = chainNode(chain);
        if(instanceOf(n, stmtK) && !lookupSimpleHash(hashSlice, (L)chain)){
            addToSimpleHash(hashSliceFlag, (L)n, 1);
            if(pre) {
                pre->next = list->next;
            }
            else {
                newChainList = list->next;
            }
        }
        else pre = list;
        list = list->next;
    }
    return newChainList;
}

static O sliceMethod(Node *method){
    ChainList *chains = nodeMethodChainList(method);
    nodeMethodChainList(method) = analyzeChain(chains); // update chain list
}

static O sliceMethodList(List *list){
    if(list) {
        sliceMethodList(list->next);
        sliceMethod(list->val);
    }
}

//

static O analyzeChainUp(Chain *chain){
    Node *n = chainNode(chain);
    if(instanceOf(n, stmtK) || instanceOf(n, returnK)){
        if(!lookupSimpleHash(hashSlice, (L)chain)){
            addToSimpleHash(hashSlice, (L)chain, 1);
            DOI(chain->defSize, analyzeChainUp(chain->chain_defs[i]))
        }
    }
    // maybe if..else / while
}

static O analyzeChainBottomUp(ChainList *list){
    while(list){
        Chain *chain = list->chain;
        if(instanceOf(chainNode(chain), returnK)){
            analyzeChainUp(chain);
        }
        list=list->next;
    }
}

static O scanMethod(Node *method){
    ChainList *chains = nodeMethodChainList(method);
    analyzeChainBottomUp(chains);
    //printChainList(chains);
}

static void scanMethodList(List *list){
    if(list) {
        scanMethodList(list->next);
        scanMethod(list->val);
    }
}

static O init(){
}

static O clean(){
    // profileSimpleHash(hashSlice);
    // getchar();
}

O optSlicing(){
    init();
    scanMethodList(compiledMethodList->next);
    sliceMethodList(compiledMethodList->next);
    clean();
}

