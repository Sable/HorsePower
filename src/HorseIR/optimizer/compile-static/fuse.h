#ifndef __H_FUSE__
#define __H_FUSE__

#ifdef LT
#undef LT
#endif

#ifdef EQ
#undef EQ
#endif


#define AND(x,y) (x)&&(y)
#define OR(x,y)  (x)||(y)
#define GEQ(x,y) (x)>=(y)
#define LEQ(x,y) (x)<=(y)
#define LT(x,y)  (x)<(y)
#define GT(x,y)  (x)>(y)
#define EQ(x,y)  (x)==(y)
#define NEQ(x,y) (x)!=(y)

#define MUL(x,y)   (x)*(y)
#define DIV(x,y)   (x)/(y)
#define MINUS(x,y) (x)-(y)
#define PLUS(x,y)  (x)+(y)
#define NOT(x)     !(x)

#endif
