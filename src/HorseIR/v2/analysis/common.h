#ifndef __A_COMMON__
#define __A_COMMON__

#define DEF_VARS_MAX 64
//int fetchVarNames(List *vars, Node *defVars[]);

typedef enum ShapeType {
    unknownH, vectorH, listH, enumH, dictH, tableH
}pShape;

typedef enum ShapeKind {
    constSP, symbolSP, scanSP
}ShapeKind;

typedef struct ShapeNode{
    pShape type;
    ShapeKind kind;
    union {
        int size;     /* (kind=0)  constSP: actual literal size */
        int sizeId;   /* (kind=1) symbolSP: sequential size id  */
        int sizeScan; /* (kind=2)   scanSP: size after compress */
    };
}ShapeNode;

typedef struct InfoNode{
    Type type;
    struct ShapeNode *shape;
    struct InfoNode *subInfo;
    struct InfoNode *next;
}InfoNode;

typedef struct InfoNodeList{
    InfoNode *in;
    struct InfoNodeList *next;
}InfoNodeList;

void printInfoNode(InfoNode *x);
Type getSubType(InfoNode *x);
ShapeNode *getSubShape(InfoNode *x);
bool isSameShape(ShapeNode *x, ShapeNode *y);
void freeInfoNode(InfoNode *in);
void printShapeNode(ShapeNode *sn);

#include "symtable.h"   // create symbol tables
#include "typerule.h"   // define typeshape rules
#include "typeshape.h"  // propagate type/shape rules
#include "udchain.h"    // build use-def chain

I totalSymbolNames(SymbolNameList *list);

#endif
