#pragma once

#ifdef	__cplusplus
extern "C" {
#endif

L libIndexOf_I(L *targ, I *src, L src_len, I *val, L val_len);
L libIndexOf_L(L *targ, L *src, L src_len, L *val, L val_len);
L libIndexOf_F(L *targ, F *src, L src_len, F *val, L val_len);
L libIndexOf_E(L *targ, E *src, L src_len, E *val, L val_len);

#ifdef	__cplusplus
}
#endif