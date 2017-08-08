# Date time

Basic built-in functions for datetime types in database systems

## About date

### date

Description: extract a date from a time

```
  dt <- 2011.03.15T15.12.57.001z
  date(dt)
> 2011.03.15d
```

Type rules

```
Z -> D
_ -> domain error
```

### date year

Description: return a year from a date

```
  d <- 2011.03.15d
  date_year(d)
> 2011:i64
```

Type rules

```
M -> L
D -> L
Z -> L
_ -> domain error
```

### date month

Description: return a month from a date

```
  d <- 2011.03.15d
  date_month(d)
> 3:i64
```

Type rules

```
M -> L
D -> L
Z -> L
_ -> domain error
```

### date day

Description: return a day from a date

```
  d <- 2011.03.15d
  date_month(d)
> 15:i64
```

Type rules

```
D -> L
Z -> L
_ -> domain error
```

## About time

### time

Description: extract a time from a date

```
  dt <- 2011.03.15T15.12.57.001z
  time(dt)
> 15.12.57.001t
```

Type rules

```
Z -> T
_ -> domain error
```

### time hour

Description: return an hour from a time

```
  t <- 15.12.57.001t
  time_hour(t)
> 15:i64
```

Type rules

```
Z -> L
U -> L
W -> L
T -> L
_ -> domain error
```

### time minute

Description: return a minute from a time

```
  t <- 15.12.57.001t
  time_minute(t)
> 12:i64
```

Type rules

```
Z -> L
U -> L
W -> L
T -> L
_ -> domain error
```

### time second

Description: return a second from a time

```
  t <- 15.12.57.001t
  time_second(t)
> 57:i64
```

Type rules

```
Z -> L
W -> L
T -> L
_ -> domain error
```


### time mill

Description: return a millisecond from a time

```
  t <- 15.12.57.001t
  time_mill(t)
> 1:i64
```

Type rules

```
Z -> L
T -> L
_ -> domain error
```


## datetime operations

General operations for all datetime types.

#### Keys

```
`year   `month   `day   `hour   `minute   `second   `mill
```

### datetime diff

Description: return the difference between two times

```
  d1 <- 2011.03.15d
  d2 <- 2011.03.16d
  datetime_diff(d1,d2)
> 1:i64
  t1 <- 15.12.57.001t
  t2 <- 15.12.57.101t
  datetime_diff(t1,t2)
> 100:i64
```

### datetime add

Description: add an interval to a time

```
  d <- 2011.03.15d
  x <- 1:i64
  datetime_add(d,x,`year:sym)
> 2012.03.15d
  t <- 15.12.57.001t
  x <- 1:i64
  datetime_add(t,x,`second)
> 15.12.58.001t
```

### datetime sub

Description: substract an interval to a time

```
  d <- 2011.03.15d
  x <- 1:i64
  datetime_sub(d,x,`year)
> 2010.03.15d
  t <- 15.12.57.001t
  x <- 2:i64
  datetime_sub(t,x,`second)
> 15.12.56.001t
```

