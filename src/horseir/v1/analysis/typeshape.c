#include "../global.h"

typedef InfoNode* (*NilFunc)();
typedef InfoNode* (*MonFunc)(InfoNode*);
typedef InfoNode* (*DyaFunc)(InfoNode*, InfoNode*);
typedef InfoNode* (*AllFunc)(InfoNodeList*);
// InfoNodeList has a leading dummy node

/* signatures */

static InfoNode *addParamCaseIdK(Node *n){
    char *name = fetchName(n);
    InfoNode *idIn = getInfoNode(name);
    if(idIn) return idIn;
    else {P("name = %s",name); error(" has not been typed\n");}
}
static InfoNode *addParamCaseLiteral(pType typ, int size){
    InfoNode *info = NEW(InfoNode);
    info->name = NULL;
    info->type = typ;
    info->shape = newShapeNode(vectorH, false, size);
    return info;
}

static int literalSize(Node *n){
    List *list = n->val.listS;
    int tot = 0; while(list) {list=list->next; tot++;}
    return tot;
}

static InfoNode *getParamItemInfo(Node *n){
    //printNodeKind(n); P("\n");
    switch(n->kind){
        case           idK: return addParamCaseIdK(n); break;
        case literalFloatK: return addParamCaseLiteral( f64T, literalSize(n)); break;
        case   literalIntK: return addParamCaseLiteral( i64T, literalSize(n)); break;
        case   literalSymK: return addParamCaseLiteral( symT, literalSize(n)); break;
        case  literalDateK: return addParamCaseLiteral(dateT, literalSize(n)); break;
        case  literalBoolK: return addParamCaseLiteral(boolT, literalSize(n)); break;
        case  literalCharK: return addParamCaseLiteral(charT, literalSize(n)); break;
        case   literalStrK: return addParamCaseLiteral( strT, literalSize(n)); break;
        case  literalFuncK: return addParamCaseLiteral(funcT, literalSize(n)); break;
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
        temp = temp->next;
        in_temp = in_temp->next;
        c++;
    }
    //P("c = %d, tot = %d\n",c,totalNode(in_list));
    return in_list;
}


void printShapeNode(ShapeNode *sn){
    if(sn){
        switch(sn->type){
            case unknownH: P("shape(?,"); break;
            case  vectorH: P("shape(vector,"); break;
            case    listH: P("shape(list,"); break;
            case   tableH: P("shape(table,"); break;
            default: EP("shape type not supported yet: %d\n",sn->type); break;
        }
        switch(sn->type){
            case unknownH: P("%d)",sn->size); break;
            case  vectorH:
            case    listH:
            case   tableH: if(isSNConst(sn)) P("%d",sn->size);
                           else if(isSNId(sn)) P("id:%d",sn->sizeId);
                           else if(isSNScan(sn)) P("scan:%d",sn->sizeScan);
                           else EP("kind not supported: %d\n", sn->kind);
                           P(")"); break;
            //case   tableH: if(sn->isId) P("id:%d)",sn->sizeId);
            //               else P("%d)",sn->size); break;
        }
    }
}

void printInfoNode(InfoNode *in){
    if(in->name){
        P("[var] %-3s -> ", in->name);
        printType(in->type);
    }
    else {
        P("[literal] ");
        printType(in->type);
    }
    P(" -> "); printShapeNode(in->shape); P("\n");
}

static InfoNode *getNode(InfoNodeList *rt, int k){
    while(rt->next && k>=0){ rt=rt->next; k--; }
    return rt->in;
}

/* entry */
InfoNode *propagateType(char *funcName, Node *param_list){
    int valence=-1;
    //P("--> %s\n", funcName);
    InfoNodeList *in_list = getParamInfo(param_list);
    void* funcRtn = fetchTypeRules(funcName, &valence);  /* entry */
    if(!funcRtn) {
        EP("Error type rules found in *%s*\n", funcName);
    }
    //P("valence = %d, total = %d\n", valence, totalNode(in_list));
    if(totalNode(in_list) == valence){
        InfoNode *newNode;
        if(valence == 2){
            DyaFunc func = (DyaFunc)funcRtn;
            newNode = func(getNode(in_list,0), getNode(in_list,1));
            if(newNode == NULL){
                printInfoNode(getNode(in_list, 0));
                printInfoNode(getNode(in_list, 1));
                EP("[Dynamic] null type rules found for %s\n",funcName);
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
            EP("null type rules found for [%s]\n",funcName);
        }
        //printInfoNode(newNode);
        return newNode;
    }
    else if(valence == -1){
        AllFunc func = (AllFunc)funcRtn;
        InfoNode *newNode = func(in_list);
        if(newNode == NULL){
            EP("null type rules found for [%s] (-1)\n",funcName);
        }
        return newNode;
    }
    else error("# of params != expected valence");
}

InfoNode *propagateTypeCopy(Node *param){
    //P("=====Copy=====\n");
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
