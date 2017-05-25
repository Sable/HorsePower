
## Important database operations

A couple of important database operations are listed as follows.

- [Join](#join)
- [Order](#order)
- [Group by](#group_by)
- [Limit](#limit)

### Join

A join may have various kinds of joins which can be implemented with different
efficient algorithms.  We consider the following joins which are included in
our implementation.  By default, a typical hash join algorithm is implemented.

SQL joins ([StackOverflow](https://stackoverflow.com/questions/5706437/whats-the-difference-between-inner-join-left-join-right-join-and-full-join))

- Nature join
- Inner join (Equi-join)
- Outer join
- Outer join less
- Left join
- Left join less
- Right join
- Right join less

### Order

There are a couple of built-in functions designed for handling ordering
problem.  If only one order condition is specified, either `asc` or `desc` is
used.  Otherwise, a more general function `sort` is used for multiple-column
sorts.

For example, `ORDER BY salary DESC` can be represented as follows.

```
// t0 is a partial or full set of the salary column
t1:list<i32> = desc(t0);
```

A list of integers (indices) is returned and saved into `t1`;

### Group by

This clause aggregates items with the same value.

For example, `GROUP BY lastname`

```
// t0 is a partial or full set of the lastname column
t1:list<list<i32>> = group_by(t0);
```

The returned `t1` is a list of lists.  The internal list has integers (indices)
which point to their original position in the list `t0`.

### <span id="limit">Limit / Top</span>

Limit (or top) is useful in selecting a limited amount of items out of a long list.

For example, `LIMIT 10` takes the first 10 elements.

```
// t0 is a partial or full set of the column
t1:list<type> = take(t0, 10:i32);
```

Another similar function, `limit_range`, can select items within a range `(m,n)`.

```
// t0 is a partial or full set of the column
t1:list<i64> = substract(n,m);   // range size
t2:list<i64> = range(t0);        // iota(t0)
t3:list<i64> = add(t2, m);       // addition
t4:list<type> = index(t0, t3);   // indexing
```



