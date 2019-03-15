!!! danger "Database related"
    A collection of database functions and operations. (Currently not maintained)

### <span id="ref">Database Functions</span>

MySQL functions

| MySQL functions      | HorseIR builtins |  Description                                               |
| :--------------------| :----------------| :----------------------------------------------------------|
| date_add             | date_add         | Returns the new date after given an interval               |
| date_sub             | date_sub         | Returns the difference in days between two date values     |
| NOW                  | time_stamp       | `2014-11-22 12:45:34`                                      |
| CURDATE              |                  | `2014-11-22`                                               |
| CURTIME              |                  | `12:45:34`                                                 |

See more about [MySQL reference](https://www.w3schools.com/sql/func_date_sub.asp).


### <span id="clause">Important Database Operations</span>

A couple of important database operations are listed as follows.

- [Join](#join)
- [Order](#order)
- [Group by](#group-by)
- [Limit](#limit)

#### Join

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

#### Order

There are a couple of built-in functions designed for handling ordering
problem.  If only one order condition is specified, either `asc` or `desc` is
used.  Otherwise, a more general function `sort` is used for multiple-column
sorts.

For example, `ORDER BY salary DESC` can be represented as follows.

```no-highlight
// t0 is a partial or full set of the salary column
t1:i64 = desc(t0);
```

A list of integers (indices) is returned and saved into `t1`;

#### Group by

This clause aggregates items with the same value.

For example, `GROUP BY lastname`

```no-highlight
// t0 is a partial or full set of the lastname column
t1:list<i64> = @group_by(t0);
```

The returned `t1` is a list of lists.  The internal list has integers (indices)
which point to their original position in the list `t0`.

#### <span id="limit">Limit / Top</span>

Limit (or top) is useful in selecting a limited amount of items out of a long list.

For example, `LIMIT 10` takes the first 10 elements.

```no-highlight
// t0 is a partial or full set of the column
// @take is a primitive function
t1:? = @take(t0, 10:i32);
```

Another similar function, `limit_range` for range queries, can select items
within a range `(m,n)`.

```no-highlight
// t0 is a partial or full set of the column
t1:i64 = @substract(n,m);  // range size
t2:i64 = @range(t0);       // iota(t0)
t3:i64 = @add(t2, m);      // addition
t4:?   = @index(t0, t3);   // indexing
```

### <span id="operations">Implementing Database Operations Using Array Operations</span>

References:

- **Zhou**, Jingren and Kenneth A. Ross. “Implementing database operations
  using SIMD instructions.” SIGMOD Conference (2002).
- **Govindaraju**, Naga K., Brandon Lloyd, Wei Wang, Ming C. Lin and Dinesh
  Manocha. “Fast computation of database operations using graphics processors.”
  SIGGRAPH Courses (2004).


#### Scan-like Operators

Problem description:

Supposedly, we have two vectors `x` {x1,x2,...,xn} and `y` {y1,y2,...,ym},
where `n` and `m` are the number of elements in `x` and `y` respectively. A
match operation is to find out the same element `y[i]` in `x`.  There are a
couple of senarios we should consider as follows.

**Return the first match**

The values of `x` elements should be distinct.

```no-highlight
t0:list<i32> = index_of(x,y);
```

For example,

```no-highlight
   x:i64 = 25 36 15
   y:i64 = 36 17 25
   t0:list<i64> = @index_of(x,y);
   t1:i64       = @print(t0);
> 1 3 0
```

We see

- y[0] (36) is the 2nd element in `x` (return 1)
- y[1] (17) is not in `x` (return 3, the length of `x`)
- y[2] (25) is the 1st element in `x` (return 0)

**Return all matches**

```no-highlight
t0:list<bool> = outer(@eq,x,y);
t1:list<i64>  = each(@where,t0);
```

For example,

```no-highlight
     x:i32         = 25 36 36;
     y:i32         = 36 17 25;
     t0:list<bool> = @outer(@eq,x,y);
(0 1 1;0 0 0;1 0 0)
     t1:list<i64>  = @each(@where, t0);
     t2:i64        = @print(t1);
(1 2; ;0)
```

We see

- `(0 1 1;0 0 0;1 0 0)` is the result of comparing (`eq`) `x` and `y` in an
  outer-product way
- `each(where, t0)` operates `where` in the each list of `t0`

#### Aggregation

Basic operations for aggregation

- count
- min and max
- sum and avg

We provide a general function `reduce` to aggregate a list `T` by an
aggregation function `f`.  Its syntax is `reduce(@f, T)`.

For example,

```no-highlight
   t0:list<i64> = (1 1 1;2 2);
   t1:list<i64> = @reduce(@count, t0);
   t2:i64       = @print(t1);
> (3;2)
```

Aggregation is a perfect candidate for data parallelism despite it may take
different aggregation functions.


### <span id="examples">Examples for database operations</span>

#### GROUP BY

[Source](https://www.w3schools.com/sql/sql_groupby.asp)

Table

```no-highlight
CustomerID   : i32
CustomerName : str
ContactName  : str
Address      : str
City         : str
PostalCode   : str
Country      : sym
```

SQL

```sql
SELECT COUNT(CustomerID), Country
FROM Customers
GROUP BY Country
ORDER BY COUNT(CustomerID) DESC;
```

HorseIR

```
modeul _default{
    c0:dict<sym,i32> = column(Customers:table, `CustomerID:sym);
    c1:dict<sym,str> = column(Customers:table, `CustomerName:sym);
    c2:dict<sym,str> = column(Customers:table, `ContactName:sym);
    c3:dict<sym,str> = column(Customers:table, `Address:sym);
    c4:dict<sym,str> = column(Customers:table, `City:sym);
    c5:dict<sym,str> = column(Customers:table, `PostalCode:sym);
    c6:dict<sym,sym> = column(Customers:table, `Country:sym);

    t0:list<i32> = value(c6);
    t1:list<i32> = unique(t0);
    t2:list<list<i32>,list<i32>> = outer(`equal:fun, t1, t0);
    t3:list<i32> = reduce(`plus:fun, t2);
    t4:list<i32> = reduce(`plus:fun, t2);
    t5:list<i32> = desc(t4);

    c7:dict<sym,i32> = dict(`count_customerid:sym;, t3); //group by
    c8:dict<sym,i32> = c7[t5];
    c9:dict<sym,sym> = c6[t5];

    z0:list<?> = list(c8,c9);
    z:table = createTable(z0);
}
```

#### ORDER BY

[Source](https://www.w3schools.com/sql/sql_orderby.asp)

*Note*: the same table `Customers` in [GROUP BY](#group_by).

SQL

```sql
SELECT * FROM Customers
ORDER BY Country ASC, CustomerName DESC;
```

HorseIR

```no-highlight
modeul _default{
    c0:dict<sym,i32> = column(Customers:table, `CustomerID:sym);
    c1:dict<sym,str> = column(Customers:table, `CustomerName:sym);
    c2:dict<sym,str> = column(Customers:table, `ContactName:sym);
    c3:dict<sym,str> = column(Customers:table, `Address:sym);
    c4:dict<sym,str> = column(Customers:table, `City:sym);
    c5:dict<sym,str> = column(Customers:table, `PostalCode:sym);
    c6:dict<sym,sym> = column(Customers:table, `Country:sym);

    t0:list<i32> = value(c6);
    t1:list<str> = value(c1);
    t2:list<?>   = list(t0, t1);
    t3:list<bool>= list(1:bool, 0:bool); // ASC:true; DESC: false
    t4:list<i32> = order(t2, t3);

     c7:dict<sym,i32> = c0[t4];
     c8:dict<sym,i32> = c1[t4];
     c9:dict<sym,i32> = c2[t4];
    c10:dict<sym,i32> = c3[t4];
    c11:dict<sym,i32> = c4[t4];
    c12:dict<sym,i32> = c5[t4];
    c13:dict<sym,i32> = c6[t4];

    z0:list<?> = list(c7,c8,c9,c10,c11,c12,c13);
    z:table = createTable(z0);
}
```

ELI code for `order by`

```no-highlight
    x <- 1 2 3 2 2
    y <- 5 3 7 1 2
    <x,[1.5]y
1 4 5 2 3
    x[1 4 5 2 3]
1 2 2 2 3
    y[1 4 5 2 3]
5 1 2 3 7
```

#### MIN and MAX

[Definition of MIN and MAX](https://www.w3schools.com/sql/sql_min_max.asp)

Table *Products*

```no-highlight
ProductID   : i32
ProductName : str
SupplierID  : i32
CategoryID  : i32
Unit        : str
Price       : f64
```

SQL

```sql
SELECT MIN(Price) AS SmallestPrice
FROM Products;
```

HorseIR

```no-highlight
modeul default{
    import Builtin.*;
    def main():table{
        c0:i32 = column(Products:table, `ProductID:sym);
        c1:str = column(Products:table, `ProductName:sym);
        c2:i32 = column(Products:table, `SupplierID:sym);
        c3:i32 = column(Products:table, `CategoryID:sym);
        c4:str = column(Products:table, `Unit:sym);
        c5:f64 = column(Products:table, `Price:sym);

        t0:f64 = min(c5);

        z0:sym = `SmallestPrice:sym;
        z1:list<f64> = list(t0)
        z:table = @table(z0, z1); 
        return z;
    }
}
```



