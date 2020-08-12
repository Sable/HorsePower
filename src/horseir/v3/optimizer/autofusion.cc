#include "../global.h"

typedef struct codegen_node{
    C kind;
    S funcName;
    B isMarked;
    S codeStr;   // if (isMarked) print codeStr
    I pnum;      // # of parameter
    I numLast;
    I numUse;    // # of used inside gNode
    Node *node;
    Node *lastNodes[5];
    Chain *gChain; // chain points to node
    struct codegen_node *pnode[5]; // parameter nodes
}gNode;

typedef struct gNode_list{
    gNode *g;
    Node  *lastNodes[10];
    I      numLast;
    struct gNode_list *next;
}gNodeList;

typedef struct fusable_vector_node{
    // nodes for input & output in a fusible section
    // loop size / body / condition / reduction
    // is reduction
}fVecNode;

typedef struct fusable_list_node{
    I  fuseId;   // phTotal
    I  varNum;
    S *varNames; // local vars
    S  strInit;
    B  isList;
    Node *nodeOpt;  // entry point for optimizing nodes
    Node *nodeRtn;
    Node *nodeIter;
    gNode *gnode;
    // add more
}fListNode;

typedef enum {
    unknownR, sumR, avgR, minR, maxR, allR, anyR, lenR
} ReductionKind;

#define NUM_R 7

static const char *ReductionNames[NUM_R] = {
    "sum", "avg", "min", "max", "all", "any", "len"
};

extern List *compiledMethodList;
extern I qid, phTotal;
extern sHashTable *hashOpt;

extern I varNum;
extern S varNames[99];
extern C code[CODE_MAX_SIZE], *ptr;

static S code_cond;
static sHashTable *hashgNode;
static gNodeList *glist;

static fListNode *fList[100];
static I fListNum, curNum;
static I depth;
static B fuseFlag[100];
static B hasCompress, findCompress;
C localIter;

#define OPT_DEBUG false

#define isGroupElem(x) isElementwise(x)
#define isGroupScan(x) (sEQ(x,"compress")||sEQ(x,"where"))
#define isGroupIndex(x) sEQ(x,"index")
#define isGroupBool(x) (sEQ(x,"member")||sEQ(x,"like"))
// isGroupEach: defined as a function

#define isKindR(rt) (rt->kind == 'R')
#define isKindS(rt) (rt->kind == 'S')
#define isKindX(rt) (rt->kind == 'X')
#define isKindE(rt) (rt->kind == 'E')
#define isKindB(rt) (rt->kind == 'B')
#define isKindH(rt) (rt->kind == 'H')

static void genCodeAuto             (gNode *rt, B isRT);
static void genCodeAutoList         (gNodeList *list, I size);
static void genCodeAutoListNode     (gNode *rt, L id);
static void genCodeAutoListSingleLen(gNode *rt);
static void fixCodeAutoListSingleLen(fListNode *f, C iter);

static I genLocalVars(S *names, I n);

/* ---------- Above declarations ----------  */

 // copy from optimizer/pattern.c
static void genIndent(){ DOI(depth, glueCode(indent4)); }
static B nextCond(Node *x, Node *next);

static void debugCode(S msg){
    WP("Output code (%s):\n %s\n", msg,code);
    getchar();
}

static void debugFusionNode(L id){
    Node *tmp = fList[id]->nodeOpt;
    L x = lookupSimpleHash(hashOpt, (L)tmp);
    ChainExtra *extra = (ChainExtra*)x;
    WP("visible: %s, %lld, x = %lld\n", getExtraKind(extra->kind), (L)tmp, x);
    printNode(tmp); getchar();
}

static void addToSimpleHashUnsafe(sHashTable *st, L key, L val){
    if(!lookupSimpleHash(st, key)){
        addToSimpleHash(st, key, val);
    }
    else {
        // ignore the latest update
        // WP("ignore: key = %lld, val = %lld\n", key, val);
    }
}

static void addToSimpleHashForce(sHashTable *st, L key, L val){
    addToSimpleHash(st, key, val);
}

static void freegNodeList(gNodeList *g){
    while(g){
        gNodeList *t = g; g = g->next; free(t);
    }
}

static B isGroupReduction(S funcName){
    DOI(NUM_R, if(sEQ(funcName, ReductionNames[i])) R true) R false;
}

Node *getStmtCall(Node *stmt){
    Node *n = nodeStmtExpr(stmt);
    if(instanceOf(n, callK)) R n;
    else if(instanceOf(n, castK)){
        n = n->val.cast.exp;
        R (instanceOf(n, callK))?n:NULL;
    }
    else R NULL;
}

static B isGroupEach(S funcName){
    R sEQ(funcName, "each")      || sEQ(funcName, "each_item") ||
      sEQ(funcName, "each_left") || sEQ(funcName, "each_right");
}

static C getFuncKind(S func){
    if(isGroupReduction(func)) R 'R';
    else if(isGroupScan(func)) R 'S';
    else if(isGroupIndex(func))R 'X';
    else if(isGroupBool(func)) R 'B';
    else if(isGroupElem(func)) R 'E';
    else if(isGroupEach(func)) R 'H';
    else R 0; // group: 'O'
}

S getCallName(Node *call){
    Node *func    = nodeCallFunc(call);
    SymbolKind sk = nodeNameKind(func);
    R (builtinS == sk)? nodeName2(func): 0;
}

C getCallKind(Node *call){
    Node *func    = nodeCallFunc(call);
    SymbolKind sk = nodeNameKind(func);
    R (builtinS == sk)? getFuncKind(nodeName2(func)): 0;
}

static B isCallKindEach(Node *call){
    Node *func    = nodeCallFunc(call);
    SymbolKind sk = nodeNameKind(func);
    R (builtinS == sk)? isGroupEach(nodeName2(func)) : 0;
}

static B isCallRaze(Node *call){
    Node *func    = nodeCallFunc(call);
    SymbolKind sk = nodeNameKind(func);
    R (builtinS == sk)? sEQ(nodeName2(func), "raze") : 0;
}

static gNode *initgNode(Node *node, C kind, S funcName){
    gNode *x = NEW(gNode);
    x->kind  = kind;
    x->node  = node;
    x->funcName = funcName;
    x->pnum  = totalList(getNodeParams(node));
    if(x->pnum >= 5)
        EP("Not enough space");
    return x;
}

static S getCallFuncName(Node *n){
    if(instanceOf(n, stmtK)){
        Node *call = getStmtCall(n);
        R call?nodeName2(nodeCallFunc(call)):NULL;
    }
    R NULL;
}

static gNode *findFusionIndex(Chain *chain);

static I getFusionLastNode(gNodeList *glist, gNode *rt){
    I k = 0;
    DOI(rt->pnum, if(rt->pnode[i]) k+=getFusionLastNode(glist, rt->pnode[i]))
    if(rt->numLast > 0){
        DOI(rt->numLast, glist->lastNodes[glist->numLast++] = rt->lastNodes[i])
    }
    R k + rt->numLast;
}

static gNodeList *initgNodeList(gNode *g){
    gNodeList *x = NEW(gNodeList);
    x->g = g;
    getFusionLastNode(x, g);
    R x;
}

static void insertgNode(gNodeList *glist, gNode *g){
    gNodeList *x = initgNodeList(g);
    x->next      = glist->next;
    glist->next  = x;
}

static fListNode *initfListNode(I fuseId){
    fListNode *x = NEW(fListNode);
    x->fuseId = fuseId;
    R x;
}

static C nextKind(Node *n){
    if(instanceOf(n, stmtK)){
        Node *callStmt = getStmtCall(n);
        return callStmt?getCallKind(callStmt):0;
    }
    R 0;
}

static C nextType(Node *n){
    C nextK = nextKind(n);
    if(nextK == 'X'){
        Node *x0 = getNodeItemIndex(n, 1); // index(x,y); --> x
        R getTypeCodeByName(x0);
    }
    R 0;
}

static B isOK2Fuse(gNode *rt){
    // condition: more than 1 stmt
    DOI(rt->pnum, if(rt->pnode[i])R 1) R 0;
}

/* -1: not a name; 0: name with def; 1/2: name, but invalid */
I checkParamDef(Node *p, Chain *chain, I *c){
    I status = -1;
    if(instanceOf(p, nameK)){
        SymbolKind sk = nodeNameKind(p);
        if(sk == localS){
            *c = findDefByName(chain, nodeName2(p));
            status = (*c) < 0? 1:0;
        }
        else status = 2;
    }
    R status;
}

// TODO: improve the code quality of optimizer/elementwise.c:findFusionUp
static gNode *findFusionUp(Chain *chain, B isRT, B isRT_R){
    if(isChainVisited(chain)) R NULL;
    //else setVisited(chain, true);
    Node *n = chainNode(chain);
    B isReduction = isRT_R;
    if(instanceOf(n, stmtK)){
        List *vars = nodeStmtVars(n);
        Node *call = getStmtCall(n);
        if(call){
            C kind = getCallKind(call);
            S funcName = getCallName(call);
            if(kind){
                if(kind=='R' && !isRT) R NULL;
            }
            else R NULL;
            if(isRT){ // root support
                switch(kind){
                    case 'R': isReduction = true;
                    case 'S':
                    case 'E': break;
                    case 'X': R findFusionIndex(chain);
                    default: R NULL;
                }
            }
            setVisited(chain, true); // set visited
            List *param = getNodeParams(n);
            //printChain(chain); getchar();
            gNode *rt = initgNode(n, kind, nodeName2(nodeCallFunc(call)));
            rt->gChain = chain;
            B flag[5]; DOI(5, flag[i]=0)
            switch(kind){ // mark parameters
                case 'E': 
                case 'S': flag[0] = flag[1] = 1; break;
                case 'X':
                case 'B': flag[0] = 1; break; // 2nd param
                case 'R': flag[0] = 1; break; // 1st param
            }
            // if(kind == 'S')
            //     findCompress = true;
            I c = 0, cnt = 0;
            while(param){
                if(flag[cnt]){
                    switch(checkParamDef(param->val, chain, &c)){
                        case -1: rt->pnode[cnt] = NULL; break;
                        case  0: {
                            Chain *next = chain->chain_defs[c];
                            if(nextCond(chainNode(chain), chainNode(next)));
                            else if(isChainVisited(next)){
                                L rtn = lookupSimpleHash(hashgNode, (L)next);
                                rt->pnode[cnt] = rtn?(gNode*)rtn:NULL;
                                //if(rtn){
                                //    gNode *t = (gNode*)rtn;
                                //    WP(".....\n"); printNode(t->node); getchar();
                                //}
                            }
                            else {
                                WP("- current node: "); printNode(chainNode(chain));
                                WP("> next node:"); printNode(chainNode(next));
                                C nextK = nextKind(chainNode(next));
                                C nextC = nextType(chainNode(next));
                                Node *nextCall = getStmtCall(chainNode(next));
                                S nextFuncName = nextCall?getCallName(nextCall):NULL;
                                // WP("nextK = %c, nextC = %c\n", nextK, nextC);
                                // WP("[%d] func = %s (%c), nextFuncName = %s (%c)\n", isRT, funcName,kind, nextFuncName, nextK); getchar();
                                if('X' == nextK && 'G' == nextC){
                                    rt->pnode[cnt] = NULL; // skip list indexing
                                }
                                else if('E' == kind && 'X' == nextK);
                                else if(isRT && 'S' == kind && 'X' == nextK){
                                    R NULL;
                                }
                                else if(isRT && 'S' == kind && 'E' == nextK){
                                    R NULL;
                                }
                                else if(isRT && 'X' == kind && 'S' == nextK){
                                    R NULL; // avoid index + where
                                }
                                else if(isRT && 'S' == kind && kind == nextK && nextFuncName != NULL && sNEQ(funcName, nextFuncName)){
                                    R NULL;
                                }
                                else if(!isReduction && 'E' == kind && 'S' == nextK){
                                    rt->pnode[cnt] = NULL;
                                }
                                else{
                                    rt->pnode[cnt] = findFusionUp(next, false, isReduction);
                                }
                            }
                        } break;
                        case 1: case 2: free(rt); R NULL;
                    }
                }
                else rt->pnode[cnt] = NULL;
                param = param->next;
                cnt++;
            }
            // printNode(chainNode(chain));
            // DOI(cnt, P("case[%lld] = %d, flag[%lld] = %d\n",i,rt->pnode[i]==NULL,i,flag[i]))
            // getchar();
            //WP("----add\n"); printChain(chain); WP("\n");
            addToSimpleHashUnsafe(hashgNode, (L)chain, (L)(rt));
            R rt;
        }
    }
    R NULL;
}

static gNode *findFusionEach(Chain *chain, B isRT){
    if(isChainVisited(chain)) R NULL;
    Node *n = chainNode(chain);
    if(instanceOf(n, stmtK)){
        Node *call = getStmtCall(n);
        if(isCallKindEach(call)){
            Node *func = getSingleFunc(getNodeFirstParam(call));
            if(func){
                setVisited(chain, true);
                S funcEach = nodeName2(func); // 1st param
                C kind = getFuncKind(funcEach);
                //WP("func: %s, %c\n", funcEach, kind); getchar();
                if(isRT && kind != 'R') R NULL;
                gNode *rt = initgNode(n, kind, funcEach);
                rt->gChain = chain;
                B flag[5]; DOI(5, flag[i]=0)
                switch(kind){ // mark parameters
                    case 'E': 
                    case 'S': flag[0] = flag[1] = 1; break;
                    case 'X':
                    case 'B': flag[0] = 1; break; // 2nd param
                    case 'R': flag[0] = 1; break; // 1st param
                }
                List *param = getNodeParams(n);
                I c = 0, cnt = 0;
                while(param->next){
                    if(flag[cnt]){
                        switch(checkParamDef(param->val, chain, &c)){
                            case -1: rt->pnode[cnt] = NULL; break;
                            case  0: {
                                Chain *next = chainDef(chain, c);
                                if(isChainVisited(next)){
                                    L rtn = lookupSimpleHash(hashgNode, (L)next);
                                    rt->pnode[cnt] = rtn?(gNode*)rtn:NULL;
                                }
                                else {
                                    gNode *gNext = findFusionEach(next, false);
                                    rt->pnode[cnt] = gNext;
                                    if(!gNext) {
                                        rt->lastNodes[rt->numLast++] = chainNode(next);
                                    }
                                }
                            } break;
                            case 1: case 2: free(rt); R NULL;
                        }
                    }
                    else rt->pnode[cnt] = NULL;
                    param = param->next;
                    cnt++;
                }
                addToSimpleHashUnsafe(hashgNode, (L)chain, (L)rt);
                R rt;
            }
        }
    }
    R NULL;
}

/*
 * Starting from @raze
 */
static gNode *findFusionUpList(Chain *chain){
    if(!isChainVisited(chain)) {
        setVisited(chain, true);
        //printChain(chain); getchar();
        Node *n = chainNode(chain);
        if(instanceOf(n, stmtK)){
            List *param = getNodeParams(n);
            if(chainDefSize(chain) == 1){
                Chain *next = chainDef(chain, 0);
                R findFusionEach(next, true);
            }
        }
    }
    R NULL;
}

static ReductionKind getReductionKind(S name){
    if(sEQ(name, "sum")) R sumR;
    else if(sEQ(name, "avg")) R avgR;
    else if(sEQ(name, "min")) R minR;
    else if(sEQ(name, "max")) R maxR;
    else if(sEQ(name, "all")) R allR;
    else if(sEQ(name, "any")) R anyR;
    else if(sEQ(name, "len")) R lenR;
    else R unknownR;
}

static const char *getMinMax(C c){
    switch(c){
        case 'B':
        case 'J':
        case 'H':
        case 'I':
        case 'L': R "INT";
        case 'F': R "FLT";
        case 'E': R "DLT";
        default: TODO("Add impl. %c", c);
    }
}

static const char *obtainReductionOp(ReductionKind k){
    switch(k){
        case sumR:
        case avgR: R "+";
        case minR: R "min";
        case maxR: R "max";
        case allR:
        case anyR:
        default: TODO("Impl. %d", k);
    }
}

/* ------ copy from: elementwise.c ------ */

extern L constIdNum;
extern S constId[99];

static const char *obtainConstSymbolId(Node *p){
    ConstValue *v = p->val.nodeC;
    if(v->type == symC){
        L id = -1;
        DOI(constIdNum, if(sEQ(constId[i],v->valS)){id=i;break;})
        if(id < 0){
            id = constIdNum++;
            constId[id] = v->valS;
        }
        C temp[99];
        SP(temp, "id%lld", id);
        R strdup(temp);
    }
    R NULL;
}

static void totalInputs(List *list, gNode *rt, I dep, S *names){
#define isLeafNode(n) !lookupSimpleHash(hashgNode, (L)(n))
    if(list){
        totalInputs(list->next, rt, dep+1, names);
        gNode *t = rt->pnode[dep];
        if(t) totalInputs(getNodeParams(t->node), t, 0, names);
        else {
            Node *p = list->val;
            if(instanceOf(p,nameK) && !isDuplicated(names, nodeName2(p))){
                //WP("p: "); printNode(p);
                //WP("rt->node: "); printNode(rt->node); getchar();
                if(isLeafNode(p))
                    names[varNum++] = nodeName2(p);
                //else WP("skip .... %s\n", nodeName2(p));
            }
            else {
                Node *c = getSingleSymbol(p);
                if(c){ // single const symbol
                    S idStr = (S)obtainConstSymbolId(c);
                    if(isLeafNode(p) && idStr){
                        names[varNum++] = idStr;
                        //STOP("idStr = %s\n", idStr);
                    }
                }
            }
            //update sequence in names:
            //    name1,id,name2 --> name1,name2,id
            S temp[99]; L tt = 0, k = 0;
            DOI(varNum, if(!strncmp(names[i],"id",2))temp[tt++]=names[i])
            DOI(varNum, if(strncmp(names[i],"id",2))names[k++]=names[i])
            DOI(tt, names[k++]=temp[i])
        }
    }
}

/* ------ copy ends ----- */

static L printgNodeCore(gNode *pt, gNode *rt, I dep, sHashTable *flag){
    L c = 0;
    if(rt){
        if(pt){
            WP(indent4 "\"%s\" -> \"%s\"\n", \
                    obtainNodeStr(pt->node), \
                    obtainNodeStr(rt->node));
            // WP(" (%d)\n",rt->numUse);
        }
        if(!lookupSimpleHash(flag,(L)rt)){
            if(pt) c++;
            DOI(rt->pnum, c+=printgNodeCore(rt, rt->pnode[i], dep+1, flag))
            addToSimpleHash(flag, (L)rt, 1);
        }
    }
    R c;
}

static L loadParamsVec(Node **rtn, List *list){
    if(list){
        L k = loadParamsVec(rtn, list->next);
        rtn[k] = list->val;
        R k + 1;
    }
    R 0;
}


/*
 * Input : a fuisble section
 * Output: a fuisble section in dot format
 * Return: number of statments
 * Function: output to stderr
 */
static L printgNode(gNode *rt){
    sHashTable *flag = initSimpleHash(1<<10);
    WP("digraph G {\n");
    L c = printgNodeCore(NULL, rt, 0, flag) + (NULL != rt);
    WP("}\n");
    freeSimpleHash(flag);
    R c;
}

static void printgNodeList(){
    // skip dummy node
    if(glist && glist->next)
        printgNode(glist->next->g);
    else
        EP("Invalid glist");
}

static void countUsegNode(gNode *rt){
    if(rt){
        if(rt->numUse == 0) {
            rt->numUse = 1;
            DOI(rt->pnum, countUsegNode(rt->pnode[i]))
        }
        else {
            rt->numUse++;
        }
    }
}

static I trimgNode(gNode *rt){
    if(rt){
        Chain *chain = rt->gChain;
        // printNode(rt->node);
        if(chain->useSize != rt->numUse){
            if(OPT_DEBUG)
                WP("-- useSize = %d vs. numUse = %d\n", chain->useSize, rt->numUse);
            R 1;
        }
        else {
            if(OPT_DEBUG)
                WP("++ useSize = %d vs. numUse = %d\n", chain->useSize, rt->numUse);
            DOI(rt->pnum, if(trimgNode(rt->pnode[i])){rt->pnode[i]=NULL;})
            R 0;
        }
    }
    R 1;
}

static void cleargNodeUse(gNode *rt){
    if(rt){
        rt->numUse = 0;
        DOI(rt->pnum, cleargNodeUse(rt->pnode[i]))
    }
}

static void setgNodeUnvisited(gNode *rt){
    if(rt){
        setVisited(rt->gChain, false);
        DOI(rt->pnum, setgNodeUnvisited(rt->pnode[i]))
    }
}

static void trimgNodeFromTop(gNode *rt){
    if(rt){
        cleargNodeUse(rt); // set to 0
        DOI(rt->pnum, countUsegNode(rt->pnode[i]));
        DOI(rt->pnum, if(trimgNode(rt->pnode[i])){ \
                setgNodeUnvisited(rt->pnode[i]); \
                rt->pnode[i]=NULL; })
            //printgNode(rt);
            //STOP("trim ...");
    }
}

static I countValidStmts(gNode *rt){
    I c = 0;
    if(rt && isKindE(rt)){
        c++;
        DOI(rt->pnum, c+=countValidStmts(rt->pnode[i]))
    }
    R c;
}

// pnode stores gNode in the reversed order
// if(rt->pnode[0]) { WP("0\n"); printNode(rt->pnode[0]->node); }
// if(rt->pnode[1]) { WP("1\n"); printNode(rt->pnode[1]->node); } // take
static B gNodeHeuristic1(gNode *rt){
    Node *n = rt->node;
    S fn = getCallFuncName(n);
    if(sEQ(fn, "compress") && !(rt->pnode[0])){
        I numStmts = countValidStmts(rt->pnode[1]);
        R (numStmts > 5); // q19: 6
    }
    R false;
}

static void updategNodeWithOnlyE(gNode *rt){
    if(rt){
        DOI(rt->pnum, {gNode *next=rt->pnode[i];
                if(next){
                  if(!isKindE(next)) rt->pnode[i]=NULL;
                  else updategNodeWithOnlyE(next); }
                })
    }
}

static B gNodeHeuristic2(gNode *rt){
    if(isKindE(rt)){
        I numStmts = countValidStmts(rt);
        //WP("numStmts = %d\n", numStmts);
        if(numStmts > 10){
            updategNodeWithOnlyE(rt);
            R true;
        }
    }
    R false;
}

static gNode *analyzegNode(gNode *rt){
    if(!rt) R NULL;
    if(gNodeHeuristic1(rt)){
        R rt->pnode[1]; // return boolean side
    }
    if(gNodeHeuristic2(rt)){
        //printgNode(rt); getchar();
        R rt;
    }
    R rt;
}


static void printNameArray(S *names, I n){
    WP("Local names (total %d):\n", n);
    DOI(n, WP("- %s\n", names[i]))
}

static void printAllNames(){
    printNameArray(varNames, varNum);
}

static L searchNameArray(S *names, I n, S s){
    DOI(n, if(sEQ(names[i],s))R i) R -1;
}

static void genCodeBody(Node *p, gNode *g){
    if(g) genCodeAuto(g, false);
    else {
        if(instanceOf(p,nameK))
            genCodeName(p,searchName(varNames,nodeName2(p)));
        else genCodeNode(p);
    }
}

static void genCodeBodyIndex(Node *p, gNode *g){
    if(g) genCodeAuto(g, false);
    else {
        if(instanceOf(p,nameK))
            genCodeIndex(p,searchName(varNames,nodeName2(p)));
        else genCodeNode(p);
    }
}

static void genCodeScanCond(Node *p, gNode *g){
    resetCode(); S oldPtr = ptr;
    genCodeBody(p, g);
    code_cond = strdup(oldPtr); 
    ptr = oldPtr;
    ptr[0] = 0; // clean the remaining part
}

static void genCodeParamReversed(List *list, gNode *rt, I dep){
    if(list){
        genCodeParamReversed(list->next, rt, dep+1);
        if(list->next) glueChar(comma);
        gNode *t = rt->pnode[dep];
        // WP("dep = %d, t = %d\n", dep, t==NULL); getchar();
        if(t) genCodeAuto(t, false);
        else genCodeBody(list->val, t);
    }
}

// index(A, x)
static void genCodeParamReversedFuncIndex(List *list, gNode *rt, I dep){
    // 1. gen the A part
    gNode *t1 = rt->pnode[1];
    Node *x0 = list->next->val;
    C x0c = getTypeCodeByName(x0);
    S x0s = nodeName2(x0);
    L xid = searchName(varNames, x0s);
    glueAny("v%c(x%lld, ",x0c,xid);
    // 2. generate the x part
    gNode *t0 = rt->pnode[0];
    if(t0) genCodeAuto(t0, false);
    else genCodeBody(list->val, t0);
    // 3. glue the remaining part
    glueAny(")");
}


static void genCodeParamMemberReversed(List *list, gNode *rt, I dep){
    Node *p0 = list->next->val; // 1st param
    Node *p1 = list->val;       // 2nd param
    genCodeBody(p0, NULL);
    glueChar(comma);
    glueAny("x%lld", searchName(varNames,nodeName2(p1)));
}


static void fixCodeParamReversed(List *list, gNode *rt, L id){
    B isTryOK = false;
    I total = rt->pnum;
    Node **rtn = NEW2(Node, total);
    loadParamsVec(rtn, list);
    // WP("params: %d, %d\n", total,rt->pnum);
    // DOI(total, {WP("%lld\n",i); printNode(rtn[i]);}) getchar();
    // important: reversed in rt->pnode
    // printgNode(rt);
    gNode *t=rt->pnode[0]; // index for the 2nd parameter
    // WP("t is NULL: %d\n", t==NULL);
    if(t) {
        Node *next = t->node;
        // printNode(next);
        if(sEQ(getCallFuncName(next), "compress")){
            isTryOK = true;
            Node *z0 = getNodeItemIndex(rt->node, 1);//pfnIndex(t79, t51, t56)
            C z0c = getTypeCodeByName(z0);
            S z0s = nodeName2(z0);
            L zid = searchName(varNames, z0s);
            Node *k0 = getNodeItemIndex(next,2); // pfnCompress(t56, t27, t55))
            C k0c = getTypeCodeByName(k0);
            S k0s = nodeName2(k0);
            L kid = searchName(varNames, k0s);
            glueAny("v%c(x%lld,vL(x%lld,%c));", z0c,zid,kid,localIter);
        }
        else if(sEQ(getCallFuncName(next), "where")){
            isTryOK = true;
            Node *z0 = getNodeItemIndex(rt->node,1); // t57:? = @where(t27);
            C z0c = getTypeCodeByName(z0);
            S z0s = nodeName2(z0);
            // WP("name2 = %s\n", z0s); getchar();
            L zid = searchName(varNames, z0s);
            glueAny("v%c(x%lld,%c);", z0c,zid,localIter);
        }
        else {
            EP("Unknown next function: %s", getCallFuncName(next));
        }
    }
    // WP("isTryOK = %d\n", isTryOK); getchar();
    if(!isTryOK){
        // genCodeParamReversed(list, rt, id);
        genCodeParamReversedFuncIndex(list, rt, id);
    }
    // TODO: free rtn
}

static void genLocalSizeExpr(I x, I k, I *nums){
    if(x<k){
        glueAny("(vn(x%d)==1?",nums[x]);
        genLocalSizeExpr(x+1,k,nums);
        glueAny(":vn(x%d))",nums[x]);
    }
    else {
        glueAny("1");
    }
}

// Todo: not a perfect solution (better to check all local vars to see if it is a scalar or not)
static void genLocalSize(S *names, I n){
    I nums[99], c=0;
    DOI(n, if(strncmp(names[i], "id", 2))nums[c++]=(I)i)
    if(c == 1)
        glueAny(indent4 "L r0=vn(x%d);\n", nums[0]);
    else if(c>1){
        glueAny(indent4 "L r0=");
        genLocalSizeExpr(0, c, nums);
        glueAny(";\n");
    }
}

static I findSizeR0ByNode(Node *n){
    if(instanceOf(n, nameK) && !isScalarShapeByName(n)){
        S name = nodeName2(n);
        L id = searchName(varNames, name);
        // printAllNames();
        // WP("name = %s, id = %lld\n", name,id); getchar();
        R (I)id;
    }
    R -1;
}

static I findSizeR0(gNode *rt){
    // printgNode(rt);
    if(!rt) R -1;
    List *param = getNodeParams(rt->node);
    I c = 0;
    B flag[5]; DOI(5, flag[i]=0)
    switch(rt->kind){ // mark parameters (not the same as findFusionUp)
        case 'E': 
        case 'S': flag[0] = flag[1] = 1; break;
        case 'X': flag[0] = 1; break; // 2nd param
        case 'B': flag[1] = 1; break; // 1st param
        case 'R': flag[0] = 1; break; // 1st param
    }
    while(param){
        Node *p = param->val;
        if(flag[c] && !(rt->pnode[c])){
            I id = findSizeR0ByNode(p);
            if(id >= 0) R id;
        }
        c++;
        param=param->next;
    }
    if(isKindR(rt)){
        R findSizeR0(rt->pnode[0]); // 1st
    }
    else if(isKindX(rt)){
        R findSizeR0(rt->pnode[0]); // 2nd
    }
    else if(isKindE(rt)){
        I p0 = findSizeR0(rt->pnode[0]);
        if(p0 < 0){
            R findSizeR0(rt->pnode[1]);
        }
        else R p0;
    }
    else if(isKindS(rt)){
        if(rt->pnum == 2){ // compress
            I p0 = findSizeR0(rt->pnode[0]);
            if(p0 < 0){
                R findSizeR0(rt->pnode[1]);
            }
            else R p0;
        }
        else if(rt->pnum == 1) {
            R findSizeR0(rt->pnode[0]);  // 1st or 2nd
        }
        else {
            EP("Unkown: %d", rt->pnum);
        }
    }
    else if(isKindB(rt)){
        R findSizeR0(rt->pnode[1]);  // 1st
    }
    else {
        EP("Unknown kind");
    }
}

static void genCodeAuto(gNode *rt, B isRT){
    //if(rt->isMarked){
    //    //debugCode((S)"LINE 452");
    //    //glueCode(rt->codeStr);
    //    glueCode("<fix marked>");
    //    R ;
    //}
    //else rt->isMarked = true;
    ChainExtra *extra = NEW(ChainExtra);
    extra->kind = isRT?OptG:SkipG;
    Node *n = rt->node;
    C temp[199], last[199];
    S oldPtr = NULL;
    if(isRT){
        Node *z0 = getNodeItemIndex(n,0);
        S z0s = getNameStr(z0);
        C z0c = getTypeCodeByName(z0);
        SP(temp, "q%d_autofusion_%d",qid,phTotal++);
        glueCode(genDeclSingle(temp, '{')); glueLine();
        //varNum = 0;
        //totalInputs(getNodeParams(n), rt, 0, varNames);
        genLocalVars(varNames, varNum);
        //DOI(varNum, glueAny(indent4 "V x%lld=x[%lld]; // %s\n",i,i,varNames[i]))
        //printAllNames(); getchar();
        // genLocalSize(varNames, varNum);
        I sizeR0=findSizeR0(rt);
        glueAny(indent4 "L r0=vn(x%d);\n", sizeR0<0?0:sizeR0);
        if(isKindR(rt)){
            glueAny(indent4);
            ReductionKind rkind = getReductionKind(rt->funcName);
            switch(rkind){
                case sumR: 
                case avgR: glueAny("%c c = 0;", z0c); break;
                case minR: glueAny("%c c = %s_MAX;", z0c,getMinMax(z0c)); break;
                case maxR: glueAny("%c c = %s_MIN;", z0c,getMinMax(z0c)); break;
                case allR: glueAny("%c c = 1;", z0c); break;
                case anyR: glueAny("%c c = 0;", z0c); break;
                default: TODO("handle reduction op: %s", rt->funcName); break;
            }
            glueAny(" L tot=0;\n");
            glueAny(indent4 "initV(z, H_%c, 1);\n", z0c);
            switch(rkind){
                case avgR: SP(last, "v%c(z,0) = c/tot;",z0c); break;
                default: SP(last, "v%c(z,0) = c;", z0c); break;
            }
            glueAny(indent4 "DOP(r0, ");
        }
        else if(isKindE(rt)){
            glueAny(indent4 "/* Fusion: kind E */\n");
            glueAny(indent4 "initV(z, H_%c, r0);\n", z0c);
            glueAny(indent4 "DOP(r0, v%c(z,i)=", z0c);
        }
        else if(isKindS(rt)){
            //debugCode((S)"LINE 492");
            //TODO("Add scan");
        }
        else {
            WP("%s\n", obtainNodeStr(rt->node));
            TODO("Add support for kind: %c", rt->kind);
        }
        extra->funcDecl = genDeclSingle(temp, ';');
        extra->funcInvc = genInvcSingle(z0s, temp, varNames, varNum);
        resetCode(); oldPtr = ptr; hasCompress = false;
    }
    if(isKindS(rt)){
        hasCompress = true;
        //printNode(n);
        List *param = getNodeParams(n);
        I total = totalVar(param);
        Node *p0 = param->val;
        if(total == 2){ //compress
            Node *p1 = param->next->val;
            if(!code_cond){ // TODO: only one scan allowed currently
                genCodeScanCond(p1, rt->pnode[1]); // 1st param
                //WP("cond: %s\n", code_cond); getchar();
            }
            genCodeBody(p0, rt->pnode[0]); // 2nd param
        }
        else { //where
            genCodeBody(p0, rt->pnode[0]); // 1st param
        }
    }
    else if(isKindR(rt)){
        switch(getReductionKind(rt->funcName)){
            case sumR:
            case avgR: glueAny("c+=");   break;
            case minR: glueAny("if(c>"); break;
            case maxR: glueAny("if(c<"); break;
            case allR:
            case anyR: TODO("Add"); break;
            default: TODO("Unknown reduction: %s", rt->funcName);
        }
        Node *p0 = getNodeParams(n)->val;
        genCodeBody(p0, rt->pnode[0]);
    }
    else if(isKindE(rt)){
        //Node *fn = getNodeFunc(n);
        //glueAny("%s(", getFuncNameAuto(nodeName2(fn)));
        glueAny("%s(", getFuncNameAuto(n));
        genCodeParamReversed(getNodeParams(n), rt, 0);
        glueChar(')');
    }
    else if(isKindB(rt)){
        Node *fn = getNodeFunc(n);
        //glueAny("%s(", getFuncNameAuto(nodeName2(fn)));
        glueAny("%s(", getFuncNameAuto(n));
        if(sEQ(nodeName2(fn),"member")) {
            genCodeParamMemberReversed(getNodeParams(n), rt, 0);
        }
        else {
            genCodeParamReversed(getNodeParams(n), rt, 0);
        }
        glueChar(')');
    }
    else if(isKindX(rt)){
        Node *fn = getNodeFunc(n);
        // printNode(n); printNode(fn); 
        //WP("code = \n%s\n", code); getchar();
        fixCodeParamReversed(getNodeParams(n), rt, 0);
        //WP("code = \n%s\n", code); getchar();
    }
    else {
        TODO("Add impl. for kind: %c", rt->kind);
    }
    if(isRT){
        if(isKindS(rt)){
            S code_body = strdup(oldPtr);
            Node *z0 = getNodeItemIndex(n,0);
            C z0c = getTypeCodeByName(z0);
            SP(oldPtr, indent4 "L parZ[H_CORE], offset[H_CORE], total = 0;\n");
            glueAny(indent4 "DOI(H_CORE, parZ[i]=offset[i]=0)\n");
            glueAny(indent4 "DOT(vn(x0), if(%s) parZ[tid]++)\n", code_cond);
            glueAny(indent4 "DOI(H_CORE, total+=parZ[i])\n");
            glueAny(indent4 "DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])\n");
            glueAny(indent4 "initV(z, H_%c, total);\n", z0c);
            glueAny(indent4 "DOT(vn(x0), \\\n");
            glueAny(indent4 "    if(%s){L c=offset[tid]++; v%c(z,c)=v%c(x2,i);})\n", code_cond,z0c,z0c);
            glueAny(indent4 "R 0;\n}");
            //debugCode((S)"LINE 535");
        }
        else if(isKindR(rt)){
            if(code_cond){
                S code_body = strdup(oldPtr);
                const char *rop = obtainReductionOp(getReductionKind(rt->funcName));
                ReductionKind rkind = getReductionKind(rt->funcName);
                if(hasCompress && rkind == avgR){
                    SP(oldPtr, "if(%s){\\\n\t%s;tot++;}, reduction(%s:c,tot)", code_cond, code_body, rop);
                }
                else {
                    SP(oldPtr, "if(%s){\\\n\t%s;}, reduction(%s:c)", code_cond, code_body, rop);
                }
                free(code_cond);
                free(code_body);
                code_cond = NULL;
            }
            else {
                glueAny(", reduction(+:c)");
            }
            glueAny(")\n");
            // remaining
            if(isKindR(rt)){
                glueAny(indent4 "%s\n", last);
            }
            else if(isKindE(rt) || isKindX(rt) || isKindB(rt)){
                // WP("code = %s\n", code); getchar();
                // do nothing
            }
            else {
                TODO("Add impl. for kind %c", rt->kind);
            }
            glueAny(indent4 "R 0;\n}");
        }
        else if(isKindE(rt)){
            glueAny(") R 0;\n}");
            //debugCode((S)"kind E");
        }
        else {
            TODO("Add more kind: %c\n", rt->kind);
        }
        extra->funcFunc = strdup(code);
    }
    addToSimpleHashUnsafe(hashOpt, (L)(rt->node), (L)extra); // insert to hash
}

static void genCodeAutoListParam1(List *list, gNode *rt, L id){
    I total = rt->pnum;
    Node **rtn = NEW2(Node, total);
    loadParamsVec(rtn, list);
    //WP("params:\n");
    //DOI(total, {WP("%lld\n",i); printNode(rtn[i]);}) getchar();
    // important: reversed in rt->pnode
    DOIa(total, {gNode *t=rt->pnode[total-i-1]; \
            if(i>1) glueChar(comma); \
            if(t) genCodeAutoListNode(t, id); \
            else genCodeBody(rtn[i], t); })
    // TODO: free rtn
}

static void fixCodeAutoListParam1(List *list, gNode *rt, L id){
    I total = rt->pnum;
    Node **rtn = NEW2(Node, total);
    loadParamsVec(rtn, list);
    // WP("params: %d, %d\n", total,rt->pnum);
    // DOI(total, {WP("%lld\n",i); printNode(rtn[i]);}) getchar();
    // important: reversed in rt->pnode
        localIter = 'k';
        gNode *t=rt->pnode[2]; // index for the first parameter
        if(t) genCodeAutoListNode(t, id);
        else genCodeBody(rtn[1], t);
        localIter = 'i';
    // TODO: free rtn
}


static ChainExtra* addNodeToChainExtra(Node *n, GenKind kind){
    ChainExtra *extra = NEW(ChainExtra);
    extra->kind = kind;
    if(kind == OptG){
        // printNode(n); WP("extra = %lld\n", (L)extra); getchar();
        addToSimpleHashForce(hashOpt, (L)n, (L)extra);
    }
    else {
        addToSimpleHashUnsafe(hashOpt, (L)(n), (L)extra); // insert to hash
    }
    return extra;
}

static ChainExtra* addToChainExtra(gNode *rt, GenKind kind){
    return addNodeToChainExtra(rt->node, kind);
}

// idea: skip all list nodes by marking nodes with SkipG
static void skipAllListNode(gNode *rt){
    if(rt){
        addToChainExtra(rt, SkipG);
        DOI(rt->pnum, skipAllListNode(rt->pnode[i]))
    }
}

static void genCodeAutoListLen(gNodeList *list, L id){
    gNode *rt = list->g;
    if(list->numLast > 0){
        Node *last = list->lastNodes[0];
        List *vars = nodeStmtVars(last);
        I total = totalList(vars);
        if(total == 1){
            Node *p = vars->val;
            S name = instanceOf(p, nameK)? nodeName2(p): instanceOf(p, varK)? nodeVarName(p):NULL;
            if(name)
                glueAny("vn(x%lld)", searchName(varNames, name));
            else
                EP("Name is NULL");
            skipAllListNode(rt);
        }
        else EP("Support multiple vars");
    }
    else EP("Unknown error");
}

static void genCodeAutoListInit(gNode *rt, L id, S initStr){
    Node *z0 = getNodeItemIndex(rt->node, 0);
       S z0s = getNameStr(z0);
       C z0c = getTypeCodeByName(z0);
    if(isKindR(rt)){
        switch(getReductionKind(rt->funcName)){
            case sumR:
            case avgR: SP(initStr,"%c c%lld = 0;",z0c,id); break;
            case minR: SP(initStr,"%c c%lld = %s_MAX;",z0c,id,getMinMax(z0c)); break;
            case maxR: SP(initStr,"%c c%lld = %s_MIN;",z0c,id,getMinMax(z0c)); break;
            case allR: SP(initStr,"%c c%lld = 1;",z0c,id); break;
            case anyR: SP(initStr,"%c c%lld = 0;",z0c,id); break;
            case lenR: initStr[0] = 0; break; // do nothing
            default: EP("Unknown reduction function: %s", rt->funcName);
        }
    }
    else TODO("Add support kind: %c", rt->kind);
}

static void genCodeAutoListBody(Node *p, gNode *g, L id){
    if(g) genCodeAutoListNode(g, id);
    else {
        if(instanceOf(p, nameK)){
            genCodeName(p, searchName(varNames, nodeName2(p)));
        }
        else genCodeNode(p);
    }
}

Node *getNodeVar1(Node *n){
    List *vars = nodeStmtVars(n);
    I total = totalList(vars);
    if(total == 1){
        R vars->val;
    }
    else {
        EP("Expect single var assignment");
    }
}

static B findElementExpr(Node *p1){
    B isFound = false;
    localIter = 'k';
    DOI(fListNum, if(!fList[i]->isList){
            S var1 = nodeVarName(getNodeVar1(fList[i]->gnode->node));
            // WP("p1 = %s, var1 = %s\n", obtainNodeStr(p1), var1);
            if(sEQ(obtainNodeStr(p1), var1)){
                isFound = true;
                genCodeAuto(fList[i]->gnode, false);
                fuseFlag[i] = true; curNum++;
            }})
    localIter = 'i';
    // if(isFound){
    //     WP("found:\n%s\n", code);
    // }
    // else {
    //     WP("not found\n");
    // }
    // getchar();
    R isFound;
}

static void genCodeAutoListNode(gNode *rt, L id){
    Node *n = rt->node;
    addToChainExtra(rt, SkipG);
    if(isKindR(rt)){
        switch(getReductionKind(rt->funcName)){
            case sumR:
            case avgR: glueAny("c%lld+="  ,id); break;
            case minR: glueAny("if(c%lld>",id); break;
            case maxR: glueAny("if(c%lld<",id); break;
            case allR:
            case anyR: TODO("Future support."); break;
            default: TODO("Unknown reduction: %s", rt->funcName);
        }
        // debugCode((S)"LINE 654");
        genCodeAutoListBody(getNodeParams(n)->val, rt->pnode[0], id);
    }
    else if(isKindE(rt) || isKindB(rt)){
        //Node *fn = getEachFuncNode(n);
        //glueAny("%s(", getFuncNameAuto(nodeName2(fn)));
        glueAny("%s(", getFuncNameAuto(n));
        genCodeAutoListParam1(getNodeParams(n), rt, id); // skip 1st one
        glueChar(')');
    }
    else if(isKindX(rt)){
        Node *fn = getEachFuncNode(n);
        // WP("kind %s (index)\n", getFuncNameAuto(nodeName2(fn)));
        // printNode(n); getchar();
        //Node *p1 = getNodeParams(n)->next->val; // 1st parameter
        //if(!findElementExpr(p1)){
        //    glueAny("%s(", getFuncNameAuto(nodeName2(fn)));
        //    genCodeAutoListParam1(getNodeParams(n), rt, id); // skip 1st one
        //    glueChar(')');
        //}
        fixCodeAutoListParam1(getNodeParams(n), rt, id); // skip 1st one
        //WP("code = %s\n", code); getchar();
    }
    else TODO("Add support kind: %c", rt->kind);
}

/*
 * An entry function for two cases
 *  - @len (skip intermediate stmts)
 *  - other kinds require intermediate stmts
 */
static void genCodeAutoListMain(gNodeList *list, L id){
    gNode *rt = list->g;
    if(sEQ(rt->funcName,"len")){
        genCodeAutoListLen(list, id); // search for last stmt
    }
    else genCodeAutoListNode(rt, id);
}

static O genCodeListReduction(fListNode *f){
    gNode *g = f->gnode;
    I fid = f->fuseId;
    Node *z0 = f->nodeRtn;
    S z0s = getNameStr(z0);
    C z0c = getTypeCodeByName(z0);
    ReductionKind k = getReductionKind(g->funcName);
    // isKindR(g)
    switch(k){
        case sumR: 
        case avgR: glueAny("%c c%d=0;", z0c,fid); break;
        case minR: glueAny("%c c%d=%s_MAX;", z0c,fid,getMinMax(z0c)); break;
        case maxR: glueAny("%c c%d=%s_MIN;", z0c,fid,getMinMax(z0c)); break;
        case allR: glueAny("%c c%d=1;", z0c,fid); break;
        case anyR: glueAny("%c c%d=0;", z0c,fid); break;
        case lenR: glueAny("E c%d=0;",fid); break; // dummy: skip for len
        default: TODO("handle reduction op: %s, fid: %d", g->funcName,fid); break;
    }
    if(k != lenR) // skip lenR
        glueChar(' ');
}

static const char *genCodeListParallelReduction(fListNode **fList, I num){
    C temp[199];  S ptr = (S)temp;
    ptr += SP(ptr, "reduction(+:");
    DOI(num, {I fid=fList[i]->fuseId;ptr+=SP(ptr,i>0?",c%d":"c%d",fid);})
    // WP("temp = %s\n", temp); getchar();
    R strdup(temp);
}


static O genCodeListBody(fListNode *f){
    gNode *g = f->gnode;
    I fid = f->fuseId;
    if(sNEQ(g->funcName, "len")){ // != len
        glueIndent();
        genCodeAutoListNode(g, fid);
        glueAny("; \\\n");
    }
}

static O genCodeListBodyLen(fListNode *f, C iter){
    gNode *g = f->gnode;
    I fid = f->fuseId;
    I zid = fid - 1;
    Node *z0 = f->nodeRtn;
    C z0c = getTypeCodeByName(z0);
    glueIndent();
    ReductionKind k = getReductionKind(g->funcName);
    switch(k){
        case lenR: glueAny("v%c(z%d,%c)=",z0c,zid,iter);
                   fixCodeAutoListSingleLen(f, iter); break;
        case sumR:
        case maxR: 
        case minR: glueAny("v%c(z%d,%c)=c%d", z0c,zid,iter,fid); break;
        case avgR: glueAny("v%c(z%d,%c)=c%d/vn(t)", z0c,zid,iter,fid); break;
        default: TODO("Future support.");
    }
    glueAny("; \\\n");
}

static void loadLocalVars(gNode *rt){
    Node *n = rt->node;
    varNum = 0;
    totalInputs(getNodeParams(n), rt, 0, varNames);
}

static void genCodeAutoList(gNodeList *list, I size){
    C temp[199];
    /* list: no dummy node */
    STOP("size = %d\n", size);
    if(size > 0){
        gNodeList *plist = list;
        // 1st pass: scan and store all local vars
        DOI(size, {loadLocalVars(plist->g); plist=plist->next;})
        //printAllNames(); getchar();
        // 2nd pass: gen pieces of code
        SP(temp, "q%d_autofusion_list_%d",qid,phTotal++);
        glueCode(genDeclSingle(temp, '{')); glueLine();
        printNode(list->g->node);
        STOP("check first");
        ChainExtra *extra = addToChainExtra(list->g, OptG);
        extra->funcDecl = genDeclSingle(temp, ';');
        extra->funcInvc = temp;
        //STOP("code = \"%s\"\n", code);
        plist = list;
        DOI(size, {gNode *x=plist->g; C temp[99]; \
                   if(i>0)addToChainExtra(x,SkipG);\
                   gNode *x0=x->pnode[0]; \
                   genCodeAutoListInit(x0,i,temp); \
                   genCodeAutoListMain(plist,i); \
                   plist=plist->next;})
    }
}

static void clearFusion(){
    cleanCode();
    ptr = code;
    varNum = 0; 
}

static L numLast;
static Node *tempLastNodes[10];

static B checkStmtByKind(Node *n, C x){
    if(instanceOf(n, stmtK)){
        Node *call = getStmtCall(n);
        C kind = getCallKind(call);
        R kind == x;
    }
    R false;
}

static Node *getNodeDef(Chain *chain, Node *n){
    I c = 0;
    if(!checkParamDef(n, chain, &c)){
        Chain *def = chain->chain_defs[c];
        R chainNode(def);
    }
    EP("Def not found");
}

static Node *findIteratorIndex(gNode *rt){
    if(checkStmtByKind(rt->node, 'X')){
        gNode *nt = rt->pnode[0];
        Node *next = nt->node;
        if(checkStmtByKind(next, 'S')){
            List *param = getNodeParams(next);
            I total = totalList(param);
            if(total == 1){
                R getNodeDef(nt->gChain, param->val);
            }
            else if(total == 2){
                R getNodeDef(nt->gChain, param->next->val);
            }
        }
    }
    R NULL;
}

static void fetchFusionLastNode(gNode *rt){
    if(rt){
        DOI(rt->pnum, if(rt->pnode[i]) fetchFusionLastNode(rt->pnode[i]))
        DOI(rt->numLast, tempLastNodes[numLast++]=rt->lastNodes[i])
    }
}

static Node *findIteratorNode(gNode *rt){
    if(numLast > 0){
        List *vars = nodeStmtVars(tempLastNodes[0]);
        I total = totalList(vars);
        if(total == 1){
            R vars->val;
        }
    }
    R findIteratorIndex(rt);
    // printgNode(rt);
    // TODO("should not return NULL");
    // R NULL;
}

static I getNameIndex(Node *n){
    S name = instanceOf(n, nameK)?nodeName2(n):instanceOf(n,varK)?nodeVarName(n):NULL;
    if(name) R searchName(varNames, name);
    else EP("Unknown name");
}

static I fixNameIndex(fListNode *f){
    Node *n = f->nodeIter;
    S name = instanceOf(n, nameK)?nodeName2(n):instanceOf(n,varK)?nodeVarName(n):NULL;
    if(name) R searchNameArray(f->varNames, f->varNum, name);
    else EP("Unknown name");
}

static void genCodeAutoListSingleLen(gNode *rt){
    //WP("numLast = %d\n", numLast);
    //DOI(numLast, printNode(tempLastNodes[i]))
    if(numLast > 0){
        List *vars = nodeStmtVars(tempLastNodes[0]);
        I total = totalList(vars);
        if(total == 1){
            Node *p = vars->val;
            S name = instanceOf(p, nameK)? nodeName2(p): instanceOf(p, varK)? nodeVarName(p):NULL;
            if(name){
                printAllNames();
                STOP("name = %s\n", name);
                glueAny("vn(vV(x%lld,i))", searchName(varNames, name));
            }
            else{
                EP("Name is NULL");
            }
            skipAllListNode(rt);
        }
        else TODO("Support multiple vars");
    }
    else {
        WP("--Error-Below--\n");
        WP("numLast = %d\n", rt->numLast);
        printgNode(rt);
        EP("The numLast > 0, but %lld found", numLast);
    }
}

static void fixCodeAutoListSingleLen(fListNode *f, C iter){
    gNode *rt = f->gnode;
    numLast = 0;
    fetchFusionLastNode(rt);
    if(numLast > 0){
        List *vars = nodeStmtVars(tempLastNodes[0]);
        I total = totalList(vars);
        if(total == 1){
            Node *p = vars->val;
            S name = instanceOf(p, nameK)? nodeName2(p): instanceOf(p, varK)? nodeVarName(p):NULL;
            if(name){
                printNameArray(f->varNames, f->varNum);
                glueAny("vn(vV(x%lld,%c))", \
                        searchNameArray(f->varNames, f->varNum, name), iter);
                // glueAny("vn(t)");
            }
            else{
                EP("Name is NULL");
            }
            skipAllListNode(rt);
        }
        else TODO("Support multiple vars");
    }
    else {
        WP("--Error-Below--\n");
        WP("numLast = %d\n", rt->numLast);
        printgNode(rt);
        EP("The numLast > 0, but %lld found", numLast);
    }
}

static B isMemberConst(Node *n){
    List *param = getNodeParams(n);
    while(param){
        Node *p = param->val;
        if(isNodeConst(p) && 'Q' == getTypeCodeByName(p)){
            R 1;
        }
        param = param->next;
    }
    R 0;
}

static B nextCond(Node *x, Node *next){
    S name1 = getCallFuncName(x);
    S name2 = getCallFuncName(next);
    if(!name1 || !name2) R false;
    // if(sEQ(name1, "and") && sEQ(name2, "member")){ // q19
    //     printNode(next);
    //     WP("result: %d\n", isMemberConst(next)); getchar();
    // }
    R (sEQ(name1,"plus"    ) && sEQ(name2,"compress")) || 
      //(sEQ(name1,"and"     ) && sEQ(name2,"member"  ) &&
      //                                    qTpchId!=22) ||
      (sEQ(name1,"and"     ) && sEQ(name2,"member"  ) &&
            (qTpchId!=22 && !isMemberConst(next)))     ||
      (sEQ(name1,"where"   ) && sEQ(name2,"lt"      )) ||
      (sEQ(name1,"member"  ) && sEQ(name2,"compress")) || 
      (sEQ(name1,"compress") && sEQ(name2,"index"))    || 
      (sEQ(name1,"compress") && sEQ(name2,"like"))     || 
      (sEQ(name1,"compress") && sEQ(name2,"compress")) || 
      (sEQ(name1,"compress") && sEQ(name2,"or"))       || 
      (sEQ(name1,"not"     ) && sEQ(name2,"like")     &&
                                          qTpchId!=14) || 
      (sEQ(name1,"not"     ) && sEQ(name2,"member"));
      //(findCompress && sEQ(name2,"compress"));
}

static void copyLocalVarsToListNode(fListNode *f){
    f->varNum = varNum;
    f->varNames = NEWL(S, varNum);
    DOI(varNum, f->varNames[i]=strdup(varNames[i]))
}

static fListNode *collectFusibleSection(Node *cur, gNode *g, B isList){
    fListNode *f = initfListNode(phTotal++);
    // load vars
    loadLocalVars(g);
    copyLocalVarsToListNode(f);
    // load last nodes
    numLast = 0;
    fetchFusionLastNode(g);
    // fetch important vars
    Node *z0 = getNodeItemIndex(cur, 0);
    Node *r0 = findIteratorNode(g);
    f->nodeOpt  = cur;
    f->nodeRtn  = z0;
    f->nodeIter = r0;
    f->gnode    = g;
    f->isList   = isList;
    R f;
}

static void genCodeAutoListSingle(Node *cur, gNode *rt){
    C temp[199];
    SP(temp, "q%d_autofusionlist_%d",qid,phTotal);
    glueCode(genDeclSingle(temp, '{')); glueLine();
    // load local vars
    loadLocalVars(rt);
    // load last nodes
    numLast = 0;
    fetchFusionLastNode(rt);
    // dump local vars
    DOI(varNum, glueAny(indent4 "V x%lld=x[%lld]; // %s\n",i,i,varNames[i]))
    // set meta information
    ChainExtra *extra = addNodeToChainExtra(cur, OptG);
    // fetch var infor for codegen
    Node *z0 = getNodeItemIndex(cur, 0);
    S z0s = getNameStr(z0);
    C z0c = getTypeCodeByName(z0);
    //STOP("z0s = %s, z0c = %c", z0s, z0c);
    Node *r0 = findIteratorNode(rt);
    I r0x = getNameIndex(r0);
    if(sEQ(rt->funcName,"len")){
        glueAny(indent4 "initV(z, H_%c, vn(x%d));\n",z0c,r0x);
        glueAny(indent4 "DOP(vn(z), vL(z,i)=");
        WP("1\n");
        genCodeAutoListSingleLen(rt);
        glueAny(") R 0;\n}");
    }
    else {
        genCodeAutoListNode(rt, phTotal);
    }
    extra->funcFunc = strdup(code);
    extra->funcDecl = genDeclSingle(temp, ';');
    extra->funcInvc = genInvcSingle(z0s, temp, varNames, varNum);
    STOP("code = %s", code);
    phTotal++;
}

/*
 * chainNode(chain) is stmtK
 */
static void findFusion(Chain *chain){
    Node *call = getStmtCall(chainNode(chain));
    if(call){
        findCompress = false;
        C kind = getCallKind(call);
        if(kind){
            // if(OPT_DEBUG && kind == 'R'){
            //     printNode(call);
            //     STOP("kind = %c\n", kind);
            // }
            // 1st pass, find all possible nodes
            gNode *rt = findFusionUp(chain, true, false);
			// 2nd pass, trim rt
			trimgNodeFromTop(rt); 
            rt = analyzegNode(rt);
            if(rt && isOK2Fuse(rt)){
				//WP("Fusion auto found:\n");
				//printgNode(rt); getchar();
				//clearFusion();
				//genCodeAuto(rt, true);
                fList[fListNum++] = collectFusibleSection(chainNode(chain), rt, false);
            }
        }
        else if(isCallRaze(call)){
            gNode *rt = findFusionUpList(chain);
			trimgNodeFromTop(rt); 
            if(rt && isOK2Fuse(rt)){
                clearFusion();
                fList[fListNum++] = collectFusibleSection(chainNode(chain), rt, true);
                //genCodeAutoListSingle(chainNode(chain), rt);
                // include raze
                //insertgNode(glist, rt);
                //printgNodeList();
                //STOP("Fusion list found:");
            }
        }
    }
}

/* reversed topological order (bottom-up) */
static void analyzeChainBottomUp(ChainList *list){
    while(list){
        Chain *chain = list->chain;
        //printChain(chain); WP("%d", isChainVisited(chain)); getchar();
        if(!isChainVisited(chain) && instanceOf(chainNode(chain), stmtK)){
            findFusion(chain);
        }
        list = list->next;
    }
}

static I totalFusionList(gNodeList *x){
    x = x->next;
    I c = 0; while(x) { c++; x = x->next; } R c;
}

static gNodeList *copygNodeList(gNodeList *x){
    gNodeList *t = NEW(gNodeList); *t = *x; R t;
}

static void insertgNodeList(gNodeList *x, gNodeList *y){
    y->next = x->next;
    x->next = y;
}

B sameLastNodes(gNodeList *a, gNodeList *b){
    if(a->numLast == b->numLast){
        DOI(a->numLast, if(a->lastNodes[i] != b->lastNodes[i]) R 0) R 1;
    }
    R 0;
}

/* scan all gNodeList to find which 'lists' can be fused again */
// static void genCodeListFusion(gNodeList *x){
//     I num = totalFusionList(x);
//     B *flag = NEWL(B, num);
//     memset(flag, 0, sizeof(B)*num);
//     WP("# of list fusion: %d\n", num);
//     gNodeList *tmp = NEW(gNodeList);
//     gNodeList *a   = x->next;
//     I c = 0;
//     while(a){
//         if(!flag[c]) {
//             flag[c] = 1;
//             I k = c+1, size = 1;
//             gNodeList *b = a->next;
//             insertgNodeList(tmp, copygNodeList(a));
//             while(b){
//                 if(!flag[k]){
//                     if(sameLastNodes(a, b)){
//                         flag[k] = 1;
//                         insertgNodeList(tmp, copygNodeList(b));
//                         size++;
//                     }
//                 }
//                 b = b->next; k++;
//             }
//             genCodeAutoList(tmp->next, size); // gen code
//             tmp->next = NULL;// TODO: free tmp
//         }
//         a = a->next; c++;
//     }
//     free(flag);
// }

// input string format: id%d
// output the integer part
static L getConstSymbolIdInt(S id){
    R (L)atoi(id+2);
}

static I genLocalVars(S *names, I num){
    I c = 0;
    DOI(num, if(!strncmp(names[i], "id", 2)){\
        glueAny(indent4 "Q %s=getSymbol(\"%s\");\n",\
                names[i],\
                constId[getConstSymbolIdInt(names[i])]); \
        }\
        else { c++; \
        glueAny(indent4 "V x%lld=x[%lld]; // %s\n",i,i,names[i]);})
    R c;
}

static O genReturnVars(I num){
    DOI(num, glueAny(indent4 "V z%lld=z[%lld]; // \n", i,i))
}

/*
 * meta info collected in the function: collectFusibleSection
 */

static B findRepeatName(S *names, I n, S str){
    DOI(n, if(sEQ(names[i],str))R 1) R 0;
}

// remove duplicated vars
static O saveToLocalVars(fListNode **fList, I num){
    varNum = 0;
    DOI(num, \
       DOJ(fList[i]->varNum, { \
          S str=fList[i]->varNames[j]; \
          if(!findRepeatName(varNames,varNum,str)) varNames[varNum++]=str;}))
}

static ChainExtra* setSingleNodeVisible(fListNode **fList, I num){
    DOIa(num, addNodeToChainExtra(fList[i]->nodeOpt, SkipG))
    // WP("visible\n"); printNode(fList[0]->nodeOpt); getchar();
    return addNodeToChainExtra(fList[0]->nodeOpt, OptG);
}

static B checkNodeHasLikeOrElem(gNode *rt){
    if(rt){
        DOI(rt->pnum, {
                gNode *x=rt->pnode[i];
                if(x){ if(checkNodeHasLikeOrElem(x)||isKindE(x)||isKindB(x))R 1; }
                });
    }
    R 0;
}

static B checkNodeHasLike(gNode *rt){
    if(rt){
        DOI(rt->pnum, {
                gNode *x=rt->pnode[i];
                if(x){ if(checkNodeHasLike(x)||isKindB(x)) R 1; }
                })
    }
    R 0;
}

// at least one parameter is constant
static B isParamConst1(Node *n){
    List *list = getNodeParams(n);
    while(list){
        if(instanceOf(list->val, constK))
            R 1;
        list = list->next;
    }
    R 0;
}

static B checkNodeSimpleMember(gNode *rt){
    if(rt){
        DOI(rt->pnum, {
                gNode *x=rt->pnode[i];
                if(x){ if(checkNodeSimpleMember(x)) R 1;
                    S namex = getCallFuncName(x->node);
                    if(namex && sEQ(namex, "member")) R 1; }
                })
    }
    R 0;
}
// if(namex && sEQ(namex, "member") && isParamConst1(x->node)) R 1; }


static B isSpecialLike(fListNode *f){
    // need: like + elementwise + reduction
    gNode *rt = f->gnode;
    R isKindR(rt) && checkNodeHasLike(rt) && checkNodeHasLikeOrElem(rt);
}

static B isSpecialIndex(fListNode *f){
    gNode *rt = f->gnode;
    R isKindX(rt) && isKindS(rt->pnode[0]); // last param in rt
}

static S specialString;
static L specialStringId;

static S findSpecialString(Node *n){
    List *list = getNodeParams(n);
    while(list){
        Node *c = getSingleSymbol(list->val);
        if(c){ // get constant string
            R c->val.nodeC->valS;
        }
        list = list->next;
    }
    R 0;
}

static void getSpecialLikeString(gNode *rt){
    if(specialString) R;
    if(rt){
        if(isKindB(rt)){
            specialString = findSpecialString(rt->node);
        }
        else {
            DOI(rt->pnum, getSpecialLikeString(rt->pnode[i]))
        }
    }
}

static L findSpecialStringId(Node *n){
    List *list = getNodeParams(n);
    while(list){
        Node *c = getSingleSymbol(list->val);
        if(!c){ // get non-constant string
            R searchName(varNames, nodeName2(list->val));
        }
        list = list->next;
    }
    R -1;
}

static void getSpecialStringId(gNode *rt){
    if(specialStringId >= 0) R;
    if(rt){
        if(isKindB(rt)){
            specialStringId = findSpecialStringId(rt->node);
        }
        else {
            DOI(rt->pnum, getSpecialStringId(rt->pnode[i]))
        }
    }
}

static void genCodeExprSpecialLike(gNode *rt);

static void genCodeParamLikeReversed(List *list, gNode *rt, I dep){
    if(list){
        genCodeParamLikeReversed(list->next, rt, dep+1);
        if(list->next) glueChar(comma);
        gNode *t = rt->pnode[dep];
        if(t) genCodeExprSpecialLike(t);
        else {
            Node *p = list->val;
            if(instanceOf(p, nameK))
                genCodeName(p, searchName(varNames, nodeName2(p)));
            else
                genCodeNode(p);
        }
    }
}

static void genCodeExprSpecialLike(gNode *rt){
    if(rt){
        Node *n = rt->node;
        if(isKindE(rt)){
            glueAny("%s(", getFuncNameAuto(n));
            genCodeParamLikeReversed(getNodeParams(n), rt, 0);
            glueChar(')');
        }
        else if(isKindB(rt)){
            glueChar('t');
        }
        else if(isKindR(rt)){
            glueAny("r[tid]+=");
            genCodeExprSpecialLike(rt->pnode[0]);
        }
        else {
            printNode(n);
            TODO("Should not go here.");
        }
        if(!isKindR(rt)){
            ChainExtra *extra = NEW(ChainExtra);
            extra->kind = SkipG;
            addToSimpleHashUnsafe(hashOpt, (L)(rt->node), (L)extra); // insert to hash
        }
    }
}

// static void genCodeBodySpecialLike1(fListNode *f){
//     gNode *rt = f->gnode;
//     depth++;
//     glueAnyLine("pcre2_code *re = getLikePatten(\"%s\");", specialString);
//     glueAnyLine("pcre2_match_data *match = pcre2_match_data_create_from_pattern(re, NULL);");
//     glueAnyLine("if(re==NULL) R 99;");
//     glueAnyLine("I jit_status = pcre2_jit_compile(re, PCRE2_JIT_COMPLETE);");
//     glueAnyLine("initV(z, H_E, 1);");
//     glueAnyLine("E r[H_CORE]; DOI(H_CORE, r[i]=0)");
//     glueAnyLine("DOLIKE(vn(x0), {");
//     depth++;
//     specialStringId = -1;
//     getSpecialStringId(rt);
//     glueAnyLine("B t = LIKEMATCH(getSymbolStr(vQ(x%lld,i)),getSymbolSize(vQ(x%lld,i)),re,match);",specialStringId,specialStringId);
//     genIndent(); genCodeExprSpecialLike(rt); glueAny(";})\n");
//     depth--;
//     glueAnyLine("E r0=0; DOI(H_CORE, r0+=r[i]) ve(z)=r0;");
//     glueAnyLine("pcre2_code_free(re);");
//     glueAnyLine("pcre2_match_data_free(match);");
//     glueAnyLine("R 0;");
//     depth--;
// }

static void genCodeBodySpecialLike1(fListNode *f){
    gNode *rt = f->gnode;
    depth++;
    glueAnyLine("initV(z, H_E, 1);");
    glueAnyLine("E r[H_CORE]; DOI(H_CORE, r[i]=0)");
    glueAnyLine("DOT(vn(x0), {");
    depth++;
    specialStringId = -1;
    getSpecialStringId(rt);
    glueAnyLine("B t = LIKE2Q(vQ(x%lld,i),id0);",specialStringId);
    genIndent(); genCodeExprSpecialLike(rt); glueAny(";})\n");
    depth--;
    glueAnyLine("E r0=0; DOI(H_CORE, r0+=r[i]) ve(z)=r0;");
    glueAnyLine("R 0;");
    depth--;
}

static void genCodeBodySpecialLike2(fListNode *f){
    gNode *rt = f->gnode;
    genCodeAuto(rt, true);
}


static void genCodeBodySpecialLike(fListNode *f){
    specialString = NULL;
    getSpecialLikeString(f->gnode);
    if(specialString){
        genCodeBodySpecialLike1(f);
    }
    else {
        EP("String expected!");
    }
}

static void genCodeSpecialLike(fListNode *f){
    B isSimpleMember = checkNodeSimpleMember(f->gnode);
    // WP("isSimpleMember = %d\n", isSimpleMember); getchar();
    if(isSimpleMember){
        genCodeBodySpecialLike2(f);
    }
    else {
        C temp[199];
        SP(temp, "q%d_autofusion_%d",qid,f->fuseId);
        glueCode(genDeclSingle(temp, '{')); glueLine();
        genLocalVars(varNames, varNum);
        genCodeBodySpecialLike(f);
        glueAny("}\n");
        S z0s = getNameStr(f->nodeRtn);
        ChainExtra *extra = addNodeToChainExtra(f->nodeOpt, OptG);
        extra->funcFunc = strdup(code);
        extra->funcDecl = genDeclSingle(temp, ';');
        extra->funcInvc = genInvcSingle(z0s, temp, f->varNames, f->varNum);
    }
    // debugCode((S)"Show like");
}

// compress + index
static void genCodeBodySpecialIndexCompress(fListNode *f){
    Node *z0 = f->nodeRtn;
    S z0s = getNameStr(z0);
    C z0c = getTypeCodeByName(z0);
    gNode *rt = f->gnode;
    gNode *next = rt->pnode[0];
    I x0 = getNameIndex(getNodeParams(next->node)->val);
    I x1 = getNameIndex(getNodeItemIndex(rt->node, 1));
    I x2 = getNameIndex(getNodeItemIndex(next->node, 1)); // boolean
    // WP("x0 = %d, x1 = %d, x2 = %d\n",x0,x1,x2);
    depth++;
    glueAnyLine("/* Fusion: special index for compress */");
    glueAnyLine("L lenZ = 0, parZ[H_CORE], offset[H_CORE];");
    glueAnyLine("DOI(H_CORE, parZ[i]=offset[i]=0)");
    glueAnyLine("CHECKE(getNumOfNonZero(x%d,parZ));",x2);
    glueAnyLine("DOI(H_CORE, lenZ += parZ[i])");
    glueAnyLine("DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])");
    glueAnyLine("initV(z, H_%c, lenZ);",z0c);
    glueAnyLine("DOT(vn(x%d), if(vB(x%d,i)){L c=offset[tid]++;\\",x2,x2);
    depth++;
    glueAnyLine("v%c(z,c) = v%c(x%d,vL(x%d,i));})",z0c,z0c,x1,x0);
    depth--;
    glueAnyLine("R 0;");
    depth--;
    glueAnyLine("}");
}


static void genCodeBodySpecialIndexWhere(fListNode *f){
    Node *z0 = f->nodeRtn;
    S z0s = getNameStr(z0);
    C z0c = getTypeCodeByName(z0);
    gNode *rt = f->gnode;
    gNode *next = rt->pnode[0];
    I x0 = getNameIndex(getNodeParams(next->node)->val);  // boolean
    I x1 = getNameIndex(getNodeItemIndex(rt->node, 1));
    // WP("x0 = %d, x1 = %d, x2 = %d\n",x0,x1,x2);
    depth++;
    glueAnyLine("/* Fusion: special index for where */");
    glueAnyLine("L lenZ = 0, parZ[H_CORE], offset[H_CORE];");
    glueAnyLine("DOI(H_CORE, parZ[i]=offset[i]=0)");
    glueAnyLine("CHECKE(getNumOfNonZero(x%d,parZ));",x0);
    glueAnyLine("DOI(H_CORE, lenZ += parZ[i])");
    glueAnyLine("DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])");
    glueAnyLine("initV(z, H_%c, lenZ);",z0c);
    glueAnyLine("DOT(vn(x%d), if(vB(x%d,i)){L c=offset[tid]++;\\",x0,x0);
    depth++;
    glueAnyLine("v%c(z,c) = v%c(x%d,i);})",z0c,z0c,x1);
    depth--;
    glueAnyLine("R 0;");
    depth--;
    glueAnyLine("}");
}

static void genCodeBodySpecialIndex(fListNode *f){
    gNode *rt   = f->gnode;
    gNode *next = rt->pnode[0];
    S func = getCallName(getStmtCall(next->node));
    // WP("func = %s\n", func); // where or compress
    if(sEQ(func, "where"))
        genCodeBodySpecialIndexWhere(f);
    else if(sEQ(func, "compress"))
        genCodeBodySpecialIndexCompress(f);
    else
        EP("Add support for function %s", func);
    addNodeToChainExtra(next->node, SkipG); // skip the second stmt
}

static void genCodeSpecialIndex(fListNode *f){
    C temp[199];
    SP(temp, "q%d_autofusion_%d",qid,f->fuseId);
    glueCode(genDeclSingle(temp, '{')); glueLine();
    genLocalVars(varNames, varNum);
    genCodeBodySpecialIndex(f);
    S z0s = getNameStr(f->nodeRtn);
    ChainExtra *extra = addNodeToChainExtra(f->nodeOpt, OptG);
    extra->funcFunc = strdup(code);
    extra->funcDecl = genDeclSingle(temp, ';');
    extra->funcInvc = genInvcSingle(z0s, temp, f->varNames, f->varNum);
    //debugCode((S)"Show index code");
}


static O genMultipleInitVars(fListNode **fList, I num){
    Node *r0 = fList[0]->nodeIter;
    I r0x = getNameIndex(r0);
    DOI(num, { fListNode *f = fList[i];
       Node *z0 = f->nodeRtn;
       S z0s = getNameStr(z0);
       glueAny(indent4 "V z%lld=z[%lld]; // %s\n",i,i,z0s); })
    DOI(num, { fListNode *f = fList[i]; \
       Node *z0 = f->nodeRtn;  \
       S z0s = getNameStr(z0); \
       C z0c = getTypeCodeByName(z0); \
       glueAny(indent4 "initV(z%lld, H_%c, vn(x%d)); // %s\n",i,z0c,r0x,z0s); })
}

static O genMultipleBodyOuter(fListNode **fList, I num, I r0x){
    // depth++;
    // glueAnyLine("DOP(vn(x%d), {V t=vV(x%d,i); \\",r0x,r0x);
    // depth++;
    // glueIndent(); DOI(num, genCodeListReduction(fList[i])) glueAny("\\\n");
    // glueAnyLine("DOJ(vn(t), {\\");
    // depth++;
    // DOI(num, genCodeListBody(fList[i]))   // skip len
    // depth--;
    // glueAnyLine("}) \\");
    // DOI(num, genCodeListBodyLen(fList[i], 'i')) // do len
    // depth--;
    // glueAnyLine(")");
    // depth--;
    // WP("code\n %s\n", code); getchar();
}

static O genMultipleBodyInner(fListNode **fList, I num, I r0x){
    depth++;
    glueAnyLine("DOJ(vn(x%d), {V t=vV(x%d,j); \\",r0x,r0x);
    depth++;
    glueIndent(); DOI(num, genCodeListReduction(fList[i])) glueAny("\\\n");
    glueAnyLine("DOP(vn(t), {L k=vL(t,i);\\");
    depth++;
    //WP("3\n");
    DOI(num, genCodeListBody(fList[i]))   // skip len
    depth--;
    glueAnyLine("}, %s)) \\", genCodeListParallelReduction(fList,num));
    //WP("4\n");
    DOI(num, genCodeListBodyLen(fList[i], 'j')) // do len
    depth--;
    glueAnyLine("})");
    depth--;
    glueAnyLine("R 0;");
    depth--;
    glueAnyLine("}");
}

static O genMultipleBody(fListNode **fList, I num){
    depth++;
    Node *r0 = fList[0]->nodeIter;
    I r0x = getNameIndex(r0);
    //glueAnyLine("if(isParOuter(x%d)){", r0x);
    //genMultipleBodyOuter(fList, num, r0x);
    //glueAnyLine("}");
    //glueAnyLine("else {");
    genMultipleBodyInner(fList, num, r0x);
    //glueAnyLine("}");
    depth--;
    //STOP("check 1.");
}

static gNode *findFusionIndex(Chain *chain){
    Node *n = chainNode(chain);
    List *param = getNodeParams(n);
    Node *p2 = param->val;
    I c = 0;
    if(!checkParamDef(param->val, chain, &c)){
        Chain *next = chain->chain_defs[c];
        if(!isChainVisited(next)){
            if('S' == nextKind(chainNode(next))){
                gNode *rt = initgNode(n, 'X', (S)"index");
                rt->gChain = chain;
                Node *p = chainNode(next);
                Node *call = getStmtCall(p);
                S funcName = nodeName2(nodeCallFunc(call));
                // STOP("funcName = %s\n", funcName);
                rt->pnode[0] = initgNode(p, 'S', funcName);
                rt->pnode[0]->gChain = next;
                if(sEQ(funcName, "compress"))
                    rt->pnode[1] = NULL;
                setVisited(chain, true);
                setVisited(next, true);
                //printgNode(rt); getchar();
                R rt;
            }
        }
    }
    R NULL;
}

static void printFusibleSections(fListNode **fList, I fListNum){
    L c = 0;
    DOI(fListNum, {
            S iter=(S)obtainNodeStr(fList[i]->nodeIter);
            WP("Fusible section: #%lld (iter: %s, List: %d, Id: %d)\n",\
                    i,iter?iter:"NULL", fList[i]->isList, fList[i]->fuseId);
            L c0=printgNode(fList[i]->gnode) + fList[i]->isList;
            WP("\n-- size: %lld\n",c0); c+=c0; })
    WP("total statements fused: %lld\n", c);
    // getchar();
}

static L countFusedStmtsCore(gNode *pt, gNode *rt, I dep, sHashTable *flag){
    L c = 0;
    if(rt){
        if(!lookupSimpleHash(flag,(L)rt)){
            if(pt) c++;
            DOI(rt->pnum, c+=countFusedStmtsCore(rt, rt->pnode[i], dep+1, flag))
            addToSimpleHash(flag, (L)rt, 1);
        }
    }
    R c;
}

static L countFusedStmts(fListNode *f){
    gNode *rt = f->gnode;
    sHashTable *flag = initSimpleHash(1<<10);
    L c = countFusedStmtsCore(NULL, rt, 0, flag)+ (NULL != rt) + f->isList;
    freeSimpleHash(flag);
    R c;
}

static S genDeclMultiple(S func, C del){
    C temp[199]; 
    SP(temp, "static I %s(V *z, V *x)%c", func, del);
    return strdup(temp);
}

static S genInvcMultiple(S func, fListNode **fList, I num){
    C temp[199]; S ptr = temp;
    saveToLocalVars(fList, num);
    ptr += SP(ptr, "%s((V []){", func);
    DOI(num, ptr+=SP(ptr,(i>0?",%s":"%s"),getNameStr(fList[i]->nodeRtn)))
    ptr += SP(ptr, "}, (V []){");
    DOI(varNum, if(strncmp(varNames[i],"id",2)) \
             ptr+=SP(ptr,(i>0?",%s":"%s"),varNames[i]))
    SP(ptr, "})");
    R strdup(temp);
}

static void genCodeListSingle(fListNode **fList){
    // generate function signature
    fListNode *f = fList[0];
    C temp[199];
    SP(temp, "q%d_autofusionlist_%d",qid,f->fuseId);
    glueCode(genDeclSingle(temp, '{')); glueLine();
    // generate local vars
    genLocalVars(f->varNames, f->varNum);
    Node *z0 = f->nodeRtn;
    //Node *r0 = f->nodeIter;
    gNode *g = f->gnode;
    S z0s = getNameStr(z0);
    C z0c = getTypeCodeByName(z0);
    I r0x = fixNameIndex(f);
    // set meta information
    if(sEQ(g->funcName, "len")){
        glueAny(indent4 "initV(z, H_%c, vn(x%d));\n",z0c,r0x);
        glueAny(indent4 "DOP(vn(z), vL(z,i)=");
        fixCodeAutoListSingleLen(f, 'i');
        glueAny(") R 0;\n}");
    }
    else { // generate non-len list fusion
        genCodeAutoListNode(g, f->fuseId);
    }
    ChainExtra *extra = addNodeToChainExtra(f->nodeOpt, OptG);
    extra->funcFunc = strdup(code);
    extra->funcDecl = genDeclSingle(temp, ';');
    extra->funcInvc = genInvcSingle(z0s, temp, f->varNames, f->varNum);
    //STOP("code = %s", code);
}

static void genCodeListMultiple(fListNode **myList, I num){
    WP("- Fusing %d fusible sections for lists\n", num);
    fListNode *f = myList[0];
    // generate function signature
    C temp[199];
    SP(temp, "q%d_autofusionlist_%d",qid,f->fuseId);
    glueCode(genDeclMultiple(temp, '{')); glueLine();
    // generate local vars
    saveToLocalVars    (myList, num); // save to varNames
    genLocalVars       (varNames, varNum);
    genMultipleInitVars(myList, num);
    genMultipleBody    (myList, num);
    ChainExtra *extra = setSingleNodeVisible(myList, num);
    extra->funcFunc = strdup(code);
    extra->funcDecl = genDeclMultiple(temp, ';');
    extra->funcInvc = genInvcMultiple(temp, myList, num);
    // WP("code = %s\n",code); getchar();
}

// done
static void genCodeVectorSingle(fListNode **fList){
    fListNode *f = fList[0];
    if(isSpecialLike(f)){
        // WP("special like found!!!\n");
        gNode *rt = f->gnode;
        totalInputs(getNodeParams(rt->node), rt, 0, varNames); // load all vars
        saveToLocalVars(fList, 1);
        genCodeSpecialLike(f);
    }
    else if(isSpecialIndex(f)){
        // WP("special index found!!!\n");
        gNode *rt = f->gnode;
        totalInputs(getNodeParams(rt->node), rt, 0, varNames); // load all vars
        saveToLocalVars(fList, 1);
        genCodeSpecialIndex(f);
    }
    else {
        //printgNode(f->gnode);
        gNode *rt = f->gnode;
        totalInputs(getNodeParams(rt->node), rt, 0, varNames); // load all vars
        saveToLocalVars(fList, 1); // save to varNames
        genCodeAuto(rt, true);
        //debugCode((S)">> Gen code for vector: single");
    }
}

static void genCodeVectorMultipleBody(fListNode **fList, I num){
    fListNode *f = fList[0];
    gNode *rt = f->gnode;
    if(checkStmtByKind(rt->node, 'X')){
        gNode *nt = rt->pnode[0];
        Node *next = nt->node;
        if(checkStmtByKind(next, 'S')){
            WP("search string: %s\n", nodeVarName(getNodeVar1(f->nodeIter)));
            L id = searchName(varNames, nodeVarName(getNodeVar1(f->nodeIter)));
            glueAny(indent4 "L parZ[H_CORE], offset[H_CORE], total = 0;\n");
            glueAny(indent4 "DOI(H_CORE, parZ[i]=offset[i]=0)\n");
            glueAny(indent4 "DOT(vn(x%lld), if(vB(x%lld,i)) parZ[tid]++)\n",id,id);
            glueAny(indent4 "DOI(H_CORE, total+=parZ[i])\n");
            glueAny(indent4 "DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])\n");
            DOI(num, glueAny(indent4 "initV(z%lld, H_Q, total);\n",i))
            glueAny(indent4 "DOT(vn(x%lld),if(vB(x%lld,i)){ L c=offset[tid]++; \\\n",id,id);
            DOI(num, { if(i>0) glueAny(" \\\n"); \
                   glueAny(indent4 indent4 "vQ(z%lld,c) = ",i); \
                   genCodeAuto(fList[i]->gnode, false); })
            glueAny("})\n");
            glueAny(indent4 "R 0;\n");
            R ;
        }
    }
    DOI(num, genCodeAuto(fList[i]->gnode, true))
}


static void genCodeVectorMultiple(fListNode **myList, I num){
    WP("- Fusing %d fusible sections for vectors\n", num);
    printFusibleSections(myList, num);
    fListNode *f = myList[0];
    // generate function signature
    C temp[199];
    SP(temp, "q%d_autofusion_%d",qid,f->fuseId);
    glueCode(genDeclMultiple(temp, '{')); glueLine();
    saveToLocalVars    (myList, num); // save to varNames
    genLocalVars       (varNames, varNum);
    genReturnVars      (num);
    genCodeVectorMultipleBody(myList, num);
    glueAny("}\n");
    ChainExtra *extra = setSingleNodeVisible(myList, num);
    extra->funcFunc = strdup(code);
    extra->funcDecl = genDeclMultiple(temp, ';');
    extra->funcInvc = genInvcMultiple(temp, myList, num);
    // Node *tmp = myList[0]->nodeOpt;
    // L x2 = lookupSimpleHash(hashOpt, (L)tmp);
    // WP("1. visible: %s, %lld, x = %lld, x2 = %lld\n", getExtraKind(extra->kind), (L)tmp, (L)extra, x2);
    // printNode(tmp); getchar();
    // debugFusionNode(2);
    //WP("1 visible: %s, %lld\n", getExtraKind(extra->kind), (L)(myList[0]->nodeOpt));
    //printNode(myList[0]->nodeOpt); getchar();
    // debugCode((S)">> Gen code for vector: multiple");
}

static L genFusionCodeMain(fListNode **fList, I num, B isList){
    clearFusion();
    if(isList){
        if(num == 1)
            genCodeListSingle(fList);
        else
            genCodeListMultiple(fList, num);
    }
    else{
        if(num == 1)
            genCodeVectorSingle(fList);
        else
            genCodeVectorMultiple(fList, num);
    }
    L c = 0;
    if(isList && num == 1);
    else {
        DOI(num, c+=countFusedStmts(fList[i]))
        P(" // - #: %lld\n", c);
    }
    R c;
}


static void genFusionCode(){
    WP("# of fusible sections: %d\n", fListNum);
    printFusibleSections(fList, fListNum);
    L total = 0;
    if(fListNum == 1){
        total = genFusionCodeMain(fList, 1, fList[0]->isList);
    }
    else if(fListNum > 1){
        memset(fuseFlag, 0, sizeof(B)*fListNum);
        fListNode **temp = NEW2(fListNode, fListNum);
        curNum = 0;
        while(curNum < fListNum){
            // WP("cur = %d, fListNum =%d\n", curNum, fListNum);
            DOI(fListNum, if(!fuseFlag[i]){ fuseFlag[i]=true; \
                I curT=0; temp[curT++]=fList[i]; \
                B isList = fList[i]->isList; \
                DOJ3(i+1, fListNum, { \
                if(!fuseFlag[j] && \
                    isList == fList[j]->isList && \
                    fList[i]->nodeIter != NULL && \
                    fList[i]->nodeIter == fList[j]->nodeIter){ \
                WP("%lld, %lld, %s\n", i,j,obtainNodeStr(fList[i]->nodeIter)); \
                fuseFlag[j]=true; temp[curT++]=fList[j];}}) \
                total+=genFusionCodeMain(temp, curT, isList); \
                curNum += curT; })
            //debugFusionNode(2);
        }
        free(temp);
    }
    P("// Total # of fused stmts: %lld\n", total);
    // else do nothing
}
// if(fList[i]->nodeIter && fList[j]->nodeIter){ \
//     WP("%lld, %lld\n", (L)fList[i]->nodeIter, (L)fList[j]->nodeIter); \
// } \


static void compileMethod(Node *n){
    ChainList *chains = nodeMethodChainList(n);
    //printChainList(chains); getchar();
    analyzeChainBottomUp(chains);
    //genCodeListFusion(glist);
    genFusionCode();
}

static void scanMethodList(List *list){
    if(list) {
        scanMethodList(list->next);
        compileMethod(list->val);
    }
}

static void init(){
    code[0] = 0;
    ptr     = code;
    glist   = NEW(gNodeList);
    code_cond = NULL;
    hashgNode = initSimpleHash(1<<10);
    fListNum  = 0;
    depth     = 0;
    localIter = 'i';
}

static void clean(){
    freegNodeList(glist);
    freeSimpleHash(hashgNode);
}

void optAutoFusion(){
    init();
    scanMethodList(compiledMethodList->next);
    clean();
}


