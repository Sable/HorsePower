/*
 * Fusion elementwise functions
 */

#include "../global.h"

/* signatures */

extern Chain *exitChain;
extern bool ElementFuncMap[999];
extern I qid;

FuseNode FuseList[99];
I FuseTotal = 0;

static void genFusedFunc(char *str, char *targ);

#define resetBuff(b) if(b[0]!=0) b+=strlen(b) /* copied from pretty.c */

static char *list_name[256];
static int list_total = 0;
static int num_func = 0; /* number of fused functions */

bool isElementwise(char *funcName){
    int p = findFuncIndex(funcName);
    return ElementFuncMap[p];
}

int findParamName(char *pName, Chain *chain){
    NameList *ptr = chain->uses;
    int c = 0;
    while(ptr->next){
        ptr = ptr->next;
        if(!strcmp(pName, ptr->name)) return c;
        c++;
    }
    return -1;
}

void fuseNameClean(){
    DOI(list_total, free(list_name[i]))
    list_total = 0;
}

static void fuseNamePrint(){
    DOI(list_total, P(indentStr "V x%lld = x[%lld]; // %s\n",i,i,list_name[i]))
}

static S fuseNameString(S targ, S invc){
    char tmp[99]; SP(tmp, "%s(%s,(V []){", invc,targ);
    DOI(list_total, {if(i>0)strcat(tmp,",");strcat(tmp,list_name[i]);})
    strcat(tmp, "})");
    //P("tmp = %s, targ = %s, invc = %s\n", tmp,targ,invc);
    return strdup(tmp);
}

static L fuseLength(){
    DOI(list_total, {\
            ShapeNode *sn=getInfoNode(list_name[i])->shape;\
            if(!isShapeScalar(sn))R i;})
    R -1;
}

static void getNameTypeAlias(char *buff, char *name){
    InfoNode *in = getInfoNode(name);
    pType k = in->type;
    switch(k){
        case  boolT: strcpy(buff, "H_B"); break;
        case   i32T: strcpy(buff, "H_I"); break;
        case   i64T: strcpy(buff, "H_L"); break;
        case   f32T: strcpy(buff, "H_F"); break;
        case   f64T: strcpy(buff, "H_E"); break;
        case  dateT: strcpy(buff, "H_D"); break;
        case monthT: strcpy(buff, "H_M"); break;
        case   symT: strcpy(buff, "H_Q"); break;
        default: EP("type %s not supported yet 2\n", getpTypeName(k));
    }
}

static void fuseNameByStr(char *buff, char *name, char *str){
    InfoNode *in = getInfoNode(name);
    pType k = in->type;
    ShapeNode *sn = in->shape;
    resetBuff(buff);
    // check if 1-length vector
    bool is1 = isShapeScalar(sn);
    switch(k){
        case  boolT: SP(buff, is1?"vb(%s)":"vB(%s,i)", str); break;
        case   i32T: SP(buff, is1?"vi(%s)":"vI(%s,i)", str); break;
        case   i64T: SP(buff, is1?"vl(%s)":"vL(%s,i)", str); break;
        case   f32T: SP(buff, is1?"vf(%s)":"vF(%s,i)", str); break;
        case   f64T: SP(buff, is1?"ve(%s)":"vE(%s,i)", str); break;
        case  dateT: SP(buff, is1?"vd(%s)":"vD(%s,i)", str); break;
        case monthT: SP(buff, is1?"vm(%s)":"vM(%s,i)", str); break;
        case   symT: SP(buff, is1?"vq(%s)":"vQ(%s,i)", str); break;
        case  charT: SP(buff, is1?"vc(%s)":"vC(%s,i)", str); break;
        default: EP("type %s not supported yet\n", getpTypeName(k));
    }
}

void fuseName(char *buff, int nameID){
    char str[20]; SP(str, "x%d",nameID);
    fuseNameByStr(buff, list_name[nameID], str);
}

int findNameID(char *name){
    DOI(list_total, if(!strcmp(list_name[i], name)) return i)
    /* add into list */
    list_name[list_total] = strdup(name);
    return list_total++;
}

char *fuseNameTarg(char *buff, Chain *chain){
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

static void fuseNameUpperCase(char *buff, char *name){
    int len = strlen(name);
    DOI(len, buff[i]=(name[i]>='a'&&name[i]<='z')?name[i]-'a'+'A':name[i])
    buff[len]=0;
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
        fuseNameUpperCase(buff, funcName); strcat(buff,"("); num_func++;
        //sprintf(buff,"%s(",funcName); num_func++;
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
                else EP("<0 index found\n");
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
            char *targ = fuseNameTarg(buff, chain);
            num_func = 0;
            findFusionSub(chain, buff);
            if(num_func == 1) setVisited(chain, false);
            else if(num_func > 1) genFusedFunc(buff, targ);
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
                else EP("<0 index found\n");
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

/* num_func must > 1 */
static void genFusedFunc_C(char *str, char *targ){
    char tmp[99], type_alias[10];
    P("/* num_func = %d, targ = %s */\n", num_func,targ);
    SP(tmp,"q%d_loopfusion_%d",qid,FuseTotal);
    P("L %s(V z, V *x){\n",tmp);
    P(indentStr "// z -> %s\n",targ);
    fuseNamePrint();
    L size_index = fuseLength();
    getNameTypeAlias(type_alias, targ);
    if(size_index >= 0){
        P(indentStr "initV(z,%s,vn(x%lld));\n", type_alias, size_index);
        P(indentStr "DOP(vn(x%lld), %s) R 0;\n", size_index, str);
    }
    else{
        P(indentStr "initV(z,%s,1);\n", type_alias);
        P(indentStr "L i=0; %s; R 0;\n", str);
    }
    P("}\n");
    FuseList[FuseTotal].invc = fuseNameString(targ, tmp); 
    FuseList[FuseTotal].targ = strdup(targ); FuseTotal++;
}

static void genFusedFunc_ACC(char *str, char *targ){
    TODO("add code gen for OpenACC\n");
}

static void genFusedFunc(char *str, char *targ){
    switch(H_TARGET){
        case TARGET_C   : genFusedFunc_C(str, targ); break;
        case TARGET_LLVM: TODO("Support for LLVM\n"); break;
        case TARGET_ACC : genFusedFunc_ACC(str, targ); break;
        case TARGET_CL  : TODO("Support for OpenCL\n"); break;
        default: EP("TARGET platform is unknown: %d\n", H_TARGET);
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
void analyzeFusionElem(){
    printBanner("FE: Loop fusion with elementwise functions");
    analyzeChain(exitChain);
}

