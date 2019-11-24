#ifndef __H_O_COMMON__
#define __H_O_COMMON__

/* debugging macros */
#define debugPattern false   // pattern.c
#define debugAuto    false   // autofusion.c
#define debugElem    false   // elementwise.c

/* node macros */
#define chainNode(c)       (c)->cur
#define chainUseSize(c)    (c)->useSize
#define chainDefSize(c)    (c)->defSize
#define chainUse(c,i)      (c)->chain_uses[i]
#define chainDef(c,i)      (c)->chain_defs[i]

#define isChainVisited(c)  (c->isVisited)
#define setVisited(c, v)   (c)->isVisited=v

// node: module
#define nodeModuleName(n)  (n)->val.module.id    // S
#define nodeModuleBody(n)  (n)->val.module.body  // List

// node: import
#define nodeImportName(n)  (n)->import.module  // S
#define nodeImportItems(n) (n)->import.content // List

// node: methodK
#define nodeMethodName(n)        (n)->val.method.fname      // S
#define nodeMethodReturnTypes(n) (n)->val.method.typ        // List
#define nodeMethodParameters(n)  (n)->val.method.param      // Node
#define nodeMethodBlock(n)       (n)->val.method.block      // Node
#define nodeMethodMeta(n)        (n)->val.method.meta       // MetaMethod 
#define nodeMethodChainList(n)   nodeMethodMeta(n)->chains  // no dummy

// node: stmtK
#define nodeStmtVars(n)   (n)->val.assignStmt.vars
#define nodeStmtExpr(n)   (n)->val.assignStmt.expr

// node: nameK
#define nodeName1(n)      (n)->val.name.id1        // S
#define nodeName2(n)      (n)->val.name.id2        // S
#define nodeNameKind(n)   (n)->val.name.sn->kind   // SymbolKind

// node: varK
#define nodeVarName(n)    (n)->val.param.id        // S
#define nodeVarType(n)    (n)->val.param.typ       // Node
#define nodeVarSymbol(n)  (n)->val.param.sn        // SymbolName
#define nodeVarKind(n)    nodeVarSymbol(n)->kind   // SymbolKind

// node: callK
#define nodeCallFunc(n)   (n)->val.call.func       // Node
#define nodeCallParam(n)  (n)->val.call.param      // Node

// node: list-based (funcK)
#define nodeList(n)       (n)->val.listS           // List

// node: gloalK
#define nodeGlobalName(n) (n)->val.global.id       // S
#define nodeGlobalType(n) (n)->val.global.typ      // Node
#define nodeGlobalOp(n)   (n)->val.global.op       // Node

// node: typeK
#define nodeTypeInfo(n)   (n)->val.type.in         // InfoNode
#define nodeTypeCells(n)  (n)->val.type.cell       // List

// node: castK
#define nodeCastExpr(n)   (n)->val.cast.exp        // Node
#define nodeCastType(n)   (n)->val.cast.typ        // Node

// node: exprStmtK
#define nodeExprStmt(n)   (n)->val.exprStmt.expr   // Node

/* declarations below */

O  optElementwise();
O  optPattern    ();
O  optAuto       ();
O  optSReduction (Prog *root);

CS obtainMaxValue(C c);
CS obtainMinValue(C c);
C  obtainNodeTypeAlias(Node *n);

S  getNameStr       (Node *n);
C  getTypeCodeByName(Node *n); 
CS getFuncNameC     (S fn);
CS getFuncNameAuto  (S fn);


B  isElementwise(S funcName);
I  findDefByName(Chain *p, S name);

S  genDeclSingle (S func, C del);
B  isDuplicated  (S *names, S s);
L  searchName    (S *names, S s);
O  genCodeConst  (Node *n);
O  genCodeNode   (Node *n);
O  genCodeList   (List *list);
O  genCodeName   (Node *n, I id);
S  genInvcSingle (S targ, S func, S *names, I num);

Node *getNodeItemIndex(Node *n, I pos);
List *getNodeParams   (Node *n);
Node *getNodeFunc     (Node *n);
List *getParamsIndex  (List *list, I pos);

Node *getNodeFirstParam(Node *n);
Node *getSingleFunc    (Node *funcs);
Node *getEachFuncNode  (Node *n);
Node *getStmtCall(Node *stmt);

SymbolName *getNodeSymbolName(Node *n);

#endif
