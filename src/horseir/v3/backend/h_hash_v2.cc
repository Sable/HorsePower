#include "../global.h"

#ifdef DO_HASH_V2

#include "h_hash_common.h" // common hash functions

TBucket createHashWithV2(L n, V *x, L *hashLen){
    if(n >= 1) R create_hash_multiply_v2(x[0]);
    else EP("Number of columns invalid: %lld", n);
}

// same as insert_index_v1
static L insert_index_v2(HN t, L td){
    const L SLOT_SIZE = 64;
    //P("insert_index_v2 stop here: h_num=%lld\n",t->h_num); getchar();
    if(t->h_num == 0){
        t->h_num = 1;
        t->h_other = td;
    }
    else if(t->h_num == 1){
        L *other = HASH_AL(L, SLOT_SIZE);
        other[0] = t->h_other;
        other[1] = td;
        t->h_other = (L)other;
        t->h_num = 2;
    }
    else if(t->h_num > 1 && t->h_num < SLOT_SIZE){
        L *other = (L*)(t->h_other);
        other[t->h_num] = td;
        t->h_num++;
    }
    else EP("Not enough slots: %lld", t->h_num);
    //P("number of slots: %lld\n", t->h_num); getchar();
    R td;
}

static L insert_hash_v2(HB ht, L htMask, void *src, L srcI, L typ){
    L srcV      = *(((L*)src)+srcI);
    L hashValue = hash_L(srcV);  //getHashValue(src,srcI,typ);
    L hashKey   = hashValue & htMask;
    //P("hash value = %lld, key = %lld\n", hashValue,hashKey); getchar();
    HB t = bV(ht,hashKey);
    if(t->cur!=0){ // non-empty
        HN node = t->node;
        DOI(t->cur, {HN x=node+i; \
            if(x->h_hash==hashValue){ \
            switch(typ){\
                caseL if(x->h_l==toL(src,srcI)) R insert_index_v2(x,srcI); break; \
                default: EP("Unexpected type: %s", getTypeName(typ)); \
            }}})
        if(t->cur + 1 < t->size){
            HN x=node+(t->cur);
            x->h_index = srcI;
            x->h_l     = srcV;
            x->h_hash  = hashValue;
            // insert_index_v2(x, srcI);
            x->h_num   = 1;
            x->h_other = srcI;
            t->cur++;
        }
        else EP("Size not enough, need to grow: %lld / %lld", t->cur, t->size);
    }
    else { // empty bucket
        const L init_size = 16;
        HN x = newHashNodeN(init_size);
        x->h_index = srcI;
        x->h_l     = srcV;
        x->h_hash  = hashValue;
        //insert_index_v2(x,srcI);
        x->h_num   = 1;
        x->h_other = srcI;
        t->node = x;
        t->size = init_size;
        t->cur  = 1;
    }
    R srcI;
}

static L insert_hash_int_v2(HB ht, L htMask, LL *src, L srcI){
    LL val = src[srcI];
    //LL hashValue = hash_LL(val); // hash value not used for a single column 
    L hashKey = hash_LL(val) & htMask;
    HB t = bV(ht, hashKey);
    //if(srcI==3806) {
    //    P("insert   srcI: %lld\n", srcI);
    //    P("          val: %d -> %lld (hash key)\n", val,hashKey);
    //    P("       t->cur: %lld\n", t->cur);
    //    P("     hashMask: %lld\n", htMask);
    //    P("    hashValue: %lld\n", hash_LL(val));
    //    getchar();
    //}
    if(t->cur >= PRE_SIZE){
        DOI(PRE_SIZE, {HN x=t->nd0+i; if(x->h_i==val) R insert_index_v2(x,srcI);})
        if(t->cur < t->size){
            DOI(t->cur-PRE_SIZE, \
                {HN x=t->node+i; if(x->h_i==val) R insert_index_v2(x,srcI);})
            // if not found, add a new node
            HN x=t->node+t->cur;
            x->h_index = srcI;
            x->h_i     = val;
            x->h_num   = 1;
            x->h_other = srcI;
            t->cur++;
        }
        else if(t->size == -1){
            #define MORE_SIZE 8
            t->size = MORE_SIZE;
            HN x=newHashNodeN(MORE_SIZE);
            x->h_index = srcI;
            x->h_i     = val;
            //x->h_hash  = hashValue;
            x->h_num   = 1;
            x->h_other = srcI;
            t->node    = x;
            t->size    = PRE_SIZE+MORE_SIZE;
            t->cur++;
        }
        else EP("Size not enough, need to grow: %lld / %lld", t->cur, t->size);
        P("size: t->cur = %lld\n", t->cur);
    }
    else if(t->cur<PRE_SIZE){ // empty bucket
        //const L init_size = 16;
        HN x = t->nd0+(t->cur);  // // <---- nd0 or node !!!!
        x->h_index = srcI;
        x->h_i     = val;
        //x->h_hash  = hashValue;
        x->h_num   = 1;
        x->h_other = srcI;
        t->node    = NULL;
        t->size    = -1;
        t->cur++;
    }
    else EP("Unexpected t->cur = %lld", t->cur);
    R srcI;
}

TBucket create_hash_multiply_v2(V x){
    L  *count      = HASH_AL(L , setT);
    L  *prefix     = HASH_AL(L , setT);
    HB *hashBucket = HASH_AL(HB, setT);
    L  *hashSize   = HASH_AL(L , setT);
    if(H_DEBUG) WP("v2 //Step 1: scan for basic info\n");
tic();
    DOI(xn, count[vLL(x,i)&setN]++)
    DOIa(setT, prefix[i]=prefix[i-1]+count[i-1])
toc();
    if(H_DEBUG) WP("v2 //Step 2: create partitions (setT=%lld)\n",(L)setT);
tic();
    DOI(setT, { \
        L hashLen=count[i]==0?0:getHashTableSize(count[i]); \
        hashBucket[i]=(hashLen>0)?createHashBucket(hashLen):NULL; \
        hashSize[i]=hashLen;})
toc();
    //test_hashSize(hashSize);
    if(H_DEBUG) WP("v2 //Step 3: insert items (%lld)\n",xn);
tic();
     //DOI(xn, {L id=vL(x,i)&setN; \
         insertHashMany_v2(hashBucket[id],hashSize[id]-1,xg,i,NULL,-1,H_L);})
    DOI(xn, {LL id=vLL(x,i)&setN; \
        insert_hash_int_v2(hashBucket[id],hashSize[id]-1,sLL(x),i);})
toc();
    // return
    TBucket tb;
    tb.hashBucket = hashBucket;
    tb.hashSize   = hashSize;
    tb.numBucket  = setT;
    R tb;
}

static L insertHashMany_v2(HB ht, L htMask, void* src, L srcI, void* val, L valI, I typ){
    R insert_hash_v2(ht,htMask,src,srcI,typ);
}

static HN find_hash_value_multiple_v2(HB hashT, V *src, L hashLen, L n, LL *args){
    LL val = args[0];
    L hashMask = hashLen - 1;
    L hashKey  = hash_LL(val);
    L hashId   = hashKey & hashMask;
    HB t = bV(hashT, hashId);
    //P(" n = %lld, begin with t->cur = %lld\n",n,t->cur);
    //P("\t"); DOI(n, P("%d ",args[i])) P("\n");
    DOI(t->cur, {HN x=(i<PRE_SIZE)?t->nd0+i:t->node+i-PRE_SIZE; \
        if(x->h_i==val && isAllMatch(n,src,hD(x),args))R x;})
    //Note: h_hash is currently not used in lookup
    //P("hash = %lld (%lld), index = %d (%d)\n",x->h_hash,hashKey,x->h_i,val);
    //if(x->h_hash==hashKey && x->h_i==val && isAllMatch(n,src,hD(x),args))R x;})
    //DOI(t->cur, {HN x=t->node+i;\
       if(x->h_hash==hashKey&&x->h_l==val&&isAllMatch(n,src,hD(x),args))R x;})
    //P("done\n"); getchar();
    R 0;
}

static HN find_hash_value_v2(HB hashT, V *src, L hashLen, L n, LL *args){
    if(1==n) R find_hash_value_single_v2(hashT, hashLen, args[0]);
    else R find_hash_value_multiple_v2(hashT, src, hashLen, n, args);
}

HN findValueFromHashV2(HB *hashT, V *src, L *hashSize, L n, ...){
    readArgs();
    //P("Input with %lld:\n",n);
    //DOI(n, P("args[%lld] = %lld\n",i,args[i]))
    LL hashSetId  = args[0] & setN;
    R find_hash_value_v2(hashT[hashSetId],src,hashSize[hashSetId],n,args);
}


HN find_hash_value_single_v2(HB ht, L hashLen, LL val){
    L  hashMask = hashLen - 1;
    //LL hashKey  = hash_LL(val);
    L hashKey = hash_LL(val) & hashMask;
    HB t = bV(ht, hashKey);
    //if(srcI == 14){ // add a new param
    //    P("hashValue: %lld, hashKey=%lld, hashMask = %lld\n", hash_LL(val),hashKey,hashMask);
    //    P("t->cur: %lld\n", t->cur);
    //    DOI(t->cur, {HN x=t->nd0+i; P("val[%lld] = %d\n",i,x->h_i);})
    //    getchar();
    //}
    if(t->cur<=PRE_SIZE){
        DOI(t->cur, {HN x=t->nd0+i; if(x->h_i==val) R x;})  // <---- nd0 or node !!!!
    }
    else {
        DOI(PRE_SIZE,        {HN x=t->nd0 +i; if(x->h_i==val) R x;})  // <---- nd0 or node !!!!
        DOI(t->cur-PRE_SIZE, {HN x=t->node+i; if(x->h_i==val) R x;})  // <---- nd0 or node !!!!
        //DOI(t->cur, {HN x=(i<PRE_SIZE)?t->nd0+i:t->node+i-PRE_SIZE; \
                if(x->h_i==val) R x;})  // <---- nd0 or node !!!!
    }
    //if(x->h_hash==hashKey&&x->h_i==val)R x;})
    R 0;
}

static I lib_join_radix_hash_v2(V z0, V z1, V x, V y){
    printBanner("Radix-based hash join v2");
    getInfoVar(x); getInfoVar(y); //getchar();
    if(vp(x)==H_LL && vp(y)==H_LL){
        HN *tempHN = HASH_AL(HN, vn(y));
    tic();
        TBucket tb = create_hash_multiply_v2(x);
        HB *hashTable = tb.hashBucket;
        L  *hashSize  = tb.hashSize;
    time_toc("> Radix hash, build phase\n");
    //DOI(50, P("hash size [%4lld]: %lld * sizeof(HB0) (%lld) = %lld KB\n",\
                i,hashSize[i],(L)sizeof(HB0),hashSize[i]*sizeof(HB0)/1024)) getchar();
        L c = 0;
    tic();
        DOI(vn(y), { LL id=vLL(y,i)&setN; \
            HN h1=find_hash_value_single_v2(hashTable[id],hashSize[id],vLL(y,i));\
            if(h1) c+=h1->h_num; tempHN[i]=h1;})
    time_toc("> Radix hash, probe phase\n");
        initV(z0, H_L, c);
        initV(z1, H_L, c);
    tic();
        c = 0;
        DOJ(vn(y), {HN h1=tempHN[j]; \
            if(h1){ L n1=h1->h_num; \
            L *d1 = n1==1?(L*)&(h1->h_other):n1>1?(L*)h1->h_other:NULL; \
            /*P("[%lld][%lld] with size %lld\n",d1[0],j,n1); getchar();*/ \
            DOI(n1, {vL(z0,c)=d1[i]; vL(z1,c)=j; c++;})}})
    time_toc("> Radix hash, write phase\n");
        P("Total elements write = %lld\n", c); // expected for Q5 3rd join: 46008
        R 0;
    }
    else {
        EP("Type not supported: %s,%s",getTypeName(vp(x)),getTypeName(vp(y)));
        R 1;
    }
}

#define runHashJoin(fn) {L hashCur=getHashHeap();I e=fn;setHashHeap(hashCur); R e;}
/*
 * Radix-based hash join
 *   build a private hash for each partition (total setT)
 *   - v1: conservative approach
 *   - v2: consecutive memory (more compact)
 */
static I lib_join_radix_hash(V z0, V z1, V x, V y){
    // replaced with ---> runHashJoin(lib_join_radix_hash_v2(z0,z1,x,y));
    L hashCur = getHashHeap();
    I status = lib_join_radix_hash_v2(z0,z1,x,y);
    setHashHeap(hashCur);
    R status;
}

I lib_join_index_hash(V z0, V z1, V x, V y, B isEq){
    //lib_join_dummy(x,y); getchar();
    if(isEq) R lib_join_radix_hash(z0,z1,x,y);
    else R lib_join_index_basic(z0,z1,x,y,isEq);
}

I exp_join_q5_v2_c1_sub(V z0, V z1, V x, V f, V y){
    printBanner("Radix-based hash join exp_join_q5_v2_c1");
    //getInfoVar(x); getInfoVar(y); getchar();
    if(vp(x)==H_LL && vp(y)==H_LL){
        HN *tempHN = HASH_AL(HN, vn(y));
    tic();
        TBucket tb = create_hash_multiply_v2(x);
        HB *hashTable = tb.hashBucket;
        L  *hashSize  = tb.hashSize;
    time_toc("> Radix hash, build phase\n");
        //getchar();
        L c = 0;
    tic();
        DOI(vn(y), if(vB(f,i)){LL id=vLL(y,i)&setN; \
            HN h1=find_hash_value_single_v2(hashTable[id],hashSize[id],vLL(y,i));\
            if(h1) c+=h1->h_num; tempHN[i]=h1;})
    time_toc("> Radix hash, probe phase\n");
        initV(z0, H_L, c);
        initV(z1, H_L, c);
    tic();
        P("Total elements found = %lld\n", c); // expected for Q5 3rd join: 46008
        c = 0;
        DOJ(vn(y), if(vB(f,j)){HN h1=tempHN[j]; \
            if(h1){ L n1=h1->h_num; \
            L *d1 = n1==1?(L*)&(h1->h_other):n1>1?(L*)h1->h_other:NULL; \
            /*P("[%lld][%lld] with size %lld\n",d1[0],j,n1); getchar();*/ \
            DOI(n1, {vL(z0,c)=d1[i]; vL(z1,c)=j; c++;})}})
    time_toc("> Radix hash, write phase\n");
        P("Total elements write = %lld\n", c); // expected for Q5 3rd join: 46008
        R 0;
    }
    else {EP("type not supported: %s,%s",getTypeName(vp(x)),getTypeName(vp(y)));R 1;}
}

I exp_join_q5_v2_c1(V z, V *x){
    V x0 = x[0]; // t53
    V x1 = x[1]; // t73
    V x2 = x[2]; // k0
    initV(z, H_G, 2);
    runHashJoin(exp_join_q5_v2_c1_sub(vV(z,0),vV(z,1),x0,x1,x2)); // single equal join
}

I exp_join_q5_v2_c2(V x){
    L t=0; DOI(xn, t+=hash_LL(vLL(x,i))) R t>100;
}

#endif
