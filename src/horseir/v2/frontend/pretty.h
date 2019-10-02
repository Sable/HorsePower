#ifndef __H_PRETTY__
#define __H_PRETTY__

/* print messages */
char *getNodeTypeStr(Node *n);
void  printNodeType (Node *n);
void  printNodeLine (Node *n);
void  printNode     (Node *n);
void printNodeStr   (Node *n);
void  printProg     (Prog *root);

void  printType     (Type x);

#endif
