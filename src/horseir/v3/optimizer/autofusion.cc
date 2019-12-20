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

static fListNode* fList[100];
static I fListNum;
static I depth;

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

static void genLocalVars(S *names, I n);

/* ---------- Above declarations ----------  */

 // copy from optimizer/pattern.c
static void genIndent(){ DOI(depth, glueCode(indent4)); }

static void debugCode(S msg){
    WP("Output code (%s):\n %s\n", msg,code);
    getchar();
}

static void addToSimpleHashUnsafe(sHashTable *st, L key, L val){
    if(!lookupSimpleHash(st, key)){
        addToSimpleHash(st, key, val);
    }
    // ignore the latest update
}

static void freegNodeList(gNodeList *g){
    while(g){
        gNodeList *t = g; g = g->next; free(t);
    }
}

static B isGroupReduction(S funcName){
    DOI(NUM_R, if(sEQ(funcName, ReductionNames[i])) R true) R false;
}

Node* getStmtCall(Node *stmt){
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

static fListNode* initfListNode(I fuseId){
    fListNode *x = NEW(fListNode);
    x->fuseId = fuseId;
    R x;
}

static C nextKind(Node *n){
    if(instanceOf(n, stmtK)){
        return getCallKind(getStmtCall(n));
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
static gNode* findFusionUp(Chain *chain, B isRT){
    if(isChainVisited(chain)) R NULL;
    //else setVisited(chain, true);
    Node *n = chainNode(chain);
    if(instanceOf(n, stmtK)){
        List *vars = nodeStmtVars(n);
        Node *call = getStmtCall(n);
        if(call){
            C kind = getCallKind(call);
            if(kind){
                if(kind=='R' && !isRT) R NULL;
            }
            else R NULL;
            if(isRT){ // root support
                switch(kind){
                    case 'R':
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
            I c = 0, cnt = 0;
            while(param){
                if(flag[cnt]){
                    switch(checkParamDef(param->val, chain, &c)){
                        case -1: rt->pnode[cnt] = NULL; break;
                        case  0: {
                            Chain *next = chain->chain_defs[c];
                            if(isChainVisited(next)){
                                L rtn = lookupSimpleHash(hashgNode, (L)next);
                                rt->pnode[cnt] = rtn?(gNode*)rtn:NULL;
                                //if(rtn){
                                //    gNode *t = (gNode*)rtn;
                                //    WP(".....\n"); printNode(t->node); getchar();
                                //}
                            }
                            else {
                                if('E' == kind && 'X' == nextKind(chainNode(next)));
                                else{
                                    rt->pnode[cnt] = findFusionUp(next, false);
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
    if(rt){
        if(pt){
            WP(indent4 "\"%s\" -> \"%s\"\n", \
                    obtainNodeStr(pt->node), \
                    obtainNodeStr(rt->node));
            // WP(" (%d)\n",rt->numUse);
        }
        L c = 0;
        if(!lookupSimpleHash(flag,(L)rt)){
            DOI(rt->pnum, c+=printgNodeCore(rt, rt->pnode[i], dep+1, flag))
            addToSimpleHash(flag, (L)rt, 1);
        }
        R c;
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
    L c = printgNodeCore(NULL, rt, 0, flag);
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
        printNode(rt->node);
        if(chain->useSize != rt->numUse){
            if(OPT_DEBUG)
                P("-- useSize = %d vs. numUse = %d\n", chain->useSize, rt->numUse);
            R 1;
        }
        else {
            if(OPT_DEBUG)
                P("++ useSize = %d vs. numUse = %d\n", chain->useSize, rt->numUse);
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

static void printAllNames(){
    WP("All local names (total %d):\n", varNum);
    DOI(varNum, WP("- %s\n", varNames[i]))
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
        //WP("dep = %d\n", dep);
        gNode *t = rt->pnode[dep];
        if(t) genCodeAuto(t, false);
        else genCodeBody(list->val, t);
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
        if(isKindR(rt)){
            glueAny(indent4);
            switch(getReductionKind(rt->funcName)){
                case sumR: 
                case avgR: glueAny("%c c = 0;", z0c); break;
                case minR: glueAny("%c c = %s_MAX;", z0c,getMinMax(z0c)); break;
                case maxR: glueAny("%c c = %s_MIN;", z0c,getMinMax(z0c)); break;
                case allR: glueAny("%c c = 1;", z0c); break;
                case anyR: glueAny("%c c = 0;", z0c); break;
                default: TODO("handle reduction op: %s", rt->funcName); break;
            }
            glueLine();
            glueAny(indent4 "initV(z, H_%c, 1);\n", z0c);
            SP(last, "v%c(z,0) = c;", z0c);
            glueAny(indent4 "DOP(vn(z), ");
        }
        else if(isKindE(rt)){
            glueAny(indent4 "initV(z, H_%c, vn(x0));\n", z0c);
            glueAny(indent4 "DOP(vn(z), v%c(z,i)=", z0c);
        }
        else if(isKindS(rt)){
            debugCode((S)"LINE 492");
            //TODO("Add scan");
        }
        else {
            TODO("Add support for kind: %c", rt->kind);
        }
        extra->funcDecl = genDeclSingle(temp, ';');
        extra->funcInvc = genInvcSingle(z0s, temp, varNames, varNum);
        resetCode(); oldPtr = ptr;
    }
    if(isKindS(rt)){
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
    else if(isKindE(rt) || isKindX(rt) || isKindB(rt)){
        Node *fn = getNodeFunc(n);
        glueAny("%s(", getFuncNameAuto(nodeName2(fn)));
        List *params = getNodeParams(n);
        genCodeParamReversed(getNodeParams(n), rt, 0);
        glueChar(')');
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
                SP(oldPtr, "if(%s){\\\n\t%s;}, reduction(%s:c)", code_cond, code_body, rop);
                free(code_cond);
                free(code_body);
                code_cond = NULL;
            }
            glueAny(")\n");
            // remaining
            if(isKindR(rt)){
                glueAny(indent4 "%s\n", last);
            }
            else if(isKindE(rt) || isKindX(rt) || isKindB(rt)){
                //WP("code = %s\n", code); getchar();
                // do nothing
            }
            else {
                TODO("Add impl. for kind %c", rt->kind);
            }
            glueAny(indent4 "R 0;\n}");
        }
        else if(isKindE(rt)){
            glueLine();
            glueAny(indent4 "R 0;\n}");
            //debugCode((S)"kind E");
        }
        else {
            TODO("Add more kind: %c\n", rt->kind);
        }
        extra->funcFunc = strdup(code);
    }
    addToSimpleHashUnsafe(hashOpt, (L)(rt->node), (L)extra); // insert to hash
}

static L loadParamsVec(Node **rtn, List *list){
    if(list){
        L k = loadParamsVec(rtn, list->next);
        rtn[k] = list->val;
        R k + 1;
    }
    R 0;
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

static ChainExtra* addNodeToChainExtra(Node *n, GenKind kind){
    ChainExtra *extra = NEW(ChainExtra);
    extra->kind = kind;
    addToSimpleHashUnsafe(hashOpt, (L)(n), (L)extra); // insert to hash
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
            case anyR: TODO("Add"); break;
            default: TODO("Unknown reduction: %s", rt->funcName);
        }
        debugCode((S)"LINE 654");
        genCodeAutoListBody(getNodeParams(n)->val, rt->pnode[0], id);
    }
    else if(isKindE(rt) || isKindX(rt) || isKindB(rt)){
        Node *fn = getEachFuncNode(n);
        glueAny("%s(", getFuncNameAuto(nodeName2(fn)));
        genCodeAutoListParam1(getNodeParams(n), rt, id); // skip 1st one
        glueChar(')');
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
        case lenR: break; // skip for len
        default: TODO("handle reduction op: %s, fid: %d", g->funcName,fid); break;
    }
    if(k != lenR) // skip lenR
        glueChar(' ');
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
    Node *z0 = f->nodeRtn;
    C z0c = getTypeCodeByName(z0);
    glueIndent();
    ReductionKind k = getReductionKind(g->funcName);
    switch(k){
        case lenR: glueAny("v%c(z%d,%c)=",z0c,fid,iter);
                   genCodeAutoListSingleLen(g); break;
        case sumR:
        case maxR: 
        case minR: glueAny("v%c(z%d,%c)=c%d", z0c,fid,iter,fid); break;
        case avgR: glueAny("v%c(z%d,%c)=c%d/vn(.)", z0c,fid,iter,fid); break;
        default: TODO("impl.");
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
    constIdNum = 0;
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

static Node* findIteratorNode(gNode *rt){
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

static void genCodeAutoListSingleLen(gNode *rt){
    //WP("numLast = %d\n", numLast);
    //DOI(numLast, printNode(tempLastNodes[i]))
    if(numLast > 0){
        List *vars = nodeStmtVars(tempLastNodes[0]);
        I total = totalList(vars);
        if(total == 1){
            Node *p = vars->val;
            S name = instanceOf(p, nameK)? nodeName2(p): instanceOf(p, varK)? nodeVarName(p):NULL;
            if(name)
                glueAny("vn(vV(x%lld,i))", searchName(varNames, name));
            else
                EP("Name is NULL");
            skipAllListNode(rt);
        }
        else TODO("Support multiple vars");
    }
    else EP("The numLast > 0, but %lld found", numLast);
}

static void copyLocalVarsToListNode(fListNode *f){
    f->varNum = varNum;
    f->varNames = NEWL(S, varNum);
    DOI(varNum, f->varNames[i]=strdup(varNames[i]))
}

static fListNode* collectFusibleSection(Node *cur, gNode *g, B isList){
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
        C kind = getCallKind(call);
        if(kind){
            // if(OPT_DEBUG && kind == 'R'){
            //     printNode(call);
            //     STOP("kind = %c\n", kind);
            // }
            // 1st pass, find all possible nodes
            gNode *rt = findFusionUp(chain, true);
			// 2nd pass, trim rt
			trimgNodeFromTop(rt); 
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
        //printChain(chain); P("%d", isChainVisited(chain)); getchar();
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

static O genLocalVars(S *names, I num){
    DOI(num, if(!strncmp(names[i], "id", 2)){\
        glueAny(indent4 "Q %s=getSymbol(\"%s\");\n",\
                names[i],\
                constId[getConstSymbolIdInt(names[i])]); \
        }\
        else { \
        glueAny(indent4 "V x%lld=x[%lld]; // %s\n",i,i,names[i]);})
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
    return addNodeToChainExtra(fList[0]->nodeOpt, OptG);
}

static O genMultipleInitVars(fListNode **fList, I num){
    Node *r0 = fList[0]->nodeIter;
    I r0x = getNameIndex(r0);
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
    P("3\n");
    DOI(num, genCodeListBody(fList[i]))   // skip len
    depth--;
    glueAnyLine("}) \\");
    P("4\n");
    DOI(num, genCodeListBodyLen(fList[i], 'j')) // do len
    depth--;
    glueAnyLine(")");
    depth--;
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
    getchar();
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
                STOP("funcName = %s\n", funcName);
                rt->pnode[0] = initgNode(p, 'S', funcName);
                rt->pnode[0]->gChain = next;
                if(sEQ(funcName, "compress"))
                    rt->pnode[1] = NULL;
                setVisited(chain, true);
                setVisited(next, true);
                printgNode(rt); getchar();
                R rt;
            }
        }
    }
    R NULL;
}



static void printFusibleSections(fListNode **fList, I fListNum){
    L c = 0;
    DOI(fListNum, { S iter=(S)obtainNodeStr(fList[i]->nodeIter); \
            WP("Fusible section: #%lld (iter: %s)\n",i,iter?iter:"NULL"); \
            L c0=printgNode(fList[i]->gnode); WP("\n (%lld)\n",c0); c+=c0; })
    WP("total statements fused: %lld\n", c);
    getchar();
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
    Node *r0 = f->nodeIter;
    gNode *g = f->gnode;
    S z0s = getNameStr(z0);
    C z0c = getTypeCodeByName(z0);
    I r0x = getNameIndex(r0);
    // set meta information
    if(sEQ(g->funcName, "len")){
        glueAny(indent4 "initV(z, H_%c, vn(x%d));\n",z0c,r0x);
        glueAny(indent4 "DOP(vn(z), vL(z,i)=");
        genCodeAutoListSingleLen(g);
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

static void genCodeListMultiple(fListNode **fList, I num){
    WP("- Fusing %d fusible sections for lists\n", num);
    fListNode *f = fList[0];
    // generate function signature
    C temp[199];
    SP(temp, "q%d_autofusionlist_%d",qid,f->fuseId);
    glueCode(genDeclSingle(temp, '{')); glueLine();
    // generate local vars
    saveToLocalVars    (fList, num); // save to varNames
    genLocalVars       (varNames, varNum);
    genMultipleInitVars(fList, num);
    genMultipleBody    (fList, num);
    //ChainExtra *extra = setSingleNodeVisible(fList, num);
    // extra->funcFunc = ...
    // extra->funcDecl = ...
    // extra->funcinvc = ...
    //STOP("code = %s",code);
    //TODO("impl. soon");
}

// done
static void genCodeVectorSingle(fListNode **fList){
    fListNode *f = fList[0];
    gNode *rt = f->gnode;
    totalInputs(getNodeParams(rt->node), rt, 0, varNames); // load all vars
    saveToLocalVars(fList, 1); // save to varNames
    genCodeAuto(rt, true);
    //debugCode((S)">> Gen code for vector: single");
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

static void genCodeVectorMultipleBody(fListNode **fList, I num){
    fListNode *f = fList[0];
    gNode *rt = f->gnode;
    if(checkStmtByKind(rt->node, 'X')){
        gNode *nt = rt->pnode[0];
        Node *next = nt->node;
        if(checkStmtByKind(next, 'S')){
            P("search string: %s\n", nodeVarName(getNodeVar1(f->nodeIter)));
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


static void genCodeVectorMultiple(fListNode **fList, I num){
    WP("- Fusing %d fusible sections for vectors\n", num);
    printFusibleSections(fList, num);
    fListNode *f = fList[0];
    // generate function signature
    C temp[199];
    SP(temp, "q%d_autofusion_%d",qid,f->fuseId);
    glueCode(genDeclSingle(temp, '{')); glueLine();
    saveToLocalVars    (fList, num); // save to varNames
    genLocalVars       (varNames, varNum);
    genReturnVars      (num);
    genCodeVectorMultipleBody(fList, num);
    glueAny("}\n");
    debugCode((S)">> Gen code for vector: multiple");
    STOP("check point");
}

static void genFusionCodeMain(fListNode **fList, I num, B isList){
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
}

static void genFusionCode(){
    WP("# of fusible sections: %d\n", fListNum);
    printFusibleSections(fList, fListNum);
    if(fListNum == 1){
        genFusionCodeMain(fList, 1, fList[0]->isList);
    }
    else if(fListNum > 1){
        B *flag = NEWL(B, fListNum);
        fListNode **temp = NEW2(fListNode, fListNum);
        I curNum = 0;
        while(curNum < fListNum){
            DOI(fListNum, if(!flag[i]){ flag[i]=true; \
                I curT=0; temp[curT++]=fList[i]; \
                B isList = fList[i]->isList; \
                DOJ3(i+1, fListNum, { \
                if(!flag[j] && \
                    isList == fList[j]->isList && \
                    fList[i]->nodeIter != NULL && \
                    fList[i]->nodeIter == fList[j]->nodeIter){ \
                STOP("%lld, %lld, %s\n", i,j,obtainNodeStr(fList[i]->nodeIter)); \
                flag[j]=true; temp[curT++]=fList[j];}}) \
                genFusionCodeMain(temp, curT, isList); \
                curNum += curT; })
        }
        free(flag);
        free(temp);
    }
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


