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

// node: methodK
#define getMethodMeta(x) (x)->val.method.meta
#define getMethodChainList(x) getMethodMeta(x)->chains

// node: stmtK
#define getStmtVars(n) (n->val.assignStmt.vars)
#define getStmtExpr(n) (n->val.assignStmt.expr)

// node: nameK
#define getName1(n)    (n)->val.name.id1
#define getName2(n)    (n)->val.name.id2
#define getNameKind(n) (n)->val.name.sn->kind

// node: varK
#define getVarKind(n)  (n)->val.param.sn->kind
#define getVarName(n)  (n)->val.param.id

// node: callK
#define getCallFunc(n)  (n)->val.call.func
#define getCallParam(n) (n)->val.call.param

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
