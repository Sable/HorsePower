#ifndef __H__TYPESHAPE__
#define __H__TYPESHAPE__

typedef enum ShapeType {
    unknownH, vectorH, listH, tableH
}pShape;

typedef struct ShapeNode{
    pShape type;
    bool isId;
    union {
        int size;    /* actual literal size */
        int sizeId;  /* sequential size id  */
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

/* ids */
extern int shapeId;

/* signatures */
void printInfoNode(InfoNode *in);
InfoNode *propagateType(char *funcName, Node *param_list);
InfoNode *propagateTypeCopy(Node *param);
void propagateTypeCast(InfoNode *in, Node *cast);

/* typerule.c */
ShapeNode *newShapeNode(pShape type, bool isId, int size);
void *fetchTypeRules(char *name, int* num);
int findFuncIndex(char *funcName);
int getValence(pFunc k);

#endif
