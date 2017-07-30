# Memory management

## Types

```
Name            Abbre.    Short     tID
----------------------------------------
bool            bool       B        0
short           i16        H        1 
int             i32        I        2 
long            i64        L        3
float           f32        F        4
double          f64        E        5
complex         complex    X        6
char            char       C        7 
symbol          sym        Q        8
month           m          M        9
date            d          D        10
datetime        z          Z        11
minute          u          U        12
second          w          W        13
time            t          T        14
list            list       G        15
dictionary      dict       N        16
enumeration     enum       Y        17
table           table      A        90
keyed table     ktable     K        91
function        func                92
----------------------------------------
total                               21
```

## Kinds

### Vector

```
B H I L F E X C Q M D Z U W T
```

**Complex (X)**

```
typedef struct h_complex {
    double imag, real;
}complex;
```

### List

```
G N Y A K
```

**List (G)**

- (1 2 3; 4 5 6)    //case 1
- (1 2 3; 'abc')    //case 2
- (1 2 3; ('a';5))  //case 3

**Dictionary (N)**

- \`age:(20 18 21)

**Enumeration (Y)**

- ??

**Table (A)**

- Comprise of multiple dictionaries

**Keyed table (K)**

- Two normal tables

### Function

```
func
```


