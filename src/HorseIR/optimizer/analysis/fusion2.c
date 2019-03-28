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

static int num_func = 0; /* number of fused functions */

static void genFusedFunc2(char *str, char *targ);

#define resetBuff(b) if(b[0]!=0) b+=strlen(b) /* copied from pretty.c */

bool isFuncFusable(char *fn){
    int p = findFuncIndex(fn);
    return FusableFuncMap[p];
}

bool isReduction(char *fn){
    int p = findFuncIndex(fn);
    return isInReductionGroup(p);
}

/*
 * TODO:
 * - add fusable conditions
 */
static bool findFusionSub2(Chain *chain, char *buff){
    resetBuff(buff);
    Node *cur = chainNode(chain);
    if(isSimpleStmt(cur)){
        Node *expr = cur->val.simpleStmt.expr;
        Node *func = expr->val.expr.func;   // function node
        if(!func) return false;             // literal assignment -> true
        Node *param = expr->val.expr.param; // parameter node
        char *funcName = fetchName(func);
        List *params   = param->val.listS;
        bool isFusable = isFuncFusable(funcName);
        if(!isFusable) return false;
        if(!isVisited(chain)) num_func++;
        int pId = 0;
        while(params){
            if(pId>0) strcat(buff, ",");
            Node *p = params->val->val.nodeS;
            if(instanceOf(p, idK)){
                char *pName = fetchName(p);
                int c= findParamName(pName, chain);
                if(c>=0){
                    if(!findFusionSub2(chain->chain_defs[c], buff+strlen(buff))){
                        fuseName(buff, findNameID(pName));
                    }
                }
                else EP("<0 index found\n");
            }
            else {
                prettyNodeBuff2C(buff, p);
            }
            params=params->next;
            pId++;
        }
        strcat(buff, ")");
        setVisited(chain, true);
        return true;
    }
    return false;
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
            num_func = 0; /* ? num of statements */
            findFusionSub2(chain, buff);
            prettyNode(cur); P("\n");
            P("num = %d\n", num_func); getchar();
            if(num_func == 1) setVisited(chain, false);
            else if(num_func > 1) genFusedFunc2(buff, targ);
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
    P("genFusedFunc2\n"); getchar();
}

void analyzeFusion2(){
    printBanner("Advanced Fusion");
    findFusion2(exitChain);
    if(H_DEBUG) printChainList();
}

