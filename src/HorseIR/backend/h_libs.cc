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

L libIndexOf_I(L *targ, I *src, L src_len, I *val, L val_len){
	HN hashT = createHashTable(src, src_len);
	targ = (L*)malloc(sizeof(L)*val_len);
	DOI(val_len, {L x=hashCode[val[i]]; B z=false;\
	             while(!(z=(hashT[x].h_value==x && val[i]==src[hashT[x].h_index])))\
	             	{x=(x+1)%val_len;}\
	             targ[i]=z?hashT[x].h_index:val_len;})
	free(hashT);
	R 0;
}

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

L calcHashSize(L x) {
	R (x<16?32:calcSizeExp(x));
}

L calcSizeExp(L x){
	L n = 64;
	while(x > n) { n<<=1; }
	R n;
}

HN createHashTable(I *src, L src_len){
	L hashLen = calcHashSize(src_len);
	HN hashT  = (HN) malloc(sizeof(HN0) * hashLen);
	memset(hashT, -1, sizeof(HN0)*hashLen);
	DOI(src_len, {L x=hashCode(src[i]);while(hashT[x].h_index!=-1){x=(x+1)%hashLen;}\
	    hashT[x].h_value=x; hashT[x].h_index=i;})
	R hashT;
}
