#include "../global.h"

#ifdef DO_HASH_V1

#include "h_hash_common.h" // common hash functions

TBucket createHashWithV1(L n, V *x, L *hashLen){
    if(n >= 1) R create_hash_multiply_v1(x[0]);
    else EP("Number of columns invalid: %lld", n);
}

// same as insert_index_v2
static L insert_index_v1(HN t, L td){
    const L SLOT_SIZE = 64;
    //P("insert_index_v1 stop here: h_num=%lld\n",t->h_num); getchar();
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

// learn from insert_hash_v2
static L insert_hash_int_v1(HB ht, L htMask, LL *src, L srcI, LL id){
    LL val = src[srcI];
    L hashKey = hash_LL(val) & htMask;
    HB t = bV(ht, hashKey);
    //if(id==73){
//P("t->cur = %lld, size = %lld, val = %d, key=%lld\n", t->cur,t->size,val,hashKey);
    //}
    if(t->cur > 0){
        DOI(t->cur, {HN x=t->node+i; if(x->h_i==val) R insert_index_v1(x,srcI);})
        if(t->cur+1<t->size){
            HN x=t->node+(t->cur);
            x->h_index = srcI;
            x->h_i     = val;
            x->h_num   = 1;
            x->h_other = srcI;
            t->cur++;
        }
        else EP("Size not enough, need to grow: %lld / %lld", t->cur, t->size);
    }
    else {
        #define INIT_BUCKET_SIZE 8
        HN x = newHashNodeN(INIT_BUCKET_SIZE);
        x->h_index = srcI;
        x->h_i     = val;
        x->h_num   = 1;
        x->h_other = srcI;
        t->node = x;
        t->cur  = 1;
        t->size = INIT_BUCKET_SIZE;
    }
    R srcI;
}

TBucket create_hash_multiply_v1(V x){
    L  *count      = HASH_AL(L , setT);
    L  *prefix     = HASH_AL(L , setT);
    HB *hashBucket = HASH_AL(HB, setT);
    L  *hashSize   = HASH_AL(L , setT);
    if(H_DEBUG) WP("v1: //Step 1: scan for basic info\n");
tic();
    DOI(xn, count[vLL(x,i)&setN]++)
    DOIa(setT, prefix[i]=prefix[i-1]+count[i-1])
toc();
    if(H_DEBUG) WP("v1: //Step 2: create partitions (setT=%lld)\n",(L)setT);
tic();
    DOI(setT, { \
        L hashLen=count[i]==0?0:getHashTableSize(count[i]); \
        hashBucket[i]=(hashLen>0)?createHashBucket(hashLen):NULL; \
        hashSize[i]=hashLen;})
toc();
    //test_hashSize(hashSize);
    if(H_DEBUG) WP("v1 //Step 3: insert items (%lld)\n",xn);
tic();
     //DOI(xn, {L id=vL(x,i)&setN; \
         insertHashMany_v2(hashBucket[id],hashSize[id]-1,xg,i,NULL,-1,H_L);})
    DOI(xn, {LL id=vLL(x,i)&setN; \
        insert_hash_int_v1(hashBucket[id],hashSize[id]-1,sLL(x),i,id);})
toc();
    // return
    TBucket tb;
    tb.hashBucket = hashBucket;
    tb.hashSize   = hashSize;
    tb.numBucket  = setT;
    R tb;
}

/* Hash node with HN */
static L lib_join_radix_hash_v1(V z0, V z1, V x, V y){
    printBanner("Radix-based hash join v1");
    if(vp(x)==H_LL && vp(y)==H_LL){
        HN *tempHN = HASH_AL(HN, vn(y));
    tic();
        TBucket tb = create_hash_multiply_v1(x);
        HB *hashTable = tb.hashBucket;
        L  *hashSize  = tb.hashSize;
    time_toc("v1> Radix hash, build phase\n");
        L c = 0;
    tic();
        DOI(vn(y), {LL id=vLL(y,i)&setN; /*P("i=%lld, id=%d\n",i,id);*/ \
            HN h1=find_hash_value_single_v1(hashTable[id],hashSize[id],vLL(y,i));\
            if(h1) c+=h1->h_num; tempHN[i]=h1;})
    time_toc("v1> Radix hash, probe phase\n");
        initV(z0, H_L, c);
        initV(z1, H_L, c);
    tic();
        c = 0;
        DOJ(vn(y), {HN h1=tempHN[j]; \
            if(h1){ L n1=h1->h_num; \
            L *d1 = n1==1?(L*)&(h1->h_other):n1>1?(L*)h1->h_other:NULL; \
            DOI(n1, {vL(z0,c)=d1[i]; vL(z1,c)=j; c++;})}})
    time_toc("v1> Radix hash, write phase\n");
        P("Total elements write = %lld\n", c); // expected for Q5 3rd join: 46008
        R 0;

    //TIME_BEGIN();
    //    L parZ[H_CORE], offset[H_CORE]; 
    //    memset(parZ  , 0, sizeof(L)*H_CORE);
    //    memset(offset, 0, sizeof(L)*H_CORE);
    //    DOT(vn(y), if(tempK[i]>=0){parZ[tid]++; HI t0=tempH[i]; while(t0){parZ[tid]++;t0=t0->inext;}})
    //    DOI(H_CORE, c+=parZ[i])
    //    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    //    initV(z0, H_L, c);
    //    initV(z1, H_L, c);
    //    DOT(vn(y), if(tempK[i]>=0){L p=offset[tid]; \
    //            vL(z0,p)=tempK[i];vL(z1,p)=i; \
    //            HI t0=tempH[i]; while(t0){p++;vL(z0,p)=t0->ival;vL(z1,p)=i;t0=t0->inext;} offset[tid]=p+1; })
    //TIME_END();
    //TIME_SHOW("> Radix hash, final:"); PP("total c = %lld\n",c);
    }
    else {
        EP("Type not supported: %s,%s",getTypeName(vp(x)),getTypeName(vp(y)));
        R 1;
    }
}

static HN find_hash_value_multiple_v1(HB hashT, V *src, L hashLen, L n, LL *args){
    LL val = args[0];
    L hashMask = hashLen - 1;
    L hashKey  = hash_LL(val);
    L hashId   = hashKey & hashMask;
    HB t = bV(hashT, hashId);
    DOI(t->cur, {HN x=t->node+i; \
        if(x->h_i==val && isAllMatch(n,src,hD(x),args))R x;})
    R 0;
}

static HN find_hash_value_v1(HB hashT, V *src, L hashLen, L n, LL *args){
    if(1==n) R find_hash_value_single_v1(hashT, hashLen, args[0]);
    else R find_hash_value_multiple_v1(hashT, src, hashLen, n, args);
}

HN find_hash_value_single_v1(HB ht, L hashLen, LL val){
    L hashMask = hashLen - 1;
    L hashKey  = hash_LL(val) & hashMask;
    HB t = bV(ht, hashKey);
    DOI(t->cur, {HN x=t->node+i; if(x->h_i==val) R x;})
    R 0;
}

static I lib_join_radix_hash(V z0, V z1, V x, V y){
    L hashCur = getHashHeap();
    I status = lib_join_radix_hash_v1(z0,z1,x,y);
    setHashHeap(hashCur);
    R status;
}

HN findValueFromHashV1(HB *hashT, V *src, L *hashSize, L n, ...){
    readArgs();
    LL hashSetId  = args[0] & setN;
    R find_hash_value_v1(hashT[hashSetId],src,hashSize[hashSetId],n,args);
}

I lib_join_index_hash(V z0, V z1, V x, V y, B isEq){
    //lib_join_dummy(x,y); getchar();
    if(isEq) R lib_join_radix_hash(z0,z1,x,y);
    else R lib_join_index_basic(z0,z1,x,y,isEq);
}

#endif

