# join

All about database join operations.

## join_index

### Description

```no-highlight
z:? = @join_index(fn, x, y);
```

- `fn` is a comparable function, (e.g. equi-join, fn=@eq)
- Input with two vectors or lists
- Output with a integer list (`list<i64>`) with two integer vectors

### Type Rules

```no-highlight
fn, Real   , Real    -> list<i64>
fn, complex, complex -> list<i64>
fn, month  , month   -> list<i64>
fn, date   , date    -> list<i64>
fn, dt     , dt      -> list<i64>
fn, minute , minute  -> list<i64>
fn, second , second  -> list<i64>
fn, time   , time    -> list<i64>
fn, list<A>, list<A> -> list<i64>
fn, list<?>, list<A> -> list<i64>
fn, list<A>, list<?> -> list<i64>
fn, list<?>, list<?> -> list<i64>
_ , _      , _     -> domain error
```

### Examples

- It is implemented with the combination of `@outer` and `@where`

