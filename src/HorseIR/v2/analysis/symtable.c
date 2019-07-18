#include "../global.h"

static void scanStatement(Node *n, SymbolTable *st);
static void scanStatementList(List *list, SymbolTable *st);
static InfoNode *getInfoNode(Node *n);
static void scanName(Node *n, SymbolTable *st);

SymbolTable *rootSymbolTable;
SymbolDecl *globalDecls;

/* declarations above */

char *strSymbolKind(SymbolKind x){
    switch(x){
        CaseLine(moduleS);
        CaseLine(globalS);
        CaseLine(methodS);
        CaseLine(localS);
        CaseLine(builtinS);
        default: EP("Kind not supported: %d\n", x);
    }
}

char *strName2(char *id1, char *id2){
    int size = strlen(id1) + strlen(id2) + 2;
    char *name2 = NEWL(char, size);
    return strcat(strcat(strcpy(name2, id1), "."), id2);
}

// types 
static void addCellInfo(InfoNode *in, InfoNode *x){
    x->next = in->next;
    in->next = x;
}

static void addSubInfo(InfoNode *in, InfoNode *x){
    x->next = in->subInfo;
    in->subInfo= x;
}

static void getSubInfo(Node *n, InfoNode *in){
    InfoNode *x = getInfoNode(n);
    addSubInfo(in, x);
}

static void getCellInfo(List *list, InfoNode *in){
    if(list){
        getCellInfo(list->next, in);
        InfoNode *sub = NEW(InfoNode);
        getSubInfo(list->val, sub);
        addCellInfo(in, sub);
    }
}

static ShapeType getShapeByName(S name){
    if(!strcmp(name, "list")) return listH;
    else if(!strcmp(name, "table")) return tableH;
    else if(!strcmp(name, "dict"))  return dictH;
    else if(!strcmp(name, "enum"))  return enumH;
    else EP("Unknown shape name: %s", name);
}

static InfoNode *getInfoNode(Node *n){
    InfoNode *x = NEW(InfoNode);
    if(n->val.type.cell){
        getCellInfo(n->val.type.cell, x);
        //x->shape = newShapeNode(getShapeByName(n->val.type.typ), SN_ID, -1);
    }
    else {
        x->type = getType(n);
        //x->shape = newShapeNode(vectorH, SN_ID, -1);
    }
    return x;
}

SymbolNameList *symList;

static void cleanSymbolNameList(SymbolNameList *list){
    //if(list && list->next){ cleanSymbolNameList(list->next); free(list->next); list->next = NULL; }
    list->next = NULL;
}

static void addSymbolNameList(SymbolNameList *list, SymbolName *sn){
    //P("added %s\n", sn->name);
    SymbolNameList *x = NEW(SymbolNameList);
    x->symName = sn;
    x->next = list->next;
    list->next = x;
}

InfoNodeList *rtnList; // return type list

static void cleanReturnTypeList(InfoNodeList *list){
    list->next = NULL;
}

static void addReturnTypeList(InfoNodeList *list, InfoNode *in){
    InfoNodeList *x = NEW(InfoNodeList);
    x->in = in;
    x->next = list->next;
    list->next = x;
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

void printSymbolNameList(SymbolNameList *list){
    if(list){
        printSymbolNameList(list->next);
        printSymbolName(list->symName);
    }
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

SymbolName *putSymbolName(SymbolTable *st, char *name, SymbolKind kind){
    int i = simpleHash(name);
    //if(kind != builtinS)
    //  P("put string: %s ==> %d ==> %lld\n", name,i,(long long)(st->table[i]));
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

SymbolName *getSymbolName(SymbolTable *st, char *name){
    int i = simpleHash(name);
    for(SymbolName *s = st->table[i]; s; s = s->next){
        if(!strcmp(s->name, name)) return s;
    }
    if(st->parent == NULL) return NULL;
    return getSymbolName(st->parent, name);
}

static bool defSymbol(SymbolTable *st, char *name){
    int i = simpleHash(name);
    for(SymbolName *s = st->table[i]; s; s = s->next){
        if(!strcmp(s->name, name)) return 1;
    }
    return 0;
}

static void checkFuncName(Node *funcName, SymbolTable *st){
    scanName(funcName, st);
    SymbolName *sn = funcName->val.name.sn;
    switch(sn->kind){
        case builtinS:
        case  methodS: break; // safe
        default: EP("Function literal only supports methods/builtins");
    }
}

static void checkFuncList(List *list, SymbolTable *st){
    if(list){
        checkFuncList(list->next, st);
        checkFuncName(list->val, st);
    }
}

// 1st pass

static void scanDeclaration(Node *n, SymbolTable *st);
static void scanDeclarationList(List *list, SymbolTable *st);

static void addDecls(SymbolTable *table, char *name){
    SymbolDecl *x = NEW(SymbolDecl);
    x->moduleName = name;
    x->symTable   = table;
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
                SymbolName *sn = getSymbolName(x->symTable, subNames[i]);
                if(sn->kind == methodS){
                    SymbolName *n = putSymbolName(st, subNames[i], methodS);
                    n->val.method = sn->val.method;
                }
                else if(sn->kind == globalS){
                    SymbolName *n  = putSymbolName(st, subNames[i], globalS);
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
    SymbolName *s = putSymbolName(st, n->val.method.fname, methodS);
    s->val.method = n;
}

static void scanGlobalDecl(Node *n, SymbolTable *st){
    SymbolName *s = putSymbolName(st, n->val.global.id, globalS);
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
    //P("var = %s\n", n->val.param.id);
    SymbolName *s = putSymbolName(st, n->val.param.id, localS);
    Node *typ = n->val.param.typ;
    s->val.local = n;
    scanStatement(typ, st);
    addSymbolNameList(symList, n->val.param.sn = s); // assign symbolname and add it to a list
}

static void scanVarDecl(Node *n, SymbolTable *st){
    scanStatementList(n->val.listS, st);
}

static void scanName(Node *n, SymbolTable *st){
    char *moduleName = n->val.name.id1;
    char *fieldName  = n->val.name.id2;
    SymbolName *sn = NULL;
    //printNode(n);
    if(n->val.name.one){
        if(!n->val.name.isUS){
            sn = getSymbolName(st, fieldName);
            n->val.name.st = st;
            n->val.name.sn = sn;
        }
        else return ; // if _, skip check
    }
    else {
        SymbolDecl *sd = findDecls(moduleName);
        sn = getSymbolName(sd->symTable, fieldName);
        n->val.name.st = sd->symTable;
        n->val.name.sn = sn;
    }
    if(sn){
        //printSymbolName(sn); getchar();
    }
    else EP("Name %s needs to be declared before used\n", strName(n));
}

static void scanFunc(Node *n, SymbolTable *st){
    checkFuncList(n->val.listS, st);
}

static void scanType(Node *n, SymbolTable *st){
    n->val.type.in = getInfoNode(n);
    //printInfoNode(n->val.type.in); getchar();
    addReturnTypeList(rtnList, n->val.type.in);
}

static void scanAssignStmt(Node *n, SymbolTable *st){
    scanStatementList(n->val.assignStmt.vars, st);
    scanStatement(n->val.assignStmt.expr, st);
}

static void scanCast(Node *n, SymbolTable *st){
    scanStatement(n->val.cast.exp, st);
    scanStatement(n->val.cast.typ, st);
}

static void scanBlockStmt(Node *n, SymbolTable *st){
    SymbolTable *new_st = scopeSymbolTable(n, st);
    scanStatementList(n->val.listS, new_st);
    n->val.block.st = new_st;
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

//static void printSymList(){
//    SymbolNameList *p = symList->next;
//    while(p){P("- %s\n", p->symName->name); p=p->next;}
//}

static void setReturnShape(InfoNodeList *list){
    if(list) {
        setReturnShape(list->next);
        list->in->shape = newShapeNode(unknownH, -1, -1);
    }
}

static void scanMethod(Node *n, SymbolTable *st){
    //SymbolName *s = putSymbol(st, n->val.method.fname, methodS);
    //s->val.method = n;
    MetaMethod *meta = NEW(MetaMethod);
    SymbolTable *t = scopeSymbolTable(n, st);
    scanStatementList(n->val.method.typ, t); // types

    cleanSymbolNameList(symList);
    scanStatement(n->val.method.param, t);   // parameters
    meta->paramVars = symList->next; // no dummy

    cleanSymbolNameList(symList);
    scanStatement(n->val.method.block, t);   // statements
    meta->localVars = symList->next; // no dummy

    cleanReturnTypeList(rtnList);
    scanStatementList(n->val.method.typ, t);
    meta->returnTypes = rtnList->next; // no dummy
    setReturnShape(meta->returnTypes); // set to unknown

    // collect all meta info
    n->val.method.meta = meta;
}

static void scanGlobal(Node *n, SymbolTable *st){
    //SymbolName *s = putSymbol(st, n->val.global.id, globalS);
    //s->val.global = n;
    scanStatement(n->val.global.typ, st);
    scanStatement(n->val.global.op , st);
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
    scanStatement(n->val.call.func, st);
    scanStatement(n->val.call.param, st);
}

static void scanArgExpr(Node *n, SymbolTable *st){
    scanStatementList(n->val.listS, st);
}

/* TODO: import partial */
static void scanImport(Node *n, SymbolTable *st){
    //SymbolName *s = putSymbol(st, n->val.import.module, otherS);
    //s->val.others = n;
    SymbolTable *t = getDecls(n->val.import.module, NULL, 0); // load all
    t->parent = st->parent->parent;
    st->parent->parent = t;
}

static void scanVector(Node *n, SymbolTable *st){
    scanStatement(n->val.vec.typ, st);
}

//#define scanContinueStmt(n,s) TODO("continue")
//#define scanBreakStmt(n,s)    TODO("break")

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
        case      funcK: scanFunc        (n, st); break;
        case   argExprK: scanArgExpr     (n, st); break;
        case    returnK: scanReturnStmt  (n, st); break;
        case        ifK: scanIfStmt      (n, st); break;
        case     whileK: scanWhileStmt   (n, st); break;
        case    repeatK: scanRepeatStmt  (n, st); break;
        case        idK: scanId          (n, st); break;
        case       varK: scanVar         (n, st); break;
        case   varDeclK: scanVarDecl     (n, st); break;
        case    globalK: scanGlobal      (n, st); break;
        case      typeK: scanType        (n, st); break;
        case     blockK: scanBlockStmt   (n, st); break;
        case    importK: scanImport      (n, st); break;
        case    vectorK: scanVector      (n, st); break;
        //case  continueK: scanContinueStmt(n, st); break;
        //case     breakK: scanBreakStmt   (n, st); break;
    }
}

static void scanStatementList(List *list, SymbolTable *st){
    if(list){
        scanStatementList(list->next, st);
        scanStatement(list->val, st);
    }
}

extern char *FunctionUnaryStr[];  /* defined in typerule.c */
extern char *FunctionBinaryStr[];
extern char *FunctionOtherStr[];

extern int  UnarySize;
extern int BinarySize;
extern int  OtherSize;

static void addDefaultBuiltin(){
    SymbolTable *st = NEW(SymbolTable);
    DOI(UnarySize , putSymbolName(st,  FunctionUnaryStr[i], builtinS));
    DOI(BinarySize, putSymbolName(st, FunctionBinaryStr[i], builtinS));
    DOI(OtherSize , putSymbolName(st,  FunctionOtherStr[i], builtinS));
    addDecls(st, "Builtin");
}

static void addDefaultSystem(){
    // TODO: add sth...
}

static void init(){
    // 1st pass
    globalDecls = NEW(SymbolDecl);
    symList     = NEW(SymbolNameList);
    rtnList     = NEW(InfoNodeList);
    // 2nd pass
    rootSymbolTable = initSymbolTable();
}

void buildSymbolTable(Prog *root){
    printBanner("Symbol Table");
    init();
    // 1st pass
    scanDeclarationList(root->module_list, NULL);
    addDefaultBuiltin();
    addDefaultSystem();
    // 2nd pass
    scanStatementList(root->module_list, rootSymbolTable);
}

