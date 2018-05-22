#include "../global.h"

/* signatures */

extern Chain *exitChain;
extern bool ElementFuncMap[999];

static void genFusedFunc(char *str, char *targ);

#define chainNode(c) (c->cur)
#define isSimpleStmt(n) instanceOf(n,simpleStmtK)
#define isCastStmt(n) instanceOf(n,castStmtK)
#define isAssignment(n) (isSimpleStmt(n)||isCastStmt(n))
#define isReturnStmt(n) instanceOf(n,returnK)
#define setVisited(c, v) c->isVisited=v
#define isVisited(c) (c->isVisited)
#define resetBuff(b) if(b[0]!=0) b+=strlen(b) /* copied from pretty.c */
#define indent "    "

static char *list_name[256];
static int list_total = 0;
static int num_func = 0; /* number of fused functions */

bool isElementwise(char *funcName){
    int p = findFuncIndex(funcName);
    return ElementFuncMap[p];
}

static int findParamName(char *pName, Chain *chain){
    NameList *ptr = chain->uses;
    int c = 0;
    while(ptr->next){
        ptr = ptr->next;
        if(!strcmp(pName, ptr->name)) return c;
        c++;
    }
    return -1;
}

static void fuseNameClean(){
    DOI(list_total, free(list_name[i]))
    list_total = 0;
}

static void fuseNamePrint(){
    DOI(list_total, P(indent "V x%d = x[%d]; // %s\n",i,i,list_name[i]))
}

static void fuseNameByStr(char *buff, char *name, char *str){
    InfoNode *in = getInfoNode(name);
    pType k = in->type;
    ShapeNode *sn = in->shape;
    resetBuff(buff);
    //if(isVector(sn))
    switch(k){
        case  boolT: SP(buff, "vB(%s,i)", str); break;
        case   i64T: SP(buff, "vL(%s,i)", str); break;
        case   f64T: SP(buff, "vE(%s,i)", str); break;
        case  dateT: SP(buff, "vL(%s,i)", str); break;
        default: EP("type %d not supported yet\n", k);
    }
}

static void fuseName(char *buff, int nameID){
    char str[20]; SP(str, "x%d",nameID);
    fuseNameByStr(buff, list_name[nameID], str);
}

static int findNameID(char *name){
    DOI(list_total, if(!strcmp(list_name[i], name)) return i)
    /* add into list */
    list_name[list_total] = strdup(name);
    return list_total++;
}

static char *fuseNameFinal(char *buff, Chain *chain){
    /* must be assignment stmt */
    Node *name = chainNode(chain)->val.simpleStmt.name;
    Node *expr = chainNode(chain)->val.simpleStmt.expr;
    Node *func = expr->val.expr.func;  // function node
    if(func){
        char *targ = fetchName(name);
        fuseNameByStr(buff, targ, "z");
        strcat(buff, "=");
        return targ;
    }
    else error("Unexpexted case in loop fusion.\n");
}

static bool findFusionSub(Chain *chain, char *buff){
    resetBuff(buff); //buff[0]=0;
    if(isSimpleStmt(chainNode(chain))){
        Node *expr = chainNode(chain)->val.simpleStmt.expr;
        Node *func = expr->val.expr.func;  // function node
        if(!func) return false;            // literal assignment -> true
        Node *param= expr->val.expr.param; // parameter node
        char *funcName = fetchName(func);
        List *params   = param->val.listS;
        bool fusable   = isElementwise(funcName);
        if(!fusable) return false;
        sprintf(buff,"%s(",funcName); num_func++;
        int pId = 0;
        while(params){
            if(pId>0) strcat(buff, ",");
            Node *pLiteral = params->val;  //literalParamK
            Node *p = pLiteral->val.nodeS;
            if(instanceOf(p, idK)){
                char *pName = fetchName(p);
                int c = findParamName(pName, chain);
                if(c>=0){
                    if(!findFusionSub(chain->chain_defs[c], buff+strlen(buff))){
                        fuseName(buff, findNameID(pName));
                    }
                }
                else error("<0 index found\n");
            }
            else {
                prettyNodeBuff2C(buff,p);
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

static void findFusion(Chain *chain){
    //PN("------------");
    //printChainInfo(chain, 'a');
    //PN("+----------+");
    if(isSimpleStmt(chainNode(chain))){
        Node *expr = chainNode(chain)->val.simpleStmt.expr;
        Node *func = expr->val.expr.func;  // function node
        if(!func) return ;                 // literal assignment -> true
        Node *param= expr->val.expr.param; // parameter node
        char *funcName = fetchName(func);
        List *params   = param->val.listS;
        bool fusable   = isElementwise(funcName);
        bool fusableNext = fusable;
        char buff[512];  buff[0]=0;
        if(fusable && !isVisited(chain)) {
            fuseNameClean();
            char *targ = fuseNameFinal(buff, chain);
            findFusionSub(chain, buff);
            genFusedFunc(buff, targ);
        }
        while(params){
            Node *pLiteral = params->val;  //literalParamK
            Node *p = pLiteral->val.nodeS;
            if(instanceOf(p, idK)){
                char *pName = fetchName(p);
                int c = findParamName(pName, chain);
                if(c>=0){
                    findFusion(chain->chain_defs[c]);
                }
                else error("<0 index found\n");
            }
            params=params->next;
        }
    }
    else if(isReturnStmt(chainNode(chain))){
        DOI(chain->defSize, findFusion(chain->chain_defs[i]))
    }
    else {
        //printNodeKind(chainNode(chain));
    }
}

static void genFusedFunc(char *str, char *targ){
    P("num_func = %d\n", num_func);
    if(num_func > 1) { /* only valid gen */
        P("void q6_loopfusion_0(V z, V *x){\n");
        P(indent "// z -> %s\n",targ);
        fuseNamePrint();
        P(indent "DOI(r0, %s)\n", str);
        P("}\n");
    }
}

static void analyzeChain(Chain *chain){
    //if(isAssignment(chainNode(chain))){
    //}
    //char buff[512]; buff[0]=0;
    findFusion(chain);
    //P("buff = %s\n",buff);
    //prettyNode(chainNode(chain));
    //DOI(chain->defSize, analyzeChain(chain->chain_defs[i]))
}

/* entry */
void analyzeLF(){
    printBanner("Loop Fusion");
    analyzeChain(exitChain);
}

