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
}Chain;

typedef struct ChainList {
    Chain *chain;
    struct ChainList *next;
}ChainList;

void printChainList();
char *fetchName(Node *n);
void printChainInfo(Chain *chain, char opt);

#endif
