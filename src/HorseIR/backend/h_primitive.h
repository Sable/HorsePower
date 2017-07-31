#pragma once

#ifdef	__cplusplus
extern "C" {
#endif

#define xReal(x) ((x).real)
#define xImag(x) ((x).imag)
#define xCopy(x,a,b) { xReal(x)=a; xImag(x)=b; }

#define isBool(x)      (H_B==vp(x))
#define isSymbol(x)    (H_Q==vp(x))
#define isInteger(x)   (H_L==vp(x))
#define isComplex(x)   (H_X==vp(x))
#define isString(x)    (H_C==vp(x))
#define isMonth(x)     (H_M==vp(x))
#define isDate(x)      (H_D==vp(x))
#define isDateTime(x)  (H_Z==vp(x))
#define isMinute(x)    (H_U==vp(x))
#define isSecond(x)    (H_W==vp(x))
#define isTime(x)      (H_T==vp(x))
#define isList(x)      (H_G==vp(x))
#define isEnum(x)      (H_Y==vp(x))
#define isTable(x)     (H_A==vp(x))
#define isDict(x)      (H_N==vp(x))
#define isOneSymbol(x) (isSymbol(x) && vn(x)==1)

#define isOne(x) (1==vn(x))

#define isSameType(x,y) (vp(x)==vp(y))
#define isEqualLength(x,y) (vn(x)==vn(y))
#define isValidLength(x,y) (isOne(x) || isOne(y) || isEqualLength(x,y))
#define isValidType(x,y) ((isTypeGroupReal(vp(x))&&isTypeGroupReal(vp(y))) || vp(x)==vp(y))

L pfnColumnValue   (V z, V x, V y);
L pfnFindValidIndex(V z, V x, V y);
L pfnFindValidItem (V z, V x, V y);
L pfnIndex         (V z, V x, V y);
L pfnList          (V z, L n, ...); // change unknown args
L pfnDict          (V z, V x, V y);
L pfnTable         (V z, V x);


/* Implement in order */

L pfnAbs           (V z, V x);
L pfnNeg           (V z, V x);
L pfnCeil          (V z, V x);
L pfnFloor         (V z, V x);
L pfnRound         (V z, V x);

L pfnTrig          (V z, V x, L op);
L pfnTrigSin       (V z, V x);
L pfnTrigAsin      (V z, V x);
L pfnTrigCos       (V z, V x);
L pfnTrigAcos      (V z, V x);
L pfnTrigTan       (V z, V x);
L pfnTrigAtan      (V z, V x);

L pfnHyper         (V z, V x, L op);
L pfnHyperSinh     (V z, V x);
L pfnHyperAsinh    (V z, V x);
L pfnHyperCosh     (V z, V x);
L pfnHyperAcosh    (V z, V x);
L pfnHyperTanh     (V z, V x);
L pfnHyperAtanh    (V z, V x);

L pfnConj          (V z, V x);
L pfnRecip         (V z, V x);
L pfnSignum        (V z, V x);
L pfnPi            (V z, V x);
L pfnNot           (V z, V x);
L pfnLen           (V z, V x);
L pfnRange         (V z, V x);
L pfnFact          (V z, V x);
L pfnReverse       (V z, V x);
L pfnUnique        (V z, V x);
L pfnWhere         (V z, V x);

L pfnReduce        (V z, V x, L op);
L pfnMin           (V z, V x);
L pfnMax           (V z, V x);

L pfnOrder         (V z, V x, L op);
L pfnAsc           (V z, V x);
L pfnDesc          (V z, V x);


L pfnChopDate      (V z, V x, L op);
L pfnDateYear      (V z, V x);
L pfnDateMonth     (V z, V x);
L pfnDateDay       (V z, V x);
L pfnDate          (V z, V x);

L pfnChopTime      (V z, V x, L op);
L pfnTimeHour      (V z, V x);
L pfnTimeMinute    (V z, V x);
L pfnTimeSecond    (V z, V x);
L pfnTimeMill      (V z, V x);
L pfnTime          (V z, V x);

L pfnCompare       (V z, V x, V y, L op);
L pfnLt            (V z, V x, V y);
L pfnLeq           (V z, V x, V y);
L pfnGt            (V z, V x, V y);
L pfnGeq           (V z, V x, V y);
L pfnEq            (V z, V x, V y);
L pfnNeq           (V z, V x, V y);

L pfnArith         (V z, V x, V y, L op);
L pfnPlus          (V z, V x, V y);
L pfnMinus         (V z, V x, V y);
L pfnMul           (V z, V x, V y);
L pfnDiv           (V z, V x, V y);

L pfnLogic         (V z, V x, V y, L op);
L pfnAnd           (V z, V x, V y);
L pfnNand          (V z, V x, V y);
L pfnOr            (V z, V x, V y);
L pfnNor           (V z, V x, V y);
L pfnXor           (V z, V x, V y);

L pfnPowerLog      (V z, V x, V y, L op);
L pfnPower         (V z, V x, V y);
L pfnLog           (V z, V x, V y);
L pfnCompress      (V z, V x, V y);

L pfnIndexOf       (V z, V x, V y);
L pfnAppend        (V z, V x, V y);
L pfnLike          (V z, V x, V y);

#ifdef	__cplusplus
}
#endif


