#ifndef __H_A_COMMON__
#define __H_A_COMMON__

#define DEF_VARS_MAX 64

typedef enum {
    unknownH, vectorH, listH, enumH, dictH, tableH
}ShapeType;

typedef enum {
    constSP, symbolSP, scanSP
}ShapeKind;

typedef struct ShapeNode{
    ShapeType type;
    ShapeKind kind;
    union {
        int size;       /* (kind=0)  constSP: actual literal size */
        int sizeId;     /* (kind=1) symbolSP: sequential size id  */
        int sizeScan;   /* (kind=2)   scanSP: size after compress */
    };
    Node *consts;       /* when kind == constSP, refer to a vector of constants */
}ShapeNode;

typedef struct InfoNode{
    Type type;
    struct ShapeNode *shape;
    struct InfoNode *subInfo;
    struct InfoNode *next;
    Node *funcs;   /* when type == funcT, refer to a vector of funcs   */
}InfoNode;

typedef struct InfoNodeList{
    InfoNode *in;
    struct InfoNodeList *next;
}InfoNodeList;

/* analysis/common.c */
void       printInfoNode(InfoNode *x);
Type       getSubType(InfoNode *x);
ShapeNode* getSubShape(InfoNode *x);

bool isSameShape(ShapeNode *x, ShapeNode *y);
void freeInfoNode(InfoNode *in);
void printShapeNode(ShapeNode *sn);

#include "symtable.h"   // create symbol tables
#include "typerule.h"   // define typeshape rules
#include "typeshape.h"  // propagate type/shape rules
#include "udchain.h"    // build use-def chain

I totalList (List *list);                    //   no dummy
I totalList1(List *list);                    // with dummy
I totalInfoNodeList (InfoNodeList *list);    //   no dummy
I totalInfoNodeList1(InfoNodeList *list);    // with dummy
I totalSymbolNameList(SymbolNameList *list); //   no dummy

/* typeshape.c */
InfoNode*  getInfoFromNode(Node *n);

#endif
