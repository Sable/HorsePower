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

typedef struct TypeShapeNode{
         char *name;
        pType  type;
    ShapeNode *shape;
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

#endif
