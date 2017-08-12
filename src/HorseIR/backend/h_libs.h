#pragma once

#ifdef	__cplusplus
extern "C" {
#endif

void lib_index_of_B(L* targ, B* src, L sLen, B* val, L vLen);
void lib_index_of_H(L* targ, H* src, L sLen, H* val, L vLen);
void lib_index_of_I(L* targ, I* src, L sLen, I* val, L vLen);
void lib_index_of_L(L* targ, L* src, L sLen, L* val, L vLen);
void lib_index_of_F(L* targ, F* src, L sLen, F* val, L vLen);
void lib_index_of_E(L* targ, E* src, L sLen, E* val, L vLen);
void lib_index_of_C(L* targ, C* src, L sLen, C* val, L vLen);
#define lib_index_of_Q lib_index_of_I

/* quick sort section */
void lib_quicksort     (L *rtn, V val, L low, L high, B *isUp);
L    lib_partition     (L *rtn, V val, L low, L high, B *isUp);
L    lib_quicksort_cmp (V val, L a, L b, B *isUp);
void lib_list_order_by (L *targ, L tLen, V val, B *isUp);

void lib_member_B(B* targ, B* src, L sLen, B* val, L vLen);
void lib_member_H(B* targ, H* src, L sLen, H* val, L vLen);
void lib_member_I(B* targ, I* src, L sLen, I* val, L vLen);
void lib_member_L(B* targ, L* src, L sLen, L* val, L vLen);
void lib_member_F(B* targ, F* src, L sLen, F* val, L vLen);
void lib_member_E(B* targ, E* src, L sLen, E* val, L vLen);
void lib_member_C(B* targ, C* src, L sLen, C* val, L vLen);
void lib_member_X(B* targ, X* src, L sLen, X* val, L vLen);
void lib_member_Q(B* targ, Q* src, L sLen, Q* val, L vLen);

#define lib_member_M lib_member_I
#define lib_member_D lib_member_I
#define lib_member_Z lib_member_L
#define lib_member_U lib_member_I
#define lib_member_W lib_member_I
#define lib_member_T lib_member_I

#ifdef	__cplusplus
}
#endif