#include "h_global.h"

const I HASH_A = 1223106847;
const I HASH_M = (1LL << 32)-5;

typedef struct hash_node {
    I h_index;
    union {
        /* no boolean or char */
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

#define hH(x) x->h_h
#define hI(x) x->h_i
#define hL(x) x->h_l
#define hF(x) x->h_f
#define hE(x) x->h_e
#define hX(x) x->h_x
#define hS(x) x->h_s

#define hD(x) x->h_index
#define hN(x) x->next
#define hV(x,k) ((x)+(k))

#define hash_B /* empty */
#define hash_H hash_i32
#define hash_I hash_i32
#define hash_L hash_i64
#define hash_F hash_f32
#define hash_E hash_f64
#define hash_X hash_clex
#define hash_S hash_djb2
#define hash_S_n hash_djb2_n
#define init_H(x) {hI(x)=-1;hN(x)=NULL;}

#define toB(v) (*(B*)(v))
#define toH(v) (*(H*)(v))
#define toI(v) (*(I*)(v))
#define toL(v) (*(L*)(v))
#define toF(v) (*(F*)(v))
#define toE(v) (*(E*)(v))
#define toX(v) (*(X*)(v))
#define toS(v) (*(S*)(v))

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

UI getHashValue(void* val, L typ){
    switch(typ){
        caseH R hash_H(toH(val));
        caseI R hash_I(toI(val));
        caseL R hash_L(toL(val));
        caseF R hash_F(toF(val));
        caseE R hash_E(toE(val));
        caseX R hash_X(toX(val));
        caseS R hash_S(toS(val));
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

/* further, consider search by list tuples */
L find_or_insert_hash(HN ht, L htSize, void* val, L index, L typ, B op){
    L hashKey = getHashValue(val,typ) % htSize;
    HN t = hV(ht,hashKey);
    while(hN(t)) {
        t = hN(t);
        switch(typ){
            caseH if(hH(t)==toH(val)) R hD(t); break;
            caseI if(hI(t)==toI(val)) R hD(t); break;
            caseL if(hL(t)==toL(val)) R hD(t); break;
            caseF if(hF(t)==toF(val)) R hD(t); break;
            caseE if(hE(t)==toE(val)) R hD(t); break;
            caseX if(xEqual(hX(t),toX(val)))  R hD(t); break;
            caseS if(!strcmp(hS(t),toS(val))) R hD(t); break;
        }
    }
    if(op){ /* insert */
        HN x = newHashNode();
        hD(x) = index;
        switch(typ){
            caseH hH(x)=toH(val); break;
            caseI hI(x)=toI(val); break;
            caseL hL(x)=toL(val); break;
            caseF hF(x)=toF(val); break;
            caseE hE(x)=toE(val); break;
            caseX hX(x)=toX(val); break;
            caseS hS(x)=toS(val); break;
        }
        hN(t) = x;
        R index;
    }
    R -1; /* not found */
}

L findHash(HN ht, L htSize, void* val, L typ){
    R find_or_insert_hash(ht,htSize,val,-1,typ,0);
}

L insertHash(HN ht, L htSize, void* val, L index, L typ){
    R find_or_insert_hash(ht,htSize,val,index,typ,1);
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

#define lib_index_template(typ) \
    HN hashT; \
    L hashLen = getHashTableSize(sLen); \
    CHECKE(createHash(&hashT,hashLen)); \
    DOI(sLen, insertHash(hashT,hashLen,&src[i],i,typ)) \
    DOP(vLen, {L t=findHash(hashT,hashLen,&val[i],typ);targ[i]=t<0?sLen:t;})

L lib_index_of_B(L* targ, B* src, L sLen, B* val, L vLen){
    L flag[2]={-1}; I c=0;
    DOI(sLen, if(c<2 && -1==flag[src[i]]){flag[src[i]]=i;c++;if(2==c)break;})
    DOP(vLen, {L t=flag[val[i]]; targ[i]=t<0?sLen:t;})
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
    DOI(vn(val), {V t=vV(val,i); B f = isUp?isUp[i]:1; \
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


L lib_get_group_by(V z, V val, L* index, L iLen){
    L k, c; V t;
    /* 1. get the total number of cells: lenZ */
    L lenZ=iLen>0?1:0;
    DOIa(iLen, if(0!=lib_quicksort_cmp(val,index[i-1],index[i],NULL))lenZ++)
    /* 2. allocate list and get the info of each cell */
    initV(z, H_G, lenZ);
    c=iLen>0?1:0;
    k=0;
    t=vV(z,k++);
    DOIa(iLen, if(0!=lib_quicksort_cmp(val,index[i-1],index[i],NULL)){ \
                 initV(t,H_L,c); t=vV(z,k++); c=1; } \
               else c++;)
    if(c>0) initV(t,H_L,c);
    /* 3. fill indices into each cell */
    k=0, c=0;
    t= vV(z,k++);
    if(iLen>0) vL(t,c++)=index[0];
    DOIa(iLen, if(0!=lib_quicksort_cmp(val,index[i-1],index[i],NULL)){ \
                  t=vV(z,k++); vL(t,0)=index[i]; c=1; } \
               else vL(t,c++)=index[i])
    R 0;
}

#define lib_member_template(typ)\
    HN hashT; \
    L hashLen = getHashTableSize(sLen); \
    CHECKE(createHash(&hashT,hashLen)); \
    DOI(sLen, insertHash(hashT,hashLen,&src[i],i,typ)) \
    DOI(vLen, targ[i]=(0<=findHash(hashT,hashLen,&val[i],typ))) \
    profileHash(hashT,hashLen);

L lib_member_B(B* targ, B* src, L sLen, B* val, L vLen){
    B flag[2]={0};
    DOI(sLen, flag[src[i]]=1)
    DOI(vLen, targ[i]=flag[val[i]])
    R 0;
}

L lib_member_H(B* targ, H* src, L sLen, H* val, L vLen){
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
    DOI(vLen, targ[i]=buff[val[i]];)
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































