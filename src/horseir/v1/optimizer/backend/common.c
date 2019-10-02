#include "../global.h"

TC H_TARGET; /* backend/common.h, define the target platform: cpu,  gpu, or others */

V initLiteralDate(L x){
    V z = allocNode();
    initV(z,H_D,1);
    vd(z) = x;
    R z;
}

V initLiteralBool(L x){
    V z = allocNode();
    initV(z,H_B,1);
    vb(z) = x;
    R z;
}

V initLiteralI64(L x){
    V z = allocNode();
    initV(z,H_L,1);
    vl(z) = x;
    R z;
}

V initLiteralString(S s){
    V z = allocNode();
    initV(z,H_S,1);
    S t = allocStrMem(strlen(s));
    strcpy(t,s);
    vs(z) = t;
    R z;
}

V initLiteralSym(S str){
    V z = allocNode();
    initV(z,H_Q,1);
    vq(z) = getSymbol(str);
    R z;
}

V initLiteralF64(E x){
    V z = allocNode();
    initV(z,H_E,1);
    ve(z) = x;
    R z;
}

V initLiteralChar(C x){
    V z = allocNode();
    initV(z,H_C,1);
    vc(z) = x;
    R z;
}

V initLiteralSymVector(L n, S strs[]){
    V z = allocNode();
    initV(z,H_Q,n);
    DOI(n, vQ(z,i)=getSymbol(strs[i]))
    R z;
}

V initLiteralBoolVector(L n, B b[]){
    V z = allocNode();
    initV(z,H_B,n);
    DOI(n, vB(z,i)=b[i])
    R z;
}

V initLiteralI64Vector(L n, L b[]){
    V z = allocNode();
    initV(z,H_L,n);
    DOI(n, vL(z,i)=b[i])
    R z;
}

V initLiteralStrVector(L n, S b[]){
    V z = allocNode();
    initV(z,H_S,n);
    DOI(n, {S t=allocStrMem(strlen(b[i])); strcpy(t,b[i]); vS(z,i)=t;})
    R z;
}

