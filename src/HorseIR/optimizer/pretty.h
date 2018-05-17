#ifndef __H_PRETTY__
#define __H_PRETTY__

void printType    (pType p);
void printNodeType(Node *n);
void printNodeKind(Node *n);
void prettyNode   (Node *n);
void prettyList   (List *list, char sep);
void prettyProg   (Prog *root);

void prettyNodeBuff   (char *b, Node *n);
void prettyListBuff   (char *b, List *list, char sep);
void printTypeBuff    (char *b, pType p);
void printKindBuff    (char *b, Kind k);
void printNodeTypeBuff(char *b, Node *n);

#endif
