#include "../global.h"

/* Pattern: general */

typedef struct PatternTree{
    I num;                    // number of child nodes
    S func1, func2;        // functin names
    struct PatternTree **child; // child nodes
    Chain *chain;
}PatternTree;

static Node *currentMethod;
static PatternTree *allPattern[99];
static I numPattern, depth;
static B isACC;
static I optPatternKind;

extern List *compiledMethodList;
extern sHashTable *hashOpt;
extern I qid, phTotal;
extern C code[CODE_MAX_SIZE], *ptr;

/* ------ declaration above ------ */

static PatternTree *initPatternTree(const char *func1, const char *func2, I n){
    PatternTree *x = (PatternTree*)malloc(sizeof(PatternTree));
    x->func1 = strdup(func1);
    x->func2 = func2?strdup(func2):NULL;
    x->num   = n;
    x->child = 0==n?NULL:(PatternTree**)malloc(sizeof(PatternTree*) * n);
    x->chain = NULL;
    return x;
}

static void deletePatternTree(PatternTree *ptree){
    DOI(ptree->num, deletePatternTree(ptree->child[i]))
    free(ptree->func1);
    if(ptree->func2) free(ptree->func2);
    free(ptree->child);
    free(ptree);
}

static void printPatternTreeCore(PatternTree *ptree, I dep){
    if(ptree){
        DOI(dep*2+2, WP(" "))
        WP("(%s, %s, %d)\n", \
                ptree->func1?ptree->func1:"NULL", \
                ptree->func2?ptree->func2:"NULL", ptree->num);
        DOI(ptree->num, printPatternTreeCore(ptree->child[i], dep+1))
    }
}

static void printPatternTree(PatternTree *ptree){
    printPatternTreeCore(ptree, 0);
}

static PatternTree *createFP1(){
    if(debugPattern)
        WP("// Pattern FP0\n");
    return NULL;
}

static PatternTree *createFP2(I op){
    if(op!=1&&op!=2){
        EP("Pattern 2 must have an option 1 or 2");
    }
    if(debugPattern)
        WP("// Pattern FP2\n");
    PatternTree *rt = initPatternTree("raze", NULL, 1);
    rt->child[0] = initPatternTree("each", op==1?"sum":op==2?"avg":NULL, 1);
    rt->child[0]->child[0] = initPatternTree("each_right", "index", 0);
    return rt;
}

static PatternTree *createFP3(){
    if(debugPattern)
        WP("// Pattern FP3\n");
    PatternTree *rt = initPatternTree("index_a", NULL, 2);
    rt->child[0] = initPatternTree("vector", NULL, 1);
    rt->child[0]->child[0] = initPatternTree("len", NULL, 0);
    rt->child[1] = initPatternTree("compress", NULL, 2);
    rt->child[1]->child[0] = initPatternTree("lt", NULL, 0);
    rt->child[1]->child[1] = initPatternTree("?", NULL, 0);
    return rt;
}

static PatternTree *createFP4(){
    if(debugPattern)
        WP("// Pattern FP4\n");
    PatternTree *rt = initPatternTree("raze", NULL, 1);
    rt->child[0] = initPatternTree("each", "len", 1);
    rt->child[0]->child[0] = initPatternTree("each", "unique", 1);
    rt->child[0]->child[0]->child[0] = initPatternTree("each_right", "index", 0);
    return rt;
}

static PatternTree *createFP5(I op){
#define getFP5op(op,a,b,c) op==0?a:op==1?b:c
    if(debugPattern)
        WP("// Pattern FP%d\n", getFP5op(op,5,6,7));
    PatternTree *rt = initPatternTree("raze", NULL, 1);
    rt->child[0] = initPatternTree("each", getFP5op(op,"min","max","len"), 1);
    rt->child[0]->child[0] = initPatternTree("each_right", "index", 0);
    return rt;
}

/*
 * 0: targ
 * 1: 1st param
 * 2: 2nd param
 */
static Node *getParamFromChain(Chain *chain, I pos){
    return getNodeItemIndex(chainNode(chain), pos);
}

static B isNodeMarked(Node *n){
    R 0!=lookupSimpleHash(hashOpt, (L)n);
}

static B isChainMarked(Chain *chain){
    R isNodeMarked(chainNode(chain));
}

static void setAllChainVisitedNonRT(PatternTree *ptree){
    Chain *chain = ptree->chain;
    DOI(ptree->num, setAllChainVisitedNonRT(ptree->child[i]))
    if(sNEQ(ptree->func1, "?")) {
        setVisited(chain, true);
        ChainExtra *extra = NEW(ChainExtra);
        extra->kind = SkipG;
        addToSimpleHash(hashOpt, (L)chainNode(chain), (L)extra);
    }
    ptree->chain = NULL;
}

static void setAllChainVisited(PatternTree *ptree, S invc, S decl, S code){
    Chain *chain = ptree->chain;
    setVisited(chain, true);
    DOI(ptree->num, setAllChainVisitedNonRT(ptree->child[i]))
    ChainExtra *extra = NEW(ChainExtra);
    extra->kind = OptG;
    extra->funcInvc = invc;
    extra->funcDecl = decl;
    extra->funcFunc = code;
    addToSimpleHash(hashOpt, (L)chainNode(ptree->chain), (L)extra);
    ptree->chain = NULL;
}

/* ------ find patterns below ------ */

static char *phNameFP2(char *z, char *invc, char *x0, char *x1){
    char tmp[99];
    SP(tmp, "%s(%s,%s,%s)", invc,z,x0,x1);
    return strdup(tmp);
}

static char *phNameFP3(char *z, char *invc, char *x0, char *x1, char *y0, char *y1){
    char tmp[99];
    SP(tmp, "%s(%s,(V []){%s,%s}, (V []){%s,%s})", invc,z,x0,x1,y0,y1);
    return strdup(tmp);
}

static char *phNameFP4(char *z, char *invc, char *x, char *y){
    char tmp[99]; SP(tmp, "%s(%s,%s,%s)", invc,z,x,y);
    return strdup(tmp);
}

static S genDecl2(S func, C del){
    C temp[199]; 
    SP(temp, "static I %s(V z, V x, V y)%c", func, del);
    return strdup(temp);
}

static S genDecl2new(S func, C del){
    C temp[199]; 
    SP(temp, "static I %s(V z, V *x, V *y)%c", func, del);
    return strdup(temp);
}

static S genDecl3(S func, C del){
    C temp[199]; 
    SP(temp, "static I %s(V z, V x, V y)%c", func, del);
    return strdup(temp);
}

static void genIndent(){ DOI(depth, glueCode("    ")); }

static void genPattern2_C_Core(PatternTree *ptree, I op){
    if(isACC){
        TODO("Add directives for pattern 2");
    }
    C tmp[199];
    Chain *chain_x = ptree->child[0]->child[0]->chain;  // 
    Chain *chain_y = ptree->child[0]->chain;            // 
    Chain *chain_z = ptree->chain;                      //
    Node *x0 = getParamFromChain(chain_x, 2); S x0s = getNameStr(x0);
    Node *y1 = getParamFromChain(chain_x, 3); S y1s = getNameStr(y1);
    Node *z0 = getParamFromChain(chain_z, 0); S z0s = getNameStr(z0);
    C x0c = getTypeCodeByName(x0);
    C z0c = getTypeCodeByName(z0);
    //WP("x0 = %s, x1 = %s, y0 = %s, y1 = %s, z = %s\n", x0,x1,y0,y1,z);
    SP(tmp, "q%d_peephole_fp%d_%d", qid, op, phTotal++);
    glueCode(genDecl2(tmp, '{'));
    glueLine();
    depth++;
    glueAnyLine("// z -> %s", z0s);
    //glueAnyLine("V x0 = x[0]; // %s", x0s);
    //glueAnyLine("V x1 = x[1]; // %s", x1s);
    glueAnyLine("initV(z, H_%c, vn(y));", z0c);
    if(op == 1 || op == 2){ // sum or avg
        glueAnyLine("DOP(vn(y), {%c a=0; V t=vV(y,i); DOJ(vn(t), a+=v%c(x,vL(t,j))) v%c(z,i)=a%s;}) R 0;", x0c, x0c, z0c, op==2?"/vn(t)":"");
    }
    else if(op == 5 || op == 6){ // min or max
        glueAnyLine("DOP(vn(y), {%c a=%s; V t=vV(y,i); DOJ(vn(t), {%c t0=v%c(x,vL(t,j)); if(t0%ca)a=t0;}) v%c(z,i)=a;}) R 0;",
                     x0c, op==5?obtainMaxValue(x0c):obtainMinValue(x0c),
                     x0c, x0c, op==5?'<':'>', z0c);
    }
    else if(op == 7){
        glueAnyLine("DOP(vn(y), vL(z,i)=vn(vV(y,i))) R 0;");
    }
    else TODO("Add support: op = %d", op);
    depth--;
    glueAnyLine("}");
    S invc = phNameFP2(z0s, tmp, x0s, y1s);
    S decl = genDecl2(tmp,';');
    setAllChainVisited(ptree, invc, decl, strdup(code));
}

static void genPattern1_C(PatternTree *ptree){
    // sum
    genPattern2_C_Core(ptree, 1);
}

static void genPattern2_C(PatternTree *ptree){
    // avg
    genPattern2_C_Core(ptree, 2);
}

static void genPattern3_C(PatternTree *ptree){
    if(isACC){
        TODO("Add directives for pattern 3");
    }
    C tmp[199];
    Chain *chain_x  = ptree->child[1]->child[0]->chain;  // 2 params of lt
    Chain *chain_y0 = ptree->child[1]->chain;            // 2nd param of compress
    Chain *chain_y1 = ptree->child[0]->child[0]->chain;  // 1st param of len
    Chain *chain_z  = ptree->chain;                      // targ of index_a
    Node *x0 = getParamFromChain(chain_x , 1); S x0s = getNameStr(x0);
    Node *x1 = getParamFromChain(chain_x , 2); S x1s = getNameStr(x1);
    Node *y0 = getParamFromChain(chain_y0, 2); S y0s = getNameStr(y0);
    Node *y1 = getParamFromChain(chain_y1, 1); S y1s = getNameStr(y1);
    Node *z0 = getParamFromChain(chain_z , 0); S z0s = getNameStr(z0);
    //WP("x0 = %s, x1 = %s, y0 = %s, y1 = %s, z = %s\n", x0,x1,y0,y1,z);
    SP(tmp, "q%d_peephole_fp3_%d", qid, phTotal++);
    glueCode(genDecl2new(tmp,'{')); glueLine();
    depth++;
    glueAnyLine("// z -> %s", z0s);
    glueAnyLine("V x0 = x[0]; // %s", x0s);
    glueAnyLine("V x1 = x[1]; // %s", x1s);
    glueAnyLine("V y0 = y[0]; // %s", y0s);
    glueAnyLine("V y1 = y[1]; // %s", y1s);
    glueAnyLine("L r0 = vn(y1);");
    glueAnyLine("initV(z, H_B, r0);");
    glueAnyLine("DOP(vn(y0), if(v%c(x0,i)<v%c(x1,i))v%c(z,vL(y0,i))=1) R 0;",
            getTypeCodeByName(x0),getTypeCodeByName(x1),getTypeCodeByName(z0));
    depth--;
    glueAnyLine("}");
    S invc = phNameFP3(z0s, tmp, x0s, x1s, y0s, y1s);
    S decl = genDecl2new(tmp,';');
    setAllChainVisited(ptree, invc, decl, strdup(code));
}

static void genPattern4_C(PatternTree *ptree){
    if(isACC){
        TODO("Add directives for pattern 4");
    }
    C tmp[199];
    Chain *chain_x  = ptree->child[0]->child[0]->child[0]->chain;  // 2 params of each_right
    Chain *chain_z  = ptree->chain;  // 2 params of each_right
    Node *x0 = getParamFromChain(chain_x, 2); S x0s = getNameStr(x0);
    Node *y0 = getParamFromChain(chain_x, 3); S y0s = getNameStr(y0);
    Node *z0 = getParamFromChain(chain_z, 0); S z0s = getNameStr(z0);
    //WP("x = %s, y = %s, z = %s\n", x,y,z);
    SP(tmp, "q%d_peephole_fp4_%d", qid, phTotal++);
    glueCode(genDecl3(tmp,'{')); glueLine();
    //glueAnyLine("L %s(V z, V x, V y){", tmp);
    depth++;
    glueAnyLine("// z -> %s, x -> %s, y -> %s", z0s,x0s,y0s);
    glueAnyLine("L r0 = vn(y);");
    glueAnyLine("initV(z, H_L, r0);");
    glueAnyLine("DOP(r0, {V t=vV(y,i); L len=vn(t); L tot=0; B f[199]={0};\\");
    depth++;
    glueAnyLine("DOJ(len, if(!f[j]){ \\");
    char x0c = getTypeCodeByName(x0);
    glueAnyLine("DOK(len, if(k!=j && v%c(x,vL(t,j))==v%c(x,vL(t,k))) f[k]=1)\\",x0c,x0c);
    glueAnyLine("f[j]=1; tot++;}) vL(z,i)=tot; }) R 0;");
    depth--; 
    depth--; glueAnyLine("}");
    S invc = phNameFP4(z0s, tmp, x0s, y0s);
    S decl = genDecl3(tmp,';');
    setAllChainVisited(ptree, invc, decl, strdup(code));
}

static void genPattern5_C(PatternTree *ptree){
    // min
    genPattern2_C_Core(ptree, 5);
}

static void genPattern6_C(PatternTree *ptree){
    // max
    genPattern2_C_Core(ptree, 6);
}

static void genPattern7_C(PatternTree *ptree){
    // len
    genPattern2_C_Core(ptree, 7);
}

static void genPattern_C(PatternTree *ptree, I pid){
    //WP("gen pattern pid = %d\n", pid);
    switch(pid){
        case 1: genPattern1_C(ptree); break;
        case 2: genPattern2_C(ptree); break; // q3
        case 3: genPattern3_C(ptree); break;
        case 4: genPattern4_C(ptree); break;
        case 5: genPattern5_C(ptree); break; // q2
        case 6: genPattern6_C(ptree); break;
        case 7: genPattern7_C(ptree); break;
        default: EP("Pattern not supported yet: %d", pid);
    }
}

static void genPattern(PatternTree *ptree, I pid){
    resetCode();
    isACC = false;
    switch(qTarg){
        case TARGET_C    : genPattern_C(ptree, pid); break;
        case TARGET_ACC  : isACC = true;
                           genPattern_C(ptree, pid); break;
        case TARGET_LLVM : TODO("Support for LLVM");        break;
        case TARGET_CL   : TODO("Support for OpenCL");      break;
        default: EP("TARGET platform is unknown: %d", qTarg);
    }
}

static S getFirstParam(List *params){
    // scan to the last list
    while(params && params->next) params = params->next;
    if(params){
        Node *n = params->val;
        if(instanceOf(n, funcK)){
            I tot = totalList(n->val.listS);
            if(1 == tot){
                Node *func = n->val.listS->val;
                if(func->val.name.sn->kind == builtinS){
                    return func->val.name.id2;
                }
            }
        }
    }
    return NULL;
}

static B matchChain2(Chain *chain, S func1, S func2){
    if(isChainVisited(chain)) return false;
    if(instanceOf(chainNode(chain), stmtK)){
        Node *expr = chainNode(chain)->val.assignStmt.expr;
        Node *rhs = NULL;
        if(instanceOf(expr,callK)) rhs = expr;
        else if(instanceOf(expr, castK)) rhs = expr->val.cast.exp;
        else return false;
        Node *func = rhs->val.call.func;
        SymbolName *sn = func->val.name.sn;
        if(sn->kind != builtinS) return false;
        S funcName = func->val.name.id2;
        //WP("Input: %s, %s\n", funcName, func1);
        if(sEQ(funcName, func1) || sEQ(func1, "?")) {
            // match func2
            if(func2){
                List *params = rhs->val.call.param->val.listS;
                S firstP = getFirstParam(params);
                //WP("--> matched?: %s, %s | %s, %s\n", funcName, func1, firstP, func2);
                return firstP?sEQ(func2, firstP):false;
            }
            else return true;
        }
    }
    return false;
}

static B matchPattern(Chain *chain, PatternTree* ptree){
    if(matchChain2(chain, ptree->func1, ptree->func2)){
        I numOfDefs = ptree->num==0?0:chain->defSize;
        //WP("numOfDefs = %d, ptree->num = %d\n", numOfDefs, ptree->num);
        if(numOfDefs == ptree->num){
            //WP("num = %d, ptree func = %s\n", numOfDefs, ptree->func1);
            //prettyPatternTree(ptree->child[0]);
            //printChain(chain->chain_defs[0]); WP("\n");
            DOI(numOfDefs, \
                    if(!matchPattern(chain->chain_defs[i], ptree->child[i]))R false)
            //WP("return true\n"); getchar();
            ptree->chain = chain; /* setup */
            return true;
        }
    }
    //WP("func not matched: %s\n", ptree->func1);
    return false;
}

/* case 1: pattern trees */

static void findCase1(ChainList *list, PatternTree* ptree, I pid){
    if(list){
        findCase1(list->next, ptree, pid);
        // check chains visited or not
        // WP("[%d] ",isChainVisited(list->chain)); printChain(list->chain); WP("\n");
        if(!isChainVisited(list->chain) && !isChainMarked(list->chain)){
            if(matchPattern(list->chain, ptree)){
                // WP("Pattern %d found\n", pid); getchar();
                cleanCode(); ptr = code;
                genPattern(ptree, pid);
            }
        }
    }
}

/* pattern 2: common compress */

static B sameVar(Node *x, Node *y){
    if(instanceOf(x,nameK)){
        if(instanceOf(y,nameK)){
            if(nodeName1(x) && nodeName1(y)){
                if(sNEQ(nodeName1(x), nodeName1(y))) R 0;
            }
            return sEQ(nodeName2(x), nodeName2(y));
        }
        else if(instanceOf(y, varK)){
            return sEQ(nodeName2(x), y->val.param.id);
        }
    }
    R 0;
}


static B isValidPatternCompress(Chain *x, Node *n){
    if(!isChainMarked(x) && instanceOf(chainNode(x), stmtK)){
        Node *call = getStmtCall(chainNode(x));
        if(call){
            Node *func = nodeCallFunc(call);
            SymbolKind sk = nodeNameKind(func);
            if(sk == builtinS && sEQ(nodeName2(func),"compress")){
                Node *p = call->val.call.param->val.listS->next->val; // 1st param
                R sameVar(p, n);
            }
        }
    }
    R 0;
}

static S genDeclCompress(S func, C del){
    C temp[199]; 
    SP(temp, "static I %s(V *z, V x, V *y)%c", func, del);
    return strdup(temp);
}

static B allUsesValidCompress(Chain *chain, Node *n){
    DOI(chain->useSize, if(!isValidPatternCompress(chain->chain_uses[i],n))R 0) R 1;
}

static S genLocalCompress(S funcName, Chain *chain, Node *n){
    C temp[199]; S t = temp;
    t += SP(t, "%s((V []){", funcName);
    DOI(chain->useSize, { 
            Node *n  = chainNode(chain->chain_uses[i]);
            Node *z0 = getNodeItemIndex(n, 0);
            S z0s = getNameStr(z0); t += SP(t, (i==0?"%s":", %s"), z0s);
            glueAnyLine("V z%lld = z[%lld]; // %s",i,i,z0s); })
    S x0s = getNameStr(n);
    t += SP(t, "}, %s, (V []){", x0s);
    DOI(chain->useSize, { 
            Node *n  = chainNode(chain->chain_uses[i]);
            Node *y0 = getNodeItemIndex(n, 2);
            S y0s = getNameStr(y0); t += SP(t, (i==0?"%s":", %s"), y0s);
            glueAnyLine("V y%lld = y[%lld]; // %s",i,i,y0s); })
    t += SP(t, "})");
    R strdup(temp);
}

// TODO: fix code gen, see cgo20/semi/q1.h
static void genPatternCompress(Chain *chain, Node *n){
    cleanCode(); ptr = code;
    C temp[199];
    SP(temp, "q%d_patterncompress_%d", qid, phTotal++);
    glueCodeLine(genDeclCompress(temp, '{'));
    depth++;
    S invc = genLocalCompress(temp, chain, n);
    // prepare for compress
    glueAnyLine("L len = vn(x), k = 0;");
    glueAnyLine("L lenZ = 0, parZ[H_CORE], offset[H_CORE];");
    glueAnyLine("DOI(H_CORE, parZ[i]=offset[i]=0)");
    glueAnyLine("CHECKE(getNumOfNonZero(x,parZ));");
    glueAnyLine("DOI(H_CORE, lenZ += parZ[i])");
    glueAnyLine("DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])");
    DOI(chain->useSize, {
            Node *n = chainNode(chain->chain_uses[i]);
            Node *z0 = getNodeItemIndex(n, 0); C z0c = getTypeCodeByName(z0);
            glueAnyLine("initV(z%lld,H_%c,lenZ);",i,z0c); })
    C x0c = getTypeCodeByName(n);
    glueAnyLine("DOT(len, if(v%c(x,i)){L c=offset[tid]++;\\",x0c);
    depth++;
    DOI(chain->useSize, {
            Node *n = chainNode(chain->chain_uses[i]);
            Node *z0 = getNodeItemIndex(n, 0); C z0c = getTypeCodeByName(z0);
            Node *y0 = getNodeItemIndex(n, 2); C y0c = getTypeCodeByName(y0);
            glueAnyLine("v%c(z%lld,c) = v%c(y%lld,i);\\", z0c,i,y0c,i); })
    depth--;
    glueCodeLine("})");
    glueCodeLine("R 0;");
    depth--;
    glueCodeLine("}");
    // add to hash tables
    DOI(chain->useSize, {
            ChainExtra *extra = NEW(ChainExtra);
            if(i==chain->useSize-1) { // put the last
                extra->kind = OptG; 
                extra->funcInvc = invc;
                extra->funcDecl = genDeclCompress(temp, ';');
                extra->funcFunc = strdup(code);
            }
            else extra->kind = SkipG;
            setVisited(chain->chain_uses[i], true);
            Node *n = chainNode(chain->chain_uses[i]);
            addToSimpleHash(hashOpt, (L)n, (L)extra);})
}

static void searchAndGenPatternCompress(Chain* chain, Node *n){
    //printNode(chainNode(chain));
    //printNode(n);
    if(allUsesValidCompress(chain,n)){
        genPatternCompress(chain, n);
    }
}

static void matchPatternCompress(Chain *chain){
    if(!isChainMarked(chain) && chain->useSize > 1){
        Node *n = chainNode(chain);
        if(instanceOf(n, stmtK)){
            List *vars = n->val.assignStmt.vars;
            while(vars){
                // printNode(vars->val);
                searchAndGenPatternCompress(chain, vars->val);
                vars = vars->next;
            }
        }
    }
}

static void findCase2(ChainList *list){
    if(list){
        findCase2(list->next);
        Chain *chain = list->chain;
        // WP("[%d] [%d]",isChainVisited(chain),isChainMarked(chain)); printChain(chain); WP("\n");
        //if(!isChainVisited(chain) && !isChainMarked(chain)){
            // WP("--> [%d] [%d]",isChainVisited(chain),isChainMarked(chain)); printChain(chain); WP("\n");
            matchPatternCompress(chain);
        //}
    }
}

/* pattern 3: common index */

static B isValidPatternIndex(Chain *x, Node *n){
    if(instanceOf(chainNode(x), stmtK)){
        Node *call = getStmtCall(chainNode(x));
        if(call){
            Node *func = nodeCallFunc(call);
            SymbolKind sk = nodeNameKind(func);
            if(sk == builtinS && sEQ(nodeName2(func),"index")){
                Node *p = call->val.call.param->val.listS->val; // 2nd param
                R sameVar(p, n);
                // TODO: check 1st param is needed before
                // TODO: within current code block
            }
        }
    }
    R 0;
}

static B allUsesValidIndex(Chain *chain, Node *n){
    //printNode(chainNode(chain));
    //DOI(chain->useSize, WP("use[%lld] = %d\n",i,isValidPatternIndex(chain->chain_uses[i],n)));
    //getchar();
    DOI(chain->useSize, if(!isValidPatternIndex(chain->chain_uses[i],n))R 0) R 1;
}

static S genDeclIndex(S func, C del){
    C temp[199]; 
    SP(temp, "static I %s(V *z, V *x, V y)%c", func, del);
    return strdup(temp);
}

static S genLocals(S funcName, Chain *chain, Node *n){
    C temp[199]; S t = temp;
    t += SP(t, "%s((V []){", funcName);
    DOI(chain->useSize, { 
            Node *n  = chainNode(chain->chain_uses[i]);
            Node *z0 = getNodeItemIndex(n, 0);
            S z0s = getNameStr(z0); t += SP(t, (i==0?"%s":", %s"), z0s);
            glueAnyLine("V z%lld = z[%lld]; // %s",i,i,z0s); })
    t += SP(t, "}, (V []){");
    DOI(chain->useSize, { 
            Node *n  = chainNode(chain->chain_uses[i]);
            Node *x0 = getNodeItemIndex(n, 1);
            S x0s = getNameStr(x0); t += SP(t, (i==0?"%s":", %s"), x0s);
            glueAnyLine("V x%lld = x[%lld]; // %s",i,i,x0s); })
    S y0s = getNameStr(n);
    t += SP(t, "}, %s)", y0s);
    R strdup(temp);
}

static void genPatternIndex(Chain *chain, Node *n){
    cleanCode(); ptr = code;
    //DOI(chain->useSize, {printChain(chain->chain_uses[i]); WP("\n");}) getchar();
    //TODO("Impl. code gen for the pattern 'index'");  // example input: q1
    C temp[199];
    // step 0: function head
    SP(temp, "q%d_patternindex_%d", qid, phTotal++);
    glueCodeLine(genDeclIndex(temp, '{'));
    depth++;
    // step 1: local decl
    S invc = genLocals(temp, chain, n);
    // step 2: allocate memory
    DOI(chain->useSize, {
            Node *n = chainNode(chain->chain_uses[i]);
            Node *x0 = getNodeItemIndex(n, 1); C x0c = getTypeCodeByName(x0);
            glueAnyLine("initV(z%lld, H_%c, vn(y));",i,x0c); })
    // step 3: main loop
    glueCodeLine("DOP(vn(z0), {");
    C y0c = getTypeCodeByName(n);
    depth++;
    DOI(chain->useSize, {
            Node *n = chainNode(chain->chain_uses[i]);
            Node *z0 = getNodeItemIndex(n, 0); C z0c = getTypeCodeByName(z0);
            Node *x0 = getNodeItemIndex(n, 1); C x0c = getTypeCodeByName(x0);
            glueAnyLine("v%c(z%lld,i) = v%c(x%lld, v%c(y,i));", z0c,i,x0c,i,y0c); })
    depth--;
    glueCodeLine("})");
    glueCodeLine("R 0;");
    depth--;
    glueCodeLine("}");
    // add to hash tables
    DOI(chain->useSize, {
            ChainExtra *extra = NEW(ChainExtra);
            if(i==0) {
                extra->kind = OptG; 
                extra->funcInvc = invc;
                extra->funcDecl = genDeclIndex(temp, ';');
                extra->funcFunc = strdup(code);
            }
            else extra->kind = SkipG;
            setVisited(chain->chain_uses[i], true);
            Node *n = chainNode(chain->chain_uses[i]);
            addToSimpleHash(hashOpt, (L)n, (L)extra);})
}

static void searchAndGenPatternIndex(Chain *chain, Node *n){
    if(allUsesValidIndex(chain,n)){
        genPatternIndex(chain, n);
    }
    // else, partial fulfillment should be allowed as well
}

static void matchPatternIndex(Chain *chain){
    if(chain->useSize > 1){
        //printNode(chainNode(chain)); getchar();
        Node *n = chainNode(chain);
        if(instanceOf(n, stmtK)){
            List *vars = n->val.assignStmt.vars;
            while(vars){
                searchAndGenPatternIndex(chain, vars->val);
                vars = vars->next;
            }
        }
    }
}

static void findCase3(ChainList *list){
    if(list){
        findCase3(list->next);
        Chain *chain = list->chain;
        if(!isChainVisited(chain) && !isChainMarked(chain)){
            matchPatternIndex(chain);
        }
    }
}

#define doFirst(x) (x==1 || x==19 || x==22)

/*
 * Cases: compress(2) and index(3) have very similar code
 */
static void analyzeChain(ChainList *list){
    if(optPatternKind == 0){
        // STOP("Find all patterns: %d", numPattern);
        DOI(numPattern, findCase1(list, allPattern[i], i+1));
        //findCase1(list, allPattern[6], 7); // test with a specifc pattern
        findCase2(list); // compress
        findCase3(list); // index
    }
    else if(optPatternKind == 1){
        //STOP("opt p1");
        if(doFirst(qTpchId)){
            findCase2(list); // compress
        }
        else {
            DOI(numPattern, findCase1(list, allPattern[i], i+1));
            findCase3(list); // index
        }
    }
    else if(optPatternKind == 2){
        //STOP("opt p2");
        if(doFirst(qTpchId)){
            DOI(numPattern, findCase1(list, allPattern[i], i+1));
            findCase3(list); // index
        }
        else {
            findCase2(list); // compress
        }
    }
    else if(optPatternKind == 5){ // dls18
        DOI(numPattern, findCase1(list, allPattern[i], i+1));
        findCase2(list); // compress
    }
    else {
        EP("Unknown pattern kind: %d", optPatternKind);
    }
    
}

#define NiceSelect(x) ((x)?'x':' ')
void printChainVisited(ChainList *list){
    // check chains visited or not
    if(list){
        printChainVisited(list->next);
        WP("[%c] [%c] ",NiceSelect(isChainVisited(list->chain)),NiceSelect(isChainMarked(list->chain)));
        printChain(list->chain);
        WP("\n");
    }
}

void cleanChainVisited(ChainList *list){
    if(list){
        cleanChainVisited(list->next);
        setVisited(list->chain, false);
    }
}

static void compileMethod(Node *n){
    Node *prevMethod = currentMethod;
    currentMethod = n;
    ChainList *chains = n->val.method.meta->chains;
    // printChainList(chains); getchar();
    cleanChainVisited(chains->next);
    // printChainVisited(chains->next); getchar();
    analyzeChain(chains->next);
    // printChainVisited(chains->next); getchar();
    currentMethod = prevMethod;
}

static void scanMethodList(List *list){
    if(list) { scanMethodList(list->next); compileMethod(list->val); }
}

static void initPatterns(){
#define insertPattern(x) allPattern[numPattern++]=x
    numPattern = 0;
    PatternTree *fp1 = createFP2(1);
    PatternTree *fp2 = createFP2(2);
    PatternTree *fp3 = createFP3();
    PatternTree *fp4 = createFP4();
    PatternTree *fp5 = createFP5(0); // min
    PatternTree *fp6 = createFP5(1); // max
    //PatternTree *fp7 = createFP5(2); // len
    insertPattern(fp1);
    insertPattern(fp2);
    insertPattern(fp3);
    insertPattern(fp4);
    insertPattern(fp5);
    insertPattern(fp6);
    //insertPattern(fp7);
}

static void cleanPatterns(){
    DOI(numPattern, deletePatternTree(allPattern[i]))
}

static void init(){
    currentMethod = NULL;
    depth   = 0;
    code[0] = 0;
    ptr     = code;
    initPatterns();
    if(debugPattern){
        DOI(numPattern, {WP("pattern %lld:\n",i+1);printPatternTree(allPattern[i]);})
    }
}

void optPattern(I kind){
    printBanner("Fusion with Patterns");
    init();
    optPatternKind = kind;
    scanMethodList(compiledMethodList->next);
    cleanPatterns();
}


