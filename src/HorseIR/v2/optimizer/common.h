#ifndef __H_O_COMMON__
#define __H_O_COMMON__

// TODO: the struct below should be removed (bcz of no use)
typedef struct FuseNode {
    S targ, invc;
}OptNode, FuseNode, PeepholeNode;

/* macros */
#define chainNode(c)      (c)->cur
#define isChainVisited(c) (c->isVisited)
#define setVisited(c, v)  (c)->isVisited=v

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
#define getCallFunc(x) x->val.call.func
#define getNameKind(n) (n)->val.name.sn->kind


#endif
