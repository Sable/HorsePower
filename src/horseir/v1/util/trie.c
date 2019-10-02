#include "../global.h"

typedef struct TNode{
    Chain *chain;
    InfoNode *in;
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

void printTireAllType(TNode *rt, char *name, int dep){
    if(rt) {
        if(rt->in){
            name[dep] = 0;
            P("%-3s:",name); printInfoNode(rt->in);
        }
        for(int i=0;i<128;i++){
            if(rt->child[i]) {
                name[dep] = i;
                printTireAllType(rt->child[i], name, dep+1);
            }
        }
    }
}

void insertKey(TNode *n, char *str, int index, Chain *chain, InfoNode *in){
    char c = str[index];
    if(c != 0){
        if(n->child[c]==NULL) n->child[c] = NEW(TNode);
        insertKey(n->child[c], str, index+1, chain, in);
    }
    else {
        n->chain = chain;
        n->in    = in;
    }
}

void insertString(char *str, Chain *chain, InfoNode *in){
    in->name = strdup(str); // set defName
    insertKey(TRoot, str, 0, chain, in);
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

/* InfoNode */
InfoNode *fetchInfoNode(TNode *n, char *str, int index){
    char c = str[index];
    if(!n) return NULL;
    //P("str =%d, %s, %c, %d\n", n,str,c,index);
    if(c != 0) return fetchInfoNode(n->child[c], str, index+1); 
    else return n->in;
}

InfoNode *getInfoNode(char *str){
    return fetchInfoNode(TRoot, str, 0);
}

void initTrie(){
    TRoot = NEW(TNode);
}

void printTypeShape(){
    char name[99];
    printBanner("Type and Shape Info");
    printTireAllType(TRoot, name, 0);
}

