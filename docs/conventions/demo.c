/* 0. include */
#include<stdio.h>

/* 1. typedef */
typedef int I;
typedef char C;
typedef void V;

/* 2. constant */
C helloWorld[] = "hello world\n";

/* 3. struct */
typedef struct _Node{
    I val;
    struct _Node *ptr;
}N0,*N;

/* 4. macros */
#define P printf
#define R return

/* 5. methods */
V foo(){P(helloWorld);}

/* 6. main */
I main(I argc, C *argv){
    foo();
    R 0;
}
/* 7. gcc demo.c */
/* 8. ./a.out */

