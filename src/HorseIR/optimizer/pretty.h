#ifndef __H_PRETTY__
#define __H_PRETTY__

void printType(pType p);
void printNodeType (Node *n);
void printNodeKind(Node *n);
void prettyNode (Node *n);
void prettyList (List *list, char sep);
void prettyProg (Prog *root);

#endif
