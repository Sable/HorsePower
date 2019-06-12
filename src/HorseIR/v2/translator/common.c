#include "../global.h"

I getHType(Type t){
    switch(t){
        case   boolT: R H_B;
        case     i8T: R H_J;
        case    i16T: R H_H;
        case    i32T: R H_I;
        case    i64T: R H_L;
        case    f32T: R H_F;
        case    f64T: R H_E;
        case   clexT: R H_X;
        case   charT: R H_C;
        case    symT: R H_Q;
        case    strT: R H_S;
        case   dateT: R H_D;
        case  monthT: R H_M;
        case     dtT: R H_Z;
        case   timeT: R H_T;
        case minuteT: R H_U;
        case secondT: R H_W;
        default: EP("Type not supported: %d\n", t);
    }
}

static O loadInt(I v, V x, L k, I t){
    switch(t){
        caseB xB(k) = v!=0; break;
        caseJ xJ(k) = (J)v; break;
        caseH xH(k) = (H)v; break;
        caseI xI(k) = (I)v; break;
        caseF xF(k) = (F)v; break;
        caseE xE(k) = (E)v; break;
        caseD xD(k) =    v; break;
        caseM xM(k) =    v; break;
        caseT xT(k) =    v; break;
        caseU xU(k) =    v; break;
        caseW xW(k) =    v; break;
        default: EP("Type not supported: %d\n", t);
    }
}

static O loadFloat(E v, V x, L k, I t){
    switch(t){
        caseF xF(k) = (F)v; break;
        caseE xE(k) = (E)v; break;
        default: EP("Type not supported: %d\n", t);
    }
}

static O loadLong(L v, V x, L k, I t){
    switch(t){
        caseB xB(k) = v!=0; break;
        caseJ xJ(k) = (J)v; break;
        caseH xH(k) = (H)v; break;
        caseI xI(k) = (I)v; break;
        caseL xL(k) =    v; break;
        caseF xF(k) = (F)v; break;
        caseE xE(k) = (E)v; break;
        caseZ xZ(k) =    v; break;
        default: EP("Type not supported: %d\n", t);
    }
}

static O loadString(S v, V x, L k, I t){
    switch(t){
        caseQ xQ(k) = getSymbol(v); break;
        caseS {S s=allocStrMem(strlen(v)); strcpy(s,v); xS(k)=s;} break;
        caseC xC(k) = v[0]; break;
        default: EP("Type not supported: %d\n", t);
    }
}

// similar to b/h_io.c:loadItem
O loadConst(Node *n, V x, L k, I t){
    ConstValue *p = n->val.nodeC;
    switch(p->type){
        case    intC: loadInt   (p->valI,x,k,t); break;
        case  floatC: loadFloat (p->valF,x,k,t); break;
        case   longC: loadLong  (p->valL,x,k,t); break;
        case   clexC: TODO("Support more const\n"); 
        case   charC:
        case    symC: 
        case    strC: loadString(p->valS,x,k,t); break;
        case  monthC: 
        case minuteC: 
        case secondC: 
        case   timeC: 
        case   dateC: loadInt   (p->valI,x,k,t); break;
        case     dtC: loadLong  (p->valF,x,k,t); break;
        default: EP("Constant type not supported: %d\n", p->type);
    }
}

