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

static void printValueNode(ValueNode *v){
    if(v->len == 1){
        switch(v->typ){
            case  boolT: P("%d:b"  ,v->b  ); break;
            case   i64T: P("%d:i64",v->i64); break;
            case  dateT: P("%d:d"  ,v->d  ); break;
            case   f64T: P("%g:f64",v->f64); break;
            default: P("type: %d not supported\n", v->typ);
        }
    }
    else if(v->len > 1){
        switch(v->typ){
            case boolT:
                {int *num=(int*)v->g; P("("); DOI(v->len, {if(i>0)P(",");P("%d",num[i]);}) P("):b");} break;
            case dateT: 
                {int *num=(int*)v->g; P("("); DOI(v->len, {if(i>0)P(",");P("%d",num[i]);}) P("):d");} break;
            case i64T: 
                {int *num=(int*)v->g; P("("); DOI(v->len, {if(i>0)P(",");P("%d",num[i]);}) P("):i64");} break;
            case f64T: 
                {double *num=(double*)v->g; P("("); DOI(v->len, {if(i>0)P(",");P("%g",num[i]);}) P("):f64");} break;
            default: P("kind: %d not supported\n", v->typ);
        }
    }
}

static pType kind2type(Kind k){
    switch(k){
        case   literalIntK: return  i64T;
        case  literalDateK: return dateT;
        case   literalSymK: return  symT;
        case  literalBoolK: return boolT;
        case literalFloatK: return  f64T;
        default: EP("kind to type fail : k = %d\n", k);
    }
}

static ValueNode *fetchLiteralFloat(Node *n, Kind k){
    List *list; int c=0;
    for(list=n->val.listS;list!=NULL;list=list->next) c++;
    double *num = NEWL(double,c); c=0;
    for(list=n->val.listS;list!=NULL;list=list->next) {
        if(list->val->kind == intK)
            num[c++] = list->val->val.intS;
        else
            num[c++] = list->val->val.floatS;
    }
    ValueNode *v = NEW(ValueNode);
    v->len = c;
    v->typ = kind2type(k);
    if(c==1){
        switch(k){
            case literalFloatK: v->f64 = num[0]; break;
        }
        free(num);
    }
    else if(c>0){
        v->g = (char*)num;
    }
    return v;
}
static ValueNode *fetchLiteralInt(Node *n, Kind k){
    List *list; int c=0;
    for(list=n->val.listS;list!=NULL;list=list->next) c++;
    int *num = NEWL(int,c); c=0;
    for(list=n->val.listS;list!=NULL;list=list->next) {
        num[c++] = list->val->val.intS;
    }
    ValueNode *v = NEW(ValueNode);
    v->len = c;
    v->typ = kind2type(k);
    if(c==1){
        switch(k){
            case  literalBoolK: v->b   = num[0]; break;
            case   literalIntK: v->i64 = num[0]; break;
            case  literalDateK: v->d   = num[0]; break;
        }
        free(num);
    }
    else if(c>0){
        v->g = (char*)num;
    }
    return v;
}

static ValueNode *scanLiteral(Node *n){
    Node *paramValue = n->val.nodeS;
    switch(paramValue->kind){
        case  literalBoolK: return fetchLiteralInt  (paramValue, literalBoolK );
        case   literalIntK: return fetchLiteralInt  (paramValue, literalIntK  );
        case  literalDateK: return fetchLiteralInt  (paramValue, literalDateK );
        case literalFloatK: return fetchLiteralFloat(paramValue, literalFloatK);
    }
    return NULL;
}

static bool isSingleInt(ValueNode *vn, int c, Node *var){
    if(vn->typ == i64T && vn->len == 1){
        return vn->i64 == c;
    }
    else if(vn->typ == f64T && vn->len == 1){
        InfoNode *in = getInfoNode(fetchName(var->val.nodeS));
        return (vn->f64 == c) && (in->type == f64T);
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
            ValueNode *v1 = scanLiteral(p1);
            //if(v1 != NULL) printValueNode(v1);
            ValueNode *v2 = scanLiteral(p2);
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

