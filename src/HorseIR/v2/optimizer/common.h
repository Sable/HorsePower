#ifndef __H_O_COMMON__
#define __H_O_COMMON__

typedef struct FuseNode {
    S targ, invc;
}OptNode, FuseNode, PeepholeNode;

/* declarations below */

O optElementwise();
O optPattern();
O analyzeSR(Prog *root);

#endif
