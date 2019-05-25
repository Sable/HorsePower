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

//typedef struct InfoNode{
//    Type type;
//    struct ShapeNode *shape;
//    struct InfoNode *subInfo;
//    struct InfoNode *next;
//}InfoNode;

//typedef struct TypeNode{
//    Type type;
//    struct ShapeNode *shape;
//    struct TypeNode *subType;
//    struct TypeNode *next;
//}TypeNode;
//
//typedef struct InfoTypeList{
//    TypeNode *tn;
//    struct InfoTypeList *next;
//}InfoTypeList;

//typedef struct InfoNodeList{
//    InfoNode *in;
//    struct InfoNodeList *next;
//}InfoNodeList;

/* declarations */
void createSymbolTable(Prog *root);
extern SymbolTable *rootSymbolTable;

char *strName2 (char *id1, char *id2);
char *strName  (Node *n);

SymbolName *getSymbol(SymbolTable *st, char *name);
SymbolName *putSymbol(SymbolTable *st, char *name, SymbolKind kind);

#endif

