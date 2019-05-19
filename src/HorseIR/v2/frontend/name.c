#include "../global.h"

static void scanStatement(Node *n, SymbolTable *st);
static void scanStatementList(List *list, SymbolTable *st);

/* declaration above */

static SymbolTable *initSymbolTable(){
    SymbolTable *t = (SymbolTable*)malloc(sizeof(SymbolTable));
    for(int i=0; i<SymbolTableSize; i++)
        t->table[i] = NULL;
    t->parent = NULL;
    return t;
}

static SymbolTable *scopeSymbolTable(SymbolTable *st){
    SymbolTable *t = initSymbolTable();
    t->parent = st;
    return t;
}

static int simpleHash(char *str){
    int hash = 0;
    while (*str) hash = (hash << 1) + *str++;
    return hash % SymbolTableSize;
}

static SymbolName *putSymbol(SymbolTable *st, char *name, SymbolKind kind){
    int i = simpleHash(name);
    //P("put string: %s ==> %d ==> %lld\n", name,i,(long long)(st->table[i]));
    for(SymbolName *s = st->table[i]; s; s = s->next){
        if(!strcmp(s->name, name)) EP("Name existed: %s\n",name);
    }
    SymbolName *s = (SymbolName*)malloc(sizeof(SymbolName));
    s->name = name;
    s->kind = kind;
    s->next = st->table[i];
    st->table[i] = s;
    return s;
}

static SymbolName *getSymbol(SymbolTable *st, char *name){
    int i = simpleHash(name);
    for(SymbolName *s = st->table[i]; s; s = s->next){
        if(!strcmp(s->name, name)) return s;
    }
    if(st->parent == NULL) return NULL;
    return getSymbol(st->parent, name);
}

static bool defSymbol(SymbolTable *st, char *name){
    int i = simpleHash(name);
    for(SymbolName *s = st->table[i]; s; s = s->next){
        if(!strcmp(s->name, name)) return 1;
    }
    return 0;
}

// scan

static void scanVars(Node *n, SymbolTable *st){
    //P("var = %s\n", n->val.param.id);
    SymbolName *s = putSymbol(st, n->val.param.id, localS);
    s->val.locals = n;
}

static void scanAssignStmt(Node *n, SymbolTable *st){
    scanStatementList(n->val.assignStmt.vars, st);
    scanStatement(n->val.assignStmt.expr, st);
}

static void scanBlockStmt(Node *n, SymbolTable *st){
    SymbolTable *new_st = scopeSymbolTable(st);
    scanStatementList(n->val.listS, new_st);
}

static void scanIfStmt(Node *n, SymbolTable *st){
    scanStatement(n->val.ifStmt.condExpr , st);
    scanStatement(n->val.ifStmt.thenBlock, st);
    scanStatement(n->val.ifStmt.elseBlock, st);
}

static void scanWhileStmt(Node *n, SymbolTable *st){
    scanStatement(n->val.whileStmt.condExpr , st);
    scanStatement(n->val.whileStmt.bodyBlock, st);
}

static void scanRepeatStmt(Node *n, SymbolTable *st){
    scanStatement(n->val.repeatStmt.condExpr , st);
    scanStatement(n->val.repeatStmt.bodyBlock, st);
}

static void scanLabelStmt(Node *n, SymbolTable *st){
    scanStatement(n->val.labelStmt.stmt, st);
}

static void scanReturnStmt(Node *n, SymbolTable *st){
    scanStatementList(n->val.listS, st);
}

static void scanMethod(Node *n, SymbolTable *st){
    SymbolName *s = putSymbol(st, n->val.method.fname, methodS);
    s->val.method = n;
    SymbolTable *t = scopeSymbolTable(st);
    scanStatement(n->val.method.param, t);
    scanStatement(n->val.method.block, t);
}

static void scanGlobal(Node *n, SymbolTable *st){
    SymbolName *s = putSymbol(st, n->val.global.id, globalS);
    s->val.global = n;
    scanStatement(n->val.global.op, st);
}

static void scanModule(Node *n, SymbolTable *st){
    SymbolName *s = putSymbol(st, n->val.module.id, moduleS);
    s->val.module = n;
    scanStatementList(n->val.module.body, st);
}

static void scanParams(Node *n, SymbolTable *st){
    scanStatementList(n->val.listS, st);
}

static void scanId(Node *n, SymbolTable *st){
    char *name = n->val.idS;
    SymbolName *s = getSymbol(st, name);
    if(s){
    }
    else EP("Name %s needs to be declared before used\n", name);
}

static void scanExprStmt(Node *n, SymbolTable *st){
    scanStatement(n->val.exprStmt.expr, st);
}

static void scanCall(Node *n, SymbolTable *st){
    scanStatement(n->val.call.param, st);
}

static void scanArgExpr(Node *n, SymbolTable *st){
    scanStatementList(n->val.listS, st);
}

static void scanImport(Node *n, SymbolTable *st){
    SymbolName *s = putSymbol(st, n->val.import.module, otherS);
    s->val.others = n;
}

static void scanStatement(Node *n, SymbolTable *st){
    if(!n) R;
    switch(n->kind){
        case       varK: scanVars      (n, st); break;
        case      stmtK: scanAssignStmt(n, st); break;
        case     blockK: scanBlockStmt (n, st); break;
        case        ifK: scanIfStmt    (n, st); break;
        case     whileK: scanWhileStmt (n, st); break;
        case    repeatK: scanRepeatStmt(n, st); break;
        case     labelK: scanLabelStmt (n, st); break;
        case    returnK: scanReturnStmt(n, st); break;
        case    methodK: scanMethod    (n, st); break;
        case    globalK: scanGlobal    (n, st); break;
        case    moduleK: scanModule    (n, st); break;
        case paramExprK: scanParams    (n, st); break;
        case        idK: scanId        (n, st); break;
        case  compoundK: break; // ignore?
        case  exprstmtK: scanExprStmt  (n, st); break;
        case      callK: scanCall      (n, st); break;
        case   argExprK: scanArgExpr   (n, st); break;
        case    importK: scanImport    (n, st); break;
    }
}

static void scanStatementList(List *list, SymbolTable *st){
    if(list){
        scanStatementList(list->next, st);
        scanStatement(list->val, st);
    }
}

void createSymbolTable(Prog *root){
    printBanner("Symbol Table");
    rootSymbolTable = initSymbolTable();
    scanStatementList(root->module_list, rootSymbolTable);
}

