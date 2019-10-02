#ifndef _H_NAME_TABLE_
#define _H_NAME_TABLE_

#define SymbolTableSize 317

typedef enum SymbolKind {
    moduleS, globalS, methodS, localS, builtinS
}SymbolKind;

typedef struct SymbolName{
    char *name;
    SymbolKind kind;
    union{
        Node *module;
        Node *global;
        Node *method;
        Node *local;
    }val;
    struct SymbolName *next;
    V value;
}SymbolName;

typedef struct SymbolTable{
    SymbolName *table[SymbolTableSize];
    struct SymbolTable *parent;
}SymbolTable;

typedef struct SymbolDecl{
    char *moduleName;
    SymbolTable *symTable;
    struct SymbolDecl *next;
}SymbolDecl;

typedef struct SymbolNameList{
    SymbolName *symName;
    struct SymbolNameList *next;
}SymbolNameList;

typedef struct MetaMethod{
    SymbolNameList *paramVars;
    SymbolNameList *localVars;
    struct InfoNodeList *returnTypes;
    struct ChainList *chains;   // with dummy
    bool isCompiling, isCalled;
    bool isCompiled;
}MetaMethod;

/* declarations */
SymbolName *getSymbolName(SymbolTable *st, char *name);
SymbolName *putSymbolName(SymbolTable *st, char *name, SymbolKind kind);

O buildSymbolTable(Prog *root); 
O printSymbolName(SymbolName *sn);
O printSymbolNameList(SymbolNameList *list);

S strName2 (S id1, S id2);
S strName  (Node *n);

#endif

