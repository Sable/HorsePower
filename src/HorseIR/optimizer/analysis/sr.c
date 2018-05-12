#include "../global.h"

/* signatures */
static void scanNode(Node *n);
static void scanList(List *list);
static void scanSimpleStmt(Node *n);
static void scanCastStmt(Node *n);
static Node *scanExpr(Node *n);

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
        switch(v->kind){
            case  literalBoolK: P("%d:b"  ,v->b  ); break;
            case   literalIntK: P("%d:i64",v->i64); break;
            case  literalDateK: P("%d:d"  ,v->d  ); break;
            case literalFloatK: P("%g:f64",v->f64); break;
            default: P("kind: %d not supported\n", v->kind);
        }
    }
    else if(v->len > 1){
        switch(v->kind){
            case  literalBoolK:
                {int *num=(int*)v->g; P("("); DOI(v->len, {if(i>0)P(",");P("%d",num[i]);}) P("):b");} break;
            case  literalDateK:
                {int *num=(int*)v->g; P("("); DOI(v->len, {if(i>0)P(",");P("%d",num[i]);}) P("):d");} break;
            case   literalIntK: 
                {int *num=(int*)v->g; P("("); DOI(v->len, {if(i>0)P(",");P("%d",num[i]);}) P("):i64");} break;
            case literalFloatK: 
                {double *num=(double*)v->g; P("("); DOI(v->len, {if(i>0)P(",");P("%g",num[i]);}) P("):f64");} break;
            default: P("kind: %d not supported\n", v->kind);
        }
    }
}

static ValueNode *fetchLiteralFloat(Node *n, Kind k){
    List *list; int c=0;
    for(list=n->val.listS;list!=NULL;list=list->next) c++;
    double *num = NEWL(double,c); c=0;
    for(list=n->val.listS;list!=NULL;list=list->next) {
        num[c++] = list->val->val.floatS;
    }
    ValueNode *v = NEW(ValueNode);
    v->len  = c;
    v->kind = k;
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
    v->len  = c;
    v->kind = k;
    if(c==1){
        switch(k){
            case literalBoolK: v->b   = num[0]; break;
            case  literalIntK: v->i64 = num[0]; break;
            case literalDateK: v->d   = num[0]; break;
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

static Node *scanExpr(Node *n){
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
                //if(singleInt(2))
            }
        }
    }
    return NULL;
}

static void scanNode(Node *n){
    switch(n->kind){
        case       moduleK: scanModule    (n); break;
        case       methodK: scanMethod    (n); break;
        case   simpleStmtK: scanSimpleStmt(n); break;
        case     castStmtK: scanCastStmt  (n); break;
        case         exprK: scanExpr      (n); break;
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
    scanList(root->module_list);
}

