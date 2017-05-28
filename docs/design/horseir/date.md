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

### date year

Description: return a year from a date

```
  d <- 2011.03.15d
  date_year(d)
> 2011:i64
```

### date month

Description: return a month from a date

```
  d <- 2011.03.15d
  date_month(d)
> 3:i64
```
### date day

Description: return a day from a date

```
  d <- 2011.03.15d
  date_month(d)
> 15:i64
```

## About time

### time

Description: extract a time from a date

```
  dt <- 2011.03.15T15.12.57.001z
  time(dt)
> 15.12.57.001t
```

### time hour

Description: return an hour from a time

```
  t <- 15.12.57.001t
  time_hour(t)
> 15:i64
```

### time minute

Description: return a minute from a time

```
  t <- 15.12.57.001t
  time_minute(t)
> 12:i64
```

### time second

Description: return a second from a time

```
  t <- 15.12.57.001t
  time_second(t)
> 57:i64
```

### time mill

Description: return a millisecond from a time

```
  t <- 15.12.57.001t
  time_mill(t)
> 1:i64
```

## datetime operations

General operations for all datetime types.

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
  datetime_add(d,x)
> 2011.03.16d
  t <- 15.12.57.001t
  x <- 1:i64
  datetime_add(t,x)
> 15.12.57.002t
```

### datetime sub

Description: substract an interval to a time

```
  d <- 2011.03.15d
  x <- 1:i64
  datetime_sub(d,x)
> 2011.03.16d
  t <- 15.12.57.001t
  x <- 2:i64
  datetime_sub(t,x)
> 15.12.56.999t
```

