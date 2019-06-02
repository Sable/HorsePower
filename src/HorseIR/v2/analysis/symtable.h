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
}MetaMethod;

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
void buildSymbolTable(Prog *root);
extern SymbolTable *rootSymbolTable;

char *strName2 (char *id1, char *id2);
char *strName  (Node *n);

SymbolName *getSymbolName(SymbolTable *st, char *name);
SymbolName *putSymbolName(SymbolTable *st, char *name, SymbolKind kind);

#endif

