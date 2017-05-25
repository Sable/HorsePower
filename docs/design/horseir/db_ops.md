## Implementing database operations using array operations

Reference:

- **Zhou**, Jingren and Kenneth A. Ross. “Implementing database operations
  using SIMD instructions.” SIGMOD Conference (2002).

### Scan-like operators

Problem description:

Supposedly, we have two vectors `x` {x1,x2,...,xn} and `y` {y1,y2,...,ym},
where `n` and `m` are the number of elements in `x` and `y` respectively. A
match operation is to find out the same element `y[i]` in `x`.  There are a
couple of senarios we should consider as follows.

**Return the first match**

The values of `x` elements should be distinct.

```
t0:list<i32> = index_of(x,y);
```

For example,

```
   x <- 25 36 15
   y <- 36 17 25
   index_of(x,y)
> 1 3 0
```

We see

- y[0] (36) is the 2nd element in `x` (return 1)
- y[1] (17) is not in `x` (return 3, the length of `x`)
- y[2] (25) is the 1st element in `x` (return 0)

**Return all matches**

```
t0:list<list<bool>> = outer(eq,x,y);
t1:list<list<i32>>  = each(where,t0);
```

For example,

```
   x <- 25 36 36
   y <- 36 17 25
   t0 <- outer(eq,x,y)
> (0 1 1;0 0 0;1 0 0)
   each(where, t0);
> (1 2; ;0)
```

We see

- `(0 1 1;0 0 0;1 0 0)` is the result of comparing (`eq`) `x` and `y` in an
  outer-product way
- `each(where, t0)` operates `where` in the each list of `t0`

### Aggregation




