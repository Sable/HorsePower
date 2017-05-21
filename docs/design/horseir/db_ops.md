
## Important database operations

A couple of important database operations are listed as follows.

- Join
- Order
- Group by
- Limit

### Join

A join may have various kinds of joins which can be implemented with different
efficient algorithms.  We consider the following joins which are included in
our implementation.  By default, a typical hash join algorithm is implemented.

1. Nature join
1. Equi-join
1. Left-join
1. Right-join


### Order

There are two built-in functions designed for handling ordering problem.  If
only one order condition is specified, either `asc` or `desc` is used.
Otherwise, a more general function `order` should be used for various
conditions.

For example,

???


### Group by

Example: ???


### Limit

Example: ???
