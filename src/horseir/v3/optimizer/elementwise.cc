#include "../global.h"

typedef struct codegen_gnode{
    Node *node;
    I pnum; // # of parameter
    struct codegen_gnode *pnode[5]; // parameter nodes
}gNode;

extern List *compiledMethodList;
extern I qid, phTotal;
extern sHashTable *hashOpt;

extern I varNum;
extern S varNames[99];
extern C code[CODE_MAX_SIZE], *ptr;
extern C localIter;

static Node *currentMethod;

/* ------ declaration above ------ */

static void printChainUses(Chain *p){
    DOI(chainUseSize(p), {WP(" --> "); printChain(chainUse(p,i)); WP("\n"); })
}

static void printChainDefs(Chain *p){
    DOI(chainDefSize(p), {WP(" <-- "); printChain(chainDef(p,i)); WP("\n"); })
}

static B isMatchedDef(Chain *p, char *name){
    List *vars = nodeStmtVars(chainNode(p));
    while(vars){
        Node *n = vars->val;
        if(instanceOf(n, nameK) && nodeNameKind(n) == localS){
            if(sEQ(name, nodeName2(n))) return true;
        }
        else if(instanceOf(n, varK) && nodeVarKind(n) == localS){
            if(sEQ(name, nodeVarName(n))) return true;
        }
        vars = vars->next;
    }
    return false;
}

I findDefByName(Chain *p, char *name){
    I c = 0, x = -1;
    DOI(p->defSize, if(isMatchedDef(p->chain_defs[i], name)){c++; x=i;})
    return c==1?x:-1;
}

static B isMatchedUse(Chain *p, char *name){
    if(!instanceOf(chainNode(p), stmtK)) return false;
    Node *expr  = nodeStmtExpr(chainNode(p));
    List *param = expr->val.call.param->val.listS;
    while(param){
        Node *n = param->val;
        if(instanceOf(n, nameK) && nodeNameKind(n) == localS){
            if(sEQ(name, nodeName2(n))) return true;
        }
        param = param->next;
    }
    return false;
}

static I findUseByName(Chain *p, char *name){
    I c = 0, x = -1;
    DOI(p->useSize, if(isMatchedUse(chainUse(p,i), name)){c++; x=i;})
    return c==1?x:-1;
}

static gNode *initgNode(Node *node){
    gNode *x = NEW(gNode);
    x->node  = node;
    x->pnum  = totalList(getNodeParams(node));
    if(x->pnum >= 5)
        EP("Not enough space");
    return x;
}

static gNode *findFusionUp(Chain *chain){
    if(isChainVisited(chain)) return NULL;
    else chain->isVisited = true;
    Node *n = chainNode(chain);
    if(instanceOf(n, stmtK)){
        List *vars = nodeStmtVars(n);
        Node *call = getStmtCall(n);
        Node *func = nodeCallFunc(call);
        SymbolKind sk = nodeNameKind(func);
        if(!(sk == builtinS && isElementwise(nodeName2(func))))
            return NULL; // if not an elemetnwsie func
        //List *param = expr->val.call.param->val.listS;
        List *param = getNodeParams(n);
        //printChainUses(chain); getchar();
        // -- useful debugging
        //printBanner("Gotcha");
        //printChain(chain); WP("\n");
        gNode *rt = initgNode(chainNode(chain));
        B isOK = true;
        I cnt = 0;
        while(param){
            Node *p = param->val;
            if(instanceOf(p, nameK)){
                SymbolKind sk = nodeNameKind(p);
                if(sk == localS){
                    I c = findDefByName(chain, nodeName2(p));
                    //WP("c4 = %d, s = %s\n", c, nodeName2(p)); getchar();
                    if(c < 0) isOK = false;
                    else {
                        rt->pnode[cnt] = findFusionUp(chain->chain_defs[c]);
                    }
                }
                else isOK = false;
            }
            else rt->pnode[cnt] = NULL;
            if(!isOK) {free(rt); return NULL;}  // stop and exit
            param = param->next;
            cnt++;
        }
        return rt;
        // isOK == true
        // isOK == true, good ==> save
    }
    return NULL;
}

static Chain *findFusionDown(Chain *chain){
    Node *n = chainNode(chain);
    if(instanceOf(n, stmtK)){
        List *vars = nodeStmtVars(n);
        Node *call = getStmtCall(n);
        if(!call) return NULL;
        Node *func = nodeCallFunc(call);
        SymbolKind sk = nodeNameKind(func);
        if(!(sk == builtinS && isElementwise(nodeName2(func))))
            return NULL; // if not an elemetnwsie func
        B isOK = true;
        while(vars){
            Node *p = vars->val;
            if(instanceOf(p, varK)){
                if(p->val.param.sn->kind == localS){
                    I c = findUseByName(chain, nodeVarName(p));
                    //WP("c1 = %d, s = %s\n", c, nodeVarName(p)); getchar();
                    if(c < 0) isOK = false;
                    else {
                        Chain *foundChain = findFusionDown(chain->chain_uses[c]);
                        if(foundChain) return foundChain;
                    }
                }
                else isOK = false;
            }
            else if(instanceOf(p, nameK)){
                if(nodeNameKind(p) == localS){
                    I c = findUseByName(chain, nodeName2(p));
                    //WP("c2 = %d, s = %s\n", c, nodeName2(p)); getchar();
                    if(c < 0) isOK = false;
                    else {
                        Chain *foundChain = findFusionDown(chain->chain_uses[c]);
                        if(foundChain) return foundChain;
                    }
                }
                else isOK = false;
            }
            else isOK = false;
            //printChain(chain);
            //WP("isOK = %d\n", isOK); getchar();
            if(!isOK) return chain;
            vars = vars->next;
        }
        return chain;
    }
    return NULL;
}

static B isOK2Fuse(gNode *rt){
    // condition: more than 1 stmt
    DOI(rt->pnum, if(rt->pnode[i])R 1) R 0;
}

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


// TODO: remove duplicated items (only distinct values wanted)
static void totalInputs(gNode *rt, S *names){
    List *params = getNodeParams(rt->node);
    DOI(rt->pnum, {I k=i2-i-1; gNode *t=rt->pnode[k]; \
            if(t) totalInputs(t,names); \
            else {Node *p = getParamsIndex(params,k)->val; \
                if(instanceOf(p,nameK) && !isDuplicated(names,nodeName2(p))){ \
                    names[varNum++]=nodeName2(p); } \
                else{
                    Node *c = getSingleSymbol(p);
                    if(c){ // single const symbol
                        S idStr = (S)obtainConstSymbolId(c); \
                        if(idStr){
                            names[varNum++] = idStr;
                        }
                    } } } })
    //update sequence in names:
    //    name1,id,name2 --> name1,name2,id
    S temp[99]; L tt = 0, k = 0;
    DOI(varNum, if(!strncmp(names[i],"id",2))temp[tt++]=names[i])
    DOI(varNum, if(strncmp(names[i],"id",2))names[k++]=names[i])
    DOI(tt, names[k++]=temp[i])
}

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

static void genCodeElem(gNode *rt, B isRT){
    ChainExtra *extra = NEW(ChainExtra);
    extra->kind = isRT?OptG:SkipG;
    Node *n = rt->node;
    C temp[199];
    if(isRT){
        Node *z0 = getNodeItemIndex(n,0); S z0s = getNameStr(z0);
        C z0c = getTypeCodeByName(z0);
        SP(temp, "q%d_elementwise_%d",qid,phTotal++);
        glueCode(genDeclSingle(temp, '{')); glueLine();
        varNum = 0;
        totalInputs(rt, varNames);
        //DOI(varNum, glueAny(indent4 "V x%lld = x[%lld]; // %s\n",i,i,varNames[i]))
        genLocalVars(varNames, varNum);
        /* TODO: shape maybe not vn(x0) */
        glueAny(indent4 "initV(z, H_%c, vn(x0));\n", z0c); 
        glueAny(indent4 "DOP(vn(z), v%c(z,i)=",z0c);
        //DOI(varNum, WP(indent "V x%lld = x[%lld]; // %s\n",i,i,varNames[i]))
        //WP(indent "DOP(vn(z), v%c(z,i)=",z0c);
        // setup invocation for final fusion
        extra->funcDecl = genDeclSingle(temp, ';');
        extra->funcInvc = genInvcSingle(z0s, temp, varNames, varNum);
    }
    Node *fn = getNodeFunc(n);
    glueAny("%s(", getFuncNameC(nodeName2(fn)));
    List *params = getNodeParams(n);
    DOI(rt->pnum, { if(i>0) glueChar(comma); \
        I k=i2-i-1; gNode *t=rt->pnode[k]; \
        if(t) genCodeElem(t,false); \
        else { \
             Node *p = getParamsIndex(params,k)->val; \
             if(instanceOf(p,nameK)) genCodeName(p,searchName(varNames,nodeName2(p))); \
             else genCodeNode(p);
        } \
    })
    glueChar(')');
    if(isRT){
        glueCode(") R 0;\n}");
        extra->funcFunc = strdup(code);
    }
    addToSimpleHash(hashOpt, (L)(rt->node), (L)extra); // insert to hash
    //printChainExtra(extra);
}

static void findFusionSub(Chain *chain){
    Chain *bottom = findFusionDown(chain);
    if(bottom){
        // if num of chains > 1, likely fusion
        gNode *rt = findFusionUp(bottom);
        if(rt && isOK2Fuse(rt)){
            WP("bottom chain found:\n");
            //printChain(bottom); getchar();
            cleanCode(); ptr = code;
            genCodeElem(rt,true);
        }
    }
    else {
        chain->isVisited = true;
        //WP("bottom chain not found at: \n\t");
        //printChain(chain); WP("\n");
    }
}

static void findFusion(Chain *chain){
    Node *n = chainNode(chain);
    //printChain(chain); WP("\n");
    if(instanceOf(n, stmtK)){
        findFusionSub(chain);
    }
}

static void analyzeChain(ChainList *list){
    if(list){
        analyzeChain(list->next);
        if(!isChainVisited(list->chain))
            findFusion(list->chain);
    }
}

static void compileMethod(Node *n){
    Node *prevMethod = currentMethod;
    currentMethod = n;
    ChainList *chains = nodeMethodChainList(n);
    analyzeChain(chains);
    currentMethod = prevMethod;
    //printChainList(chains); getchar(); // TODO: printChainListBasic
}

static void scanMethodList(List *list){
    if(list) { scanMethodList(list->next); compileMethod(list->val); }
}

static void init(){
    currentMethod = NULL;
    localIter = 'i';
}

// entry: fuse elementwise
void optElementwise(){
    init();
    scanMethodList(compiledMethodList->next);
}

