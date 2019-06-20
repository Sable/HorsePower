#ifndef __H_UDCHAIN__
#define __H_UDCHAIN__

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

typedef struct ChainList {
    Chain *chain;
    struct ChainList *next;
}ChainList;

typedef struct FlowList{
    ChainList *flow;
    struct FlowList *next;
}FlowList;

//void printChainList();
//void printChainInfo(Chain *chain, char opt);

#define printChain(x)  printNodeStr(x->cur)
#define printChainList printFlow

void buildUDChain   (Prog *root);
void printChainInfo (Chain *p);
void printFlow      (ChainList *list);

void addToChainList(ChainList *chains, Chain *c);


#endif
