#include "h_global.h"

const L HASH_A = 1223106847L;
const L HASH_M = (1L << 32)-5;

typedef struct hash_node {
    L h_value, h_index;
}HN0,*HN;

/*
 * A method name starts with "lib" and ends with a specific type
 * e.g. libIndexOf_I
 */


// L lib_index_of_i32(L *indx, L *src, L slen, L *targ, L tlen){
//  R 0;
// }


// L libIndexOf_I(L *targ, I *src, L src_len, I *val, L val_len){
//  HN hashT = createHashTable(src, src_len);
//  targ = (L*)malloc(sizeof(L)*val_len);
//  DOI(val_len, {L x=hashCode[val[i]]; B z=false;\
//               while(!(z=(hashT[x].h_value==x && val[i]==src[hashT[x].h_index])))\
//                  {x=(x+1)%val_len;}\
//               targ[i]=z?hashT[x].h_index:val_len;})
//  free(hashT);
//  R 0;
// }

/*
 * Hash tables
 * http://stackoverflow.com/questions/9624963/java-simplest-integer-hash
 */
L quickMod(L x){
    L j = x >> 32;
    L k = x & 0xffffffffL;
    return j*5+k;
}

L hashCode(I n){
    L h = HASH_A*(n&0xffffffffL);
    L h2 = quickMod(quickMod(h));
    L rtn =  (L) (h2 >= HASH_M ? (h2-HASH_M) : h2);
    return rtn < 0 ? -rtn : rtn;
}


L calcSizeExp(L x){
    L n = 64;
    while(x > n) { n<<=1; }
    R n;
}

L calcHashSize(L x) {
    R (x<16?32:calcSizeExp(x));
}

HN createHashTable(I *src, L src_len){
    L hashLen = calcHashSize(src_len);
    HN hashT  = (HN) malloc(sizeof(HN0) * hashLen);
    memset(hashT, -1, sizeof(HN0)*hashLen);
    DOI(src_len, {L x=hashCode(src[i]);while(hashT[x].h_index!=-1){x=(x+1)%hashLen;}\
        hashT[x].h_value=x; hashT[x].h_index=i;})
    R hashT;
}

/* functions exposed as libraries */

void lib_index_of_B(L* targ, B* src, L sLen, B* val, L vLen){
}

void lib_index_of_H(L* targ, H* src, L sLen, H* val, L vLen){
}

void lib_index_of_I(L* targ, I* src, L sLen, I* val, L vLen){
}

void lib_index_of_L(L* targ, L* src, L sLen, L* val, L vLen){
    DOI(vLen, {B f=1;\
        DOJ(sLen, if(val[i]==src[j]){targ[i]=j;f=0;break;})\
        if(f)targ[i]=sLen;})
}

void lib_index_of_F(L* targ, F* src, L sLen, F* val, L vLen){
}

void lib_index_of_E(L* targ, E* src, L sLen, E* val, L vLen){
}

void lib_index_of_C(L* targ, C* src, L sLen, C* val, L vLen){
    L buff[256];
    memset(buff,-1,256*sizeof(L));
    DOI(sLen, if(buff[src[i]]==-1) buff[src[i]]=i)
    DOI(vLen, {L t=buff[val[i]]; targ[i]=-1==t?sLen:t;})
}


void lib_quicksort(L *rtn, V val, L low, L high, B *isUp){
    if(low < high){
        L pos = lib_partition(rtn, val, low, high, isUp);
        lib_quicksort(rtn, val, low, pos-1, isUp);
        lib_quicksort(rtn, val, pos+1, high,isUp);
    }
}

L lib_partition(L *rtn, V val, L low, L high, B *isUp){
    L pivot = low, i = low, j = high+1, t;
    while(true){
        do{++i;} while(lib_quicksort_cmp(val,rtn[i],rtn[pivot],isUp) <=0 && i < high);
        do{--j;} while(lib_quicksort_cmp(val,rtn[j],rtn[pivot],isUp) > 0);
        if(i>=j) break;
        t = rtn[i]; rtn[i] = rtn[j]; rtn[j] = t;
    }
    t = rtn[low]; rtn[low] = rtn[j]; rtn[j] = t;
    R j;
}

#define cmp_switch(x) (x?-1:1):(x?1:-1)

L lib_quicksort_cmp(V val, L a, L b, B *isUp){
    DOI(vn(val), {V t=vV(val,i); B f = isUp[i]; \
        switch(vp(t)){ \
        caseB if(vB(t,a)!=vB(t,b)) R vB(t,a)<vB(t,b)?cmp_switch(f); break; \
        caseH if(vH(t,a)!=vH(t,b)) R vH(t,a)<vH(t,b)?cmp_switch(f); break; \
        caseI if(vI(t,a)!=vI(t,b)) R vI(t,a)<vI(t,b)?cmp_switch(f); break; \
        caseL if(vL(t,a)!=vL(t,b)) R vL(t,a)<vL(t,b)?cmp_switch(f); break; \
        caseF if(vF(t,a)!=vF(t,b)) R vF(t,a)<vF(t,b)?cmp_switch(f); break; \
        caseE if(vE(t,a)!=vE(t,b)) R vE(t,a)<vE(t,b)?cmp_switch(f); break; \
        caseQ if(vQ(t,a)!=vQ(t,b)) R compareSymbol(vQ(t,a),vQ(t,b))<0?cmp_switch(f); break; \
        /* Pending: caseC */ \
        } \
    })
    R 0;
}

void lib_list_order_by(L *targ, L tLen, V val, B *isUp){
    DOI(tLen, targ[i]=i)
    lib_quicksort(targ, val, 0, tLen-1, isUp);
}



































