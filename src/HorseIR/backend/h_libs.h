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
#define lib_index_of_Q lib_index_of_L

void lib_order_B(L* targ, B* val, L vLen, B isUp);
void lib_order_H(L* targ, H* val, L vLen, B isUp);
void lib_order_I(L* targ, I* val, L vLen, B isUp);
void lib_order_L(L* targ, L* val, L vLen, B isUp);
void lib_order_F(L* targ, F* val, L vLen, B isUp);
void lib_order_E(L* targ, E* val, L vLen, B isUp);

#ifdef	__cplusplus
}
#endif