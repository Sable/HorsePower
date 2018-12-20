#ifndef __H_GLOBAL__
#define __H_GLOBAL__

#if defined(__APPLE__)
    #define H_MACOS
#endif

#if defined(__linux__)
    #define H_LINUX
#endif

#if defined(__MINGW32__)
    #define H_WINDOWS
#endif

/* declare additional macros */
#ifdef H_LINUX
  #define _BSD_SOURCE
/* check for ANSI extensions: e.g. strdup */
  #ifdef __STDC_ALLOC_LIB__
    #define __STDC_WANT_LIB_EXT2__ 1
  #else
    #define _POSIX_C_SOURCE 200810L
  #endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>

#ifdef H_LINUX
#include <getopt.h>
#endif

#ifdef H_CALL
#include <valgrind/callgrind.h>
#endif

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
#define PN(x)     P("%s\n",x)
#define newLine() P("\n")
#define INFO(s)    FP(stderr, s "(%s:%d:%s):\n\t", __FILE__, __LINE__, __func__)
#define WP(...)   FP(stderr,__VA_ARGS__)  // warning
#define EP(...)   do{INFO("[ERROR]"); FP(stderr,__VA_ARGS__); exit(1);}while(0)  // error
#define PP(...)   do{/*INFO("[DEBUG]");*/ FP(stderr,__VA_ARGS__);     }while(0)  // debug
#define FT(s,...) FP(stdout,s,__VA_ARGS__)
#define FS(x)     FT("%s",x)

/* For debugging */
#define printBanner(s) P("/*==== "s" ====*/\n")

#include "backend/common.h"

#include "frontend/tree.h"
#include "frontend/pretty.h"
#include "frontend/weed.h"
#include "analysis/common.h"
#include "analysis/udchain.h"
#include "analysis/typeshape.h"
#include "translator/common.h"

void initGlobal   ();
void initTrie     ();
void initUDChain  ();
void initNameTable();
void initBackend  ();
void initTablesByQid(I id);

void insertString(char *str, Chain *chain, InfoNode *in);
Chain    *getChain(char *str);
InfoNode *getInfoNode(char *str);

void buildUDChain(Prog *root);
void analyzeSR(Prog *root);
void analyzeLF();
void analyzeCPF();
void analyzePeephole();
void printTypeShape(); /*util/trie.c*/
bool isElementwise(char *funcName);
E calcInterval(struct timeval t0, struct timeval t1); /* util/mics.c*/

/* util/nametable.c */
void saveToNameTable(char *str, V val);
V getValueFromNameTable(char *str);
/* util/connect.c */
int runModeServer();
int runModeClient();
/* util/info.c */
const char *getTypeName(L x);
const char *getpTypeName(pType x);
const char *getKindName(Kind x);
const char *getpFuncName(pFunc x);
int   getpTypeSize(pType x);
void  getInfoVar2(V x, S name);
#define getInfoVar(x) getInfoVar2(x, #x)
#define showTime(s) P("[%-10s]> The elapsed time (ms): %g\n",s,calcInterval(tv0, tv1))

void dotProg(Prog *root); /* util/dot.c */

#endif
