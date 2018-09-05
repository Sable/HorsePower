#ifndef __H_TREE__
#define __H_TREE__

#include <stdbool.h>

typedef struct List {
    struct Node *val;
    struct List *next;
}List;

typedef struct Program {
    struct List *module_list;
}Prog;

/* TODO: add listT */
typedef enum pType {
    unknownT, boolT, i16T, i32T, i64T, f32T, f64T, charT, clexT, symT, strT,
    monthT, dateT, dtT, hourT, minuteT, timeT,
    tableT, ktableT, listT, enumT, dictT, funcT,
    totalT
}pType;

typedef enum Kind {
    idK, floatK, intK, typeK, compoundK, dateK, symK, strK,
    literalFloatK, literalSymK, literalDateK, literalCharK, literalStrK,
    literalBoolK, literalParamK, literalIntK, literalFuncK,
    funcK, exprK, paramExprK, nameTypeK,
    simpleStmtK, castStmtK, returnK, importK, methodK, moduleK
}Kind;

typedef struct Node {
    union {
        struct compoundID{char *name1,*name2;} compoundID;
        struct expression{struct Node *func,*param;} expr;
        struct simpleStmt{struct Node *name,*type,*expr;} simpleStmt;
        struct castStmt  {struct Node *name,*type,*expr,*cast;} castStmt;
        struct method    {struct Node *name,*type; List *list,*param;} method;
        struct module    {struct Node *name; List *body;} module;
        struct nameType  {struct Node *name,*type;} nameType;
        struct Node *nodeS;
        bool   boolS;
        int    intS, dateS, monthS;
        double floatS;
        char   *idS, *charS, *strS, *symS;
        pType  typeS;
        List   *listS;
    } val;
    Kind kind;
    int lineno;
}Node;


Prog *makeProg (List *module_list);
List *makeList (Node *val, List *next);

Node *makeNodeID           (char *id);
Node *makeNodeKind         (Node *d, Kind k);
Node *makeListKind         (List *t, Kind k);
Node *makeNodeModule       (Node *name, Node *content);
Node *makeNodeModuleContent(List *body);
Node *makeNodeModuleMethod (Node *name, List *param, Node *type, List *stmt);
Node *makeNodeImport       (Node *cID);
Node *makeNodeSimpleStmt1  (Node *name, Node *type, Node *expr);
Node *makeNodeSimpleStmt2  (Node *name, Node *type, Node *expr, Node *cast_type);
Node *makeNodeReturnStmt   (Node *rtn);
Node *makeNodeExpr         (Node *func, Node *paramExpr);
Node *makeNodeFunction     (Node *name);
Node *makeNodeParamLiteral (Node *paramValue);
Node *makeNodeLiteralBool  (List *int_list);
Node *makeNodeLiteralChar  (char *charValue);
Node *makeNodeLiteralString(List *string_list);
Node *makeNodeLiteralInt   (List *int_list, Node *type);
Node *makeNodeIntType      (pType typ);
Node *makeNodeLiteralFloat (List *float_list, Node *type);
Node *makeNodeFloatType    (pType typ);
Node *makeNodeLiteralSymbol(List *symbol_list);
Node *makeNodeLiteralDate  (List *date_list);
Node *makeNodeCompoundID1  (char *name1, char *name2);
Node *makeNodeCompoundID2  (char *name1);
Node *makeNodeType         (pType typ);
Node *makeNodeTypeWithName (char *type);
Node *makeNodeIntValue     (int value, char op);
Node *makeNodeFloatValue   (double value, char op);
Node *makeNodeStringValue  (char *str);
Node *makeNodeConstSymbol  (char *value);
Node *makeNodeConstDate    (int value);
Node *makeNodeParamExpr    (List *param_list);
Node *makeNodeNameType     (Node *name, Node *type);

#endif
