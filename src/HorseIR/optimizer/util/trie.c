#include "../global.h"

typedef struct TNode{
    Chain *chain;
    struct TNode *child[128];
} TNode;

TNode *TRoot;

/* debug */
void printTrie(TNode *rt){
    for(int i=0;i<128;i++){
        if(rt->child[i]) {
            P("test = %c\n",i);
            printTrie(rt->child[i]);
        }
    }
}

void insertKey(TNode *n, char *str, int index, Chain *chain){
    char c = str[index];
    if(c != 0){
        if(n->child[c]==NULL) n->child[c] = NEW(TNode);
        insertKey(n->child[c], str, index+1, chain);
    }
    else n->chain = chain;
}

void insertString(char *str, Chain *chain){
    insertKey(TRoot, str, 0, chain);
}

Chain *fetchChain(TNode *n, char *str, int index){
    char c = str[index];
    if(!n) return NULL;
    //P("str =%d, %s, %c, %d\n", n,str,c,index);
    if(c != 0) return fetchChain(n->child[c], str, index+1); 
    else return n->chain;
}

Chain *getChain(char *str){
    return fetchChain(TRoot, str, 0);
}

void initTrie(){
    TRoot = NEW(TNode);
}

