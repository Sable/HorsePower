## Bult-in functions

Type alias

- A = B, H, I, L, F, E
- K = B, C, H, I, L, F, E, S, M, D, Z, U, W, T
  

### <p id="basic">Basics</p>

**Unary**

No.| Function             | Description                 | Type of Argument(s)\*            | Type of Value                 | Details                      |No.|
:--| :------------------- | :-------------------------- | :------------------------------- | :---------------------------- | :--------------------------- |:--|
1  | abs(x)               | Absolute value of x         | A                                | Same as argument              | [link](builtin/abs.md)       |1  |
2  | neg(x)               | Negative value of x         | A                                | Same as argument              | [link](builtin/neg.md)       |2  |
3  | ceil(x)              | Ceiling value of x          | A                                | Various                       | [link](builtin/ceil.md)      |3  |
4  | floor(x)             | Floor value of x            | A                                | Various                       | [link](builtin/floor.md)     |4  |
5  | round(x)             | Round value of x            | A                                | Various                       | link                         |5  |
6  | conj(x)              | Conjugate value of x        | X                                | X                             | [link](builtin/conj.md)      |6  |
7  | recip(x)             | Reciprocal                  | A                                | Various                       | [link](builtin/recip.md)     |7  |
8  | signum(x)            | Signum                      | A                                | Various                       | [link](builtin/signum.md)    |8  |
9  | pi(x)                | PI times x                  | A,X                              | Various                       | [link](builtin/pi.md)        |9  |
10 | not(x)               | Not                         | B                                | B                             | [link](builtin/logic.md#not) |10 |
11 | date(x)              | Time: date                  | Z                                | D                             | [link](date.md#date)         |11 |
12 | date_year(x)         | Time: year                  | D,M,Z                            | L                             | [link](date.md#date-year)    |12 |
13 | date_month(x)        | Time: month                 | D,M,Z                            | L                             | [link](date.md#date-month)   |13 |
14 | date_day(x)          | Time: day                   | D,Z                              | L                             | [link](date.md#date-day)     |14 |
15 | time(x)              | Time: time                  | Z                                | T                             | [link](date.md#time)         |15 |
16 | time_hour(x)         | Time: hour                  | U,W,T,Z                          | L                             | [link](date.md#time-hour)    |16 |
17 | time_minute(x)       | Time: minute                | U,W,T,Z                          | L                             | [link](date.md#time-minute)  |17 |
18 | time_second(x)       | Time: second                | W,T,Z                            | L                             | [link](date.md#time-second)  |18 |
19 | time_mill(x)         | Time: millisecond           | T,Z                              | L                             | [link](date.md#time-mill)    |19 |


**Binary**

No.| Function             | Description                 | Type of Argument(s)\*            | Type of Value                 | Details                      |No.|
:--| :------------------- | :-------------------------- | :------------------------------- | :---------------------------- | :--------------------------- |:--|
1  | lt(x,y)              | x < y                       | (K;K)                            | B                             | [link](builtin/lt.md)        |1  |
2  | gt(x,y)              | x > y                       | (K;K)                            | B                             | [link](builtin/gt.md)        |2  |
3  | leq(x,y)             | x <= y                      | (K;K)                            | B                             | [link](builtin/leq.md)       |3  |
4  | geq(x,y)             | x >= y                      | (K;K)                            | B                             | [link](builtin/geq.md)       |4  |
5  | eq(x,y)              | x == y                      | (K;K)                            | B                             | [link](builtin/eq.md)        |5  |
6  | neq(x,y)             | x != y                      | (K;K)                            | B                             | [link](builtin/neq.md)       |6  |
7  | plus(x,y)            | Addition                    | Various                          | Various                       | [link](builtin/plus.md)      |7  |
8  | minus(x,y)           | Subtraction                 | Various                          | Various                       | [link](builtin/minus.md)     |8  |
9  | mul(x,y)             | Multiplication              | (A;A)                            | Various                       | [link](builtin/mul.md)       |9  |
10 | div(x,y)             | Division                    | (A;A)                            | Various                       | [link](builtin/div.md)       |10 |
11 | power(x,y)           | Power x of y                | (A;A)                            | Various                       | link                         |11 |
12 | log(x,y)             | log_x(y)                    | (A;A)                            | Various                       | link                         |12 |
13 | mod(x,y)             | x mod y                     | (A;A)                            | Various                       | link                         |13 |
14 | and(x,y)             | And                         | (B;B)                            | B                             | [link](builtin/logic.md#and) |14 |
15 | or(x,y)              | Or                          | (B;B)                            | B                             | [link](builtin/logic.md#or)  |15 |
16 | nand(x,y)            | NAND                        | (B;B)                            | B                             | [link](builtin/logic.md#nand)|16 |
17 | nor(x,y)             | NOR                         | (B;B)                            | B                             | [link](builtin/logic.md#nor) |17 |
18 | xor(x,y)             | XOR                         | (B;B)                            | B                             | [link](builtin/logic.md#xor) |18 |
19 | datetime_diff(x,y)\* | Datetime diff               | Pending                          | Pending                       | [link](date.md#datetime-diff)|19 |
20 | datetime_add(x,y)    | Datetime add                | Pending                          | Pending                       | [link](date.md#datetime-add) |20 |
21 | datetime_sub(x,y)    | Datetime sub                | Pending                          | Pending                       | [link](date.md#datetime-sub) |21 |

\* [Source, db functions](https://www.w3schools.com/sql/sql_ref_mysql.asp)

### <p id="Algebraic">Algebraic related</p>

**Binary**

No.| Function             | Description                 | Type of Argument(s)\*            | Type of Value                 | Details                      |No.|
:--| :------------------- | :-------------------------- | :------------------------------- | :---------------------------- | :--------------------------- |:--|
1  | asc(x)               | Ascending order (index)     | Pending                          | L                             | [link](builtin/asc.md)       |1  |

### <p id="others">Others</p>

Trigonometric

| Function                        | Description             |
| :------------------------------ | :---------------------- |
| [trig](builtin/trig.md)         | trigonometric functions |

### <p id="general">General</p>

| Function                        | Description             | Function                        | Description             |
| :------------------------------ | :---------------------- | :------------------------------ | :---------------------- |
| [asc](builtin/asc.md)           | ascending order         | [rand*](builtin/rand.md)        | random number generator |
| [desc](builtin/desc.md)         | descending order        | [randk*](builtin/randk.md)      | repeat rand             |
| [unique](builtin/unique.md)     | unique values           | [seed*](builtin/seed.md)        | random seed             |
| [str](builtin/str.md)           | format                  | [flip*](builtin/flip.md)        | flip to a table         |
| [len](builtin/len.md)           | length                  | [reverse*](builtin/reverse.md)  | reverse                 |
| [range](builtin/range.md)       | range(0,n-1)            | [where*](builtin/where.md)      | find all trues          |
| [fact](builtin/fact.md)         | factorial*              | [raze*](builtin/raze.md)        | raze                    |
| [at](builtin/at.md)             | member                  | [enlist*](builtin/enlist.md)    | return a list           |
| [max*](builtin/max.md)          | maximum                 | [scalar*](builtin/scalar.md)    | return a scalar         | 
| [min*](builtin/min.md)          | min                     | [index_of*](builtin/indexof.md) | index of (mysql: FIELD) | 
| [append*](builtin/append.md)    | append                  | [take*](builtin/take.md)        | take elements           | 
| [like*](builtin/like.md)        | match                   | [drop*](builtin/drop.md)        | drop elements           |  
| [concat*]()                     | concatenation           | [alphabet*]()                   | alphabet                |

Indexing

| Function                        | Description             | Function                        | Description             |
| :------------------------------ | :---------------------- | :------------------------------ | :---------------------- |
| [index*](builtin/index.md)      | list indexing           | [index_a*](builtin/index.md)    | list indexing assignment|

Printing

| Function                        | Description             | Function                        | Description             |
| :------------------------------ | :---------------------- | :------------------------------ | :---------------------- |
| [print*]()                      | print                   |                                 |                         |

### <p id="database">Database related</p>

| Function                        | Description             | Function                        | Description             |
| :------------------------------ | :---------------------- | :------------------------------ | :---------------------- |
| list                            |                         | [column_value](builtin/column_value.md) |                 |
| enum                            |                         |                                 |                         |
| dict                            |                         |                                 |                         |
| table                           | create a new table      |                                 |                         |
| key                             |                         |                                 |                         | 
| value                           |                         |                                 |                         | 
| meta                            |                         |                                 |                         | 
| ktable                          | create a new keyed table|                                 |                         | 

### <p id="system">System related</p>

I/O

| Function                        | Description             |
| :------------------------------ | :---------------------- |
| load_csv                        | load a csv file         |


