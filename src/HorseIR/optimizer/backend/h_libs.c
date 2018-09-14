#include "../global.h"

const I HASH_A = 1223106847;
const I HASH_M = (1LL << 32)-5;

typedef struct hash_index_node {
    I ival;
    struct hash_index_node *inext;
}HI0, *HI;

typedef struct hash_node {
    I h_index;
    HI h_index_other;
    union {
        /* no boolean or char */
        J h_j;
        H h_h;
        I h_i;
        L h_l;
        F h_f;
        E h_e;
        X h_x;
        S h_s;
    };
    struct hash_node *next;
}HN0,*HN;

#define hJ(x) x->h_j
#define hH(x) x->h_h
#define hI(x) x->h_i
#define hL(x) x->h_l
#define hF(x) x->h_f
#define hE(x) x->h_e
#define hX(x) x->h_x
#define hS(x) x->h_s

#define hD(x) x->h_index
#define hT(x) x->h_index_other
#define hN(x) x->next
#define hV(x,k) ((x)+(k))

#define hash_B /* empty */
#define hash_J hash_i32
#define hash_H hash_i32
#define hash_I hash_i32
#define hash_L hash_i64
#define hash_F hash_f32
#define hash_E hash_f64
#define hash_X hash_clex
#define hash_S hash_djb2
#define hash_S_n hash_djb2_n
#define init_H(x) {hI(x)=-1;hN(x)=NULL;hT(x)=NULL;}
#define init_I(x) {x->ival=-1; x->inext=NULL;}

#define toBase(t,v,f) (*((t*)(v)+f))
#define toB(v,f) toBase(B,v,f)
#define toJ(v,f) toBase(J,v,f)
#define toH(v,f) toBase(H,v,f)
#define toI(v,f) toBase(I,v,f)
#define toL(v,f) toBase(L,v,f)
#define toF(v,f) toBase(F,v,f)
#define toE(v,f) toBase(E,v,f)
#define toX(v,f) toBase(X,v,f)
#define toS(v,f) toBase(S,v,f)
#define toC(v,f) toBase(C,v,f)

L *LARGE_BUFF; // used in merge sort

/*
 * A method name starts with "lib" and ends with a specific type
 * e.g. libIndexOf_I
 */

/*
 * Hash tables
 * http://stackoverflow.com/questions/9624963/java-simplest-integer-hash
 */

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

UI hash_djb2(S str){
    UI h = 5381; I c;
    while(0!=(c=*str++))
        h=((h<<5)+h)+(*str++);
    R h;
}

UI hash_djb2_n(S str, L n){
    UI h = 5381;
    DOI(n, h=((h<<5)+h)+(*str++)) /* h * 33 + c */
    R h;
}

UI hash_sdbm(S str, L n){
    UI h = 0;
    DOI(n, h=(*str++)+(h<<6)+(h<<6)-h)
    return h;
}

UI hash_lose(S str, L n){
    UI h = 0;
    DOI(n, h+=(*str++))
    R h;
}

UI hash_f32(F n){
    R hash_S_n((S)(&n),sizeof(F));
}

UI hash_f64(E n){
    R hash_S_n((S)(&n),sizeof(E));
}

UI hash_clex(X n){
    R hash_S_n((S)(&n),sizeof(X));
}

UI getHashTableSize(L x){
    R (x<16?32:(x<<1));
}

UI getHashValue(void* val, L valI, L typ){
    switch(typ){
        caseJ R hash_J(toJ(val,valI));
        caseH R hash_H(toH(val,valI));
        caseI R hash_I(toI(val,valI));
        caseL R hash_L(toL(val,valI));
        caseF R hash_F(toF(val,valI));
        caseE R hash_E(toE(val,valI));
        caseX R hash_X(toX(val,valI));
        caseS R hash_S(toS(val,valI));
        caseG { V x=(V)val; UI c=0; DOI(vn(x), c+=getHashValue(sG(vV(x,i)),valI,vp(vV(x,i)))) R c; }
    }
    R 0;
}

L createHash(HN *hashT, L hashLen){
    *hashT = (HN) malloc(sizeof(HN0) * hashLen);
    memset(*hashT, 0, sizeof(HN0)*hashLen);
    R 0;
}

/* free a hash table */
L freeHash(HN hashT){
    R 0;
}

HN newHashNode(){
    HN x = (HN) malloc(sizeof(HN0)); init_H(x);
    R x;
}

HI newIndexNode(){
    HI x = (HI) malloc(sizeof(HI0)); init_I(x);
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

// isU: isUnique
static L insert_hash(HN ht, L htSize, void* src, L srcI, L typ, B isU){
    L hashKey = getHashValue(src,srcI,typ) % htSize;
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
            caseS if(!strcmp(toS(src,td),toS(src,srcI)))
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

static L find_hash(HN ht, L htSize, void* src, void* val, L valI, L typ){
    L hashKey = getHashValue(val,valI,typ) % htSize;
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
            caseS if(!strcmp(toS(src,td),toS(val,valI))) R td; break;
            caseG if(compareTuple((V)src,td,(V)val,valI))R td; break;
            default: R -1;
        }
    }
    R -1;
}

static L find_hash_many(HN ht, L htSize, void* src, void* val, L valI, L typ, HI *other){
    L hashKey = getHashValue(val,valI,typ) % htSize;
    HN t = hV(ht,hashKey);
    *other = NULL; // default
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
            caseS if(!strcmp(toS(src,td),toS(val,valI))) {*other=hT(t);R td;} break;
            caseG if(compareTuple((V)src,td,(V)val,valI)){*other=hT(t);R td;} break;
            default: R -1;
        }
    }
    R -1;
}

//L findHash(HN ht, L htSize, void* src, L srcI, void* val, L valI, L typ){
//    R find_hash(ht,htSize,src,val,valI,typ);
//}

L insertHash(HN ht, L htSize, void* src, L srcI, void* val, L valI, L typ){
    R insert_hash(ht,htSize,src,srcI,typ,true);
}

L insertHashMany(HN ht, L htSize, void* src, L srcI, void* val, L valI, L typ){
    R insert_hash(ht,htSize,src,srcI,typ,false);
}

L profileHash(HN ht, L htSize){
    L minValue=99999, maxValue=-1, nonZero=0;
    P("\n> Hash table size: %lld\n",htSize);
    DOI(htSize, {L c=0; HN t=hV(ht,i); while(hN(t)){c++;t=hN(t);} \
        if(c!=0) nonZero++; \
        if(c<minValue) minValue=c; \
        if(c>maxValue) maxValue=c; })
    P("> Sparsity (%lld/%lld) = %g%%\n",nonZero,htSize, nonZero*100.0/htSize);
    P("> min = %lld, max = %lld\n\n",minValue,maxValue);
    R 0;
}


/* functions exposed as libraries */

// #define lib_index_template(typ) \
//     P("......"); getchar(); \
//     HN hashT; \
//     L hashLen = getHashTableSize(sLen); \
//     CHECKE(createHash(&hashT,hashLen)); \
//     DOI(sLen, insertHash(hashT,hashLen,src,i,NULL,-1,typ)) \
//     struct timeval tv0, tv1; E optTime[10];\
//     DOJ(10, {gettimeofday(&tv0, NULL); DOP(vLen, {L t=findHash(hashT,hashLen,src,-1,val,i,typ);targ[i]=t<0?sLen:t;}) \
//     gettimeofday(&tv1, NULL); optTime[j] = calcInterval(tv0, tv1)/1000.0;}) \
//     E total = 0; DOI(10, total += optTime[i]) P("Createing index time (avg): %g ms\n", total/10);

#define lib_index_template(typ) \
    HN hashT; \
    L hashLen = getHashTableSize(sLen); \
    CHECKE(createHash(&hashT,hashLen)); \
    DOI(sLen, insertHash(hashT,hashLen,src,i,NULL,-1,typ)) \
    DOI(vLen, {L t=find_hash(hashT,hashLen,src,val,i,typ);targ[i]=t<0?sLen:t;})

L lib_index_of_B(L* targ, B* src, L sLen, B* val, L vLen){
    L flag[2]={-1}; I c=0;
    DOI(sLen, if(c<2 && -1==flag[src[i]]){flag[src[i]]=i;c++;if(2==c)break;})
    DOP(vLen, {L t=flag[val[i]]; targ[i]=t<0?sLen:t;})
    R 0;
}

L lib_index_of_J(L* targ, J* src, L sLen, J* val, L vLen){
    lib_index_template(H_J);
    R 0;
}

L lib_index_of_H(L* targ, H* src, L sLen, H* val, L vLen){
    lib_index_template(H_H);
    R 0;
}

L lib_index_of_I(L* targ, I* src, L sLen, I* val, L vLen){
    lib_index_template(H_I);
    R 0;
}

L lib_index_of_L(L* targ, L* src, L sLen, L* val, L vLen){
    lib_index_template(H_L);
    R 0;
}

// void lib_index_of_L(L* targ, L* src, L sLen, L* val, L vLen){
//     DOI(vLen, {B f=1;\
//         DOJ(sLen, if(val[i]==src[j]){targ[i]=j;f=0;break;})\
//         if(f)targ[i]=sLen;})
// }

L lib_index_of_F(L* targ, F* src, L sLen, F* val, L vLen){
    lib_index_template(H_F);
    R 0;
}

L lib_index_of_E(L* targ, E* src, L sLen, E* val, L vLen){
    lib_index_template(H_E);
    R 0;
}

L lib_index_of_C(L* targ, C* src, L sLen, C* val, L vLen){
    L buff[256];
    memset(buff,-1,256*sizeof(L));
    DOI(sLen, if(buff[src[i]]==-1) buff[src[i]]=i)
    DOI(vLen, {L t=buff[val[i]]; targ[i]=-1==t?sLen:t;})
    R 0;
}

L lib_index_of_X(L* targ, X* src, L sLen, X* val, L vLen){
    lib_index_template(H_X);
    R 0;
}

L lib_index_of_S(L* targ, S* src, L sLen, S* val, L vLen){
    lib_index_template(H_S);
    R 0;
}

L lib_index_of_G(L* targ, V src, L sLen, V val, L vLen){
    lib_index_template(H_G);
    R 0;
}

void lib_quicksort(L *rtn, V val, L low, L high, B *isUp, FUNC_CMP(cmp)){
    switch(vp(val)){
        caseC lib_quicksort_char(rtn, val, low, high, isUp, cmp); break;
        caseL lib_radixsort_long(rtn, val, high, isUp, true); break;
        default: lib_quicksort_other(rtn, val, low, high, isUp, cmp); break;
    }
}

void lib_quicksort_char(L *rtn, V val, L low, L high, B *isUp, FUNC_CMP(cmp)){
    S str = sC(val);  L len = high-low;  B f0 = isUp?1:*isUp;
    if(len < 10){
        DOI3(low, high, DOJ3(low, high, if(str[rtn[i]] > str[rtn[j]]) { L t=rtn[i]; rtn[i]=rtn[j]; rtn[j]=t; } ))
    }
    else {
        P("entering large char sort: len = %lld\n", len);
        L cnt[256]={0}, step[256]={0}, cursor=0; L *temp = (L*)malloc(sizeof(L)*len);
        DOI3(low, high, cnt[str[rtn[i]]]++);
        DOI(256, {if(cnt[i]!=0) step[i] = cursor; cursor += cnt[i]; })
        DOI(256, if(step[i]!=0)P("step[%lld] = %lld\n",i,step[i]))
        DOI3(low, high, temp[step[str[rtn[i]]]++]=rtn[i])
        memcpy(rtn+low, temp, sizeof(L)*len);
        free(temp);
    }
    if(!f0){ // desc order
        DOI3(low, (low+high)/2, { L t=rtn[i]; rtn[i]=rtn[high-i+1]; rtn[high-i+1]=t; })
    }
}


void lib_quicksort_other(L *rtn, V val, L low, L high, B *isUp, FUNC_CMP(cmp)){
    if(low < high){
        B leftSame=true;
        L pos = lib_partition(rtn, val, low, high, isUp, cmp, &leftSame);
        //if(leftSame) { P("low = %lld, high = %lld\n",low,high); getchar(); }
        //P("low = %lld, high = %lld, %lld, %lld\n",low,high,leftSame,pos);
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


L lib_partition(L *rtn, V val, L low, L high, B *isUp, FUNC_CMP(cmp), B *leftSame){
    L pivot = low, i = low, j = high, t;
    while(true){
        while(++i && i<high){
            L temp = (*cmp)(val,rtn[i],rtn[pivot],isUp);
            if(temp != 0) *leftSame = false;
            if(temp > 0) break;
        }
        do{--j;} while((*cmp)(val,rtn[j],rtn[pivot],isUp) > 0);
        if(i>=j) break;
        t = rtn[i]; rtn[i] = rtn[j]; rtn[j] = t;
    }
    t = rtn[low]; rtn[low] = rtn[j]; rtn[j] = t;
    R j;
}

#define cmp_switch(x) (x?-1:1):(x?1:-1)

L lib_quicksort_cmp(V val, L a, L b, B *isUp){
    DOI(vn(val), {V t=vV(val,i); B f=isUp?isUp[i]:1; \
                  L x=lib_quicksort_cmp_item(t,a,b,&f); if(x!=0)R x;})
    R 0;
}

L lib_quicksort_cmp_item(V t, L a, L b, B *isUp){
    B f = isUp?*isUp:1;
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
        default: P("No impl. for type %lld\n",vp(t)); exit(99); break; \
        /* Pending: caseC */
    } 
    // R (a>b?1:a<b?-1:0);
    #ifdef SORT_MERGE
        R a>b?cmp_switch(f); //think..
    #else
        R 0;
    #endif
}

void lib_mergesort(L *rtn, V val, L low, L high, B *isUp, FUNC_CMP(cmp)){
    // P("Entering merge sort: (%lld, %lld)\n",low,high);
    // #define SWAP_I(a,b) { L temp=rtn[a]; rtn[a]=rtn[b]; rtn[b]=temp; }
    #define SWAP_I(a,b) { rtn[a]^=rtn[b]; rtn[b]^=rtn[a]; rtn[a]^=rtn[b]; }
    // P("type of val: %lld\n", vp(val));
    // P("before:"); DOI3(low, high, P(" %lld",vL(val,rtn[i]))); P("\n");
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
                        // P("trace 1\n");
                        LARGE_BUFF[i]=rtn[curRight++];
                    }
                    else if(curRight == high){
                        // P("trace 2\n");
                        LARGE_BUFF[i]=rtn[curLeft++];
                    }
                    else if((*cmp)(val,rtn[curLeft],rtn[curRight],isUp)>0){
                        // P("trace 3\n");
                        LARGE_BUFF[i]=rtn[curRight++];
                    }
                    else {
                        // P("trace 4\n");
                        LARGE_BUFF[i]=rtn[curLeft++];
                    }
                }
                memcpy(rtn+low,LARGE_BUFF,sizeof(L)*len);
                // P("merged: %lld + %lld = %lld\n", mid-low, high-mid, len);
                // free(buff);
            } // else no need to sort
        }
    }
    // P("after:"); DOI3(low, high, P(" %lld",vL(val,rtn[i]))); P("\n");
    // getchar();
    // P("exit.\n");
}

// the same as lib_quicksort_list
L lib_mergesort_decide(V val, L a, L b, B *isUp, FUNC_CMP(cmp)){
    DOI(vn(val), {L k=(*cmp)(vV(val,i),a,b,isUp+i); if(k!=0) R k; })
    R (a>b?1:a<b?-1:0);
}

void lib_mergesort_list(L *rtn, V val, L low, L high, B *isUp, FUNC_CMP(cmp)){
    // P("Entering merge sort: (%lld, %lld)\n",low,high);
    // #define SWAP_I(a,b) { L temp=rtn[a]; rtn[a]=rtn[b]; rtn[b]=temp; }
    #define SWAP_I(a,b) { rtn[a]^=rtn[b]; rtn[b]^=rtn[a]; rtn[a]^=rtn[b]; }
    // P("type of val: %lld\n", vp(val));
    // P("before:"); DOI3(low, high, P(" %lld",vL(val,rtn[i]))); P("\n");
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
                // P("1. low = %lld, high = %lld\n", low, high); // check
                L curLeft=low, curRight=mid;
                // L *buff = (L*)malloc(sizeof(L)*len);
                for(L i=0;i<len;i++){
                    if(curLeft == mid){
                        // P("trace 1\n");
                        LARGE_BUFF[i]=rtn[curRight++];
                    }
                    else if(curRight == high){
                        // P("trace 2\n");
                        LARGE_BUFF[i]=rtn[curLeft++];
                    }
                    // else if((*cmp)(val,rtn[curLeft],rtn[curRight],isUp)>0){
                    else if(lib_mergesort_decide(val,rtn[curLeft],rtn[curRight],isUp,cmp)>0){
                        // P("trace 3\n");
                        LARGE_BUFF[i]=rtn[curRight++];
                    }
                    else {
                        // P("trace 4\n");
                        LARGE_BUFF[i]=rtn[curLeft++];
                    }
                }
                memcpy(rtn+low,LARGE_BUFF,sizeof(L)*len);
                // P("merged: %lld + %lld = %lld\n", mid-low, high-mid, len);
                // free(buff);
            } // else no need to sort
            else if(lib_mergesort_decide(val,rtn[low],rtn[high-1],isUp,cmp)>0){
                P("2. low = %lld, high = %lld\n", low, high); // in q16, no case found
            }
        }
    }
    // P("after:"); DOI3(low, high, P(" %lld",vL(val,rtn[i]))); P("\n");
    // getchar();
    // P("exit.\n");
}

/*
 * check if it is a sorted vector by a given order, isUp
 * isUp: true  -> -1
 *     : false ->  1
 */
B lib_order_by_sorted(L *targ, V val, B *isUp, L low, L high, FUNC_CMP(cmp)){
    L rtn = 0;
    #define DOIm(m, n, x) {for(L i=m+1,i2=n;i<i2;i++)x;}
    DOIm(low, high, {L t=(*cmp)(val,targ[i-1],targ[i],isUp); \
        rtn=rtn==0?t:rtn==-1?(t==1?-2:rtn):(t==-1?-2:rtn); \
        if(rtn==-2) { /*P("i=%lld, %lld, %lld\n",i,vL(val,i-1),vL(val,i));*/ R 0;} })
    R rtn==(*isUp?-1:1);
}

void lib_order_by_list(L *targ, V val, B *isUp, L tLen, L colId, FUNC_CMP(cmp)){
    DOP(tLen, targ[i]=i);
    #if defined(OPT_Q16) || defined(SORT_MERGE)
        /* quick sort */
        // lib_quicksort_other(targ, val, 0, tLen, isUp, lib_quicksort_cmp);
        /* merge sort (fast) */ 
        LARGE_BUFF = (L*)malloc(sizeof(L)*vn(vV(val,0)));
        lib_mergesort_list(targ, val, 0, tLen, isUp, cmp);
        free(LARGE_BUFF);
    #else
        lib_quicksort_list(targ, val, isUp, 0, tLen, colId, cmp);
    #endif
}

void lib_quicksort_list(L *targ, V val, B *isUp, L low, L high, L colId, FUNC_CMP(cmp)){
    if(colId >= vn(val)) R;
    V curV = vV(val,colId);
    B* curB = isUp+colId;
    //struct timeval tv0, tv1;
    // gettimeofday(&tv0, NULL);
    if(lib_order_by_sorted(targ,curV,curB,low,high,cmp)){
        // if(colId == 2 && low == 29 && high == 33){
        //     P("happy: colId = %lld, range = (%lld,%lld)\n", colId,low,high);
        // }
    }
    else {
        // if(colId == 2 && low == 29 && high == 33){
        //     P("xxxxx\n");
        // }
        //#ifdef OPT_Q16
        //    LARGE_BUFF = (L*)malloc(sizeof(L)*vn(curV));
        //    lib_mergesort(targ, curV, low, high, curB, cmp);
        //    free(LARGE_BUFF);
        //#else
            if(isInteger(curV)) {
                LARGE_BUFF = (L*)malloc(sizeof(L)*vn(curV));
                lib_mergesort(targ, curV, low, high, curB, cmp);
                free(LARGE_BUFF);
            }
            else lib_quicksort(targ, curV, low, high, curB, cmp);
        //#endif
    }
    // gettimeofday(&tv1, NULL);
    // if(colId == 0)
    // P("1. Result (elapsed time %g ms)\n\n", calcInterval(tv0,tv1)/1000.0);
    // gettimeofday(&tv0, NULL);
    {
        // P("len: %lld - %lld = %lld\n", high, low, high-low); getchar();
        int pos = low;
        do{
            // find next
            L start = pos, end = high;
            L hend = start + 5, hpos = -1; // heuristic end/pos
            if(hend>high) hend=high;
            // P("type of curV: %lld\n", vp(curV));
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
                //     P("pivot char: %c | %c\n", vC(curV, targ[pivot]), vC(curV, targ[end]));
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
        default: EP("xxxx\n");
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
    if(vp(val) != H_L) DOP(tLen, targ[i]=i)
    // P("tLen = %lld\n", tLen);
    lib_quicksort(targ, val, 0, tLen, isUp, cmp);
    //lib_order_vector(targ, val, tLen, isUp); // experimental
    // P("done.\n");
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
    // P("Result:\n");
    // DOI(lenZ, P("segZ[%lld] = %lld\n", i,segZ[i]))
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

L lib_get_group_by_other(V z, V val, L* index, L iLen, L (*cmp)(V,L,L,B*)){
    L k, c, cz; V d,t;
    /* 1. get the total number of cells: lenZ */
    // P("step 1\n");
    L lenZ=iLen>0?1:0;
    DOIa(iLen, if(0!=(*cmp)(val,index[i-1],index[i],NULL))lenZ++)
    /* 2. allocate list and get the info of each cell */
    // P("step 2\n");
    // initV(z, H_N, lenZ);
    initV(z, H_N, 2);
    V zKey = getDictKeys(z);
    V zVal = getDictVals(z);
    initV(zKey, H_L, lenZ);
    initV(zVal, H_G, lenZ);
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
    // P("step 3\n");
    k=0, c=0;
    d=vV(zVal,k++);
    if(iLen>0) vL(d,c++)=index[0];
    DOIa(iLen, if(0!=(*cmp)(val,index[i-1],index[i],NULL)){ \
                  d=vV(zVal,k++); vL(d,0)=index[i]; c=1; } \
               else vL(d,c++)=index[i])
    // P("exit\n");
    R 0;
}

/* 
 * copy from lib_get_group_by_other
 * index[x] => x
 */
L lib_get_group_by_order(V z, V val, L* index, L iLen, L (*cmp)(V,L,L,B*)){
    L k, c, cz; V d,t;
    /* 1. get the total number of cells: lenZ */
    // P("step 1\n");
    L lenZ=iLen>0?1:0;
    DOIa(iLen, if(0!=(*cmp)(val,i-1,i,NULL))lenZ++)
    /* 2. allocate list and get the info of each cell */
    // P("step 2\n");
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
    // P("step 3\n");
    k=0, c=0;
    d=vV(zVal,k++);
    if(iLen>0) vL(d,c++)=0;
    DOIa(iLen, if(0!=(*cmp)(val,i-1,i,NULL)){ \
                  d=vV(zVal,k++); vL(d,0)=i; c=1; } \
               else vL(d,c++)=i)
    // P("exit\n");
    R 0;
}

L lib_get_group_by(V z, V val, L* index, L iLen, L (*cmp)(V,L,L,B*)){
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
//     P("step 1\n");
//     L lenZ=iLen>0?1:0;
//     DOIa(iLen, if(0!=(*cmp)(val,index[i-1],index[i],NULL))lenZ++)
//     /* 2. allocate list and get the info of each cell */
//     P("step 2\n");
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
//     P("step 3\n");
//     k=0, c=0;
//     d=vV(zVal,k++);
//     if(iLen>0) vL(d,c++)=index[0];
//     DOIa(iLen, if(0!=(*cmp)(val,index[i-1],index[i],NULL)){ \
//                   d=vV(zVal,k++); vL(d,0)=index[i]; c=1; } \
//                else vL(d,c++)=index[i])
//     P("exit\n");
//     R 0;
// }

#define LIB_MEMBER_F1(p) case##p R to##p(val,valI)==to##p(src,0)
#define LIB_MEMBER_F1F(p,foo) case##p R foo(to##p(val,valI),to##p(src,0))

B lib_member_fast1(void* src, void* val, L valI, L typ){
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
        default: { P("Not impl. for type %lld\n", typ); exit(99); }
    }
    R 0;
}


#define LIB_MEMBER_F2(p) case##p R to##p(val,valI)==to##p(src,0) || to##p(val,valI)==to##p(src,1)
#define LIB_MEMBER_F2F(p,foo) case##p R foo(to##p(val,valI),to##p(src,0)) || foo(to##p(val,valI),to##p(src,1))

B lib_member_fast2(void* src, void* val, L valI, L typ){
    // P("match %lld (%s), %lld (%s), %lld (%s) == %d\n", toL(val,valI), getSymbolStr(toL(val,valI))\
    //                                                  , toL(src,0),    getSymbolStr(toL(src,0)) \
    //                                                  , toL(src,1),    getSymbolStr(toL(src,1))\
    //                                                  , toL(val,valI)==toL(src,0) || toL(val,valI)==toL(src,1));
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
        default: { P("Not impl. for type %lld\n", typ); exit(99); }
    }
    R 0;
}

#define lib_member_template(typ)\
    if(sLen > 2) { lib_member_template_normal(typ) }\
    else if(sLen == 2) { DOP(vLen, targ[i]=lib_member_fast2(src,val,i,typ)) }\
    else { DOP(vLen, targ[i]=lib_member_fast1(src,val,i,typ)) }
    // { L k=0; DOI(vLen, k+=targ[i]) P("total sum = %lld\n", k); }

#define lib_member_template_normal(typ)\
    HN hashT; \
    L hashLen = getHashTableSize(sLen); \
    CHECKE(createHash(&hashT,hashLen)); \
    DOI(sLen, insertHash(hashT,hashLen,src,i,NULL,-1,typ)) \
    DOP(vLen, targ[i]=(0<=find_hash(hashT,hashLen,src,val,i,typ)))
    // profileHash(hashT,hashLen);


L lib_member_B(B* targ, B* src, L sLen, B* val, L vLen){
    B flag[2]={0};
    DOI(sLen, flag[src[i]]=1)
    DOP(vLen, targ[i]=flag[val[i]])
    R 0;
}

L lib_member_H(B* targ, H* src, L sLen, H* val, L vLen){
    lib_member_template(H_H);
    R 0;
}

L lib_member_I(B* targ, I* src, L sLen, I* val, L vLen){
    lib_member_template(H_I);
    R 0;
}

L lib_member_L(B* targ, L* src, L sLen, L* val, L vLen){
    lib_member_template(H_L);
    R 0;
}

L lib_member_F(B* targ, F* src, L sLen, F* val, L vLen){
    lib_member_template(H_F);
    R 0;
}

L lib_member_E(B* targ, E* src, L sLen, E* val, L vLen){
    lib_member_template(H_E);
    R 0;
}

L lib_member_C(B* targ, C* src, L sLen, C* val, L vLen){
    B buff[256];
    memset(buff,0,256*sizeof(B));
    DOI(sLen, if(!buff[src[i]]) buff[src[i]]=1)
    DOP(vLen, targ[i]=buff[val[i]];)
    R 0;
}

L lib_member_X(B* targ, X* src, L sLen, X* val, L vLen){
    lib_member_template(H_X);
    R 0;
}

L lib_member_S(B* targ, S* src, L sLen, S* val, L vLen){
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
    P("current path: %s\n", pBuf);
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
//    //P("done.\n"); getchar();
//    U32 out; L size = n * 2;
//    struct timeval tv0, tv1;
//    gettimeofday(&tv0, NULL);
//    L *qq = (L*)malloc(sizeof(L)*10);
//    HT* table = (HT*)malloc(sizeof(HT)*size);
//    HT* lastp = (HT*)malloc(sizeof(HT)*size);
//    HT nodes  = (HT)malloc(sizeof(HT0)*n);
//    memset(table, 0, sizeof(HT)*size);
//    gettimeofday(&tv1, NULL);
//    P("0. Exe. time (ms): %g\n", calcInterval(tv0, tv1));
//    //DOI(n, {MurmurHash3(val+i, 1, 16807, &out); if(out%size==9348)P("%lld ", val[i]); table[out%size]++;})
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
//    P("1. Exe. time (ms): %g\n", calcInterval(tv0, tv1)); getchar();
//    P("out = %u\n", out);
//    gettimeofday(&tv0, NULL);
//    for(L i=0; i<size;i++){
//        if(table[i]){
//            HT t = table[i]; L tot = 0; while(t){ tot++; /*P("%lld ", val[t->index]);*/ t=t->next;} //P("\n");
//            B *visited = (B*)malloc(sizeof(B)*tot); memset(visited, 0, sizeof(B)*tot);
//            t = table[i];
//            DOJ(tot, {if(!visited[j]){ visited[j]=true; HT p = t->next; L cnt = 0; \
//                L k = j+1; while(p){ if(val[t->index]==val[p->index]){visited[k]=true; cnt++;} p = p->next; k++; } \
//                /*P("val = %lld, cnt = %lld\n",val[t->index],cnt);*/ } t=t->next;} )
//            free(visited);
//        }
//    } 
//    gettimeofday(&tv1, NULL);
//    P("2. Exe. time (ms): %g\n", calcInterval(tv0, tv1)); getchar();
//    /*L tot = 0, maxv = 0;
//    DOI(size, if(table[i]>table[maxv])maxv=i)
//    L minv = maxv;
//    DOI(size, if(table[i]>0 && table[i]<table[minv])minv=i)
//    DOI(size, tot += table[i]>0)
//    P(">0: %lld, %lld (%lld), %lld (%lld)\n",tot,maxv,table[maxv],minv,table[minv]);
//    */
//    free(lastp);
//    free(table);
//    free(nodes);
//}

/* radix sort (experimental) */
typedef struct Pos{L x;I i;}Pos;

/*
 * https://blog.csdn.net/WINCOL/article/details/4799979
 */
static void lib_radixsort_basic(Pos *val, I len){
    const L size = 255;
    int *tempC = (int*)malloc(sizeof(int)<<8);
    Pos *tempB = (Pos*)malloc(sizeof(Pos)*len);
    I maxSize = 32;
    const L halfL = 0xffffffff;
    for(I i=0; i<len; i++){
        //int numOfDigits = (int)log10(val[i].x) + 1;
        if(((val[i].x)>>32) > 0){
            maxSize = 64; break;
        }
    }
    WP("maxSize = %d\n", maxSize);
    for(I j=0,j2=maxSize; j<j2; j+=8){
        memset(tempC, 0, sizeof(int)<<8);
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
    B f0 = isUp?1:*isUp;
    Pos *pos = (Pos*)malloc(sizeof(Pos)*len);
    DOP(len, {pos[i].x=vL(val,i); pos[i].i=i;})
    lib_radixsort_basic(pos, len);
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

L lib_group_by_normal(V z, V x){
    Pos *pos = (Pos*)malloc(sizeof(Pos)*xn);
    DOP(xn, {pos[i].x=vL(x,i); pos[i].i=i;})
    lib_radixsort_basic(pos, xn);
    L k, c, cz; V d,t;
    L start=xn>0?1:0;
    /* 1. get the total number of cells: lenZ */
    // P("step 1\n");
    L lenZ=start;
    DOIa(xn, if(pos[i-1].x!=pos[i].x)lenZ++)
    /* 2. allocate list and get the info of each cell */
    // P("step 2\n");
    initV(z, H_N, 2);
    V zKey = getDictKeys(z);
    V zVal = getDictVals(z);
    initV(zKey, H_L, lenZ);
    initV(zVal, H_G, lenZ);
    /* 3. fill indices into each cell */
    // P("step 3\n");
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
L lib_group_by_flat(V z, V x){
    Pos *pos = (Pos*)malloc(sizeof(Pos)*xn);
    DOP(xn, {pos[i].x=vL(x,i); pos[i].i=i;})
    lib_radixsort_basic(pos, xn);
    L k, c, cz; V d,t;
    L start=xn>0?1:0;
    /* 1. get the total number of cells: lenZ */
    // P("step 1\n");
    L lenZ=start;
    DOIa(xn, if(pos[i-1].x!=pos[i].x)lenZ++)
    /* 2. allocate list and get the info of each cell */
    // P("step 2\n");
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
    // P("step 3\n");
    k=0, c=start;
    if(start) vL(zKey,k)=pos[0].i;
    DOIa(xn, if(pos[i-1].x!=pos[i].x){vL(zVal,k++)=c; vL(zKey,k)=pos[i].i; c=1;} \
               else c++)
    if(c>0) vL(zVal,k)=c;
    //printV(x); printV(zKey); printV(zVal); getchar();
    free(pos);
    R 0;
}


/* index for join index*/

L lib_join_index_hash(V z0, V z1, V x, V y){
    L sLen=vn(x), vLen=vn(y), typ=vp(x);
    void *src=sG(x), *val=sG(y);
    HN hashT;
    L hashLen = getHashTableSize(sLen);
    CHECKE(createHash(&hashT,hashLen));
    struct timeval tv0, tv1;
    gettimeofday(&tv0, NULL);
    DOI(sLen, insertHashMany(hashT,hashLen,src,i,NULL,-1,typ))
    gettimeofday(&tv1, NULL);
    P("Time 1 (ms): %g\n", calcInterval(tv0,tv1));
    gettimeofday(&tv0, NULL);
    L c = 0;
    L *tempK = (L*)malloc(sizeof(L)*vLen);
    HI *tempH = (HI*)malloc(sizeof(HI)*vLen);
    //DOP(vLen, {HI t0;L k=find_hash_many(hashT,hashLen,src,val,i,typ,&t0); \
               tempK[i]=k; tempH[i]=t0;\
               if(k>=0){c++;while(t0){c++;t0=t0->inext;}}}, \
       reduction(+:c))
    DOP(vLen, tempK[i]=find_hash_many(hashT,hashLen,src,val,i,typ,&tempH[i]))
    gettimeofday(&tv1, NULL);
    P("Time 2 (ms): %g\n", calcInterval(tv0,tv1));
    L parZ[H_CORE], offset[H_CORE]; 
    memset(parZ, 0, sizeof(L)*H_CORE);
    memset(offset, 0, sizeof(L)*H_CORE);
    DOT(vLen, if(tempK[i]>=0) {parZ[tid]++; HI t0=tempH[i]; while(t0){parZ[tid]++;t0=t0->inext;} })
    DOI(H_CORE, P("%lld ",parZ[i])) P("\n");
    DOI(H_CORE, c+=parZ[i])
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    initV(z0, H_L, c);
    initV(z1, H_L, c);
    //c = 0; L tt=0;
    gettimeofday(&tv0, NULL);
    DOT(vLen, if(tempK[i]>=0){L p=offset[tid];\
                             vL(z0,p)=tempK[i];vL(z1,p)=i; \
                             HI t0=tempH[i]; while(t0){p++;vL(z0,p)=t0->ival;vL(z1,p)=i;t0=t0->inext;} offset[tid]=p+1; })
//#pragma omp parallel for
//    for(L i=0;i<vLen;i++){
//        HI t0=tempH[i];L k=tempK[i];
//#pragma omp critical
//        {
//        if(k>=0){vL(z0,c)=k;vL(z1,c)=i;c++;tt++;while(t0){vL(z0,c)=t0->ival;vL(z1,c)=i;c++;t0=t0->inext;}}
//        }
//    }
    gettimeofday(&tv1, NULL);
    P("Time 3 (ms): %g\n", calcInterval(tv0,tv1));
    P("size = %lld\n",c);
    //P("size = %lld, tt = %lld\n",c,tt);
    // parallel - error (race condition)
    //DOP(vLen, {HI t0;L k=find_hash_many(hashT,hashLen,src,val,i,typ,&t0);\
               if(k>=0){vL(z0,c)=k;vL(z1,c)=i;c++;while(t0){vL(z0,c)=t0->ival;vL(z1,c)=i;c++;t0=t0->inext;}}},\
        reduction(+:c))
    // serial
    //DOI(vLen, {HI t0;L k=find_hash_many(hashT,hashLen,src,val,i,typ,&t0);\
               if(k>=0){P("i=%lld, k=%lld\n",i,k);vL(z0,c)=k;vL(z1,c)=i;c++;while(t0){vL(z0,c)=t0->ival;vL(z1,c)=i;c++;t0=t0->inext;}}})
    // free hashT
    profileHash(hashT, hashLen);
    free(tempK);
    free(tempH);
    R 0;
}



















