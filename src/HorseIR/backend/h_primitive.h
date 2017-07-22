#pragma once

#ifdef	__cplusplus
extern "C" {
#endif

#define xReal(x) ((x).real)
#define xImag(x) ((x).imag)
#define xCopy(x,a,b) { xReal(x)=a; xImag(x)=b; }

#define isBool(x)      (H_B==vp(x))
#define isSymbol(x)    (H_S==vp(x))
#define isInteger(x)   (H_L==vp(x))
#define isComplex(x)   (H_X==vp(x))
#define isOneSymbol(x) (isSymbol(x) && vn(x)==1)

#define isOne(x) (1==vn(x))

#define isEqualLength(x,y) (vn(x)==vn(y))
#define isValidLength(x,y) (isOne(x) || isOne(y) || isEqualLength(x,y))
#define isValidType(x,y) ((isTypeGroupReal(vp(x))&&isTypeGroupReal(vp(y))) || vp(x)==vp(y))

L pfnColumnValue   (V z, V x, V y);
L pfnIndexOf       (V z, V x, V y);
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
L pfnReduce        (V z, V x, V y);

#ifdef	__cplusplus
}
#endif


