#include "../global.h"
#include "typerule.h" // need above nodes

typedef InfoNode* (*NilFunc)();
typedef InfoNode* (*MonFunc)(InfoNode*);
typedef InfoNode* (*DyaFunc)(InfoNode*, InfoNode*);
typedef InfoNode* (*AllFunc)(InfoNodeList*);

/* signatures */

static ShapeNode *newShapeNode(pShape type, int size){
    ShapeNode *sn = NEW(ShapeNode);
    sn->type = type;
    if(type == unknownH) { sn->sizeId = size; }
    else sn->size = size;
    return sn;
}

static InfoNode *addParamCaseIdK(Node *n){
    char *name = fetchName(n);
    InfoNode *idIn = getInfoNode(name);
    if(idIn) return idIn;
    else {P("name = %s",name); error(" has not been typed\n");}
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
    in_list       = in_list->next;
    in_list->in   = in;
}

static int totalNode(InfoNodeList *rt){
    int n = 0;
    while(rt->next) {n++; rt=rt->next;}
    return n;
}

static InfoNodeList *getParamInfo(Node *n){
    List *params = n->val.listS;
    List *temp = params;
    int c = 0;
    InfoNodeList *in_list = NEW(InfoNodeList);
    InfoNodeList *in_temp = in_list;
    while(temp){
        Node *pVal = temp->val; //paramValue
        Node *n   = pVal->val.nodeS;
        InfoNode *in = getParamItemInfo(n);
        addInfoNodeToList(in_temp, in);
        printInfoNode(in);
        temp = temp->next;
        in_temp = in_temp->next;
        c++;
    }
    P("c = %d, tot = %d\n",c,totalNode(in_list));
    return in_list;
}

void printInfoNode(InfoNode *in){
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

static InfoNode *getNode(InfoNodeList *rt, int k){
    while(rt->next && k>=0){ rt=rt->next; k--; }
    return rt->in;
}

/* entry */
InfoNode *propagateType(char *funcName, Node *param_list){
    int valence=-1;
    P("funcName = %s\n", funcName);
    InfoNodeList *in_list = getParamInfo(param_list);
    void* funcRtn = fetchTypeRules(funcName, &valence);  /* entry */
    P("valence = %d, total = %d\n", valence, totalNode(in_list));
    if(totalNode(in_list) == valence){
        InfoNode *newNode;
        if(valence == 2){
            DyaFunc func = (DyaFunc)funcRtn;
            newNode = func(getNode(in_list,0), getNode(in_list, 1));
            if(newNode == NULL){
                error("1 null type rules found\n");
            }
        }
        else if(valence == 1){
            MonFunc func = (MonFunc)funcRtn;
            newNode = func(getNode(in_list,0));
        }
        else if(valence > 2){
            AllFunc func = (AllFunc)funcRtn;
            newNode = func(in_list);
        }
        else if(valence == 0){
            NilFunc func = (NilFunc)funcRtn;
            newNode = func();
        }
        else error("valence must >= 0");
        if(newNode == NULL){
            error("null type rules found\n");
        }
        printInfoNode(newNode);
        return newNode;
    }
    else error("# of params != expected valence");
}

InfoNode *propagateTypeCopy(Node *param){
    P("=====Copy=====\n");
    return getParamItemInfo(param->val.nodeS);
}

/* TODO: add more cases */
static bool isCastable(pType x, pType c){
    if(x == unknownT) return true;
    else false;
}

void propagateTypeCast(InfoNode *in, Node *cast){
    pType ctype = cast->val.typeS;
    if(isCastable(in->type, ctype)){
        in->type = ctype;
    }
    else error("can't cast the type");
}
