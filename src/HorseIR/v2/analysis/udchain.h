#ifndef __H_UDCHAIN__
#define __H_UDCHAIN__

typedef enum { NativeG, SkipG, OptG } GenKind;

typedef struct NameList{
    char *name;
    struct NameList *next;
}NameList;

typedef struct Chain {
    Node *cur; // current node
    NameList *defs, *uses;
    int defSize, maxDSize;
    int useSize, maxUSize;
    struct Chain **chain_uses;
    struct Chain **chain_defs;
    bool isVisited;
    //InfoNodeList *info;
}Chain;

typedef struct ChainExtra{
    GenKind kind;
    char *funcDecl;
    char *funcFunc;
    char *funcInvc;
}ChainExtra;

typedef struct ChainList {
    Chain *chain;
    struct ChainList *next;
}ChainList;

typedef struct FlowList{
    ChainList *flow;
    struct FlowList *next;
}FlowList;

typedef struct ChainBasicBlock {
    L bbId; /* basic block id */
    Chain *entry, *last;
}ChainBlock;

//void printChainList();
//void printChainInfo(Chain *chain, char opt);

#define printChain(x)  printNodeStr(x->cur)
#define printChainList printFlow

void buildUDChain   (Prog *root);
void printChainInfo (Chain *p);
void printFlow      (ChainList *x);
void printChainExtra(ChainExtra *extra);

void addToChainList(ChainList *chains, Chain *c);


#endif
