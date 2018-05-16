#ifndef __H_COMMON__
#define __H_COMMON__

/* nodes */
typedef struct ValueNode{
    Kind kind; int len;
    union {
        bool   b;
        int    i64,d;
        char   c;
        float  f32;
        double f64;
    };
    char *g;
}ValueNode;

/* signatures */
#endif
