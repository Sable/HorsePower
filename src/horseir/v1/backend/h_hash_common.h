static const LL setT = setT_CONSTANT; // gcc-5 fails to know setT is a constant
static const LL setN = setT_CONSTANT - 1;

//UI hash_i32         (I a);
//UL hash_murmur3_i32 (I a); // default: return 64-bit
//UL hash_murmur3_i64 (L a);
//UI hash_djb2        (S str);
//UI hash_djb2_n      (S str, L n);
//UI hash_sdbm        (S str, L n);
//UI hash_lose        (S str, L n);
//UI hash_f32         (F n);
//UI hash_f64         (E n);
//UI hash_clex        (X n);

/*
 * quick hash functions
 * src1: https://burtleburtle.net/bob/hash/integer.html
 * src2: https://gist.github.com/badboy/6267743
 */
static UI hash_i32(I a){
//static UL hash_i32(I a){ // default: return 64-bit
    a = (a ^ 61) ^ (a >> 16);
    a = a + (a << 3);
    a = a ^ (a >> 4);
    a = a * 0x27d4eb2d;
    a = a ^ (a >> 15);
    return (L)a;
}

#define ROTL32(x, r) ((x<<r)|(x>>(32-r)))
//static UI hash_murmur3_i32(I a){
static UL hash_murmur3_i32(I a){ // default: return 64-bit
    UI h1 = 16807;
    UI c1 = 0xcc9e2d51;
    UI c2 = 0x1b873593;
    a *= c1;
    a = ROTL32(a, 15);
    a *= c2;
    h1 ^= a;
    h1 = ROTL32(h1,13);
    h1 = h1*5+0xe6546b64;
    R h1;
}

static UL hash_murmur3_i64(L a){
    I a_low = a&0xFFFFFFFF;
    if(a_low != a){ EP("need to hash a_high as well: %d, %lld\n",a_low,a); }
    R hash_murmur3_i32(a_low);
}

static UI hash_djb2(S str){
    UI h = 5381; I c;
    while(0!=(c=*str++))
        h=((h<<5)+h)+(*str++);
    R h;
}

static UI hash_djb2_n(S str, L n){
    UI h = 5381;
    DOI(n, h=((h<<5)+h)+(*str++)) /* h * 33 + c */
    R h;
}

static UI hash_sdbm(S str, L n){
    UI h = 0;
    DOI(n, h=(*str++)+(h<<6)+(h<<6)-h)
    return h;
}

static UI hash_lose(S str, L n){
    UI h = 0;
    DOI(n, h+=(*str++))
    R h;
}

static UI hash_f32(F n){
    R hash_S_n((S)(&n),sizeof(F));
}

static UI hash_f64(E n){
    R hash_S_n((S)(&n),sizeof(E));
}

static UI hash_clex(X n){
    R hash_S_n((S)(&n),sizeof(X));
}

static UI hash_list(V x, L k){
    UI h = 0;
    DOI(xn, {V x0=vV(x,i); \
        switch(vp(x0)){ \
        caseI h+=hash_I(vI(x0,k)); break; \
        caseL h+=hash_L(vL(x0,k)); break; \
        caseF h+=hash_F(vF(x0,k)); break; \
        caseE h+=hash_E(vE(x0,k)); break; \
        default: EP("type not supported: %s\n", getTypeName(vp(x0))); \
        }})
    R h;
}

// x[k] == y[k]
static B isAllMatch(L n, V *x, L k, LL *y){
    DOI(n, if(vLL(x[i],k)!=y[i]) R 0) R 1;
}

// first is I
static B isAllMatchBasic(L n, V p, L k0, V q, L k1){
    if(k1==13){
        DOIa(n, {V x=p+i; V y=q+i; \
                switch(xp){ \
                caseI P("vI(x,%lld)=%d, vI(y,%lld)=%d\n",k0,vI(x,k0),k1,vI(y,k1)); getchar(); break; \
                default: EP("type not suported: %s\n", getTypeName(xp));}})
    }
    DOIa(n, {V x=p+i; V y=q+i; \
            switch(xp){ \
            caseI if(vI(x,k0)!=vI(y,k1)) R 0; break; \
            caseE if(vE(x,k0)!=vE(y,k1)) R 0; break; \
            default: EP("type not suported: %s\n", getTypeName(xp));}})
    R 1;
}
static B isAllMatchList(V x, V y, L kx, L ky){
    DOI(xn, {V x0=vV(x,i); V y0=vV(y,i);\
        switch(vp(x0)){\
        caseI if(vI(x0,kx)!=vI(y0,ky)) R false; break; \
        caseL if(vL(x0,kx)!=vL(y0,ky)) R false; break; \
        caseF if(vF(x0,kx)!=vF(y0,ky)) R false; break; \
        caseE if(vE(x0,kx)!=vE(y0,ky)) R false; break; \
        default: EP("type not supported: %s\n", getTypeName(vp(x0)));\
        }})
    R true;
}

static UI getHashTableSize(L x){
    if(x < 16) R 16;
    L n=16; while(n>0 && n<=x){n<<=1;}
    R n<<1;
}


