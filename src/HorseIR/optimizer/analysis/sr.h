
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

