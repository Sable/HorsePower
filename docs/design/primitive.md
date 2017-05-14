## Bult-in functions

- [Monadic](#monadic) : `z <- foo(x)`
  + [Arithmatic](#arithmatic1)
  + [Algebraic](#algebraic)
- [Dyadic](#dyadic) : `z <- foo(x,y)`
  + [Arithmatic](#arithmatic2)
  + [Logic](#logic2)
- [Niladic](#niladic) : `z <- foo()`

### Monadic

<p id="arithmatic1">Arithmatic</p>

| Function                        | Description             |
| :------------------------------ | :---------------------- |
| [abs](builtin/m/abs.md)         | absolute                |
| [ceil](builtin/m/ceil.md)       | ceiling                 |
| [conj](builtin/m/conj.md)       | conjugate               |
| [floor](builtin/m/floor.md)     | floor                   |
| [neg](builtin/m/neg.md)         | negate                  |
| [recip](builtin/m/neg.md)       | reciprocal              |
| [signum](builtin/m/signum.md)   | signum (-1,0,1)         |


<p id="algebraic">Algebraic</p>

| Function                        | Description             |
| :------------------------------ | :---------------------- |
| [asc](builtin/m/asc.md)         | ascending order         |
| [desc](builtin/m/desc.md)       | descending order        |
| [unique](builtin/m/unique.md)   | unique values           |

### Dyadic

<p id="arithmatic2">Arithmatic</p>

| Function                        | Description             |
| :------------------------------ | :---------------------- |
| [lt](builtin/d/lt.md)           | less than               |
| [leq](builtin/d/leq.md)         | less equal than         |
| [gt](builtin/d/gt.md)           | greater than            |
| [geq](builtin/d/geq.md)         | greater equal than      |
| [eq](builtin/d/eq.md)           | equal                   |
| [neq](builtin/d/neq.md)         | not equal               |
| [plus](builtin/d/plus.md)       | plus                    |
| [minus](builtin/d/minus.md)     | minus                   |
| [mul](builtin/d/mul.md)         | multiple                |
| [div](builtin/d/div.md)         | division                |

<p id="logic2">Logic</p>

| Function                        | Description             |
| :------------------------------ | :---------------------- |
| [and](builtin/d/and.md)         | and                     |
| [or](builtin/d/or.md)           | or                      |
| [nand](builtin/d/nand.md)       | not and                 |
| [nor](builtin/d/nor.md)         | not or                  |