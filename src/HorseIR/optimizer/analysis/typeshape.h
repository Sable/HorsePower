#ifndef __H__TYPESHAPE__
#define __H__TYPESHAPE__

typedef enum ShapeType {
    unknownH, vectorH, listH
}pShape;

typedef struct ShapeNode{
    pShape type;
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

/* signatures */
void printInfoNode(InfoNode *in);
InfoNode *propagateType(char *funcName, Node *param_list);
InfoNode *propagateTypeCopy(Node *param);
void propagateTypeCast(InfoNode *in, Node *cast);

#endif
