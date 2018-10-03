#include "../global.h"

/* signatures */
static void scanNode(Node *n);
static void scanList(List *list);
static void scanSimpleStmt(Node *n);
static void scanCastStmt(Node *n);
static void scanExpr(Node *n);

#define scanModule(n)    scanList(n->val.module.body)
#define scanMethod(n)    scanList(n->val.method.list)
#define scanParam(n)     scanNode(n->val.nodeS)
#define scanParamExpr(n) scanList(n->val.listS)

#define cmp(m,s) !strcmp(m, s)
#define isMul(m) cmp(m,"mul")

static void scanSimpleStmt(Node *n){
    Node *expr = n->val.simpleStmt.expr;
    scanExpr(expr);
}

static void scanCastStmt(Node *n){
    Node *expr = n->val.castStmt.expr;
    scanExpr(expr);
}

static void printValueNode(V v){
    if(v->len == 1){
        switch(v->typ){
            caseB P("%d:b"  ,v->b  ); break;
            caseL P("%d:i64",v->i64); break;
            caseD P("%d:d"  ,v->d  ); break;
            caseE P("%g:f64",v->f64); break;
            default: P("type: %d not supported\n", v->typ);
        }
    }
    else if(v->len > 1){
        switch(v->typ){
            caseB {int *num=(int*)v->g; P("("); DOI(v->len, {if(i>0)P(",");P("%d",num[i]);}) P("):b");} break;
            caseD {int *num=(int*)v->g; P("("); DOI(v->len, {if(i>0)P(",");P("%d",num[i]);}) P("):d");} break;
            caseL {int *num=(int*)v->g; P("("); DOI(v->len, {if(i>0)P(",");P("%d",num[i]);}) P("):i64");} break;
            caseE {double *num=(double*)v->g; P("("); DOI(v->len, {if(i>0)P(",");P("%g",num[i]);}) P("):f64");} break;
            default: P("kind: %d not supported\n", v->typ);
        }
    }
}

static int kind2type(Kind k){
    switch(k){
        case   literalIntK: return H_L;
        case  literalDateK: return H_D;
        case   literalSymK: return H_Q;
        case  literalBoolK: return H_B;
        case literalFloatK: return H_E;
        default: EP("kind to type fail : k = %d\n", k);
    }
}

static V fetchLiteralFloat(Node *n, Kind k){
    List *list; int c=0;
    for(list=n->val.listS;list!=NULL;list=list->next) c++;
    double *num = NEWL(double,c); c=0;
    for(list=n->val.listS;list!=NULL;list=list->next) {
        if(list->val->kind == intK)
            num[c++] = list->val->val.intS;
        else
            num[c++] = list->val->val.floatS;
    }
    V z = allocNode();
    initV(z, kind2type(k), c);
    if(c==1){
        switch(k){
            case literalFloatK: ve(z) = num[0]; break;
        }
    }
    else if(c>0){
        switch(k){
            case literalFloatK: DOI(c, vE(z,i) = num[i]) break;
        }
    }
    free(num);
    return z;
}
static V fetchLiteralInt(Node *n, Kind k){
    List *list; int c=0;
    for(list=n->val.listS;list!=NULL;list=list->next) c++;
    int *num = NEWL(int,c); c=0;
    for(list=n->val.listS;list!=NULL;list=list->next) {
        num[c++] = list->val->val.intS;
    }
    V z = allocNode();
    initV(z, kind2type(k), c);
    if(c==1){
        switch(k){
            case  literalBoolK: vb(z) = num[0]; break;
            case   literalIntK: vl(z) = num[0]; break;
            case  literalDateK: vd(z) = num[0]; break;
        }
    }
    else if(c>0){
        switch(k){
            case  literalBoolK: DOI(c, vB(z,i) = num[i]) break;
            case   literalIntK: DOI(c, vL(z,i) = num[i]) break;
            case  literalDateK: DOI(c, vD(z,i) = num[i]) break;
        }
    }
    free(num);
    return z;
}

static V fetchLiteralSym(Node *n, Kind k){
    List *list; int c=0;
    for(list=n->val.listS;list!=NULL;list=list->next) c++;
    char **sym = NEW2(char,c); c=0;
    for(list=n->val.listS;list!=NULL;list=list->next) {
        sym[c++] = list->val->val.symS;
    }
    V z = allocNode();
    initV(z,H_Q,c);
    if(c==1){
        vq(z) = getSymbol(sym[0]);
        free(sym);
    }
    else if(c>0){
        DOI(c, vQ(z,i)=getSymbol(sym[i]))
    }
    return z;
}

//static V fetchLiteralFunc(Node *n, Kind k){
//    Node *name = n->val.nodeS;
//    char *sym = name->val.idS;
//    V z = allocNode();
//    initV(z,H_Q,1);
//    vq(z) = getSymbol(sym); 
//    return z;
//}

static V fetchLiteralFunc(Node *n, Kind k){
    List *list; int c=0;
    for(list=n->val.listS;list!=NULL;list=list->next) c++;
    char **sym = NEW2(char,c); c=0;
    for(list=n->val.listS;list!=NULL;list=list->next) {
        // TODO: check name1
        sym[c++] = list->val->val.nodeS->val.compoundID.name2;
    }
    V z = allocNode();
    initV(z,H_Q,c);
    if(c==1){
        vq(z) = getSymbol(sym[0]);
    }
    else if(c>1){
        DOI(c, vQ(z,i)=getSymbol(sym[i]))
    }
    if(c>0) free(sym);
    return z;
}

static V fetchLiteralStr(Node *n){
    List *t = n->val.listS;
    V z = allocNode();
    L c=0; while(t){c++; t=t->next;}
    t = n->val.listS;
    initV(z,H_S,c);
    c=0; while(t){vS(z,c)=strdup(t->val->val.strS); c++; t=t->next;}
    R z;
}

static V fetchLiteralChar(Node *n){
    char *charS= n->val.charS;
    L len = strlen(charS);
    V z = allocNode();
    initV(z,H_C,len);
    if(len==1) vc(z) = charS[0];
    else if(len>1) strcpy(vg(z), charS);
    else vg(z) = NULL;
    return z;
}

V getLiteralFromNode(Node *n){
    Node *paramValue = n->val.nodeS;
    switch(paramValue->kind){
        case  literalBoolK: return fetchLiteralInt  (paramValue, literalBoolK );
        case   literalIntK: return fetchLiteralInt  (paramValue, literalIntK  );
        case  literalDateK: return fetchLiteralInt  (paramValue, literalDateK );
        case literalFloatK: return fetchLiteralFloat(paramValue, literalFloatK);
        case   literalSymK: return fetchLiteralSym  (paramValue, literalSymK  );
        case  literalFuncK: return fetchLiteralFunc (paramValue, literalFuncK );
        case   literalStrK: return fetchLiteralStr  (paramValue);
        case  literalCharK: return fetchLiteralChar (paramValue);
        //default: printNodeKind(paramValue);
        //         EP("[getLiteralFromNode]: kind (%d) not supported yet.\n", paramValue->kind);
    }
    return NULL;
}

static bool isSingleInt(V vn, int c, Node *var){
    if(vn->typ == H_L && vn->len == 1){
        return vn->i64 == c;
    }
    else if(vn->typ == H_E && vn->len == 1){
        InfoNode *in = getInfoNode(fetchName(var->val.nodeS));
        return (vn->f64 == c) && (in->type == H_E);
    }
    else return false;
}

static void scanExpr(Node *n){
    Node *func = n->val.expr.func;
    Node *param= n->val.expr.param;
    if(func){
        char *funcName = fetchName(func);
        List *params   = param->val.listS;
        if(isMul(funcName)){
            //prettyNode(param);
            Node *p1 = params->val;
            Node *p2 = params->next->val;
            V v1 = getLiteralFromNode(p1);
            //if(v1 != NULL) printValueNode(v1);
            V v2 = getLiteralFromNode(p2);
            //if(v2 != NULL) printValueNode(v2);
            if(!v1 && v2){ // mul(var, const)
                if(isSingleInt(v2, 2, p1)){
                    P("Update: "); prettyNode(n);
                    /* expr.func */
                    Node *funcNew = NEW(Node);
                    funcNew->val.idS = strdup("plus");
                    funcNew->kind= idK;
                    n->val.expr.func = funcNew;
                    /* expr.param */
                    params->next->val = p1;
                    P(" => "); prettyNode(n); P("\n");
                }
            }
            else if(v1 && !v2){
                if(isSingleInt(v1, 2, p2)){
                    prettyNode(p2);
                    P("Update: "); prettyNode(n);
                    /* expr.func */
                    Node *funcNew = NEW(Node);
                    funcNew->val.idS = strdup("plus");
                    funcNew->kind= idK;
                    n->val.expr.func = funcNew;
                    /* expr.param */
                    params->val = p2;
                    P(" => "); prettyNode(n); P("\n");
                }
            }
        }
    }
    return ;
}

static void scanNode(Node *n){
    switch(n->kind){
        case       moduleK: scanModule    (n); break;
        case       methodK: scanMethod    (n); break;
        case   simpleStmtK: scanSimpleStmt(n); break;
        case     castStmtK: scanCastStmt  (n); break;
        case    paramExprK: scanParamExpr (n); break;
        case literalParamK: scanParam     (n); break;
    }
}

static void scanList(List *list){
    if(list){
        List *ptr = list;
        while(ptr){
            if(ptr->val) scanNode(ptr->val);
            ptr = ptr->next;
        }
    }
}


/* entry */
void analyzeSR(Prog *root){
    printBanner("Strength Reduction");
    scanList(root->module_list);
}

