#ifndef __A_TYPERULE__
#define __A_TYPERULE__

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


#endif
