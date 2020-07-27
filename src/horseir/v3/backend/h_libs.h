#ifndef __H_LIBS__
#define __H_LIBS__

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct hash_index_node {
    I ival;
    struct hash_index_node *inext;
}HI0, *HI;

I lib_index_of_B(L* targ, B* src, L sLen, B* val, L vLen);
I lib_index_of_J(L* targ, J* src, L sLen, J* val, L vLen);
I lib_index_of_H(L* targ, H* src, L sLen, H* val, L vLen);
I lib_index_of_I(L* targ, I* src, L sLen, I* val, L vLen);
I lib_index_of_L(L* targ, L* src, L sLen, L* val, L vLen);
I lib_index_of_F(L* targ, F* src, L sLen, F* val, L vLen);
I lib_index_of_E(L* targ, E* src, L sLen, E* val, L vLen);
I lib_index_of_C(L* targ, C* src, L sLen, C* val, L vLen);
I lib_index_of_S(L* targ, S* src, L sLen, S* val, L vLen);
I lib_index_of_X(L* targ, X* src, L sLen, X* val, L vLen);
I lib_index_of_G(L* targ, V  src, L sLen, V  val, L vLen);

#define lib_index_of_Q lib_index_of_L
#define lib_index_of_M lib_index_of_I
#define lib_index_of_D lib_index_of_I
#define lib_index_of_Z lib_index_of_L
#define lib_index_of_U lib_index_of_I
#define lib_index_of_W lib_index_of_I
#define lib_index_of_T lib_index_of_I

/* quick sort section */
#define FUNC_QCMP(x) I (*x)(const void*, const void*)

#define FUNC_CMP(x) L (*x)(V,L,L,B*)
void lib_quicksort            (L *rtn, V val, L low, L high, B *isUp, FUNC_CMP(cmp));
void lib_quicksort_char       (L *rtn, V val, L low, L high, B *isUp, FUNC_CMP(cmp));
void lib_quicksort_other      (L *rtn, V val, L low, L high, B *isUp, FUNC_CMP(cmp));
L   lib_quicksort_cmp        (V val, L a, L b, B *isUp);
L   lib_quicksort_cmp_item   (V t  , L a, L b, B *isUp);
void lib_order_by_list        (L *targ, V val, B *isUp, L tLen, L colId, FUNC_CMP(cmp));
void lib_quicksort_list       (L *targ, V val, B *isUp, L low , L high, L colId, FUNC_CMP(cmp));
void lib_order_by_vector      (L *targ, V val, B *isUp, L tLen, FUNC_CMP(cmp));
I    lib_get_group_by         (V z, V val, L* index, L iLen, FUNC_CMP(cmp));
I    lib_group_by_flat        (V z, V x);
I    lib_group_by_normal_int  (V z, V x);
I    lib_group_by_normal_long (V z, V x);
I lib_group_by_normal_par_long(V z, V x);
void lib_radixsort_int        (L *rtn, V val, L len, B *isUp, B isRtnIndex);
void lib_radixsort_long       (L *rtn, V val, L len, B *isUp, B isRtnIndex);
void lib_radixsort_long_v0    (L *rtn_val, L *rtn_pos, V x);

I lib_member_B(B* targ, B* src, L sLen, B* val, L vLen);
I lib_member_H(B* targ, H* src, L sLen, H* val, L vLen);
I lib_member_I(B* targ, I* src, L sLen, I* val, L vLen);
I lib_member_L(B* targ, L* src, L sLen, L* val, L vLen);
I lib_member_F(B* targ, F* src, L sLen, F* val, L vLen);
I lib_member_E(B* targ, E* src, L sLen, E* val, L vLen);
I lib_member_C(B* targ, C* src, L sLen, C* val, L vLen);
I lib_member_X(B* targ, X* src, L sLen, X* val, L vLen);
I lib_member_S(B* targ, S* src, L sLen, S* val, L vLen);

#define lib_member_Q lib_member_L
#define lib_member_M lib_member_I
#define lib_member_D lib_member_I
#define lib_member_Z lib_member_L
#define lib_member_U lib_member_I
#define lib_member_W lib_member_I
#define lib_member_T lib_member_I

//void lib_hash_test(L *val, L n);

//extern L debug_hash_total1;
//extern L debug_hash_total2;
//extern L debug_hash_find1;
//extern L debug_hash_find2;
//extern L debug_hash_max1;
//extern L debug_hash_max2;
//extern L debug_hash_count1;
//extern L debug_hash_count2;
//extern L debug_hash_need_search1;
//extern L debug_hash_need_search2;
//extern L debug_hash_op_total1;
//extern L debug_hash_op_total2;

/* ------------------ Common Joins ------------------ */

I lib_join_index_hash     (V z0, V z1, V x, V y, B isEq);
I lib_join_index_linear   (V z0, V z1, V x, V y, B isEq);
I lib_join_index_compare  (V z0, V z1, V x, V y, I op  );
I lib_join_index_hash_many(V z0, V z1, V x, V y, V f   );
I lib_join_index_basic    (V z0, V z1, V x, V y, B isEq);
I lib_join_dummy          (V z0, V z1, V x, V y);
I lib_join_dummy2         (V x, V y);
I lib_join_dummy3         (V x, V y);

void profileListR         (V x);
B    checkOrderCase1      (V z, V x, V f);
void lib_groupby_dummy    (V x);

L getInteger1             (V x);
L genRand                 (L x);

#ifdef	__cplusplus
}
#endif

#endif
