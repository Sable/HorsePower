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
#include <stdarg.h> /* va_start */

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
#define NEW(typ)    (typ* )newM(sizeof(typ))
#define NEW2(typ,n) (typ**)newM(sizeof(typ*)*(n))
#define NEWL(typ,n) (typ* )newM(sizeof(typ)*(n))
#define instanceOf(x,t) (x!=NULL&&(x->kind)==(t))

#define P printf
#define R return
#define SP sprintf
#define FP fprintf
#define PN(x)     P("%s\n",x)
#define newLine() P("\n")
#define INFO(s)   FP(stderr, s "(%s:%d:%s):\n\t", __FILE__, __LINE__, __func__)
#define WP(...)   FP(stderr,__VA_ARGS__)  // warning
#define EP(...)   do{INFO("[ERROR]"); FP(stderr,__VA_ARGS__); exit(1);}while(0)  // error
#define TODO(...) do{INFO("[TODO]");  FP(stderr,__VA_ARGS__); exit(1);}while(0)  // todo
//#define PP(...)   do{/*INFO("[DEBUG]");*/ FP(stderr,__VA_ARGS__);     }while(0)  // debug
#define PP(...)   do{/*INFO("[DEBUG]");*/ FP(stdout,__VA_ARGS__);     }while(0) 
#define FT(s,...) FP(stdout,s,__VA_ARGS__)
#define FS(x)     FT("%s",x)

#define isSerial() (1==H_CORE)
#define isMulti()  (1!=H_CORE)

/* For debugging */
#define printBanner(s) P("/*==== "s" ====*/\n")
#define debug_tic struct timeval tv0, tv1; gettimeofday(&tv0, NULL)
#define debug_toc gettimeofday(&tv1, NULL); showTime("debug_toc")

#include "frontend/tree.h"
#include "frontend/pretty.h"
#include "frontend/weed.h"
#include "backend/common.h"
#include "analysis/common.h"
//#include "analysis/udchain.h"
//#include "analysis/typeshape.h"
#include "translator/common.h"
#include "util/common.h"

void initGlobal   ();
void initTrie     ();
void initUDChain  ();
void initNameTable();
void initBackend  ();
void initTablesByQid(I id);

void insertString(char *str, Chain *chain, InfoNode *in);
Chain    *getChain(char *str);
InfoNode *getInfoNode(char *str);

bool isInElementwiseGroup(pFunc x);
bool isInReductionGroup  (pFunc x);
bool isInScanGroup       (pFunc x);
bool isInIndexingGroup   (pFunc x);
bool isInSpecialGroup    (pFunc x);

#endif
