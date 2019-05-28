#ifndef __A_TYPERULE__
#define __A_TYPERULE__

/* consts */
typedef enum TypeUnary { /* unary 60 */
    absF, negF, ceilF, floorF, roundF, conjF, recipF, signumF,   piF, notF,
    logF, log2F, log10F, expF, cosF, sinF, tanF, acosF, asinF, atanF, coshF,
    sinhF, tanhF, acoshF, asinhF, atanhF, dateF, yearF, monthF, dayF, timeF,
    hourF, minuteF, secondF, millF, uniqueF, strF, lenF, rangeF, factF, randF,
    seedF, flipF, reverseF, whereF, groupF, countF, sumF, avgF, minF, maxF,
    razeF, enlistF, tolistF, formatF, keysF, valuesF, metaF, loadTableF,
    fetchF,
    totalU
}TypeUnary;

typedef enum TypeBinary { /* binary 33 */
    ltF,  gtF,  leqF,   geqF,    eqF,  neqF,  plusF,  minusF,  mulF, divF,
    powerF, logbF, modF, andF, orF, nandF, norF, xorF, dtdiffF, appendF, likeF,
    compressF, randkF, indexofF, takeF, dropF, orderF, memberF, vectorF,
    matchF, indexF, columnValueF, subStringF,
    totalB
}TypeBinary;

typedef enum TypeOther { /* special 14 */
    eachF, eachItemF, eachLeftF, eachRightF, enumF, dictF, tableF, ktableF,
    indexAF, listF, outerF, joinIndexF, dtaddF, dtsubF,
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


/* macros */

#define inType(n)  (n)->type
#define inShape(n) (n)->shape

#define isW(n)     (wildT==inType(n))
#define isListT(n) (listT==inType(n))
#define isListS(n) (listT==getSubType(n))
#define isEnumT(n) (enumT==inType(n))

#define isS(n, t) (t == n->type)
#define isShapeU(n) isS(n, unknownH)
#define isShapeV(n) isS(n,  vectorH)
#define isShapeL(n) isS(n,    listH)
#define isShapeT(n) isS(n,   tableH)
#define sameH(x,y) ((x)->type==(y)->type)

// ShapeNode->kind
#define SN_CONST 0
#define SN_ID    1
#define SN_SCAN  2

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

void checkFuncNumber();
void *fetchTypeRules(char *name, int* num);
ShapeNode *newShapeNode(pShape type, int kind, int size);
bool checkType(InfoNode *x, InfoNode *y);
bool checkShape(InfoNode *x, InfoNode *y);
int getValence(FuncUnit *x);
void getFuncIndexByName(char *name, FuncUnit *x);

#endif
