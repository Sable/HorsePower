#include "../global.h"

#ifdef DO_HASH_V0

#include "h_hash_common.h" // common hash functions

static L createHash(HN *hashT, L hashLen){
    *hashT = (HN) malloc(sizeof(HN0) * hashLen);
    memset(*hashT, 0, sizeof(HN0)*hashLen);
    R 0;
}

static void test_hashSize(L *hashSize){
    L debug_size_max = -1;
    L debug_size_tot = 0;
    DOI(setT, if(hashSize[i]>debug_size_max) debug_size_max=hashSize[i])
    DOI(setT, debug_size_tot+=hashSize[i])
    DOI(setT, P("hashSize[%lld] = %lld\n", i,hashSize[i]))
    P("max size %lld, with total %lld\n", debug_size_max, debug_size_tot);
    getchar();
}

static THash createHashMultiple(V x){
    P("create hash multiple\n");
    L *count      = NEWL(L , setT);
    L *prefix     = NEWL(L , setT);
    HN *hashTable = NEWL(HN, setT);
    L  *hashSize  = NEWL(L , setT);
    // Step 1: scan for basic info
    DOI(xn, count[vL(x,i)&setN]++)
    DOIa(setT, prefix[i]=prefix[i-1]+count[i-1])
    // Step 2: create partitions
    DOI(setT, { HN ht=NULL; \
        L hashLen=count[i]==0?0:getHashTableSize(count[i]);\
        if(hashLen>0)createHash(&ht,hashLen); \
        hashTable[i]=ht; hashSize[i]=hashLen;})
    //test_hashSize(hashSize);
    // Step 3: insert items
    DOI(xn, {L t=vL(x,i)&setN; \
        insertHashMany(hashTable[t],hashSize[t]-1,xg,i,NULL,-1,H_L);})
    THash th;
    th.hashTable = hashTable;
    th.hashSize  = hashSize;
    th.numTable  = setT;
    R th;
}

static HN createHashSingle(V x, L *hashLen){
    printBanner("hash join single v0\n");
    HN hashT; createHash(&hashT, *hashLen=getHashTableSize(xn));
    L hashMask = (*hashLen) - 1;
    DOI(xn, insertHashMany(hashT,hashMask,xg,i,NULL,-1,xp))
    //profileHash(hashT, *hashLen); getchar();
    R hashT;
}

HN createHashWithV(L n, V *x){
    if(n >= 1) R createHashSingle(x[0]);
    else EP("Number of columns invalid: %lld", n);
}

// naive version
static HN findValueFromHashCore(HN hashT, V *src, L hashLen, L n, LL *args){
    if(n == 1){
        LL val = args[0];
        L hashMask = hashLen - 1;
        L hashKey = hash_LL(args[0]) & hashMask;
        HN t = hV(hashT, hashKey);
        V src0 = src[0];
        //P(" n = 1, begin\n");
        while(hN(t)){
            t = hN(t); L td = hD(t);
            if(vL(src0,td)==val)  R t;
        }
        //P(" n = 1, done\n");
    }
    else {
        LL val = args[0];
        L hashMask = hashLen - 1;
        L hashKey = hash_LL(args[0]) & hashMask;
        HN t = hV(hashT, hashKey);
        //P("1: n = %lld\n",n);
        while(hN(t)){
            t = hN(t); L td = hD(t);
            if(isAllMatch(n,src,td,args)) R t;
        }
        //P("2: done\n");
    }
    R 0;
}

HN findValueFromHash(HN hashT, V *src, L hashLen, L n, ...){
    readArgs();
    //P("Input with %lld:\n",n);
    //DOI(n, P("args[%lld] = %lld\n",i,args[i]))
    R findValueFromHashCore(hashT,src,hashLen,n,args);
}

#endif


