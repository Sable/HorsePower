#ifndef __H_HASH__MINE__
#define __H_HASH__MINE__

#ifdef	__cplusplus
extern "C" {
#endif

/* determine which hash to implement */
//#define DO_HASH_V0 "v0"
//#define DO_HASH_V1 "v1"
//#define DO_HASH_V2 "v2"
#define DO_HASH_V3 "v3"

#define setT_CONSTANT 128 //512 //512, 1024, 2048, 4096
#define PRE_SIZE 8  // for hash join v2

#define hash_B /* empty */
#define hash_J hash_i32
#define hash_H hash_i32
#define hash_I hash_murmur3_i32 //hash_murmur3_i32 or hash_i32
#define hash_L hash_murmur3_i64 //hash_i64
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

#define TIME_INIT()  struct timeval tv0, tv1
#define TIME_BEGIN() gettimeofday(&tv0, NULL)
#define TIME_END()   gettimeofday(&tv1, NULL)
#define TIME_SHOW(t) PP(t " %g ms\n", calcInterval(tv0,tv1))

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
#define bV(x,k) hV(x,k)

#define readArgs() \
    LL args[99]; va_list vlist; \
    va_start(vlist, n); DOI(n, args[i]=va_arg(vlist, LL)); va_end(vlist);

#define newHashNodeN(n)     HASH_AL(HN0, n)
#define createHashBucket(n) HASH_AL(HB0, n)

/* implementations: h_hash_v0/1/2.c */

/***** hash v0 *****/
#ifdef DO_HASH_V0
typedef struct hash_node_v0 {
    L   h_index;
    HI  h_index_other;
    union { /* no boolean or char */
        J h_j;
        H h_h;
        I h_i;
        L h_l;
        F h_f;
        E h_e;
        X h_x;
        S h_s;
    };
    struct hash_node_v0 *next;
}HN0,*HN;

typedef struct TupleHash{
    HN *hashTable;
    L  *hashSize;
    L  numTable;
}THash;

#define find_hash_value_single   findValueFromHash
#define find_hash_value_compound findValueFromHash
#define create_hash_multiply     createHashSingle   //createHashWithV

HN createHashWithV(L n, V *x);    /* single partition */
HN findValueFromHash(HN hashT, V *src, L hashLen, L n, ...);
#endif

/***** hash v1 *****/
#ifdef DO_HASH_V1
typedef struct hash_node_v1 {
    L   h_index;
    L   h_num;   /* number of duplicated items */
    L   h_other; /* if !h_num, then h_other != NULL */
    union { /* no boolean or char */
        J h_j;
        H h_h;
        I h_i;
        L h_l;
        F h_f;
        E h_e;
        L h_hash;  /* hash value */
       HI h_index_other;
    };
    struct hash_node_v1 *next;
}HN0,*HN;

typedef struct hash_bucket_v1 {
    L size, cur;
    HN node;
}HB0,*HB;

typedef struct TupleBucket{
    HB *hashBucket;
    L   *hashSize;
    L   numBucket;
}TBucket;

#define find_hash_value_single   find_hash_value_single_v1
#define find_hash_value_compound findValueFromHashV1
#define create_hash_multiply     create_hash_multiply_v1

TBucket createHashWithV1(L n, V *x, L *hashLen); 
HN      find_hash_value_single_v1(HB ht, L hashLen, LL val);
HN      findValueFromHashV1(HB* hashT, V *src, L *hashSize, L n, ...);
TBucket create_hash_multiply_v1(V x);
#endif

/***** hash v2 *****/
#ifdef DO_HASH_V2
typedef struct hash_node_v2 {
    L   h_index;
    L   h_num;   /* number of duplicated items */
    L   h_other; /* if !h_num, then h_other != NULL */
    union { /* no boolean or char */
        J h_j;
        H h_h;
        I h_i;
        L h_l;
        F h_f;
        E h_e;
        L h_hash;  /* hash value */
       HI h_index_other;
    };
    struct hash_node_v2 *next;
}HN0,*HN;

typedef struct hash_bucket_v2 {
    L size, cur;
    HN0 nd0[4]; 
    HN node;
}HB0,*HB;

typedef struct TupleBucket{
    HB *hashBucket;
    L   *hashSize;
    L   numBucket;
}TBucket;

#define find_hash_value_single   find_hash_value_single_v2
#define find_hash_value_compound findValueFromHashV2
#define create_hash_multiply     create_hash_multiply_v2

TBucket createHashWithV2(L n, V *x, L *hashLen);  /* multiple partitions */
HN      find_hash_value_single_v2(HB ht, L hashLen, LL val);
HN      findValueFromHashV2(HB* hashT, V *src, L *hashSize, L n, ...);
TBucket create_hash_multiply_v2(V x);

I exp_join_q5_v2_c1(V z, V *x);
I exp_join_q5_v2_c2(V x);
#endif

/***** hash v3 *****/
#ifdef DO_HASH_V3
#define PRE_SIZE_CELL 32 // q5: 5 , q7: 64
typedef struct hash_node_v3 { // 8*3+8=32
    L h_index;
    L h_num;   /* number of duplicated items */
    L h_other; /* if !h_num, then h_other != NULL */
    union { /* no boolean or char */
        J h_j;
        H h_h;
        I h_i;
        L h_l;
        F h_f;
        E h_e;
        I h_v; // hash value
       HI h_index_other; /* never used */
       struct hash_node_v3 *next;
    };
}HN0,*HN;

// 8+
typedef struct hash_cell { // 8+3*32+8 = 256 (4 cache lines)
    L c_total;
    HN0 c_node[PRE_SIZE_CELL];
    struct hash_cell *c_next;
}HC0,*HC;

typedef struct TupleBucket{
    HC **hashCell;
    L   *hashMask;
}TB;

#define find_hash_value_single   find_hash_value_single_v3
#define find_hash_value_compound find_hash_value_multiple_v3 //findValueFromHashV3
#define create_hash_multiply     create_hash_multiply_v3

TB create_hash_multiply_v3     (V x);
HN find_hash_value_single_v3   (HC *ht, L hashLen, LL val);
HN find_hash_value_multiple_v3 (HC *ht, V *src, L hashLen, L n, LL *args);
HN findValueFromHashV3         (HC **hashT, V *src, L *hashMask, L n, ...);

void profile_hash_join_v3      (HC **ht, L *hashMask);
I    lib_join_index_basic_v3   (V z0, V z1, V x, V y, B isEq);
#endif

/* methods: h_lib.c */

#define HORSE_ARRAY_SIZE 2048
typedef struct HorseArray{
    L size, data[HORSE_ARRAY_SIZE];
    struct HorseArray *next;
}HA0,*HA;

#ifdef DO_HASH_V0
void profileHashTable(THash th);
#endif

#ifdef DO_HASH_V1
void profileHashBucket(TBucket tb);
#endif

#ifdef	__cplusplus
}
#endif

#endif
