#include "../global.h"

/* 
 * main functions
 *   lib_join_index_hash    :  entry function
 *
 * debugging functions
 *   lib_join_dummy         :  verify join results (brute force)
 */

/* declared in h_lib.h: HI, HN */

const I HASH_A = 1223106847;
const I HASH_M = (I)((1LL << 32)-5);

L *LARGE_BUFF; // used in merge sort

static void writeToFileFromPtr(L *x, L n, const char *fn){
    FILE *fp = fopen(fn, "w");
    FP(fp, "%lld\n",n);
    DOI(n, FP(fp,"%lld\n",x[i]))
    fclose(fp);
    WP("file %s is saved\n", fn);
}

static void writeToFileForDebug(V x, const char *file_name){
    if(xp==H_L){
        writeToFileFromPtr(sL(x),xn,file_name);
    }
    else EP("H_L is expected");
}

#include "h_hash_common.h"

/*
 * A method name starts with "lib" and ends with a specific type
 * e.g. libIndexOf_I
 */

/*
 * Hash tables
 * http://stackoverflow.com/questions/9624963/java-simplest-integer-hash
 */

/*
UI quickMod(L x){
    L j = x >> 32;
    L k = x & 0xffffffffL;
    return j*5+k;
}

UI hash_i32(I n){
    I  h = HASH_A*(n&0xffffffffL);
    I  h2 = quickMod(quickMod(h));
    UI rtn =  (UI) (h2 >= HASH_M ? (h2-HASH_M) : h2);
    return rtn;
}

UI hash_i64(L n){
    I high = (I)(0x00000000ffffffffLL & (n>>32));
    I low  = (I)(0x00000000ffffffffLL & n);
    R hash_i32(high) + hash_i32(low);
}
*/

/*
static UL hash_i64(L a){
    a = (~a) + (a << 21); // key = (key << 21) - key - 1;
    a = a ^ (a >> 24);
    a = (a + (a << 3)) + (a << 8); // key * 265
    a = a ^ (a >> 14);
    a = (a + (a << 2)) + (a << 4); // key * 21
    a = a ^ (a >> 28);
    a = a + (a << 31);
    return a;
}*/

//UI getHashTableSize(L x){
//    R (x<16?32:(x<<1));
//}

static UI getHashValueComplete(void *val, L valI, L typ){
    switch(typ){
        caseJ R hash_J(toJ(val,valI));
        caseH R hash_H(toH(val,valI));
        caseI R hash_I(toI(val,valI));
        caseL R hash_L(toL(val,valI));
        caseF R hash_F(toF(val,valI));
        caseE R hash_E(toE(val,valI));
        caseX R hash_X(toX(val,valI));
        caseS R hash_S(toS(val,valI));
        caseG { V x=(V)val; UI c=0;
            DOI(vn(x), {V y=vV(x,i); DOJ(vn(y),c+=getHashValueComplete(isList(y)?(void *)y:sG(y),j,vp(y)))}) R c; }
        default: EP("Type not supported: %lld", typ);
    }
}

static UI getHashValue(void *val, L valI, L typ){
    /* row based, val[valI] */
    switch(typ){
        caseG {V x=(V)val; UI c=0; 
              DOI(vn(x), {V y=vV(x,i); c+=getHashValueComplete(isList(y)?(void *)y:sG(y),valI,vp(y));}) R c; }
        default: R getHashValueComplete(val, valI, typ);
    }
}

static I createHash(HN *hashT, L htSize){
    *hashT = (HN) allocHashMem(sizeof(HN0) * htSize);
    //WP("mem size: %lld (%lld * %lld)\n", sizeof(HN0)*htSize,sizeof(HN0),htSize);
    //memset(*hashT, 0, sizeof(HN0)*htSize); // cleared (1) begin, and (2) exit
    R 0;
}

// HB createHashBucket(L htSize){
//     HB hashT = HASH_AL(HB0, htSize);
//     memset(hashT, 0, sizeof(HN0)*htSize);
//     R hashT;
// }

/* free a hash table */
L freeHash(HN hashT){
    R 0;
}

static HN newHashNode(){
    //HN x = (HN) allocHashMem(sizeof(HN0));
    HN x = HASH_AL1(HN0);
    init_H(x);
    R x;
}

// static HN newHashNodeN(L n){
//     HN x = HASH_AL(HN0, n);
//     init_H(x);
//     R x;
// }

static HI newIndexNode(){
    //HI x = (HI) allocHashMem(sizeof(HI0));
    HI x = HASH_AL1(HI0);
    init_I(x);
    R x;
}

L insert_index(HN t, L td){
    HI t0 = newIndexNode();
    t0->ival = td;
    if(hT(t)){
        HI p = hT(t);
        while(p->inext) p=p->inext;
        p->inext = t0;
    }
    else {
        hT(t) = t0;
    }
    R td;
}

/* isU: isUnique */
static I insert_hash(HN ht, L htMask, void* src, L srcI, I typ, B isU){
    L hashKey = getHashValue(src,srcI,typ) & htMask;
    HN t = hV(ht,hashKey);
    while(hN(t)){
        t = hN(t); L td = hD(t);
        switch(typ){
            caseJ if(toJ(src,td)==toJ(src,srcI)) R isU?td:insert_index(t,srcI); break;
            caseH if(toH(src,td)==toH(src,srcI)) R isU?td:insert_index(t,srcI); break;
            caseI if(toI(src,td)==toI(src,srcI)) R isU?td:insert_index(t,srcI); break;
            caseL if(toL(src,td)==toL(src,srcI)) R isU?td:insert_index(t,srcI); break;
            caseF if(toF(src,td)==toF(src,srcI)) R isU?td:insert_index(t,srcI); break;
            caseE if(toE(src,td)==toE(src,srcI)) R isU?td:insert_index(t,srcI); break;
            caseX if(xEqual(toX(src,td),toX(src,srcI)))
                     R isU?td:insert_index(t,srcI); break;
            caseS if(sEQ(toS(src,td),toS(src,srcI)))
                     R isU?td:insert_index(t,srcI); break;
            caseG if(compareTuple((V)src,td,(V)src,srcI))
                     R isU?td:insert_index(t,srcI); break;
            default: R -1; /* ... */
        }
    }
    HN x = newHashNode();
    hD(x) = srcI;
    hN(t) = x;
    R srcI;
}

static L find_hash(HN ht, L htMask, void* src, void* val, L valI, L typ){
    L hashKey = getHashValue(val,valI,typ) & htMask;
    HN t = hV(ht,hashKey);
    while(hN(t)){
        t = hN(t); L td = hD(t);
        switch(typ){
            caseJ if(toJ(src,td)==toJ(val,valI)) R td; break;
            caseH if(toH(src,td)==toH(val,valI)) R td; break;
            caseI if(toI(src,td)==toI(val,valI)) R td; break;
            caseL if(toL(src,td)==toL(val,valI)) R td; break;
            caseF if(toF(src,td)==toF(val,valI)) R td; break;
            caseE if(toE(src,td)==toE(val,valI)) R td; break;
            caseX if(xEqual(toX(src,td),toX(val,valI)))  R td; break;
            caseS if(sEQ(toS(src,td),toS(val,valI))) R td; break;
            caseG if(compareTuple((V)src,td,(V)val,valI))R td; break;
            default: R -1;
        }
    }
    R -1;
}

#ifdef PROFILE_HASH_JOIN
static L temp_search_counter = 0;
#endif

static L find_hash_many(HN ht, L htMask, void* src, void* val, L valI, L typ, HI *other){
    L hashKey = getHashValue(val,valI,typ) & htMask;
    HN t = hV(ht,hashKey);
    *other = NULL; // default
#ifdef PROFILE_HASH_JOIN
    temp_search_counter++;
#endif
    while(hN(t)){
        t = hN(t); L td = hD(t);
        switch(typ){
            caseJ if(toJ(src,td)==toJ(val,valI)) {*other=hT(t);R td;} break;
            caseH if(toH(src,td)==toH(val,valI)) {*other=hT(t);R td;} break;
            caseI if(toI(src,td)==toI(val,valI)) {*other=hT(t);R td;} break;
            caseL if(toL(src,td)==toL(val,valI)) {*other=hT(t);R td;} break;
            caseF if(toF(src,td)==toF(val,valI)) {*other=hT(t);R td;} break;
            caseE if(toE(src,td)==toE(val,valI)) {*other=hT(t);R td;} break;
            caseX if(xEqual(toX(src,td),toX(val,valI)))  {*other=hT(t);R td;} break;
            caseS if(sEQ(toS(src,td),toS(val,valI))) {*other=hT(t);R td;} break;
            caseG if(compareTuple((V)src,td,(V)val,valI)){*other=hT(t);R td;} break;
            default: R -1;
        }
#ifdef PROFILE_HASH_JOIN
        temp_search_counter++;
#endif
    }
    R -1;
}

//L findHash(HN ht, L htSize, void* src, L srcI, void* val, L valI, L typ){
//    R find_hash(ht,htSize,src,val,valI,typ);
//}

static L insertHash(HN ht, L htMask, void* src, L srcI, void* val, L valI, I typ){
    R insert_hash(ht,htMask,src,srcI,typ,true);
}

static L insertHashMany(HN ht, L htMask, void* src, L srcI, void* val, L valI, I typ){
    R insert_hash(ht,htMask,src,srcI,typ,false);
}

L profileHash(HN ht, L htSize){
    L minValue=99999, maxValue=-1, nonZero=0, totElem=0; 
    DOI(htSize, {L c=0; HN t=hV(ht,i); while(hN(t)){c++;t=hN(t);} \
        if(c!=0) nonZero++; totElem+=c; \
        if(c>0 && c<minValue) minValue=c; \
        if(c>maxValue) maxValue=c; })
    WP("\n");
    printBanner("Hash table profiling report");
    WP("> Hash table size   : %lld\n",htSize);
    WP("> Hash table density: %g%% (%lld/%lld)\n",nonZero*100.0/htSize,nonZero,htSize);
    WP("> No. items per line: [%lld, %lld]\n",minValue,maxValue);
    WP("> Avg elem. per line: %.1lf (%lld/%lld)\n",totElem*1.0/nonZero,totElem,nonZero);
    B isVerbose=false;
    if(isVerbose){
        L c=0, c2=0, c3=0;
        DOI(htSize, {HN t=hN(hV(ht,i)); \
            while(t){HI p=hT(t); L k=0; \
              while(p){k++; p=p->inext;} c+=k+1; c2+=k; c3+=k>0; t=hN(t);}})
        WP("> total = %lld, print (%lld / %lld) = %.1lf\n", c,c2,c3,1.0*c2/c3);
    }
    WP("\n"); R 0;
}

static L profileInt(V x){
    if(xp == H_LL){
        L vmax = LLONG_MIN, vmin = LLONG_MAX;
        DOI(vn(x), {if(vmax<vL(x,i))vmax=vL(x,i); if(vmin>vL(x,i))vmin=vL(x,i);})
        WP(">> len=%lld, max=%lld, min=%lld\n", xn,vmax,vmin);
    }
    else WP("type not supported: %s\n", getTypeName(xp));
    R 0;
}


/* functions exposed as libraries */

// #define lib_index_template(typ) \
//     WP("......"); getchar(); \
//     HN hashT; \
//     L hashLen = getHashTableSize(sLen); \
//     CHECKE(createHash(&hashT,hashLen)); \
//     DOI(sLen, insertHash(hashT,hashLen,src,i,NULL,-1,typ)) \
//     struct timeval tv0, tv1; E optTime[10];\
//     DOJ(10, {gettimeofday(&tv0, NULL); DOP(vLen, {L t=findHash(hashT,hashLen,src,-1,val,i,typ);targ[i]=t<0?sLen:t;}) \
//     gettimeofday(&tv1, NULL); optTime[j] = calcInterval(tv0, tv1)/1000.0;}) \
//     E total = 0; DOI(10, total += optTime[i]) WP("Createing index time (avg): %g ms\n", total/10);

#define lib_index_template(typ) \
    HN hashT; \
    L hashLen  = getHashTableSize(sLen); \
    L hashMask = hashLen - 1; \
    L hashCur = getHashHeap(); if(H_DEBUG)WP("size1 = %lld, %lld\n",hashLen,hashCur); \
    CHECKE(createHash(&hashT,hashLen)); \
    DOI(sLen, insertHash(hashT,hashMask,src,i,NULL,-1,typ)) \
    DOI(vLen, {L t=find_hash(hashT,hashMask,src,val,i,typ);targ[i]=t<0?sLen:t;}) \
    setHashHeap(hashCur);

I lib_index_of_B(L* targ, B* src, L sLen, B* val, L vLen){
    L flag[2]={-1}; I c=0;
    DOI(sLen, if(c<2 && -1==flag[src[i]]){flag[src[i]]=i;c++;if(2==c)break;})
    DOP(vLen, {L t=flag[val[i]]; targ[i]=t<0?sLen:t;})
    R 0;
}

I lib_index_of_J(L* targ, J* src, L sLen, J* val, L vLen){
    lib_index_template(H_J);
    R 0;
}

I lib_index_of_H(L* targ, H* src, L sLen, H* val, L vLen){
    lib_index_template(H_H);
    R 0;
}

I lib_index_of_I(L* targ, I* src, L sLen, I* val, L vLen){
    lib_index_template(H_I);
    R 0;
}

I lib_index_of_L(L* targ, L* src, L sLen, L* val, L vLen){
    lib_index_template(H_L);
    R 0;
}

// void lib_index_of_L(L* targ, L* src, L sLen, L* val, L vLen){
//     DOI(vLen, {B f=1;\
//         DOJ(sLen, if(val[i]==src[j]){targ[i]=j;f=0;break;})\
//         if(f)targ[i]=sLen;})
// }

I lib_index_of_F(L* targ, F* src, L sLen, F* val, L vLen){
    lib_index_template(H_F);
    R 0;
}

I lib_index_of_E(L* targ, E* src, L sLen, E* val, L vLen){
    lib_index_template(H_E);
    R 0;
}

I lib_index_of_C(L* targ, C* src, L sLen, C* val, L vLen){
    L buff[256];
    memset(buff,-1,256*sizeof(L));
    DOI(sLen, if(buff[src[i]]==-1) buff[src[i]]=i)
    DOI(vLen, {L t=buff[val[i]]; targ[i]=-1==t?sLen:t;})
    R 0;
}

I lib_index_of_X(L* targ, X* src, L sLen, X* val, L vLen){
    lib_index_template(H_X);
    R 0;
}

I lib_index_of_S(L* targ, S* src, L sLen, S* val, L vLen){
    lib_index_template(H_S);
    R 0;
}

I lib_index_of_G(L* targ, V src, L sLen, V val, L vLen){
    lib_index_template(H_G);
    R 0;
}

void lib_quicksort(L *rtn, V val, L low, L high, B *isUp, FUNC_CMP(cmp)){
    //WP("low = %lld, high = %lld, type = %s\n", low,high,getTypeName(vp(val)));
    switch(vp(val)){
        caseC lib_quicksort_char(rtn, val, low , high, isUp, cmp); break;
        caseI lib_radixsort_int (rtn, val, high, isUp, true); break;
        caseL lib_radixsort_long(rtn, val, high, isUp, true); break;
        default: //WP("sorting on type: %s\n",getTypeName(vp(val)));
                 lib_quicksort_other(rtn, val, low, high, isUp, cmp); break;
    }
}

void lib_quicksort_char(L *rtn, V val, L low, L high, B *isUp, FUNC_CMP(cmp)){
    S str = sC(val);  L len = high-low;  B f0 = isUp?*isUp:1; //isUp?1:*isUp;
    if(len < 10){
        DOI3(low, high, DOJ3(low, high, if(str[rtn[i]] > str[rtn[j]]) { L t=rtn[i]; rtn[i]=rtn[j]; rtn[j]=t; } ))
    }
    else {
        if(H_DEBUG) WP("entering large char sort: len = %lld\n", len);
        L cnt[256]={0}, step[256]={0}, cursor=0;
        L *temp = (L*)malloc(sizeof(L)*len);
        DOI3(low, high, cnt[str[rtn[i]]]++);
        DOI(256, {if(cnt[i]!=0) step[i] = cursor; cursor += cnt[i]; })
        if(H_DEBUG){
            DOI(256, if(step[i]!=0)WP("step[%lld] = %lld\n",(L)i,(L)step[i]))
        }
        DOI3(low, high, temp[step[str[rtn[i]]]++]=rtn[i])
        memcpy(rtn+low, temp, sizeof(L)*len);
        free(temp);
    }
    if(!f0){ // desc order
        DOI3(low, (low+high)/2, { L t=rtn[i]; rtn[i]=rtn[high-i+1]; rtn[high-i+1]=t; })
    }
}

static L lib_partition(L *rtn, V val, L low, L high, B *isUp, FUNC_CMP(cmp), B *leftSame){
    L pivot = low, i = low, j = high, t;
    while(true){
        while(++i && i<high){
            L temp = (*cmp)(val,rtn[i],rtn[pivot],isUp);
            if(temp != 0) *leftSame = false;
            if(temp > 0) break;
        }
        do{--j;} while(j>=0 && ((*cmp)(val,rtn[j],rtn[pivot],isUp)>0));
        if(i>=j) break;
        t = rtn[i]; rtn[i] = rtn[j]; rtn[j] = t;
    }
    if(!(*leftSame)){ t = rtn[low]; rtn[low] = rtn[j]; rtn[j] = t; }
    R j;
}


void lib_quicksort_other(L *rtn, V val, L low, L high, B *isUp, FUNC_CMP(cmp)){
    if(low < high){
        B leftSame=true;
        L pos = lib_partition(rtn, val, low, high, isUp, cmp, &leftSame);
        //if(leftSame) { WP("low = %lld, high = %lld\n",low,high); getchar(); }
        //WP("low = %lld, high = %lld, %lld, %lld\n",low,high,leftSame,pos);
//#pragma omp parallel sections
        {
//#pragma omp section
        if(!leftSame)
            lib_quicksort_other(rtn, val, low, pos, isUp, cmp);
//#pragma omp section
        if(pos<high)
            lib_quicksort_other(rtn, val, pos+1, high,isUp, cmp);
        }
    }
}


/* f(0) desc; f(1) asc */
#define cmp_switch(x) (x?-1:1):(x?1:-1)
//#define cmp_switch(x) (-1):(1)

L lib_quicksort_cmp(V val, L a, L b, B *isUp){
    DOI(vn(val), {V t=vV(val,i); B f=isUp?isUp[i]:1; \
                  L x=lib_quicksort_cmp_item(t,a,b,&f); if(x!=0)R x;})
    R 0;
}

L lib_quicksort_cmp_item(V t, L a, L b, B *isUp){
    B f = isUp?*isUp:1;
    // switch(vp(t)){
    //     caseH if(f==0 && vH(t,a)==1992){
    //         WP("1992: a = %d, b = %d\n",vH(t,a),vH(t,b)); getchar();} break;
    // }
    #define SORT_CMP(q) case##q if(v##q(t,a)!=v##q(t,b)) R v##q(t,a)<v##q(t,b)?cmp_switch(f); break
    switch(vp(t)){ \
        SORT_CMP(B); \
        SORT_CMP(J); \
        SORT_CMP(H); \
        SORT_CMP(I); \
        SORT_CMP(L); \
        SORT_CMP(F); \
        SORT_CMP(E); \
        SORT_CMP(C); \
        caseS {L tmp=strcmp(vS(t,a),vS(t,b)); if(tmp) R tmp<0?cmp_switch(f);} break; \
        caseQ if(vQ(t,a)!=vQ(t,b)) R compareSymbol(vQ(t,a),vQ(t,b))<0?cmp_switch(f); break; \
        SORT_CMP(M); \
        SORT_CMP(D); \
        SORT_CMP(Z); \
        SORT_CMP(U); \
        SORT_CMP(W); \
        SORT_CMP(T); \
        default: EP("No impl. for type %s\n",getTypeName(vp(t))); \
        /* Pending: caseC */
    } 
    // R (a>b?1:a<b?-1:0);
    #ifdef SORT_MERGE
        R a>b?cmp_switch(f); //think..
    #else
        R 0;
    #endif
}

static void lib_mergesort(L *rtn, V val, L low, L high, B *isUp, FUNC_CMP(cmp)){
    //WP("Entering merge sort: (%lld, %lld)\n",low,high);
    // #define SWAP_I(a,b) { L temp=rtn[a]; rtn[a]=rtn[b]; rtn[b]=temp; }
    #define SWAP_I(a,b) { rtn[a]^=rtn[b]; rtn[b]^=rtn[a]; rtn[a]^=rtn[b]; }
    // WP("type of val: %lld\n", vp(val));
    // WP("before:"); DOI3(low, high, WP(" %lld",vL(val,rtn[i]))); WP("\n");
    if(low < high - 1){
        if(low + 1 == high - 1){
            if((*cmp)(val,rtn[low],rtn[high-1],isUp)>0){
                SWAP_I(low, high-1);
            }
        }
        else {
            L mid = (low + high) >> 1, len = high-low;
            lib_mergesort(rtn, val, low, mid , isUp, cmp);
            lib_mergesort(rtn, val, mid, high, isUp, cmp);
            if((*cmp)(val,rtn[mid-1],rtn[mid],isUp)>0){ // need to sort
                L curLeft=low, curRight=mid;
                // L *buff = (L*)malloc(sizeof(L)*len);
                for(L i=0;i<len;i++){
                    if(curLeft == mid){
                        // WP("trace 1\n");
                        LARGE_BUFF[i]=rtn[curRight++];
                    }
                    else if(curRight == high){
                        // WP("trace 2\n");
                        LARGE_BUFF[i]=rtn[curLeft++];
                    }
                    else if((*cmp)(val,rtn[curLeft],rtn[curRight],isUp)>0){
                        // WP("trace 3\n");
                        LARGE_BUFF[i]=rtn[curRight++];
                    }
                    else {
                        // WP("trace 4\n");
                        LARGE_BUFF[i]=rtn[curLeft++];
                    }
                }
                memcpy(rtn+low,LARGE_BUFF,sizeof(L)*len);
                // WP("merged: %lld + %lld = %lld\n", mid-low, high-mid, len);
                // free(buff);
            } // else no need to sort
        }
    }
    // WP("after:"); DOI3(low, high, WP(" %lld",vL(val,rtn[i]))); WP("\n");
    // getchar();
    // WP("exit.\n");
}

// the same as lib_quicksort_list
static L lib_mergesort_decide(V val, L a, L b, B *isUp, FUNC_CMP(cmp)){
    DOI(vn(val), {L k=(*cmp)(vV(val,i),a,b,isUp+i); if(k!=0) R k; })
    R (a>b?1:a<b?-1:0);
}

static void lib_mergesort_list(L *rtn, V val, L low, L high, B *isUp, FUNC_CMP(cmp)){
    // WP("Entering merge sort: (%lld, %lld)\n",low,high);
    // #define SWAP_I(a,b) { L temp=rtn[a]; rtn[a]=rtn[b]; rtn[b]=temp; }
    #define SWAP_I(a,b) { rtn[a]^=rtn[b]; rtn[b]^=rtn[a]; rtn[a]^=rtn[b]; }
    // WP("type of val: %lld\n", vp(val));
    // WP("before:"); DOI3(low, high, WP(" %lld",vL(val,rtn[i]))); WP("\n");
    if(low < high - 1){
        if(low + 1 == high - 1){
            // if((*cmp)(val,rtn[low],rtn[high-1],isUp)>0){
            if(lib_mergesort_decide(val,rtn[low],rtn[high-1],isUp,cmp)>0){
                SWAP_I(low, high-1);
            }
        }
        else {
            L mid = (low + high) >> 1, len = high-low;
            lib_mergesort_list(rtn, val, low, mid , isUp, cmp);
            lib_mergesort_list(rtn, val, mid, high, isUp, cmp);
            // if((*cmp)(val,rtn[mid-1],rtn[mid],isUp)>0){ // need to sort
            if(lib_mergesort_decide(val,rtn[mid-1],rtn[mid],isUp,cmp)>0){
                // WP("1. low = %lld, high = %lld\n", low, high); // check
                L curLeft=low, curRight=mid;
                // L *buff = (L*)malloc(sizeof(L)*len);
                for(L i=0;i<len;i++){
                    if(curLeft == mid){
                        // WP("trace 1\n");
                        LARGE_BUFF[i]=rtn[curRight++];
                    }
                    else if(curRight == high){
                        // WP("trace 2\n");
                        LARGE_BUFF[i]=rtn[curLeft++];
                    }
                    // else if((*cmp)(val,rtn[curLeft],rtn[curRight],isUp)>0){
                    else if(lib_mergesort_decide(val,rtn[curLeft],rtn[curRight],isUp,cmp)>0){
                        // WP("trace 3\n");
                        LARGE_BUFF[i]=rtn[curRight++];
                    }
                    else {
                        // WP("trace 4\n");
                        LARGE_BUFF[i]=rtn[curLeft++];
                    }
                }
                memcpy(rtn+low,LARGE_BUFF,sizeof(L)*len);
                // WP("merged: %lld + %lld = %lld\n", mid-low, high-mid, len);
                // free(buff);
            } // else no need to sort
            else if(lib_mergesort_decide(val,rtn[low],rtn[high-1],isUp,cmp)>0){
                WP("2. low = %lld, high = %lld\n", (L)low, (L)high); // in q16, no case found
            }
        }
    }
    // WP("after:"); DOI3(low, high, WP(" %lld",vL(val,rtn[i]))); WP("\n");
    // getchar();
    // WP("exit.\n");
}

/*
 * check if it is a sorted vector by a given order, isUp
 * isUp: true  -> -1 (asc)
 *     : false ->  1 (desc)
 */
static B lib_order_by_sorted(L *targ, V val, B *isUp, L low, L high, FUNC_CMP(cmp)){
    L rtn = 0;
    #define DOIm(m, n, x) {for(L i=m+1,i2=n;i<i2;i++)x;}
    DOIm(low, high, {L t=(*cmp)(val,targ[i-1],targ[i],isUp); \
        rtn=rtn==0?t:rtn==-1?(t==1?-2:rtn):(t==-1?-2:rtn); \
        if(rtn==-2) { /*WP("i=%lld, %lld, %lld\n",i,vL(val,i-1),vL(val,i));*/ R 0;} })
    B isOrder= rtn==(*isUp?-1:1);
    if(isOrder && !(*isUp)){ // reverse order
        //WP("low = %lld, high = %lld\n", low,high); getchar();
        DOI3(low, (low+high)/2, \
            {L temp=targ[i]; L pos=high-(i-low)-1; targ[i]=targ[pos]; targ[pos] = temp;})
    }
    R isOrder;
}

void lib_order_by_list(L *targ, V val, B *isUp, L tLen, L colId, FUNC_CMP(cmp)){
    //WP("1.----%lld\n",tLen);
    DOI(tLen, targ[i]=i);
    //WP("2.----\n"); getchar();
    //#if defined(OPT_Q16) || defined(SORT_MERGE)
    //    /* quick sort */
    //    // lib_quicksort_other(targ, val, 0, tLen, isUp, lib_quicksort_cmp);
    //    /* merge sort (fast) */ 
    //    LARGE_BUFF = (L*)malloc(sizeof(L)*vn(vV(val,0)));
    //    lib_mergesort_list(targ, val, 0, tLen, isUp, cmp);
    //    free(LARGE_BUFF);
    //#else
        //WP("before: "); DOI(tLen, WP(" %lld",targ[i])) WP("\n");
        lib_quicksort_list(targ, val, isUp, 0, tLen, colId, cmp);
        //WP("after : "); DOI(tLen, WP(" %lld",targ[i])) WP("\n");
    //#endif
}

void lib_quicksort_list(L *targ, V val, B *isUp, L low, L high, L colId, FUNC_CMP(cmp)){
    if(colId >= vn(val)) R;
    V curV = vV(val,colId);
    B* curB = isUp+colId;
    if(lib_order_by_sorted(targ,curV,curB,low,high,cmp)){
        ;
    }
    else {
        //#ifdef OPT_Q16
        //    LARGE_BUFF = (L*)malloc(sizeof(L)*vn(curV));
        //    lib_mergesort(targ, curV, low, high, curB, cmp);
        //    free(LARGE_BUFF);
        //#else
            if(isIntegers(curV)) {
                LARGE_BUFF = (L*)malloc(sizeof(L)*vn(curV));
                lib_mergesort(targ, curV, low, high, curB, cmp);
                free(LARGE_BUFF);
            }
            else lib_quicksort(targ, curV, low, high, curB, cmp);
        //#endif
    }
    // gettimeofday(&tv1, NULL);
    // if(colId == 0)
    // WP("1. Result (elapsed time %g ms)\n\n", calcInterval(tv0,tv1)/1000.0);
    // gettimeofday(&tv0, NULL);
    {
        // WP("len: %lld - %lld = %lld\n", high, low, high-low); getchar();
        L pos = low;
        do{
            // find next
            L start = pos, end = high;
            L hend = start + 5, hpos = -1; // heuristic end/pos
            if(hend>high) hend=high;
            // WP("type of curV: %lld\n", vp(curV));
            DOI3(start+1, hend, { if(0!=(*cmp)(curV,targ[i-1],targ[i],curB)){ hpos=i; break; } })
            if(hpos >= 0){
                lib_quicksort_list(targ, val, isUp, start, hpos, colId+1, cmp);
                pos = hpos;
            }
            else if(hend == high){
                lib_quicksort_list(targ, val, isUp, start, high, colId+1, cmp);
                pos = high;
            }
            else {
                L pivot = start;
                do{
                    if(0 != (*cmp)(curV,targ[pivot],targ[end-1],curB)) {
                        L mid = (start + end) >> 1;
                        if(0 != (*cmp)(curV,targ[pivot],targ[mid],curB)) end = mid;
                        else start = mid+1;
                    }
                    else break;
                }while(start < end);
                // if(end < high)
                //     WP("pivot char: %c | %c\n", vC(curV, targ[pivot]), vC(curV, targ[end]));
                lib_quicksort_list(targ, val, isUp, pivot, end, colId+1, cmp);
                pos = end;
            }
        }while(pos < high);
    }
}

/*
 * The performance of qsort is affected by the size of node
 */
typedef struct compare_node_L { L i64; L p; }CNL,CN;
int cmp_L(const void *a, const void *b){
    R vl(((CNL*)a)) - vl(((CNL*)b));
}
static void lib_qsort_vector_L(L *val, L size){
    CNL *nodes = (CNL*)malloc(sizeof(CNL)*size);
    DOI(size, {nodes[i].i64=val[i]; nodes[i].p=i;})
    qsort(nodes, size, sizeof(CNL), cmp_L);
}

static CN *lib_qsort_node(V val, L size, FUNC_QCMP(cmp)){
    CN *nodes = (CN*)malloc(sizeof(CN)*size);
    switch(vp(val)){
        caseL DOI(size, {nodes[i].i64=vL(val,i); nodes[i].p=i;})
              qsort(nodes, size, sizeof(CN), cmp_L); break;
        default: EP("Support more types: %s", getTypeName(vp(val)));
    }
    //DOI(size, lib_node_assign(nodes+i, val, i))
    //qsort(nodes, size, sizeof(CN), cmp_test);
    R nodes;
}

void lib_order_vector(L *targ, V val, L vLen, B *isUp){
    CN *nodes = lib_qsort_node(val, vLen, cmp_L);
    DOI(vLen, targ[i] = nodes[i].p)
    free(nodes);
}

void lib_order_by_vector(L *targ, V val, B *isUp, L tLen, FUNC_CMP(cmp)){
    if(!isIntegers(val)) DOP(tLen, targ[i]=i)
    // WP("tLen = %lld\n", tLen);
    lib_quicksort(targ, val, 0, tLen, isUp, cmp);
    //lib_order_vector(targ, val, tLen, isUp); // experimental
    // WP("done.\n");
}

L lib_bs_find_same(V val, L* index, L iLen, L (*cmp)(V,L,L,B*), B opt, L* seg){
    L start = 0, len = 0;
    while(start < iLen){
        L pivot = start, end = iLen;
        do{
            if(0 != (*cmp)(val,index[pivot],index[end-1],NULL)) {
                L mid = (start + end) >> 1;
                if(0 != (*cmp)(val,index[pivot],index[mid],NULL)) end = mid;
                else start = mid+1;
            }
            else start = end;
        }while(start < end);
        if(opt) seg[len++] = pivot;
        else len++;
    }
    R len;
}

L lib_get_group_by_q1(V z, V val, L* index, L iLen, L (*cmp)(V,L,L,B*)){
    L  lenZ = lib_bs_find_same(val, index, iLen, cmp, false, NULL);
    L* segZ = (L*)malloc(sizeof(L) * lenZ);
    lib_bs_find_same(val, index, iLen, cmp, true, segZ);
    // WP("Result:\n");
    // DOI(lenZ, WP("segZ[%lld] = %lld\n", i,segZ[i]))
    /* dict */
    initV(z, H_N, 2);
    V zKey = getDictKeys(z);
    V zVal = getDictVals(z);
    initV(zKey, H_L, lenZ);
    initV(zVal, H_G, lenZ);
    DOI(lenZ, vL(zKey,i)=index[segZ[i]])
    DOI(lenZ, { L start=segZ[i]; L end=(i+1<lenZ)?segZ[i+1]:iLen; \
        L size= end-start; V d=vV(zVal, i); initV(d, H_L, size); \
        L k=0; DOI3(start, end, vL(d,k++)=index[i]) })
    free(segZ);
    R 0;
}

static I lib_get_group_by_other(V z, V val, L* index, L iLen, L (*cmp)(V,L,L,B*)){
    L k, c, cz; V d,t;
    /* 1. get the total number of cells: lenZ */
    // WP("step 1\n");
    L lenZ=iLen>0?1:0;
    DOIa(iLen, if(0!=(*cmp)(val,index[i-1],index[i],NULL))lenZ++)
    /* 2. allocate list and get the info of each cell */
    // WP("step 2\n");
    // initV(z, H_N, lenZ);
    initV(z, H_N, 2);
    V zKey = getDictKeys(z);
    V zVal = getDictVals(z);
    initV(zKey, H_L, lenZ);
    initV(zVal, H_G , lenZ);
    c=iLen>0?1:0;
    k=cz=0;
    d=vV(zVal,k++);
    if(iLen>0) vL(zKey,cz++)=index[0];
    DOIa(iLen, if(0!=(*cmp)(val,index[i-1],index[i],NULL)){ \
                 vL(zKey,cz++)=index[i]; \
                 initV(d,H_L,c); d=vV(zVal,k++); c=1; } \
               else c++;)
    if(c>0) initV(d,H_L,c);
    /* 3. fill indices into each cell */
    // WP("step 3\n");
    k=0, c=0;
    d=vV(zVal,k++);
    if(iLen>0) vL(d,c++)=index[0];
    DOIa(iLen, if(0!=(*cmp)(val,index[i-1],index[i],NULL)){ \
                  d=vV(zVal,k++); vL(d,0)=index[i]; c=1; } \
               else vL(d,c++)=index[i])
    // WP("exit\n");
    R 0;
}

/* 
 * copy from lib_get_group_by_other
 * index[x] => x
 */
static I lib_get_group_by_order(V z, V val, L* index, L iLen, L (*cmp)(V,L,L,B*)){
    L k, c, cz; V d,t;
    /* 1. get the total number of cells: lenZ */
    // WP("step 1\n");
    L lenZ=iLen>0?1:0;
    DOIa(iLen, if(0!=(*cmp)(val,i-1,i,NULL))lenZ++)
    /* 2. allocate list and get the info of each cell */
    // WP("step 2\n");
    // initV(z, H_N, lenZ);
    initV(z, H_N, 2);
    V zKey = getDictKeys(z);
    V zVal = getDictVals(z);
    initV(zKey, H_L, lenZ);
    initV(zVal, H_G, lenZ);
    c=iLen>0?1:0;
    k=cz=0;
    d=vV(zVal,k++);
    if(iLen>0) vL(zKey,cz++)=0;
    DOIa(iLen, if(0!=(*cmp)(val,i-1,i,NULL)){ \
                 vL(zKey,cz++)=i; \
                 initV(d,H_L,c); d=vV(zVal,k++); c=1; } \
               else c++;)
    if(c>0) initV(d,H_L,c);
    /* 3. fill indices into each cell */
    // WP("step 3\n");
    k=0, c=0;
    d=vV(zVal,k++);
    if(iLen>0) vL(d,c++)=0;
    DOIa(iLen, if(0!=(*cmp)(val,i-1,i,NULL)){ \
                  d=vV(zVal,k++); vL(d,0)=i; c=1; } \
               else vL(d,c++)=i)
    // WP("exit\n");
    R 0;
}

I lib_get_group_by(V z, V val, L *index, L iLen, L (*cmp)(V,L,L,B*)){
    // #ifdef OPT_Q1
    //     R lib_get_group_by_q1(z, val, index, iLen, cmp);
    // #else
       if(index == NULL)
           R lib_get_group_by_order(z, val, index, iLen, cmp);
       else 
           R lib_get_group_by_other(z, val, index, iLen, cmp);
           //R lib_group_by_flat(z, val, index, iLen, cmp);
    // #endif
}

// L lib_get_group_by(V z, V val, L* index, L iLen, L (*cmp)(V,L,L,B*)){
//     L k, c, cz; V d,t;
//     /* 1. get the total number of cells: lenZ */
//     WP("step 1\n");
//     L lenZ=iLen>0?1:0;
//     DOIa(iLen, if(0!=(*cmp)(val,index[i-1],index[i],NULL))lenZ++)
//     /* 2. allocate list and get the info of each cell */
//     WP("step 2\n");
//     // initV(z, H_N, lenZ);
//     initV(z, H_N, 2);
//     V zKey = getDictKeys(z);
//     V zVal = getDictVals(z);
//     initListCopy(zKey, val, lenZ);
//     initV(zVal, H_G, lenZ);
//     c=iLen>0?1:0;
//     k=cz=0;
//     d=vV(zVal,k++);
//     if(iLen>0) CHECKE(copyByIndex(zKey,cz++,val,index[0]))
//     DOIa(iLen, if(0!=(*cmp)(val,index[i-1],index[i],NULL)){ \
//                  CHECKE(copyByIndex(zKey,cz++,val,index[i])) \
//                  initV(d,H_L,c); d=vV(zVal,k++); c=1; } \
//                else c++;)
//     if(c>0) initV(d,H_L,c);
//     /* 3. fill indices into each cell */
//     WP("step 3\n");
//     k=0, c=0;
//     d=vV(zVal,k++);
//     if(iLen>0) vL(d,c++)=index[0];
//     DOIa(iLen, if(0!=(*cmp)(val,index[i-1],index[i],NULL)){ \
//                   d=vV(zVal,k++); vL(d,0)=index[i]; c=1; } \
//                else vL(d,c++)=index[i])
//     WP("exit\n");
//     R 0;
// }

#define LIB_MEMBER_F1(p) case##p R to##p(val,valI)==to##p(src,0)
#define LIB_MEMBER_F1F(p,foo) case##p R foo(to##p(val,valI),to##p(src,0))

static B lib_member_fast1(void* src, void* val, L valI, L typ){
    switch(typ){
        LIB_MEMBER_F1(B);
        LIB_MEMBER_F1(J);
        LIB_MEMBER_F1(H);
        LIB_MEMBER_F1(I);
        LIB_MEMBER_F1(L);
        LIB_MEMBER_F1(F);
        LIB_MEMBER_F1(E);
        LIB_MEMBER_F1F(X, xEqual);
        LIB_MEMBER_F1(C);
        LIB_MEMBER_F1F(S, !strcmp);
        default: { EP("Not impl. for type: %s\n", getTypeName(typ)); }
    }
    R 0;
}


#define LIB_MEMBER_F2(p) case##p R to##p(val,valI)==to##p(src,0) || to##p(val,valI)==to##p(src,1)
#define LIB_MEMBER_F2F(p,foo) case##p R foo(to##p(val,valI),to##p(src,0)) || foo(to##p(val,valI),to##p(src,1))

static B lib_member_fast2(void* src, void* val, L valI, L typ){
    // WP("match %lld (%s), %lld (%s), %lld (%s) == %d\n", \
    //       toL(val,valI), getSymbolStr(toL(val,valI))\
    //     , toL(src,0),    getSymbolStr(toL(src,0)) \
    //     , toL(src,1),    getSymbolStr(toL(src,1))\
    //     , toL(val,valI)==toL(src,0) || toL(val,valI)==toL(src,1));
    // getchar();
    switch(typ){
        LIB_MEMBER_F2(B);
        LIB_MEMBER_F2(J);
        LIB_MEMBER_F2(H);
        LIB_MEMBER_F2(I);
        LIB_MEMBER_F2(L);
        LIB_MEMBER_F2(F);
        LIB_MEMBER_F2(E);
        LIB_MEMBER_F2F(X, xEqual);
        LIB_MEMBER_F2(C);
        LIB_MEMBER_F2F(S, !strcmp);
        default: { EP("Not impl. for type %s", getTypeName(typ)); }
    }
    R 0;
}

#define lib_member_template(typ)\
    L hashCur = getHashHeap(); \
    if(sLen > 2) { lib_member_template_normal(typ) }\
    else if(sLen == 2) { DOP(vLen, targ[i]=lib_member_fast2(src,val,i,typ)) }\
    else { DOP(vLen, targ[i]=lib_member_fast1(src,val,i,typ)) } \
    setHashHeap(hashCur);
    // { L k=0; DOI(vLen, k+=targ[i]) WP("total sum = %lld\n", k); }

#define lib_member_template_normal(typ)\
    HN hashT; \
    L hashLen  = getHashTableSize(sLen); \
    L hashMask = hashLen - 1; \
    L hashCur = getHashHeap(); \
    /*WP("size2 = %lld\n",hashLen);*/ \
    tic(); \
    CHECKE(createHash(&hashT,hashLen)); \
    DOI(sLen, insertHash(hashT,hashMask,src,i,NULL,-1,typ)) \
    time_toc("build time"); \
    tic(); \
    DOP(vLen, targ[i]=(0<=find_hash(hashT,hashMask,src,val,i,typ))) \
    time_toc("probe time"); \
    setHashHeap(hashCur);
    // profileHash(hashT,hashLen);


I lib_member_B(B* targ, B* src, L sLen, B* val, L vLen){
    B flag[2]={0};
    DOI(sLen, flag[src[i]]=1)
    DOP(vLen, targ[i]=flag[val[i]])
    R 0;
}

I lib_member_H(B* targ, H* src, L sLen, H* val, L vLen){
    lib_member_template(H_H);
    R 0;
}

I lib_member_I(B* targ, I* src, L sLen, I* val, L vLen){
    lib_member_template(H_I);
    R 0;
}

I lib_member_L(B* targ, L* src, L sLen, L* val, L vLen){
    //writeToFileFromPtr(src,sLen,"temp_member_left.txt");
    lib_member_template(H_L);
    R 0;
}

I lib_member_F(B* targ, F* src, L sLen, F* val, L vLen){
    lib_member_template(H_F);
    R 0;
}

I lib_member_E(B* targ, E* src, L sLen, E* val, L vLen){
    lib_member_template(H_E);
    R 0;
}

I lib_member_C(B* targ, C* src, L sLen, C* val, L vLen){
    B buff[256];
    memset(buff,0,256*sizeof(B));
    DOI(sLen, if(!buff[src[i]]) buff[src[i]]=1)
    DOP(vLen, targ[i]=buff[val[i]];)
    R 0;
}

I lib_member_X(B* targ, X* src, L sLen, X* val, L vLen){
    lib_member_template(H_X);
    R 0;
}

I lib_member_S(B* targ, S* src, L sLen, S* val, L vLen){
    lib_member_template(H_S);
    R 0;
}


/*
 Call external shared libraries
*/
// #include <dlfcn.h>
// #include <unistd.h>
// 
// void *load_shared_lib(S name){
//     void *handle=NULL;
//     handle=dlopen(name, RTLD_NOW | RTLD_GLOBAL);
//     if(!handle){
//         printf("Load lib %s fail: %s\n", name,dlerror());
//         exit(1);
//     }
//     return handle;
// }
// 
// void *load_shared_func(S fn, S name, void *handle){
//     void *func = dlsym(handle, fn);
//     if(!func){
//         fprintf(stderr, "Can't find func %s in %s: %s\n", fn,name,dlerror());
//         dlclose(handle);
//         exit(1);
//     }
//     return func;
// }

/* a helper function */
void get_current_path(){
    char szTmp[32], pBuf[128];
    int len = 128; 
    sprintf(szTmp, "/proc/%d/exe", getpid());
    int bytes = MIN(readlink(szTmp, pBuf, len), len - 1);
    if(bytes >= 0)
        pBuf[bytes] = '\0';
    WP("current path: %s\n", pBuf);
}

#define U8  uint8_t
#define U32 uint32_t
#define CU8 const U8
#define CU32 const U32
#define getBlock32(p,i) p[i]
#define ROTL32(x, r) ((x<<r)|(x>>(32-r)))
#define fmix32(h) {h^=h>>16; h*=0x85ebca6b; h^=h>>13; h*=0xc2b2ae35; h^= h>>16;}

static void MurmurHash3(const void *key, int len, U32 seed, void *out) {
    CU8 *data = (CU8*) key;
    const int nblocks = len/4;
    U32 h1 = seed;
    CU32 c1 = 0xcc9e2d51;
    CU32 c2 = 0x1b873593;
    // body
    CU32 *blocks = (CU32*)(data+(nblocks<<2));
    for(int i=-nblocks;i;i++){
        U32 k1 = getBlock32(blocks,i);
        k1 *= c1;
        k1 = ROTL32(k1, 15);
        k1 *= c2;

        h1 ^= k1;
        h1 = ROTL32(h1,13);
        h1 = h1*5+0xe6546b64;
    }
    // tail
    CU8 *tail  = (CU8*)(data + (nblocks<<2));
    U32 k1 = 0;

    switch(len & 3){
        case 3: k1 ^= tail[2] << 16;
        case 2: k1 ^= tail[1] << 8;
        case 1: k1 ^= tail[0];
                k1 *= c1;
                k1 = ROTL32(k1, 15);
                k1 *=c2;
                h1 ^= k1;
    } 

    // final
    h1 ^= len;
    fmix32(h1);
    *(U32*)out = h1;
}

//int main(){
//    U32 outString = 0, outDouble = 0;
//    char helloString[] = "hello world";
//    MurmurHash3(helloString, strlen(helloString), 16807, &outString);
//    printf("outString = %d\n", outString);
//    double helloDouble = 25.9;
//    MurmurHash3(&helloDouble, sizeof(double), 16807, &outDouble);
//    printf("outDouble = %d\n", outDouble);
//    return 0;
//}

/* hash grouping (not used) */

//typedef struct hash_test{
//    L index;
//    struct hash_test *next;
//}HT0, *HT;
//
//void lib_hash_test(L *val, L n){
//    //FILE *fp = fopen("problem1_data.txt", "w");
//    //FP(fp, "%lld\n", n);
//    //DOI(n, FP(fp, "%lld ", val[i])) FP(fp, "\n"); fclose(fp);
//    //WP("done.\n"); getchar();
//    U32 out; L size = n * 2;
//    struct timeval tv0, tv1;
//    gettimeofday(&tv0, NULL);
//    L *qq = (L*)malloc(sizeof(L)*10);
//    HT* table = (HT*)malloc(sizeof(HT)*size);
//    HT* lastp = (HT*)malloc(sizeof(HT)*size);
//    HT nodes  = (HT)malloc(sizeof(HT0)*n);
//    memset(table, 0, sizeof(HT)*size);
//    gettimeofday(&tv1, NULL);
//    WP("0. Exe. time (ms): %g\n", calcInterval(tv0, tv1));
//    //DOI(n, {MurmurHash3(val+i, 1, 16807, &out); if(out%size==9348)WP("%lld ", val[i]); table[out%size]++;})
//    gettimeofday(&tv0, NULL);
//    DOI(n, {out = hash_djb2_n(val+i, sizeof(L));
//            HT node = nodes+i; node->next = NULL; node->index = i; \
//            L x=out%size; \
//            if(table[x]==NULL) lastp[x]=table[x]=node; \
//            else {lastp[x]->next = node; lastp[x] = node;} \
//            });
//    //DOI(n, {out = hash_djb2_n(val+i, sizeof(L)); \
//    //        HT node = nodes+i; node->next = NULL; node->index = i; \
//    //        L x=out%size; \
//    //        if(table[x]==NULL) lastp[x]=table[x]=node; \
//    //        else {lastp[x]->next = node; lastp[x] = node;} \
//    //        });
//    gettimeofday(&tv1, NULL);
//    WP("1. Exe. time (ms): %g\n", calcInterval(tv0, tv1)); getchar();
//    WP("out = %u\n", out);
//    gettimeofday(&tv0, NULL);
//    for(L i=0; i<size;i++){
//        if(table[i]){
//            HT t = table[i]; L tot = 0; while(t){ tot++; /*WP("%lld ", val[t->index]);*/ t=t->next;} //WP("\n");
//            B *visited = (B*)malloc(sizeof(B)*tot); memset(visited, 0, sizeof(B)*tot);
//            t = table[i];
//            DOJ(tot, {if(!visited[j]){ visited[j]=true; HT p = t->next; L cnt = 0; \
//                L k = j+1; while(p){ if(val[t->index]==val[p->index]){visited[k]=true; cnt++;} p = p->next; k++; } \
//                /*WP("val = %lld, cnt = %lld\n",val[t->index],cnt);*/ } t=t->next;} )
//            free(visited);
//        }
//    } 
//    gettimeofday(&tv1, NULL);
//    WP("2. Exe. time (ms): %g\n", calcInterval(tv0, tv1)); getchar();
//    /*L tot = 0, maxv = 0;
//    DOI(size, if(table[i]>table[maxv])maxv=i)
//    L minv = maxv;
//    DOI(size, if(table[i]>0 && table[i]<table[minv])minv=i)
//    DOI(size, tot += table[i]>0)
//    WP(">0: %lld, %lld (%lld), %lld (%lld)\n",tot,maxv,table[maxv],minv,table[minv]);
//    */
//    free(lastp);
//    free(table);
//    free(nodes);
//}

/* radix sort (experimental) */
typedef struct Pos{ L x; I i;} Pos;
typedef struct Pos_i{ I x; L i; }Pos_i;

static void lib_radixsort_core_int(Pos_i *val, I len){
    const L base = 8;
    const L size = (1<<base)-1; //255
    const L maxSize = 32;
    L     *tempC = (L    *)malloc(sizeof(L    )<<base);
    Pos_i *tempB = (Pos_i*)malloc(sizeof(Pos_i)*len);
    //WP("maxSize = %d\n", maxSize);
    for(L j=0; j<maxSize; j+=base){
        memset(tempC, 0, sizeof(L)<<base);
        DOI(len, tempC[((val[i].x)>>j) & size]++)
        DOIa(size+1, tempC[i] = tempC[i] + tempC[i-1])
        //printf("j = %d\n", j);
        for(L i=len-1;i>=0;i--){
            L k = ((val[i].x)>>j) & size;
            //printf("val = %lld, k = %d\n", val[i].x,k);
            tempC[k]--;
            tempB[tempC[k]] = val[i];
        }
        memcpy(val, tempB, sizeof(Pos_i) * len);
        //for(int i=0; i<len; i++){
        //    val[i] = B[i];
        //}
    }
    free(tempB);
    free(tempC);
}

static B seeOrder(Pos_i *pos, L n){
    DOIa(n, if(pos[i].x < pos[i-1].x){WP("stop at pos[%lld].x=%d\n",i,pos[i].x);R 0;}) R 1;
}

void lib_radixsort_int(L *rtn, V val, L len, B *isUp, B isRtnIndex){
    if(H_DEBUG) WP("... lib_radixsort_int");
    if(vp(val) != H_I) EP("require H_I");
    B f0 = isUp?*isUp:1;
    Pos_i *pos = (Pos_i*)malloc(sizeof(Pos_i)*len);
    DOP(len, {pos[i].x=vI(val,i); pos[i].i=i;})
tic();
    lib_radixsort_core_int(pos, len);
toc(); //getchar();
    //WP("f0 = %d, rtn = %d\n", f0,isRtnIndex); DOI(len, WP("%lld: %lld\n",i,pos[i].i)) getchar();
    if(isRtnIndex){ // return index
        if(f0) DOP(len, rtn[i]=pos[i].i)
        else DOP(len, rtn[i]=pos[len-i-1].i)
    }
    else { // return value
        if(f0) DOP(len, rtn[i]=pos[i].x)
        else DOP(len, rtn[i]=pos[len-i-1].x)
    }
    //WP("result: "); DOI(len, WP("%d ",pos[i].x)) WP("\n"); getchar();
    //if(seeOrder(pos,len)) WP(" result ordered\n");
    //else WP(" result ordered not\n");
    free(pos);
}

/*
 * https://blog.csdn.net/WINCOL/article/details/4799979
 *    old name -> lib_radixsort_basic
 */
static void lib_radixsort_core_long(Pos *val, I len){
    const L size = 255;
    I *tempC = (I*)malloc(sizeof(I)<<8);
    Pos *tempB = (Pos*)malloc(sizeof(Pos)*len);
    I maxSize = 32;
    const L halfL = 0xffffffff;
    for(I i=0; i<len; i++){
        //int numOfDigits = (int)log10(val[i].x) + 1;
        if(((val[i].x)>>32) > 0){
            maxSize = 64; break;
        }
    }
    //WP("maxSize = %d\n", maxSize);
    for(I j=0,j2=maxSize; j<j2; j+=8){
        memset(tempC, 0, sizeof(I)<<8);
        DOI(len, tempC[((val[i].x)>>j) & size]++)
        DOIa(size+1, tempC[i] = tempC[i] + tempC[i-1])
        //printf("j = %d\n", j);
        for(I i=len-1;i>=0;i--){
            L k = ((val[i].x)>>j) & size;
            //printf("val = %lld, k = %d\n", val[i].x,k);
            tempC[k]--;
            tempB[tempC[k]] = val[i];
        }
        memcpy(val, tempB, sizeof(Pos) * len);
        //for(int i=0; i<len; i++){
        //    val[i] = B[i];
        //}
    }
    free(tempB);
    free(tempC);
}

void lib_radixsort_long(L *rtn, V val, L len, B *isUp, B isRtnIndex){
    if(H_DEBUG) WP("... lib_radixsort_long\n");
    if(vp(val) != H_L) EP("require H_L");
    B f0 = isUp?*isUp:1;
    Pos *pos = (Pos*)malloc(sizeof(Pos)*len);
    DOP(len, {pos[i].x=vL(val,i); pos[i].i=i;})
    lib_radixsort_core_long(pos, len);
    //WP("f0 = %d, rtn = %d\n", f0,isRtnIndex); DOI(len, WP("%lld: %lld\n",i,pos[i].i)) getchar();
    if(isRtnIndex){ // return index
        if(f0) DOP(len, rtn[i]=pos[i].i)
        else DOP(len, rtn[i]=pos[len-i-1].i)
    }
    else { // return value
        if(f0) DOP(len, rtn[i]=pos[i].x)
        else DOP(len, rtn[i]=pos[len-i-1].x)
    }
    free(pos);
}

I lib_group_by_normal_int(V z, V x){
    Pos_i *pos = (Pos_i*)malloc(sizeof(Pos_i)*xn);
    DOP(xn, {pos[i].x=vI(x,i); pos[i].i=i;})
    lib_radixsort_core_int(pos, xn);
    L k, c, cz; V d,t;
    L start=xn>0?1:0;
    /* 1. get the total number of cells: lenZ */
    // WP("step 1\n");
    L lenZ=start;
    DOIa(xn, if(pos[i-1].x!=pos[i].x)lenZ++)
    /* 2. allocate list and get the info of each cell */
    // WP("step 2\n");
    initV(z, H_N, 2);
    V zKey = getDictKeys(z);
    V zVal = getDictVals(z);
    initV(zKey, H_L, lenZ);
    initV(zVal, H_G, lenZ);
    /* 3. fill indices into each cell */
    // WP("step 3\n");
    k=cz=0, c=start;
    d=vV(zVal,k++);
    if(start) vL(zKey,cz++)=pos[0].i;
    DOIa(xn, if(pos[i-1].x!=pos[i].x){ \
               vL(zKey,cz++)=pos[i].i; initV(d,H_L,c); d=vV(zVal,k++); c=1;} \
             else c++)
    if(c>0) initV(d,H_L,c);
    k=0, c=0;
    d=vV(zVal,k++);
    if(start) vL(d,c++)=pos[0].i;
    DOIa(xn, if(pos[i-1].x!=pos[i].x){\
               d=vV(zVal,k++); vL(d,0)=pos[i].i; c=1;} \
             else vL(d,c++)=pos[i].i)
    free(pos);
    R 0;
}

// I lib_group_by_normal_long(V z, V x){
//     P("hello \n"); getchar();
//     Pos_i *pos = (Pos_i*)malloc(sizeof(Pos_i)*xn);
//     DOP(xn, {pos[i].x=vL(x,i)&0xffffffffL; pos[i].i=i;})
//     lib_radixsort_core_int(pos, xn);
//     L k, c, cz; V d,t;
//     L start=xn>0?1:0;
//     /* 1. get the total number of cells: lenZ */
//     // WP("step 1\n");
//     L lenZ=start;
//     DOIa(xn, if(pos[i-1].x!=pos[i].x)lenZ++)
//     /* 2. allocate list and get the info of each cell */
//     // WP("step 2\n");
//     initV(z, H_N, 2);
//     V zKey = getDictKeys(z);
//     V zVal = getDictVals(z);
//     initV(zKey, H_L, lenZ);
//     initV(zVal, H_G, lenZ);
//     /* 3. fill indices into each cell */
//     // WP("step 3\n");
//     k=cz=0, c=start;
//     d=vV(zVal,k++);
//     if(start) vL(zKey,cz++)=pos[0].i;
//     DOIa(xn, if(pos[i-1].x!=pos[i].x){ \
//                vL(zKey,cz++)=pos[i].i; initV(d,H_L,c); d=vV(zVal,k++); c=1;} \
//              else c++)
//     if(c>0) initV(d,H_L,c);
//     k=0, c=0;
//     d=vV(zVal,k++);
//     if(start) vL(d,c++)=pos[0].i;
//     DOIa(xn, if(pos[i-1].x!=pos[i].x){\
//                d=vV(zVal,k++); vL(d,0)=pos[i].i; c=1;} \
//              else vL(d,c++)=pos[i].i)
//     free(pos);
//     R 0;
// }


I lib_group_by_normal_long(V z, V x){
    Pos *pos = (Pos*)malloc(sizeof(Pos)*xn);
    DOP(xn, {pos[i].x=vL(x,i); pos[i].i=i;})
    lib_radixsort_core_long(pos, xn);
    L k, c, cz; V d,t;
    L start=xn>0?1:0;
    /* 1. get the total number of cells: lenZ */
    // WP("step 1\n");
    L lenZ=start;
    DOIa(xn, if(pos[i-1].x!=pos[i].x)lenZ++)
    /* 2. allocate list and get the info of each cell */
    // WP("step 2\n");
    initV(z, H_N, 2);
    V zKey = getDictKeys(z);
    V zVal = getDictVals(z);
    initV(zKey, H_L, lenZ);
    initV(zVal, H_G, lenZ);
    /* 3. fill indices into each cell */
    // WP("step 3\n");
    k=cz=0, c=start;
    d=vV(zVal,k++);
    if(start) vL(zKey,cz++)=pos[0].i;
    DOIa(xn, if(pos[i-1].x!=pos[i].x){ \
               vL(zKey,cz++)=pos[i].i; initV(d,H_L,c); d=vV(zVal,k++); c=1;} \
             else c++)
    if(c>0) initV(d,H_L,c);
    k=0, c=0;
    d=vV(zVal,k++);
    if(start) vL(d,c++)=pos[0].i;
    DOIa(xn, if(pos[i-1].x!=pos[i].x){\
               d=vV(zVal,k++); vL(d,0)=pos[i].i; c=1;} \
             else vL(d,c++)=pos[i].i)
    free(pos);
    R 0;
}

// TODO: extend from only (H_L == vp(x))
I lib_group_by_flat(V z, V x){
    Pos *pos = (Pos*)malloc(sizeof(Pos)*xn);
    DOP(xn, {pos[i].x=vL(x,i); pos[i].i=i;})
    lib_radixsort_core_long(pos, xn);
    L k, c, cz; V d,t;
    L start=xn>0?1:0;
    /* 1. get the total number of cells: lenZ */
    // WP("step 1\n");
    L lenZ=start;
    DOIa(xn, if(pos[i-1].x!=pos[i].x)lenZ++)
    /* 2. allocate list and get the info of each cell */
    // WP("step 2\n");
    initV(z, H_N, 2);
    V zKey = getDictKeys(z);
    V zVal = getDictVals(z);
    initV(zKey, H_L, lenZ);
    initFlatList(zVal, lenZ);
    /* store index value */
    V g2 = allocNode();
    initV(g2,H_L,xn);
    DOP(xn,vL(g2,i)=pos[i].i)
    vg2(zVal) = g2;
    /* 3. fill indices into each cell */
    // WP("step 3\n");
    k=0, c=start;
    if(start) vL(zKey,k)=pos[0].i;
    DOIa(xn, if(pos[i-1].x!=pos[i].x){vL(zVal,k++)=c; vL(zKey,k)=pos[i].i; c=1;} \
               else c++)
    if(c>0) vL(zVal,k)=c;
    //printV(x); printV(zKey); printV(zVal); getchar();
    free(pos);
    R 0;
}

static L getFirstEqual(V f){
    DOI(vn(f), if(sEQ(getSymbolStr(vQ(f,i)), "eq"))R i)
    //DOI(vn(f), if(sEQ(getSymbolStr(vQ(f,i)), "neq"))R i)
    R 0;
}

static B compareOpRow(V x, V y, L cx, L cy, V f){
    DOI(xn, if(!compareOpWithIndex(vV(x,i),vV(y,i),cx,cy,getOpFromSymbol(vL(f,i))))R 0) R 1;
}

static L binary_search(V x, L p0, V y, V d, I op){
    L sta = 0, end = vn(y)-1;
    while(sta < end){
        L mid = (sta + end)/2;
        if(compareOpWithIndex(x,y,p0,vL(d,mid),op)){ sta = mid + 1; }
        else { end = mid - 1; }
    }
    if(sta == end) R sta;
    else if(end < 0) R end;
    else EP("Binary search result different:(%lld, %lld)",sta,end);
}

/* ------ Hash Related ------ */

////L debug_hash_total1 = 0;
////L debug_hash_total2 = 0;
//L debug_hash_find1 = 0;
//L debug_hash_find2 = 0;
//L debug_hash_max1 = -1;
//L debug_hash_max2 = -1;
//
//L debug_hash_count1 = 0;
//L debug_hash_count2 = 0;
//L debug_hash_need_search1 = 0;
//L debug_hash_need_search2 = 0;
//L debug_hash_op_total1 = 0;
//L debug_hash_op_total2 = 0;

/*
static HN findValueFromHashCore(HN hashT, V *src, L hashLen, L n, L *args){
    if(n == 1){
        L val = args[0];
        L hashMask = hashLen - 1;
        L hashKey = hash_L(args[0]) & hashMask;
        HN t = hV(hashT, hashKey);
        V src0 = src[0];
        //WP(" n = 1, begin\n");
        L debug_local_num = 0;
        if(hN(t)) debug_hash_need_search1++;
        debug_hash_count1++;
        debug_hash_op_total1++;
        while(hN(t)){
            t = hN(t); L td = hD(t);
            debug_local_num++;
            if(vL(src0,td)==val) { debug_hash_find1++; R t; }
            debug_hash_op_total1++;
        }
        //debug_hash_total1 += debug_local_num;
        if(debug_hash_max1 < debug_local_num)
            debug_hash_max1 = debug_local_num;
        //WP(" n = 1, done\n");
    }
    else {
        L val = args[0];
        L hashMask = hashLen - 1;
        L hashKey = hash_L(args[0]) & hashMask;
        HN t = hV(hashT, hashKey);
        //WP("1: n = %lld\n",n);
        L debug_local_num = 0;
        if(hN(t)) debug_hash_need_search2++;
        debug_hash_count2++;
        debug_hash_op_total2++;
        while(hN(t)){
            t = hN(t); L td = hD(t);
            debug_local_num++;
            if(isAllMatch(n,src,td,args)) {debug_hash_find2++; R t;}
            debug_hash_op_total2++;
        }
        //debug_hash_total2 += debug_local_num;
        if(debug_hash_max2 < debug_local_num)
            debug_hash_max2 = debug_local_num;
        //WP("2: done\n");
    }
    R 0;
}
*/


// THash createHashWithV2(L n, V *x, L *hashLen){
//     if(n >= 1) R createHashMultiple(x[0], hashLen);
//     else EP("Number of columns invalid: %lld\n", n);
// }

// HN findValueFromHash2(HN *hashT, V *src, L *hashSize, L n, ...){
//     readArgs();
//     //WP("Input with %lld:\n",n);
//     //DOI(n, WP("args[%lld] = %lld\n",i,args[i]))
//     L hashSetId  = args[0] & setN;
//     R findValueFromHashCore(hashT[hashSetId],src,hashSize[hashSetId],n,args);
// }

// hash join linear scan
I lib_join_index_linear(V z0, V z1, V x, V y, B isEq){
    /* create a special case when one side is a very small number */
#define join_linear_calc(op) \
    switch(xp){ \
        caseI DOI(vn(y), {DOJ(vn(x), if(vI(y,i) op vI(x,j)){size++;break;})}) break; \
        caseL DOI(vn(y), {DOJ(vn(x), if(vL(y,i) op vL(x,j)){size++;break;})}) break; \
        caseE DOI(vn(y), {DOJ(vn(x), if(vE(y,i) op vE(x,j)){size++;break;})}) break; \
        default: EP("Type not supported: %s",getTypeName(xp)); \
    }
#define join_linear_save(op) \
    switch(xp){ \
        caseI DOI(vn(y), {DOJ(vn(x), if(vI(y,i) op vI(x,j)){vL(z0,size)=j; vL(z1,size++)=i; break;})}) break; \
        caseL DOI(vn(y), {DOJ(vn(x), if(vL(y,i) op vL(x,j)){vL(z0,size)=j; vL(z1,size++)=i; break;})}) break; \
        caseE DOI(vn(y), {DOJ(vn(x), if(vE(y,i) op vE(x,j)){vL(z0,size)=j; vL(z1,size++)=i; break;})}) break; \
    }
#define join_linear_main(foo) \
    if(isEq) {foo(==);} else {foo(!=);}

    if(H_DEBUG){
        printBanner("Linear search join");
        getInfoVar(x); getInfoVar(y);
    }
    L size=0;
    //time_tic();
    join_linear_main(join_linear_calc);
    initV(z0,H_L,size);
    initV(z1,H_L,size);
    size=0;
    join_linear_main(join_linear_save);
    //time_toc();
    //WP("size = %lld\n", size);
    //DOI(size>10?10:size, WP(" %lld",vL(z1,i))) WP("\n"); getchar();
    R 0;
}

I lib_join_index_compare(V z0, V z1, V x, V y, I op){
    // x and y have the same type
    V0 tt; V d=&tt; L n=0; I op_f=(0==op||1==op?0:1); // lt/leq:0 (desc); gt/geq:1 (asc)
    //WP("op = %d, op_f = %d\n",op,op_f);
    pfnOrderBy(d, y, initLiteralBool(op_f));
    //printV(x); printV(y); printV(d); getchar();
    // 1. fetch length
    DOI(xn, {L k=binary_search(x,i,y,d,op); if(k>=0)n+=k+1;})
    // 2. alloc memory & do it
    L n0=0;
    initV(z0, H_L, n);
    initV(z1, H_L, n);
    DOI(xn, {L k=binary_search(x,i,y,d,op); \
            if(k>=0){DOJ(k+1, {vL(z0,n0+j)=i;vL(z1,n0+j)=vL(d,j);})n0+=k+1;}})
    //printV(z0);
    //printV(z1);
    //getchar();
    R 0;
}

/*
 * x0: sLen
 * y0: vLen
 */
I lib_join_index_hash_many(V z0, V z1, V x, V y, V f){
    WP("experiment: lib_join_index_hash_many\n");
    L fx=getFirstEqual(f); L f0=vQ(f,fx);
    V x0=vV(x,fx), y0=vV(y,fx);
    L sLen=vn(x0), vLen=vn(y0), typ=vp(x0);
    void *src=sG(x0), *val=sG(y0);
    //WP("Profiling x: "); profileInt(x);
    //WP("Profiling y: "); profileInt(y);
    //WP("xLen = %lld, yLen = %lld\n", sLen,vLen);
    HN hashT;
    L hashLen = getHashTableSize(sLen);
    L hashMask = hashLen - 1;
    E elapsed = 0;
    CHECKE(createHash(&hashT,hashLen));
tic();
    DOI(sLen, insertHashMany(hashT,hashMask,src,i,NULL,-1,typ))
time_toc("Step 1: Hash build %g ms, %.1lf MB/s\n", elapsed, 1.0*sLen/elapsed/1000);
tic();
    L c = 0;
    L *tempK = (L*)malloc(sizeof(L)*vLen);
    HI *tempH = (HI*)malloc(sizeof(HI)*vLen);
    DOP(vLen, tempK[i]=find_hash_many(hashT,hashMask,src,val,i,typ,&tempH[i]))
time_toc("Step 2: Hash probe %g ms, %.1lf MB/s\n", elapsed, 1.0*vLen/elapsed/1000);
tic();
    /* debug: print result in 0/1 */
    //DOI(vLen, WP("tempK[%lld] = %lld\n",i,tempK[i])); getchar();
    L parZ[H_CORE], offset[H_CORE]; 
    memset(parZ  , 0, sizeof(L)*H_CORE);
    memset(offset, 0, sizeof(L)*H_CORE);
    DOT(vLen, if(tempK[i]>=0){ B none=true; \
         if(compareOpRow(x,y,tempK[i],i,f)){parZ[tid]++;none=false;} \
         HI t0=tempH[i]; while(t0){if(compareOpRow(x,y,tempK[i],i,f)){parZ[tid]++; none=false;} t0=t0->inext;} \
         if(none)tempK[i]=-1;})
    //DOI(H_CORE, WP("parZ[%lld] = %lld\n",i,parZ[i])); getchar();
    DOI(H_CORE, c+=parZ[i])
    //DOI(H_CORE, WP("%lld ",parZ[i])) WP(" => total %lld\n",c); // show segment info
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    initV(z0, H_L, c);
    initV(z1, H_L, c);
    //L tot=0; DOI(vLen, if(tempK[i]>=0){HI t0=tempH[i]; while(t0){t0=t0->inext;tot++;}}) WP("tot = %lld\n",tot); getchar();
    DOT(vLen, if(tempK[i]>=0){L p=offset[tid]; \
         if(compareOpRow(x,y,tempK[i],i,f)){vL(z0,p)=tempK[i];vL(z1,p)=i;} \
         HI t0=tempH[i]; while(t0){p++; if(compareOpRow(x,y,tempK[i],i,f)){vL(z0,p)=t0->ival;vL(z1,p)=i;}t0=t0->inext;} \
         offset[tid]=p+1; })
time_toc("Step 3: hash finish %g ms\n", elapsed);
    //WP("size = %lld, z0 = %lld, z1 = %lld\n",c,vn(z0),vn(z1));
    profileHash(hashT, hashLen);
    free(tempK);
    free(tempH);
    DOI(20, WP("**")) WP("\n");
    R 0;
}

/* index for join index*/

//I lib_join_index_basic(V z0, V z1, V x, V y, B isEq){
//    if(H_DEBUG){
//        printBanner("Basic hash join v3");
//        getInfoVar(x); getInfoVar(y);
//    }
//    V *hashset = (V []){vV(x,0),vV(x,1)};
//    TB th1 = create_hash_multiply(hash1set[0]);
//    HC **hash1 = th1.hashCell;
//    L *hash1Mask = th1.hashMask;
//    for(L i=0,i2=vn(x0); i<i2;i++){
//    }
//}

I lib_join_index_basic(V z0, V z1, V x, V y, B isEq){
    if(H_DEBUG){
        printBanner("Basic hash join");
        getInfoVar(x); getInfoVar(y);
    }
    L sLen,vLen,typ; void *src,*val;
    if(isList(x)&&isList(y)){
        sLen=vn(vV(x,0)), vLen=vn(vV(y,0)), typ=vp(x);
        src=x; val=y; 
    } // special for list
    else {
        sLen=vn(x), vLen=vn(y), typ=vp(x);
        src=sG(x), val=sG(y);
    }
    //if(vn(y)==1500000){
    //    writeToFileForDebug(x, "join-r.txt");
    //    writeToFileForDebug(y, "join-s.txt");
    //}
    //WP("Profiling x: "); profileInt(x);
    //WP("Profiling y: "); profileInt(y);
    WP("xLen = %lld, yLen = %lld\n", sLen,vLen);
    //DOI(vn(x), if(vL(x,i)<74){WP("stop at x: %lld\n",vL(x,i)); getchar();})
    //DOI(vn(y), if(vL(y,i)==1){WP("see y == 1 at y[%lld]\n",i); break;}) getchar();
    HN hashT; E elapsed;
    L hashLen  = getHashTableSize(sLen);
tic();
    L hashMask = hashLen - 1;
    CHECKE(createHash(&hashT,hashLen));
toc(); //getchar();
tic();
    DOI(sLen, insertHashMany(hashT,hashMask,src,i,NULL,-1,typ))
time_toc("Step 1: Hash build %g ms, %.1lf K/ms (%s)\n", elapsed, 1.0*sLen/elapsed/1000, getTypeName(typ));
tic();
    L c = 0;
    L *tempK = (L*)malloc(sizeof(L)*vLen);
    HI *tempH = (HI*)malloc(sizeof(HI)*vLen);
    //DOP(vLen, {HI t0;L k=find_hash_many(hashT,hashMask,src,val,i,typ,&t0); \
               tempK[i]=k; tempH[i]=t0;\
               if(k>=0){c++;while(t0){c++;t0=t0->inext;}}}, \
       reduction(+:c))
    //DOP(vLen, tempK[i]=find_hash_many(hashT,hashMask,src,val,i,typ,&tempH[i]))
    DOI(vLen, tempK[i]=find_hash_many(hashT,hashMask,src,val,i,typ,&tempH[i]))
time_toc("Step 2: Hash probe %g ms, %.1lf K/ms (%s)\n", elapsed, 1.0*vLen/elapsed/1000, getTypeName(typ));
tic();
    /* debug: print result in 0/1 */
    //DOI(vLen, WP("tempK[%lld] = %lld\n",i,tempK[i])); getchar();
    L parZ[H_CORE], offset[H_CORE]; 
    memset(parZ, 0, sizeof(L)*H_CORE);
    memset(offset, 0, sizeof(L)*H_CORE);
    if(isEq){
        DOT(vLen, if(tempK[i]>=0){parZ[tid]++; HI t0=tempH[i]; while(t0){parZ[tid]++;t0=t0->inext;}})
    }
    else {
        DOT(vLen, if(tempK[i]>=0){L cc=1; HI t0=tempH[i]; while(t0){cc++;t0=t0->inext;} parZ[tid]+=sLen-cc;}\
                  else{parZ[tid]+=sLen;})
    }
    //DOI(H_CORE, WP("parZ[%lld] = %lld\n",i,parZ[i])); getchar();
    DOI(H_CORE, c+=parZ[i])
    //DOI(H_CORE, WP("%lld ",parZ[i])) WP(" => total %lld\n",c); // show segment info
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    initV(z0, H_L, c);
    initV(z1, H_L, c);
    //c = 0; L tt=0;
    if(isEq){
        //L tot=0; DOI(vLen, if(tempK[i]>=0){HI t0=tempH[i]; while(t0){t0=t0->inext;tot++;}}) WP("tot = %lld\n",tot); getchar();
        DOT(vLen, if(tempK[i]>=0){L p=offset[tid]; \
                     vL(z0,p)=tempK[i];vL(z1,p)=i; \
                     HI t0=tempH[i]; while(t0){p++;vL(z0,p)=t0->ival;vL(z1,p)=i;t0=t0->inext;} offset[tid]=p+1; })
    }
    else {
        DOT(vLen, if(tempK[i]>=0){L p=offset[tid]; L k0=tempK[i]; DOJ(k0,{vL(z0,p)=j;vL(z1,p)=i;p++;}) k0++;\
                     HI t0=tempH[i]; while(t0){L k1=t0->ival;while(k0<k1){vL(z0,p)=k0;vL(z1,p++)=i;k0++;}k0++;t0=t0->inext;}\
                     while(k0<sLen){vL(z0,p)=k0++; vL(z1,p++)=i;}  offset[tid]=p;}\
                  else{L p=offset[tid]; DOJ(sLen,{vL(z0,p)=j;vL(z1,p)=i;p++;}) offset[tid]=p;})
    }
//#pragma omp parallel for
//    for(L i=0;i<vLen;i++){
//        HI t0=tempH[i];L k=tempK[i];
//#pragma omp critical
//        {
//        if(k>=0){vL(z0,c)=k;vL(z1,c)=i;c++;tt++;while(t0){vL(z0,c)=t0->ival;vL(z1,c)=i;c++;t0=t0->inext;}}
//        }
//    }
time_toc("Step 3: Hash finish\n");
    //TIME_SHOW("Step 3: hash finish");
    //WP("size = %lld, z0 = %lld, z1 = %lld\n",c,vn(z0),vn(z1));
    //WP("size = %lld, tt = %lld\n",c,tt);
    // parallel - error (race condition)
    //DOP(vLen, {HI t0;L k=find_hash_many(hashT,hashMask,src,val,i,typ,&t0);\
               if(k>=0){vL(z0,c)=k;vL(z1,c)=i;c++;while(t0){vL(z0,c)=t0->ival;vL(z1,c)=i;c++;t0=t0->inext;}}},\
        reduction(+:c))
    // serial
    //DOI(vLen, {HI t0;L k=find_hash_many(hashT,hashMask,src,val,i,typ,&t0);\
               if(k>=0){WP("i=%lld, k=%lld\n",i,k);vL(z0,c)=k;vL(z1,c)=i;c++;while(t0){vL(z0,c)=t0->ival;vL(z1,c)=i;c++;t0=t0->inext;}}})
    // free hashT
    profileHash(hashT, hashLen);
    free(tempK);
    free(tempH);
    DOI(20, WP("--")) WP("\n");
    R 0;
}


/* ------------------ Join Experiments ------------------ */

/*
 * This dummy function for verifying the result of hash join,
 *   basically, just the total number of items returned.
 */
I lib_join_dummy(V z0, V z1, V x, V y){
    printBanner("join dummy dummy dummy");
    if(vp(x) == vp(y) && vp(x) == H_LL){
        L c=0;
        WP("Input x and y:\n"); getInfoVar(x); getInfoVar(y); 
        switch(vp(x)){
            caseI DOI(vn(x), DOJ(vn(y), if(vI(x,i)==vI(y,j))c++;)) break;
            //caseI DOJ(vn(y), DOI(vn(x), if(vI(x,i)==vI(y,j))                  \
                     {WP("x[%lld]==%lld, y[%lld]=%lld\n",i,vI(x,i),j,vI(y,j)); \
                      getchar();c++;})) break;
            caseL DOI(vn(x), DOJ(vn(y), if(vL(x,i)==vL(y,j))c++)) break;
        }
        WP("Total # of items after join: %lld\n",c); getchar();
        initV(z0, H_L, c);
        initV(z1, H_L, c);
        c = 0;
        switch(vp(x)){
            caseI DOI(vn(x), DOJ(vn(y), if(vI(x,i)==vI(y,j)){vL(z0,c)=i; vL(z1,c)=j; c++;}))
        }
    }
    else EP("Type not supported: %s, %s",getTypeName(vp(x)),getTypeName(vp(y)));
    R 0;
}

/*
 * check the number of rows in result after a join
 */
I lib_join_dummy2(V x, V y){
    V x0 = vV(x,0), x1 = vV(x,1);
    V y0 = vV(y,0), y1 = vV(y,1);
    if(isInt(x0)&&isInt(x1)&&isInt(y0)&&isInt(y1)){
        L tot = 0;
        DOI(vn(x0), {L c=0;\
            DOJ(vn(y0), if(vI(x0,i)==vI(y0,j) && vI(x1,i)==vI(y1,j)){c++;tot++;})\
            if(c>0)WP("[%lld] vI(x0,%lld)=%d, vI(x1,%lld)=%d\n",c,i,vI(x0,i),i,vI(x1,i));})
        WP("Dummy2: total = %lld\n",tot); getchar();
    }
    else {
        getInfoVar(x); getInfoVar(y);
        EP("Dummy2 only supports: list(I,I) join list(I,I)");
    }
    R 0;
}

/*
 * q17
 * dummy3: 2 int keys, (lt and eq)
 */
I lib_join_dummy3(V x, V y){
    V x0 = vV(x,0), x1 = vV(x,1);
    V y0 = vV(y,0), y1 = vV(y,1);
    if(isDouble(x0)&&isInt(x1)&&isDouble(y0)&&isInt(y1)){
        L tot = 0;
        // DOI(vn(x0), { \
            DOJ(vn(y0), if(vI(x1,i)==vI(y1,j) && vI(x1,i)==122187){ \
                WP("vE(x0,%lld)=%g, vE(y0,%lld)=%g\n",i,vE(x0,i),j,vE(y0,j)); })\
        })
        DOI(vn(x0), {L c=0;\
            DOJ(vn(y0), if(vE(x0,i)<vE(y0,j) && vI(x1,i)==vI(y1,j)){c++;tot++;})\
        })
        WP("Dummy3: total = %lld\n",tot); getchar();
    }
    else {
        getInfoVar(x); getInfoVar(y);
        EP("Dummy3 only supports: list(E,I) join list(E,I)");
    }
    R 0;
}



#ifdef DO_HASH_V0
void profileHashTable(THash th){
    HN *ht = th.hashTable;
    L  *hs = th.hashSize;
    L  num = th.numTable;
    L  cnt = 0;
    DOI(num, if(hs[i]>0){cnt++;WP("hash[%3lld]: ",i);profileHashTableSingle(ht[i],hs[i]);})
    WP("Summary: total %lld tables out of %lld used (%.2lf %%)\n",cnt,num,percent(cnt,num));
}
#endif

#ifdef DO_HASH_V1
static void profileHashBucketSingle(HB x, L size){
    WP("slots used %.2lf %% (%lld/%lld)\n",percent(x->cur,x->size),x->cur,x->size);
}

static L countDupLinkSize(HN x){
    L c = 0;
    if(x){
        HI t0 = hT(x);
        c++; while(t0){t0=t0->inext;c++;}
    }
    R c;
}

static void profileHashBucketDup(TBucket tb){
    HB *ht = tb.hashBucket;
    L  *hs = tb.hashSize;
    L  num = tb.numBucket;
    L max_dup_size = 0;
    DOI(num, \
       DOJ(hs[i], \
          {HB t0=(ht[i]+j); DOK(t0->cur, {L size=countDupLinkSize(t0->node+k); if(max_dup_size<size) max_dup_size=size;})}))
    WP("max dup size: %lld\n", max_dup_size);
    if(max_dup_size == 1){
        WP("  ** data used in key are completely distinct **\n");
    }
}

void profileHashBucket(TBucket tb){
    HB *ht = tb.hashBucket;
    L  *hs = tb.hashSize;
    L  num = tb.numBucket;
    L  cnt = 0;
    //DOI(num, if(hs[i]>0){cnt++;WP("hash[%3lld]: ",i);profileHashBucketSingle(ht[i]);})
    WP("profiling bucket\n");
    profileHashBucketDup(tb);
    WP("Summary: total %lld tables out of %lld used (%.2lf %%)\n",cnt,num,percent(cnt,num));
}
#endif

static B profileIsNested(V x){ DOI(xn, if(isList(vV(x,i)))R 1) R 0; }

void profileListR(V x){
    if(isList(x)){
        if(profileIsNested(x))
            WP("# Nested list (depth > 1) found\n");
        WP("data <- c("); DOI(xn, {if(i>0)WP(",");WP("%lld",vn(vV(x,i)));}) WP(")\n");
    }
    else EP("Type list expected");
}

// special case for q20
B checkOrderCase1(V z, V x, V f){
    if(2==xn && isInt(vV(x,0)) && isInt(vV(x,1)) && vB(f,0)==vB(f,1)){
        if(H_DEBUG)
            STOP("order case 1");
        V x0  = vV(x,0);
        V x1  = vV(x,1);
        //getInfoVar(x0); getInfoVar(x1); getchar();
        //Variable x0 has type H_I and len 909455
        //Variable x1 has type H_I and len 909455
        B f0  = vB(f,0);
        L size = vn(vV(x,0));
    tic();
        V0 t0; V t = &t0;
        initV(t, H_L, 0);
        L *temp = NEWL(L, size);
        vg(t) = (G)temp;
        vn(t) = size;
        DOI(size, temp[i]=((temp[i]|vI(x0,i))<<32)|vI(x1,i))
    toc();
    tic();
        initV(z, H_L, size);
        lib_radixsort_long(sL(z), t, size, &f0, true); // return index
    toc();
        //DOI(100, WP("[%lld] %lld: %d, %d\n",i,temp[vL(z,i)],vI(x0,vL(z,i)),vI(x1,vL(z,i))))
        free(vg(t));
        R true;
    }
    R false;
}

// check the number of keys after groupby
void lib_groupby_dummy(V x){
    if(xp == H_G && xn == 2 && vp(vV(x,0)) == H_I && vp(vV(x,1)) == H_I){
        WP("begin group dummy\n");
        V x0 = vV(x,0);
        V x1 = vV(x,1);
        B *flag = NEWL(B, vn(x0));
        memset(flag, 0, sizeof(B) *vn(x0));
        L cnt = 0;
        for(L i=0,i2=vn(x0); i<i2; i++){
            if(flag[i]) continue;
            flag[i] = 1;
            cnt++;
            for(L j=i+1,j2=vn(x0); j<j2; j++){
                if(vI(x0,i)==vI(x0,j) && vI(x1,i)==vI(x1,j)){
                    flag[j] = 1;
                }
            }
        }
        WP("total result = %lld\n", cnt); getchar(); // Q20: 543210
    }
    else EP("Condition not satisfied");
}

L getInteger1(V x){
    if(isOne(x)){
        switch(xp){
            caseB R xb;
            caseJ R xj;
            caseH R xh;
            caseI R xi;
            caseL R xl;
            default: EP("Not an integer type: %s", getTypeName(xp));
        }
    }
    else EP("Value node must be a singleton");
}

#define InitSeed 16807
#define MaxSeed  2147483647
L HorseSeed = InitSeed;

L genRand(L x){
    HorseSeed = HorseSeed * InitSeed % MaxSeed;
    if(HorseSeed == 0)
        HorseSeed = InitSeed;
    R (L)(x * (HorseSeed * 1.0 / MaxSeed));
}

