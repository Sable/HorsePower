#include "../global.h"

static void printTypeNode(InfoNode *x);

static void printTypeNodeNext(InfoNode *x){
    if(x){
        printTypeNodeNext(x->next);
        P(","); printTypeNode(x);
    }
}

static void printTypeNodeSub(InfoNode *x){
    printTypeNode(x);
    printTypeNodeNext(x->next);
    //while(x->next){ // sequential
    //    P(","); printTypeNode(x->next); x = x->next;
    //}
}

static void printTypeNode(InfoNode *x){
    printType(x->type);
    if(x->subInfo){
        P("<"); printTypeNodeSub(x->subInfo); P(">");
    }
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
            default: break;
            //case   tableH: if(sn->isId) P("id:%d)",sn->sizeId);
            //               else P("%d)",sn->size); break;
        }
    }
}

/* InfoNode */

void printInfoNode(InfoNode *x){
    P("\t{type: ");
    printTypeNode(x);
    P("; shape: ");
    printShapeNode(x->shape);
    P("}\n");
}

static InfoNode *getSubInfo(InfoNode *x){
    InfoNode *sub = x->subInfo;
    if(sub){
        if(sub->next || sub->subInfo)
            EP("Not a simple info node");
        else
            return sub;
    }
    else EP("Cell not existed");
}

HorseType getSubType(InfoNode *x){
    return getSubInfo(x)->type;
}

ShapeNode *getSubShape(InfoNode *x){
    return getSubInfo(x)->shape;
}

bool isSameShape(ShapeNode *x, ShapeNode *y){
    if(x->type == y->type && x->kind == y->kind){
        switch(x->kind){
            case  constSP: return x->size == y->size;
            case symbolSP: return x->sizeId == y->sizeId;
            case   scanSP: return x->sizeScan == y->sizeScan;
        }
    }
    return false;
}

void freeInfoNode(InfoNode *in){
    if(in){
        freeInfoNode(in->subInfo);
        freeInfoNode(in->next);
        free(in);
    }
}

// I totalList(List *list){
//     R list?(1+totalList(list->next)):0;
// }

I totalList(List *list){ // no dummy
    I c=0; while(list){c++; list=list->next;} R c;
}

I totalList1(List *list){ // with dummy
    I c=0; while((list=list->next))c++; R c;
}

I totalInfoNodeList(InfoNodeList *list){ // no dummy
    I c=0; while(list){c++; list=list->next;} R c;
}

I totalInfoNodeList1(InfoNodeList *list){ // with dummy
    I c=0; while((list=list->next))c++; R c;
}

I totalSymbolNameList(SymbolNameList *list){ // no dummy
    I c=0; while(list){c++; list=list->next;} R c;
}


