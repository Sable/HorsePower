#ifndef __H_TYPESHAPE__
#define __H_TYPESHAPE__

/* InfoNode has been moved to analysis/common.h */

/* signatures */

InfoNodeList *propagateType(Node *func, InfoNodeList *list);
I totalInfo(InfoNodeList *list); // skip dummy
I totalElement(List *list);      //   no dummy
O propagateTypeShape(Prog *root);


InfoNode *addToInfoList(InfoNodeList *list, InfoNode *in);
O cleanInfoList(InfoNodeList *in_list);

#endif

