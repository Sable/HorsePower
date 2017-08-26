#include "h_global.h"
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

L pfnLoadTable(V z, V x){
    if(isSymbol(x)){
        V t = findTableByName(vq(x));
        if(!t) R E_TABLE_NOT_FOUND;
        *z = *t;
        R 0;
    }
    else R E_DOMAIN;
}

/* copy alias */
L pfnList(V z, L n, V* x){
    initList(z, n);
    DOI(n, *vV(z,i)=*x[i])
    R 0;
}


/*
 * indexing: x[y]
 */
L pfnIndex(V z, V x, V y){
    if(H_DEBUG) P("-> Entering index\n");
    if(isTypeGroupInt(vp(y))){
        L typZ = vp(x), lenZ = vn(y), lenX = vn(x);
        V tempY = allocNode();
        CHECKE(promoteValue(tempY, y, H_L));
        DOI(lenZ, if(lenX <= vL(tempY,i))R E_INDEX)
        if(isTypeGroupBasic(vp(x))){
            initV(z, typZ, lenZ);
            switch(vp(x)){
                caseB DOI(lenZ, vB(z,i)=vB(x,vL(y,i))) break;
                caseH DOI(lenZ, vH(z,i)=vH(x,vL(y,i))) break;
                caseI DOI(lenZ, vI(z,i)=vI(x,vL(y,i))) break;
                caseL DOI(lenZ, vL(z,i)=vL(x,vL(y,i))) break;
                caseF DOI(lenZ, vF(z,i)=vF(x,vL(y,i))) break;
                caseE DOI(lenZ, vE(z,i)=vE(x,vL(y,i))) break;
                caseX DOI(lenZ, vX(z,i)=vX(x,vL(y,i))) break;
                caseQ DOI(lenZ, vQ(z,i)=vQ(x,vL(y,i))) break;
                caseS DOI(lenZ, vS(z,i)=vS(x,vL(y,i))) break;
            }
            R 0;
        }
        else R E_NOT_IMPL;
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
    initTable(z, vn(x));
    /* need to check dict */
    DOI(vn(x), *(sV(z)+i)=*(sV(x)+i))
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
    if(isTable(x) || isDict(x)){
        L lenZ = vn(x);
        initV(z,H_G,lenZ);
        DOI(lenZ, CHECKE(copyV(vV(z,i),getColKey(vV(x,i)))))
        R 0;
    }
    else if(isKTable(x)){
        V keyTable = getKTableKey(x);
        V valTable = getKTableVal(x);
        L lenKey = vn(keyTable);
        L lenVal = vn(valTable);
        L lenZ = lenKey + lenVal, c = 0;
        initV(z,H_G,lenZ);
        DOI(lenKey, CHECKE(copyV(vV(z,i       ),getColKey(vV(x,i)))))
        DOI(lenVal, CHECKE(copyV(vV(z,i+lenKey),getColKey(vV(x,i)))))
        R 0;
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

L pfnColumnValue(V z, V x, V y){
    if(H_DEBUG) P("-> Entering column_value\n");
    if(isTable2(x) && isOneSymbol(y)){
        L colId = vq(y);
        L colIndex = findColFromTable(x,colId);
        if(colIndex >= 0){
            R copyColumnValue(z, getColVal(getTableCol(x,colIndex)));
        }
        else R E_COL_NOT_FOUND;
    }
    else R E_DOMAIN;
}

L pfnIsValidBranch(V z, V x){
    B t = 0;
    initV(z, H_B, 1);
    if(isOne(x)){
        switch(xp){
            caseB t = xb; break;
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
#define NOT(x) (~x)
#define POWER(x,y) pow(x,y)
#define LOG(x,y) (log(y)/log(x))
#define FACT(n) calcFact(n)

/* abs */
L pfnAbs(V z, V x){
    if(isTypeGroupReal(vp(x))){
        initV(z,vp(x),vn(x));  // Step 1: initialize z
        switch(vp(x)){         // Step 2: based on x
            caseB DOI(vn(x), vB(z,i)=vB(x,i))      break; //opt
            caseH DOI(vn(x), vH(z,i)=ABS(vH(x,i))) break;
            caseI DOI(vn(x), vI(z,i)=ABS(vI(x,i))) break;
            caseL DOI(vn(x), vL(z,i)=ABS(vL(x,i))) break;
            caseF DOI(vn(x), vF(z,i)=ABS(vF(x,i))) break;
            caseE DOI(vn(x), vE(z,i)=ABS(vE(x,i))) break;
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
            caseB DOI(lenZ, vB(z,i)=NEG(vB(x,i))) break;
            caseH DOI(lenZ, vH(z,i)=NEG(vH(x,i))) break;
            caseI DOI(lenZ, vI(z,i)=NEG(vI(x,i))) break;
            caseL DOI(lenZ, vL(z,i)=NEG(vL(x,i))) break;
            caseF DOI(lenZ, vF(z,i)=NEG(vF(x,i))) break;
            caseE DOI(lenZ, vE(z,i)=NEG(vE(x,i))) break;
        }
        R 0;
    }
    else R E_DOMAIN;
}

L pfnCeil(V z, V x){
    if(isTypeGroupReal(vp(x))){
        L typZ = inferReal2Int(vp(x));
        L lenZ = vn(x);
        initV(z,typZ,lenZ);
        switch(vp(x)){
            caseB DOI(lenZ, vB(z,i)=vB(x,i)) break; //merely copy (opt)
            caseH DOI(lenZ, vH(z,i)=vH(x,i)) break;
            caseI DOI(lenZ, vI(z,i)=vI(x,i)) break;
            caseL DOI(lenZ, vL(z,i)=vL(x,i)) break;
            caseF DOI(lenZ, vL(z,i)=CEIL(vF(x,i))) break;
            caseE DOI(lenZ, vL(z,i)=CEIL(vE(x,i))) break;
        }
        R 0;
    }
    else R E_DOMAIN;
}

L pfnFloor(V z, V x){
    if(isTypeGroupReal(vp(x))){
        L typZ = inferReal2Int(vp(x));
        L lenZ = vn(x);
        initV(z,typZ,lenZ);
        switch(vp(x)){
            caseB DOI(lenZ, vB(z,i)=vB(x,i)) break; //merely copy (opt)
            caseH DOI(lenZ, vH(z,i)=vH(x,i)) break;
            caseI DOI(lenZ, vI(z,i)=vI(x,i)) break;
            caseL DOI(lenZ, vL(z,i)=vL(x,i)) break;
            caseF DOI(lenZ, vL(z,i)=FLOOR(vF(x,i))) break;
            caseE DOI(lenZ, vL(z,i)=FLOOR(vE(x,i))) break;
        }
        R 0;
    }
    else R E_DOMAIN;
}

L pfnRound(V z, V x){
    if(isTypeGroupReal(vp(x))){
        L typZ = inferReal2Int(vp(x));
        L lenZ = vn(x);
        initV(z,typZ,lenZ);
        switch(vp(x)){
            caseB DOI(lenZ, vB(z,i)=vB(x,i)) break; //merely copy (opt)
            caseH DOI(lenZ, vH(z,i)=vH(x,i)) break;
            caseI DOI(lenZ, vI(z,i)=vI(x,i)) break;
            caseL DOI(lenZ, vL(z,i)=vL(x,i)) break;
            caseF DOI(lenZ, vL(z,i)=ROUND(vF(x,i))) break;
            caseE DOI(lenZ, vL(z,i)=ROUND(vE(x,i))) break;
        }
        R 0;
    }
    else R E_DOMAIN;
}

#define TRIG(op,x) (2>op?TRIGSIN(op,x):4>op?TRIGCOS(op,x):TRIGTAN(op,x))
#define TRIGSIN(op,x) (0==op?sin(x):asin(x))
#define TRIGCOS(op,x) (2==op?cos(x):acos(x))
#define TRIGTAN(op,x) (4==op?tan(x):atan(x))

L pfnTrig(V z, V x, L op){
    if(isTypeGroupReal(vp(x))){
        L typZ = H_E;
        L lenZ = vn(x);
        initV(z,typZ,lenZ);
        switch(vp(x)){
            caseB DOI(lenZ, vE(z,i)=TRIG(op,vB(x,i))) break;
            caseH DOI(lenZ, vE(z,i)=TRIG(op,vH(x,i))) break;
            caseI DOI(lenZ, vE(z,i)=TRIG(op,vI(x,i))) break;
            caseL DOI(lenZ, vE(z,i)=TRIG(op,vL(x,i))) break;
            caseF DOI(lenZ, vE(z,i)=TRIG(op,vF(x,i))) break;
            caseE DOI(lenZ, vE(z,i)=TRIG(op,vE(x,i))) break;
            default: R E_NOT_IMPL;
        }
        R 0;
    }
    else R E_DOMAIN;
}

L pfnTrigSin(V z, V x){
    R pfnTrig(z,x,0);
}

L pfnTrigAsin(V z, V x){
    R pfnTrig(z,x,1);
}

L pfnTrigCos(V z, V x){
    R pfnTrig(z,x,2);
}

L pfnTrigAcos(V z, V x){
    R pfnTrig(z,x,3);
}

L pfnTrigTan(V z, V x){
    R pfnTrig(z,x,4);
}

L pfnTrigAtan(V z, V x){
    R pfnTrig(z,x,5);
}

#define HYPER(op,x) (2>op?HYPERSIN(op,x):4>op?HYPERCOS(op,x):HYPERTAN(op,x))
#define HYPERSIN(op,x) (0==op?sinh(x):asinh(x))
#define HYPERCOS(op,x) (2==op?cosh(x):acosh(x))
#define HYPERTAN(op,x) (4==op?tanh(x):atanh(x))

L pfnHyper(V z, V x, L op){
    if(isTypeGroupReal(vp(x))){
        L typZ = H_E;
        L lenZ = vn(x);
        initV(z,typZ,lenZ);
        switch(vp(x)){
            caseB DOI(lenZ, vE(z,i)=HYPER(op,vB(x,i))) break;
            caseH DOI(lenZ, vE(z,i)=HYPER(op,vH(x,i))) break;
            caseI DOI(lenZ, vE(z,i)=HYPER(op,vI(x,i))) break;
            caseL DOI(lenZ, vE(z,i)=HYPER(op,vL(x,i))) break;
            caseF DOI(lenZ, vE(z,i)=HYPER(op,vF(x,i))) break;
            caseE DOI(lenZ, vE(z,i)=HYPER(op,vE(x,i))) break;
            default: R E_NOT_IMPL;
        }
        R 0;
    }
    else R E_DOMAIN;
}

L pfnHyperSinh(V z, V x){
    R pfnHyper(z,x,0);
}

L pfnHyperAsinh(V z, V x){
    R pfnHyper(z,x,1);
}

L pfnHyperCosh(V z, V x){
    R pfnHyper(z,x,2);
}

L pfnHyperAcosh(V z, V x){
    R pfnHyper(z,x,3);
}

L pfnHyperTanh(V z, V x){
    R pfnHyper(z,x,4);
}

L pfnHyperAtanh(V z, V x){
    R pfnHyper(z,x,5);
}

L pfnConj(V z, V x){
    if(isComplex(x)){
        L typZ = H_X;
        L lenZ = vn(x);
        initV(z,typZ,lenZ);
        DOI(lenZ, {xReal(vX(z,i))=xReal(vX(x,i));xImag(vX(z,i))=-xImag(vX(x,i));})
        R 0;
    }
    else R E_DOMAIN;
}

#define recipSum(t, ptr) { DOI(lenZ, t+=ptr(x,i)); if(0==t) R E_DIV_ZERO; }
L pfnRecip(V z, V x){
    if(isTypeGroupReal(vp(x))){
        L typZ = inferRecip(vp(x));
        L lenZ = vn(x);
        initV(z,typZ,lenZ);
        switch(vp(x)){
            caseB {E t=0; recipSum(t,vB); DOI(lenZ, vE(z,i)=DIVDE(vB(x,i),t)); } break;
            caseH {E t=0; recipSum(t,vH); DOI(lenZ, vE(z,i)=DIVDE(vH(x,i),t)); } break;
            caseI {E t=0; recipSum(t,vI); DOI(lenZ, vE(z,i)=DIVDE(vI(x,i),t)); } break;
            caseL {E t=0; recipSum(t,vL); DOI(lenZ, vE(z,i)=DIVDE(vL(x,i),t)); } break;
            caseF {F t=0; recipSum(t,vF); DOI(lenZ, vF(z,i)=DIVDE(vF(x,i),t)); } break; //F
            caseE {E t=0; recipSum(t,vE); DOI(lenZ, vE(z,i)=DIVDE(vE(x,i),t)); } break;
        }
        R 0;
    }
    else R E_DOMAIN;
}

/* 1 -1 0 */
L pfnSignum(V z, V x){
    if(isTypeGroupReal(vp(x))){
        L typZ = inferSignum(vp(x));
        L lenZ = vn(x);
        initV(z,typZ,lenZ);
        switch(vp(x)){
            caseB DOI(lenZ, vL(z,i)=vB(x,i))         break; //opt, no -1
            caseH DOI(lenZ, vH(z,i)=SIGNUM(vH(x,i))) break;
            caseI DOI(lenZ, vI(z,i)=SIGNUM(vI(x,i))) break;
            caseL DOI(lenZ, vL(z,i)=SIGNUM(vL(x,i))) break;
            caseF DOI(lenZ, vL(z,i)=SIGNUM(vF(x,i))) break;
            caseE DOI(lenZ, vL(z,i)=SIGNUM(vE(x,i))) break;
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
            caseB DOI(lenZ, vE(z,i)=PIMUL(vB(x,i))) break;
            caseH DOI(lenZ, vE(z,i)=PIMUL(vH(x,i))) break;
            caseI DOI(lenZ, vE(z,i)=PIMUL(vI(x,i))) break;
            caseL DOI(lenZ, vE(z,i)=PIMUL(vL(x,i))) break;
            caseF DOI(lenZ, vF(z,i)=PIMUL(vF(x,i))) break; //F
            caseE DOI(lenZ, vE(z,i)=PIMUL(vE(x,i))) break;
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
        DOI(lenZ, vB(z,i)=NOT(vB(x,i)));
        R 0;
    }
    else R E_DOMAIN;
}

L pfnLen(V z, V x){
    initV(z,H_L,1);
    vl(z)= isTable(x)?getTableRowNumber(x):vn(x);
    R 0;
}

L pfnRange(V z, V x){
    if(isOne(x) && isTypeGroupInt(xp)){
        L size = getSingleIntValue(x);
        initV(z,H_L,size);
        DOI(size, vL(z,i)=i) // opt, stride?
        R 0;
    }
    else R E_DOMAIN;
}

L pfnFact(V z, V x){
    if(isTypeGroupInt(xp)){
        initV(z,H_L,xn);
        switch(xp){
            caseB DOI(xn, vL(z,i)=FACT(vB(x,i))) break;
            caseH DOI(xn, vL(z,i)=FACT(vH(x,i))) break;
            caseI DOI(xn, vL(z,i)=FACT(vI(x,i))) break;
            caseL DOI(xn, vL(z,i)=FACT(vL(x,i))) break;
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
            caseB DOI(lenZ, vB(z,i)=vB(x,lenZ-i-1)) break;
            caseH DOI(lenZ, vH(z,i)=vH(x,lenZ-i-1)) break;
            caseI DOI(lenZ, vI(z,i)=vI(x,lenZ-i-1)) break;
            caseL DOI(lenZ, vL(z,i)=vL(x,lenZ-i-1)) break;
            caseF DOI(lenZ, vF(z,i)=vF(x,lenZ-i-1)) break;
            caseE DOI(lenZ, vE(z,i)=vE(x,lenZ-i-1)) break;
            caseX DOI(lenZ, vX(z,i)=vX(x,lenZ-i-1)) break;
            caseC DOI(lenZ, vC(z,i)=vC(x,lenZ-i-1)) break;
            caseQ DOI(lenZ, vQ(z,i)=vQ(x,lenZ-i-1)) break;
            caseS DOI(lenZ, vS(z,i)=vS(x,lenZ-i-1)) break;
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
        DOI(vn(z0), lenZ += vL(z0,i)==i)
        initV(z,typZ,lenZ);
        DOI(vn(x), if(vL(z0,i)==i)vL(z,c++)=i)
        /* free z0 */
        R 0;
    }
    else R E_DOMAIN;
}

L pfnWhere(V z, V x){
    if(isBool(x)){
        L typZ = H_L, lenZ = 0, c = 0;
        DOI(vn(x), lenZ+=vB(x,i))
        initV(z,typZ,lenZ);
        DOI(vn(x), if(vB(x,i))vL(z,c++)=i)
        R 0;
    }
    else R E_DOMAIN;
}

L pfnSum(V z, V x){
    if(isTypeGroupReal(vp(x))){
        L typZ = H_F==vp(x)?H_F:H_E;
        initV(z,H_E,1);
        switch(vp(x)){
            caseB {E t=0; DOI(vn(x), t+=vB(x,i)) ve(z)=t;} break;
            caseH {E t=0; DOI(vn(x), t+=vH(x,i)) ve(z)=t;} break;
            caseI {E t=0; DOI(vn(x), t+=vI(x,i)) ve(z)=t;} break;
            caseL {E t=0; DOI(vn(x), t+=vL(x,i)) ve(z)=t;} break;
            caseF {F t=0; DOI(vn(x), t+=vF(x,i)) vf(z)=t;} break;
            caseE {E t=0; DOI(vn(x), t+=vE(x,i)) ve(z)=t;} break;
        }
        R 0;
    }
    else R E_DOMAIN;
}

#define REDUCELONG(op) (0==op?LLONG_MAX:LLONG_MIN)
#define REDUCEFLT(op)  (0==op?FLT_MAX:FLT_MIN)
#define REDUCEDBL(op)  (0==op?DBL_MAX:DBL_MIN)
#define REDUCE(op,t,x) (0==op?min(t,x):1==op?max(t,x):-1)
#define REDUCELINE(p,op,x) {p t=v##p(x,0); DOI(vn(x), t=REDUCE(op,t,v##p(x,i)))}

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
            }
        }
        else{
            L typZ = vp(x);
            L lenZ = 1;
            initV(z,typZ,lenZ);
            switch(typZ){
                caseB REDUCELINE(B,op,x); break;
                caseH REDUCELINE(H,op,x); break;
                caseI REDUCELINE(I,op,x); break;
                caseL REDUCELINE(L,op,x); break;
                caseF REDUCELINE(F,op,x); break;
                caseE REDUCELINE(E,op,x); break;
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
        initV(z,H_L,vn(x));
        switch(vp(x)){
            caseM DOI(vn(x), vL(z,i)=CHOPM(op,vM(x,i))) break;
            caseD DOI(vn(x), vL(z,i)=CHOPD(op,vD(x,i))) break;
            caseZ DOI(vn(x), CHOPZ(op,vL(z,i),vZ(x,i))) break;
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
        DOI(vn(x), vD(z,i)=(D)Z2D(vZ(x,i)))
        R 0;
    }
    else R E_DOMAIN;
}


L pfnChopTime(V z, V x, L op){
    if(isTypeGroupTime(vp(x))){
        initV(z,H_L,vn(x));
        switch(vp(x)){
            caseU DOI(vn(x), vL(z,i)= CHOPU(op,vU(x,i))) break;
            caseW DOI(vn(x), vL(z,i)= CHOPW(op,vW(x,i))) break;
            caseT DOI(vn(x), CHOPT (op,vL(z,i),vT(x,i))) break;
            caseZ DOI(vn(x), CHOPZT(op,vL(z,i),vZ(x,i))) break;
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
        DOI(vn(x), vT(z,i)=(T)Z2T(vZ(x,i)))
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
        L typZ, lenZ, n;
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

L pfnTolist(V z, V x){
    if(isTypeGroupBasic(xp)){
        initV(z,H_G,xn);
        DOI(xn, { V t=vV(z,i); initV(t,xp,1);\
                switch(xp){ \
                    caseB vb(t)=vB(x,i); break; \
                    caseH vh(t)=vH(x,i); break; \
                    caseI vi(t)=vI(x,i); break; \
                    caseL vl(t)=vL(x,i); break; \
                    caseF vf(t)=vF(x,i); break; \
                    caseE ve(t)=vE(x,i); break; \
                    caseX vx(t)=vX(x,i); break; \
                    caseQ vq(t)=vQ(x,i); break; \
                    caseS vs(t)=vS(x,i); break; \
                    default: R E_NOT_IMPL; \
                } \
            })
        R 0;
    }
    else R E_DOMAIN;
}

L pfnGroup(V z, V x){
    V0 y0,t0; V y = &y0, t = &t0;
    L lenZ = isList(x)?vn(x):1;
    initV(y,H_B,lenZ);
    DOI(vn(x),vB(y,i)=1)
    CHECKE(pfnOrderBy(t,x,y));

    if(isList(x)){
        L numRow= 0==vn(x)?0:vn(vV(x,0));
        CHECKE(lib_get_group_by(z,x,sL(t),numRow));
    }
    else if(isTypeGroupBasic(xp)){
        V0 t1; V tx=&t1;
        L numRow= vn(x);
        CHECKE(pfnEnlist(tx,x));
        CHECKE(lib_get_group_by(z,tx,sL(t),numRow));
    }
    else R E_DOMAIN;
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
            L typMax = max(vp(x),vp(y));
            V tempX = allocNode();
            V tempY = allocNode();
            CHECKE(promoteValue(tempX, x, typMax));
            CHECKE(promoteValue(tempY, y, typMax));
            initV(z,typZ,lenZ);
            if(isOne(x)) {
                switch(typMax){
                    caseB DOI(lenZ, vB(z,i)=COMP(op,vB(tempX,0),vB(tempY,i))) break;
                    caseH DOI(lenZ, vB(z,i)=COMP(op,vH(tempX,0),vH(tempY,i))) break;
                    caseI DOI(lenZ, vB(z,i)=COMP(op,vI(tempX,0),vI(tempY,i))) break;
                    caseL DOI(lenZ, vB(z,i)=COMP(op,vL(tempX,0),vL(tempY,i))) break;
                    caseF DOI(lenZ, vB(z,i)=COMPFN(op,vF(tempX,0),vF(tempY,i),compareFloat)) break;
                    caseE DOI(lenZ, vB(z,i)=COMPFN(op,vE(tempX,0),vE(tempY,i),compareFloat)) break;
                }
            }
            else if(isOne(y)) {
                switch(typMax){
                    caseB DOI(lenZ, vB(z,i)=COMP(op,vB(tempX,i),vB(tempY,0))) break;
                    caseH DOI(lenZ, vB(z,i)=COMP(op,vH(tempX,i),vH(tempY,0))) break;
                    caseI DOI(lenZ, vB(z,i)=COMP(op,vI(tempX,i),vI(tempY,0))) break;
                    caseL DOI(lenZ, vB(z,i)=COMP(op,vL(tempX,i),vL(tempY,0))) break;
                    caseF DOI(lenZ, vB(z,i)=COMPFN(op,vF(tempX,i),vF(tempY,0),compareFloat)) break;
                    caseE DOI(lenZ, vB(z,i)=COMPFN(op,vE(tempX,i),vE(tempY,0),compareFloat)) break;
                }
            }
            else {
                switch(typMax){
                    caseB DOI(lenZ, vB(z,i)=COMP(op,vB(tempX,i),vB(tempY,i))) break;
                    caseH DOI(lenZ, vB(z,i)=COMP(op,vH(tempX,i),vH(tempY,i))) break;
                    caseI DOI(lenZ, vB(z,i)=COMP(op,vI(tempX,i),vI(tempY,i))) break;
                    caseL DOI(lenZ, vB(z,i)=COMP(op,vL(tempX,i),vL(tempY,i))) break;
                    caseF DOI(lenZ, vB(z,i)=COMPFN(op,vF(tempX,i),vF(tempY,i),compareFloat)) break;
                    caseE DOI(lenZ, vB(z,i)=COMPFN(op,vE(tempX,i),vE(tempY,i),compareFloat)) break;
                }
            }
        }
        else if(isSameType(x,y)){
            initV(z,typZ,lenZ);
            if(isOne(x)) {
                switch(vp(x)){
                    caseQ DOI(lenZ, vB(z,i)=COMPFN(op,vQ(x,0),vQ(y,i),compareSymbol)) break;
                    caseS DOI(lenZ, vB(z,i)=COMPFN(op,vS(x,0),vS(y,i),strcmp))        break;
                    caseC DOI(lenZ, vB(z,i)=COMP(op,vC(x,0),vC(y,i))) break;
                    caseM DOI(lenZ, vB(z,i)=COMP(op,vM(x,0),vM(y,i))) break;
                    caseD DOI(lenZ, vB(z,i)=COMP(op,vD(x,0),vD(y,i))) break;
                    caseZ DOI(lenZ, vB(z,i)=COMP(op,vZ(x,0),vZ(y,i))) break;
                    caseU DOI(lenZ, vB(z,i)=COMP(op,vU(x,0),vU(y,i))) break;
                    caseV DOI(lenZ, vB(z,i)=COMP(op,vV(x,0),vV(y,i))) break;
                    caseT DOI(lenZ, vB(z,i)=COMP(op,vT(x,0),vT(y,i))) break;
                }
            }
            else if(isOne(y)) {
                switch(vp(x)){
                    caseQ DOI(lenZ, vB(z,i)=COMPFN(op,vQ(x,i),vQ(y,0),compareSymbol)) break;
                    caseS DOI(lenZ, vB(z,i)=COMPFN(op,vS(x,i),vS(y,0),strcmp))        break;
                    caseC DOI(lenZ, vB(z,i)=COMP(op,vC(x,i),vC(y,0))) break;
                    caseM DOI(lenZ, vB(z,i)=COMP(op,vM(x,i),vM(y,0))) break;
                    caseD DOI(lenZ, vB(z,i)=COMP(op,vD(x,i),vD(y,0))) break;
                    caseZ DOI(lenZ, vB(z,i)=COMP(op,vZ(x,i),vZ(y,0))) break;
                    caseU DOI(lenZ, vB(z,i)=COMP(op,vU(x,i),vU(y,0))) break;
                    caseV DOI(lenZ, vB(z,i)=COMP(op,vV(x,i),vV(y,0))) break;
                    caseT DOI(lenZ, vB(z,i)=COMP(op,vT(x,i),vT(y,0))) break;
                }
            }
            else {
                switch(vp(x)){
                    caseQ DOI(lenZ, vB(z,i)=COMPFN(op,vQ(x,i),vQ(y,i),compareSymbol)) break;
                    caseS DOI(lenZ, vB(z,i)=COMPFN(op,vS(x,i),vS(y,i),strcmp))        break;
                    caseC DOI(lenZ, vB(z,i)=COMP(op,vC(x,i),vC(y,i))) break;
                    caseM DOI(lenZ, vB(z,i)=COMP(op,vM(x,i),vM(y,i))) break;
                    caseD DOI(lenZ, vB(z,i)=COMP(op,vD(x,i),vD(y,i))) break;
                    caseZ DOI(lenZ, vB(z,i)=COMP(op,vZ(x,i),vZ(y,i))) break;
                    caseU DOI(lenZ, vB(z,i)=COMP(op,vU(x,i),vU(y,i))) break;
                    caseV DOI(lenZ, vB(z,i)=COMP(op,vV(x,i),vV(y,i))) break;
                    caseT DOI(lenZ, vB(z,i)=COMP(op,vT(x,i),vT(y,i))) break;
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

#define ARITH2(op,x,y) (0==op?(x+y):1==op?(x-y):2==op?(x*y):3==op?(x/y):0)

L pfnArith(V z, V x, V y, L op){
    if(isTypeGroupReal(vp(x)) && isTypeGroupReal(vp(y))){
        if(!isValidLength(x,y))   R E_LENGTH;
        if(3==op && checkZero(y)) R E_DIV_ZERO;
        L lenZ   = isOne(x)?vn(y):vn(x);
        L typMax = max(vp(x),vp(y));
        L typZ   = typMax;
        V tempX = allocNode();
        V tempY = allocNode();
        CHECKE(promoteValue(tempX, x, typMax));
        CHECKE(promoteValue(tempY, y, typMax));
        initV(z,typZ,lenZ);
        if(isOne(x)) {
            switch(typMax){
                caseB DOI(lenZ, vB(z,i)=ARITH2(op,vB(tempX,0),vB(tempY,i))) break;
                caseH DOI(lenZ, vH(z,i)=ARITH2(op,vH(tempX,0),vH(tempY,i))) break;
                caseI DOI(lenZ, vI(z,i)=ARITH2(op,vI(tempX,0),vI(tempY,i))) break;
                caseL DOI(lenZ, vL(z,i)=ARITH2(op,vL(tempX,0),vL(tempY,i))) break;
                caseF DOI(lenZ, vF(z,i)=ARITH2(op,vF(tempX,0),vF(tempY,i))) break;
                caseE DOI(lenZ, vE(z,i)=ARITH2(op,vE(tempX,0),vE(tempY,i))) break;
                default: R E_NOT_IMPL;
            }
        }
        else if(isOne(y)) {
            switch(typMax){
                caseB DOI(lenZ, vB(z,i)=ARITH2(op,vB(tempX,i),vB(tempY,0))) break;
                caseH DOI(lenZ, vH(z,i)=ARITH2(op,vH(tempX,i),vH(tempY,0))) break;
                caseI DOI(lenZ, vI(z,i)=ARITH2(op,vI(tempX,i),vI(tempY,0))) break;
                caseL DOI(lenZ, vL(z,i)=ARITH2(op,vL(tempX,i),vL(tempY,0))) break;
                caseF DOI(lenZ, vF(z,i)=ARITH2(op,vF(tempX,i),vF(tempY,0))) break;
                caseE DOI(lenZ, vE(z,i)=ARITH2(op,vE(tempX,i),vE(tempY,0))) break;
                default: R E_NOT_IMPL;
            }
        }
        else {
            switch(typMax){
                caseB DOI(lenZ, vB(z,i)=ARITH2(op,vB(tempX,i),vB(tempY,i))) break;
                caseH DOI(lenZ, vH(z,i)=ARITH2(op,vH(tempX,i),vH(tempY,i))) break;
                caseI DOI(lenZ, vI(z,i)=ARITH2(op,vI(tempX,i),vI(tempY,i))) break;
                caseL DOI(lenZ, vL(z,i)=ARITH2(op,vL(tempX,i),vL(tempY,i))) break;
                caseF DOI(lenZ, vF(z,i)=ARITH2(op,vF(tempX,i),vF(tempY,i))) break;
                caseE DOI(lenZ, vE(z,i)=ARITH2(op,vE(tempX,i),vE(tempY,i))) break;
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

#define LOGIC(op,x,y) (2>op?LOGICAND(op,x,y):4>op?LOGICOR(op,x,y):(x^y))
#define LOGICAND(op,x,y) (0==op?(x&y):~(x&y))
#define LOGICOR(op,x,y)  (3==op?(x|y):~(x|y))

L pfnLogic(V z, V x, V y, L op){
    if(isBool(x) && isBool(y)){
        if(!isValidLength(x,y)) R E_LENGTH;
        L lenZ = isOne(x)?vn(y):vn(x), typZ = H_B;
        initV(z,typZ,lenZ);
        if(isOne(x)) {
            DOI(lenZ, vB(z,i)=LOGIC(op,vB(x,0),vB(y,i)))
        }
        else if(isOne(y)) {
            DOI(lenZ, vB(z,i)=LOGIC(op,vB(x,i),vB(y,0)))
        }
        else {
            DOI(lenZ, vB(z,i)=LOGIC(op,vB(x,i),vB(y,i)))
        }
        R 0;
    }
    else R E_DOMAIN;
}

/* and: 0 */
L pfnAnd(V z, V x, V y){
    R pfnLogic(z,x,y,0);
}

/* nand: 1 */
L pfnNand(V z, V x, V y){
    R pfnLogic(z,x,y,1);
}

/* or: 2 */
L pfnOr(V z, V x, V y){
    R pfnLogic(z,x,y,2);
}

/* nor: 3 */
L pfnNor(V z, V x, V y){
    R pfnLogic(z,x,y,3);
}

/* xor: 4 */
L pfnXor(V z, V x, V y){
    R pfnLogic(z,x,y,4);
}

#define POWERLOG(op,x,y) (op==0?POWER(x,y):LOG(x,y))
L pfnPowerLog(V z, V x, V y, L op){
    if(isTypeGroupReal(vp(x)) && isTypeGroupReal(vp(y))){
        if(!isValidLength(x,y)) R E_LENGTH;
        L lenZ  = isOne(x)?vn(y):vn(x), typZ = H_E; // default: E
        L typMax= max(vp(x),vp(y));
        V tempX = allocNode();
        V tempY = allocNode();
        CHECKE(promoteValue(tempX, x, typMax));
        CHECKE(promoteValue(tempY, y, typMax));
        initV(z,typZ,lenZ);
        if(isOne(x)){
            switch(typMax){
                caseB DOI(lenZ, vE(z,i)=POWERLOG(op,vB(tempX,0),vB(tempY,i))) break;
                caseH DOI(lenZ, vE(z,i)=POWERLOG(op,vH(tempX,0),vH(tempY,i))) break;
                caseI DOI(lenZ, vE(z,i)=POWERLOG(op,vI(tempX,0),vI(tempY,i))) break;
                caseL DOI(lenZ, vE(z,i)=POWERLOG(op,vL(tempX,0),vL(tempY,i))) break;
                caseF DOI(lenZ, vE(z,i)=POWERLOG(op,vF(tempX,0),vF(tempY,i))) break;
                caseE DOI(lenZ, vE(z,i)=POWERLOG(op,vE(tempX,0),vE(tempY,i))) break;
                default: R E_NOT_IMPL;
            }
        }
        else if(isOne(y)){
            switch(typMax){
                caseB DOI(lenZ, vE(z,i)=POWERLOG(op,vB(tempX,i),vB(tempY,0))) break;
                caseH DOI(lenZ, vE(z,i)=POWERLOG(op,vH(tempX,i),vH(tempY,0))) break;
                caseI DOI(lenZ, vE(z,i)=POWERLOG(op,vI(tempX,i),vI(tempY,0))) break;
                caseL DOI(lenZ, vE(z,i)=POWERLOG(op,vL(tempX,i),vL(tempY,0))) break;
                caseF DOI(lenZ, vE(z,i)=POWERLOG(op,vF(tempX,i),vF(tempY,0))) break;
                caseE DOI(lenZ, vE(z,i)=POWERLOG(op,vE(tempX,i),vE(tempY,0))) break;
                default: R E_NOT_IMPL;
            }
        }
        else {
            switch(typMax){
                caseB DOI(lenZ, vE(z,i)=POWERLOG(op,vB(tempX,i),vB(tempY,i))) break;
                caseH DOI(lenZ, vE(z,i)=POWERLOG(op,vH(tempX,i),vH(tempY,i))) break;
                caseI DOI(lenZ, vE(z,i)=POWERLOG(op,vI(tempX,i),vI(tempY,i))) break;
                caseL DOI(lenZ, vE(z,i)=POWERLOG(op,vL(tempX,i),vL(tempY,i))) break;
                caseF DOI(lenZ, vE(z,i)=POWERLOG(op,vF(tempX,i),vF(tempY,i))) break;
                caseE DOI(lenZ, vE(z,i)=POWERLOG(op,vE(tempX,i),vE(tempY,i))) break;
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
    R pfnPowerLog(z,x,y,0);
}

L pfnLog(V z, V x, V y){
    R pfnPowerLog(z,x,y,1);
}

L pfnCompress(V z, V x, V y){
    if(isBool(x)){
        if(!isEqualLength(x,y)) R E_LENGTH;
        L lenX = vn(x);
        L typZ = vp(y);
        L lenZ = getNumOfNonZero(x);
        L k    = 0;
        initV(z,typZ,lenZ);
        if(k == lenZ){ // copy all of items
            switch(typZ){
                caseB DOI(lenX, vB(z,i)=vB(y,i)) break;
                caseH DOI(lenX, vH(z,i)=vH(y,i)) break;
                caseI DOI(lenX, vI(z,i)=vI(y,i)) break;
                caseL DOI(lenX, vL(z,i)=vL(y,i)) break;
                caseF DOI(lenX, vF(z,i)=vF(y,i)) break;
                caseE DOI(lenX, vE(z,i)=vE(y,i)) break;
                default: R E_NOT_IMPL;
            }
        }
        else{
            switch(typZ){
                caseB DOI(lenX, if(vB(x,i))vB(z,k++)=vB(y,i)) break;
                caseH DOI(lenX, if(vB(x,i))vH(z,k++)=vH(y,i)) break;
                caseI DOI(lenX, if(vB(x,i))vI(z,k++)=vI(y,i)) break;
                caseL DOI(lenX, if(vB(x,i))vL(z,k++)=vL(y,i)) break;
                caseF DOI(lenX, if(vB(x,i))vF(z,k++)=vF(y,i)) break;
                caseE DOI(lenX, if(vB(x,i))vE(z,k++)=vE(y,i)) break;
                default: R E_NOT_IMPL;
            }
        }
        R 0;
    }
    else R E_DOMAIN;
}

#define INDEXOF(p,z,x,y) lib_index_of_##p(sL(z),s##p(x),vn(x),s##p(y),vn(y))
L pfnIndexOf(V z, V x, V y){
    if(isTypeGroupReal(vp(x)) && isTypeGroupReal(vp(y))){
        L typMax = max(vp(x), vp(y));
        L lenZ   = vn(y);
        V tempX = allocNode();
        V tempY = allocNode();
        CHECKE(promoteValue(tempX, x, typMax));
        CHECKE(promoteValue(tempY, y, typMax));
        initV(z,H_L,lenZ);
        switch(typMax){
            caseB INDEXOF(B, z, tempX, tempY); break;
            caseH INDEXOF(H, z, tempX, tempY); break;
            caseI INDEXOF(I, z, tempX, tempY); break;
            caseL INDEXOF(L, z, tempX, tempY); break;
            caseF INDEXOF(F, z, tempX, tempY); break;
            caseE INDEXOF(E, z, tempX, tempY); break;
            default: R E_NOT_IMPL;
        }
        R 0;
    }
    else if(isSameType(x,y)) {
        switch(xp){
            caseX INDEXOF(X, z, x, y); break;
            caseC INDEXOF(C, z, x, y); break;
            caseQ INDEXOF(Q, z, x, y); break;
            caseS INDEXOF(S, z, x, y); break;
            caseM INDEXOF(M, z, x, y); break;
            caseD INDEXOF(D, z, x, y); break;
            caseZ INDEXOF(Z, z, x, y); break;
            caseU INDEXOF(U, z, x, y); break;
            caseW INDEXOF(W, z, x, y); break;
            caseT INDEXOF(T, z, x, y); break;
            default: R E_DOMAIN;
        }
        R 0;
    }
    else R E_TYPE;
}

L pfnAppend(V z, V x, V y){
    if(isTypeGroupReal(vp(x)) && isTypeGroupReal(vp(y))){
        L lenZ   = vn(x) + vn(y), c = vn(x);
        L typMax = max(vp(x),vp(y));
        V tempX = allocNode();
        V tempY = allocNode();
        CHECKE(promoteValue(tempX, x, typMax));
        CHECKE(promoteValue(tempY, y, typMax));
        initV(z,typMax,lenZ);
        switch(typMax){
            caseB DOI(vn(x),vB(z,i)=vB(x,i)) DOI(vn(y),vB(z,c+i)=vB(y,i)) break;
            caseH DOI(vn(x),vH(z,i)=vB(x,i)) DOI(vn(y),vH(z,c+i)=vH(y,i)) break;
            caseI DOI(vn(x),vI(z,i)=vB(x,i)) DOI(vn(y),vI(z,c+i)=vI(y,i)) break;
            caseL DOI(vn(x),vL(z,i)=vB(x,i)) DOI(vn(y),vL(z,c+i)=vL(y,i)) break;
            caseF DOI(vn(x),vF(z,i)=vB(x,i)) DOI(vn(y),vF(z,c+i)=vF(y,i)) break;
            caseE DOI(vn(x),vE(z,i)=vB(x,i)) DOI(vn(y),vE(z,c+i)=vE(y,i)) break;
        }
        R 0;
    }
    else if(isSameType(x,y) && (isTypeGroupString(vp(x)) || isComplex(x))) {
        L typZ = vp(x), c = vn(x);
        L lenZ = vn(x) + vn(y);
        initV(z,typZ,lenZ);
        switch(vp(x)){
            caseX DOI(vn(x), vX(z,i)=vX(x,i))
                  DOI(vn(y), vX(z,c+i)=vX(y,i))
                  break;
            caseQ DOI(vn(x), vQ(z,i)=vQ(x,i))
                  DOI(vn(y), vQ(z,c+i)=vQ(y,i))
                  break;
            caseC DOI(vn(x), vC(z,i)=vC(x,i))
                  DOI(vn(y), vC(z,c+i)=vC(x,i))
                  break;
            caseS DOI(vn(x), vS(z,i)=vS(x,i))
                  DOI(vn(y), vS(z,c+i)=vS(x,i))
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
 */
L pfnLike(V z, V x, V y){
    if(isTypeGroupString(vp(x)) && (isChar(y) || isString(y))){
        B t;
        if(isChar(y) || isOne(y)){
            B f=isChar(y);
            #define getStrY() f?sC(y):vs(y)
            switch(vp(x)){
                caseC CHECKE(getLikeFromString(&t,sC(x),getStrY()))                  break;
                caseQ DOI(vn(x), \
                      CHECKE(getLikeFromString(&t,getSymbolStr(vL(x,i)),getStrY()))) break;
                caseS DOI(vn(x), \
                      CHECKE(getLikeFromString(&t,vS(x,i),getStrY())))               break;
            }
            initV(z,H_B,isChar(x)?1:vn(x));
            switch(vp(x)){
                caseC getLikeFromString(&t,sC(x),getStrY()); vB(z,0)=t;              break;
                caseQ DOI(vn(x), \
                      {getLikeFromString(&t,getSymbolStr(vL(x,i)),getStrY()); \
                       vB(z,i)=t;})                                                  break;
                caseS DOI(vn(x), \
                      {getLikeFromString(&t,vS(x,i),getStrY()); \
                       vB(z,i)=t;})                                                  break;
            }
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
            R 0;
        }
        else R E_LENGTH;
    }
    else R E_DOMAIN;
}

L pfnOrderBy(V z, V x, V y){
    if(isList(x) && isBool(y) && isEqualLength(x,y)){
        DOI(vn(x), if(!isTypeGroupReal(vp(vV(x,i))))R E_DOMAIN)
        if(!checkMatch(x)) R E_MATCH;
        L lenZ= 0==vn(x)?0:vn(vV(x,0));
        initV(z,H_L,lenZ);
        lib_list_order_by(sL(z), lenZ, x, sB(y));
        R 0;
    }
    else if(isTypeGroupReal(vp(x)) && isBool(y) && 1==vn(y)){
        V0 t0; V t= &t0;
        vp(t)= H_G; vn(t)= 1; vg(t)= (G)x;
        initV(z,H_L,vn(x));
        lib_list_order_by(sL(z), vn(x), t, sB(y));
        R 0;
    }
    else R E_DOMAIN;
}

L pfnEach(V z, V x, FUNC1(foo)){
    if(isList(x)){
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
        L lenZ = vn(y);
        initV(z,H_G,lenZ);
        DOI(lenZ, CHECKE((*foo)(vV(z,i),x,vV(y,i))))
    }
    else {
        CHECKE((*foo)(z,x,y));
    }
    R 0;
}


/* Literals */

L pfnDictTable(V z, V x, V y, L op){
    if(isList(x) && isList(y)){
        if(isEqualLength(x,y)){
            L lenZ = vn(x);
            L typZ = 0==op?H_N:H_A;
            initV(z,typZ,lenZ);
            DOI(lenZ, {V t=vV(z,i); \
                initList(t,2); \
                CHECKE(copyV(vV(t,0),vV(x,i))) \
                CHECKE(copyV(vV(t,1),vV(y,i))) })
            R 0;
        }
        else R E_LENGTH;
    }
    else R E_DOMAIN;
}

L pfnDict(V z, V x, V y){
    R pfnDictTable(z,x,y,0);
}

L pfnTable(V z, V x, V y){
    if(isList(x) && isList(y)){
        DOI(vn(x), if(!isSymbol(vV(x,i)))R E_DOMAIN)
        DOI(vn(y), if(!isTypeGroupColumn(vp(vV(x,i))))R E_DOMAIN)
        CHECKE(pfnDictTable(z,x,y,1));
        va(z).row = (0>=vn(z))?0:vn(getColVal(getTableCol(z,0)));
        va(z).col = vn(x);
        R 0;
    }
    else R E_DOMAIN;
}

/* pseudo version */
L pfnEnum(V z, V x, V y){
    V eKey = x;
    V eVal = allocNode();
    L sym  = 3;
    CHECKE(pfnIndexOf(eVal,eKey,y));
    L lenZ = vn(eVal);
    DOI(lenZ, if(vL(eVal,i)>=vn(eKey))R E_ENUM_INDEX)
    initV(z,H_Y,lenZ);
    initEnum(z,sym,eKey,vg(eVal));
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
//             else R E_DOMAIN;
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
            CHECKE(copyV(vV(z,0),x));
            CHECKE(copyV(vV(z,1),y));
            va(z).row = tableRow(x);
            va(z).col = tableCol(x)+tableCol(y);
            R 0;
        }
        else R E_MATCH;
    }
    else R E_DOMAIN;
}


#define MEMBER(t,z,x,y) case##t CHECKE(lib_member_##t(sB(z),s##t(x),vn(x),s##t(y),vn(y))); break
L pfnMember(V z, V x, V y){
    if(isTypeGroupReal(vp(x))){
        V tempX = allocNode();
        V tempY = allocNode();
        L typMax = max(vp(x),vp(y));
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
 * datetime_add(d, 1:64, `year)
 */
L pfnDatetimeAdd(V z, V x, V y, V m){
    if(isTypeGroupDate(vp(x)) && isTypeGroupInt(vp(y)) && isOneSymbol(m)){
        I op = getDatetimeOp(vq(m));
        if(isOne(x)){
            L typZ = vp(x);
            L lenZ = vn(y);
            V tempY = allocNode();
            CHECKE(promoteValue(tempY, y, H_L));
            initV(z,typZ,lenZ);
            switch(vp(x)){
                caseD DOI(lenZ, vD(z,i)=calcDate(vd(x),vL(tempY,i),op)) break;
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
                caseD DOI(lenZ, vD(z,i)=calcDate(vD(x,i),vl(tempY),op)) break;
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
                caseD DOI(lenZ, vD(z,i)=calcDate(vD(x,i),vL(tempY,i),op)) break;
                default: R E_NOT_IMPL;  /* caseM, caseZ */
            }
        }
        else R E_LENGTH;
    }
    else R E_DOMAIN;
    R 0;
}




