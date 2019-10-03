#include "../global.h"

typedef struct codegen_node{
    C kind; S funcName;
    B isMarked; S codeStr; // if (isMarked) print codeStr
    I pnum; // # of parameter
    I numLast;
    Node *node;
    Node *lastNodes[5];
    struct codegen_node *pnode[5]; // parameter nodes
}gNode;

typedef struct gNode_list{
    gNode *g;
    Node  *lastNodes[10];
    I      numLast;
    struct gNode_list *next;
}gNodeList;

extern List *compiledMethodList;
extern I qid, phTotal;
extern sHashTable *hashOpt;

extern I varNum;
extern S varNames[99];
extern C code[CODE_MAX_SIZE], *ptr;

static S code_cond;
static sHashTable *hashgNode;
static gNodeList *glist;

#define isGroupElem(x) isElementwise(x)
#define isGroupScan(x) (sEQ(x,"compress")||sEQ(x,"where"))
#define isGroupIndex(x) sEQ(x,"index")
#define isGroupBool(x) (sEQ(x,"member")||sEQ(x,"like"))

#define isKindR(rt) (rt->kind == 'R')
#define isKindS(rt) (rt->kind == 'S')
#define isKindD(rt) (rt->kind == 'D')
#define isKindE(rt) (rt->kind == 'E')
#define isKindB(rt) (rt->kind == 'B')
#define isKindM(rt) (rt->kind == 'M')

static void genCodeAuto    (gNode *rt, B isRT);
static void genCodeAutoList(gNodeList *list, I size);
static void genCodeAutoListNode(gNode *rt, L id);

/* ---------- Above declarations ----------  */

static B isGroupReduction(S funcName){
    R sEQ(funcName, "sum") || sEQ(funcName, "avg") ||
      sEQ(funcName, "min") || sEQ(funcName, "max") ||
      sEQ(funcName, "all") || sEQ(funcName, "any");
}

static B isGroupMeta(S funcName){
    R sEQ(funcName, "len");
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

static C getFuncKind(S func){
    if(isGroupReduction(func)) R 'R';
    else if(isGroupScan(func)) R 'S';
    else if(isGroupIndex(func))R 'D';
    else if(isGroupBool(func)) R 'B';
    else if(isGroupElem(func)) R 'E';
    else if(isGroupMeta(func)) R 'M';
    else R 0;
}

C getCallKind(Node *call){
    Node *func    = nodeCallFunc(call);
    SymbolKind sk = nodeNameKind(func);
    R (builtinS == sk)? getFuncKind(nodeName2(func)): 0;
}

static B isGroupEach(S funcName){
    R sEQ(funcName, "each")      || sEQ(funcName, "each_item") ||
      sEQ(funcName, "each_left") || sEQ(funcName, "each_right");
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
    else setVisited(chain, true);
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
            if(isRT && kind != 'R') R NULL; // TODO: add more support for 'E' ...
            List *param = getNodeParams(n);
            //printChain(chain); getchar();
            gNode *rt = initgNode(n, kind, nodeName2(nodeCallFunc(call)));
            B flag[5]; DOI(5, flag[i]=0)
            switch(kind){ // mark parameters
                case 'E': 
                case 'S': flag[0] = flag[1] = 1; break;
                case 'D':
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
                                //    P(".....\n"); printNode(t->node); getchar();
                                //}
                            }
                            else rt->pnode[cnt] = findFusionUp(next, false);
                        } break;
                        case 1: case 2: free(rt); R NULL;
                    }
                }
                else rt->pnode[cnt] = NULL;
                param = param->next;
                cnt++;
            }
            //P("----add\n"); printChain(chain); P("\n");
            addToSimpleHash(hashgNode, (L)chain, (L)(rt));
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
                //P("func: %s, %c\n", funcEach, kind); getchar();
                if(isRT && (kind != 'R' && kind != 'M')) R NULL;
                gNode *rt = initgNode(n, kind, funcEach);
                B flag[5]; DOI(5, flag[i]=0)
                switch(kind){ // mark parameters
                    case 'E': 
                    case 'S': flag[0] = flag[1] = 1; break;
                    case 'D':
                    case 'B': flag[0] = 1; break; // 2nd param
                    case 'M': 
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
                addToSimpleHash(hashgNode, (L)chain, (L)rt);
                R rt;
            }
        }
    }
    R NULL;
}

static gNode *findFusionUpList(Chain *chain){
    if(isChainVisited(chain)) R NULL;
    else setVisited(chain, true);
    //printChain(chain); getchar();
    Node *n = chainNode(chain);
    if(instanceOf(n, stmtK)){
        List *param = getNodeParams(n);
        if(chainDefSize(chain) == 1){
            Chain *next = chainDef(chain, 0);
            R findFusionEach(next, true);
        }
    }
    R NULL;
}

typedef enum {
    unknownR, sumR, avgR, minR, maxR, allR, anyR
} ReductionKind;

static ReductionKind getReductionKind(S name){
    if(sEQ(name, "sum")) R sumR;
    else if(sEQ(name, "avg")) R avgR;
    else if(sEQ(name, "min")) R minR;
    else if(sEQ(name, "max")) R maxR;
    else if(sEQ(name, "all")) R allR;
    else if(sEQ(name, "any")) R anyR;
    else R unknownR;
}

static S getMinMax(C c){
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

/* ------ copy from: elementwise.c ------ */

static void totalInputs(List *list, gNode *rt, I dep, S *names){
#define isLeafNode(n) !lookupSimpleHash(hashgNode, (L)(n))
    if(list){
        totalInputs(list->next, rt, dep+1, names);
        gNode *t = rt->pnode[dep];
        if(t) totalInputs(getNodeParams(t->node), t, 0, names);
        else {
            Node *p = list->val;
            if(instanceOf(p,nameK) && !isDuplicated(names, nodeName2(p))){
                //P("p: "); printNode(p);
                //P("rt->node: "); printNode(rt->node); getchar();
                if(isLeafNode(p))
                    names[varNum++] = nodeName2(p);
                //else P("skip .... %s\n", nodeName2(p));
            }
        }
    }
}

/* ------ copy ends ----- */

static void printAllNames(){
    P("All local names (total %d):\n", varNum);
    DOI(varNum, P("- %s\n", varNames[i]))
}

static void genCodeBody(Node *p, gNode *g){
    if(g) genCodeAuto(g, false);
    else {
        if(instanceOf(p,nameK))
            genCodeName(p,searchName(varNames,nodeName2(p)));
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
        if(list->next) glueChar(',');
        //P("dep = %d\n", dep);
        gNode *t = rt->pnode[dep];
        if(t) genCodeAuto(t, false);
        else genCodeBody(list->val, t);
    }
}

static void genCodeAuto(gNode *rt, B isRT){
    if(rt->isMarked){
        glueCode(rt->codeStr); R ;
    }
    else rt->isMarked = true;
    ChainExtra *extra = NEW(ChainExtra);
    extra->kind = isRT?OptG:SkipG;
    Node *n = rt->node;
    C temp[199], last[199]; S oldPtr = NULL;
    if(isRT){
        Node *z0 = getNodeItemIndex(n,0);
        S z0s = getNameStr(z0);
        C z0c = getTypeCodeByName(z0);
        SP(temp, "q%d_autofusion_%d",qid,phTotal++);
        glueCode(genDeclSingle(temp, '{')); glueLine();
        varNum = 0;
        totalInputs(getNodeParams(n), rt, 0, varNames);
        DOI(varNum, glueAny(indent "V x%lld=x[%lld]; // %s\n",i,i,varNames[i]))
        //P("%s\n", code); getchar();
        //printAllNames(); getchar();
        glueAny(indent);
        if(isKindR(rt)){
            switch(getReductionKind(rt->funcName)){
                case sumR: 
                case avgR: glueAny("%c c = 0;", z0c); break;
                case minR: glueAny("%c c = %s_MAX;", z0c,getMinMax(z0c)); break;
                case maxR: glueAny("%c c = %s_MIN;", z0c,getMinMax(z0c)); break;
                case allR: glueAny("%c c = 1;", z0c); break;
                case anyR: glueAny("%c c = 0;", z0c); break;
            }
            glueLine();
            glueAny(indent "initV(z, H_%c, 1);\n", z0c);
            SP(last, "v%c(z,0) = c;", z0c);
            glueAny(indent "DOP(vn(z), ");
        }
        else if(isKindE(rt) || isKindD(rt) || isKindB(rt)){
            glueAny("DOP(vn(z), v%c(z,i)=", z0c);
        }
        else if(isKindS(rt)){
            TODO("Add scan");
        }
        extra->funcDecl = genDeclSingle(temp, ';');
        extra->funcInvc = genInvcSingle(z0s, temp, varNames, varNum);
        resetCode(); oldPtr = ptr;
    }
    if(isKindS(rt)){
        //printNode(n);
        List *params = getNodeParams(n);
        Node *p0 = params->val;
        Node *p1 = params->next->val;
        if(!code_cond){ // TODO: only one scan allowed currently
            genCodeScanCond(p1, rt->pnode[1]); // 1st param
            //P("cond: %s\n", code_cond); getchar();
        }
        genCodeBody(p0, rt->pnode[0]); // 2nd param
    }
    else if(isKindR(rt)){
        switch(getReductionKind(rt->funcName)){
            case sumR:
            case avgR: glueAny("c+=");   break;
            case minR: glueAny("if(c>"); break;
            case maxR: glueAny("if(c<"); break;
            case allR:
            case anyR: TODO("Add"); break;
        }
        Node *p0 = getNodeParams(n)->val;
        genCodeBody(p0, rt->pnode[0]);
    }
    else if(isKindE(rt) || isKindD(rt) || isKindB(rt)){
        Node *fn = getNodeFunc(n);
        glueAny("%s(", getFuncNameAuto(nodeName2(fn)));
        List *params = getNodeParams(n);
        genCodeParamReversed(getNodeParams(n), rt, 0);
        glueChar(')');
    }
    else TODO("Add impl. for kind: %c", rt->kind);
    if(isRT){
        if(code_cond){
            S code_body = strdup(oldPtr);
            SP(oldPtr, "if(%s){\\\n\t%s;}", code_cond, code_body);
            free(code_cond);
            free(code_body);
            code_cond = NULL;
        }
        glueAny(")\n");
        // remaining
        if(isKindR(rt)){
            glueAny(indent "%s\n", last);
        }
        else if(isKindE(rt) || isKindD(rt) || isKindB(rt)){
            //P("code = %s\n", code); getchar();
            // do nothing
        }
        else TODO("Add impl. for kind %c", rt->kind);
        glueAny(indent "R 0;\n}");
        extra->funcFunc = strdup(code);
    }
    addToSimpleHash(hashOpt, (L)(rt->node), (L)extra); // insert to hash
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
    //P("params:\n");
    //DOI(total, {P("%lld\n",i); printNode(rtn[i]);}) getchar();
    // important: reversed in rt->pnode
    DOIa(total, {gNode *t=rt->pnode[total-i-1]; \
            if(i>1) glueChar(','); \
            if(t) genCodeAutoListNode(t, id); \
            else genCodeBody(rtn[i], t); })
    // TODO: free rtn
}

static void genCodeAutoListLen(gNodeList *list, L id){
    gNode *rt = list->g;
    if(list->numLast > 0){
        Node *last = list->lastNodes[0];
        List *vars = nodeStmtVars(last);
        I total = totalList(vars);
        if(total == 1){
            Node *p = vars->val;
            S id = instanceOf(p, nameK)? nodeName2(p): instanceOf(p, varK)? nodeVarName(p):NULL;
            if(id)
                glueAny("vn(x%lld)", searchName(varNames, id));
            else
                EP("id is NULL");
        }
        else EP("Support multiple vars");
    }
    else EP("Error...");
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
            default: EP("Unknown reduction function: %s", rt->funcName);
        }
    }
    else if(isKindM(rt)){
        initStr[0] = 0; // do nothing
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
    if(isKindR(rt)){
        switch(getReductionKind(rt->funcName)){
            case sumR:
            case avgR: glueAny("c%lld+="  ,id); break;
            case minR: glueAny("if(c%lld>",id); break;
            case maxR: glueAny("if(c%lld<",id); break;
            case allR:
            case anyR: TODO("Add"); break;
        }
        genCodeAutoListBody(getNodeParams(n)->val, rt->pnode[0], id);
    }
    else if(isKindE(rt) || isKindD(rt) || isKindB(rt)){
        Node *fn = getEachFuncNode(n);
        //P("-------\n"); printNode(fn); getchar();
        glueAny("%s(", getFuncNameAuto(nodeName2(fn)));
        genCodeAutoListParam1(getNodeParams(n), rt, id); // skip 1st one
        glueChar(')');
    }
    else TODO("Add support kind: %c", rt->kind);
}

/*
 * An entry function for two cases
 *  - kind 'M': len (skip intermediate stmts)
 *  - other kinds require intermediate stmts
 */
static void genCodeAutoListMain(gNodeList *list, L id){
    gNode *rt = list->g;
    if(isKindM(rt)){
        genCodeAutoListLen(list, id); // search for last stmt
    }
    else genCodeAutoListNode(rt, id);
}

static void loadLocalVars(gNode *rt){
    Node *n = rt->node;
    totalInputs(getNodeParams(n), rt, 0, varNames);
}

static void genCodeAutoList(gNodeList *list, I size){
    /* list: no dummy node */
    //P("size = %d\n", size); getchar();
    if(size > 0){
        gNodeList *plist = list;
        // 1st pass: scan and store all local vars
        DOI(size, {loadLocalVars(plist->g); plist=plist->next;})
        //printAllNames(); getchar();
        // 2nd pass: gen pieces of code
        plist = list;
        //P("temp = \"%s\"\n", temp); getchar();
        //P("code = \"%s\"\n", code); getchar();
        DOI(size, {gNode *x=plist->g; C temp[99]; \
                genCodeAutoListInit(x,i,temp); \
                genCodeAutoListMain(plist,i); \
                plist=plist->next;})
    }
}

static void findFusionSub(Chain *chain){
    Node *n = chainNode(chain);
    if(instanceOf(n, stmtK)){
        Node *call = getStmtCall(n);
        if(call){
            if(getCallKind(call)){
    //printChain(chain);
    //P("\nrt: %d, isOK2Fuse(rt): %d\n", rt != 0, rt?isOK2Fuse(rt):0); getchar();
    // TODO: need top to bottom to check if fusion is allowed
                gNode *rt = findFusionUp(chain, true);
                if(rt && isOK2Fuse(rt)){
                    P("Fusion auto found:\n");
                    //printNode(rt->node); getchar();
                    cleanCode(); ptr = code;
                    genCodeAuto(rt, true);
                    //getchar();
                }
            }
            else if(isCallRaze(call)){
                gNode *rt = findFusionUpList(chain);
                if(rt && isOK2Fuse(rt)){
                    P("Fusion list found: \n");
                    insertgNode(glist, rt);
                    //DOI(rt->pnum, P("[%lld] %d\n", i,rt->pnode[i]!=NULL))
                    //cleanCode(); ptr = code;
                    //genCodeDeepList(rt, true);
                    //getchar();
                }
            }
        }
    }
}

static void findFusion(Chain *chain){
    if(instanceOf(chainNode(chain), stmtK)){
        findFusionSub(chain);
    }
    //InfoNode *in = getInfoFromNode(chainNode(chain));
}

static void analyzeChainBottomUp(ChainList *list){
/* order: bottom to up */
    while(list){
        if(!isChainVisited(list->chain))
            findFusion(list->chain);
        list = list->next;
    }
}

static I numListFusion(gNodeList *x){
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
static void genCodeListFusion(gNodeList *x){
    I num = numListFusion(x);
    B *flag = NEWL(B, num);
    gNodeList *tmp = NEW(gNodeList);
    P("# of list fusion: %d\n", num);
    gNodeList *a = x->next;
    I c = 0;
    while(a){
        if(!flag[c]) {
            flag[c] = 1;
            I k = c+1, size = 1;
            gNodeList *b = a->next;
            insertgNodeList(tmp, copygNodeList(a));
            while(b){
                if(!flag[k]){
                    if(sameLastNodes(a, b)){
                        flag[k] = 1;
                        insertgNodeList(tmp, copygNodeList(b));
                        size++;
                    }
                }
                b = b->next; k++;
            }
            genCodeAutoList(tmp->next, size); // gen code
            tmp->next = NULL;// TODO: free tmp
        }
        a = a->next; c++;
    }
    free(flag);
}

static void compileMethod(Node *n){
    ChainList *chains = nodeMethodChainList(n);
    //printChainList(chains); getchar();
    analyzeChainBottomUp(chains->next); // reversed topological order
    genCodeListFusion(glist);
}

static void scanMethodList(List *list){
    if(list) { scanMethodList(list->next); compileMethod(list->val); }
}

static void init(){
    code[0] = 0;
    ptr     = code;
    glist   = NEW(gNodeList);
    code_cond = NULL;
    hashgNode = initSimpleHash(1<<10);
}

void optAuto(){
    init();
    scanMethodList(compiledMethodList->next);
}


