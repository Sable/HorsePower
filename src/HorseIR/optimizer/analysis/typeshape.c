#include "../global.h"

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

#include "typerule.h" // need above nodes

/* signatures */
static void printInfoNode(InfoNode *in);

static ShapeNode *newShapeNode(pShape type, int size){
    ShapeNode *sn = NEW(ShapeNode);
    sn->type = type;
    if(type == unknownH) { sn->sizeId = size; }
    else sn->size = size;
    return sn;
}

// delete later
static InfoNode *queryInSet(char *def){
    InfoNode *info = NEW(InfoNode);
    info->name  = strdup("pending");
    info->type  = unknownT;
    info->shape = newShapeNode(vectorH, 1);
}

static InfoNode *addParamCaseIdK(Node *n){
    char *name = fetchName(n);
    return queryInSet(name);
}
static InfoNode *addParamCaseLiteral(pType typ){
    InfoNode *info = NEW(InfoNode);
    info->name = NULL;
    info->type = typ;
    info->shape = newShapeNode(vectorH, 1);
}

static InfoNode *getParamItemInfo(Node *n){
    printNodeKind(n); P("\n");
    switch(n->kind){
        case           idK: return addParamCaseIdK(n); break;
        case literalFloatK: return addParamCaseLiteral(f64T ); break;
        case   literalIntK: return addParamCaseLiteral(i64T ); break;
        case   literalSymK: return addParamCaseLiteral(symT ); break;
        case  literalDateK: return addParamCaseLiteral(dateT); break;
        case  literalBoolK: return addParamCaseLiteral(boolT); break;
        case  literalCharK: return addParamCaseLiteral(charT); break;
        default: printNodeKind(n);
                 error("Unknown kind found\n");
    }
}

static void addInfoNodeToList(InfoNodeList *in_list, InfoNode *in){
    in_list->next = NEW(InfoNodeList);
    in_list = in_list->next;
    in_list->in = in;
}

static InfoNodeList *getParamInfo(Node *n){
    List *params = n->val.listS;
    List *temp = params;
    int c = 0;
    InfoNodeList *in_list = NEW(InfoNodeList);
    while(temp){
        Node *pVal = temp->val; //paramValue
        Node *n   = pVal->val.nodeS;
        InfoNode *in = getParamItemInfo(n);
        addInfoNodeToList(in_list, in);
        printInfoNode(in);
        temp = temp->next;
        c++;
    }
    return in_list;
}

static void printInfoNode(InfoNode *in){
    if(in->name){
        P("[var] %s -> ", in->name);
        printType(in->type);
        P(" -> %d (shape)\n", in->shape->type);
    }
    else {
        P("[literal] ");
        printType(in->type);
        P("\n");
    }
}

static int totalNode(InfoNodeList *rt){
    int n = 0;
    while(rt->next) {n++; rt=rt->next;}
    return n;
}

static InfoNode *getNode(InfoNodeList *rt, int k){
    while(rt->next && k>=0){ rt=rt->next; k--; }
    return rt->in;
}

typedef InfoNode* (*nilFunc)();
typedef InfoNode* (*monFunc)(InfoNode*);
typedef InfoNode* (*dyaFunc)(InfoNode*, InfoNode*);
typedef InfoNode* (*allFunc)(InfoNodeList*);

/* entry */
void propagateType(char *funcName, Node *param_list){
    int valence=-1;
    P("funcName = %s\n", funcName);
    InfoNodeList *in_list = getParamInfo(param_list);
    void* funcRtn = fetchTypeRules(funcName, &valence);
    if(totalNode(in_list) == valence){
        InfoNode *newNode;
        if(valence == 2){
            dyaFunc func = (dyaFunc)funcRtn;
            newNode = func(getNode(in_list,0), getNode(in_list, 1));
            printInfoNode(newNode);
        }
        if(valence == 1){
            monFunc func = (monFunc)funcRtn;
            InfoNode *newNode = func(getNode(in_list,0));
            printInfoNode(newNode);
        }
        else if(valence > 2){
            allFunc func = (allFunc)funcRtn;
            newNode = func(in_list);
            printInfoNode(newNode);
        }
        else if(valence == 0){
            nilFunc func = (nilFunc)funcRtn;
            newNode = func();
            printInfoNode(newNode);
        }
        else error("valence must >= 0");
    }
    else error("# of params != expected valence");
}

