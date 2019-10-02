#include "../global.h"

typedef struct NNode{
    V val;
    struct NNode *child[128];
} NNode;

NNode *NRoot;

/* debug */
void printNameTable(NNode *rt){
    for(int i=0;i<128;i++){
        if(rt->child[i]) {
            P("test = %c\n",i);
            printNameTable(rt->child[i]);
        }
    }
}

static void insertKey(NNode *n, char *str, int index, V val){
    char c = str[index];
    if(c != 0){
        if(n->child[c]==NULL) n->child[c] = NEW(NNode);
        insertKey(n->child[c], str, index+1, val);
    }
    else {
        n->val = val;
    }
}

void saveToNameTable(char *str, V val){
    insertKey(NRoot, str, 0, val);
}

static V fetchValue(NNode *n, char *str, int index){
    char c = str[index];
    if(!n) return NULL;
    //P("str =%d, %s, %c, %d\n", n,str,c,index);
    if(c != 0) return fetchValue(n->child[c], str, index+1); 
    else return n->val;
}

V getValueFromNameTable(char *str){
    return fetchValue(NRoot, str, 0);
}

void initNameTable(){
    NRoot = NEW(NNode);
}


