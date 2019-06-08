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

typedef enum Kind {
    idK, varK, globalK, typeK, nameK, funcK, argExprK, paramExprK, blockK,
    stmtK, castK, varDeclK, exprstmtK, importK, methodK, moduleK, ifK, whileK,
    repeatK, returnK, breakK, continueK, callK, vectorK, constK,
    totalK
}Kind;

typedef struct Node {
    union {
        struct module     {char *id; List *body;                        } module;
        struct import     {char *module; List *content;                 } import;
        struct method     {char *kind,*fname; struct Node *param,*block; \
                           List *typ; struct MetaMethod *meta;          } method;
        struct param      {char *id; struct Node *typ; \
                           struct SymbolName *sn;                       } param;   /* change to var? */
        struct global     {char *id; struct Node *typ,*op;              } global;
        struct type       {bool isWild; char *typ; List *cell; \
                           struct InfoNode *in;                         } type;
        struct cast       {struct Node *exp,*typ;                       } cast;  /* exp -> expr ? */
        struct assignStmt {struct Node *expr; List *vars;               } assignStmt;
        struct exprStmt   {struct Node *expr;                           } exprStmt;
        struct ifStmt     {struct Node *condExpr,*thenBlock,*elseBlock; } ifStmt;
        struct whileStmt  {struct Node *condExpr,*bodyBlock;            } whileStmt, repeatStmt;
        struct funcCall   {struct Node *func,*param;                    } call;
        struct func       {struct Node *name,*typ;                      } func;
        struct vec        {bool one; List *val; struct Node *typ;       } vec;
        struct name       {bool one, isUS; char *id1,*id2; \
                           struct SymbolTable *st;struct SymbolName *sn;} name;
        struct block      {List *stmts; struct SymbolTable *st;         } block;
        struct ConstValue *nodeC;
        char   *idS;
        List   *listS;
    } val;
    Kind kind;
    int lineno;
}Node;

typedef enum Constant {
    intC, floatC, longC, clexC, charC, strC, symC,
    dtC, dateC, monthC, timeC, minuteC, secondC,
    totalC
}Constant;

typedef struct ConstValue{
    Constant type; 
    union {
        int       valI;
        double    valF;
        long long valL;
        float     valX[2];
        char     *valS;
    };
}ConstValue;

// wrongT: uninitialized type
typedef enum Type {
    wildT, boolT, i8T, i16T, i32T, i64T, f32T, f64T, clexT, charT, symT, strT,
    dateT, monthT, timeT, dtT, minuteT, secondT, listT, dictT, enumT, tableT,
    ktableT, funcT, wrongT,
    totalT
}Type;

#define makeInt(x)    makeIntType(x,    intC)
#define makeDate(x)   makeIntType(x,   dateC)
#define makeMonth(x)  makeIntType(x,  monthC)
#define makeTime(x)   makeIntType(x,   timeC)
#define makeMinute(x) makeIntType(x, minuteC)
#define makeSecond(x) makeIntType(x, secondC)

#define makeLong(x)   makeLongType(x,  longC)
#define makeDT(x)     makeLongType(x,    dtC)

#define makeString(x) makeStringType(x, strC)
#define makeSymbol(x) makeStringType(x, symC)
#define makeChar(x)   makeStringType(x,charC)

Prog *makeProg (List *modules);
List *makeList (List *next, Node *val); /* list , node */

Node *makeNodeID           (char *id);
Node *makeListKind         (List *t, Kind k);

Node *makeNodeModule       (char *id, List *body);
Node *makeNodeImport       (char *id, List *value);
Node *makeNodeModuleMethod (char *kind, char *id, Node *param, List *type, Node *block);
Node *makeNodeVar          (char *id, Node *type);
Node *makeNodeVarDecl      (List *vars, Node *type);
Node *makeNodeGlobal       (char *id, Node *type, Node *operand);
Node *makeNodeType         (bool wild, char *id, List *cellType);
Node *makeNodeAssignment   (List *vars, Node *expr);
Node *makeNodeCast         (Node *expr, Node *type);
Node *makeNodeName         (char *id1, char *id2);
Node *makeNodeStmtIf       (Node *cond, Node *then, Node *other);
Node *makeNodeStmtWhile    (Node *cond, Node *body);
Node *makeNodeStmtRepeat   (Node *cond, Node *body);
Node *makeNodeStmtExpr     (Node *expr);
Node *makeNodeStmtBreak    ();
Node *makeNodeStmtContinue ();
Node *makeNodeFuncCall     (Node *func, Node *paramExpr);
Node *makeNodeLiteralFunc  (Node *func, Node *type);
Node *makeNodeLiteralVector(bool isOne, List *value, Node *type);
Node *makeNodeConst        (ConstValue *val);
Node *makeNodeParamExpr    (List *params);
Node *makeNodeBlock        (List *stmts);
Node *makeNodeStmtReturn   (List *operands);
Node *makeNodeArgExpr      (List *operands);

/* constant functions */
ConstValue *makeIntType    (int x, Constant t);
ConstValue *makeLongType   (long long x, Constant t);
ConstValue *makeStringType (char *x, Constant t);
ConstValue *makeFloat      (double x);
ConstValue *makeReal       (char op, ConstValue *x);
ConstValue *makeComplex    (ConstValue *x, char op, ConstValue *y);

#endif
