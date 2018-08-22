#ifndef __H_LIBS__
#define __H_LIBS__

#ifdef	__cplusplus
extern "C" {
#endif

L lib_index_of_B(L* targ, B* src, L sLen, B* val, L vLen);
L lib_index_of_H(L* targ, H* src, L sLen, H* val, L vLen);
L lib_index_of_I(L* targ, I* src, L sLen, I* val, L vLen);
L lib_index_of_L(L* targ, L* src, L sLen, L* val, L vLen);
L lib_index_of_F(L* targ, F* src, L sLen, F* val, L vLen);
L lib_index_of_E(L* targ, E* src, L sLen, E* val, L vLen);
L lib_index_of_C(L* targ, C* src, L sLen, C* val, L vLen);
L lib_index_of_S(L* targ, S* src, L sLen, S* val, L vLen);
L lib_index_of_X(L* targ, X* src, L sLen, X* val, L vLen);
L lib_index_of_G(L* targ, V  src, L sLen, V  val, L vLen);

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
void lib_quicksort          (L *rtn, V val, L low, L high, B *isUp, FUNC_CMP(cmp));
void lib_quicksort_char     (L *rtn, V val, L low, L high, B *isUp, FUNC_CMP(cmp));
void lib_quicksort_other    (L *rtn, V val, L low, L high, B *isUp, FUNC_CMP(cmp));
L    lib_partition          (L *rtn, V val, L low, L high, B *isUp, FUNC_CMP(cmp), B *leftSame);
L    lib_quicksort_cmp      (V val, L a, L b, B *isUp);
L    lib_quicksort_cmp_item (V t, L a, L b, B *isUp);
void lib_order_by_list      (L *targ, V val, B *isUp, L tLen, L colId, FUNC_CMP(cmp));
void lib_quicksort_list     (L *targ, V val, B *isUp, L low, L high, L colId, FUNC_CMP(cmp));
void lib_order_by_vector    (L *targ, V val, B *isUp, L tLen, FUNC_CMP(cmp));
L    lib_get_group_by       (V z, V val, L* index, L iLen, FUNC_CMP(cmp));

L lib_member_B(B* targ, B* src, L sLen, B* val, L vLen);
L lib_member_H(B* targ, H* src, L sLen, H* val, L vLen);
L lib_member_I(B* targ, I* src, L sLen, I* val, L vLen);
L lib_member_L(B* targ, L* src, L sLen, L* val, L vLen);
L lib_member_F(B* targ, F* src, L sLen, F* val, L vLen);
L lib_member_E(B* targ, E* src, L sLen, E* val, L vLen);
L lib_member_C(B* targ, C* src, L sLen, C* val, L vLen);
L lib_member_X(B* targ, X* src, L sLen, X* val, L vLen);
L lib_member_S(B* targ, S* src, L sLen, S* val, L vLen);

#define lib_member_Q lib_member_L
#define lib_member_M lib_member_I
#define lib_member_D lib_member_I
#define lib_member_Z lib_member_L
#define lib_member_U lib_member_I
#define lib_member_W lib_member_I
#define lib_member_T lib_member_I

//void lib_hash_test(L *val, L n);

L lib_join_index_hash(V z0, V z1, V x, V y);

#ifdef	__cplusplus
}
#endif

#endif
