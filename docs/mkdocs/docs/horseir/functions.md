!!! tip "Built-in Functions"
    Built-in functions with definitions and examples.

## 1. Type Alias

See [type specifications](../../horseir/#types)

<div class="left-column">
<table>
<tr>
    <th> 1-level </th>
    <th> Description </th>
</tr>
<tr>
    <td>Int</td>
    <td>B, J, H, I, L</td>
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
</div>

<div class="right-column">
<table>
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
    <td>Calendar</td>
    <td>Date, Time</td>
</tr>
<tr>
    <td>Column</td>
    <td>Number, Date, Time, Q, S, Y</td>
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
</div>

## 2. Basics

Status

- &#10004;: Implemented
- P: Partially implemented
- "empty": Not implemented yet

### 2.1 Unary

<!--
Credit: https://www.w3schools.com/css/css_tooltip.asp
<style type="text/css">
    /*a:hover {color: black;background-color: yellow;}*/
.tooltip {
    position: relative;
    display: inline-block;
    border-bottom: 1px dotted black;
}

.tooltip .tooltiptext {
    visibility: hidden;
    width: 200px;
    background-color: black;
    color: #fff;
    text-align: center;
    border-radius: 6px;
    padding: 5px 0;

    /* Position the tooltip */
    position: absolute;
    top: -5px;
    left: 105%;
    z-index: 1;
}

.tooltip:hover .tooltiptext {
    visibility: visible;
}
.tooltip:hover {
    cursor: help;
}
</style>
-->

| No.| Function                                                               | Type of Arg(s)\*    | Type of Value                 | Details                      | Status | Parallel |
| :-:| :--------------------------------------------------------------------- | :------------------ | :---------------------------- | :--------------------------: | :------| :------: |
| 1  | <script>hgFunc('abs(x)'          ,'Absolute value of x    ');</script> | Real                | Same as argument              | [link](builtin/abs.md)       |&#10004;| Y        |
| 2  | <script>hgFunc('neg(x)'          ,'Negative value of x    ');</script> | Real                | Same as argument              | [link](builtin/neg.md)       |&#10004;| Y        |
| 3  | <script>hgFunc('ceil(x)'         ,'Ceiling value of x     ');</script> | Real                | Same as argument              | [link](builtin/ceil.md)      |&#10004;| Y        |
| 4  | <script>hgFunc('floor(x)'        ,'Floor value of x       ');</script> | Real                | Same as argument              | [link](builtin/floor.md)     |&#10004;| Y        |
| 5  | <script>hgFunc('round(x)'        ,'Round value of x       ');</script> | Real                | Same as argument              | [link](builtin/round.md)     |&#10004;| Y        |
| 6  | <script>hgFunc('conj(x)'         ,'Conjugate value of x   ');</script> | Complex             | Complex                       | [link](builtin/conj.md)      |&#10004;| Y        |
| 7  | <script>hgFunc('recip(x)'        ,'Reciprocal             ');</script> | Real                | Various                       | [link](builtin/recip.md)     |&#10004;| Y        |
| 8  | <script>hgFunc('signum(x)'       ,'Signum                 ');</script> | Real                | Various                       | [link](builtin/signum.md)    |&#10004;| Y        |
| 9  | <script>hgFunc('pi(x)'           ,'PI times x             ');</script> | Number              | Various                       | [link](builtin/pi.md)        |&#10004;| Y        |
| 10 | <script>hgFunc('not(x)'          ,'Not                    ');</script> | B                   | B                             | [link](builtin/logic.md#not) |&#10004;| Y        |
| 11 | <script>hgFunc('log(x)'          ,'Natural log            ');</script> | Real                | Number                        | [link](builtin/log.md#log)   |P       | Y        |
| 12 | <script>hgFunc('log2(x)'         ,'log base 2             ');</script> | Real                | Number                        | [link](builtin/log.md#log2)  |P       | Y        |
| 13 | <script>hgFunc('log10(x)'        ,'log base 10            ');</script> | Real                | Number                        | [link](builtin/log.md#log10) |P       | Y        |
| 14 | <script>hgFunc('sqrt(x)'         ,'Sqrt root              ');</script> | Real                | Number                        | [link](builtin/power.md#sqrt)|P       | Y        |
| 15 | <script>hgFunc('exp(x)'          ,'Exponential            ');</script> | Real                | Number                        | [link](builtin/power.md#exp) |P       | Y        |
| 16 | <script>hgFunc('date(x)'         ,'Time: date             ');</script> | Z                   | D                             | [link](builtin/date.md#date)        |&#10004;| Y        |
| 17 | <script>hgFunc('date\_year(x)'   ,'Time: year (yyyy)      ');</script> | Date                | H                             | [link](builtin/date.md#date-year)   |&#10004;| Y        |
| 18 | <script>hgFunc('date\_month(x)'  ,'Time: month (mm)       ');</script> | Date                | H                             | [link](builtin/date.md#date-month)  |&#10004;| Y        |
| 19 | <script>hgFunc('date\_day(x)'    ,'Time: day (dd)         ');</script> | D,Z                 | H                             | [link](builtin/date.md#date-day)    |&#10004;| Y        |
| 20 | <script>hgFunc('time(x)'         ,'Time: time             ');</script> | Z                   | T                             | [link](builtin/date.md#time)        |&#10004;| Y        |
| 21 | <script>hgFunc('time\_hour(x)'   ,'Time: hour (HH)        ');</script> | Time                | L                             | [link](builtin/date.md#time-hour)   |&#10004;| Y        |
| 22 | <script>hgFunc('time\_minute(x)' ,'Time: minute (MM)      ');</script> | Time                | L                             | [link](builtin/date.md#time-minute) |&#10004;| Y        |
| 23 | <script>hgFunc('time\_second(x)' ,'Time: second (SS)      ');</script> | W,T,Z               | L                             | [link](builtin/date.md#time-second) |&#10004;| Y        |
| 24 | <script>hgFunc('time\_mill(x)'   ,'Time: millisecond (LL) ');</script> | T,Z                 | L                             | [link](builtin/date.md#time-mill)   |&#10004;| Y        |
| 25 | <script>hgFunc('cos(x)'          ,'Trigonometric functions');</script> | Real                | Real                          | [link](builtin/trig.md#cos)  |&#10004;| Y        |
| 26 | <script>hgFunc('sin(x)'          ,'Trigonometric functions');</script> | Real                | Real                          | [link](builtin/trig.md#sin)  |&#10004;| Y        |
| 27 | <script>hgFunc('tan(x)'          ,'Trigonometric functions');</script> | Real                | Real                          | [link](builtin/trig.md#tan)  |&#10004;| Y        |
| 28 | <script>hgFunc('acos(x)'         ,'Trigonometric functions');</script> | Real                | Real                          | [link](builtin/trig.md#acos) |&#10004;| Y        |
| 29 | <script>hgFunc('asin(x)'         ,'Trigonometric functions');</script> | Real                | Real                          | [link](builtin/trig.md#asin) |&#10004;| Y        |
| 30 | <script>hgFunc('atan(x)'         ,'Trigonometric functions');</script> | Real                | Real                          | [link](builtin/trig.md#atan) |&#10004;| Y        |
| 31 | <script>hgFunc('cosh(x)'         ,'Hyperbolic functions   ');</script> | Real                | Real                          | [link](builtin/trig.md#cosh)|&#10004;| Y        |
| 32 | <script>hgFunc('sinh(x)'         ,'Hyperbolic functions   ');</script> | Real                | Real                          | [link](builtin/trig.md#sinh)|&#10004;| Y        |
| 33 | <script>hgFunc('tanh(x)'         ,'Hyperbolic functions   ');</script> | Real                | Real                          | [link](builtin/trig.md#tanh)|&#10004;| Y        |
| 34 | <script>hgFunc('acosh(x)'        ,'Hyperbolic functions   ');</script> | Real                | Real                          |[link](builtin/trig.md#acosh)|&#10004;| Y        |
| 35 | <script>hgFunc('asinh(x)'        ,'Hyperbolic functions   ');</script> | Real                | Real                          |[link](builtin/trig.md#asinh)|&#10004;| Y        |
| 36 | <script>hgFunc('atanh(x)'        ,'Hyperbolic functions   ');</script> | Real                | Real                          |[link](builtin/trig.md#atanh)|&#10004;| Y        |

\* [Math.h](http://www.cplusplus.com/reference/cmath/)

### 2.2 Binary

| No.| Function                                                             | Type of Arg(s)\*    | Type of Value                 | Details                      | Status | Parallel |
| :-:| :------------------------------------------------------------------- | :------------------ | :---------------------------- | :--------------------------: | :----- | :------: |
| 1  | <script>hgFunc('lt(x,y)              ','x < y         ');</script>   | (CP\*\*;CP)         | B                             | [link](builtin/lt.md)        |&#10004;| Y        |
| 2  | <script>hgFunc('gt(x,y)              ','x > y         ');</script>   | (CP;CP)             | B                             | [link](builtin/gt.md)        |&#10004;| Y        |
| 3  | <script>hgFunc('leq(x,y)             ','x <= y        ');</script>   | (CP;CP)             | B                             | [link](builtin/leq.md)       |&#10004;| Y        |
| 4  | <script>hgFunc('geq(x,y)             ','x >= y        ');</script>   | (CP;CP)             | B                             | [link](builtin/geq.md)       |&#10004;| Y        |
| 5  | <script>hgFunc('eq(x,y)              ','x == y        ');</script>   | (CP;CP)             | B                             | [link](builtin/eq.md)        |&#10004;| Y        |
| 6  | <script>hgFunc('neq(x,y)             ','x != y        ');</script>   | (CP;CP)             | B                             | [link](builtin/neq.md)       |&#10004;| Y        |
| 7  | <script>hgFunc('plus(x,y)            ','Addition      ');</script>   | Various             | Various                       | [link](builtin/plus.md)      |P       | Y        |
| 8  | <script>hgFunc('minus(x,y)           ','Subtraction   ');</script>   | Various             | Various                       | [link](builtin/minus.md)     |P       | Y        |
| 9  | <script>hgFunc('mul(x,y)             ','Multiplication');</script>   | (Real;Real)         | Various                       | [link](builtin/mul.md)       |P       | Y        |
| 10 | <script>hgFunc('div(x,y)             ','Division      ');</script>   | (Real;Real)         | Various                       | [link](builtin/div.md)       |P       | Y        |
| 11 | <script>hgFunc('power(x,y)           ','Power x of y  ');</script>   | (Real;Real)         | Various                       | [link](builtin/power.md#power)|P       | Y        |
| 12 | <script>hgFunc('logb(x,y)            ','log\_x(y)     ');</script>   | (Real;Real)         | Various                       | [link](builtin/log.md#logb)  |P       | Y        |
| 13 | <script>hgFunc('mod(x,y)             ','x mod y       ');</script>   | (Real;Real)         | Various                       | [link](builtin/mod.md)       |&#10004;| Y        |
| 14 | <script>hgFunc('and(x,y)             ','And           ');</script>   | (B;B)               | B                             | [link](builtin/logic.md#and) |&#10004;| Y        |
| 15 | <script>hgFunc('or(x,y)              ','Or            ');</script>   | (B;B)               | B                             | [link](builtin/logic.md#or)  |&#10004;| Y        |
| 16 | <script>hgFunc('nand(x,y)            ','NAND          ');</script>   | (B;B)               | B                             | [link](builtin/logic.md#nand)|&#10004;| Y        |
| 17 | <script>hgFunc('nor(x,y)             ','NOR           ');</script>   | (B;B)               | B                             | [link](builtin/logic.md#nor) |&#10004;| Y        |
| 18 | <script>hgFunc('xor(x,y)             ','XOR           ');</script>   | (B;B)               | B                             | [link](builtin/logic.md#xor) |&#10004;| Y        |

\* [Source, db functions](https://www.w3schools.com/sql/sql_ref_mysql.asp) | 
\*\* CP: Comparable

## 3. Algebraic Related

### 3.1 Unary

| No.| Function                                                              | Type of Arg(s)\*            | Type of Value        | Details                       | Status | Parallel |
| :-:| :-------------------------------------------------------------------- | :---------------------------| :------------------- | :---------------------------: | :----- | :------: |
| 1  | <script>hgFunc('unique(x)  ','Unique values          ');</script>     | Basic                       | L                    | [link](builtin/unique.md)     |&#10004;| P        |
| 2  | <script>hgFunc('str(x)     ','Format                 ');</script>     | Any                         | S                    | [link](builtin/str.md)        |        |         |
| 3  | <script>hgFunc('len(x)     ','Length                 ');</script>     | Any                         | L                    | [link](builtin/len.md)        |&#10004;| N        |
| 4  | <script>hgFunc('range(x)   ','Iota                   ');</script>     | Int                         | L                    | [link](builtin/range.md)      |&#10004;| Y        |
| 5  | <script>hgFunc('fact(x)    ','Factorial              ');</script>     | Int                         | L                    | [link](builtin/fact.md)       |&#10004;| Y        |
| 6  | <script>hgFunc('rand(x)    ','Random number generator');</script>     | Int                         | Same as argument     | link                          |        |         |
| 7  | <script>hgFunc('seed(x)    ','Random seed            ');</script>     | Int                         | L                    | link                          |        |         |
| 8  | <script>hgFunc('flip(x)    ','Convert a dict to a table and vice versa');</script>   | Dict, Table  | Table or Dict        | link                          |        | Y        |
| 9  | <script>hgFunc('reverse(x) ','Reverse                ');</script>     | Basic                       | Same as argument     | [link](builtin/reverse.md)    |&#10004;| Y        |
| 10 | <script>hgFunc('where(x)   ','Find all trues         ');</script>     | B                           | L                    | [link](builtin/where.md)      |&#10004;| P        |
| 11 | <script>hgFunc('group(x)   ','Group items            ');</script>     | Basic                       | List<i64>            | [link](builtin/group.md)      |&#10004;| P        |


### 3.2 Binary

| No.| Function                                                                | Type of Arg(s)\*            | Type of Value            | Details                      | Status | Parallel |
| :-:| :---------------------------------------------------------------------- | :-------------------------- | :----------------------- | :--------------------------: | :----- | :------: |
| 1  | <script>hgFunc('append(x,y)    ','Append to a list          ');</script>| Various                     | Various                  | [link](builtin/append.md)    |&#10004;| Y        |
| 2  | <script>hgFunc('like(x,y)      ','Patten x matches y        ');</script>| (String; String)            | B                        | [link](builtin/like.md)      |P       | ?(CHECKE)|
| 3  | <script>hgFunc('compress(x,y)  ','Compress on y based on x  ');</script>| (B;Basic)                   | Same as the 2nd argument | [link](builtin/compress.md)  |&#10004;| ?        |
| 4  | <script>hgFunc('randk(x,k)     ','rand(x) in k times        ');</script>| Pending                     | Pending                  | link                         |        |         |
| 5  | <script>hgFunc('index\_of(x,y) ','index of y in x           ');</script>| (Basic;Basic)               | L                        | [link](builtin/indexof.md)   |&#10004;| Y        |
| 6  | <script>hgFunc('take(k,x)      ','Take 1st/last k elem. of y');</script>| Pending                     | Pending                  | link                         |        |         |
| 7  | <script>hgFunc('drop(k,x)      ','Drop 1st/last k elem. of y');</script>| Pending                     | Pending                  | link                         |        |         |
| 8  | <script>hgFunc('order(x,y)     ','Sort x based on y         ');</script>| (Real;B)                    | L                        | [link](builtin/order.md)     |&#10004;| N        |
| 9  | <script>hgFunc('member(x,y)    ','Member                    ');</script>| (Basic;Basic)               | B                        | [link](builtin/member.md)    |&#10004;| ?        |
| 10 | <script>hgFunc('vector(x,y)    ','Init a vector             ');</script>| (L;Basic,List)              | Basic or List            | [link](builtin/vector.md)    |&#10004;| P        |

### 3.3 Reduction

| No.| Function                                                     | Type of Arg(s)\*   | Type of Value                 | Details                      | Status | Parallel |
| :-:| :----------------------------------------------------------- | :----------------- | :---------------------------- | :--------------------------: | :----- | :------: |
| 1  | <script>hgFunc('sum(x)    ','Sum of items in x ');</script>  | Real               | Same as argument              | [link](builtin/reduce.md#sum)|&#10004;| Y        |
| 2  | <script>hgFunc('avg(x)    ','sum(x) / len(x)   ');</script>  | Real               | Same as argument              | [link](builtin/reduce.md#avg)|&#10004;| Y        |
| 3  | <script>hgFunc('min(x)    ','Minimum           ');</script>  | Real               | Same as argument              | [link](builtin/reduce.md#min)|&#10004;| N        |
| 4  | <script>hgFunc('max(x)    ','Maximum           ');</script>  | Real               | Same as argument              | [link](builtin/reduce.md#max)|&#10004;| N        |

### 3.4 List

| No.| Function                                                                    | Type of Arg(s)\*    | Type of Value        | Details                           | Status | Parallel |
| :-:| :-------------------------------------------------------------------------- | :------------------ | :------------------- | :-------------------------------: | :----- | :------: |
| 1  | <script>hgFunc('raze(x)              ','Raze a nested list      ');</script>| List, Basic         | Basic                | [link](builtin/list.md#raze)      |&#10004;|          |
| 2  | <script>hgFunc('list(...)            ','Return a list (n-cell)  ');</script>| (Any,...)           | List                 | [link](builtin/list.md#list)      |&#10004;|          |
| 3  | <script>hgFunc('tolist(x)            ','Vector to list          ');</script>| Basic               | List                 | [link](builtin/list.md#tolist)    |&#10004;| ?(CHECKE)|
| 4  | <script>hgFunc('each(fn,x)           ','Each on x with func `fn`');</script>| (Func; List)        | List                 | [link](builtin/list.md#each)      |&#10004;| ?(CHECKE)|
| 5  | <script>hgFunc('each\_item(fn,x,y)   ','Each on both sides      ');</script>| (Func; List; List)  | List                 | [link](builtin/list.md#each_item) |&#10004;| ?(CHECKE)|
| 6  | <script>hgFunc('each\_left(fn,x,y)   ','Each on left side       ');</script>| (Func; List; List)  | List                 | [link](builtin/list.md#each_left) |&#10004;| ?(CHECKE)|
| 7  | <script>hgFunc('each\_right(fn,x,y)  ','Each on Right side      ');</script>| (Func; List; List)  | List                 | [link](builtin/list.md#each_right)|&#10004;| ?(CHECKE)|
| 8  | <script>hgFunc('match(x,y)           ','Check if both match     ');</script>| (Any; Any)          | Bool                 | link                              |&#10004;| N        |
| 9  | <script>hgFunc('outer(fn,x,y)        ','General outer product   ');</script>| (Real; Real)        | List<?>              | [link](builtin/outer.md)          |&#10004;| N        |

## 4. Database Related

| No.| Function                                                                | Type of Arg(s)\*            | Type of Value    | Details                           | Status | Parallel |
| :-:| :---------------------------------------------------------------------- | :---------------------------| :--------------- | :-------------------------------: | :----- | :------: |
| 1  | <script>hgFunc('enum          ','enum(key, value)         ');</script>  | (Basic;Basic),(List;List)   | Enum             | [link](builtin/advanced.md#enum)  |&#10004;| N        |
| 2  | <script>hgFunc('dict          ','dict(keys, values)       ');</script>  | (Any;Any)                   | Dict             | [link](builtin/advanced.md#dict)  |&#10004;| ?        |
| 3  | <script>hgFunc('table         ','table(col\_names, values)');</script>  | (List;Column)               | Table            | [link](builtin/advanced.md#table) |&#10004;| ?        |
| 4  | <script>hgFunc('ktable        ','ktable(table1, table2)   ');</script>  | (Table;Table)               | KTable           | [link](builtin/advanced.md#ktable)|&#10004;| N        |
| 5  | <script>hgFunc('keys          ','Return keys              ');</script>  | Table, KTable, Dict         | List             | [link](builtin/dbhelper.md#keys)  |&#10004;| ?(CHECKE)|
| 6  | <script>hgFunc('values        ','Return value part        ');</script>  | Various                     | Various          | [link](builtin/dbhelper.md#values)|&#10004;| N        |
| 7  | <script>hgFunc('meta          ','Return meta info         ');</script>  | Various                     | Table            | [link](builtin/dbhelper.md#meta)  |        |          |
| 8  | <script>hgFunc('fetch         ','Fetch values             ');</script>  | Y                           | Various          | [link](builtin/dbhelper.md#fetch) |&#10004;| Y        |
| 9  | <script>hgFunc('column\_value ','                         ');</script>  | Table, KTable               | Various          | [link](builtin/dbhelper.md#column_value)|&#10004;| N  |
| 10 | <script>hgFunc('load\_table   ','Load a table             ');</script>  | Symbol                      | Table, KTable    | [link](builtin/dbhelper.md#load_table)  |&#10004;| N  |
| 11 | <script>hgFunc('datetime\_add(x,y,m) ','Date time add     ');</script>  | (Calendar;Int;Sym)          | Calendar         | [link](builtin/date.md#datetime-add) |&#10004;| Y     |
| 12 | <script>hgFunc('datetime\_sub(x,y,m) ','Date time sub     ');</script>  | (Calendar;Int;Sym)          | Calendar         | [link](builtin/date.md#datetime-sub) |        |       |
| 13 | <script>hgFunc('join\_index   ','General join index       ');</script>  | (func;Real;Real)            | List             | [link](builtin/join.md#join_index)   |&#10004;| ?     |

## 5. Misc

| No.| Function                                                                     | Type of Arg(s)\*     | Type of Value        | Details                            | Status | Parallel |
| :-:| :--------------------------------------------------------------------------- | :------------------- | :------------------- | :--------------------------------: | :----- | :------: |
| 1  | <script>hgFunc('index(x,y)       ','List indexing (x[y])        ');</script> | Various              | Various              | [link](builtin/indexing.md#index)  | P      | Y        |
| 2  | <script>hgFunc('index\_a(x,y,m)  ','List indexing assignment    ');</script> | (BasicA;Int;BasicB)  | BasicB               | [link](builtin/indexing.md#indexa) | P      | N        |
| 3  | <script>hgFunc('load\_csv        ','Load a csv file             ');</script> | String               | Table                | link                               |&#10004;| N        |
| 4  | <script>hgFunc('print            ','Output                      ');</script> | Any                  | Long (exit code)     | link                               |        | N        |
| 5  | <script>hgFunc('sub_string(x,a,b)','Substring: x[a,b]           ');</script> | (str;Int;Int)        | str                  | [link](builtin/substring.md)       |&#10004;| N        |


