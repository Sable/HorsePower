#include "../global.h"

static void scanStatement(Node *n, SymbolTable *st);
static void scanStatementList(List *list, SymbolTable *st);

SymbolTable *rootSymbolTable;
SymbolDecl *globalDecls;

/* declarations above */

char *strSymbolKind(SymbolKind x){
    switch(x){
        CaseLine(moduleS);
        CaseLine(globalS);
        CaseLine(methodS);
        CaseLine(localS);
        default: EP("Kind not supported: %d\n", x);
    }
}

char *strName2(char *id1, char *id2){
    int size = strlen(id1) + strlen(id2) + 2;
    char *name2 = NEWL(char, size);
    return strcat(strcat(strcpy(name2, id1), "."), id2);
}

// "id1.id2"
char *strName(Node *n){
    if(n->val.name.one) return n->val.name.id2;
    else {
        char *id1 = n->val.name.id1;
        char *id2 = n->val.name.id2;
        return strName2(id1, id2);
    }
}

void printSymbolName(SymbolName *sn){
    P("symbol: %s, kind: %s\n", sn->name, strSymbolKind(sn->kind));
    switch(sn->kind){
        case moduleS: printNode(sn->val.module); break;
        case methodS: printNode(sn->val.method); break;
        case globalS: printNode(sn->val.global); break;
        case  localS: printNode(sn->val.local); break;
    }
    P("\n");
}

static SymbolTable *initSymbolTable(){
    SymbolTable *t = (SymbolTable*)malloc(sizeof(SymbolTable));
    for(int i=0; i<SymbolTableSize; i++)
        t->table[i] = NULL;
    t->parent = NULL;
    return t;
}

//SymbolTables *initChildTable(Symboltable t){
//    SymbolTables *s = NEW(SymbolTables);
//    s->child = t;
//    return s;
//}

static SymbolTable *scopeSymbolTable(Node *n, SymbolTable *st){
    SymbolTable *t = initSymbolTable();
    t->parent = st;
    /* insert */
    //SymbolTables *s = initChildTable(t);
    //s->next = st->children;
    //st->children = s;
    return t;
}

static int simpleHash(char *str){
    int hash = 0;
    while (*str) hash = (hash << 1) + *str++;
    return hash % SymbolTableSize;
}

static SymbolName *putSymbol(SymbolTable *st, char *name, SymbolKind kind){
    int i = simpleHash(name);
    P("put string: %s ==> %d ==> %lld\n", name,i,(long long)(st->table[i]));
    for(SymbolName *s = st->table[i]; s; s = s->next){
        if(!strcmp(s->name, name)) EP("Name existed: %s\n",name);
    }
    SymbolName *s = (SymbolName*)malloc(sizeof(SymbolName));
    s->name  = name;
    s->kind  = kind;
    s->next  = st->table[i];
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

// 1st pass

static void scanDeclaration(Node *n, SymbolTable *st);
static void scanDeclarationList(List *list, SymbolTable *st);

static void addDecls(SymbolTable *table, char *name){
    SymbolDecl *x = NEW(SymbolDecl);
    x->moduleName = name;
    x->symTable = table;
    x->next = globalDecls->next;
    globalDecls->next = x;
}

static SymbolDecl *findDecls(char *name){
    SymbolDecl *x = globalDecls->next;
    while(x){
        if(!strcmp(x->moduleName, name)) return x;
        x = x->next;
    }
    return NULL;
}

static SymbolTable *copySymbolTable(SymbolTable *st){
    SymbolTable *x = NEW(SymbolTable);
    memcpy(x, st, sizeof(SymbolTable));
    x->parent = NULL;
    return x;
}

// import moduleName.{subNames}
static SymbolTable *getDecls(char *moduleName, char **subNames, int size){
    SymbolDecl *x = findDecls(moduleName);
    if(x){
        if(size == 0){ // load all
            return copySymbolTable(x->symTable);
        }
        else if(size > 0){
            SymbolTable *st = NEW(SymbolTable);
            for(int i=0; i<size; i++){
                SymbolName *sn = getSymbol(x->symTable, subNames[i]);
                if(sn->kind == methodS){
                    SymbolName *n = putSymbol(st, subNames[i], methodS);
                    n->val.method = sn->val.method;
                }
                else if(sn->kind == globalS){
                    SymbolName *n  = putSymbol(st, subNames[i], globalS);
                    n->val.global = sn->val.global;
                }
            }
            return st;
        }
        else EP("size must >=0: %d\n",size);
    }
    else EP("module %s not found\n",moduleName);
}

static void scanModuleDecl(Node *n, SymbolTable *st){
    SymbolTable *t = scopeSymbolTable(n, st);
    scanDeclarationList(n->val.module.body, t);
    addDecls(t, n->val.module.id);
}

static void scanMethodDecl(Node *n, SymbolTable *st){
    SymbolName *s = putSymbol(st, n->val.method.fname, methodS);
    s->val.method = n;
}

static void scanGlobalDecl(Node *n, SymbolTable *st){
    SymbolName *s = putSymbol(st, n->val.global.id, globalS);
    s->val.global = n;
}

static void scanDeclaration(Node *n, SymbolTable *st){
    if(!n) R;
    switch(n->kind){
        case moduleK: scanModuleDecl(n, st); break;
        case methodK: scanMethodDecl(n, st); break;
        case globalK: scanGlobalDecl(n, st); break;
    }
}

static void scanDeclarationList(List *list, SymbolTable *st){
    if(list){
        scanDeclarationList(list->next, st);
        scanDeclaration(list->val, st);
    }
}

// 2nd pass: scan

static void scanVar(Node *n, SymbolTable *st){
    P("var = %s\n", n->val.param.id);
    SymbolName *s = putSymbol(st, n->val.param.id, localS);
    s->val.local = n;
}

static void scanName(Node *n, SymbolTable *st){
    char *name = strName(n);
    SymbolName *s = getSymbol(st, name);
    if(s){ // local/global
        printSymbolName(s); 
        getchar();
    }
    else EP("Name %s needs to be declared before used\n", name);
}

static void scanAssignStmt(Node *n, SymbolTable *st){
    scanStatementList(n->val.assignStmt.vars, st);
    scanStatement(n->val.assignStmt.expr, st);
}

static void scanCast(Node *n, SymbolTable *st){
    scanStatement(n->val.cast.exp, st);
}

static void scanBlockStmt(Node *n, SymbolTable *st){
    SymbolTable *new_st = scopeSymbolTable(n, st);
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

static void scanReturnStmt(Node *n, SymbolTable *st){
    scanStatementList(n->val.listS, st);
}

static void scanMethod(Node *n, SymbolTable *st){
    //SymbolName *s = putSymbol(st, n->val.method.fname, methodS);
    //s->val.method = n;
    SymbolTable *t = scopeSymbolTable(n, st);
    scanStatement(n->val.method.param, t);
    scanStatement(n->val.method.block, t);
}

static void scanGlobal(Node *n, SymbolTable *st){
    //SymbolName *s = putSymbol(st, n->val.global.id, globalS);
    //s->val.global = n;
    scanStatement(n->val.global.op, st);
}

static void scanModule(Node *n, SymbolTable *st){
    //SymbolName *s = putSymbol(st, n->val.module.id, moduleS);
    //s->val.module = n;
    //SymbolTable *t = scopeSymbolTable(n, st);
    SymbolTable *t = getDecls(n->val.module.id, NULL, 0);
    t->parent = st;
    SymbolTable *t2 = scopeSymbolTable(n, t);
    scanStatementList(n->val.module.body, t2);
}

static void scanParams(Node *n, SymbolTable *st){
    scanStatementList(n->val.listS, st);
}

static void scanId(Node *n, SymbolTable *st){
    printNode(n); EP("Nothing for ID\n");
}

static void scanExprStmt(Node *n, SymbolTable *st){
    scanStatement(n->val.exprStmt.expr, st);
}

static void scanCall(Node *n, SymbolTable *st){
    Node *func = n->val.call.func;
    char *moduleName = func->val.name.id1;
    char *fieldName  = func->val.name.id2;
    SymbolName *sn = NULL;
    printNode(n);
    if(func->val.name.one){
        sn = getSymbol(st, fieldName);
    }
    else {
        SymbolDecl *sd = findDecls(moduleName);
        sn = getSymbol(sd->symTable, fieldName);
    }
    if(sn){
        printSymbolName(sn);
        getchar();
    }
    else EP("Function not defined: %s\n", strName(func));
    scanStatement(n->val.call.param, st);
}

static void scanArgExpr(Node *n, SymbolTable *st){
    scanStatementList(n->val.listS, st);
}

static void scanImport(Node *n, SymbolTable *st){
    //SymbolName *s = putSymbol(st, n->val.import.module, otherS);
    //s->val.others = n;
    SymbolTable *t = getDecls(n->val.import.module, NULL, 0); // load all
    t->parent = st->parent->parent;
    st->parent->parent = t;
}

#define scanContinueStmt(n,s) TODO("continue")
#define scanBreakStmt(n,s)    TODO("break")
#define scanWhileStmt(n,s)    TODO("while")
#define scanRepeatStmt(n,s)   TODO("repeat")

static void scanStatement(Node *n, SymbolTable *st){
    if(!n) R;
    switch(n->kind){
        case    moduleK: scanModule      (n, st); break;
        case    methodK: scanMethod      (n, st); break;
        case      stmtK: scanAssignStmt  (n, st); break;
        case      castK: scanCast        (n, st); break;
        case  exprstmtK: scanExprStmt    (n, st); break;
        case paramExprK: scanParams      (n, st); break;
        case      callK: scanCall        (n, st); break;
        case      nameK: scanName        (n, st); break;
        case   argExprK: scanArgExpr     (n, st); break;
        case    returnK: scanReturnStmt  (n, st); break;
        case  continueK: scanContinueStmt(n, st); break;
        case     breakK: scanBreakStmt   (n, st); break;
        case        ifK: scanIfStmt      (n, st); break;
        case     whileK: scanWhileStmt   (n, st); break;
        case    repeatK: scanRepeatStmt  (n, st); break;
        case        idK: scanId          (n, st); break;
        case       varK: scanVar         (n, st); break;
        case    globalK: scanGlobal      (n, st); break;
        case      typeK: break; // ignore?
        case     blockK: scanBlockStmt   (n, st); break;
        case    importK: scanImport      (n, st); break;
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
    // 1st pass
    globalDecls = NEW(SymbolDecl);
    scanDeclarationList(root->module_list, NULL);
    // 2nd pass
    rootSymbolTable = initSymbolTable();
    scanStatementList(root->module_list, rootSymbolTable);
}

