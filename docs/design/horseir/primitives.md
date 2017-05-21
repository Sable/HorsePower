## Bult-in functions

- [Arithmetic](#arithmetic)
- [Logic](#logic)
- [Datetime](#datetime)
- [Trigonometric](#trig)
- [General](#general)
- [Database related](#database)

### <p id="arithmetic">Arithmetic</p>

| Function                        | Description             | Function                        | Description             |
| :------------------------------ | :---------------------- | :------------------------------ | :---------------------- |
| [abs](builtin/abs.md)           | absolute                | [lt](builtin/lt.md)             | less than               |
| [ceil](builtin/ceil.md)         | ceiling                 | [leq](builtin/leq.md)           | less equal than         |
| [floor](builtin/floor.md)       | floor                   | [gt](builtin/gt.md)             | greater than            |
| [round*](builtin/round.md)      | round                   | [geq](builtin/geq.md)           | greater equal than      |
| [conj](builtin/conj.md)         | conjugate               | [eq](builtin/eq.md)             | equal                   |
| [neg](builtin/neg.md)           | negate                  | [neq](builtin/neq.md)           | not equal               |
| [recip](builtin/recip.md)       | reciprocal              | [plus](builtin/plus.md)         | plus                    |
| [signum](builtin/signum.md)     | signum (-1,0,1)         | [minus](builtin/minus.md)       | minus                   |
| [pi](builtin/pi.md)             | pi                      | [mul](builtin/mul.md)           | multiple                |
|                                 |                         | [div](builtin/div.md)           | division                |
|                                 |                         | [power](builtin/power.md)       | power(x,k)              |
|                                 |                         | [log](builtin/log.md)           | log(b,x)                |
|                                 |                         | [mod](builtin/mod.md)           | mod(x,y)                |

### <p id="logic">Logic</p>

| Function                        | Description             |
| :------------------------------ | :---------------------- |
| [and](builtin/and.md)           | and                     |
| [or](builtin/or.md)             | or                      |
| [not*](builtin/not.md)          | not                     |
| [nand](builtin/nand.md)         | not and                 |
| [nor](builtin/nor.md)           | not or                  |

### <p id="datetime">Datetime</p>

| Function                        | Description             | Function                        | Description             |
| :------------------------------ | :---------------------- | :------------------------------ | :---------------------- |
| date                            | Date extraction         | time                            | Time extraction         |
| date_year                       | Date.year               | time_hour                       | Time.hour               |
| date_month                      | Date.month              | time_minute                     | Time.minute             |
| date_day                        | Date.day                | time_second                     | Time.second             |
|                                 |                         | time_mill                       | Time.mill               |
| date_diff                       | Date difference         | time_diff                       | Time difference         |
| date_add                        | Date plus               | time_add                        | Time plus               |
| date_sub                        | Date minus              | time_sub                        | Time sub                |

[Source, db functions](https://www.w3schools.com/sql/sql_ref_mysql.asp)

### <p id="trig">Trigonometric</p>

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
| [min*](builtin/min.md)          | min                     | [indexof*](builtin/indexof.md)  | index of                | 
| [append*](builtin/append.md)    | append                  | [take*](builtin/take.md)        | take elements           | 
| [like*](builtin/like.md)        | match                   | [drop*](builtin/drop.md)        | drop elements           |  

### <p id="database">Database related</p>

| Function                        | Description             | Function                        | Description             |
| :------------------------------ | :---------------------- | :------------------------------ | :---------------------- |
| list                            |                         | key                             |                         |
| enum                            |                         | value                           |                         |
| dict                            |                         | meta                            |                         |
