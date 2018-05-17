#ifndef __H_GLOBAL__
#define __H_GLOBAL__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

void *newM(int n);
void error(const char* s);
#define NEW(typ) (typ*)newM(sizeof(typ))
#define NEW2(typ,n) (typ**)newM(sizeof(typ*)*(n))
#define NEWL(typ,n) (typ*)newM(sizeof(typ)*(n))
#define DOI(n,x) {int i=0,i2=n; for(;i<i2;i++)x;}
#define instanceOf(x,t) (x!=NULL&&(x->kind)==(t))


#define P printf
#define SP sprintf
#define PN(x) P("%s\n",x)
#define newLine() P("\n")


#include "tree.h"
#include "pretty.h"
#include "analysis/common.h"
#include "analysis/udchain.h"
#include "analysis/typeshape.h"

void initGlobal ();
void initTrie   ();
void initUDChain();

void insertString(char *str, Chain *chain, InfoNode *in);
Chain    *getChain(char *str);
InfoNode *getInfoNode(char *str);

void buildUDChain(Prog *root);
void analyzeSR(Prog *root);
void analyzeLF();
void printTrieType();

#endif
