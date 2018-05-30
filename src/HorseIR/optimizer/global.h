#ifndef __H_GLOBAL__
#define __H_GLOBAL__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>
#include <sys/time.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

void *newM(int n);
void error(const char* s);
#define NEW(typ) (typ*)newM(sizeof(typ))
#define NEW2(typ,n) (typ**)newM(sizeof(typ*)*(n))
#define NEWL(typ,n) (typ*)newM(sizeof(typ)*(n))
#define instanceOf(x,t) (x!=NULL&&(x->kind)==(t))

#define P printf
#define R return
#define SP sprintf
#define FP fprintf
#define PN(x) P("%s\n",x)
#define newLine() P("\n")
#define WP(...) FP(stderr,__VA_ARGS__)  // warning
#define EP(...) do{FP(stderr,__VA_ARGS__);exit(1);}while(0)  // error
#define FT(s,...) FP(stdout,s,__VA_ARGS__)
#define FS(x)     FT("%s",x)

#define printBanner(s) P("====="s"=====\n")

#include "backend/common.h"

#include "tree.h"
#include "pretty.h"
#include "analysis/common.h"
#include "analysis/udchain.h"
#include "analysis/typeshape.h"
#include "interpreter/intp.h"

void initGlobal ();
void initTrie   ();
void initUDChain();
void initNameTable();
void initBackend();

void insertString(char *str, Chain *chain, InfoNode *in);
Chain    *getChain(char *str);
InfoNode *getInfoNode(char *str);

void buildUDChain(Prog *root);
void analyzeSR(Prog *root);
void analyzeLF();
void analyzeCPF();
void printTypeShape(); /*util/trie.c*/
bool isElementwise(char *funcName);
L calcInterval(struct timeval t0, struct timeval t1); /* util/mics.c*/

/* util/nametable.c */
void saveToNameTable(char *str, V val);
V getValueFromNameTable(char *str);

#endif
