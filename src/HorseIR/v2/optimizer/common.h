#ifndef __H_O_COMMON__
#define __H_O_COMMON__

typedef struct FuseNode {
    S targ, invc;
}OptNode, FuseNode, PeepholeNode;

/* declarations below */

O optElementwise();
O optPattern();
O analyzeSR(Prog *root);

S getMaxValue(C c);
S getMinValue(C c);
S getNameStr       (Node *n);
C getTypeCodeByName(Node *n);

S genFuncNameC(S fn);

Node *getParamFromNode (Node *n, I pos);
List *fetchParams      (Node *n);
Node *fetchFuncNode    (Node *n);
Node *fetchFuncSingle  (Node *n);
List *fetchParamsIndex (List *list, I pos);


#endif
