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
// 	R 0;
// }


// L libIndexOf_I(L *targ, I *src, L src_len, I *val, L val_len){
// 	HN hashT = createHashTable(src, src_len);
// 	targ = (L*)malloc(sizeof(L)*val_len);
// 	DOI(val_len, {L x=hashCode[val[i]]; B z=false;\
// 	             while(!(z=(hashT[x].h_value==x && val[i]==src[hashT[x].h_index])))\
// 	             	{x=(x+1)%val_len;}\
// 	             targ[i]=z?hashT[x].h_index:val_len;})
// 	free(hashT);
// 	R 0;
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
}

void lib_index_of_Q(L* targ, Q* src, L sLen, Q* val, L vLen){
}










































