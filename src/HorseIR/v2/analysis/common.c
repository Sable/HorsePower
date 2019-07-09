#include "../global.h"

static void printTypeNode(InfoNode *x){
    printType(x->type);
    if(x->subInfo){
        P("<");
        printTypeNode(x->subInfo);
        P(">");
    }
    if(x->next){
        P(","); printTypeNode(x->next);
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
            //case   tableH: if(sn->isId) P("id:%d)",sn->sizeId);
            //               else P("%d)",sn->size); break;
        }
    }
}

/* InfoNode */

void printInfoNode(InfoNode *x){
    P("\t{");
    printTypeNode(x);
    P("; ");
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

Type getSubType(InfoNode *x){
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

// no dummy
I totalSymbolNames(SymbolNameList *list){
    L c=0; while(list){c++; list=list->next;} R c;
}


