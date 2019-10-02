#include "../global.h"

BSTree rootT;
BSTree *symTable;
L symCur, symSize; // symSize(init. 5000)

#define symCopySym(x, y) { strcpy(x.str, y.str); x.len=y.len; }
#define symCopyStr(x, s) { strcpy(x.str, s); x.len=strlen(s); }
#define safeDelete(x)    { if(x!=NULL) free(x); x=NULL; }

void R_Rotate(BSTree *p) {
    BSTree lc;
    lc = (*p)->lchild;
    (*p)->lchild = lc->rchild;
    lc->rchild = (*p);
    (*p) = lc;
}

void L_Rotate(BSTree *p) {
    BSTree rc;
    rc = (*p)->rchild;
    (*p)->rchild = rc->lchild;
    rc->lchild = (*p);
    (*p) = rc;
}

void leftBalance(BSTree *T) { //make left child balance
    BSTree lc, rd;
    lc = (*T)->lchild;
    switch (lc->bf) {
        case LH: //LL
            (*T)->bf = lc->bf = EH;
            R_Rotate(T);
            break;
        case RH: //LR
            rd = lc->rchild;
            switch (rd->bf) {
                case LH:(*T)->bf = RH;
                    lc->bf = EH;
                    break;
                case EH:(*T)->bf = lc->bf = EH;
                    break;
                case RH:(*T)->bf = EH;
                    lc->bf = LH;
                    break;
            }
            rd->bf = EH;
            L_Rotate(&((*T)->lchild));
            R_Rotate(T);
            break;
    }
}

void rightBalance(BSTree *T) { //make right child balance
    BSTree rc, ld;
    rc = (*T)->rchild;
    switch (rc->bf) {
        case RH: //RR
            (*T)->bf = rc->bf = EH;
            L_Rotate(T);
            break;
        case LH: //RL
            ld = rc->lchild;
            switch (ld->bf) {
                case LH:(*T)->bf = EH;
                    rc->bf = RH;
                    break;
                case EH:(*T)->bf = rc->bf = EH;
                    break;
                case RH:(*T)->bf = LH;
                    rc->bf = EH;
                    break;
            }
            ld->bf = EH;
            R_Rotate(&((*T)->rchild));
            L_Rotate(T);
            break;
    }
}

/*
index initial -1
id is the returned value
 */
bool insertAVL(BSTree *T, Elemtype e, bool *taller, L *index, L *id)
{
    if (!(*T)) {
        BSTree Tx = (BSTree) malloc(sizeof (BSTnode));
        *T = Tx;
        symCopySym(Tx->data,e); //T->data = e;
        Tx->lchild = Tx->rchild = NULL;
        Tx->index = ++(*index); //insert successfully
        Tx->bf = 0; //should be initialized zero
        *id = *index;
        symTable[*id] = Tx; //BTtree psym[10005];psym[id]->data 
        *taller = true;
    } else {
        if (EQ(e, (*T)->data)) { //insert fail
            *id = (*T)->index;
            *taller = false;
            return 0;
        }
        if (LT(e, (*T)->data)) {
            // if (!InsertAVL(T->lchild, e, taller, index, id))return 0;
            if (!insertAVL(&((*T)->lchild), e, taller, index, id))return 0;
            if (*taller) {
                switch ((*T)->bf) {
                    case LH:
                        leftBalance(T);
                        *taller = false;
                        break;
                    case EH:
                        (*T)->bf = LH;
                        *taller = true;
                        break;
                    case RH:
                        (*T)->bf = EH;
                        *taller = false;
                        break;
                }
            }
        } else {
            // if (!InsertAVL(T->rchild, e, taller, index, id))return 0;
            if (!insertAVL(&((*T)->rchild), e, taller, index, id)) return 0;
            if (*taller) {
                switch ((*T)->bf) {
                    case LH:
                        (*T)->bf = EH;
                        *taller = false;
                        break;
                    case EH:
                        (*T)->bf = RH;
                        *taller = true;
                        break;
                    case RH:
                        rightBalance(T);
                        *taller = false;
                        break;
                }
            }
        }
    }
    return 1;
}

void initSym()
{
    rootT=NULL;
    symCur = -1;
    symSize = 5000;
    symTable=(BSTree*)malloc(sizeof(BSTree) * symSize); //initial size
}

void incSym()
{
    BSTree *t = (BSTree*)malloc(sizeof(BSTree) * symSize * 2);
    // DOI(sybmax, t[i]=psym[i]); DOI(sybmax, t[i+sybmax]=NULL); 
    memcpy(t,symTable,symSize*sizeof(BSTree));
    memset(t+symSize,0,symSize*sizeof(BSTree));
    symSize *= 2; free(symTable); symTable=t;
}

L getSymbol(S name) {
    Elemtype e;
    B taller = false;
    L id = -1;
    symCopyStr(e, name); // e = name;
    if(symCur >= symSize-1) incSym();
    insertAVL(&rootT, e, &taller, &symCur, &id);
    return id;
}

L insertSym(Elemtype h1) {
    R getSymbol(h1.str);
}

void cleanSym() {
    L c;
    deleteT(rootT); rootT = NULL;
    for (c = 0; c <= symCur; c++) {
        symTable[c] = NULL;
    }
}

// remove all & in method arguments
void deleteT(BSTree T)
{
    if (T) {
        if (T->lchild) deleteT(T->lchild);
        if (T->rchild) deleteT(T->rchild);
        safeDelete(T);
    }
}

B symEqual(Elemtype h1, Elemtype h2) {
    return strcmp(h1.str, h2.str)==0;
}

B symLess(Elemtype h1, Elemtype h2) {
    return strcmp(h1.str, h2.str) <0;
}

B symLessEqual(Elemtype h1, Elemtype h2) {
    return strcmp(h1.str, h2.str)<=0;
}

Elemtype createSymbol(S s){
    Elemtype x;
    strcpy(x.str, s);
    x.len = strlen(s);
    R x;
}

L printSymbol(L x, S strBuff){
    R SP(strBuff, "%s", symTable[x]->data.str);
}

L printSymTick(L x, S strBuff){
    R SP(strBuff, "`%s", symTable[x]->data.str);
}

void printAllSymol(){
    P("total size = %lld\n", symCur+1);
    DOI(symCur+1, P("[%2lld] %s\n",i,symTable[i]->data.str));
}

void printSymInfo(){
    if(H_DEBUG)
        P("-> [Info symbol] Init. %lld, used %lld (%lf%%)\n",\
            symSize, symCur+1, (symCur+1)*100.0/symSize);
}

L getSymbolSize(L x){
    R symTable[x]->data.len;
}

S getSymbolStr(L x){
    R symTable[x]->data.str;
}

L compareSymbol(L x, L y){
    R (x==y)?0:strcmp(getSymbolStr(x), getSymbolStr(y));
}

