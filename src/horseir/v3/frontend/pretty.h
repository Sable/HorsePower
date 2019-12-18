#ifndef __H_PRETTY__
#define __H_PRETTY__

/* print messages */
void  printNodeType (Node *n);
void  printNodeLine (Node *n);
void  printNode     (Node *n);
void  printNodeStr  (Node *n);
void  printProg     (Prog *root);
void  printType     (HorseType x);

const char *getNodeTypeStr(Node *n);
const char *obtainNodeStr (Node *n);

#endif
