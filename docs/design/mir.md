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
import Builtin

module default {
    def main() {
        t0:list<sym>  = @column_value(employee:table,   `DepartmentID);
        t1:list<sym>  = @column_value(department:table, `DepartmentID);
        t2:i32        = @len(t0);
        t3:i32        = @len(t1);
        t4:list<i32>  = @index_of(t0, t1);  // begin
        t5:list<bool> = @lt(t4, t3);
        t6:list<int>  = @range(t3);
        t7:list<i32>  = @compress(t5, t6); // index for t1
        t8:list<i32>  = @compress(t5, t1);
        t9:list<i32>  = @unique(t8);
        t10:list<i32> = @asc(t9);          // index for t0

        t11:list<str> = @column_value(employee:table,   `LastName);
        t12:list<str> = @column_value(department:table, `DepartmentName);

        t13:list<str> = @index(t11,t10);
        t14:list<sym> = @index(t0 ,t10);
        t15:list<sym> = @index(t1 ,t7);
        t16:list<str> = @index(t12,t7);

        z0:dict<sym,str> = dict(`LastName,       t13);
        z1:dict<sym,sym> = dict(`DepartmentID,   t14);
        z2:dict<sym,sym> = dict(`DepartmentID,   t15);
        z3:dict<sym,str> = dict(`DepartmentName, t16);
        z4:?             = list(z0,z1,z2,z3);
        z:table          = @createTable(z4);
        print(z);
    }
}
```

### Conventions

`module`

    default module: sys

`method`

    entry method: main



## Grammar (See [details](MIR.g4))

```java
mir_program      ::= { module | method }
module           ::= "module" name "{" { method } "}"
method           ::= "def" name parameter_list type "{" statement_list "}"
parameter_list   ::= "(" name type { "," name type } ")"

/* main */
statement_list    ::= { statement ";" }
statement         ::= [label] statement_core
statement_core    ::= name type assign expression
                    | "return" expression
                    | "goto" label [name]
                    | "nop"
expression        ::= [ type_cast ] sub_expression
type_cast         ::= "(" type ")"
sub_expression    ::= method_call
                    | operand
operand           ::= name
                    | literal

method_call       ::= compound_name argument_list
argument_list     ::= "(" operand { "," operand } ")"

literal           ::= literal_nil      type
                    | literal_bool     type_bool
                    | literal_char     type_char
                    | literal_int      type_int
                    | literal_float    type_float
                    | literal_complex [type_complex]
                    | literal_symbol  [type_symbol]
                    | literal_time     type_time
                    | literal_list    [type_list]
                    | literal_dict    [type_dict]
                    | literal_table   [type_table]
                    | literal_ktable  [type_ktable]

type              ::= ":" (scalar_type | compound_type | unknown_type)
scalar_type       ::= type_bool
                    | type_char
                    | type_int
                    | type_float
                    | type_complex
                    | type_sym
                    | type_time
compound_type     ::= type_list
                    | type_dict
                    | type_enum
                    | type_table
                    | type_ktable
unknown_type      ::= "?"

type_bool         ::= "bool"
type_char         ::= "char"
type_int          ::= "i8" | "i16" | "i32" | "i64"
type_float        ::= "f32" | "f64"
type_complex      ::= "complex"
type_sym          ::= "sym"
type_time         ::= "m" | "d" | "z" | "u" | "v" | "t"
type_list         ::= "list" "<" type ">"
type_dict         ::= "dict" "<" type, type ">"
type_enum         ::= "enum" "<" type ">"
type_table        ::= "table"
type_ktable       ::= "ktable"

name              ::= id
compound_name     ::= name {"." name}
type_name         ::= name
label             ::= name
assign            ::= "="
literal_nil       ::= "nil"
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
