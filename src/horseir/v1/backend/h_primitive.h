#pragma once

#ifdef	__cplusplus
extern "C" {
#endif

#define xReal(x) ((x).real)
#define xImag(x) ((x).imag)
#define xCopy(x,a,b) { xReal(x)=a; xImag(x)=b; }
#define xEqual(x,y) (xReal(x)==xReal(y) && xImag(x)==xImag(y))

#define initEnum(x,keyN,keyV,valV,indx) {\
    getEnumName(x) =   keyN; \
    getEnumKey(x)  =(S)keyV; \
    getEnumVal(x)  =(S)valV; \
    getEnumIndex(x)=(G)indx;}

#define isBool(x)      (H_B==vp(x))
#define isSymbol(x)    (H_Q==vp(x))
#define isInt(x)       (H_I==vp(x))
#define isLong(x)      (H_L==vp(x))
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
#define isListFlat(x)  (NULL!=vg2(x))
#define isEnum(x)      (H_Y==vp(x))
#define isTable(x)     (H_A==vp(x))
#define isKTable(x)    (H_K==vp(x))
#define isDict(x)      (H_N==vp(x))
#define isTable2(x)    (isTable(x) || isKTable(x))
#define isIntegers(x)  (isInt(x) || isLong(x))

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

#define FUNC1(x) I (*x)(V,V)
#define FUNC2(x) I (*x)(V,V,V)
#define CHECKE(x) { L e=x; if(e) R e; }

I pfnLoadTable     (V z, V x);
I pfnList          (V z, IL n, V x[]);
I pfnIndex         (V z, V x, V y);
I pfnIndexA        (V z, V x, V y, V m);
I pfnFlip          (V z, V x);
I pfnMatch         (V z, V x, V y);
I pfnMeta          (V z, V x);
I pfnKeys          (V z, V x);
I pfnValues        (V z, V x);
I pfnFetch         (V z, V x);
I pfnColumnValue   (V z, V x, V y);
I pfnIsValidBranch (V z, V x);

/* Implement in order */

I pfnAbs           (V z, V x);
I pfnNeg           (V z, V x);
I pfnCeil          (V z, V x);
I pfnFloor         (V z, V x);
I pfnRound         (V z, V x);

I pfnTrigSin       (V z, V x);
I pfnTrigAsin      (V z, V x);
I pfnTrigCos       (V z, V x);
I pfnTrigAcos      (V z, V x);
I pfnTrigTan       (V z, V x);
I pfnTrigAtan      (V z, V x);

I pfnHyperSinh     (V z, V x);
I pfnHyperAsinh    (V z, V x);
I pfnHyperCosh     (V z, V x);
I pfnHyperAcosh    (V z, V x);
I pfnHyperTanh     (V z, V x);
I pfnHyperAtanh    (V z, V x);

I pfnConj          (V z, V x);
I pfnRecip         (V z, V x);
I pfnSignum        (V z, V x);
I pfnPi            (V z, V x);
I pfnNot           (V z, V x);
I pfnExp           (V z, V x);
I pfnLog           (V z, V x);
I pfnLog2          (V z, V x);
I pfnLog10         (V z, V x);
I pfnLen           (V z, V x);
I pfnRange         (V z, V x);
I pfnFact          (V z, V x);
I pfnReverse       (V z, V x);
I pfnUnique        (V z, V x);
I pfnWhere         (V z, V x);
I pfnSum           (V z, V x);
I pfnAvg           (V z, V x);

I pfnReduce        (V z, V x, I op);
I pfnMin           (V z, V x);
I pfnMax           (V z, V x);

I pfnChopDate      (V z, V x, I op);
I pfnDateYear      (V z, V x);
I pfnDateMonth     (V z, V x);
I pfnDateDay       (V z, V x);
I pfnDate          (V z, V x);

I pfnChopTime      (V z, V x, I op);
I pfnTimeHour      (V z, V x);
I pfnTimeMinute    (V z, V x);
I pfnTimeSecond    (V z, V x);
I pfnTimeMill      (V z, V x);
I pfnTime          (V z, V x);

I pfnEnlist        (V z, V x);
I pfnRaze          (V z, V x);
I pfnToList        (V z, V x);
I pfnGroup         (V z, V x);

I pfnCompare       (V z, V x, V y, I op);
I pfnLt            (V z, V x, V y);
I pfnLeq           (V z, V x, V y);
I pfnGt            (V z, V x, V y);
I pfnGeq           (V z, V x, V y);
I pfnEq            (V z, V x, V y);
I pfnNeq           (V z, V x, V y);

I pfnArith         (V z, V x, V y, I op);
I pfnPlus          (V z, V x, V y);
I pfnMinus         (V z, V x, V y);
I pfnMul           (V z, V x, V y);
I pfnDiv           (V z, V x, V y);

I pfnAnd           (V z, V x, V y);
I pfnNand          (V z, V x, V y);
I pfnOr            (V z, V x, V y);
I pfnNor           (V z, V x, V y);
I pfnXor           (V z, V x, V y);

I pfnPower         (V z, V x, V y);
I pfnLogBase       (V z, V x, V y);
I pfnMod           (V z, V x, V y);
I pfnCompress      (V z, V x, V y);

I pfnIndexOf       (V z, V x, V y);
I pfnAppend        (V z, V x, V y);
I pfnLike          (V z, V x, V y);
I pfnOrderBy       (V z, V x, V y);

I pfnEach          (V z, V x,      FUNC1(foo));
I pfnEachItem      (V z, V x, V y, FUNC2(foo));
I pfnEachLeft      (V z, V x, V y, FUNC2(foo));
I pfnEachRight     (V z, V x, V y, FUNC2(foo));

I pfnOuter         (V z, V x, V y, FUNC2(foo));
//L pfnJoinIndex     (V z, V x, V y, FUNC2(foo));
I pfnJoinIndex     (V z, V x, V y, V f);

I pfnDictTable     (V z, V x, V y, I op);
I pfnDict          (V z, V x, V y);
I pfnTable         (V z, V x, V y);
I pfnEnum          (V z, V x, V y);
I pfnKTable        (V z, V x, V y);

I pfnMember        (V z, V x, V y);
I pfnVector        (V z, V x, V y);

I pfnDatetimeAdd   (V z, V x, V y, V m);
I pfnDatetimeSub   (V z, V x, V y, V m);
I pfnAddFKey       (V x, V xKey, V y, V yKey);
I pfnSubString     (V z, V x, V y);

/* optimizations */

// L optLoopFusionQ1_1 (V z0, V z1, L r0, V t3, V g6, V g5);
// L optLoopFusionQ1_2 (V z, L r0, V g6, V g5);
// L optLoopFusionQ1_3 (V z, L r0, V t1, V g6, V g5);
// L optLoopFusionQ1_4 (V z0, V z1, L r0, V t0, V t1, V t6, V g6, V g5);
// L optLoopFusionQ1_all(V s19, V s20, V s21, V s22, V s23, V s24, V s25, L r0, V t0, V t1, V t3, V t6, V g5, V g6);
// 
// L optLoopFusionQ3_1 (V z, L r0, V p8, V g3);
// 
// L optLoopFusionQ4_1 (V z, L r0, V t2, V t3, V w2, V w3);
// 
// L optLoopFusionQ6_1 (V z, L r0, V t1, V t2, V t3);
// L optLoopFusionQ6_2 (V z, L r0, V t15, V t0, V t1);
// L optLoopFusionQ6_3 (V t16, V t17, V t15, V t0, V t1);
// 
// L optLoopFusionQ14_1(V z, L r0, V t0);
// L optLoopFusionQ14_2(V z, L r0, V p2, V p4, V p5);
// L optLoopFusionQ14_3(V w4, V p2, V p3, V w3, V t1, V t4, V t5);
// 
// L optLoopFusionQ16_1(V z, L r0, V g3, V g6);
// 
// L optLoopFusionQ17_1(V z, L r0, V t2, V c2, V t3, V c3);
// L optLoopFusionQ18_1(V z, V x, V t1);
// 
// L optLoopFusionQ19_1(V z, L r0, V t3, V w0, V w1);
// L optLoopFusionQ19_2(V z, L r0, V d9, V c9, V d11, V d12, V r1);
// L optLoopFusionQ19_3(V z, L r0, V d9, V c9, V d11, V d12, V k1);
// L optLoopFusionQ19_4(V z, L r0, V d9, V c9, V d11, V d12, V h1);
// L optLoopFusionQ19_5(V d1, V d7, V p2, V t4);
// 
// L optLoopFusionQ22_1(V w3, L r0, V w1, V t0, V sub2);
// 
// L optLoopFusionBS_1(V z, L r0, V volatility, V time);
// L optLoopFusionBS_2(V z, L r0, V sptprice, V strike, V time, V rate, V volatility);
// L optLoopFusionBS_3(V z, L r0, V sptprice, L id);
// L optLoopFusionBS_4(V z, L r0, V sptprice, V optionprice);
// 
// L optLoopFusionPR_1(V m0, V m1, L r0, V web);
// L optLoopFusionPR_2(V m0, V m1, V m2, L r0, V web, L id);
// L optLoopFusionPR_4(V w5, L r0, V w2);
// L optLoopFusionPR_5(V w8, L r0, V w1, V w5, L id);
// L optLoopFusionPR_6(V m2, L r0, V w4, V m3);

//I pfnGroupBucket   (V z, V x);
I pfnGroupTrie     (V z, V x);

#ifdef	__cplusplus
}
#endif


