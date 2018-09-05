#include "../global.h"
#include <math.h>
#include <float.h>
#include <limits.h>

/*
 * primitive function -> pfn
 * input  parameters  -> x, y, m ...
 * return parameters  -> z
 * return value       ->  0 (normal)
 *                    -> >0 (error)
 * opt. idea
 *    in-lining + CSE
 */

#define inferLog inferRecip
#define inferTrig inferRecip
#define inferPowerLog inferRecip

L pfnLoadTable(V z, V x){
    if(isSymbol(x)){
        initTableByName(getSymbolStr(vq(x))); // register table
        V t = findTableByName(vq(x));
        if(!t) R E_TABLE_NOT_FOUND;
        *z = *t;
        R 0;
    }
    else R E_DOMAIN;
}

/* copy alias */
L pfnList(V z, L n, V x[]){
    initList(z, n);
    DOI(n, *vV(z,i)=*x[i])
    R 0;
}


/*
 * indexing: x[y]
 */
L pfnIndex(V z, V x, V y){
    if(H_DEBUG) P("-> Entering index\n");
    // P("info x y: (%lld, %lld) (%lld, %lld)\n",vp(x),vn(x),vp(y),vn(y));
    if(isTypeGroupInt(vp(y))){
        L typZ = vp(x), lenZ = vn(y), lenX = vn(x);
        V tempY = allocNode();
        CHECKE(promoteValue(tempY, y, H_L));
        DOI(lenZ, if(lenX <= vL(tempY,i))R E_INDEX)
        if(isTypeGroupBasic(vp(x))){
            initV(z, typZ, lenZ);
            #define INDEX_BASIC(p) case##p DOP(lenZ, v##p(z,i)=v##p(x,vL(y,i))) break
            switch(vp(x)){
                INDEX_BASIC(B);
                INDEX_BASIC(J);
                INDEX_BASIC(H);
                INDEX_BASIC(I);
                INDEX_BASIC(L);
                INDEX_BASIC(F);
                INDEX_BASIC(E);
                INDEX_BASIC(X);
                INDEX_BASIC(Q);
                INDEX_BASIC(S);
                INDEX_BASIC(C);
                INDEX_BASIC(M);
                INDEX_BASIC(D);
                INDEX_BASIC(Z);
                INDEX_BASIC(U);
                INDEX_BASIC(W);
                INDEX_BASIC(T);
                default: R E_NOT_IMPL; /* date time */
            }
            R 0;
        }
        else if(isList(x)){
            if(isOneInt(y)){
                L k = 0;
                switch(vp(y)){
                    caseB k = vb(y); break;
                    caseJ k = vj(y); break;
                    caseH k = vh(y); break;
                    caseI k = vi(y); break;
                    caseL k = vl(y); break;
                }
                CHECKE(copyV(z, vV(x,k)));
                R 0;
            }
            else R E_NOT_IMPL;
        }
        else R E_NOT_IMPL;
    }
    else R E_DOMAIN;
}

/*
 * indexing assignment: x[y] = m
 * no return
 */
L pfnIndexA(V x, V y, V m){
    if(isTypeGroupInt(vp(y))){
        if(isAssignableType(vp(x),vp(m))){
            if(isTypeGroupBasic(vp(x))){
                if(isEqualLength(y,m) || isOne(m)){
                    V tempY = allocNode();
                    V tempM = allocNode();
                    CHECKE(promoteValue(tempY,y,H_L));
                    CHECKE(promoteValue(tempM,m,vp(x)));
                    if(isEqualLength(y,m)){
                        switch(vp(x)){
                            caseB DOI(vn(y), vB(x,vL(tempY,i))=vB(tempM,i)) break;
                            caseJ DOI(vn(y), vJ(x,vL(tempY,i))=vJ(tempM,i)) break;
                            caseH DOI(vn(y), vH(x,vL(tempY,i))=vH(tempM,i)) break;
                            caseI DOI(vn(y), vI(x,vL(tempY,i))=vI(tempM,i)) break;
                            caseL DOI(vn(y), vL(x,vL(tempY,i))=vL(tempM,i)) break;
                            caseF DOI(vn(y), vF(x,vL(tempY,i))=vF(tempM,i)) break;
                            caseE DOI(vn(y), vE(x,vL(tempY,i))=vE(tempM,i)) break;
                            caseX DOI(vn(y), vX(x,vL(tempY,i))=vX(tempM,i)) break;
                            caseQ DOI(vn(y), vQ(x,vL(tempY,i))=vQ(tempM,i)) break;
                            caseS DOI(vn(y), vS(x,vL(tempY,i))=vS(tempM,i)) break;
                            default: R E_NOT_IMPL; /* date time */
                        }
                    }
                    else{
                        switch(vp(x)){
                            caseB DOI(vn(y), vB(x,vL(tempY,i))=vB(tempM,0)) break;
                            caseJ DOI(vn(y), vJ(x,vL(tempY,i))=vJ(tempM,0)) break;
                            caseH DOI(vn(y), vH(x,vL(tempY,i))=vH(tempM,0)) break;
                            caseI DOI(vn(y), vI(x,vL(tempY,i))=vI(tempM,0)) break;
                            caseL DOI(vn(y), vL(x,vL(tempY,i))=vL(tempM,0)) break;
                            caseF DOI(vn(y), vF(x,vL(tempY,i))=vF(tempM,0)) break;
                            caseE DOI(vn(y), vE(x,vL(tempY,i))=vE(tempM,0)) break;
                            caseX DOI(vn(y), vX(x,vL(tempY,i))=vX(tempM,0)) break;
                            caseQ DOI(vn(y), vQ(x,vL(tempY,i))=vQ(tempM,0)) break;
                            caseS DOI(vn(y), vS(x,vL(tempY,i))=vS(tempM,0)) break;
                            default: R E_NOT_IMPL; /* date time */
                        }
                    }
                    R 0;
                }
                else R E_LENGTH;
            }
            else R E_NOT_IMPL;
        }
        else R E_TYPE;
    }
    else R E_DOMAIN;
}

/* copy alias */
// L pfnDict(V z, V x, V y){
//     initDict(z);
//     V key = getDictKey(z);
//     V val = getDictVal(z);
//     *key = *x;
//     *val = *y;
//     R 0;
// }

/* copy alias */
L pfnFlip(V z, V x){
    if(isDict(x)){
        initTable(z, vn(x));
        /* need to check dict */
        DOP(vn(x), *(sV(z)+i)=*(sV(x)+i))
    }
    else if(isList(x)){
        L cellSize = vn(x)>0?vn(vV(x,0)):0;
        DOI(vn(x), if(cellSize != vn(vV(x,i))) R E_DOMAIN)
        initList(z, cellSize);
        DOJ(cellSize, {V y=vV(z,j); initV(y,H_L,vn(x));  DOP(vn(x), { vL(y,i)=vL(vV(x,i),j); })})
    }
    else R E_DOMAIN;
    R 0;
}

L pfnMatch(V z, V x, V y){
    initV(z,H_B,1);
    B t; CHECKE(matchPair(&t,x,y)); vb(z)=t;
    R 0;
}

L pfnMeta(V z, V x){
    R 0;
}

L pfnKeys(V z, V x){
    if(isEnum(x)){
        R copyV(z, (V)getEnumTarget(x));
    }
    else if(isTable(x) || isDict(x)){
        L lenZ = vn(x);
        R copyV(z, getTableKeys(x));
    }
    else if(isKTable(x)){
        R copyV(z, getKTableKey(x));
    }
    else R E_DOMAIN;
}

L pfnValues(V z, V x){
    if(isTable2(x)){
        CHECKE(getColumnValue(z,x));
    }
    else if(isDict(x)){
        CHECKE(getDictValue(z,x));
    }
    else if(isEnum(x)){
        CHECKE(getEnumValue(z,x));
    }
    else R E_DOMAIN;
    R 0;
}

L pfnFetch(V z, V x){
    P("typ = %lld\n", xp);
    if(isEnum(x)){
        V targ = (V)getEnumTarget(x);
        L typZ = vp(targ);
        L lenZ = vn(x);
        initV(z, typZ, lenZ);
        // P("1."); DOI(20, P(" %lld",vY(x,i)))           P("\n");
        // P("2."); DOI(20, P(" %lld",vL(targ, vY(x,i)))) P("\n"); getchar();
        switch(typZ){
            caseB DOP(lenZ, vB(z,i)=vB(targ,vY(x,i))) break;
            caseJ DOP(lenZ, vJ(z,i)=vJ(targ,vY(x,i))) break;
            caseH DOP(lenZ, vH(z,i)=vH(targ,vY(x,i))) break;
            caseI DOP(lenZ, vI(z,i)=vI(targ,vY(x,i))) break;
            caseL DOP(lenZ, vL(z,i)=vL(targ,vY(x,i))) break;
            caseF DOP(lenZ, vF(z,i)=vF(targ,vY(x,i))) break;
            caseE DOP(lenZ, vE(z,i)=vE(targ,vY(x,i))) break;
            caseX DOP(lenZ, vX(z,i)=vX(targ,vY(x,i))) break;
            caseS DOP(lenZ, vS(z,i)=vS(targ,vY(x,i))) break;
            caseQ DOP(lenZ, vQ(z,i)=vQ(targ,vY(x,i))) break;
            caseC DOP(lenZ, vC(z,i)=vC(targ,vY(x,i))) break;
            default: R E_NOT_IMPL;
        }
        R 0;
    }
    else R E_DOMAIN;
}


V fetchTableByName(V x){
    if(isSymbol(x) && isOne(x)){
        R findTableByName(vq(x));
    }
    R NULL;
}

V fetchFKeyByName(V x, V m){
    if(isTable(x) && isSymbol(m)){
        ListY *y = x->a.fkey;
        while(y){
            V col = (V)(y->cn);
            if(vp(m) == vp(col) && vn(m) == vn(col)){
                B f = 1;
                DOI(vn(m), if(vQ(m,i)!=vQ(col,i)){f=0;break;})
                if(f) {
                    //P(" matched!\n"); printV(m); getchar();
                    R (V)(y->y);
                }
            }
            y = y->next;
        }
    }
    R NULL;
}

L fetchColumnValue(V z, V x, V y, B f){
    if(isTable2(x) && isSymbol(y)){
        if(f){ // load enumeration
            V e0 = fetchFKeyByName(x,y);
            if(e0) R copyColumnValue(z, e0);
        }
        if(isOne(y)){
            Q colId = vq(y);
            L colIndex = findColFromTable(x,colId);
            if(colIndex < 0) { EP("1. col not found\n"); R E_COL_NOT_FOUND; }
            else R copyColumnValue(z, getTableCol(getTableVals(x),colIndex));
        }
        else {
            L lenZ = vn(y);
            initV(z, H_G, lenZ);
            P("lenZ = %lld\n", lenZ);
            DOI(lenZ, { \
               L colIndex = findColFromTable(x, vQ(y,i)); \
               P("colIndex = %lld, %s\n", colIndex,getSymbolStr(vQ(y,i))); \
               if(colIndex < 0) R E_COL_NOT_FOUND; \
               CHECKE(copyColumnValue(vV(z,i), getTableCol(getTableVals(x), colIndex));)})
            R 0 ;
        }
    }
    else R E_DOMAIN;
}

L pfnColumnValue(V z, V x, V y){
    if(H_DEBUG) P("-> Entering column_value\n");
    R fetchColumnValue(z,x,y,1);
}

L pfnIsValidBranch(V z, V x){
    B t = 0;
    initV(z, H_B, 1);
    if(isOne(x)){
        switch(xp){
            caseB t = xb; break;
            caseJ t = 0!=xj; break;
            caseI t = 0!=xi; break;
            caseL t = 0!=xl; break;
            caseF t = 0!=xf; break; // precision ?
            caseE t = 0!=xe; break; // precision ?
        }
    }
    vb(z) = t;
    R 0;
}

/* Implement in order */

const E PI = acos(-1);

#define ABS(x) ((x)<0?(-x):x)
#define NEG(x) -(x)
#define CEIL(x) ceil(x)
#define FLOOR(x) floor(x)
#define ROUND(x) round(x)
#define DIVDE(x,t) (x/t)
#define SIGNUM(x) (0<(x)?1:0>(x)?-1:0)
#define PIMUL(x) (PI*x)
#define NOT(x) (!x)
#define EXP(x) exp(x)
#define LOGE(x) log(x)
#define LOG2(x) log2(x)
#define LOG10(x) log10(x)
#define POWER(x,y) pow(x,y)
#define LOG(x,y) (log(y)/log(x))
#define MODI(x,y) ((x)%(y))
#define MODF(x,y) fmod(x,y)
#define FACT(n) calcFact(n)

/* abs */
L pfnAbs(V z, V x){
    if(isTypeGroupReal(vp(x))){
        initV(z,vp(x),vn(x));  // Step 1: initialize z
        switch(vp(x)){         // Step 2: based on x
            caseB DOP(vn(x), vH(z,i)=vB(x,i))      break; //opt
            caseJ DOP(vn(x), vJ(z,i)=ABS(vJ(x,i))) break; //opt
            caseH DOP(vn(x), vH(z,i)=ABS(vH(x,i))) break;
            caseI DOP(vn(x), vI(z,i)=ABS(vI(x,i))) break;
            caseL DOP(vn(x), vL(z,i)=ABS(vL(x,i))) break;
            caseF DOP(vn(x), vF(z,i)=ABS(vF(x,i))) break;
            caseE DOP(vn(x), vE(z,i)=ABS(vE(x,i))) break;
            default: R E_NOT_IMPL;
        }
        R 0;
    }
    else R E_DOMAIN;
}

/* neg */
L pfnNeg(V z, V x){
    if(isTypeGroupReal(vp(x))){
        L typZ = inferNeg(vp(x));
        L lenZ = vn(x);
        initV(z,typZ,lenZ);
        switch(vp(x)){       
            caseB DOP(lenZ, vB(z,i)=NEG(vB(x,i))) break;
            caseJ DOP(lenZ, vJ(z,i)=NEG(vJ(x,i))) break;
            caseH DOP(lenZ, vH(z,i)=NEG(vH(x,i))) break;
            caseI DOP(lenZ, vI(z,i)=NEG(vI(x,i))) break;
            caseL DOP(lenZ, vL(z,i)=NEG(vL(x,i))) break;
            caseF DOP(lenZ, vF(z,i)=NEG(vF(x,i))) break;
            caseE DOP(lenZ, vE(z,i)=NEG(vE(x,i))) break;
            default: R E_NOT_IMPL;
        }
        R 0;
    }
    else R E_DOMAIN;
}

L pfnCeil(V z, V x){
    if(isTypeGroupReal(vp(x))){
        L typZ = vp(x); // inferReal2Int
        L lenZ = vn(x);
        initV(z,typZ,lenZ);
        switch(vp(x)){
            caseB DOP(lenZ, vB(z,i)=vB(x,i)) break; //merely copy (opt)
            caseJ DOP(lenZ, vJ(z,i)=vJ(x,i)) break;
            caseH DOP(lenZ, vH(z,i)=vH(x,i)) break;
            caseI DOP(lenZ, vI(z,i)=vI(x,i)) break;
            caseL DOP(lenZ, vL(z,i)=vL(x,i)) break;
            caseF DOP(lenZ, vF(z,i)=CEIL(vF(x,i))) break;
            caseE DOP(lenZ, vE(z,i)=CEIL(vE(x,i))) break;
            default: R E_NOT_IMPL;
        }
        R 0;
    }
    else R E_DOMAIN;
}

L pfnFloor(V z, V x){
    if(isTypeGroupReal(vp(x))){
        L typZ = vp(x); // inferReal2Int
        L lenZ = vn(x);
        initV(z,typZ,lenZ);
        switch(vp(x)){
            caseB DOP(lenZ, vB(z,i)=vB(x,i)) break; //merely copy (opt)
            caseJ DOP(lenZ, vJ(z,i)=vJ(x,i)) break;
            caseH DOP(lenZ, vH(z,i)=vH(x,i)) break;
            caseI DOP(lenZ, vI(z,i)=vI(x,i)) break;
            caseL DOP(lenZ, vL(z,i)=vL(x,i)) break;
            caseF DOP(lenZ, vF(z,i)=FLOOR(vF(x,i))) break;
            caseE DOP(lenZ, vE(z,i)=FLOOR(vE(x,i))) break;
            default: R E_NOT_IMPL;
        }
        R 0;
    }
    else R E_DOMAIN;
}

L pfnRound(V z, V x){
    if(isTypeGroupReal(vp(x))){
        L typZ = vp(x); // inferReal2Int
        L lenZ = vn(x);
        initV(z,typZ,lenZ);
        switch(vp(x)){
            caseB DOP(lenZ, vB(z,i)=vB(x,i)) break; //merely copy (opt)
            caseJ DOP(lenZ, vJ(z,i)=vJ(x,i)) break;
            caseH DOP(lenZ, vH(z,i)=vH(x,i)) break;
            caseI DOP(lenZ, vI(z,i)=vI(x,i)) break;
            caseL DOP(lenZ, vL(z,i)=vL(x,i)) break;
            caseF DOP(lenZ, vF(z,i)=ROUND(vF(x,i))) break;
            caseE DOP(lenZ, vE(z,i)=ROUND(vE(x,i))) break;
            default: R E_NOT_IMPL;
        }
        R 0;
    }
    else R E_DOMAIN;
}

static L pfnTrig(V z, V x, E (*fn_e)(E), F (*fn_f)(F)){
    if(isTypeGroupReal(vp(x))){
        L typZ = inferTrig(vp(x));
        L lenZ = vn(x);
        initV(z,typZ,lenZ);
        switch(vp(x)){
            caseB DOP(lenZ, vF(z,i)=(*fn_f)(vB(x,i))) break;
            caseJ DOP(lenZ, vF(z,i)=(*fn_f)(vJ(x,i))) break;
            caseH DOP(lenZ, vF(z,i)=(*fn_f)(vH(x,i))) break;
            caseI DOP(lenZ, vF(z,i)=(*fn_f)(vI(x,i))) break;
            caseL DOP(lenZ, vE(z,i)=(*fn_e)(vL(x,i))) break;
            caseF DOP(lenZ, vF(z,i)=(*fn_f)(vF(x,i))) break;
            caseE DOP(lenZ, vE(z,i)=(*fn_e)(vE(x,i))) break;
            default: R E_NOT_IMPL;
        }
        R 0;
    }
    else R E_DOMAIN;
}

L pfnTrigSin(V z, V x){
    R pfnTrig(z,x,&sin,&sinf);
}

L pfnTrigAsin(V z, V x){
    R pfnTrig(z,x,&asin,&asinf);
}

L pfnTrigCos(V z, V x){
    R pfnTrig(z,x,&cos,&cosf);
}

L pfnTrigAcos(V z, V x){
    R pfnTrig(z,x,&acos,&acosf);
}

L pfnTrigTan(V z, V x){
    R pfnTrig(z,x,&tan,&tanf);
}

L pfnTrigAtan(V z, V x){
    R pfnTrig(z,x,&atan,&atanf);
}

#define pfnHyper pfnTrig

L pfnHyperSinh(V z, V x){
    R pfnHyper(z,x,&sinh,&sinhf);
}

L pfnHyperAsinh(V z, V x){
    R pfnHyper(z,x,&asinh,&asinhf);
}

L pfnHyperCosh(V z, V x){
    R pfnHyper(z,x,&cosh,&coshf);
}

L pfnHyperAcosh(V z, V x){
    R pfnHyper(z,x,&acosh,&acoshf);
}

L pfnHyperTanh(V z, V x){
    R pfnHyper(z,x,&tanh,&tanhf);
}

L pfnHyperAtanh(V z, V x){
    R pfnHyper(z,x,&atanh,&atanhf);
}

L pfnConj(V z, V x){
    if(isComplex(x)){
        L typZ = H_X;
        L lenZ = vn(x);
        initV(z,typZ,lenZ);
        DOP(lenZ, {xReal(vX(z,i))=xReal(vX(x,i));xImag(vX(z,i))=-xImag(vX(x,i));})
        R 0;
    }
    else R E_DOMAIN;
}

#define recipSum(t, ptr) { DOP(lenZ, t+=ptr(x,i), reduction(+:t)); if(0==t) R E_DIV_ZERO; }
L pfnRecip(V z, V x){
    if(isTypeGroupReal(vp(x))){
        L typZ = inferRecip(vp(x));
        L lenZ = vn(x);
        initV(z,typZ,lenZ);
        switch(vp(x)){
            caseB {F t=0; recipSum(t,vB); DOP(lenZ, vF(z,i)=DIVDE(vB(x,i),t)); } break;
            caseJ {F t=0; recipSum(t,vJ); DOP(lenZ, vF(z,i)=DIVDE(vJ(x,i),t)); } break;
            caseH {F t=0; recipSum(t,vH); DOP(lenZ, vF(z,i)=DIVDE(vH(x,i),t)); } break;
            caseI {F t=0; recipSum(t,vI); DOP(lenZ, vF(z,i)=DIVDE(vI(x,i),t)); } break;
            caseL {E t=0; recipSum(t,vL); DOP(lenZ, vE(z,i)=DIVDE(vL(x,i),t)); } break;
            caseF {F t=0; recipSum(t,vF); DOP(lenZ, vF(z,i)=DIVDE(vF(x,i),t)); } break; //F
            caseE {E t=0; recipSum(t,vE); DOP(lenZ, vE(z,i)=DIVDE(vE(x,i),t)); } break;
            default: R E_NOT_IMPL;
        }
        R 0;
    }
    else R E_DOMAIN;
}

/* 1 -1 0 */
L pfnSignum(V z, V x){
    if(isTypeGroupReal(vp(x))){
        L typZ = H_H; // inferSignum(vp(x))
        L lenZ = vn(x);
        initV(z,typZ,lenZ);
        switch(vp(x)){
            caseB DOP(lenZ, vH(z,i)=vB(x,i))         break; //opt, no -1
            caseJ DOP(lenZ, vH(z,i)=SIGNUM(vJ(x,i))) break;
            caseH DOP(lenZ, vH(z,i)=SIGNUM(vH(x,i))) break;
            caseI DOP(lenZ, vH(z,i)=SIGNUM(vI(x,i))) break;
            caseL DOP(lenZ, vH(z,i)=SIGNUM(vL(x,i))) break;
            caseF DOP(lenZ, vH(z,i)=SIGNUM(vF(x,i))) break;
            caseE DOP(lenZ, vH(z,i)=SIGNUM(vE(x,i))) break;
            default: R E_NOT_IMPL;
        }
        R 0;
    }
    else R E_DOMAIN;
}

L pfnPi(V z, V x){
    if(isTypeGroupNumber(vp(x))){
        L typZ = inferPi(vp(x));
        L lenZ = vn(x);
        initV(z,typZ,lenZ);
        switch(vp(x)){
            caseB DOP(lenZ, vF(z,i)=PIMUL(vB(x,i))) break;
            caseJ DOP(lenZ, vF(z,i)=PIMUL(vJ(x,i))) break;
            caseH DOP(lenZ, vF(z,i)=PIMUL(vH(x,i))) break;
            caseI DOP(lenZ, vF(z,i)=PIMUL(vI(x,i))) break;
            caseL DOP(lenZ, vE(z,i)=PIMUL(vL(x,i))) break;
            caseF DOP(lenZ, vF(z,i)=PIMUL(vF(x,i))) break; //F
            caseE DOP(lenZ, vE(z,i)=PIMUL(vE(x,i))) break;
            caseX R E_NOT_IMPL;
        }
        R 0;
    }
    else R E_DOMAIN;
}

L pfnNot(V z, V x){
    if(isBool(x)){
        L typZ = H_B;
        L lenZ = vn(x);
        initV(z,typZ,lenZ);
        DOP(lenZ, vB(z,i)=NOT(vB(x,i)));
        R 0;
    }
    else R E_DOMAIN;
}

static L pfnExpLog(V z, V x, E (*fn_e)(E), F (*fn_f)(F)){
    if(isTypeGroupReal(xp)){
        L typZ = inferLog(vp(x));
        initV(z, typZ, vn(x));
        switch(xp){
            caseB DOP(xn, vF(z,i)=(*fn_f)(vB(x,i))) break;
            caseJ DOP(xn, vF(z,i)=(*fn_f)(vJ(x,i))) break;
            caseH DOP(xn, vF(z,i)=(*fn_f)(vH(x,i))) break;
            caseI DOP(xn, vF(z,i)=(*fn_f)(vI(x,i))) break;
            caseL DOP(xn, vE(z,i)=(*fn_e)(vL(x,i))) break;
            caseF DOP(xn, vF(z,i)=(*fn_f)(vF(x,i))) break;
            caseE DOP(xn, vE(z,i)=(*fn_e)(vE(x,i))) break;
            default: R E_NOT_IMPL;
        }
        R 0;
    }
    else R E_DOMAIN;
}

L pfnExp(V z, V x){
    R pfnExpLog(z,x,&exp,&expf);
}

L pfnLog(V z, V x){
    R pfnExpLog(z,x,&log,&logf);
}

L pfnLog2(V z, V x){
    R pfnExpLog(z,x,&log2,&log2f);
}

L pfnLog10(V z, V x){
    R pfnExpLog(z,x,&log10,&log10f);
}

L pfnSqrt(V z, V x){
    R pfnExpLog(z,x,&sqrt,&sqrtf);
}

L pfnLen(V z, V x){
    //P("len: x = %lld\n", xn);
    //printV2(x, 20);
    initV(z,H_L,1);
    vl(z)= isTable(x)?tableRow(x):vn(x);
    R 0;
}

L pfnRange(V z, V x){
    if(isOne(x) && isTypeGroupInt(xp)){
        L size = getSingleIntValue(x);
        initV(z,H_L,size);
        DOP(size, vL(z,i)=i) // opt, stride?
        R 0;
    }
    else R E_DOMAIN;
}

L pfnFact(V z, V x){
    if(isTypeGroupInt(xp)){
        initV(z,H_L,xn);
        switch(xp){
            caseB DOP(xn, vL(z,i)=FACT(vB(x,i))) break;
            caseJ DOP(xn, vL(z,i)=FACT(vJ(x,i))) break;
            caseH DOP(xn, vL(z,i)=FACT(vH(x,i))) break;
            caseI DOP(xn, vL(z,i)=FACT(vI(x,i))) break;
            caseL DOP(xn, vL(z,i)=FACT(vL(x,i))) break;
            default: R E_NOT_IMPL;
        }
        R 0;
    }
    else R E_DOMAIN;
}

L pfnReverse(V z, V x){
    if(isTypeGroupBasic(xp)){
        L typZ = xp, lenZ = xn;
        initV(z,typZ,lenZ);
        switch(typZ){
            caseB DOP(lenZ, vB(z,i)=vB(x,lenZ-i-1)) break;
            caseJ DOP(lenZ, vJ(z,i)=vJ(x,lenZ-i-1)) break;
            caseH DOP(lenZ, vH(z,i)=vH(x,lenZ-i-1)) break;
            caseI DOP(lenZ, vI(z,i)=vI(x,lenZ-i-1)) break;
            caseL DOP(lenZ, vL(z,i)=vL(x,lenZ-i-1)) break;
            caseF DOP(lenZ, vF(z,i)=vF(x,lenZ-i-1)) break;
            caseE DOP(lenZ, vE(z,i)=vE(x,lenZ-i-1)) break;
            caseX DOP(lenZ, vX(z,i)=vX(x,lenZ-i-1)) break;
            caseC DOP(lenZ, vC(z,i)=vC(x,lenZ-i-1)) break;
            caseQ DOP(lenZ, vQ(z,i)=vQ(x,lenZ-i-1)) break;
            caseS DOP(lenZ, vS(z,i)=vS(x,lenZ-i-1)) break;
            default: R E_NOT_IMPL;
        }
        R 0;
    }
    else R E_DOMAIN;
}


L pfnUnique(V z, V x){
    if(isTypeGroupBasic(vp(x))){
        V z0 = allocNode();
        CHECKE(pfnIndexOf(z0,x,x));  /* refer to indexof */
        L typZ = H_L, lenZ = 0, c = 0;
        DOP(vn(z0), lenZ += vL(z0,i)==i, reduction(+:lenZ))
        initV(z,typZ,lenZ);
        DOI(vn(x), if(vL(z0,i)==i)vL(z,c++)=i)
        /* free z0 */
        R 0;
    }
    else R E_DOMAIN;
}

L pfnWhere(V z, V x){
    if(isBool(x)){
        L typZ = H_L, lenZ = 0, lenX = vn(x), thresheld = 10000;
        if(lenX < thresheld){
            L c = 0;
            DOI(lenX, lenZ+=vB(x,i)) // faster?
            initV(z,typZ,lenZ);
            DOI(lenX, if(vB(x,i))vL(z,c++)=i)
        }
        else {
            L parZ[H_CORE], offset[H_CORE];
            memset(parZ  , 0, sizeof(L)*H_CORE);
            memset(offset, 0, sizeof(L)*H_CORE);
            CHECKE(getNumOfNonZero(x,parZ));
            // P("parZ:"); DOI(H_CORE, P(" %lld", parZ[i])); P("\n");
            DOI(H_CORE, lenZ += parZ[i])
            DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
            // P("offset:"); DOI(H_CORE, P(" %lld", offset[i])); P("\n");
            initV(z,typZ,lenZ);
            DOT(lenX, if(vB(x,i))vL(z,offset[tid]++)=i)
        }
        R 0;
    }
    else if(isListSameType(x, H_B)){
        L c=0,lenZ = 2;
        DOI(vn(x), {V t=vV(x,i);DOJ(vn(t), if(vB(t,j))c++)})
        initV(z,H_G,lenZ);
        V t0 = vV(z,0),  t1 = vV(z,1);
        initV(t0,H_L,c); initV(t1,H_L,c);
        c = 0;
        DOI(vn(x),{V t=vV(x,i); DOJ(vn(t), if(vB(t,j)){vL(t0,c)=i; vL(t1,c)=j; c++;})})
        R 0;
    }
    else R E_DOMAIN;
}

L pfnSum(V z, V x){
    if(isTypeGroupReal(vp(x))){
        L typZ = isFloat(x)?H_F:isDouble(x)?H_E:H_L;
        initV(z,typZ,1);
        switch(vp(x)){
            caseB {L t=0; DOP(vn(x), t+=vB(x,i), reduction(+:t)) vl(z)=t;} break;
            caseJ {L t=0; DOP(vn(x), t+=vJ(x,i), reduction(+:t)) vl(z)=t;} break;
            caseH {L t=0; DOP(vn(x), t+=vH(x,i), reduction(+:t)) vl(z)=t;} break;
            caseI {L t=0; DOP(vn(x), t+=vI(x,i), reduction(+:t)) vl(z)=t;} break;
            caseL {L t=0; DOP(vn(x), t+=vL(x,i), reduction(+:t)) vl(z)=t;} break;
            caseF {F t=0; DOP(vn(x), t+=vF(x,i), reduction(+:t)) vf(z)=t;} break;
            caseE {E t=0; DOP(vn(x), t+=vE(x,i), reduction(+:t)) ve(z)=t;} break;
        }
        R 0;
    }
    else R E_DOMAIN;
}

L pfnAvg(V z, V x){
    V t = allocNode();
    CHECKE(pfnSum(t,x));
    L typZ = isFloat(t)?H_F:H_E;
    initV(z,typZ,1);
    switch(vp(t)){
        caseL ve(z) = vl(t)*1.0/vn(x); break;
        caseF vf(z) = vf(t)    /vn(x); break;
        caseE ve(z) = ve(t)    /vn(x); break;
    }
    R 0;
}

#define REDUCELONG(op) (0==op?LLONG_MAX:LLONG_MIN)
#define REDUCEFLT(op)  (0==op?FLT_MAX:FLT_MIN)
#define REDUCEDBL(op)  (0==op?DBL_MAX:DBL_MIN)
#define REDUCE(op,t,x) (0==op?MIN(t,x):1==op?MAX(t,x):-1)
#define REDUCELINE(p,op,x) {p t=v##p(x,0); DOI(vn(x), t=REDUCE(op,t,v##p(x,i))) v##p(z,0)=t;}

L pfnReduce(V z, V x, L op){
    if(isTypeGroupReal(vp(x))){
        if(1>vn(x)){
            L typZ = isTypeGroupInt(vp(x))?H_L:vp(x);
            L lenZ = 1;
            initV(z,typZ,lenZ);
            switch(typZ){
                caseL vl(z) = REDUCELONG(op); break;
                caseF vf(z) = REDUCEFLT(op);  break;
                caseE ve(z) = REDUCEDBL(op);  break;
                default: R E_NOT_IMPL;
            }
        }
        else{
            L typZ = vp(x);
            L lenZ = 1;
            initV(z,typZ,lenZ);
            switch(typZ){
                caseB REDUCELINE(B,op,x); break;
                caseJ REDUCELINE(J,op,x); break;
                caseH REDUCELINE(H,op,x); break;
                caseI REDUCELINE(I,op,x); break;
                caseL REDUCELINE(L,op,x); break;
                caseF REDUCELINE(F,op,x); break;
                caseE REDUCELINE(E,op,x); break;
                default: R E_NOT_IMPL;
            }
        }
        R 0;
    }
    else R E_DOMAIN;
}

L pfnMin(V z, V x){
    R pfnReduce(z,x,0);
}

L pfnMax(V z, V x){
    R pfnReduce(z,x,1);
}

/* Date & Time */
L pfnChopDate(V z, V x, L op){
    if(isTypeGroupDate(vp(x))){
        initV(z,H_H,vn(x));
        switch(vp(x)){
            caseM DOP(vn(x), vH(z,i)=CHOPM(op,vM(x,i))) break;
            caseD DOP(vn(x), vH(z,i)=CHOPD(op,vD(x,i))) break;
            caseZ DOP(vn(x), CHOPZ(op,vH(z,i),vZ(x,i))) break;
            default: R E_NOT_IMPL;
        }
        R 0;
    }
    else R E_DOMAIN;
}

L pfnDateYear(V z, V x){
    R pfnChopDate(z,x,0);
}

L pfnDateMonth(V z, V x){
    R pfnChopDate(z,x,1);
}

L pfnDateDay(V z, V x){
    R (isDate(x)||isDateTime(x))?pfnChopDate(z,x,2):E_DOMAIN;
}

L pfnDate(V z, V x){
    if(isDateTime(x)){
        initV(z,H_D,vn(x));
        DOP(vn(x), vD(z,i)=(D)Z2D(vZ(x,i)))
        R 0;
    }
    else R E_DOMAIN;
}


L pfnChopTime(V z, V x, L op){
    if(isTypeGroupTime(vp(x))){
        initV(z,H_H,vn(x));
        switch(vp(x)){
            caseU DOP(vn(x), vH(z,i)= CHOPU(op,vU(x,i))) break;
            caseW DOP(vn(x), vH(z,i)= CHOPW(op,vW(x,i))) break;
            caseT DOP(vn(x), CHOPT (op,vH(z,i),vT(x,i))) break;
            caseZ DOP(vn(x), CHOPZT(op,vH(z,i),vZ(x,i))) break;
            default: R E_NOT_IMPL;
        }
        R 0;
    }
    else R E_DOMAIN;
}

L pfnTimeHour(V z, V x){
    R pfnChopTime(z,x,0);
}

L pfnTimeMinute(V z, V x){
    R pfnChopTime(z,x,1);
}

L pfnTimeSecond(V z, V x){
    R (isSecond(x)||isTime(x))?pfnChopTime(z,x,2):E_DOMAIN;
}

L pfnTimeMill(V z, V x){
    R isTime(x)?pfnChopTime(z,x,3):E_DOMAIN;
}

L pfnTime(V z, V x){
    if(isDateTime(x)){
        initV(z,H_T,vn(x));
        DOP(vn(x), vT(z,i)=(T)Z2T(vZ(x,i)))
        R 0;
    }
    else R E_DOMAIN;
}

L pfnEnlist(V z, V x){
    if(isTypeGroupAny(vp(x))){
        initV(z,H_G,1);
        CHECKE(copyV(vV(z,0),x));
        R 0;
    }
    else R E_DOMAIN;
}

L pfnRaze(V z, V x){
    if(isList(x)){
        L typZ=-1, lenZ=0, n=0;
        CHECKE(getCommonType(x, &typZ, &lenZ));
        initV(z,typZ,lenZ);
        CHECKE(fillRaze(z,&n,x));
        CHECKE(n!=lenZ?E_UNKNOWN:0);
        R 0;
    }
    else if(isTypeGroupBasic(vp(x))){
        R copyV(z,x);
    }
    else R E_DOMAIN;
}

L pfnToList(V z, V x){
    if(isTypeGroupBasic(xp)){
        initV(z,H_G,xn);
        switch(xp){
            caseB DOP(xn, {V t=vV(z,i); initV(t,xp,1); vb(t)=vB(x,i);}) break;
            caseJ DOP(xn, {V t=vV(z,i); initV(t,xp,1); vj(t)=vJ(x,i);}) break;
            caseH DOP(xn, {V t=vV(z,i); initV(t,xp,1); vh(t)=vH(x,i);}) break;
            caseI DOP(xn, {V t=vV(z,i); initV(t,xp,1); vi(t)=vI(x,i);}) break;
            caseL DOP(xn, {V t=vV(z,i); initV(t,xp,1); vl(t)=vL(x,i);}) break;
            caseF DOP(xn, {V t=vV(z,i); initV(t,xp,1); vf(t)=vF(x,i);}) break;
            caseE DOP(xn, {V t=vV(z,i); initV(t,xp,1); ve(t)=vE(x,i);}) break;
            caseX DOP(xn, {V t=vV(z,i); initV(t,xp,1); vx(t)=vX(x,i);}) break;
            caseQ DOP(xn, {V t=vV(z,i); initV(t,xp,1); vq(t)=vQ(x,i);}) break;
            caseS DOP(xn, {V t=vV(z,i); initV(t,xp,1); vs(t)=vS(x,i);}) break;
            default: R E_NOT_IMPL;
        }
        R 0;
    }
    else R E_DOMAIN;
}

L pfnToIndex(V z, V x){
    if(isEnum(x)){
        vp(z) = H_L;
        vn(z) = vn(x);
        vg(z) = vg(x); /* alias copy */
        R 0;
    }
    else R E_DOMAIN;
}

L pfnGroup(V z, V x){
    P("Input len = %lld, type = %lld\n", xn,xp);
    //printV2(x,100);
    // V0 y0,t0; V y = &y0, t = &t0;
    V y = allocNode();
    V t = allocNode();
    L lenZ = isList(x)?vn(x):1;
    L *order_list = NULL;
    initV(y,H_B,lenZ);
    //if(xn == 1483918){
        //DOI(20, P("%lld ",vL(x,i))) P("\n");
        //lib_hash_test(sL(x), vn(x));
    //}
 struct timeval tv0, tv1;
 gettimeofday(&tv0, NULL);
    if(isOrdered(x)){
        if(H_DEBUG) P("Ordered data found in pfnGroup\n");
        order_list = NULL;
    }
    else if(isInteger(x)){ // H_L
    }
    else {
        DOP(lenZ,vB(y,i)=1)
        CHECKE(pfnOrderBy(t,x,y));
        order_list = sL(t);
    }
 gettimeofday(&tv1, NULL);
 P("1.(elapsed time %g ms)\n\n", calcInterval(tv0,tv1));
    // P("t = \n");
    // printV(t);

 gettimeofday(&tv0, NULL);
    if(isList(x)){
        L numRow= 0==vn(x)?0:vn(vV(x,0));
        CHECKE(lib_get_group_by(z,x,order_list,numRow,lib_quicksort_cmp));
    }
    else if(isTypeGroupBasic(xp)){
        // V0 t1; V tx=&t1;
        L numRow= vn(x);
        // CHECKE(pfnEnlist(tx,x));
        CHECKE(lib_get_group_by(z,x,order_list,numRow,lib_quicksort_cmp_item));
    }
    else R E_DOMAIN;
 gettimeofday(&tv1, NULL);
 P("2.(elapsed time %g ms)\n\n", calcInterval(tv0,tv1));
 //getchar();
    //L tid = getSymbol((S)"148561");
    //DOI(vn(x), if(vQ(x,i)==tid)P("%lld\n",i))
    R 0;
}

/* Binary */

#define compareFloat(x,y) (x<y?(y-x<H_EPSILON?0:-1):(x-y<H_EPSILON?0:1))

#define COMP(op,x,y) (2>op?COMPLESS(op,x,y):4>op?COMPMORE(op,x,y):6>op?COMPEQ(op,x,y):0)
#define COMPLESS(op,x,y) (0==op?(x<y):(x)<=(y))
#define COMPMORE(op,x,y) (2==op?(x>y):(x)>=(y))
#define COMPEQ(op,x,y) (4==op?(x==y):(x!=y))

#define COMPFN(op,x,y,fn) (2>op?COMPLESSFN(op,x,y,fn):4>op?COMPMOREFN(op,x,y,fn):6>op?COMPEQFN(op,x,y,fn):0)
#define COMPLESSFN(op,x,y,fn) (0==op?fn(x,y)<0:fn(x,y)<=0)
#define COMPMOREFN(op,x,y,fn) (2==op?fn(x,y)>0:fn(x,y)>=0)
#define COMPEQFN(op,x,y,fn) (4==op?fn(x,y)==0:fn(x,y)!=0)

L pfnCompare(V z, V x, V y, L op){
    if(isTypeGroupComparable(vp(x)) && isTypeGroupComparable(vp(y))){
        if(!isValidLength(x,y)) R E_LENGTH;
        L lenZ = isOne(x)?vn(y):vn(x), typZ = H_B;
        if(isTypeGroupReal(vp(x)) && isTypeGroupReal(vp(y))){
            L typMax = MAX(vp(x),vp(y));
            V tempX = allocNode();
            V tempY = allocNode();
            CHECKE(promoteValue(tempX, x, typMax));
            CHECKE(promoteValue(tempY, y, typMax));
            initV(z,typZ,lenZ);
            if(isOne(x)) {
                switch(typMax){
                    caseB DOP(lenZ, vB(z,i)=COMP(op,vB(tempX,0),vB(tempY,i))) break;
                    caseJ DOP(lenZ, vB(z,i)=COMP(op,vJ(tempX,0),vJ(tempY,i))) break;
                    caseH DOP(lenZ, vB(z,i)=COMP(op,vH(tempX,0),vH(tempY,i))) break;
                    caseI DOP(lenZ, vB(z,i)=COMP(op,vI(tempX,0),vI(tempY,i))) break;
                    caseL DOP(lenZ, vB(z,i)=COMP(op,vL(tempX,0),vL(tempY,i))) break;
                    caseF DOP(lenZ, vB(z,i)=COMPFN(op,vF(tempX,0),vF(tempY,i),compareFloat)) break;
                    caseE DOP(lenZ, vB(z,i)=COMPFN(op,vE(tempX,0),vE(tempY,i),compareFloat)) break;
                }
            }
            else if(isOne(y)) {
                switch(typMax){
                    caseB DOP(lenZ, vB(z,i)=COMP(op,vB(tempX,i),vB(tempY,0))) break;
                    caseJ DOP(lenZ, vB(z,i)=COMP(op,vJ(tempX,i),vJ(tempY,0))) break;
                    caseH DOP(lenZ, vB(z,i)=COMP(op,vH(tempX,i),vH(tempY,0))) break;
                    caseI DOP(lenZ, vB(z,i)=COMP(op,vI(tempX,i),vI(tempY,0))) break;
                    caseL DOP(lenZ, vB(z,i)=COMP(op,vL(tempX,i),vL(tempY,0))) break;
                    caseF DOP(lenZ, vB(z,i)=COMPFN(op,vF(tempX,i),vF(tempY,0),compareFloat)) break;
                    caseE DOP(lenZ, vB(z,i)=COMPFN(op,vE(tempX,i),vE(tempY,0),compareFloat)) break;
                }
            }
            else {
                switch(typMax){
                    caseB DOP(lenZ, vB(z,i)=COMP(op,vB(tempX,i),vB(tempY,i))) break;
                    caseJ DOP(lenZ, vB(z,i)=COMP(op,vJ(tempX,i),vJ(tempY,i))) break;
                    caseH DOP(lenZ, vB(z,i)=COMP(op,vH(tempX,i),vH(tempY,i))) break;
                    caseI DOP(lenZ, vB(z,i)=COMP(op,vI(tempX,i),vI(tempY,i))) break;
                    caseL DOP(lenZ, vB(z,i)=COMP(op,vL(tempX,i),vL(tempY,i))) break;
                    caseF DOP(lenZ, vB(z,i)=COMPFN(op,vF(tempX,i),vF(tempY,i),compareFloat)) break;
                    caseE DOP(lenZ, vB(z,i)=COMPFN(op,vE(tempX,i),vE(tempY,i),compareFloat)) break;
                }
            }
        }
        else if(isSameType(x,y)){
            initV(z,typZ,lenZ);
            if(isOne(x)) {
                switch(vp(x)){
                    caseQ DOP(lenZ, vB(z,i)=COMPFN(op,vQ(x,0),vQ(y,i),compareSymbol)) break;
                    caseS DOP(lenZ, vB(z,i)=COMPFN(op,vS(x,0),vS(y,i),strcmp))        break;
                    caseM DOP(lenZ, vB(z,i)=COMP(op,vM(x,0),vM(y,i))) break;
                    caseD DOP(lenZ, vB(z,i)=COMP(op,vD(x,0),vD(y,i))) break;
                    caseZ DOP(lenZ, vB(z,i)=COMP(op,vZ(x,0),vZ(y,i))) break;
                    caseU DOP(lenZ, vB(z,i)=COMP(op,vU(x,0),vU(y,i))) break;
                    caseV DOP(lenZ, vB(z,i)=COMP(op,vV(x,0),vV(y,i))) break;
                    caseT DOP(lenZ, vB(z,i)=COMP(op,vT(x,0),vT(y,i))) break;
                }
            }
            else if(isOne(y)) {
                switch(vp(x)){
                    caseQ DOP(lenZ, vB(z,i)=COMPFN(op,vQ(x,i),vQ(y,0),compareSymbol)) break;
                    caseS DOP(lenZ, vB(z,i)=COMPFN(op,vS(x,i),vS(y,0),strcmp))        break;
                    caseM DOP(lenZ, vB(z,i)=COMP(op,vM(x,i),vM(y,0))) break;
                    caseD DOP(lenZ, vB(z,i)=COMP(op,vD(x,i),vD(y,0))) break;
                    caseZ DOP(lenZ, vB(z,i)=COMP(op,vZ(x,i),vZ(y,0))) break;
                    caseU DOP(lenZ, vB(z,i)=COMP(op,vU(x,i),vU(y,0))) break;
                    caseV DOP(lenZ, vB(z,i)=COMP(op,vV(x,i),vV(y,0))) break;
                    caseT DOP(lenZ, vB(z,i)=COMP(op,vT(x,i),vT(y,0))) break;
                }
            }
            else {
                switch(vp(x)){
                    caseQ DOP(lenZ, vB(z,i)=COMPFN(op,vQ(x,i),vQ(y,i),compareSymbol)) break;
                    caseS DOP(lenZ, vB(z,i)=COMPFN(op,vS(x,i),vS(y,i),strcmp))        break;
                    caseM DOP(lenZ, vB(z,i)=COMP(op,vM(x,i),vM(y,i))) break;
                    caseD DOP(lenZ, vB(z,i)=COMP(op,vD(x,i),vD(y,i))) break;
                    caseZ DOP(lenZ, vB(z,i)=COMP(op,vZ(x,i),vZ(y,i))) break;
                    caseU DOP(lenZ, vB(z,i)=COMP(op,vU(x,i),vU(y,i))) break;
                    caseV DOP(lenZ, vB(z,i)=COMP(op,vV(x,i),vV(y,i))) break;
                    caseT DOP(lenZ, vB(z,i)=COMP(op,vT(x,i),vT(y,i))) break;
                }
            }
        }
        else R E_TYPE;
        R 0;
    }
    else R E_DOMAIN;
}

/* less than */
L pfnLt(V z, V x, V y){
    R pfnCompare(z,x,y,0);
}

/* less equal than */
L pfnLeq(V z, V x, V y){
    R pfnCompare(z,x,y,1);
}

/* greater than */
L pfnGt(V z, V x, V y){
    R pfnCompare(z,x,y,2);
}

/* greater equal than */
L pfnGeq(V z, V x, V y){
    R pfnCompare(z,x,y,3);
}

/* equal */
L pfnEq(V z, V x, V y){
    R pfnCompare(z,x,y,4);
}

/* not equal */
L pfnNeq(V z, V x, V y){
    R pfnCompare(z,x,y,5);
}

#define BETWEEN(x,p,q,fn) (compareFloat(x,p)>=0 && compareFloat(x,q)<=0)
L pfnBetween(V z, V x, V p, V q){
    if(isTypeGroupComparable(vp(x)) && \
        isTypeGroupComparable(vp(p)) && \
        isTypeGroupComparable(vp(q))){
        if(isOne(p) && isOne(q)){
            L lenZ = vn(x), typZ = H_B;
            if(isTypeGroupReal(vp(x)) && isTypeGroupFloat(vp(p)) && isTypeGroupFloat(vp(q))){
                E valP = isFloat(p)?vf(p):ve(p);
                E valQ = isFloat(q)?vf(q):ve(q);
                initV(z,typZ,lenZ);
                switch(vp(x)){
                    caseF DOP(vn(x), vB(z,i)=BETWEEN(vF(x,i),valP,valQ,compareBetween)) break;
                    caseE DOP(vn(x), vB(z,i)=BETWEEN(vE(x,i),valP,valQ,compareBetween)) break;
                }
                R 0;
            }
            else R E_DOMAIN;
        }
        else R E_LENGTH;
    }
    else R E_DOMAIN;
}

#define ARITH2(op,x,y) (0==op?(x+y):1==op?(x-y):2==op?(x*y):3==op?(x/y):0)

L pfnArith(V z, V x, V y, L op){
    if(isTypeGroupReal(vp(x)) && isTypeGroupReal(vp(y))){
        if(!isValidLength(x,y))   R E_LENGTH;
        if(3==op && checkZero(y)) R E_DIV_ZERO;
        L lenZ   = isOne(x)?vn(y):vn(x);
        L typMax = MAX(vp(x),vp(y));
        L typZ   = typMax;
        V tempX = allocNode();
        V tempY = allocNode();
        CHECKE(promoteValue(tempX, x, typMax));
        CHECKE(promoteValue(tempY, y, typMax));
        initV(z,typZ,lenZ);
        if(isOne(x)) {
            switch(typMax){
                caseB DOP(lenZ, vB(z,i)=ARITH2(op,vB(tempX,0),vB(tempY,i))) break;
                caseJ DOP(lenZ, vJ(z,i)=ARITH2(op,vJ(tempX,0),vJ(tempY,i))) break;
                caseH DOP(lenZ, vH(z,i)=ARITH2(op,vH(tempX,0),vH(tempY,i))) break;
                caseI DOP(lenZ, vI(z,i)=ARITH2(op,vI(tempX,0),vI(tempY,i))) break;
                caseL DOP(lenZ, vL(z,i)=ARITH2(op,vL(tempX,0),vL(tempY,i))) break;
                caseF DOP(lenZ, vF(z,i)=ARITH2(op,vF(tempX,0),vF(tempY,i))) break;
                caseE DOP(lenZ, vE(z,i)=ARITH2(op,vE(tempX,0),vE(tempY,i))) break;
                default: R E_NOT_IMPL;
            }
        }
        else if(isOne(y)) {
            switch(typMax){
                caseB DOP(lenZ, vB(z,i)=ARITH2(op,vB(tempX,i),vB(tempY,0))) break;
                caseJ DOP(lenZ, vJ(z,i)=ARITH2(op,vJ(tempX,i),vJ(tempY,0))) break;
                caseH DOP(lenZ, vH(z,i)=ARITH2(op,vH(tempX,i),vH(tempY,0))) break;
                caseI DOP(lenZ, vI(z,i)=ARITH2(op,vI(tempX,i),vI(tempY,0))) break;
                caseL DOP(lenZ, vL(z,i)=ARITH2(op,vL(tempX,i),vL(tempY,0))) break;
                caseF DOP(lenZ, vF(z,i)=ARITH2(op,vF(tempX,i),vF(tempY,0))) break;
                caseE DOP(lenZ, vE(z,i)=ARITH2(op,vE(tempX,i),vE(tempY,0))) break;
                default: R E_NOT_IMPL;
            }
        }
        else {
            switch(typMax){
                caseB DOP(lenZ, vB(z,i)=ARITH2(op,vB(tempX,i),vB(tempY,i))) break;
                caseJ DOP(lenZ, vJ(z,i)=ARITH2(op,vJ(tempX,i),vJ(tempY,i))) break;
                caseH DOP(lenZ, vH(z,i)=ARITH2(op,vH(tempX,i),vH(tempY,i))) break;
                caseI DOP(lenZ, vI(z,i)=ARITH2(op,vI(tempX,i),vI(tempY,i))) break;
                caseL DOP(lenZ, vL(z,i)=ARITH2(op,vL(tempX,i),vL(tempY,i))) break;
                caseF DOP(lenZ, vF(z,i)=ARITH2(op,vF(tempX,i),vF(tempY,i))) break;
                caseE DOP(lenZ, vE(z,i)=ARITH2(op,vE(tempX,i),vE(tempY,i))) break;
                default: R E_NOT_IMPL;
            }
        }
        R 0;
    }
    else if(isTypeGroupDTime(vp(x)) && isTypeGroupDTime(vp(y))){
        switch(op){
            case 0:  break; //plus
            case 1:  break; //sub
            case 2:  break; //mul
            case 3:  break; //div
        }
        R E_NOT_IMPL;
    }
    else R E_DOMAIN;
}

/* plus: 0 */
L pfnPlus(V z, V x, V y){
    R pfnArith(z,x,y,0);
}

/* plus: 1 */
L pfnMinus(V z, V x, V y){
    R pfnArith(z,x,y,1);
}

/* plus: 2 */
L pfnMul(V z, V x, V y){
    R pfnArith(z,x,y,2);
}

/* plus: 3 */
L pfnDiv(V z, V x, V y){
    R pfnArith(z,x,y,3);
}

//#define LOGIC(op,x,y) (2>op?LOGICAND(op,x,y):4>op?LOGICOR(op,x,y):(x^y))
//#define LOGICAND(op,x,y) (0==op?(x&y):~(x&y))
//#define LOGICOR(op,x,y)  (2==op?(x||y):~(x|y))

L pfnLogic(V z, V x, V y, B (*fn_b)(B,B)){
    if(isBool(x) && isBool(y)){
        if(!isValidLength(x,y)) R E_LENGTH;
        L lenZ = isOne(x)?vn(y):vn(x), typZ = H_B;
        initV(z,typZ,lenZ);
        if(isOne(x)) {
            DOP(lenZ, vB(z,i)=(*fn_b)(vB(x,0),vB(y,i)))
        }
        else if(isOne(y)) {
            DOP(lenZ, vB(z,i)=(*fn_b)(vB(x,i),vB(y,0)))
        }
        else {
            DOP(lenZ, vB(z,i)=(*fn_b)(vB(x,i),vB(y,i)))
        }
        R 0;
    }
    else R E_DOMAIN;
}

/* and: 0 */
L pfnAnd(V z, V x, V y){
    R pfnLogic(z,x,y,&logicAnd);
}

/* nand: 1 */
L pfnNand(V z, V x, V y){
    R pfnLogic(z,x,y,&logicNand);
}

/* or: 2 */
L pfnOr(V z, V x, V y){
    R pfnLogic(z,x,y,&logicOr);
}

/* nor: 3 */
L pfnNor(V z, V x, V y){
    R pfnLogic(z,x,y,&logicNor);
}

/* xor: 4 */
L pfnXor(V z, V x, V y){
    R pfnLogic(z,x,y,&logicXor);
}

L pfnPowerLog(V z, V x, V y, E (*fn_e)(E,E), F (*fn_f)(F,F)){
    if(isTypeGroupReal(vp(x)) && isTypeGroupReal(vp(y))){
        if(!isValidLength(x,y)) R E_LENGTH;
        L lenZ  = isOne(x)?vn(y):vn(x);
        L typMax= MAX(vp(x),vp(y));
        L typZ  = inferPowerLog(typMax);
        V tempX = allocNode();
        V tempY = allocNode();
        CHECKE(promoteValue(tempX, x, typMax));
        CHECKE(promoteValue(tempY, y, typMax));
        initV(z,typZ,lenZ);
        if(isOne(x)){
            switch(typMax){
                caseB DOP(lenZ, vF(z,i)=(*fn_f)(vB(tempX,0),vB(tempY,i))) break;
                caseJ DOP(lenZ, vF(z,i)=(*fn_f)(vJ(tempX,0),vJ(tempY,i))) break;
                caseH DOP(lenZ, vF(z,i)=(*fn_f)(vH(tempX,0),vH(tempY,i))) break;
                caseI DOP(lenZ, vF(z,i)=(*fn_f)(vI(tempX,0),vI(tempY,i))) break;
                caseL DOP(lenZ, vE(z,i)=(*fn_e)(vL(tempX,0),vL(tempY,i))) break;
                caseF DOP(lenZ, vF(z,i)=(*fn_f)(vF(tempX,0),vF(tempY,i))) break;
                caseE DOP(lenZ, vE(z,i)=(*fn_e)(vE(tempX,0),vE(tempY,i))) break;
                default: R E_NOT_IMPL;
            }
        }
        else if(isOne(y)){
            switch(typMax){
                caseB DOP(lenZ, vF(z,i)=(*fn_f)(vB(tempX,i),vB(tempY,0))) break;
                caseJ DOP(lenZ, vF(z,i)=(*fn_f)(vJ(tempX,i),vJ(tempY,0))) break;
                caseH DOP(lenZ, vF(z,i)=(*fn_f)(vH(tempX,i),vH(tempY,0))) break;
                caseI DOP(lenZ, vF(z,i)=(*fn_f)(vI(tempX,i),vI(tempY,0))) break;
                caseL DOP(lenZ, vE(z,i)=(*fn_e)(vL(tempX,i),vL(tempY,0))) break;
                caseF DOP(lenZ, vF(z,i)=(*fn_f)(vF(tempX,i),vF(tempY,0))) break;
                caseE DOP(lenZ, vE(z,i)=(*fn_e)(vE(tempX,i),vE(tempY,0))) break;
                default: R E_NOT_IMPL;
            }
        }
        else {
            switch(typMax){
                caseB DOP(lenZ, vF(z,i)=(*fn_f)(vB(tempX,i),vB(tempY,i))) break;
                caseJ DOP(lenZ, vF(z,i)=(*fn_f)(vJ(tempX,i),vJ(tempY,i))) break;
                caseH DOP(lenZ, vF(z,i)=(*fn_f)(vH(tempX,i),vH(tempY,i))) break;
                caseI DOP(lenZ, vF(z,i)=(*fn_f)(vI(tempX,i),vI(tempY,i))) break;
                caseL DOP(lenZ, vE(z,i)=(*fn_e)(vL(tempX,i),vL(tempY,i))) break;
                caseF DOP(lenZ, vF(z,i)=(*fn_f)(vF(tempX,i),vF(tempY,i))) break;
                caseE DOP(lenZ, vE(z,i)=(*fn_e)(vE(tempX,i),vE(tempY,i))) break;
                default: R E_NOT_IMPL;
            }
        }
        /* 
         * Next steps:
         * 1) check if error cases occur (e.g. (-1)^0.5)
         * 2) if yes, redo the power with complex numbers
         */
        R 0;
    }
    else R E_DOMAIN;
}

L pfnPower(V z, V x, V y){
    R pfnPowerLog(z,x,y,&pow,&powf);
}

L pfnLogBase(V z, V x, V y){
    R pfnPowerLog(z,x,y,&logBaseE,&logBaseF);
}

/*
 * if fmod(x,0), return nan
 */
L pfnMod(V z, V x, V y){
    if(isTypeGroupReal(vp(x)) && isTypeGroupReal(vp(y))){
        if(!isValidLength(x,y)) R E_LENGTH;
        L lenZ   = isOne(x)?vn(y):vn(x);
        L typMax = MAX(vp(x),vp(y));
        L typZ   = H_B==typMax?H_H:typMax;
        V tempX = allocNode();
        V tempY = allocNode();
        CHECKE(promoteValue(tempX, x, typMax));
        CHECKE(promoteValue(tempY, y, typMax));
        initV(z,typZ,lenZ);
        if(isOne(x)){
            switch(typMax){
                caseB DOP(lenZ, vH(z,i)=MODI(vB(tempX,0),vB(tempY,i))) break;
                caseJ DOP(lenZ, vJ(z,i)=MODI(vJ(tempX,0),vJ(tempY,i))) break;
                caseH DOP(lenZ, vH(z,i)=MODI(vH(tempX,0),vH(tempY,i))) break;
                caseI DOP(lenZ, vI(z,i)=MODI(vI(tempX,0),vI(tempY,i))) break;
                caseL DOP(lenZ, vL(z,i)=MODI(vL(tempX,0),vL(tempY,i))) break;
                caseF DOP(lenZ, vF(z,i)=MODF(vF(tempX,0),vF(tempY,i))) break;
                caseE DOP(lenZ, vE(z,i)=MODF(vE(tempX,0),vE(tempY,i))) break;
                default: R E_NOT_IMPL;
            }
        }
        else if(isOne(y)){
            switch(typMax){
                caseB DOP(lenZ, vH(z,i)=MODI(vB(tempX,i),vB(tempY,0))) break;
                caseJ DOP(lenZ, vJ(z,i)=MODI(vJ(tempX,i),vJ(tempY,0))) break;
                caseH DOP(lenZ, vH(z,i)=MODI(vH(tempX,i),vH(tempY,0))) break;
                caseI DOP(lenZ, vI(z,i)=MODI(vI(tempX,i),vI(tempY,0))) break;
                caseL DOP(lenZ, vL(z,i)=MODI(vL(tempX,i),vL(tempY,0))) break;
                caseF DOP(lenZ, vF(z,i)=MODF(vF(tempX,i),vF(tempY,0))) break;
                caseE DOP(lenZ, vE(z,i)=MODF(vE(tempX,i),vE(tempY,0))) break;
                default: R E_NOT_IMPL;
            }
        }
        else {
            switch(typMax){
                caseB DOP(lenZ, vH(z,i)=MODI(vB(tempX,i),vB(tempY,i))) break;
                caseJ DOP(lenZ, vJ(z,i)=MODI(vJ(tempX,i),vJ(tempY,i))) break;
                caseH DOP(lenZ, vH(z,i)=MODI(vH(tempX,i),vH(tempY,i))) break;
                caseI DOP(lenZ, vI(z,i)=MODI(vI(tempX,i),vI(tempY,i))) break;
                caseL DOP(lenZ, vL(z,i)=MODI(vL(tempX,i),vL(tempY,i))) break;
                caseF DOP(lenZ, vF(z,i)=MODF(vF(tempX,i),vF(tempY,i))) break;
                caseE DOP(lenZ, vE(z,i)=MODF(vE(tempX,i),vE(tempY,i))) break;
                default: R E_NOT_IMPL;
            }
        }
        R 0;
    }
    else R E_DOMAIN;
}

L pfnCompress(V z, V x, V y){
    //P("typ: x = %lld, y = %lld\n", vp(x),vp(y));
    if(isBool(x)){
        if(!isEqualLength(x,y)) R E_LENGTH;
        L lenX = vn(x);
        L typZ = vp(y);
        L k    = 0;
        L lenZ = 0, parZ[H_CORE], offset[H_CORE];
        memset(parZ  , 0, sizeof(L)*H_CORE);
        memset(offset, 0, sizeof(L)*H_CORE);
        CHECKE(getNumOfNonZero(x,parZ));
        DOI(H_CORE, lenZ += parZ[i])
        DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
        initV(z,typZ,lenZ);
        if(k != lenZ){
            switch(typZ){
                caseB DOT(lenX, if(vB(x,i))vB(z,offset[tid]++)=vB(y,i)) break;
                caseJ DOT(lenX, if(vB(x,i))vJ(z,offset[tid]++)=vJ(y,i)) break;
                caseH DOT(lenX, if(vB(x,i))vH(z,offset[tid]++)=vH(y,i)) break;
                caseI DOT(lenX, if(vB(x,i))vI(z,offset[tid]++)=vI(y,i)) break;
                caseL DOT(lenX, if(vB(x,i))vL(z,offset[tid]++)=vL(y,i)) break;
                caseF DOT(lenX, if(vB(x,i))vF(z,offset[tid]++)=vF(y,i)) break;
                caseE DOT(lenX, if(vB(x,i))vE(z,offset[tid]++)=vE(y,i)) break;
                caseQ DOT(lenX, if(vB(x,i))vQ(z,offset[tid]++)=vQ(y,i)) break;
                caseC DOT(lenX, if(vB(x,i))vC(z,offset[tid]++)=vC(y,i)) break;
                caseS DOT(lenX, if(vB(x,i))vS(z,offset[tid]++)=vS(y,i)) break;
                caseY {vy(z) = vy(y);\
                      DOT(lenX, if(vB(x,i))vY(z,offset[tid]++)=vY(y,i)) break;}
                caseD DOT(lenX, if(vB(x,i))vD(z,offset[tid]++)=vD(y,i)) break;
                default: R E_NOT_IMPL;
            }
        }
        else if(lenZ > 0){ // copy all of items
            CHECKE(copyV(z,y));
        }
        //P("Before: %lld; After: %lld\n", vn(x), vn(z));
        R 0;
    }
    else R E_DOMAIN;
}

#define INDEXOF(p,z,x,y) lib_index_of_##p(sL(z),s##p(x),vn(x),s##p(y),vn(y))
#define INDEXOFG(z,x,lenX,y,lenY) lib_index_of_G(sL(z),x,lenX,y,lenY)
L pfnIndexOf(V z, V x, V y){
    if(isTypeGroupReal(vp(x)) && isTypeGroupReal(vp(y))){
        if(isOrdered(x)){
            if(H_DEBUG) P("Ordered data found in index_of\n");
            R searchOrdered(z,x,y);
        }
        else {
            L typMax = MAX(vp(x), vp(y));
            L lenZ   = vn(y);
            V tempX = allocNode();
            V tempY = allocNode();
            CHECKE(promoteValue(tempX, x, typMax));
            CHECKE(promoteValue(tempY, y, typMax));
            initV(z,H_L,lenZ);
            switch(typMax){
                caseB INDEXOF(B, z, tempX, tempY); break;
                caseJ INDEXOF(J, z, tempX, tempY); break;
                caseH INDEXOF(H, z, tempX, tempY); break;
                caseI INDEXOF(I, z, tempX, tempY); break;
                caseL INDEXOF(L, z, tempX, tempY); break;
                caseF INDEXOF(F, z, tempX, tempY); break;
                caseE INDEXOF(E, z, tempX, tempY); break;
                default: R E_NOT_IMPL;
            }
        }
        R 0;
    }
    else if(isSameType(x,y)) {
        if(isTypeGroupBasic(vp(x))){
            L lenZ = vn(y);
            initV(z,H_L,lenZ);
            switch(xp){
                caseX INDEXOF(X, z, x, y); break;
                caseQ INDEXOF(Q, z, x, y); break;
                caseS INDEXOF(S, z, x, y); break;
                caseM INDEXOF(M, z, x, y); break;
                caseD INDEXOF(D, z, x, y); break;
                caseZ INDEXOF(Z, z, x, y); break;
                caseU INDEXOF(U, z, x, y); break;
                caseW INDEXOF(W, z, x, y); break;
                caseT INDEXOF(T, z, x, y); break;
            }
            R 0;
        }
        else if(isList(x)){
            L lenX = 0, lenY = 0;
            CHECKE(isListIndexOf(x,y,&lenX,&lenY));
            initV(z,H_L,lenY);
            INDEXOFG(z, x, lenX, y, lenY);
            R 0;
        }
        else R E_DOMAIN;
    }
    else R E_TYPE;
}

L pfnAppend(V z, V x, V y){
    if(isTypeGroupReal(vp(x)) && isTypeGroupReal(vp(y))){
        L lenZ   = vn(x) + vn(y), c = vn(x);
        L typMax = MAX(vp(x),vp(y));
        V tempX = allocNode();
        V tempY = allocNode();
        CHECKE(promoteValue(tempX, x, typMax));
        CHECKE(promoteValue(tempY, y, typMax));
        initV(z,typMax,lenZ);
        switch(typMax){
            caseB DOP(vn(x),vB(z,i)=vB(tempX,i)) DOP(vn(y),vB(z,c+i)=vB(tempY,i)) break;
            caseJ DOP(vn(x),vJ(z,i)=vJ(tempX,i)) DOP(vn(y),vJ(z,c+i)=vJ(tempY,i)) break;
            caseH DOP(vn(x),vH(z,i)=vH(tempX,i)) DOP(vn(y),vH(z,c+i)=vH(tempY,i)) break;
            caseI DOP(vn(x),vI(z,i)=vI(tempX,i)) DOP(vn(y),vI(z,c+i)=vI(tempY,i)) break;
            caseL DOP(vn(x),vL(z,i)=vL(tempX,i)) DOP(vn(y),vL(z,c+i)=vL(tempY,i)) break;
            caseF DOP(vn(x),vF(z,i)=vF(tempX,i)) DOP(vn(y),vF(z,c+i)=vF(tempY,i)) break;
            caseE DOP(vn(x),vE(z,i)=vE(tempX,i)) DOP(vn(y),vE(z,c+i)=vE(tempY,i)) break;
            default: R E_NOT_IMPL;
        }
        R 0;
    }
    else if(isSameType(x,y) && (isTypeGroupString(vp(x)) || isComplex(x))) {
        L typZ = vp(x), c = vn(x);
        L lenZ = vn(x) + vn(y);
        initV(z,typZ,lenZ);
        switch(vp(x)){
            caseX DOP(vn(x), vX(z,i)=vX(x,i))
                  DOP(vn(y), vX(z,c+i)=vX(y,i))
                  break;
            caseQ DOP(vn(x), vQ(z,i)=vQ(x,i))
                  DOP(vn(y), vQ(z,c+i)=vQ(y,i))
                  break;
            caseC DOP(vn(x), vC(z,i)=vC(x,i))
                  DOP(vn(y), vC(z,c+i)=vC(x,i))
                  break;
            caseS DOP(vn(x), vS(z,i)=vS(x,i))
                  DOP(vn(y), vS(z,c+i)=vS(x,i))
                  break;
        }
        R 0;
    }
    else if(isList(x)){
        R appendList(z,x,y);
    }
    else if(isList(y)){
        R appendList(z,y,x);
    }
    else if((isEnum(x) && isTypeGroupNumber(vp(y)))){
        R appendEnum(z,x,y);
    }
    else if((isEnum(y) && isTypeGroupNumber(vp(x)))){
        R appendEnum(z,y,x);
    }
    else R E_DOMAIN;
}

/* 
 * x: string
 * y: string (done), symbol, list of string and symbol (pending)
 * 
 * Case 1: create_jit; DOP(...); free_jit
 * Case 2: DOLIKE(...){create_jit; do; free_jit}  (faster)
 */
//#define LIKEMATCH(src,slen,re,matchData) \
//    pcre2_match(re,\
//        reinterpret_cast<unsigned char*>(src),\
//        slen,0,PCRE2_ANCHORED,matchData,NULL\
//    )<0?0:1
// PCRE2_SPTR -> unsigned char*
#define LIKEMATCH(src,slen,re,matchData) \
    pcre2_jit_match(re,\
       (PCRE2_SPTR)src,\
        slen,0,PCRE2_ANCHORED,matchData,NULL\
    )<0?0:1
#define DOLIKE(n, x, ...){ L seg=(n)/H_CORE; \
    _Pragma(STRINGIFY(omp parallel __VA_ARGS__)) \
    { \
        pcre2_match_context *mcontext = pcre2_match_context_create(NULL); \
        pcre2_jit_stack *jit_stack = pcre2_jit_stack_create(64*1024, 512*1024, NULL);\
        pcre2_jit_stack_assign(mcontext, NULL, jit_stack);\
        pcre2_match_data *match = pcre2_match_data_create_from_pattern(re, NULL);\
        L tid = omp_get_thread_num(); \
        for(L i=tid*seg,i2=(tid!=H_CORE-1?(i+seg):(n));i<i2;i++) x; \
        pcre2_match_data_free(match); \
        pcre2_match_context_free(mcontext); \
        pcre2_jit_stack_free(jit_stack); \
    } \
}
// TODO: debug caseQ, a non-deterministic bug
L pfnLike(V z, V x, V y){
    if(isTypeGroupString(vp(x)) && (isChar(y) || isString(y))){
        B t;
        if(isChar(y) || isOne(y)){
            L lenZ = isChar(x)?1:vn(x);
            S strY = isChar(y)?sC(y):vs(y);
            pcre2_code *re = getLikePatten(strY);
            /* jit facilities */
            I jit_status = pcre2_jit_compile(re, PCRE2_JIT_COMPLETE);
            B case1 = false;
            //case 1
            // if(jit_status == 0) P("jit pattern initialized successfully\n");
            // else P("jit pattern initialized failed %lld\n", jit_status);
            // pcre2_match_context *mcontext = pcre2_match_context_create(NULL);
            // pcre2_jit_stack *jit_stack = pcre2_jit_stack_create(32*1024, 1024*1024, NULL);
            // pcre2_jit_stack_assign(mcontext, NULL, jit_stack);
            pcre2_match_data *match = pcre2_match_data_create_from_pattern(re, NULL);
            if(re==NULL) R E_NULL_VALUE;
            initV(z,H_B,lenZ);
            //P("Entering pfnLike: x(%lld, %lld), y(%lld, %lld)\n",vp(x),vn(x),vp(y),vn(y)); //getchar();
            switch(vp(x)){
                caseC vB(z,0)=getLikeMatch(sC(x),re,match);                                break;
                caseQ DOLIKE(vn(x), {vB(z,i)=LIKEMATCH(getSymbolStr(vQ(x,i)),getSymbolSize(vQ(x,i)),re,match);}) break;
                caseS DOLIKE(vn(x), {vB(z,i)=LIKEMATCH(vS(x,i),strlen(vS(x,i)),re,match);}) break;
            }
            //L jit_size = pcre2_pattern_info(re, PCRE2_INFO_JITSIZE, NULL);
            //P("pfnLike time (ms): %g ms , jit size = %lld\n", calcInterval(tv0, tv1), jit_size); //getchar();
            pcre2_code_free(re);
            pcre2_match_data_free(match);
            //case 1
            // pcre2_match_context_free(mcontext);
            // pcre2_jit_stack_free(jit_stack);
            R 0;
        }
        else if(isChar(x) || isEqualLength(x,y)){  // y:string
            switch(vp(x)){
                caseC DOI(vn(y), \
                      CHECKE(getLikeFromString(&t,sC(x),vS(y,i))))                   break;
                caseQ DOI(vn(y), \
                      CHECKE(getLikeFromString(&t,getSymbolStr(vL(x,i)),vS(y,i))))   break;
                caseS DOI(vn(y), \
                      CHECKE(getLikeFromString(&t,vS(x,i),vS(y,i))))                 break;
            }
            initV(z,H_B,vn(y));
            switch(vp(x)){
                caseC DOI(vn(y), \
                      {getLikeFromString(&t,sC(x),vS(y,i)); vB(z,0)=t;})             break;
                caseQ DOI(vn(y), \
                      {getLikeFromString(&t,getSymbolStr(vL(x,i)),vS(y,i)); \
                       vB(z,i)=t;})                                                  break;
                caseS DOI(vn(y), \
                      {getLikeFromString(&t,vS(x,i),vS(y,i)); \
                       vB(z,i)=t;})                                                  break;
            }
            DOI(10, P("%lld ",vB(z,i))) P("\n");
            R 0;
        }
        else R E_LENGTH;
    }
    else R E_DOMAIN;
}

L pfnOrderBy(V z, V x, V y){
    if(isList(x) && isBool(y) && isEqualLength(x,y)){
        DOI(vn(x), if(!isTypeGroupBasic(vp(vV(x,i))))R E_DOMAIN)
        if(!checkMatch(x)) R E_MATCH;
        L lenZ= 0==vn(x)?0:vn(vV(x,0));
        initV(z,H_L,lenZ);
        // P("lenZ = %lld, item = %lld\n",lenZ,vn(x));
        // P("before\n");
        // DOI(10, P("%s, %s, %lld\n", \
        //     getSymbolStr(vQ(vV(x,0),i)), getSymbolStr(vQ(vV(x,1),i)),vL(vV(x,2),i)))
        // lib_list_order_by(sL(z), lenZ, x, sB(y), lib_quicksort_cmp);
        lib_order_by_list(sL(z), x, sB(y), lenZ, 0, lib_quicksort_cmp_item);
        // P("after\n");
        // DOI(lenZ, if(!strcmp(getSymbolStr(vQ(vV(x,0),vL(z,i))), "Brand#54") && \
        //     !strcmp(getSymbolStr(vQ(vV(x,1),vL(z,i))), "STANDARD BRUSHED COPPER")) \
        //     P("[%5lld] %s, %s, %lld\n", i, \
        //     getSymbolStr(vQ(vV(x,0),vL(z,i))), getSymbolStr(vQ(vV(x,1),vL(z,i))), vL(vV(x,2),vL(z,i))))
        // getchar();
        // P("----\n");
        // DOI(20, P("[%3lld] %s, %s, %lld\n", i, \
        //     getSymbolStr(vQ(vV(x,0),vL(z,i))), getSymbolStr(vQ(vV(x,1),vL(z,i))), vL(vV(x,2),vL(z,i))))
        // getchar();
        R 0;
    }
    else if(isTypeGroupBasic(vp(x)) && isBool(y) && 1==vn(y)){
        initV(z,H_L,vn(x));
        // lib_list_order_by(sL(z), vn(x), x, sB(y), lib_quicksort_cmp_item);
        lib_order_by_vector(sL(z), x, sB(y), vn(x), lib_quicksort_cmp_item);
        R 0;
    }
    else R E_DOMAIN;
}

L pfnEach(V z, V x, FUNC1(foo)){
    if(isList(x)){
        initV(z,H_G,vn(x));
        //P("Each: input size = %lld\n", vn(x));
        L total = 0;
        //DOI(xn, total += vn(vV(x,i))) P("avg = %g\n", total*1.0/vn(x));
        DOI(xn, CHECKE((*foo)(vV(z,i),vV(x,i))))
        R 0;
    }
    else R E_DOMAIN;
}

L pfnEachItem(V z, V x, V y, FUNC2(foo)){
    L lenX = isList(x)?vn(x):1;
    L lenY = isList(y)?vn(y):1;
    if(isList(x) && isList(y)){
        if(lenX==lenY){
            initV(z,H_G,lenX);
            DOI(lenX, CHECKE((*foo)(vV(z,i),vV(x,i),vV(y,i))))
        }
        else if(1==lenX){
            initV(z,H_G,lenY);
            DOI(lenY, CHECKE((*foo)(vV(z,i),vV(x,0),vV(y,i))))
        }
        else if(1==lenY){
            initV(z,H_G,lenX);
            DOI(lenY, CHECKE((*foo)(vV(z,i),vV(x,i),vV(y,0))))
        }
        else R E_LENGTH;
    }
    else if(isList(y)){
        initV(z,H_G,lenY);
        DOI(lenY, CHECKE((*foo)(vV(z,i),x,vV(y,i))))
    }
    else if(isList(x)){
        initV(z,H_G,lenX);
        DOI(lenX, CHECKE((*foo)(vV(z,i),vV(x,i),y)))
    }
    else { /* offload to foo */
        CHECKE((*foo)(z,x,y));
    }
    R 0;
}

L pfnEachLeft(V z, V x, V y, FUNC2(foo)){
    if(isList(x)){
        L lenZ = vn(x);
        initV(z,H_G,lenZ);
        DOI(lenZ, CHECKE((*foo)(vV(z,i),vV(x,i),y)))
    }
    else {
        CHECKE((*foo)(z,x,y));
    }
    R 0;
}

L pfnEachRight(V z, V x, V y, FUNC2(foo)){
    if(isList(y)){
        P("len(x) = %lld, len(y) = %lld\n", vn(x),vn(y));
        L lenZ = vn(y);
        initV(z,H_G,lenZ);
        // P("pfnEachRight: size(z) = %lld\n", lenZ);
        // L minVal = 999999, maxVal = -1;
        // DOI(lenZ, {V tt=vV(y,i); if(minVal>vn(tt)) minVal=vn(tt); if(maxVal<vn(tt)) maxVal=vn(tt);})
        // P("lenZ = %lld, minVal = %lld, maxVal = %lld\n", lenZ, minVal, maxVal); getchar();
        DOI(lenZ, CHECKE((*foo)(vV(z,i),x,vV(y,i))))  // seg fault after DOI -> DOP
    }
    else {
        CHECKE((*foo)(z,x,y));
    }
    R 0;
}

/* TODO: support more op (currently only eq */
#define OuterOp(op,x,y) (op==0?(x)==(y):-1)
L pfnOuter(V z, V x, V y, FUNC2(foo)){
    if(isTypeGroupReal(vp(x)) && isTypeGroupReal(vp(y))){
        L lenX    = vn(x), lenY = vn(y);
        L typMax  = MAX(vp(x),vp(y));
        L typCell = -1, op = -1;
        if(foo == &pfnEq){ typCell = H_B; op = 0; }
        else R E_DOMAIN;
        V tempX = allocNode();
        V tempY = allocNode();
        CHECKE(promoteValue(tempX, x, typMax));
        CHECKE(promoteValue(tempY, y, typMax));
        P("total size = %lld\n", lenX * lenY);
        initV(z,H_G,lenX);
        DOI(lenX, initV(vV(z,i),typCell,lenY))
        switch(typCell){
            caseB {
                switch(typMax){
                    caseB DOI(lenX, {V t=vV(z,i); DOJ(lenY, vB(t,j)=OuterOp(op,vB(tempX,i),vB(tempY,j)))}) break;
                    caseJ DOI(lenX, {V t=vV(z,i); DOJ(lenY, vB(t,j)=OuterOp(op,vJ(tempX,i),vJ(tempY,j)))}) break;
                    caseH DOI(lenX, {V t=vV(z,i); DOJ(lenY, vB(t,j)=OuterOp(op,vH(tempX,i),vH(tempY,j)))}) break;
                    caseI DOI(lenX, {V t=vV(z,i); DOJ(lenY, vB(t,j)=OuterOp(op,vI(tempX,i),vI(tempY,j)))}) break;
                    caseL DOI(lenX, {V t=vV(z,i); DOJ(lenY, vB(t,j)=OuterOp(op,vL(tempX,i),vL(tempY,j)))}) break;
                    caseF DOI(lenX, {V t=vV(z,i); DOJ(lenY, vB(t,j)=OuterOp(op,vF(tempX,i),vF(tempY,j)))}) break;
                    caseE DOI(lenX, {V t=vV(z,i); DOJ(lenY, vB(t,j)=OuterOp(op,vE(tempX,i),vE(tempY,j)))}) break;
                    caseC DOI(lenX, {V t=vV(z,i); DOJ(lenY, vB(t,j)=OuterOp(op,vC(tempX,i),vC(tempY,j)))}) break;
                    default: R E_DOMAIN;
                }
            } break;
            default: R E_DOMAIN;
        }
    }
    else R E_DOMAIN;
    R 0;
}

L pfnJoinIndex(V z, V x, V y, FUNC2(foo)){
    P("typ: x = %lld, y = %lld\n", vp(x), vp(y));
    L typCell = -1, op = -1, lenZ = 2;
    if(foo == &pfnEq){ typCell = H_B; op = 0; }
    else R E_DOMAIN;
    P("1,order_x = %d, order_y = %d\n",isOrdered(x),isOrdered(y));
    if(isOrdered(x)){
        R joinIndexHash(z,x,y,'l');
    }
    else if(isOrdered(y)){
        R joinIndexHash(z,x,y,'r');
    }
    else {
        if(isTypeGroupReal(vp(x)) && isTypeGroupReal(vp(y))){
            L lenX    = vn(x), lenY = vn(y);
            L typMax  = MAX(vp(x),vp(y));
            V tempX = allocNode();
            V tempY = allocNode();
            CHECKE(promoteValue(tempX, x, typMax));
            CHECKE(promoteValue(tempY, y, typMax));
            initV(z,H_G,lenZ);
            if(foo == &pfnEq){
                switch(typMax){
                    caseH
                    caseI
                    caseL
                    caseF
                    caseE R lib_join_index_hash(vV(z,0),vV(z,1),tempX,tempY);
                    default: R E_NOT_IMPL;
                }
            }
            else { // brutal force
                L c = 0;
                P("3,x=%lld, y=%lld\n",lenX,lenY);
                switch(typCell){
                    caseB {
                        switch(typMax){
                            caseB DOI(lenX, DOJ(lenY, if(OuterOp(op,vB(tempX,i),vB(tempY,j)))c++)); break;
                            caseJ DOI(lenX, DOJ(lenY, if(OuterOp(op,vJ(tempX,i),vJ(tempY,j)))c++)); break;
                            caseH DOI(lenX, DOJ(lenY, if(OuterOp(op,vH(tempX,i),vH(tempY,j)))c++)); break;
                            caseI DOI(lenX, DOJ(lenY, if(OuterOp(op,vI(tempX,i),vI(tempY,j)))c++)); break;
                            caseL DOI(lenX, DOJ(lenY, if(OuterOp(op,vL(tempX,i),vL(tempY,j)))c++)); break;
                            caseF DOI(lenX, DOJ(lenY, if(OuterOp(op,vF(tempX,i),vF(tempY,j)))c++)); break;
                            caseE DOI(lenX, DOJ(lenY, if(OuterOp(op,vE(tempX,i),vE(tempY,j)))c++)); break;
                            caseC DOI(lenX, DOJ(lenY, if(OuterOp(op,vC(tempX,i),vC(tempY,j)))c++)); break;
                            default: EP("add more types for %lld\n", vp(x), vp(y));
                        }
                    } break;
                    default: R E_DOMAIN;
                }
                P("4, c = %lld\n",c);
                DOI(lenZ, initV(vV(z,i),H_L,c))
                    // assign value
                c = 0;
                switch(typCell){
                    caseB {
                        switch(typMax){
                            caseB DOI(lenX, DOJ(lenY, if(OuterOp(op,vB(tempX,i),vB(tempY,j))){vL(vV(z,0),c)=i;vL(vV(z,1),c)=j;c++;})); break;
                            caseJ DOI(lenX, DOJ(lenY, if(OuterOp(op,vJ(tempX,i),vJ(tempY,j))){vL(vV(z,0),c)=i;vL(vV(z,1),c)=j;c++;})); break;
                            caseH DOI(lenX, DOJ(lenY, if(OuterOp(op,vH(tempX,i),vH(tempY,j))){vL(vV(z,0),c)=i;vL(vV(z,1),c)=j;c++;})); break;
                            caseI DOI(lenX, DOJ(lenY, if(OuterOp(op,vI(tempX,i),vI(tempY,j))){vL(vV(z,0),c)=i;vL(vV(z,1),c)=j;c++;})); break;
                            caseL DOI(lenX, DOJ(lenY, if(OuterOp(op,vL(tempX,i),vL(tempY,j))){vL(vV(z,0),c)=i;vL(vV(z,1),c)=j;c++;})); break;
                            caseF DOI(lenX, DOJ(lenY, if(OuterOp(op,vF(tempX,i),vF(tempY,j))){vL(vV(z,0),c)=i;vL(vV(z,1),c)=j;c++;})); break;
                            caseE DOI(lenX, DOJ(lenY, if(OuterOp(op,vE(tempX,i),vE(tempY,j))){vL(vV(z,0),c)=i;vL(vV(z,1),c)=j;c++;})); break;
                            caseC DOI(lenX, DOJ(lenY, if(OuterOp(op,vC(tempX,i),vC(tempY,j))){vL(vV(z,0),c)=i;vL(vV(z,1),c)=j;c++;})); break;
                        }
                    } break;
                    default: R E_DOMAIN;
                }
            }
        }
        else if(isSameType(x,y)){
            L typ  = vp(x);
            L lenX = vn(x), lenY = vn(y);
            initV(z,H_G,lenZ);
            L c = 0;
            if(foo == &pfnEq){
                switch(typ){
                    caseQ R lib_join_index_hash(vV(z,0),vV(z,1),x,y);
                    default: R E_NOT_IMPL;
                }
            }
            else { // brutal force
                switch(typCell){
                    caseB {
                        switch(typ){
                            caseQ DOI(lenX, DOJ(lenY, if(OuterOp(op, vQ(x,i),vQ(x,j)))c++)); break;
                            default: EP("add more types for %lld\n", vp(x), vp(y));
                        }
                    } break;
                    default: R E_DOMAIN;
                }
                DOI(lenZ, initV(vV(z,i),H_L,c))
                switch(typCell){
                    caseB {
                        switch(typ){
                            caseQ DOI(lenX, DOJ(lenY, if(OuterOp(op, vQ(x,i),vQ(x,j))){vL(vV(z,0),c)=i; vL(vV(z,1),c)=j; c++;})); break;
                        }
                    } break;
                    default: R E_DOMAIN;
                }
            }
        }
        else R E_TYPE;
    }
    R 0;
}

/* Literals */

L pfnDictTable(V z, V x, V y, L op){
    if(isSymbol(x) && isList(y)){
        if(isEqualLength(x,y)){
            L lenZ = 2;
            L typZ = 0==op?H_N:H_A;
            initV(z,typZ,lenZ);
            CHECKE(copyV(getTableKeys(z),x));
            CHECKE(copyV(getTableVals(z),y));
            R 0;
        }
        else R E_LENGTH;
    }
    else R E_DOMAIN;
}

L pfnDict(V z, V x, V y){
    CHECKE(pfnDictTable(z,x,y,0));
    dictNum(z) = vn(x);
    R 0;
}

L pfnTable(V z, V x, V y){
    if(isSymbol(x) && isList(y)){
        // DOI(vn(x), if(!isSymbol(vV(x,i)))R E_DOMAIN)
        DOI(vn(y), if(!isTypeGroupColumn(vp(vV(y,i))))R E_DOMAIN)
        CHECKE(pfnDictTable(z,x,y,1));
        tableRow(z) = (0>=vn(z))?0:vn(getTableCol(getTableVals(z),0));
        tableCol(z) = vn(x);
        R 0;
    }
    else R E_DOMAIN;
}

/*
 * pseudo version
 * x: basic type / list
 */
L pfnEnum(V z, V x, V y){
    if(isTypeGroupBasic(vp(x))){
        V eKey = x;
        V eVal = allocNode();
        CHECKE(pfnIndexOf(eVal,eKey,y));
        L lenZ = vn(eVal);
        DOI(lenZ, if(vL(eVal,i)>=vn(eKey))R E_ENUM_INDEX)
        initV(z,H_Y,lenZ);
        initEnum(z,-1,eKey,vg(eVal));
    }
    else if(isList(x) && isList(y) && isEqualLength(x,y)){
        CHECKE(checkLength(x));
        CHECKE(checkLength(y)); /* same length */
        V eKey = x;
        V eVal = allocNode();
        CHECKE(pfnIndexOf(eVal,eKey,y));
        L lenZ = vn(eVal);
        L lenX = vn(x)>0?vn(vV(x,0)):0;
        DOI(lenZ, if(vL(eVal,i)>=lenX)R E_ENUM_INDEX)
        initV(z,H_Y,lenZ);
        initEnum(z,-1,eKey,vg(eVal));
    }
    else R E_DOMAIN;
    R 0;
}

/* right version */
// L pfnEnum(V z, V x, V y){
//     if(isOneSymbol(x)){
//         V eKey = getValueFromSymbol(vq(x));
//         if(isTypeGroupBasic(vp(eKey))){
//             V eVal = allocNode();
//             CHECKE(pfnIndexOf(eVal,eKey,y));
//             L lenZ = vn(eVal);
//             DOI(lenZ, if(vL(eVal,i)>=vn(eKey))R E_ENUM_INDEX)
//             initV(z,H_Y,lenZ);
//             initEnum(z,vq(x),eKey,vg(eVal));
//             R 0;
//         }
//         else if(isKTable(eKey)){
//             if(isKeySingle(x)){
//                 V keyCol = getTableCol(eKey, 0);
//                 V eVal   = allocNode();
//                 CHECKE(pfnIndexOf(eVal,keyCol,y));
//                 L lenZ   = vn(eVal);
//                 DOI(lenZ, if(vL(eVal,i)>=vn(keyCol))R E_ENUM_INDEX)
//                 initV(z,H_Y,lenZ);
//                 initEnum(z,vq(x),keyCol,vg(eVal));
//                 R 0;
//             }
//             else R E_NOT_IMPL;
//         }
//         else R E_DOMAIN;
//     }
//     else if(2==vn(x)){
//         V eKey = getValueFromSymbol(vQ(x,0));
//         if(isKTable(eKey)){
//             V keyCol;
//             CHECKE(findColFromTable2(&keyCol,eKey,vQ(x,1)));
//             V eVal = allocNode();
//             CHECKE(pfnIndexOf(eVal,keyCol,y));
//             L lenZ = vn(eVal);
//             DOI(lenZ, if(vL(eVal,i)>=vn(keyCol))R E_ENUM_INDEX)
//             initV(z,H_Y,lenZ);
//             initEnum(z,vQ(x,0),keyCol,vg(eVal));
//             R 0;
//         }
//         else R E_DOMAIN;
//     }
//     else R E_DOMAIN;
// }

L pfnKTable(V z, V x, V y){
    if(isTable(x) && isTable(y)){
        /* Todo: check key table */
        if(tableRow(x) == tableRow(y)){
            initKTable(z);
            CHECKE(copyV(getKTableKey(z),x));
            CHECKE(copyV(getKTableVal(z),y));
            tableRow(z) = tableRow(x);
            tableCol(z) = tableCol(x)+tableCol(y);
            R 0;
        }
        else R E_MATCH;
    }
    else R E_DOMAIN;
}


#define MEMBER(t,z,x,y) case##t CHECKE(lib_member_##t(sB(z),s##t(x),vn(x),s##t(y),vn(y))); break
L pfnMember(V z, V x, V y){
    //P("vp(x) = %d, vp(y) = %d\n", vp(x),vp(y));
    if(isTypeGroupReal(vp(x))){
        V tempX = allocNode();
        V tempY = allocNode();
        L typMax = MAX(vp(x),vp(y));
        CHECKE(promoteValue(tempX,x,typMax));
        CHECKE(promoteValue(tempY,y,typMax));
        initV(z,H_B,vn(y));
        switch(typMax){
            MEMBER(B,z,tempX,tempY);
            MEMBER(H,z,tempX,tempY);
            MEMBER(I,z,tempX,tempY);
            MEMBER(L,z,tempX,tempY);
            MEMBER(F,z,tempX,tempY);
            MEMBER(E,z,tempX,tempY);
        }
    }
    else if(isTypeGroupString(vp(x)) || isComplex(x) || isTypeGroupDTime(vp(x))){
        initV(z,H_B,vn(y));
        switch(vp(x)){
            MEMBER(X,z,x,y);
            MEMBER(Q,z,x,y);
            MEMBER(C,z,x,y);
            MEMBER(S,z,x,y);
            MEMBER(M,z,x,y); /* time */
            MEMBER(D,z,x,y);
            MEMBER(Z,z,x,y);
            MEMBER(U,z,x,y);
            MEMBER(W,z,x,y);
            MEMBER(T,z,x,y);
        }
    }
    else R E_DOMAIN;
    R 0;
}

/*
 * rho
 */
#define RHO(t) case##t DOP(lenZ, v##t(z,i)=v##t(y,i%lenY)) break;
#define RHO_LIST(t) case##t DOI(lenZ, CHECKE(copyV(vV(z,i), vV(y,i%lenY)))) break;
L pfnVector(V z, V x, V y){
    if(isOneInt(x) && (isTypeGroupBasic(vp(y)) || isList(y))){
        L typZ = vp(y), lenZ = 0, lenY = vn(y);
        switch(vp(x)){
            caseB lenZ=xb; break;
            caseJ lenZ=xj; break;
            caseH lenZ=xh; break;
            caseI lenZ=xi; break;
            caseL lenZ=xl; break;
        }
        initV(z,typZ,lenZ);
        switch(typZ){
            RHO(B); RHO(J); RHO(H); RHO(I); RHO(L);
            RHO(F); RHO(E); RHO(X);
            RHO(S); RHO(Q); RHO(C);
            RHO(M); RHO(D); RHO(Z); RHO(U); RHO(W); RHO(T);
            RHO_LIST(G);
            default: R E_NOT_IMPL; break;
        }
        R 0;
    }
    else R E_DOMAIN;
}

/*
 * datetime_add(d, 1:64, `year)
 */
L pfnDatetime(V z, V x, V y, V m, L op){
    if(isTypeGroupDate(vp(x)) && isTypeGroupInt(vp(y)) && isOneSymbol(m)){
        I dop = getDatetimeOp(vq(m));
        if(isOne(x)){
            L typZ = vp(x);
            L lenZ = vn(y);
            V tempY = allocNode();
            CHECKE(promoteValue(tempY, y, H_L));
            initV(z,typZ,lenZ);
            switch(vp(x)){
                caseD DOP(lenZ, vD(z,i)=calcDate(vd(x),vL(tempY,i),dop,op)) break;
                default: R E_NOT_IMPL;  /* caseM, caseZ */
            }
        }
        else if(isOne(y)){
            L typZ = vp(x);
            L lenZ = vn(x);
            V tempY = allocNode();
            CHECKE(promoteValue(tempY, y, H_L));
            initV(z,typZ,lenZ);
            switch(vp(x)){
                caseD DOP(lenZ, vD(z,i)=calcDate(vD(x,i),vl(tempY),dop,op)) break;
                default: R E_NOT_IMPL;  /* caseM, caseZ */
            }
        }
        else if(isEqualLength(x,y)){
            L typZ = vp(x);
            L lenZ = vn(x);
            V tempY = allocNode();
            CHECKE(promoteValue(tempY, y, H_L));
            initV(z,typZ,lenZ);
            switch(vp(x)){
                caseD DOP(lenZ, vD(z,i)=calcDate(vD(x,i),vL(tempY,i),dop,op)) break;
                default: R E_NOT_IMPL;  /* caseM, caseZ */
            }
        }
        else R E_LENGTH;
    }
    else R E_DOMAIN;
    R 0;
}

L pfnDatetimeAdd(V z, V x, V y, V m){
    R pfnDatetime(z,x,y,m,0);
}

L pfnDatetimeSub(V z, V x, V y, V m){
    R pfnDatetime(z,x,y,m,1);
}


/*
 * xKey: primary key
 * yKey: foreign key
 */
L pfnAddFKey(V x, V xKey, V y, V yKey){
    V xCol   = allocNode();
    V yCol   = allocNode();
    V fKey   = allocNode();
    V tableX = fetchTableByName(x); // pfnLoadTable: copy item
    CHECKE(tableX==NULL);
    V tableY = fetchTableByName(y); // fetchTableByName: return alias
    CHECKE(tableY==NULL);
    if(isSymbol(xKey) && isSameType(xKey,yKey) && isEqualLength(xKey,yKey)){
        CHECKE(fetchColumnValue(xCol, tableX, xKey, 0));
        CHECKE(fetchColumnValue(yCol, tableY, yKey, 0));
        if(vn(xKey) > 1){
            P(" keys: \n");
            printV(xKey);
            printV(yKey);
            P(" values: \n");
            printV(xCol);
            printV(yCol);
            getchar();
        }
        CHECKE(pfnEnum(fKey, xCol, yCol));
        CHECKE(setFKey(tableY, yKey, fKey));
        if(isOne(xKey)){
            P("Added fkeys successfully: %s.%s (key) -> %s.%s (fkey)\n", \
                    getSymbolStr(vq(x)), getSymbolStr(vq(xKey)),\
                    getSymbolStr(vq(y)), getSymbolStr(vq(yKey)));
        }
        else if(vn(xKey)>1) {
            P("Added fkeys successfully: %s.", getSymbolStr(vq(x)));
            printValue(xKey);
            P(" (key) -> %s.", getSymbolStr(vq(y)));
            printValue(yKey);
            P(" (fkey)\n");
        }
        //else R E_NOT_IMPL;
    }
    else R E_DOMAIN;
    R 0;
}

L pfnSubString(V z, V x, V y){
    if(isString(x) && isInteger(y)){
        if(2==vn(y)){
            L start = vL(y,0); // starting position
            L seg   = vL(y,1); // length of segment
            initV(z,H_S,vn(x));
            // warning: must not DOI -> DOP, because of race condition in allocStrMem
            DOI(vn(x), {S t=vS(x,i); L t0=strlen(t); S p=allocStrMem(seg); \
                       L space = t0 - start; L len=seg<space?seg:space;\
                       DOJ(len, p[j]=t[start+j-1]) while(len<seg) p[len++]=' ';\
                       p[seg]=0; vS(z,i)=p; })
            R 0;
        }
        else R E_NOT_IMPL;
    }
    else R E_DOMAIN;
}

/* handcraft optimization for UDFs: BS and PR */
/* status: on */
// L optLoopFusionBS_1(V z, L r0, V volatility, V time){
//     initV(z,H_E,r0);
//     DOP(r0, vE(z,i)=vE(volatility,i)*sqrt(vE(time,i)))
//     R 0;
// }
// 
// L optLoopFusionBS_2(V z, L r0, V sptprice, V strike, V time, V rate, V volatility){
//     initV(z,H_E,r0);
//     DOP(r0, vE(z,i)=log(vE(sptprice,i)/vE(strike,i)) \
//         + vE(time,i)*(vE(rate,i) + vE(volatility,i)*vE(volatility,i)*0.5))
//     R 0;
// }
// 
// L optLoopFusionBS_3(V z, L r0, V sptprice, L id){
//     initV(z,H_B,r0);
//     if(id <= 3){
//         DOP(r0, vB(z,i)=(vE(sptprice,i)>=50) && (vE(sptprice,i)<=100))
//     }
//     else if(id <= 6){
//         DOP(r0, vB(z,i)=(vE(sptprice,i)<50) || (vE(sptprice,i)>100))
//     }
//     else {
//         DOP(r0, vB(z,i)=(vE(sptprice,i)>=51) && (vE(sptprice,i)<=100))
//     }
//     R 0;
// }
// 
// L optLoopFusionBS_4(V z, L r0, V sptprice, V optionprice){
//     initV(z,H_B,r0);
//     // DOP(r0, vB(z,i)=((vE(sptprice,i)<51) || (vE(sptprice,i)>100)) && (vE(optionprice,i)>15))
//     DOP(r0, vB(z,i)=((vE(sptprice,i)<51) || (vE(sptprice,i)>100)) || (vE(optionprice,i)<=15))
//     R 0;
// }
// 
// L optLoopFusionPR_1(V m0, V m1, L r0, V web){
//     initV(m0, H_L, r0);
//     initV(m1, H_L, r0);
//     memset(sL(m1), 0 , sizeof(L)*r0);
//     DOJ(r0, {V x=vV(web,j); {L t=0; DOI(vn(x), t+=vL(x,i)) vL(m0,j)=t;\
//              DOI(vn(x), vL(m1,i)+=vL(x,i))} })
//     // DOJ(r0, {V x=vV(web,j); {L t=0; DOP(vn(x), t+=vL(x,i), reduction(+:t)) vL(m0,j)=t;\
//     //          DOI(vn(x), vL(m1,i)+=vL(x,i))} }) // slow with DOP
//     R 0;
// }
// 
// L optLoopFusionPR_2(V m0, V m1, V m2, L r0, V web, L id){
//     L *cnt = (L*)malloc(sizeof(L)*r0); memset(cnt, 0, sizeof(L)*r0);
//     L *indx= (L*)malloc(sizeof(L)*r0); memset(indx,0, sizeof(L)*r0);
//     DOI(r0, { DOJ(r0, cnt[j] +=vL(vV(web,i),j)) } )
//     L tot = 0;
//     if(id<=3) {
//         DOI(r0, if(cnt[i]>70)indx[tot++]=i)
//     }
//     else if(id<=6){
//         DOI(r0, if(cnt[i]<=70)indx[tot++]=i)
//     }
//     else {
//         DOI(r0, if(cnt[i]>54)indx[tot++]=i)
//     }
//     initV(m0, H_L, tot);
//     initV(m1, H_L, tot);
//     initV(m2, H_L, tot);
//     DOI(tot, vL(m0,i)=indx[i])
//     DOI(tot, vL(m2,i)=cnt[indx[i]])
//     DOI(tot, {V x=vV(web,indx[i]); {L t=0; DOJ(vn(x), t+=vL(x,j)) vL(m1,i)=t;}})
//     free(cnt);
//     free(indx);
//     R 0;
// }
// 
// 
// L optLoopFusionPR_4(V w5, L r0, V w2){
//     initV(w5, H_L, r0);
//     DOI(r0, vL(w5,vL(w2,i))=i)
//     R 0;
// }
// 
// L optLoopFusionPR_5(V w8, L r0, V w1, V w5, L id){
//     initV(w8, H_B, r0);
//     if(id <= 3){
//         DOI(r0, vB(w8,i)=vL(w1,i)>70&&vL(w5,i)<100)
//     }
//     else {
//         // DOI(r0, vB(w8,i)=vL(w1,i)<=70&&vL(w5,i)<100)
//         DOI(r0, vB(w8,i)=vL(w1,i)<=70||vL(w5,i)>=100)
//     }
//     R 0;
// }
// 
// L optLoopFusionPR_6(V m2, L r0, V w4, V m3){
//     initV(m2, H_L, r0);
//     DOI(r0, vL(m2,vL(w4,i))=vL(m3,i))
//     R 0;
// }

// /* q18: bucket group by */
// L pfnGroupBucket(V z, V x){
//     if(isInteger(x)){
//         L lenX = -1;
//         DOI(vn(x), lenX = MAX(lenX, vL(x,i))) lenX++;
//         // P("lenX 1 = %lld\n", lenX);
//         L *temp  = (L*)malloc(sizeof(L)*lenX);
//         L *count = (L*)malloc(sizeof(L)*lenX);
//         L *maps  = (L*)malloc(sizeof(L)*lenX);
//         memset(temp , 0, sizeof(L)*lenX);
//         memset(count, 0, sizeof(L)*lenX);
//         memset(maps ,-1, sizeof(L)*lenX);
//         DOI(vn(x), temp[vL(x,i)]++)
//         L cnt = 0;
//         DOI(lenX, cnt+= temp[i]!=0)
//         initV(z, H_N, 2);
//         V keys = getDictKeys(z);
//         V vals = getDictVals(z);
//         // P("size of keys: %lld\n", cnt);
//         initV(keys, H_L, cnt); cnt = 0;
//         DOI(lenX, if(temp[i]!=0) {vL(keys,cnt)=i; maps[i]=cnt++; })
//         initV(vals, H_G, cnt);
//         DOI(vn(x), {L k=vL(x,i); V v0=vV(vals,maps[k]); if(count[k]==0) initV(v0,H_L,temp[k]); vL(v0,count[k]++)=i; })
//         free(temp);
//         free(count);
//         free(maps);
//         R 0;
//     }
//     else R E_DOMAIN;
// }
// 
// 
// L optLoopFusionQ18_1(V z, V x, V t1){
//     if(isInteger(x)){
//         L lenX = -1;
//         // lenX = vn(x);
//         DOI(vn(x), lenX = MAX(lenX, vL(x,i))) lenX++;
//         // P("lenX 2 = %lld\n", lenX);
//         L *temp  = (L*)malloc(sizeof(L)*lenX);
//         L *maps  = (L*)malloc(sizeof(L)*lenX);
//         memset(temp , 0, sizeof(L)*lenX);
//         memset(maps ,-1, sizeof(L)*lenX);
//         DOI(vn(x), temp[vL(x,i)]++)
//         L cnt = 0;
//         DOI(lenX, cnt+= temp[i]!=0)
//         initV(z, H_N, 2);
//         V keys = getDictKeys(z);
//         V vals = getDictVals(z);
//         // P("size of keys: %lld\n", cnt);
//         initV(keys, H_L, cnt); cnt = 0;
//         DOI(lenX, if(temp[i]!=0) {vL(keys,cnt)=i; maps[i]=cnt++; })
//         initV(vals, H_E, cnt);
//         memset(sE(vals), 0, sizeof(E)*cnt);  // clean before sum up
//         DOI(vn(x), {L k=vL(x,i); L c=maps[k]; vE(vals,c)+=vE(t1,i); })
//         free(temp);
//         free(maps);
//         R 0;
//     }
//     else R E_DOMAIN;
// }

/*
 * two columns: (row 650K)
 *   A, N, R
 *   O, F
 */

#define ENCODE2(x,y) ((x<<8)+y)

L pfnGroupTrie(V z, V x){
    if(isList(x) && vn(x)==2){
        initV(z, H_N, 2);
        V keys = getDictKeys(z);
        V vals = getDictVals(z);
        L charMap[65536]={0}; // 256*256
        L charId[65536]={-1};
        L lenX = vn(vV(x,0));
    // struct timeval tv0, tv1;
    // gettimeofday(&tv0, NULL);
        DOI(lenX, {C ch0=vC(vV(x,0),i); C ch1=vC(vV(x,1),i); L t=ENCODE2(ch0,ch1); charMap[t]++; })
    // gettimeofday(&tv1, NULL);
    // P("1. The elapsed time (ms): %g\n\n", calcInterval(tv0,tv1)/1000.0);
        L lenZ = 0; DOI(65536, if(charMap[i]!=0)charId[i]=lenZ++)
        // P("val of lenZ: %lld\n", lenZ);
        initV(vals, H_G, lenZ);
        L c=0; DOI(65536, if(charMap[i]!=0){ initV(vV(vals,c++),H_L,charMap[i]); })
        L *count = (L*)malloc(sizeof(L)*lenZ); memset(count, 0, sizeof(L)*lenZ);
    // gettimeofday(&tv0, NULL);
        DOI(lenX, {C ch0=vC(vV(x,0),i); C ch1=vC(vV(x,1),i); L t=ENCODE2(ch0,ch1); \
                   L id=charId[t]; vL(vV(vals,id),count[id]++)=i; })
    // gettimeofday(&tv1, NULL);
    // P("2. The elapsed time (ms): %g\n\n", calcInterval(tv0,tv1)/1000.0);
        initV(keys, H_L, lenZ);
        DOI(lenZ, vL(keys,i)=vL(vV(vals,i),0))
        free(count);
        R 0;
    }
    else R E_NOT_IMPL;
}






