#ifndef __H_O_COMMON__
#define __H_O_COMMON__

// TODO: the struct below should be removed (bcz of no use)
typedef struct FuseNode {
    S targ, invc;
}OptNode, FuseNode, PeepholeNode;

/* macros */
#define chainNode(c)      (c)->cur
#define chainUseSize(c)   (c)->useSize
#define chainDefSize(c)   (c)->defSize
#define chainUse(c,i)     (c)->chain_uses[i]
#define chainDef(c,i)     (c)->chain_defs[i]

#define isChainVisited(c) (c->isVisited)
#define setVisited(c, v)  (c)->isVisited=v

// node: module
#define getModuleName(n)  (n)->module.id    // S
#define getModuleBody(n)  (n)->module.body  // List

// node: import
#define getImportName(n)  (n)->import.module  // S
#define getImportItems(n) (n)->import.content // List

// node: methodK
#define getMethodName(n)        (n)->val.method.fname  // S
#define getMethodReturnTypes(n) (n)->val.method.typ    // List
#define getMethodParameters(n)  (n)->val.method.param  // Node
#define getMethodBlock(n)       (n)->val.method.block  // Node
#define getMethodMeta(n)        (n)->val.method.meta   // MetaMethod 
#define getMethodChainList(n)   getMethodMeta(n)->chains

// node: stmtK
#define getStmtVars(n)   (n)->val.assignStmt.vars
#define getStmtExpr(n)   (n)->val.assignStmt.expr

// node: nameK
#define getName1(n)      (n)->val.name.id1        // S
#define getName2(n)      (n)->val.name.id2        // S
#define getNameKind(n)   (n)->val.name.sn->kind   // SymbolKind

// node: varK
#define getVarKind(n)    (n)->val.param.sn->kind  // SymbolKind
#define getVarName(n)    (n)->val.param.id        // S

// node: callK
#define getCallFunc(n)   (n)->val.call.func       // Node
#define getCallParam(n)  (n)->val.call.param      // Node

// node: list-based (funcK)
#define getNodeList(n)   (n)->val.listS           // List

// node: gloalK
#define getGlobalName(n) (n)->val.global.id       // S
#define getGlobalType(n) (n)->val.global.typ      // Node
#define getGlobalOp(n)   (n)->val.global.op       // Node

// node: typeK
#define getTypeInfo(n)   (n)->val.type.in

// node: castK
#define getCastExpr(n)   (n)->val.cast.exp        // Node
#define getCastType(n)   (n)->val.cast.typ        // Node

// node: exprStmtK
#define getExprStmt(n)   (n)->val.exprStmt.expr   // Node

/* declarations below */

O optElementwise();
O optPattern();
O optDeep();
O optSReduction(Prog *root);

S getMaxValue(C c);
S getMinValue(C c);
S getNameStr       (Node *n);
C getTypeCodeByName(Node *n);

S genFuncNameC   (S fn);
S genFuncNameDeep(S fn);

Node *getParamFromNode (Node *n, I pos);
List *fetchParams      (Node *n);
Node *fetchFuncNode    (Node *n);
Node *fetchFuncSingle  (Node *n);
List *fetchParamsIndex (List *list, I pos);

Node *getNodeFirstParam(Node *n);
Node *getSingleFunc    (Node *funcs);
Node *fetchEachFuncNode(Node *n);

B isElementwise(S funcName);
I findDefByName(Chain *p, S name);

Node* getStmtCall(Node *stmt);

#define CODE_MAX_SIZE 10240  // C code[CODE_MAX_SIZE]
S genDeclSingle (S func, C del);
B isDuplicated  (S *names, S s);
L searchName    (S *names, S s);
O genCodeConst  (Node *n);
O genCodeNode   (Node *n);
O genCodeList   (List *list);
O genCodeName   (Node *n, I id);
S genInvcSingle (S targ, S func, S *names, I num);

#endif
