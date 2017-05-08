# Horse IR  - MIR

```java
mir_program      ::= mir_module
                   | mir_expression

mir_module       ::= "module" ' module_name ' "{" module_body  "}"

module_name      ::= identifier
module_body      ::= module_shapes
                   | module_methods
                   | module_expression

module_shapes    ::= {shape_id ":" shape_info}
shape_id         ::= identifier
shape_info       ::= "<" shape_dim {"," shape_dim} ">"
shape_dim        ::= literal_int
                   | identifier
                   | id_dot

module_methods   ::= "def" method_name parameter_list method_body

method_name      ::= identifier

parameter_list   ::= "(" parameter_name { "," parameter_name } ")"
parameter_name   ::= identifier

method_body      ::= "{" statement_list "}"

/* main */
statement_list    ::= { statement }
statement         ::= empty
                    | expression

statement_flow    ::= "return" | label
label             ::= id ":"
goto              ::= "goto" label

expression        ::= [statement_flow] core_expression
                    | goto
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

variable          ::= id { "." id }
variable_index    ::= variable { "[" index_cell  "]" }
index_cell        ::= empty
                    | operand
/*
 * literal includes:
 *   int, float, double, list ...
 * empty means:
 *   <empty string>
 */
```

Types

```
b bool    0n  bool           1 bit
c char    0n  char           1 byte
h i16     0nh short         16 bytes
i i32     0ni int           32 bytes
u i64     0nu long          64 bytes
f f32     0nf float         32 bytes
e f64     0ne double        64 bytes
  clex    0nx complex      128 bytes //2 f64
  sym     0ns symbol        32 bytes //1 i32

m m       0nm month         32 bytes //1 i32
d d       0nd date          32 bytes //1 i32
z z       0nz date time     64 bytes //1 i64 
u u       0nu minute        32 bytes //1 i32
v v       0nv second        32 bytes //1 i32
t t       0nt time          32 bytes //1 i32

  list        list          -
  string  ''  varchar(n)    -
  enum        enumertion    -
  dict        dictionary    -
  table       normal table  -
  ktable      keyed table   -
```

Primitives

```
// op   name     dyadic           monadic
+       add      plus             conjugate
-       min      minus            negate
*       mul      multiplication   signum
%       div      division         reciprocal
=       eq       equal            unique
?       at       member
<       lt       less than        grade up
>       gt       greater than     grade down
#       len                       rho (length)
!       iota                      iota
~       not                       logical not

// Built-in (starting with 'sys.', e.g. 'sys.pi')
pi        3.1415...                  pi(n)
circle    sin/cos/tan/atan           circle(n, val)
rand      return a random number     rand(n) / rand(n, k)
seed      set random seed            seed(n)
power     power                      power(n, k)
sqrt      power(n, 0.5)              sqrt(n)
each      apply to lists             each(f,x) / each(f,x,y)
log       log                        log(b,n)
abs       abs                        abs(n)
mod       mod                        mod(x,y)
fact      factorial                  fact(n)
str       format                     str(s)
index     index of                   index(x,y)
and       logical and                and(x,y)
or        logical or                 or(x,y)
take      take n elements            take(n,x)
drop      drop n elements            drop(n,x)
floor     floor                      floor(n)
ceil      ceiling                    ceil(n)
round     round                      round(n)
max       maximum                    max(x,y)
min       minimum                    min(x,y)
compress  boolean selection          compress(x,y)
reduct    reduction                  reduct(fn, x)
scan      scan                       scan(fn, x)
append    concatenation              append(x,y)
raze      raze                       raze(x)
nor       nor                        nor(x) / nor(x,y)
nand      nand                       nand(x) / nand(x,y)
le        less equal than            le(x,y)
ge        greater equal than         ge(x,y)
ne        not equal                  ne(x,y)
print     print                      print(x)
flip      transform to table         flip(x)
reverse   reverse                    reverse(x)
like      match                      like(x,y)
where     find true                  where(x)

// Special
list      create a list
enum      create an enum
dict      create a dict
goto      goto a branch
```



