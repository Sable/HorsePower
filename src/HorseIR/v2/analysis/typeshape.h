#ifndef __H_TYPESHAPE__
#define __H_TYPESHAPE__

//typedef enum ShapeType {
//    unknownH, vectorH, listH, enumH, dictH, tableH
//}pShape;
//
//typedef struct ShapeNode{
//    pShape type;
//    //bool isId;
//    int kind;
//    union {
//        int size;     /* (kind=0) actual literal size */
//        int sizeId;   /* (kind=1) sequential size id  */
//        int sizeScan; /* (kind=2) size after compress */
//    };
//}ShapeNode;

//typedef struct TypeShapeNode{
//    Type type;
//    ShapeNode *shape;
//    struct TypeShapeNode *sub;
//    struct TypeShapeNode *next;
//}TypeShapeNode;

//typedef struct InfoNode{
//    char *name;
//    TypeShapeNode *node;
//}InfoNode;


//typedef struct ShapeNodeSub{
//    bool isSame; /* true if all sub cells have the same shape */
//    int  sizeId;
//}ShapeNodeSub;
//
//typedef struct TypeShapeNode{
//         char *name;
//        pType  type, subtype;
//    ShapeNode *shape;
// ShapeNodeSub *subshape;
//}InfoNode;

//typedef struct TypeShapeNodeList{
//    InfoNode *in;
//    struct TypeShapeNodeList *next;
//}InfoNodeList;

/* signatures */

InfoNodeList *propagateType(Node *func, InfoNodeList *list);
int totalInfo(InfoNodeList *list);
void propagateTypeShape(Prog *root);


#endif

