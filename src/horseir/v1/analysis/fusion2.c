/*
 * fusion2 = advanced fusion
 *   including:
 *     - conformability analysis
 *     - fusion over groups
 */

#include "../global.h"

/* signatures */

extern Chain *exitChain;
extern bool FusableFuncMap[999];

typedef enum GroupName{
    elemG, redG, scanG, indexG, specialG, otherG
}gName;

typedef struct LoopObeject{
    I loop_size, loop_size_kind; /* 0:const; 1:symbolic */
    I loop_cond;
    S op_targ;
    S op_name;
    I op_valence; /* 1,2 */
    S op_args[2];
    B op_args_mask[2];
    struct LoopObeject *defs[2];
}*objLoop,objLoop0;

objLoop newLoopObject(){
    objLoop x = (objLoop)malloc(sizeof(objLoop0));
    memset(x, 0, sizeof(objLoop0));
    R x;
}

#define resetBuff(b) if(b[0]!=0) b+=strlen(b) /* copied from pretty.c */

static int     num_func = 0; /* number of fused functions */
static void    genFusedFunc2(char *str, char *targ);
static objLoop extractInfoStmt(Chain *chain);
static void    printLoopObject(objLoop x);

bool isFuncFusable(char *fn){
    int p = findFuncIndex(fn);
    return FusableFuncMap[p];
}

bool isReduction(char *fn){
    int p = findFuncIndex(fn);
    return isInReductionGroup(p);
}

static gName decideGroupName(char *fn){
    pFunc p = findFuncIndex(fn);
    if(isInElementwiseGroup(p))    R elemG;
    else if(isInReductionGroup(p)) R redG;
    else if(isInScanGroup(p))      R scanG;
    else if(isInIndexingGroup(p))  R indexG;
    else if(isInSpecialGroup(p))   R specialG;
    else R otherG;
}

static void printGroupName(gName x){
    switch(x){
        case    elemG: P("group(elementwise)"); break;
        case     redG: P("group(reduction)"  ); break;
        case    scanG: P("group(scan)"       ); break;
        case   indexG: P("group(indexing)"   ); break;
        case specialG: P("group(specialB)"   ); break;
        case   otherG: P("group(other)"      ); break;
        default:       P("group(error)"      ); break;
    }
    P("\n");
}

// static bool decideFusionMode(Chain *chain, char *buff, gName gn){
//     if(elemG == gn || redG == gn){ /* reduction mode */
//     }
// }

/*
 * TODO:
 * - add fusable conditions
 */
static objLoop findFusionSub2(Chain *chain){
    Node *cur = chainNode(chain);
    if(isSimpleStmt(cur)){
        Node *expr = cur->val.simpleStmt.expr;
        Node *func = expr->val.expr.func;   // function node
        if(!func) return NULL;             // literal assignment -> true
        Node *param = expr->val.expr.param; // parameter node
        char *funcName = fetchName(func);
        List *params   = param->val.listS;
        bool isFusable = isFuncFusable(funcName);
        if(!isFusable) return NULL;
        if(!isVisited(chain)) num_func++;
        objLoop curObj = extractInfoStmt(chain);
        int valence = 0;
        while(params){
            Node *p = params->val->val.nodeS;
            objLoop defObj = NULL;
            if(instanceOf(p, idK)){
                char *pName = fetchName(p);
                int c= findParamName(pName, chain);
                if(c>=0){
                    defObj = findFusionSub2(chain->chain_defs[c]);
                }
                else EP("<0 index found\n");
            }
            else {
                //prettyNodeBuff2C(buff, p);
            }
            curObj->defs[valence] = defObj;
            params=params->next;
            valence++;
        }
        setVisited(chain, true);
        return curObj;
    }
    return NULL;
}

static void findFusion2(Chain *chain){
    Node *cur = chainNode(chain);
    //prettyNode(cur); P("\n");
    if(isSimpleStmt(cur)){
        Node *expr = cur->val.simpleStmt.expr;
        Node *func = expr->val.expr.func;   // function node
        if(!func) return ;                  // literal assignment
        Node *param = expr->val.expr.param; // parameter node
        char *funcName = fetchName(func);
        List *params   = param->val.listS;
        bool isFusable = isFuncFusable(funcName);
        bool isReduct  = isReduction(funcName);
        char buff[512]; buff[0]=0;
        P("function name: %-10s, %-5s\n",funcName,isFusable?"true":"false");
        /* customized code starts */
        if(isFusable && !isVisited(chain)){
            fuseNameClean();
            char *targ = fuseNameTarg(buff, chain);
            gName gn   = decideGroupName(funcName);
            //P("targ = %s\n", targ); getchar();
            num_func = 0; /* ? num of statements */
            objLoop rootObj = findFusionSub2(chain);
            printLoopObject(rootObj); getchar();
            prettyNode(cur); P("\n");
            P("num = %d\n", num_func); getchar();
            if(num_func == 1) setVisited(chain, false);
            else if(num_func > 1) genFusedFunc2(buff, buff);
        }
        /* customized code ends   */
        while(params){
            Node *p = params->val->val.nodeS;
            if(instanceOf(p, idK)){
                char *pName = fetchName(p);
                int c= findParamName(pName, chain);
                if(c>=0) {
                    findFusion2(chain->chain_defs[c]);
                }
                else EP("<0 index found\n");
            }
            params=params->next;
        }
    }
    else if(isReturnStmt(cur)){
        DOI(chain->defSize, findFusion2(chain->chain_defs[i]))
    }
    else {
        //printNodeKind(cur);
    }
}

static void genFusedFunc2(char *str, char *targ){
    P("genFusedFunc2\n");
    P("%s\n",str);
    getchar();
}

/* add new functions below */

static S getTargStr(Chain *chain){
    Node *cur = chainNode(chain);
    if(isSimpleStmt(cur)){
        Node *name = cur->val.simpleStmt.name;
        return fetchName(name);
    }
    else EP("not a simple statement\n");
}

static B isVarVector(S var){
    if(var){
        InfoNode *in = getInfoNode(var);
        ShapeNode *sn= in->shape;
        R !isShapeScalar(sn);
    }
    else R false;
}

static void getLoopInfo(S var, objLoop obj){
    if(var){
        InfoNode *in  = getInfoNode(var);
        ShapeNode *sn = in->shape;
        if(isShapeV(sn)){
            if(isSNConst(sn)){
                obj->loop_size = sn->size;
                obj->loop_cond = -1;
                obj->loop_size_kind = 0;
            }
            else if(isSNId(sn)){
                obj->loop_size = sn->sizeId;
                obj->loop_cond = -1;
                obj->loop_size_kind = 1;
            }
            else if(isSNScan(sn)){
                obj->loop_size = sn->sizeScan;
                obj->loop_cond = sn->sizeScan;
                obj->loop_size_kind = 1;
                //P("scan catched"); getchar();
            }
            else EP("unexpected condition\n");
        }
        else EP("only consider vector shape now\n");
    }
}

static objLoop extractInfoStmt(Chain *chain){
    I valence = 0;
    Node *cur = chainNode(chain);
    if(isSimpleStmt(cur)){
        objLoop obj= newLoopObject();
        Node *name = cur->val.simpleStmt.name;
        Node *expr = cur->val.simpleStmt.expr;
        Node *func = expr->val.expr.func;
        if(!func) return NULL;
        Node *param = expr->val.expr.param;
        char *funcName = fetchName(func);
        char *targName = fetchName(name);
        List *params   = param->val.listS;
        InfoNode *in = getInfoNode(targName);
        while(params){ 
            Node *p = params->val->val.nodeS;
            S pn = instanceOf(p, idK)?fetchName(p):NULL;
            obj->op_args[valence] = pn;
            obj->op_args_mask[valence] = isVarVector(pn);
            valence++;
            params=params->next;
        }
        obj->op_name = fetchName(func);
        obj->op_targ = targName;
        obj->op_valence = valence;
        getLoopInfo(targName, obj);
        //P("targName = %s, funcName = %s\n", targName, funcName);
        R obj;
    }
    else EP("not supported\n");
    R NULL;
}

static void printLoopObjectDep(objLoop x, I dep){
#define printS(x) x?x:"NULL"
    if(!x) R;
    I valence = x->op_valence;
    DOI(dep, P("  "))
    P("{%c(%d),%d,{%s,%s,%d,",
            x->loop_size_kind==0?'c':'s',
            x->loop_size,
            x->loop_cond,
            printS(x->op_targ),
            printS(x->op_name),
            x->op_valence);
    DOI(valence, {if(i>0)P(",");P("{%s,%d}",printS(x->op_args[i]),x->op_args_mask[i]);})
    P("}}\n");
    DOI(valence, printLoopObjectDep(x->defs[i],dep+1))
}

static void printLoopObject(objLoop x){
    printLoopObjectDep(x, 0);
}


void analyzeFusion2(){
    printBanner("Advanced Fusion");
    findFusion2(exitChain);
    if(H_DEBUG) printChainList();
}

