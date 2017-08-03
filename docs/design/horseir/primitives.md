# Bult-in functions

## <p id="alias">1. Type alias</p>

1-level

- Int     = B, H, I, L
- Float   = F, E
- Complex = X
- Time    = M, D, Z, U, W, T
- String  = C, Q 

2-level

- Real    = Int,  Flt
- Numeric = Real, Time
- Basic   = Real, Time, String, Complex
- Advance = A, N, K 
- Any     = Any of all types
  

## <p id="basic">2. Basics</p>

Status

- &#10004;: Implemented
- P: Partially implemented
- \<empty\>: Not implement yet

### 2.1 Unary

No.| Function             | Description                 | Type of Arg(s)\*            | Type of Value                 | Details                      |No.| Status |
:--| :------------------- | :-------------------------- | :---------------------------| :---------------------------- | :--------------------------- |:--| :------|
1  | abs(x)               | Absolute value of x         | Real                        | Same as argument              | [link](builtin/abs.md)       |1  |&#10004;|
2  | neg(x)               | Negative value of x         | Real                        | Same as argument              | [link](builtin/neg.md)       |2  |&#10004;|
3  | ceil(x)              | Ceiling value of x          | Real                        | Various                       | [link](builtin/ceil.md)      |3  |&#10004;|
4  | floor(x)             | Floor value of x            | Real                        | Various                       | [link](builtin/floor.md)     |4  |&#10004;|
5  | round(x)             | Round value of x            | Real                        | Various                       | link                         |5  |&#10004;|
6  | conj(x)              | Conjugate value of x        | Complex                     | Complex                       | [link](builtin/conj.md)      |6  |&#10004;|
7  | recip(x)             | Reciprocal                  | Real                        | Various                       | [link](builtin/recip.md)     |7  |&#10004;|
8  | signum(x)            | Signum                      | Real                        | Various                       | [link](builtin/signum.md)    |8  |&#10004;|
9  | pi(x)                | PI times x                  | Real,Complex                | Various                       | [link](builtin/pi.md)        |9  |&#10004;|
10 | not(x)               | Not                         | B                           | B                             | [link](builtin/logic.md#not) |10 |&#10004;|
11 | date(x)              | Time: date                  | Z                           | D                             | [link](date.md#date)         |11 |&#10004;|
12 | date_year(x)         | Time: year                  | D,M,Z                       | L                             | [link](date.md#date-year)    |12 |&#10004;|
13 | date_month(x)        | Time: month                 | D,M,Z                       | L                             | [link](date.md#date-month)   |13 |&#10004;|
14 | date_day(x)          | Time: day                   | D,Z                         | L                             | [link](date.md#date-day)     |14 |&#10004;|
15 | time(x)              | Time: time                  | Z                           | T                             | [link](date.md#time)         |15 |&#10004;|
16 | time_hour(x)         | Time: hour                  | U,W,T,Z                     | L                             | [link](date.md#time-hour)    |16 |&#10004;|
17 | time_minute(x)       | Time: minute                | U,W,T,Z                     | L                             | [link](date.md#time-minute)  |17 |&#10004;|
18 | time_second(x)       | Time: second                | W,T,Z                       | L                             | [link](date.md#time-second)  |18 |&#10004;|
19 | time_mill(x)         | Time: millisecond           | T,Z                         | L                             | [link](date.md#time-mill)    |19 |&#10004;|
20 | cos(x)               | Trigonometric functions     | Real                        | Real                          | [link](builtin/trig.md#cos)  |20 |&#10004;|
21 | sin(x)               | Trigonometric functions     | Real                        | Real                          | [link](builtin/trig.md#sin)  |21 |&#10004;|
22 | tan(x)               | Trigonometric functions     | Real                        | Real                          | [link](builtin/trig.md#tan)  |22 |&#10004;|
23 | acos(x)              | Trigonometric functions     | Real                        | Real                          | [link](builtin/trig.md#acos) |23 |&#10004;|
24 | asin(x)              | Trigonometric functions     | Real                        | Real                          | [link](builtin/trig.md#asin) |24 |&#10004;|
25 | atan(x)              | Trigonometric functions     | Real                        | Real                          | [link](builtin/trig.md#atan) |25 |&#10004;|
26 | cosh(x)              | Hyperbolic functions        | Real                        | Real                          | [link](builtin/hyper.md#cosh)|26 |&#10004;|
27 | sinh(x)              | Hyperbolic functions        | Real                        | Real                          | [link](builtin/hyper.md#sinh)|27 |&#10004;|
28 | tanh(x)              | Hyperbolic functions        | Real                        | Real                          | [link](builtin/hyper.md#tanh)|28 |&#10004;|
29 | acosh(x)             | Hyperbolic functions        | Real                        | Real                          |[link](builtin/hyper.md#acosh)|29 |&#10004;|
30 | asinh(x)             | Hyperbolic functions        | Real                        | Real                          |[link](builtin/hyper.md#asinh)|30 |&#10004;|
31 | atanh(x)             | Hyperbolic functions        | Real                        | Real                          |[link](builtin/hyper.md#atanh)|31 |&#10004;|

\* [Math.h](http://www.cplusplus.com/reference/cmath/)

### 2.2 Binary

No.| Function             | Description                 | Type of Arg(s)\*            | Type of Value                 | Details                      |No.| Status |
:--| :------------------- | :-------------------------- | :---------------------------| :---------------------------- | :--------------------------- |:--| :----- |
1  | lt(x,y)              | x < y                       | (Basic;Basic)               | B                             | [link](builtin/lt.md)        |1  |&#10004;|
2  | gt(x,y)              | x > y                       | (Basic;Basic)               | B                             | [link](builtin/gt.md)        |2  |&#10004;|
3  | leq(x,y)             | x <= y                      | (Basic;Basic)               | B                             | [link](builtin/leq.md)       |3  |&#10004;|
4  | geq(x,y)             | x >= y                      | (Basic;Basic)               | B                             | [link](builtin/geq.md)       |4  |&#10004;|
5  | eq(x,y)              | x == y                      | (Basic;Basic)               | B                             | [link](builtin/eq.md)        |5  |&#10004;|
6  | neq(x,y)             | x != y                      | (Basic;Basic)               | B                             | [link](builtin/neq.md)       |6  |&#10004;|
7  | plus(x,y)            | Addition                    | Various                     | Various                       | [link](builtin/plus.md)      |7  |P|
8  | minus(x,y)           | Subtraction                 | Various                     | Various                       | [link](builtin/minus.md)     |8  |P|
9  | mul(x,y)             | Multiplication              | (Real;Real)                 | Various                       | [link](builtin/mul.md)       |9  |P|
10 | div(x,y)             | Division                    | (Real;Real)                 | Various                       | [link](builtin/div.md)       |10 |P|
11 | power(x,y)           | Power x of y                | (Real;Real)                 | Various                       | [link](builtin/power.md)     |11 |P|
12 | log(x,y)             | log_x(y)                    | (Real;Real)                 | Various                       | [link](builtin/log.md)       |12 |P|
13 | mod(x,y)             | x mod y                     | (Real;Real)                 | Various                       | [link](builtin/mod.md)       |13 |&#10004;|
14 | and(x,y)             | And                         | (B;B)                       | B                             | [link](builtin/logic.md#and) |14 |&#10004;|
15 | or(x,y)              | Or                          | (B;B)                       | B                             | [link](builtin/logic.md#or)  |15 |&#10004;|
16 | nand(x,y)            | NAND                        | (B;B)                       | B                             | [link](builtin/logic.md#nand)|16 |&#10004;|
17 | nor(x,y)             | NOR                         | (B;B)                       | B                             | [link](builtin/logic.md#nor) |17 |&#10004;|
18 | xor(x,y)             | XOR                         | (B;B)                       | B                             | [link](builtin/logic.md#xor) |18 |&#10004;|
19 | datetime_diff(x,y)\* | Datetime diff               | Pending                     | Pending                       | [link](date.md#datetime-diff)|19 | |
20 | datetime_add(x,y)    | Datetime add                | Pending                     | Pending                       | [link](date.md#datetime-add) |20 | |
21 | datetime_sub(x,y)    | Datetime sub                | Pending                     | Pending                       | [link](date.md#datetime-sub) |21 | |

\* [Source, db functions](https://www.w3schools.com/sql/sql_ref_mysql.asp)

## <p id="Algebraic">3. Algebraic related</p>

### 3.1 Unary

No.| Function             | Description                 | Type of Arg(s)\*            | Type of Value                 | Details                      |No.| Status |
:--| :------------------- | :-------------------------- | :---------------------------| :---------------------------- | :--------------------------- |:--| :----- |
1  | unique(x)            | Unique values               | Pending                     | L                             | [link](builtin/unique.md)    |1  |&#10004;|
2  | str(x)               | Format                      | Pending                     | C                             | [link](builtin/str.md)       |2  |P|
3  | len(x)               | Length                      | Any                         | L                             | [link](builtin/len.md)       |3  |&#10004;|
4  | range(x)             | Iota                        | Real                        | List                          | [link](builtin/range.md)     |4  |&#10004;|
5  | fact(x)              | Factorial                   | Real                        | E                             | [link](builtin/fact.md)      |5  |&#10004;|
6  | at(x)                | Member                      | Pending                     | B                             | [link](builtin/at.md)        |6  | |
7  | rand(x)              | Random number generator     | B,H,I,L                     | Same as argument              | link                         |7  | |
8  | seed(x)              | Random seed                 | B,H,I,L                     | L                             | link                         |8  | |
9  | flip(x)              | Transform a dict to a table and vice versa | Dict, Table  | Table or Dict                 | link                         |9  | |
10 | reverse(x)           | Reverse                     | Basic                       | Same as argument              | [link](builtin/reverse.md)   |10 |&#10004;|
11 | where(x)             | Find all trues              | B                           | L                             | [link](builtin/where.md)     |11 |&#10004;|
12 | raze(x)              | Raze a nested list          | Pending                     | Pending                       | link                         |12 | |
13 | enlist(x)            | Return a list               | Pending                     | Pending                       | link                         |13 | |
14 | scalar(x)            | Return a scalar             | Pending                     | Pending                       | link                         |14 | |


### 3.2 Binary

No.| Function             | Description                 | Type of Arg(s)\*            | Type of Value                 | Details                      |No.| Status |
:--| :------------------- | :-------------------------- | :-------------------------- | :---------------------------- | :--------------------------- |:--| :----- |
1  | append(x,y)          | Append to a list            | Pending                     | Pending                       | [link](builtin/append.md)    |1  |&#10004;|
2  | like(x,y)            | Patten x matches y          | Pending                     | B                             | [link](builtin/like.md)      |2  |P|
3  | compress(x,y)        | Compression on y based on x | (B;Basic)                   | Same as the 2nd argument      | [link](builtin/compress.md)  |3  |&#10004;|
4  | randk(x,k)           | rand(x) in k times          | Pending                     | Pending                       | link                         |4  | |
5  | index_of(x,y)        | index of y in x             | (Basic;Basic)               | L                             | [link](builtin/indexof.md)   |5  |&#10004;|
6  | take(k,x)            | Take first/last k elem. of y| Pending                     | Pending                       | link                         |6  | |
7  | drop(k,x)            | Drop first/last k elem. of y| Pending                     | Pending                       | link                         |7  | |
8  | order(x,y)           | Sort x based on y           | (Real;B)                    | L                             | [link](builtin/order.md)     |8  |&#10004;|

### 3.3 Reduction

No.| Function             | Description                 | Type of Arg(s)\*            | Type of Value                 | Details                      |No.| Status |
:--| :------------------- | :-------------------------- | :-------------------------- | :---------------------------- | :--------------------------- |:--| :----- |
1  | count(x)             | Alias of @len               | See @len                    | See @len                      | [link](builtin/len.md)       |1  |&#10004;|
2  | sum(x)               | Sum of items in x           | Real                        | Same as argument              | [link](builtin/sum.md)       |2  |&#10004;|
3  | avg(x)               | sum(x) / len(x)             | Real                        | Real                          | N/A                          |3  |&#10004;|
4  | min(x)               | Minimum                     | Real                        | Real                          | [link](builtin/reduce.md#min)|4  |&#10004;|
5  | max(x)               | Maximum                     | Real                        | Real                          | [link](builtin/reduce.md#max)|5  |&#10004;|

## <p id="database">4. Database related</p>

No.| Function             | Description                 | Type of Arg(s)\*            | Type of Value                 | Details                      |No.|
:--| :------------------- | :-------------------------- | :---------------------------| :---------------------------- | :--------------------------- |:--|
1  | list                 |                             | Pending                     | Pending                       | link                         |1  |
2  | enum                 |                             | Pending                     | Pending                       | link                         |2  |
3  | dict                 |                             | Pending                     | Pending                       | link                         |3  |
4  | table                |                             | Pending                     | Pending                       | link                         |4  |
5  | key                  |                             | Pending                     | Pending                       | link                         |5  |
6  | value                |                             | Pending                     | Pending                       | link                         |6  |
7  | meta                 |                             | Pending                     | Pending                       | link                         |7  |
8  | ktable               |                             | Pending                     | Pending                       | link                         |8  |
9  | column_value         |                             | Pending                     | Pending                       | link                         |9  |

## <p id="others">5. Others</p>

No.| Function             | Description                 | Type of Arg(s)\*            | Type of Value                 | Details                      |No.|
:--| :------------------- | :-------------------------- | :-------------------------- | :---------------------------- | :--------------------------- |:--|
1  | trig(k,x)            | Trigonometric functions     | Pending                     | Pending                       | [link](builtin/trig.md)      |1  |
2  | alphabet()           | All alphabet set            | Pending                     | Pending                       | link                         |2  |
3  | index(x,y)           | List indexing (x[y])        | Pending                     | Pending                       | link                         |3  |
4  | index_a              | List indexing assignment    | Pending                     | Pending                       | link                         |4  |
5  | print                | Output                      | Pending                     | Pending                       | link                         |5  |
6  | load_csv             | Load a csv file             | Pending                     | Pending                       | link                         |6  |

