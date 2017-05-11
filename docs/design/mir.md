# Horse IR  - MIR

See [an example](#example).

```java
/*
 * literal includes:
 *   int, float, double, list ...
 * empty means:
 *   <empty string>
 */

mir_program      ::= mir_module
                   | mir_expression
mir_module       ::= "module" module_name "{" module_body  "}"
module_name      ::= id
module_body      ::= module_methods
                   | module_expression
module_methods   ::= "def" method_name parameter_list method_body
method_name      ::= id
parameter_list   ::= "(" parameter_name { "," parameter_name } ")"
parameter_name   ::= id
method_body      ::= "{" statement_list "}"

/* main */
statement_list    ::= { statement }
statement         ::= empty
                    | expression

statement_flow    ::= "return" | label
label             ::= id ":"
goto_lable        ::= "goto" label
assign            ::= "="

expression        ::= [statement_flow] core_expression
                    | goto_label
core_expression   ::= variable       [ assign sub_expression ]
                    | variable_index [ assign variable ]
sub_expression    ::= function_call
                    | primitive_call
                    | variable_index
                    | operand
                    | literal

function_call     ::= module_name "." id "(" argument_list ")"
primitive_call    ::= [operand] primitive operand
operand           ::= id
variable          ::= id { "." id } ":" type
type              ::= id
variable_index    ::= variable { "[" index_cell  "]" }
index_cell        ::= ":"
                    | operand
```

Types

```
b   bool    0n      bool           1 bit
c   char    0n      char           1 byte
h   i16     0nh     short         16 bytes
i   i32     0ni     int           32 bytes
u   i64     0nu     long          64 bytes
f   f32     0nf     float         32 bytes
e   f64     0ne     double        64 bytes
    clex    0nx     complex      128 bytes //2 f64
    sym     0ns     symbol        32 bytes //1 i32

m   m       0nm     month         32 bytes //1 i32
d           0nd     date          32 bytes //1 i32
z           0nz     date time     64 bytes //1 i64 
u           0nu     minute        32 bytes //1 i32
v           0nv     second        32 bytes //1 i32
t           0nt     time          32 bytes //1 i32

    list    ()      list          -
    string  ''      varchar(n)    -
    enum            enumertion    -
    dict            dictionary    -
    table           normal table  -
    ktable          keyed table   -
```

Primitives

```
// Built-in (starting with 'sys.', e.g. 'sys.pi')

// Monadic
conj      conjjugate                conj(x)
neg       negate                    neg(x)
signum    -1,0,1                    signum(x)
recip     reciprocal                recip(x)
unique    unique                    unique(x)
asc       index: grade up           asc(x)
desc      index: grade down         desc(x)
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
raze      raze                      raze(x)
flip      transform to table        flip(x)
reverse   reverse                   reverse(x)
where     find true                 where(x)

// Dyadic
plus      elementwise: x+y          plus(x,y)
minus     elementwise: x-y          minus(x,y)
mul       elementwise: x*y          mul(x,y)
div       elementwise: x/y          div(x,y)
eq        elementwise: x=y          eq(x,y)
at        member                    at(x,y)
lt        elementwise: x<y          lt(x,y)
gt        elementwise: x>y          gt(x,y)
circle    sin/cos/tan/atan          circle(n, val)
randk     k times rand(x)           rand(x,k)
power     power                     power(x, k)
log       b:base                    log(b,x)
mod       mod                       mod(x,y)
index     index of                  index(x,y)
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
```

## Example

Two tables

```
employee
- LastName       : str
- DepartmentID   : sym

department
- DepartmentName : str
- DepartmentID   : sym
```

SQL query - Equi-join

```sql
SELECT * FROM employee, department
WHERE employee.DepartmentID = department.DepartmentID;
```

HorseIR - MIR

```
module _default {
    c0:dict<sym,sym> = column(employee:table, `DepartmentID:sym);
    c1:dict<sym,sym> = column(department:table, `DepartmentID:sym);
    t0:list<sym> = value(c0);
    t1:list<sym> = value(c1);
    t2:i32  = len(t0);
    t3:i32  = len(t1);
    t4:i32  = index(t0, t1);    // begin
    t5:bool = lt(t4, t3);
    t6:list = range(t3);
    t7:i32  = compress(t5, t6); // index for t1
    t8:i32  = compress(t5, t1);
    t9:i32  = unique(t8);
    t10:i32 = asc(t9);      // index for t0

    t11:dict<sym,str> = column(employee:table, `LastName);
    t12:dict<sym,str> = column(department:table, `DepartmentName);

    M0:dict<sym,str> = index(t11,t10);
    M1:dict<sym,sym> = index(t0 ,t10);
    M2:dict<sym,sym> = index(t1 ,t7);
    M3:dict<sym,str> = index(t12,t7);

    z0:list<?> = list(M0,M1,M2,M3);
    z:table    = createTable(z0);
}
```

## More examples

- [GROUP BY and ORDER](mir-example1.md)


