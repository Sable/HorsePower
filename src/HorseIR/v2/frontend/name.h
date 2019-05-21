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
    struct SymbolTable  *parent;
}SymbolTable;

/* declarations */
void createSymbolTable(Prog *root);
extern SymbolTable *rootSymbolTable;

char *strName2 (char *id1, char *id2);
char *strName  (Node *n);

#endif

