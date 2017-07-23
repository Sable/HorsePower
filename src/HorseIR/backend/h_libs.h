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
void lib_index_of_Q(L* targ, Q* src, L sLen, Q* val, L vLen);

#ifdef	__cplusplus
}
#endif