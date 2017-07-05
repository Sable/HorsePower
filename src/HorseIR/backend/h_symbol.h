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
}SB0,*SB;

#define Elemtype SB0
#define LH +1
#define EH  0
#define RH -1
#define EQ(a,b) symEqual(a,b)
#define LT(a,b) symLess(a,b)
#define LQ(a,b) symLessEqual(a,b)


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

L getSymbol      (S name);
L insertSym      (Elemtype h1);
void cleanSym    ();
void deleteT     (BSTree T);

B symEqual      (Elemtype h1, Elemtype h2);
B symLess       (Elemtype h1, Elemtype h2);
B symLessEqual  (Elemtype h1, Elemtype h2);

#ifdef	__cplusplus
}
#endif
