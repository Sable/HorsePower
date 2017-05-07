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
bool  bool        1 bit
char  char        1 byte
i16   short      16 bytes
i32   int        32 bytes
i64   long       64 bytes
f32   float      32 bytes
f64   double     64 bytes
clex  complex   128 bytes  //2 double
sym   symbol     32 bytes  //1 int

list    list         -
varchar varchar(n)   -
enum    enumertion   -
dict    dictionary   -
table   normal table -
ktable  keyed table  -
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

// Special
list      create a list
enum      create an enum
dict      create a dict
goto      goto a branch
```

