#include "../global.h"

extern int yylineno;
/*
 * name: a node
 * id  : a string
 * exp : expression (construct)
 * typ : expression (construct)
 *
 * expr: expression (arguments)
 * type: expression (arguments)
 */


Prog *makeProg(List *modules){
    Prog *p = NEW(Prog);
    p->module_list = modules;
    return p;
}

List *makeList(List *next, Node *val){
    List *t = NEW(List);
    t->val  = val;
    t->next = next;
    return t;
}

Node *makeNodeID(char *id){
    Node *n    = NEW(Node);
    n->kind    = idK;
    n->val.idS = id;
    n->lineno  = yylineno;
    return n;
}

Node *makeListKind(List *t, Kind k){
    Node *n     = NEW(Node);
    n->kind     = k;
    n->val.listS= t;
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeModule(char *id, List *body){
    Node *n     = NEW(Node);
    n->kind     = moduleK;
    n->val.module.id   = id;
    n->val.module.body = body;
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeImport(char *id, List *value){
    Node *n     = NEW(Node);
    n->kind     = importK;
    n->val.import.module  = id;
    n->val.import.content = value;
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeModuleMethod(char *kind, char *id, Node *param, List *type, Node *block){
    Node *n     = NEW(Node);
    n->kind     = methodK;
    n->val.method.kind  = kind;
    n->val.method.fname = id;
    n->val.method.typ   = type;
    n->val.method.param = param;
    n->val.method.block = block;
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeVar(char *id, Node *type){
    Node *n     = NEW(Node);
    n->kind     = varK;
    n->val.param.id  = id;
    n->val.param.typ = type;
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeGlobal(char *id, Node *type, Node *operand){
    Node *n     = NEW(Node);
    n->kind     = globalK;
    n->val.global.id  = id;
    n->val.global.typ = type;
    n->val.global.op  = operand;
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeType(bool wild, char *id, List *cellType){
    Node *n     = NEW(Node);
    n->kind     = typeK;
    n->val.type.isWild = wild;
    n->val.type.typ    = id;
    n->val.type.cell   = cellType;
    n->lineno   = yylineno;
    return n; 
}

// old: makeNodeSimpleStmt1
Node *makeNodeAssignment(List *vars, Node *expr){
    Node *n     = NEW(Node);
    n->kind     = stmtK;
    n->val.assignStmt.vars = vars;
    n->val.assignStmt.expr = expr;
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeCast(Node *expr, Node *type){
    Node *n     = NEW(Node);
    n->kind     = castK;
    n->val.cast.exp = expr;
    n->val.cast.typ = type;
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeName(char *id1, char *id2){
    Node *n     = NEW(Node);
    n->kind     = nameK;
    n->val.name.one = NULL==id1;
    n->val.name.id1 = id1;
    n->val.name.id2 = id2;
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeStmtIf(Node *cond, Node *then, Node *other){
    Node *n     = NEW(Node);
    n->kind     = ifK;
    n->val.ifStmt.condExpr  = cond;
    n->val.ifStmt.thenBlock = then;
    n->val.ifStmt.elseBlock = other;
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeStmtWhile(Node *cond, Node *body){
    Node *n     = NEW(Node);
    n->kind     = whileK;
    n->val.whileStmt.condExpr  = cond;
    n->val.whileStmt.bodyBlock = body;
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeStmtRepeat(Node *cond, Node *body){
    Node *n     = NEW(Node);
    n->kind     = repeatK;
    n->val.repeatStmt.condExpr  = cond;
    n->val.repeatStmt.bodyBlock = body;
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeStmtExpr(Node *expr){
    Node *n     = NEW(Node);
    n->kind     = exprstmtK;
    n->val.exprStmt.expr = expr;
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeStmtLabel(char *id, Node *stmt){
    Node *n     = NEW(Node);
    n->kind     = labelK;
    n->val.labelStmt.id   = id;
    n->val.labelStmt.stmt = stmt;
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeStmtGoto(char *targ, List *val, List *label){
    Node *n     = NEW(Node);
    n->kind     = gotoK;
    n->val.gotoStmt.targ  = targ; /* NULL -> targ; NOT NULL label */
    n->val.gotoStmt.val   = val;
    n->val.gotoStmt.label = label;
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeStmtBreak(char *id){
    Node *n     = NEW(Node);
    n->kind     = breakK;
    n->val.idS = id;  /* NULL or label */
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeStmtContinue(char *id){
    Node *n     = NEW(Node);
    n->kind     = continueK;
    n->val.idS = id;  /* NULL or label */
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeFuncCall(Node *func, Node *paramExpr){
    Node *n     = NEW(Node);
    n->kind     = callK;
    n->val.call.func  = func;
    n->val.call.param = paramExpr;
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeLiteralFunc(Node *func, Node *type){
    Node *n     = NEW(Node);
    n->kind     = funcK;
    n->val.func.name = func;
    n->val.func.typ  = type;
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeLiteralVector(bool isOne, List *value, Node *type){
    Node *n     = NEW(Node);
    n->kind     = vectorK;
    n->val.vec.one = isOne;
    n->val.vec.val = value;
    n->val.vec.typ = type;  // may be NULL when isOne is true
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeConst(ConstValue *val){
    Node *n     = NEW(Node);
    n->kind     = constK;
    n->val.nodeC= val;
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeParamExpr(List *params){
    return makeListKind(params, paramExprK);
}

Node *makeNodeBlock(List *stmts){
    return makeListKind(stmts, blockK);
}

Node *makeNodeStmtReturn(List *operands){
    return makeListKind(operands, returnK);
}

Node *makeNodeArgExpr(List *operands){
    return makeListKind(operands, argExprK);
}

/* constant values */

ConstValue *makeIntType(int x, Constant t){
    ConstValue *n = NEW(ConstValue);
    n->type       = t;
    n->valI       = x;
    return n;
}

ConstValue *makeLongType(long long x, Constant t){
    ConstValue *n = NEW(ConstValue);
    n->type       = t;
    n->valL       = x;
    return n;
}

ConstValue *makeStringType(char *x, Constant t){
    ConstValue *n = NEW(ConstValue);
    n->type       = t;
    n->valS       = x;
    return n;
}

ConstValue *makeFloat(double x){
    ConstValue *n = NEW(ConstValue);
    n->type       = floatC;
    n->valF       = x;
    return n;
}

static float getFloat(ConstValue *x){
    switch(x->type){
        case   intC: return x->valI;
        case floatC: return x->valF;
        case  longC: return x->valL;
        default: P("type not supported: %d\n", x->type); exit(1); 
    }
}

ConstValue *makeComplex(ConstValue *x, char op, ConstValue *y){
    ConstValue *n = NEW(ConstValue);
    n->type       = clexC;
    n->valX[0]    = getFloat(x);
    n->valX[1]    = op=='+'?getFloat(y):-getFloat(y);
    return n;
}

ConstValue *makeReal(char op, ConstValue *x){
    ConstValue *n = NEW(ConstValue);
    n->type       = x->type;
    switch(x->type){
        case   intC: n->valI = op=='-'?-(x->valI):x->valI; break;
        case floatC: n->valF = op=='-'?-(x->valF):x->valF; break;
        case  longC: n->valL = op=='-'?-(x->valL):x->valL; break;
        case  clexC: n->valX[0] = op=='-'?-(x->valX[0]):x->valX[0]; break;
        default: P("type not supported\n"); exit(1);
    }
    return n;
}


