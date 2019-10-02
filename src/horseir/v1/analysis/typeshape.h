#ifndef __H__TYPESHAPE__
#define __H__TYPESHAPE__

typedef enum ShapeType {
    unknownH, vectorH, listH, tableH
}pShape;

typedef struct ShapeNode{
    pShape type;
    //bool isId;
    int kind;
    union {
        int size;     /* (kind=0) actual literal size */
        int sizeId;   /* (kind=1) sequential size id  */
        int sizeScan; /* (kind=2) size after compress */
    };
}ShapeNode;

typedef struct ShapeNodeSub{
    bool isSame; /* true if all sub cells have the same shape */
    int  sizeId;
}ShapeNodeSub;

typedef struct TypeShapeNode{
         char *name;
        pType  type, subtype;
    ShapeNode *shape;
 ShapeNodeSub *subshape;
}InfoNode;

typedef struct TypeShapeNodeList{
    InfoNode *in;
    struct TypeShapeNodeList *next;
}InfoNodeList;

/* macros */

#define isU(n)  (unknownT==(n)->type)
#define isListT(n) (listT==(n)->type)
#define isListS(n) (listT==(n)->subtype)
#define isEnumT(n) (enumT==(n)->type)

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

#define decideShapeLeft(x,y) x->shape
#define decideShapeRight(x,y) y->shape

/* ids */
extern int shapeId;

/* signatures */
void printInfoNode(InfoNode *in);
void printShapeNode(ShapeNode *sn);
InfoNode *propagateType(char *funcName, Node *param_list);
InfoNode *propagateTypeCopy(Node *param);
void propagateTypeCast(InfoNode *in, Node *cast);

/* typerule.c */
//ShapeNode *newShapeNode(pShape type, bool isId, int size);
ShapeNode *newShapeNode(pShape type, int kind, int size);
void *fetchTypeRules(char *name, int* num);
I findFuncIndex(char *funcName);
I getValence(pFunc k);

#endif
