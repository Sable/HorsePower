#pragma once

#ifdef	__cplusplus
extern "C" {
#endif

#define xReal(x) ((x).real)
#define xImag(x) ((x).imag)
#define xCopy(x,a,b) { xReal(x)=a; xImag(x)=b; }
#define xEqual(x,y) (xReal(x)==xReal(y) && xImag(x)==xImag(y))
#define initEnum(x,keyN,keyV,val) {getEnumName(x)=keyN; getEnumTarget(x)=(S)keyV; vg(x)=(G)val;}

#define isBool(x)      (H_B==vp(x))
#define isSymbol(x)    (H_Q==vp(x))
#define isInteger(x)   (H_L==vp(x))
#define isFloat(x)     (H_F==vp(x))
#define isDouble(x)    (H_E==vp(x))
#define isComplex(x)   (H_X==vp(x))
#define isChar(x)      (H_C==vp(x))
#define isString(x)    (H_S==vp(x))
#define isMonth(x)     (H_M==vp(x))
#define isDate(x)      (H_D==vp(x))
#define isDateTime(x)  (H_Z==vp(x))
#define isMinute(x)    (H_U==vp(x))
#define isSecond(x)    (H_W==vp(x))
#define isTime(x)      (H_T==vp(x))
#define isList(x)      (H_G==vp(x))
#define isEnum(x)      (H_Y==vp(x))
#define isTable(x)     (H_A==vp(x))
#define isKTable(x)    (H_K==vp(x))
#define isDict(x)      (H_N==vp(x))
#define isTable2(x)    (isTable(x) || isKTable(x))

#define isOneInt(x) (isTypeGroupInt(vp(x)) && 1==vn(x))
#define isOneSymbol(x) (isSymbol(x) && 1==vn(x))

#define isOne(x) (1==vn(x))

#define isSameType(x,y)    (vp(x)==vp(y))
#define isEqualLength(x,y) (vn(x)==vn(y))
#define isValidLength(x,y) (isOne(x) || isOne(y) || isEqualLength(x,y))
#define isValidType(x,y)   ((isTypeGroupReal(vp(x))&&isTypeGroupReal(vp(y))) || vp(x)==vp(y))

#define Z2D(x) ((x)/216000000LL)
#define Z2T(x) ((x)%216000000LL) //60x60x60x10000
#define CHOPM(op,x) (0==op?(x/100):(x%100))
#define CHOPD(op,x) (0==op?(x/10000):1==op?(x/100%100):(x%100))
#define CHOPZ(op,z,x) {L t=Z2D(x); z=CHOPD(op,t);}

#define CHOPU(op,x) (0==op?(x/60):(x%60))
#define CHOPW(op,x) (0==op?(x/3600):1==op?(x/60%60):(x%60))
#define CHOPT(op,z,x) {L t=T2W(x); z=4>op?CHOPW(op,t):x%1000;}
#define CHOPZT(op,z,x){L t=Z2T(x); CHOPT(op,z,t)}
#define T2W(x) ((x)/1E3)


/* keyed table */
#define isKeySingle(x) (1==tableCol(getKTableKey(x)))

#define FUNC1(x) L (*x)(V,V)
#define FUNC2(x) L (*x)(V,V,V)
#define CHECKE(x) { L e=x; if(e) R e; }

#define tableRow(x) (va(x).row)
#define tableCol(x) (va(x).col)
#define dictNum(x)  tableCol(x)  // # of key-value pairs

L pfnLoadTable     (V z, V x);
L pfnList          (V z, L n, V x[]);
L pfnIndex         (V z, V x, V y);
L pfnIndexA        (V x, V y, V m);
L pfnFlip          (V z, V x);
L pfnMatch         (V z, V x, V y);
L pfnMeta          (V z, V x);
L pfnKeys          (V z, V x);
L pfnValues        (V z, V x);
L pfnFetch         (V z, V x);
L pfnColumnValue   (V z, V x, V y);
L pfnIsValidBranch (V z, V x);

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
L pfnExpLog        (V z, V x, L op);
L pfnExp           (V z, V x);
L pfnLog           (V z, V x);
L pfnLen           (V z, V x);
L pfnRange         (V z, V x);
L pfnFact          (V z, V x);
L pfnReverse       (V z, V x);
L pfnUnique        (V z, V x);
L pfnWhere         (V z, V x);
L pfnSum           (V z, V x);
L pfnAvg           (V z, V x);

L pfnReduce        (V z, V x, L op);
L pfnMin           (V z, V x);
L pfnMax           (V z, V x);

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

L pfnEnlist        (V z, V x);
L pfnRaze          (V z, V x);
L pfnToList        (V z, V x);
L pfnToIndex       (V z, V x);
L pfnGroup         (V z, V x);

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
L pfnLog2          (V z, V x, V y);
L pfnMod           (V z, V x, V y);
L pfnCompress      (V z, V x, V y);

L pfnIndexOf       (V z, V x, V y);
L pfnAppend        (V z, V x, V y);
L pfnLike          (V z, V x, V y);
L pfnOrderBy       (V z, V x, V y);

L pfnEach          (V z, V x,      FUNC1(foo));
L pfnEachItem      (V z, V x, V y, FUNC2(foo));
L pfnEachLeft      (V z, V x, V y, FUNC2(foo));
L pfnEachRight     (V z, V x, V y, FUNC2(foo));

L pfnDictTable     (V z, V x, V y, L op);
L pfnDict          (V z, V x, V y);
L pfnTable         (V z, V x, V y);
L pfnEnum          (V z, V x, V y);
L pfnKTable        (V z, V x, V y);

L pfnMember        (V z, V x, V y);
L pfnVector        (V z, V x, V y);

L pfnDatetimeAdd   (V z, V x, V y, V m);
L pfnDatetimeSub   (V z, V x, V y, V m);
L pfnBetween       (V z, V x, V p, V q);
L pfnAddFKey       (V x, V xKey, V y, V yKey);

L optLoopFusionQ6_1 (V z, L r0, V t1, V t2, V t3);
L optLoopFusionQ6_2 (V z, L r0, V t15, V t0, V t1);
L optLoopFusionQ14_1(V z, L r0, V t0);
L optLoopFusionQ14_2(V z, L r0, V p2, V p4, V p5);
L optLoopFusionQ17_1(V z, L r0, V t2, V c2, V t3, V c3);

L optLoopFusionQ19_1(V z, L r0, V t3, V w0, V w1);
L optLoopFusionQ19_2(V z, L r0, V d9, V c9, V d11, V d12, V r1);
L optLoopFusionQ19_3(V z, L r0, V d9, V c9, V d11, V d12, V k1);
L optLoopFusionQ19_4(V z, L r0, V d9, V c9, V d11, V d12, V h1);

L optLoopFusionBS_1(V z, L r0, V volatility, V time);
L optLoopFusionBS_2(V z, L r0, V sptprice, V strike, V time, V rate, V volatility);

L pfnGroupBucket   (V z, V x);

#ifdef	__cplusplus
}
#endif


