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

See [type specifications](types.md).

## 2. Basics

Status

- &#10004;: Implemented
- P: Partially implemented
- "empty": Not implement yet

### 2.1 Unary

<!--
Credit: https://www.w3schools.com/css/css_tooltip.asp
-->
<style type="text/css">
    /*a:hover {color: black;background-color: yellow;}*/
.tooltip {
    position: relative;
    display: inline-block;
    border-bottom: 1px dotted black;
}

.tooltip .tooltiptext {
    visibility: hidden;
    width: 150px;
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

<script>
    function hgFunc(funcName, funcDesc){
        document.write('<div class="tooltip">'
                       + funcName.trim()
                       + '<span class="tooltiptext">'
                       + funcDesc.trim()
                       + '</span>'
                       + '</div>')
        //document.write('<div title="' + funcDesc.trim() + '"><a>' + funcName.trim() + '</a></div>');
    }
</script>

| No.| Function                                                               | Type of Arg(s)\*    | Type of Value                 | Details                      | Status | Parallel |
| :-:| :--------------------------------------------------------------------- | :------------------ | :---------------------------- | :--------------------------: | :------| :------: |
| 1  | <script>hgFunc('abs(x)'          ,'Absolute value of x    ');</script> | Real                | Same as argument              | [link](builtin/abs.md)       |&#10004;| Y        |
| 2  | <script>hgFunc('neg(x)'          ,'Negative value of x    ');</script> | Real                | Same as argument              | [link](builtin/neg.md)       |&#10004;| Y        |
| 3  | <script>hgFunc('ceil(x)'         ,'Ceiling value of x     ');</script> | Real                | Various                       | [link](builtin/ceil.md)      |&#10004;| Y        |
| 4  | <script>hgFunc('floor(x)'        ,'Floor value of x       ');</script> | Real                | Various                       | [link](builtin/floor.md)     |&#10004;| Y        |
| 5  | <script>hgFunc('round(x)'        ,'Round value of x       ');</script> | Real                | Various                       | [link](builtin/round.md)     |&#10004;| Y        |
| 6  | <script>hgFunc('conj(x)'         ,'Conjugate value of x   ');</script> | Complex             | Complex                       | [link](builtin/conj.md)      |&#10004;| Y        |
| 7  | <script>hgFunc('recip(x)'        ,'Reciprocal             ');</script> | Real                | Various                       | [link](builtin/recip.md)     |&#10004;| Y        |
| 8  | <script>hgFunc('signum(x)'       ,'Signum                 ');</script> | Real                | Various                       | [link](builtin/signum.md)    |&#10004;| Y        |
| 9  | <script>hgFunc('pi(x)'           ,'PI times x             ');</script> | Number              | Various                       | [link](builtin/pi.md)        |&#10004;| Y        |
| 10 | <script>hgFunc('not(x)'          ,'Not                    ');</script> | B                   | B                             | [link](builtin/logic.md#not) |&#10004;| Y        |
| 11 | <script>hgFunc('date(x)'         ,'Time: date             ');</script> | Z                   | D                             | [link](date.md#date)         |&#10004;| Y        |
| 12 | <script>hgFunc('date\_year(x)'   ,'Time: year             ');</script> | Date                | L                             | [link](date.md#date-year)    |&#10004;| Y        |
| 13 | <script>hgFunc('date\_month(x)'  ,'Time: month            ');</script> | Date                | L                             | [link](date.md#date-month)   |&#10004;| Y        |
| 14 | <script>hgFunc('date\_day(x)'    ,'Time: day              ');</script> | D,Z                 | L                             | [link](date.md#date-day)     |&#10004;| Y        |
| 15 | <script>hgFunc('time(x)'         ,'Time: time             ');</script> | Z                   | T                             | [link](date.md#time)         |&#10004;| Y        |
| 16 | <script>hgFunc('time\_hour(x)'   ,'Time: hour             ');</script> | Time                | L                             | [link](date.md#time-hour)    |&#10004;| Y        |
| 17 | <script>hgFunc('time\_minute(x)' ,'Time: minute           ');</script> | Time                | L                             | [link](date.md#time-minute)  |&#10004;| Y        |
| 18 | <script>hgFunc('time\_second(x)' ,'Time: second           ');</script> | W,T,Z               | L                             | [link](date.md#time-second)  |&#10004;| Y        |
| 19 | <script>hgFunc('time\_mill(x)'   ,'Time: millisecond      ');</script> | T,Z                 | L                             | [link](date.md#time-mill)    |&#10004;| Y        |
| 20 | <script>hgFunc('cos(x)'          ,'Trigonometric functions');</script> | Real                | Real                          | [link](builtin/trig.md#cos)  |&#10004;| Y        |
| 21 | <script>hgFunc('sin(x)'          ,'Trigonometric functions');</script> | Real                | Real                          | [link](builtin/trig.md#sin)  |&#10004;| Y        |
| 22 | <script>hgFunc('tan(x)'          ,'Trigonometric functions');</script> | Real                | Real                          | [link](builtin/trig.md#tan)  |&#10004;| Y        |
| 23 | <script>hgFunc('acos(x)'         ,'Trigonometric functions');</script> | Real                | Real                          | [link](builtin/trig.md#acos) |&#10004;| Y        |
| 24 | <script>hgFunc('asin(x)'         ,'Trigonometric functions');</script> | Real                | Real                          | [link](builtin/trig.md#asin) |&#10004;| Y        |
| 25 | <script>hgFunc('atan(x)'         ,'Trigonometric functions');</script> | Real                | Real                          | [link](builtin/trig.md#atan) |&#10004;| Y        |
| 26 | <script>hgFunc('cosh(x)'         ,'Hyperbolic functions   ');</script> | Real                | Real                          | [link](builtin/hyper.md#cosh)|&#10004;| Y        |
| 27 | <script>hgFunc('sinh(x)'         ,'Hyperbolic functions   ');</script> | Real                | Real                          | [link](builtin/hyper.md#sinh)|&#10004;| Y        |
| 28 | <script>hgFunc('tanh(x)'         ,'Hyperbolic functions   ');</script> | Real                | Real                          | [link](builtin/hyper.md#tanh)|&#10004;| Y        |
| 29 | <script>hgFunc('acosh(x)'        ,'Hyperbolic functions   ');</script> | Real                | Real                          |[link](builtin/hyper.md#acosh)|&#10004;| Y        |
| 30 | <script>hgFunc('asinh(x)'        ,'Hyperbolic functions   ');</script> | Real                | Real                          |[link](builtin/hyper.md#asinh)|&#10004;| Y        |
| 31 | <script>hgFunc('atanh(x)'        ,'Hyperbolic functions   ');</script> | Real                | Real                          |[link](builtin/hyper.md#atanh)|&#10004;| Y        |

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
| 11 | <script>hgFunc('power(x,y)           ','Power x of y  ');</script>   | (Real;Real)         | Various                       | [link](builtin/power.md)     |P       | Y        |
| 12 | <script>hgFunc('log(x,y)             ','log\_x(y)     ');</script>   | (Real;Real)         | Various                       | [link](builtin/log.md)       |P       | Y        |
| 13 | <script>hgFunc('mod(x,y)             ','x mod y       ');</script>   | (Real;Real)         | Various                       | [link](builtin/mod.md)       |&#10004;| Y        |
| 14 | <script>hgFunc('and(x,y)             ','And           ');</script>   | (B;B)               | B                             | [link](builtin/logic.md#and) |&#10004;| Y        |
| 15 | <script>hgFunc('or(x,y)              ','Or            ');</script>   | (B;B)               | B                             | [link](builtin/logic.md#or)  |&#10004;| Y        |
| 16 | <script>hgFunc('nand(x,y)            ','NAND          ');</script>   | (B;B)               | B                             | [link](builtin/logic.md#nand)|&#10004;| Y        |
| 17 | <script>hgFunc('nor(x,y)             ','NOR           ');</script>   | (B;B)               | B                             | [link](builtin/logic.md#nor) |&#10004;| Y        |
| 18 | <script>hgFunc('xor(x,y)             ','XOR           ');</script>   | (B;B)               | B                             | [link](builtin/logic.md#xor) |&#10004;| Y        |
| 19 | <script>hgFunc('datetime\_diff(x,y)\*','Datetime diff ');</script>   | Pending             | Pending                       | [link](date.md#datetime-diff)|        |         |
| 20 | <script>hgFunc('datetime\_add(x,y,m) ','Datetime add  ');</script>   | (Date;Int;Sym)      | Date                          | [link](date.md#datetime-add) |&#10004;| Y        |
| 21 | <script>hgFunc('datetime\_sub(x,y,m) ','Datetime sub  ');</script>   | (Date;Int;Sym)      | Date                          | [link](date.md#datetime-sub) |        |         |

\* [Source, db functions](https://www.w3schools.com/sql/sql_ref_mysql.asp) | 
\*\* CP: Comparable

## 3. Algebraic related

### 3.1 Unary

| No.| Function                                                              | Type of Arg(s)\*            | Type of Value        | Details                       | Status | Parallel |
| :-:| :-------------------------------------------------------------------- | :---------------------------| :------------------- | :---------------------------: | :----- | :------: |
| 1  | <script>hgFunc('unique(x)  ','Unique values          ');</script>     | Basic                       | L                    | [link](builtin/unique.md)     |&#10004;| P        |
| 2  | <script>hgFunc('str(x)     ','Format                 ');</script>     | Any                         | C                    | [link](builtin/str.md)        |P       |         |
| 3  | <script>hgFunc('len(x)     ','Length                 ');</script>     | Any                         | L                    | [link](builtin/len.md)        |&#10004;| N        |
| 4  | <script>hgFunc('range(x)   ','Iota                   ');</script>     | Int                         | List                 | [link](builtin/range.md)      |&#10004;| Y        |
| 5  | <script>hgFunc('fact(x)    ','Factorial              ');</script>     | Real                        | E                    | [link](builtin/fact.md)       |&#10004;| Y        |
| 6  | <script>hgFunc('rand(x)    ','Random number generator');</script>     | Int                         | Same as argument     | link                          |        |         |
| 7  | <script>hgFunc('seed(x)    ','Random seed            ');</script>     | Int                         | L                    | link                          |        |         |
| 8  | <script>hgFunc('flip(x)    ','Convert a dict to a table and vice versa');</script>   | Dict, Table  | Table or Dict        | link                          |        | Y        |
| 9  | <script>hgFunc('reverse(x) ','Reverse                ');</script>     | Basic                       | Same as argument     | [link](builtin/reverse.md)    |&#10004;| Y        |
| 10 | <script>hgFunc('where(x)   ','Find all trues         ');</script>     | B                           | L                    | [link](builtin/where.md)      |&#10004;| P        |
| 11 | <script>hgFunc('group(x)   ','Group items            ');</script>     | Basic                       | List<i64>            | [link](builtin/group.md)      |&#10004;| P        |


### 3.2 Binary

| No.| Function                                                                | Type of Arg(s)\*            | Type of Value            | Details                      | Status | Parallel |
| :-:| :---------------------------------------------------------------------- | :-------------------------- | :----------------------- | :--------------------------: | :----- | :------: |
| 1  | <script>hgFunc('append(x,y)    ','Append to a list          ');</script>| (Basic, List; Basic, List)  | Basic or List            | [link](builtin/append.md)    |&#10004;| Y        |
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
| 1  | <script>hgFunc('count(x)  ','Alias of @len     ');</script>  | See @len           | See @len                      | [link](builtin/len.md)       |&#10004;|         |
| 2  | <script>hgFunc('sum(x)    ','Sum of items in x ');</script>  | Real               | Same as argument              | link                         |&#10004;| Y        |
| 3  | <script>hgFunc('avg(x)    ','sum(x) / len(x)   ');</script>  | Real               | Same as argument              | N/A                          |&#10004;|         |
| 4  | <script>hgFunc('min(x)    ','Minimum           ');</script>  | Real               | Same as argument              | [link](builtin/reduce.md#min)|&#10004;| N        |
| 5  | <script>hgFunc('max(x)    ','Maximum           ');</script>  | Real               | Same as argument              | [link](builtin/reduce.md#max)|&#10004;| N        |

### 3.4 List

| No.| Function                                                                    | Type of Arg(s)\*    | Type of Value        | Details                           | Status | Parallel |
| :-:| :-------------------------------------------------------------------------- | :------------------ | :------------------- | :-------------------------------: | :----- | :------: |
| 1  | <script>hgFunc('raze(x)              ','Raze a nested list      ');</script>| List, Basic         | Basic                | [link](builtin/list.md#raze)      |&#10004;|         |
| 2  | <script>hgFunc('enlist(x)            ','Return a list           ');</script>| Any                 | List                 | [link](builtin/list.md#enlist)    |&#10004;|         |
| 7  | <script>hgFunc('tolist(x)            ','Vector to list          ');</script>| Basic               | List                 | [link](builtin/list.md#tolist)    |&#10004;| ?(CHECKE)|
| 3  | <script>hgFunc('each\_item(fn,x)     ','Each on x with func `fn`');</script>| (Func; List)        | List                 | [link](builtin/list.md#each_item) |&#10004;| ?(CHECKE)|
| 4  | <script>hgFunc('each(fn,x,y)         ','Each on both sides      ');</script>| (Func; List; List)  | List                 | [link](builtin/list.md#each)      |&#10004;| ?(CHECKE)|
| 5  | <script>hgFunc('each\_left(fn,x,y)   ','Each on left side       ');</script>| (Func; List; List)  | List                 | [link](builtin/list.md#each_left) |&#10004;| ?(CHECKE)|
| 6  | <script>hgFunc('each\_right(fn,x,y)  ','Each on Right side      ');</script>| (Func; List; List)  | List                 | [link](builtin/list.md#each_right)|&#10004;| ?(CHECKE)|
| 7  | <script>hgFunc('match(x,y)           ','Check if both match     ');</script>| (Any; Any)          | Bool                 | link                              |&#10004;| N        |

## 4. Database related

| No.| Function                                                                | Type of Arg(s)\*            | Type of Value        | Details                      | Status | Parallel |
| :-:| :---------------------------------------------------------------------- | :---------------------------| :------------------- | :--------------------------: | :----- | :------: |
| 1  | <script>hgFunc('enum          ','enum(variable, values)   ');</script>  | (Symol;Any)                 | Enum                 | [link](builtin/liter.md#enum)|&#10004;| N        |
| 2  | <script>hgFunc('dict          ','dict(keys, values)       ');</script>  | (Any;Any)                   | Dict                 | link                         |&#10004;| ?        |
| 3  | <script>hgFunc('table         ','table(col\_names, values)');</script>  | (List;Column)               | Table                | link                         |&#10004;| ?        |
| 4  | <script>hgFunc('ktable        ','ktable(table1, table2)   ');</script>  | (Table;Table)               | KTable               | link                         |&#10004;| N        |
| 5  | <script>hgFunc('keys          ','Return keys              ');</script>  | Table, KTable, Dict         | List                 | link                         |&#10004;| ?(CHECKE)|
| 6  | <script>hgFunc('values        ','                         ');</script>  | Pending                     | Pending              | link                         |&#10004;| N        |
| 7  | <script>hgFunc('meta          ','                         ');</script>  | Pending                     | Pending              | link                         |        |         |
| 8  | <script>hgFunc('column\_value ','                         ');</script>  | Table, KTable               | Various              | link                         |&#10004;| N        |
| 9  | <script>hgFunc('load\_table   ','Load a table             ');</script>  | Symbol                      | Table, KTable        | link                         |&#10004;| N        |

## 5. Indexing

| No.| Function                                                            | Type of Arg(s)\*   | Type of Value        | Details                            | Status | Parallel |
| :-:| :------------------------------------------------------------------ | :----------------- | :------------------- | :--------------------------------: | :----- | :------: |
| 1  | <script>hgFunc('index(x,y) ','List indexing (x[y])    ');</script>  | Various            | Various              | [link](builtin/indexing.md#index)  | P      | Y        |
| 2  | <script>hgFunc('index\_a   ','List indexing assignment');</script>  | Pending            | Pending              | link                               |        |          |

## 6. Others

| No.| Function                                                             | Type of Arg(s)\*   | Type of Value        | Details                            | Status | Parallel |
| :-:| :------------------------------------------------------------------- | :----------------- | :------------------- | :--------------------------------: | :----- | :------: |
| 1  | <script>hgFunc('load\_csv ','Load a csv file             ');</script>| String             | Table                | link                               |&#10004;| N        |
| 2  | <script>hgFunc('print     ','Output                      ');</script>| Any                | Long (exit code)     | link                               |        |          |
| 3  | <script>hgFunc('format(x) ','Format output (print(str(x))');</script>| Any                | String               | link                               |        |



