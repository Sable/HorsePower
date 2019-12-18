#include "../global.h"

static sHashNode *initHashNode(L key, L val){
    sHashNode *n = NEW(sHashNode);
    n->key = key;
    n->val = val;
    return n;
}

static L countSimpleNode(sHashNode *n){
    L c=0; while(n){c++; n=n->next;} R c;
}

O profileSimpleHash(sHashTable *st){
    L minv = INT_MAX, maxv = INT_MIN, c = 0;
    DOI(st->size, if(st->table[i]){ \
            L x=countSimpleNode(st->table[i]);\
            if(minv>x) minv=x; \
            if(maxv<x) maxv=x; c++;})
    WP("Profile simple hash\n");
    WP(indent4 "size: %g %% (%lld/%lld)\n", percent(c,st->size),c,st->size);
    WP(indent4 "range: [%lld,%lld]\n",minv,maxv);
}

#define ROTL64(x,n) ((x>>n)|(x<<(64-n)))

// Hash functions:
//   https://stackoverflow.com/a/56501471
static L getHashValue(L val, L size){
    val = ROTL64(val, 12) ^ ROTL64(val, 37);
    val*= 0xA24BAED4963EE407LL;
    val = ROTL64(val, 22) ^ ROTL64(val, 47);
    val*= 0xE8ad730BD63BE811LL;
    val = val ^ (val >> 28);
    return (val < 0 ? -val : val)%size;
}

static sHashNode *findSimpleHash(sHashNode *n, L key){
    while(n && n->key != key) n = n->next; R n;
}

O addToSimpleHash(sHashTable *st, L key, L val){
    L t = getHashValue(key, st->size);
    if(!st->table[t]){
        st->table[t] = initHashNode(key, val);
    }
    else {
        sHashNode *p = findSimpleHash(st->table[t], key);
        if(p == NULL){
            sHashNode *n = initHashNode(key, val);
            n->next = st->table[t];
            st->table[t] = n;
        }
        else {
            EP("Duplicated key found with values: %lld vs. %lld",(L)(p->val),val);
        }
    }
}

L lookupSimpleHash(sHashTable *st, L key){
    L t = getHashValue(key, st->size);
    sHashNode *p = findSimpleHash(st->table[t], key);
    R p==NULL?0:p->val;
}

sHashTable* initSimpleHash(L size){
    sHashTable *st = NEW(sHashTable);
    st->table = NEW2(sHashNode, size);
    st->size  = size;
    R st;
}

void freeSimpleHash(sHashTable *st){
    DOI(st->size, { \
            sHashNode *t=st->table[i]; \
            if(t){ \
                while(t){ sHashNode *p=t; t=t->next; free(p); } \
                st->table[i]=NULL; }} )
    free(st);
    // STOP("The simple hash table has been freed!\n");
}


