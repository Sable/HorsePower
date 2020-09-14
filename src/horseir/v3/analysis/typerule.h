#ifndef __A_TYPERULE__
#define __A_TYPERULE__

/* consts */
typedef enum TypeUnary { /* unary 60 */
    absF, negF, ceilF, floorF, roundF, conjF, recipF, signumF,   piF, notF,
    logF, log2F, log10F, sqrtF, expF, cosF, sinF, tanF, acosF, asinF, atanF, coshF,
    sinhF, tanhF, acoshF, asinhF, atanhF, dateF, yearF, monthF, dayF, timeF,
    hourF, minuteF, secondF, millF, uniqueF, strF, lenF, rangeF, factF, randF,
    seedF, flipF, reverseF, whereF, groupF, sumF, avgF, minF, maxF,
    razeF, tolistF, keysF, valuesF, metaF, loadTableF, fetchF, printF, cumsumF,
    totalU
}TypeUnary;
// removed: countF/enlistF/format

typedef enum TypeBinary { /* binary 33 */
    ltF,  gtF,  leqF,   geqF,    eqF,  neqF,  plusF,  minusF,  mulF, divF,
    powerF, logbF, modF, andF, orF, nandF, norF, xorF, appendF, likeF,
    compressF, randkF, indexofF, takeF, dropF, orderF, memberF, vectorF,
    matchF, indexF, columnValueF, subStringF, range2F, index2F,
    totalB
}TypeBinary;
// removed: datetime_diff

typedef enum TypeOther { /* special 13 */
    eachF, eachItemF, eachLeftF, eachRightF, enumF, dictF, tableF, ktableF,
    indexAF, listF, joinIndexF, dtaddF, dtsubF,
    totalO
}TypeOther;

typedef struct FuncUnit{
    int kind;
    union{
        TypeUnary  u;  /* kind == 1 */
        TypeBinary b;  /* kind == 2 */
        TypeOther  t;  /* kind == 3 */
    };
}FuncUnit;

/* support more precise shape information */
typedef struct {
    union{
        //struct enumMeta  { int keyId;   }  enumMeta;
        //struct tableMeta { int tableId; } tableMeta;
        struct { int keyId;   }  enumMeta;
        struct { int tableId; } tableMeta;
    }meta;
}MetaData;

/* macros */

#define inType(n)  (n)->type
#define inCell(x)  (x)->subInfo
#define inNext(x)  (x)->next
#define inShape(n) (n)->shape
#define inShapeType(x) inShape(x)->type

#define isW(n)     (wildT==inType(n))
#define isListT(n) (listT==inType(n))
#define isListS(n) (listT==getSubType(n))
#define isEnumT(n) (enumT==inType(n))

#define isS(n,t) (t == n->type)
#define isShapeU(n) isS(n, unknownH)
#define isShapeV(n) isS(n,  vectorH)
#define isShapeL(n) isS(n,    listH)
#define isShapeT(n) isS(n,   tableH)
#define sameH(x,y) ((x)->type==(y)->type)

// ShapeNode->kind
#define SN_CONST  constSP   // 0
#define SN_ID    symbolSP   // 1
#define SN_SCAN    scanSP   // 2

#define isSizeConst(x) ((x)==SN_CONST)
#define isSizeId(x)    ((x)==SN_ID)
#define isSizeScan(x)  ((x)==SN_SCAN)

#define isSNConst(x) isSizeConst((x)->kind)
#define isSNId(x)    isSizeId((x)->kind)
#define isSNScan(x)  isSizeScan((x)->kind)

#define isShapeScalar(n) (isShapeV(n) && isSNConst(n) && n->size==1)

#define decideShapeLeft(x,y) inShape(x)
#define decideShapeRight(x,y) inShape(y)

/* signatures */

O  checkFuncNumber();
B  checkType(InfoNode *x, InfoNode *y);
B  checkShape(InfoNode *x, InfoNode *y);
I  getValence(FuncUnit *x);
O  getFuncIndexByName(S name, FuncUnit *x);
O *getTypeRules(char *name, int* num);

ShapeNode *newShapeNode(ShapeType type, ShapeKind kind, I size);

CS obtainTypeUnary (TypeUnary  t);
CS obtainTypeBinary(TypeBinary t);
CS obtainTypeOther (TypeOther  t);

#endif
