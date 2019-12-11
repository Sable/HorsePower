#include "../global.h"

/*
 * TODO:
 * - Type shape node can be printed using JSON objects
 */

static void printTypeNode(InfoNode *x);

static void printTypeNodeNext(InfoNode *x){
    if(x){
        printTypeNodeNext(x->next);
        WP(","); printTypeNode(x);
    }
}

static void printTypeNodeSub(InfoNode *x){
    printTypeNode(x);
    printTypeNodeNext(x->next);
    //while(x->next){ // sequential
    //    WP(","); printTypeNode(x->next); x = x->next;
    //}
}

static void printTypeNode(InfoNode *x){
    printType(x->type);
    if(x->subInfo){
        WP("<"); printTypeNodeSub(x->subInfo); WP(">");
    }
}

void printShapeNode(ShapeNode *sn){
    if(sn){
        switch(sn->type){
            case unknownH: WP("shape(?,"); break;
            case  vectorH: WP("shape(vector,"); break;
            case    listH: WP("shape(list,"); break;
            case   tableH: WP("shape(table,"); break;
            default: EP("Shape type not supported yet: %d\n",sn->type); break;
        }
        switch(sn->type){
            case unknownH: WP("%d)",sn->size); break;
            case  vectorH:
            case    listH:
            case   tableH: if(isSNConst(sn)) WP("%d",sn->size);
                           else if(isSNId(sn)) WP("id:%d",sn->sizeId);
                           else if(isSNScan(sn)) WP("scan:%d",sn->sizeScan);
                           else EP("Kind not supported: %d\n", sn->kind);
                           WP(")"); break;
            default: break;
            //case   tableH: if(sn->isId) WP("id:%d)",sn->sizeId);
            //               else WP("%d)",sn->size); break;
        }
    }
}

void printInfoNode(InfoNode *x){
    WP("\t{type: ");
    printTypeNode(x);
    WP("; shape: ");
    printShapeNode(x->shape);
    WP("}\n");
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


