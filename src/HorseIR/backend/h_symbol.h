/*
 * Impl. for symbols
 * - Balance tree
 */
#pragma once

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct h_symbol{
	L len;
	C str[128];
}hsym;

#define Elemtype hsym
#define LH +1
#define EH  0
#define RH -1
#define EQ(a,b) hsymEqual(a,b)
#define LT(a,b) hsymLess(a,b)
#define LQ(a,b) hsymLessEqual(a,b)


typedef struct BST_node{
	Elemtype data;
	L bf;       //balance factor
	L index;	//for hashtable
	struct BST_node *lchild,*rchild;
}BSTnode,*BSTree;

void rRotate     (BSTree *p);
void L_Rotate    (BSTree *p);
void leftBalance (BSTree *T);
void rightBalance(BSTree *T);
bool InsertAVL   (BSTree *T, Elemtype e, bool *taller, L *index, L *id);
void initSym     ();

L getSymbol      (char* name);
void insertSym   (hsym h1);
void cleanSym    ();
void deleteT     (BSTree T);

B hsymEqual      (hsym h1, hsym h2);
B hsymLess       (hsym h1, hsym h2);
B hsymLessEqual  (hsym h1, hsym h2);

#ifdef	__cplusplus
}
#endif
