#ifndef _H_NAME_TABLE_
#define _H_NAME_TABLE_

#define SymbolTableSize 317

typedef enum SymbolKind {
    moduleS, globalS, methodS, localS, otherS
}SymbolKind;

typedef struct SymbolName{
    char *name;
    SymbolKind kind;
    union{
        Node *module;
        Node *global;
        Node *method;
        Node *locals;
        Node *others;
    }val;
    struct SymbolName *next;
}SymbolName;

typedef struct SymbolTable{
    SymbolName *table[SymbolTableSize];
    struct SymbolTable *parent;
}SymbolTable;

/* declarations */
SymbolTable *rootSymbolTable;
void createSymbolTable(Prog *root);

#endif

