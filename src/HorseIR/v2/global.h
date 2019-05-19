#ifndef H_GLOBAL_H
#define H_GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <float.h>
#include <limits.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdarg.h> /* va_start */

/* declaration */
void  *newM(int n);
void   error(const char* s);
void   my_tic();
double my_toc(bool isPrint);
void   time_clear();

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

/* For debugging */
#define printBanner(s) P("/*==== "s" ====*/\n")
#define debug_tic struct timeval tv0, tv1; gettimeofday(&tv0, NULL)
#define debug_toc gettimeofday(&tv1, NULL); showTime("debug_toc")

/* time functions */
#define tic()      my_tic()
#define toc(...)   do{E elapsed=my_toc(0);P(__VA_ARGS__);}while(0)
#define time_toc() my_toc(1)

#include "frontend/tree.h"
#include "frontend/weed.h"
#include "frontend/pretty.h"
#include "frontend/pretty.h"
#include "frontend/name.h"
#include "backend/common.h"
#include "translator/common.h"

#endif
