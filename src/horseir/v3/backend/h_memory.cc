#include "../global.h"

/*
 * 1   G = 1073741824
 * 1.5 G = 1610612736
 * 2   G = 2147483648
 * 2.5 G = 2684354560
 * 50  G = 53687091200LL
 */
const L INIT_HEAP_SIZE = 53687091200LL; //64MB, 64*1024*1024
const L INIT_HASH_SIZE = 1073741824; //1GB, 536870912;  //512MB

typedef struct buddy_node { /* size 48 */
    L size, level; G value;
    struct buddy_node *left, *right, *parent;
}BN0,*BN;

#define bn(x) x->size
#define bv(x) x->level
#define ba(x) x->value
#define bx(x) x->left
#define by(x) x->right
#define bp(x) x->parent

/* global vars */

#define USE_NAIVE
// #define USE_BUDDY

L  whichSystme = 0;           // naive (0) or buddy (1)
G  gHeap = NULL; L gHeapCur;  // naive: global heap and its cursor
BN gRoot = NULL;              // buddy: global heap
L  numBlocks = 0;

G  hHeap = NULL; L hHeapCur;  // hash heap

/* methods */

//void initMain(){
void initMemory(){
    #ifdef USE_NAIVE
        useNaiveSystem();
    #endif

    #ifdef USE_BUDDY
        useBuddySystem();
    #endif
    initHashHeap();
}

void useNaiveSystem(){
    gHeap = (G)malloc(INIT_HEAP_SIZE);
    gHeapCur = 0;
    if(H_DEBUG)
        WP("-> [Info heap] Successfully initialized\n");
}

G getHeapMem(I typ, L len){
    #ifdef USE_NAIVE
        R allocMem(gHeap, &gHeapCur, INIT_HEAP_SIZE, typ, len);
    #endif

    #ifdef USE_BUDDY
        R allocBlock(typ, len);
    #endif
}

G allocHeap(L size){
    G g = (G)malloc(size);
    if(!g) {
        FP(stderr, "No sufficient memory for the request: %lld!\n",size);
        exit(99);
    }
    R g;
}

/* Naive system */

G allocMem(G heap, L *cur, L top, I typ, L len){
#define CHUNK_MASK 0xF
#define CHUNK_SIZE 4
    L size = getTypeSize(typ, len);
    if((size & CHUNK_MASK) !=0) {
        size = ((size>>CHUNK_SIZE)+1)<<CHUNK_SIZE;
    }
    G g = NULL;
    if((*cur)+size < top){
        if(H_DEBUG){
            // WP("-> [Info heap] Allocated %3lld for info (", size);
            // printType(typ);
            // WP(", %2lld)\n",len);
        }
        g = heap + (*cur);
        *cur = (*cur) + size;
    }
    else EP("Heap full!! when requesting %lld",size);
    R g;
}

static G allocMemSize(L size){
    G g = NULL;
    if(gHeapCur+size < INIT_HEAP_SIZE){
        g = gHeap + gHeapCur;
        gHeapCur += size;
    }
    else EP("Heap full!! when requesting %lld",size);
    R g;
}

L getTypeSize(I typ, L len){
    L r = -1, k = 0;
    switch(typ){
        case H_B  : r = sizeof(B);     break;
        case H_J  : r = sizeof(J);     break;
        case H_C  : r = sizeof(C); k=1;break;
        case H_H  : r = sizeof(H);     break;
        case H_I  : r = sizeof(I);     break;
        case H_L  : r = sizeof(L);     break;
        case H_F  : r = sizeof(F);     break;
        case H_E  : r = sizeof(E);     break;
        case H_X  : r = sizeof(X);     break;
        case H_Q  : r = sizeof(Q);     break;
        case H_S  : r = sizeof(S);     break;
        case H_M  : r = sizeof(M);     break;
        case H_D  : r = sizeof(D);     break;
        case H_Z  : r = sizeof(Z);     break;
        case H_U  : r = sizeof(U);     break;
        case H_W  : r = sizeof(W);     break;
        case H_T  : r = sizeof(T);     break;
        case H_G  : r = sizeof(V0);    break;
        case H_N  : r = sizeof(V0);    break;
        case H_Y  : r = sizeof(L);     break; /* integers */
        case H_A  : r = sizeof(V0);    break;
        case H_K  : r = sizeof(V0);    break;
        case H_V  : r = sizeof(V0);    break;
        default: EP("Type not supported: %s", getTypeName(typ));
    }
    return r * len + k;
}

L getHeapOffset(){
    R gHeapCur;
}

L setHeapOffset(L cur){
    R gHeapCur = cur;
}

/* allocate */

V allocNode(){
    R (V)getHeapMem(H_V, 1); //malloc(sizeof(V0));
}

V allocTable(L numCols){
    V x = allocNode();
    initV(x, H_A, 2);
    R initTableDict(x,H_A,numCols);
}

/* may move to a string pool */
G allocStrMem(L n){
    R (n>=0)?getHeapMem(H_C,n):NULL;
}

/* initialization */

V initV(V x, I typ, L len){
    xp = typ;
    xn = len;
    if(isTypeGroupBasic(typ)){
        xg = (len>1)?getHeapMem(typ, len):(G)&xb;
    }
    else {
        xg = getHeapMem(typ, len);
        switch(typ){
            caseY vy(x) = (Y)allocMemSize(sizeof(Y0)); break;
            caseA va(x) = (A)allocMemSize(sizeof(A0)); break;
        }
    }
    R x; 
}

V initSymbol(V x, L val){
    initV(x, H_Q, 1);
    xq = val;
    R x;
}

V initList(V x, L numItems){
    initV(x, H_G, numItems);
    vg2(x) = NULL;
    R x;
}

V initFlatList(V x, L numItems){
    initV(x, H_L, numItems);
    vp(x) = H_G;
    vg2(x) = NULL;
    R x;
}


V initValue(V x, I typ, L len){
    R initV(x,typ,len);
}

V initTable(V x, L numCols){
    initV(x, H_A, 2);
    R initTableDict(x,H_A,numCols);
}

V initDict(V x, L numCols){
    initV(x, H_N, 2);
    R initTableDict(x,H_N,numCols);
}

V initKTable(V x){
    R initV(x, H_K, 2);
}

V initTableDict(V x, I typ, L numCols){
    initV(getTableKeys(x),H_Q,numCols);
    initV(getTableVals(x),H_G,numCols);
    R x;
}

S insertString2(S str){
    L len = strlen(str);
    R (S)memcpy(getHeapMem(H_C, len), str, len+1);
}


/* memory management: buddy system */

BN newBlock(L n, BN p, G ptr){
    BN x = (BN)malloc(sizeof(BN0));
    bx(x) = NULL;
    by(x) = NULL;
    bp(x) = p;
    bn(x) = n;
    bv(x) = n;
    ba(x) = ptr;
    numBlocks++; //debug
    R x;
}

void useBuddySystem(){
    gRoot = newBlock(INIT_HEAP_SIZE, NULL, allocHeap(INIT_HEAP_SIZE));
}

void updateBlock(BN rt){
    if(rt && bp(rt)){
        BN parent  = bp(rt);
        bn(parent) = bn(bx(parent)) + bn(by(parent));
        updateBlock(parent);
    }
}

BN addBlock(BN rt, L n){
    if(H_DEBUG)
        WP("n = %lld, size = %lld\n",n, rt->size);
    if(n <= bn(rt)){
        if(bx(rt) == NULL && by(rt) == NULL){
            if((n<<1) <= bn(rt) && bn(rt)>256){
                L half = bn(rt)>>1;
                G hptr = ba(rt);
                bx(rt) = newBlock(half,rt,hptr);
                by(rt) = newBlock(half,rt,hptr+half);
                R addBlock(bx(rt), n);
            }
            else {
                bn(rt) = 0;
                updateBlock(rt);
                R rt;
            }
        }
        else if(bn(bx(rt)) < n){
            R addBlock(by(rt), n);
        }
        else if(bn(by(rt)) < n){
            R addBlock(bx(rt), n);
        }
        else if(bn(bx(rt)) < bn(by(rt))){
            R addBlock(bx(rt), n);
        }
        else {
            R addBlock(by(rt), n);
        }
    }
    else {
        FS("full ...\n");
        exit(99);
    }
    R NULL; 
}

void releaseBlock(BN rt){
    if(rt){
        bn(rt) = bv(rt);
        ba(rt) = NULL;
        updateBlock(rt);
    }
}

void freeBlock(BN rt){
    if(rt){
        freeBlock(bx(rt));
        freeBlock(by(rt));
        free(rt);
    }
}

void printTree(BN rt){
    FS("(");
    if(rt){
        if(ba(rt))
            FT("%ld", ba(rt)-ba(gRoot));
        else
            FS("NULL");
        if(rt->left){
            printTree(bx(rt));
            printTree(by(rt));
        }
    }
    FS(")");
}

void printRoot(BN rt){
    printTree(rt);
    FS("\n");
}

G allocBlock(I typ, L len){
    const L refNum = sizeof(L);
    L size = refNum + getTypeSize(typ, len);
    BN t = addBlock(gRoot, size);
    R ba(t)+refNum;
}

/*
 * Initial: 1GB (minimal block size 64 bytes)
 * Worst case: 2^20/2^6 = 2^14 blocks (about 2^14 * 48 = 768MB)
 * Worse case percent: sizeof(BN0)/minBlockSize * 100%
 */
L testMemory(){
    printf("Test memory: buddy system\n");
    // BN root   = newBlock(512, NULL, allocHeap(512));
    BN root = gRoot;
    BN work_a = addBlock(root, 70); //add(A, 70)
    printRoot(root);
    BN work_b = addBlock(root, 35); //add(B, 35)
    printRoot(root);
    BN work_c = addBlock(root, 80); //add(C, 80)
    printRoot(root);
    WP("> end (A, 70)\n");
    releaseBlock(work_a);            //end A
    printRoot(root);
    BN work_d = addBlock(root, 60); //add(D, 80)
    printRoot(root);
    WP("> end (B, 35)\n");
    releaseBlock(work_b);            //end B
    printRoot(root);
    WP("> end (D, 80)\n");
    releaseBlock(work_d);            //end D
    printRoot(root);
    WP("> end (C, 80)\n");
    releaseBlock(work_c);            //end C
    printRoot(root);
    freeBlock(root);
    WP("numBlocks = %lld, total mem = %lld out of %lld (%g%%)\n", \
        numBlocks, \
        numBlocks*sizeof(BN0), \
        INIT_HEAP_SIZE, \
        numBlocks*sizeof(BN0)*100.0/INIT_HEAP_SIZE \
        );
    return 0;
}

#define getRefPtr(x) (L*)(x-sizeof(BN0))

L getRefCount(G x){
    R *(getRefPtr(x));
}

L updateRefCount(G x){
    L *n = getRefPtr(x);
    R ++(*n);
}

void printHeapInfo(){
    #ifdef USE_NAIVE
    // if(H_DEBUG)
        WP("-> [Info heap] Init. %lld, used %lld (%lf%%)\n", \
            INIT_HEAP_SIZE, \
            gHeapCur, \
            gHeapCur*100.0/INIT_HEAP_SIZE);
        // WP("gHeap = 0x%016x, gHeapCur = %lld\n",gHeap,gHeapCur);
    #endif

    #ifdef USE_BUDDY
        WP("-> [Buddy heap] Init. %lld, used %lld/%lld, extra %lld (%g%%)\n", \
            INIT_HEAP_SIZE, \
            gRoot->size,    \
            gRoot->level,   \
            numBlocks*sizeof(BN0), \
            numBlocks*sizeof(BN0)*100.0/INIT_HEAP_SIZE \
            );
        WP("-> [Buddy heap] "); printRoot(gRoot);
    #endif
}


/* Heap allocation for heap */

void initHashHeap(){
    hHeap = (G)malloc(INIT_HASH_SIZE);
    hHeapCur = 0;
    memset(hHeap, 0, INIT_HASH_SIZE);
    if(H_DEBUG)
        WP("-> [Info hash heap] Successfully initialized\n");
}

G allocHashMem(L size){
    if((size&0x3)!=0){
        size = ((size>>4)+1)<<4;
    }
    if(hHeapCur+size < INIT_HASH_SIZE){
        G g = hHeap + hHeapCur;
        hHeapCur += size;
        R g;
    }
    else {
        EP("Hash heap full!! (%lld+%lld)",hHeapCur,size);
    }
}

L getHashHeap(){
    R hHeapCur;
}

void setHashHeap(L x){
    if(x>=0 && x<INIT_HASH_SIZE) {
        if(x<hHeapCur){ 
            memset(hHeap+x, 0, hHeapCur-x);
        }
        if(H_DEBUG)
            WP("[HashHeap] Setting pointer from %lld to %lld\n",hHeapCur,x);
        hHeapCur=x;
    }
    else EP("Unexpected hash heap size: %lld [0,%lld)",x,INIT_HASH_SIZE);
}

void printHashInfo(){
    WP("-> [Info hash heap] usage = %.2lf%% (%lld/%lld)\n", \
        hHeapCur*100.0/INIT_HASH_SIZE,hHeapCur,INIT_HASH_SIZE);
}

