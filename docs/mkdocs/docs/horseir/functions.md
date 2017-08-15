# Bult-in functions

## 1. Type alias

<table style="width:45%;float:left">
<tr>
    <th> 1-level </th>
    <th> Description </th>
</tr>
<tr>
    <td>Int</td>
    <td>B, H, I, L</td>
</tr>
<tr>
    <td>Float</td>
    <td>F, E</td>
</tr>
<tr>
    <td>Complex</td>
    <td>X</td>
</tr>
<tr>
    <td>Date</td>
    <td>M, D, Z</td>
</tr>
<tr>
    <td>Time</td>
    <td>U, W, T</td>
</tr>
<tr>
    <td>String</td>
    <td>C, Q, S</td>
</tr>
<tr>
    <td>Compound</td>
    <td>G, N, Y, A, K</td>
</tr>
</table>

<table style="width:55%;float:left">
<tr>
    <th> 2-level </th>
    <th> Description </th>
</tr>
<tr>
    <td>Real</td>
    <td>Int, Float</td>
</tr>
<tr>
    <td>Number</td>
    <td>Real, Complex</td>
</tr>
<tr>
    <td>Datetime</td>
    <td>Date, Time</td>
</tr>
<tr>
    <td>Column</td>
    <td>Real, Date, Time, Q, S</td>
</tr>
<tr>
    <td>Comparable</td>
    <td>Real, Date, Time, String</td>
</tr>
<tr>
    <td>Basic</td>
    <td>Real, Date, Time, String, Complex</td>
</tr>
<tr>
    <td>Any</td>
    <td>Any of all types</td>
</tr>
</table>

See [type specifications](types.md).

# 2. Basics

Status

- &#10004;: Implemented
- P: Partially implemented
- "empty": Not implement yet

### 2.1 Unary

| No.| Function             | Description                 | Type of Arg(s)\*            | Type of Value                 | Details                      | Status |
| :--| :------------------- | :-------------------------- | :---------------------------| :---------------------------- | :--------------------------- | :------|
| 1  | abs(x)               | Absolute value of x         | Real                        | Same as argument              | [link](builtin/abs.md)       |&#10004;|
| 2  | neg(x)               | Negative value of x         | Real                        | Same as argument              | [link](builtin/neg.md)       |&#10004;|
| 3  | ceil(x)              | Ceiling value of x          | Real                        | Various                       | [link](builtin/ceil.md)      |&#10004;|
| 4  | floor(x)             | Floor value of x            | Real                        | Various                       | [link](builtin/floor.md)     |&#10004;|
| 5  | round(x)             | Round value of x            | Real                        | Various                       | [link](builtin/round.md)     |&#10004;|
| 6  | conj(x)              | Conjugate value of x        | Complex                     | Complex                       | [link](builtin/conj.md)      |&#10004;|
| 7  | recip(x)             | Reciprocal                  | Real                        | Various                       | [link](builtin/recip.md)     |&#10004;|
| 8  | signum(x)            | Signum                      | Real                        | Various                       | [link](builtin/signum.md)    |&#10004;|
| 9  | pi(x)                | PI times x                  | Number                      | Various                       | [link](builtin/pi.md)        |&#10004;|
| 10 | not(x)               | Not                         | B                           | B                             | [link](builtin/logic.md#not) |&#10004;|
| 11 | date(x)              | Time: date                  | Z                           | D                             | [link](date.md#date)         |&#10004;|
| 12 | date_year(x)         | Time: year                  | Date                        | L                             | [link](date.md#date-year)    |&#10004;|
| 13 | date_month(x)        | Time: month                 | Date                        | L                             | [link](date.md#date-month)   |&#10004;|
| 14 | date_day(x)          | Time: day                   | D,Z                         | L                             | [link](date.md#date-day)     |&#10004;|
| 15 | time(x)              | Time: time                  | Z                           | T                             | [link](date.md#time)         |&#10004;|
| 16 | time_hour(x)         | Time: hour                  | Time                        | L                             | [link](date.md#time-hour)    |&#10004;|
| 17 | time_minute(x)       | Time: minute                | Time                        | L                             | [link](date.md#time-minute)  |&#10004;|
| 18 | time_second(x)       | Time: second                | W,T,Z                       | L                             | [link](date.md#time-second)  |&#10004;|
| 19 | time_mill(x)         | Time: millisecond           | T,Z                         | L                             | [link](date.md#time-mill)    |&#10004;|
| 20 | cos(x)               | Trigonometric functions     | Real                        | Real                          | [link](builtin/trig.md#cos)  |&#10004;|
| 21 | sin(x)               | Trigonometric functions     | Real                        | Real                          | [link](builtin/trig.md#sin)  |&#10004;|
| 22 | tan(x)               | Trigonometric functions     | Real                        | Real                          | [link](builtin/trig.md#tan)  |&#10004;|
| 23 | acos(x)              | Trigonometric functions     | Real                        | Real                          | [link](builtin/trig.md#acos) |&#10004;|
| 24 | asin(x)              | Trigonometric functions     | Real                        | Real                          | [link](builtin/trig.md#asin) |&#10004;|
| 25 | atan(x)              | Trigonometric functions     | Real                        | Real                          | [link](builtin/trig.md#atan) |&#10004;|
| 26 | cosh(x)              | Hyperbolic functions        | Real                        | Real                          | [link](builtin/hyper.md#cosh)|&#10004;|
| 27 | sinh(x)              | Hyperbolic functions        | Real                        | Real                          | [link](builtin/hyper.md#sinh)|&#10004;|
| 28 | tanh(x)              | Hyperbolic functions        | Real                        | Real                          | [link](builtin/hyper.md#tanh)|&#10004;|
| 29 | acosh(x)             | Hyperbolic functions        | Real                        | Real                          |[link](builtin/hyper.md#acosh)|&#10004;|
| 30 | asinh(x)             | Hyperbolic functions        | Real                        | Real                          |[link](builtin/hyper.md#asinh)|&#10004;|
| 31 | atanh(x)             | Hyperbolic functions        | Real                        | Real                          |[link](builtin/hyper.md#atanh)|&#10004;|

\* [Math.h](http://www.cplusplus.com/reference/cmath/)

### 2.2 Binary

| No.| Function             | Description                 | Type of Arg(s)\*            | Type of Value                 | Details                      | Status |
| :--| :------------------- | :-------------------------- | :---------------------------| :---------------------------- | :--------------------------- | :----- |
| 1  | lt(x,y)              | x < y                       | (CP\*\*;CP)                 | B                             | [link](builtin/lt.md)        |&#10004;|
| 2  | gt(x,y)              | x > y                       | (CP;CP)                     | B                             | [link](builtin/gt.md)        |&#10004;|
| 3  | leq(x,y)             | x <= y                      | (CP;CP)                     | B                             | [link](builtin/leq.md)       |&#10004;|
| 4  | geq(x,y)             | x >= y                      | (CP;CP)                     | B                             | [link](builtin/geq.md)       |&#10004;|
| 5  | eq(x,y)              | x == y                      | (CP;CP)                     | B                             | [link](builtin/eq.md)        |&#10004;|
| 6  | neq(x,y)             | x != y                      | (CP;CP)                     | B                             | [link](builtin/neq.md)       |&#10004;|
| 7  | plus(x,y)            | Addition                    | Various                     | Various                       | [link](builtin/plus.md)      |P|
| 8  | minus(x,y)           | Subtraction                 | Various                     | Various                       | [link](builtin/minus.md)     |P|
| 9  | mul(x,y)             | Multiplication              | (Real;Real)                 | Various                       | [link](builtin/mul.md)       |P|
| 10 | div(x,y)             | Division                    | (Real;Real)                 | Various                       | [link](builtin/div.md)       |P|
| 11 | power(x,y)           | Power x of y                | (Real;Real)                 | Various                       | [link](builtin/power.md)     |P|
| 12 | log(x,y)             | log_x(y)                    | (Real;Real)                 | Various                       | [link](builtin/log.md)       |P|
| 13 | mod(x,y)             | x mod y                     | (Real;Real)                 | Various                       | [link](builtin/mod.md)       |&#10004;|
| 14 | and(x,y)             | And                         | (B;B)                       | B                             | [link](builtin/logic.md#and) |&#10004;|
| 15 | or(x,y)              | Or                          | (B;B)                       | B                             | [link](builtin/logic.md#or)  |&#10004;|
| 16 | nand(x,y)            | NAND                        | (B;B)                       | B                             | [link](builtin/logic.md#nand)|&#10004;|
| 17 | nor(x,y)             | NOR                         | (B;B)                       | B                             | [link](builtin/logic.md#nor) |&#10004;|
| 18 | xor(x,y)             | XOR                         | (B;B)                       | B                             | [link](builtin/logic.md#xor) |&#10004;|
| 19 | datetime_diff(x,y)\* | Datetime diff               | Pending                     | Pending                       | [link](date.md#datetime-diff)| |
| 20 | datetime_add(x,y)    | Datetime add                | Pending                     | Pending                       | [link](date.md#datetime-add) | |
| 21 | datetime_sub(x,y)    | Datetime sub                | Pending                     | Pending                       | [link](date.md#datetime-sub) | |

\* [Source, db functions](https://www.w3schools.com/sql/sql_ref_mysql.asp) | 
\*\* CP: Comparable

## 3. Algebraic related

### 3.1 Unary

| No.| Function             | Description                 | Type of Arg(s)\*            | Type of Value                 | Details                       | Status |
| :--| :------------------- | :-------------------------- | :---------------------------| :---------------------------- | :---------------------------- | :----- |
| 1  | unique(x)            | Unique values               | Basic                       | L                             | [link](builtin/unique.md)     |&#10004;|
| 2  | str(x)               | Format                      | Any                         | C                             | [link](builtin/str.md)        |P|
| 3  | len(x)               | Length                      | Any                         | L                             | [link](builtin/len.md)        |&#10004;|
| 4  | range(x)             | Iota                        | Int                         | List                          | [link](builtin/range.md)      |&#10004;|
| 5  | fact(x)              | Factorial                   | Real                        | E                             | [link](builtin/fact.md)       |&#10004;|
| 6  | rand(x)              | Random number generator     | Int                         | Same as argument              | link                          | |
| 7  | seed(x)              | Random seed                 | Int                         | L                             | link                          | |
| 8  | flip(x)              | Transform a dict to a table and vice versa | Dict, Table  | Table or Dict                 | link                          | |
| 9  | reverse(x)           | Reverse                     | Basic                       | Same as argument              | [link](builtin/reverse.md)    |&#10004;|
| 10 | where(x)             | Find all trues              | B                           | L                             | [link](builtin/where.md)      |&#10004;|


### 3.2 Binary

| No.| Function             | Description                 | Type of Arg(s)\*            | Type of Value                 | Details                      | Status |
| :--| :------------------- | :-------------------------- | :-------------------------- | :---------------------------- | :--------------------------- | :----- |
| 1  | append(x,y)          | Append to a list            | (Basic, List; Basic, List)  | Basic or List                 | [link](builtin/append.md)    |&#10004;|
| 2  | like(x,y)            | Patten x matches y          | (String; String)            | B                             | [link](builtin/like.md)      |P|
| 3  | compress(x,y)        | Compression on y based on x | (B;Basic)                   | Same as the 2nd argument      | [link](builtin/compress.md)  |&#10004;|
| 4  | randk(x,k)           | rand(x) in k times          | Pending                     | Pending                       | link                         | |
| 5  | index_of(x,y)        | index of y in x             | (Basic;Basic)               | L                             | [link](builtin/indexof.md)   |&#10004;|
| 6  | take(k,x)            | Take first/last k elem. of y| Pending                     | Pending                       | link                         | |
| 7  | drop(k,x)            | Drop first/last k elem. of y| Pending                     | Pending                       | link                         | |
| 8  | order(x,y)           | Sort x based on y           | (Real;B)                    | L                             | [link](builtin/order.md)     |&#10004;|
| 9  | member(x)            | Member                      | (Basic;Basic)               | B                             | [link](builtin/member.md)    |&#10004;|

### 3.3 Reduction

| No.| Function             | Description                 | Type of Arg(s)\*            | Type of Value                 | Details                      | Status |
| :--| :------------------- | :-------------------------- | :-------------------------- | :---------------------------- | :--------------------------- | :----- |
| 1  | count(x)             | Alias of @len               | See @len                    | See @len                      | [link](builtin/len.md)       |&#10004;|
| 2  | sum(x)               | Sum of items in x           | Real                        | Same as argument              | link                         |&#10004;|
| 3  | avg(x)               | sum(x) / len(x)             | Real                        | Same as argument              | N/A                          |&#10004;|
| 4  | min(x)               | Minimum                     | Real                        | Same as argument              | [link](builtin/reduce.md#min)|&#10004;|
| 5  | max(x)               | Maximum                     | Real                        | Same as argument              | [link](builtin/reduce.md#max)|&#10004;|

### 3.4 List

| No.| Function             | Description                 | Type of Arg(s)\*            | Type of Value        | Details                           | Status |
| :--| :------------------- | :-------------------------- | :-------------------------- | :------------------- | :-------------------------------- | :----- |
| 1  | raze(x)              | Raze a nested list          | List, Basic                 | Basic                | [link](builtin/list.md#raze)      |&#10004;|
| 2  | enlist(x)            | Return a list               | Any                         | List                 | [link](builtin/list.md#enlist)    |&#10004;|
| 7  | tolist(x)            | Vector to list              | Basic                       | List                 | [link](builtin/list.md#tolist)    |&#10004;|
| 3  | each_item(fn,x)      | Each on x with func `fn`    | (Func; List)                | List                 | [link](builtin/list.md#each_item) |&#10004;|
| 4  | each(fn,x,y)         | Each on both sides          | (Func; List; List)          | List                 | [link](builtin/list.md#each)      |&#10004;|
| 5  | each_left(fn,x,y)    | Each on left side           | (Func; List; List)          | List                 | [link](builtin/list.md#each_left) |&#10004;|
| 6  | each_right(fn,x,y)   | Each on Right side          | (Func; List; List)          | List                 | [link](builtin/list.md#each_right)|&#10004;|
| 7  | match(x,y)           | Check if both match         | (Any; Any)                  | Bool                 | link                              |&#10004;|

## 4. Database related

| No.| Function             | Description                 | Type of Arg(s)\*            | Type of Value        | Details                      | Status |
| :--| :------------------- | :-------------------------- | :---------------------------| :------------------- | :--------------------------- | :----- |
| 1  | enum                 | enum(variable, values)      | (Symol;Any)                 | Enum                 | [link](builtin/liter.md#enum)|&#10004;|
| 2  | dict                 | dict(keys, values)          | (Any;Any)                   | Dict                 | link                         |&#10004;|
| 3  | table                | table(col_names, values)    | (List;Column)               | Table                | link                         |&#10004;|
| 4  | ktable               | ktable(table1, table2)      | (Table;Table)               | KTable               | link                         |&#10004;|
| 5  | keys                 | Return keys                 | Table, KTable, Dict         | List                 | link                         |&#10004;|
| 6  | values               |                             | Pending                     | Pending              | link                         | |
| 7  | meta                 |                             | Pending                     | Pending              | link                         | |
| 8  | column_value         |                             | Table, KTable               | Various              | link                         |&#10004;|
| 9  | load_table           | Load a table                | Symbol                      | Table, KTable        | link                         |&#10004;|

## 5. Indexing

| No.| Function             | Description                 | Type of Arg(s)\*            | Type of Value        | Details                            | Status |
| :--| :------------------- | :-------------------------- | :-------------------------- | :------------------- | :--------------------------------- | :----- |
| 1  | index(x,y)           | List indexing (x[y])        | Various                     | Various              | [link](builtin/indexing.md#index)  |P|
| 2  | index_a              | List indexing assignment    | Pending                     | Pending              | link                               | |

## 6. Others

| No.| Function             | Description                 | Type of Arg(s)\*            | Type of Value                 | Details                      | Status |
| :--| :------------------- | :-------------------------- | :-------------------------- | :---------------------------- | :--------------------------- | :----- |
| 1  | load_csv             | Load a csv file             | String                      | Table                         | link                         |&#10004;|
| 2  | print                | Output                      | Any                         | Long (exit code)              | link                         | |
| 3  | format(x)            | Format output (print(str(x))| Any                         | String                        | link                         | |



