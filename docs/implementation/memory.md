# Memory management

## Types

```
Name            Abbre.     tID
-------------------------------
bool            bool        0
char            char        1
short           i16         2
int             i32         3
long            i64         4
float           f32         5
double          f64         6
complex         complex     7
symbol          sym         8
month           m           9
date            d           10
datetime        z           11
minute          u           12
second          v           13
time            t           14
list            list        15
function        func        16
dictionary      dict        17
enumeration     enum        18
table           table       90
keyed table     ktable      91
-------------------------------
total                       21
```

### Basic types

Overview

```
typedef long long h_size;
enum h_types = {
    t_bool, t_char, t_short, t_int, ...
};

typedef struct _Value{
     h_size  n;          // length
enum h_types t;          // type
     union {
         bool      b;
         char      c;
         short int i16;
         int       i32;
         long long i64;
         float     f32;
         double    f64;
         complex   clx;
         int       sym;
         char      g[0]; // memory pointer
     };
} V0, *V;
```

Complex

```
typedef struct h_complex {
    double imag, real;
}complex;
```

### Advanced types

Overview

- List
- Dictionary
- Enumeration
- Table
- Keyed Table

**List**

- (1 2 3; 4 5 6)    //case 1
- (1 2 3; 'abc')    //case 2
- (1 2 3; ('a';5))  //case 3

** Dictionary**

- \`age:(20 18 21)

**Enumeration**

- ??

**Table**

- Comprise of multiple dictionaries

**Keyed table**

- Two normal tables


