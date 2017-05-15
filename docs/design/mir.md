# Horse IR  - MIR

## Introduction - An example

Two tables

```
employee
- LastName       : str
- DepartmentID   : sym

department
- DepartmentName : str
- DepartmentID   : sym (primary key)
```

SQL query - Equi-join

```sql
SELECT * FROM employee, department
WHERE employee.DepartmentID = department.DepartmentID;
```

HorseIR - MIR

```
module sys {
    def main() {
        c0:dict<sym,sym> = column(employee:table, `DepartmentID, `sym);
        c1:dict<sym,sym> = column(department:table, `DepartmentID, `sym);
        t0:list<sym> = value(c0);
        t1:list<sym> = value(c1);
        t2:i32  = len(t0);
        t3:i32  = len(t1);
        t4:i32  = indexof(t0, t1);  // begin
        t5:bool = lt(t4, t3);
        t6:list = range(t3);
        t7:i32  = compress(t5, t6); // index for t1
        t8:i32  = compress(t5, t1);
        t9:i32  = unique(t8);
        t10:i32 = asc(t9);          // index for t0

        t11:dict<sym,str> = column(employee:table, `LastName);
        t12:dict<sym,str> = column(department:table, `DepartmentName);

        M0:dict<sym,str> = index(t11,t10);
        M1:dict<sym,sym> = index(t0 ,t10);
        M2:dict<sym,sym> = index(t1 ,t7);
        M3:dict<sym,str> = index(t12,t7);

        z0:?    = list(M0,M1,M2,M3);
        z:table = createTable(z0);
        print(z);
    }
}
```

### Conventions

`module`

    default module: sys

`method`

    entry method: main



## Grammar

```java
mir_program      ::= { module | method }
module           ::= "module" name "{" { method } "}"
method           ::= "def" name parameter_list type "{" statement_list "}"
parameter_list   ::= "(" name type { "," name type } ")"

/* main */
statement_list    ::= { statement ";" }
statement         ::= name type assign expression
                    | "return" expression
                    | "goto" label
                    | label ":"
expression        ::= [ type_cast ] sub_expression
type_cast         ::= "(" type ")"
sub_expression    ::= method_call
                    | operand
operand           ::= name
                    | literal

method_call       ::= compound_name argument_list
argument_list     ::= "(" operand { "," operand } ")"

literal           ::= literal_basic
                    | literal_list    [type]
                    | literal_dict    [type]
literal_basic     ::= literal_bool     type
                    | literal_char     type
                    | literal_int      type
                    | literal_float    type
                    | literal_complex [type]
                    | literal_symbol  [type]
                    | literal_time     type

type              ::= ":" (scalar_type | compound_type | unknown_type)
scalar_type       ::= "bool" | "char"    | "i16"   | "i32" | "i64" | "f32"
                    | "f64"  | "complex" | "sym"
                    | "m"    | "d"       | "z"     | "u"   | "v"   | "t"
                    | "str"  | "func"    | "table" | "ktable"
compound_type     ::= "list" "<" type ">"
                    | "dict" "<" type, type ">"
                    | "enum" "<" type ">"
unknown_type      ::= "?"

name              ::= id
compound_name     ::= name {"." name}
type_name         ::= name
label             ::= id
assign            ::= "="
```

Types (See [examples](mir-example2.md))

```
b   bool      0n      bool           1 bit
c   char      0n      char           1 byte
h   i16       0nh     short         16 bytes
i   i32       0ni     int           32 bytes
u   i64       0nu     long          64 bytes
f   f32       0nf     float         32 bytes
e   f64       0ne     double        64 bytes
x   complex   0nx     complex      128 bytes //2 f64
    sym       0ns     symbol        32 bytes //1 i32

m   m         0nm     month         32 bytes //1 i32
d   d         0nd     date          32 bytes //1 i32
z   z         0nz     date time     64 bytes //1 i64
u   u         0nu     minute        32 bytes //1 i32
v   v         0nv     second        32 bytes //1 i32
t   t         0nt     time          32 bytes //1 i32

    list      ()      list          -
    string    ''      varchar(n)    -
    func              function      -
    enum              enumertion    -
    dict              dictionary    -
    table             normal table  -
    ktable            keyed table   -
```

Primitives (See [primitives](primitive.md))

```
// Built-in (starting with 'sys.', e.g. 'sys.pi')

// Monadic
conj      conjugate                 conj(x)
neg       negate                    neg(x)
signum    -1,0,1                    signum(x)
recip     reciprocal                recip(x)
unique    unique                    unique(x)
asc       order(1:bool)             asc(x)
desc      order(0:bool)             desc(x)
order     asc:true, desc:false      order(x,y)
len       rho (length)              len(x)
range     iota                      range(x)
not       logiclal not              not(x)
pi        3.1415...                 pi(x)
rand      return a random number    rand(x)
seed      set random seed           seed(x)
sqrt      power(x, 0.5)             sqrt(x)
abs       abs                       abs(n)
fact      factorial                 fact(n)
str       format                    str(s)
floor     floor                     floor(n)
ceil      ceiling                   ceil(n)
round     round                     round(n)
flip      transform to table        flip(x)
reverse   reverse                   reverse(x)
where     find true                 where(x)
raze      raze                      raze(x)
enlist    scalar to list            enlist(x)
scalar    list to scalar            scalar(x)

// Dyadic
plus      elementwise: x+y          plus(x,y)
minus     elementwise: x-y          minus(x,y)
mul       elementwise: x*y          mul(x,y)
div       elementwise: x/y          div(x,y)
eq        elementwise: x=y          eq(x,y)
at        member                    at(x,y)
lt        elementwise: x<y          lt(x,y)
gt        elementwise: x>y          gt(x,y)
trig      sin/cos/tan/atan          trig(n, x)
randk     k times rand(x)           rand(x,k)
power     power                     power(x, k)
log       b:base                    log(b,x)
mod       mod                       mod(x,y)
indexof   index of                  indexof(x,y)
and       logical and               and(x,y)
or        logical or                or(x,y)
take      take n elements           take(n,x)
drop      drop n elements           drop(n,x)
max       maximum                   max(x,y)
min       minimum                   min(x,y)
append    concatenation             append(x,y)
le        less equal than           le(x,y)
ge        greater equal than        ge(x,y)
ne        not equal                 ne(x,y)
like      match                     like(x,y)

// Special
each      apply to lists            each(f,x) / each(f,x,y)
compress  boolean selection         compress(x,y)
reduct    reduction                 reduct(fn, x)
scan      scan                      scan(fn, x)
inner     inner product             inner(fn,gn,x,y)
outer     outer product             outer(fn,x,y)
nor       nor                       nor(x) / nor(x,y)
nand      nand                      nand(x) / nand(x,y)
print     print                     print(x)
list      create a list
enum      create an enum
dict      create a dict
goto      goto a branch
key       return keys
value     return value
meta      return meta info
order     order
```

Directives

```
CAST      type casting
CHECK     type checking
```


## More examples

- [GROUP BY and ORDER](mir-example1.md)
- [Examples of types](mir-example2.md)


## Discussion

- No implicit type rules
- CAST: <required>
  + i32 = i8
  + list<i32> = list<i8> //memory transform from i8 to i32
- CHECK: <optimal in some way>
  + i32 = ?
  + list<i32> = list<?>  // check if ? is i32
  + left type should be specified (not ?)
