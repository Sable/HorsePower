# Date time

Basic built-in functions for datetime types in database systems.

## Limit

The ranges of date and time are defined as follows.

### Date range

- Year: [1900, 2100]
- Month: [01, 12]
- Day (non-leap year): {31,28,31,30,31,30,31,31,30,31,30,31}
- Day (leap year): {31,29,31,30,31,30,31,31,30,31,30,31}

### Time range

- Hour: [00,23]
- Minute: [00,59]
- Second: [00,59]
- Millisecond: [000,999]

## About date

### date

Description: extract a date from a time

```no-highlight
  dt <- 2011.03.15T15:12:57.001z
  date(dt)
> 2011.03.15d
```

Type rules

```no-highlight
Z -> D
_ -> domain error
```

### date year

Description: return a year from a date

```no-highlight
  d <- 2011.03.15d
  date_year(d)
> 2011:i64
```

Type rules

```no-highlight
M -> L
D -> L
Z -> L
_ -> domain error
```

### date month

Description: return a month from a date

```no-highlight
  d <- 2011.03.15d
  date_month(d)
> 3:i64
```

Type rules

```no-highlight
M -> L
D -> L
Z -> L
_ -> domain error
```

### date day

Description: return a day from a date

```no-highlight
  d <- 2011.03.15d
  date_month(d)
> 15:i64
```

Type rules

```no-highlight
D -> L
Z -> L
_ -> domain error
```

## About time

### time

Description: extract a time from a date

```no-highlight
  dt <- 2011.03.15T15:12:57.001z
  time(dt)
> 15:12:57.001:t
```

Type rules

```no-highlight
Z -> T
_ -> domain error
```

### time hour

Description: return an hour from a time

```no-highlight
  t <- 15:12:57.001:t
  time_hour(t)
> 15:i64
```

Type rules

```no-highlight
Z -> L
U -> L
W -> L
T -> L
_ -> domain error
```

### time minute

Description: return a minute from a time

```no-highlight
  t <- 15:12:57.001:t
  time_minute(t)
> 12:i64
```

Type rules

```no-highlight
Z -> L
U -> L
W -> L
T -> L
_ -> domain error
```

### time second

Description: return a second from a time

```no-highlight
  t <- 15:12:57.001:t
  time_second(t)
> 57:i64
```

Type rules

```no-highlight
Z -> L
W -> L
T -> L
_ -> domain error
```


### time mill

Description: return a millisecond from a time

```no-highlight
  t <- 15:12:57.001:t
  time_mill(t)
> 1:i64
```

Type rules

```no-highlight
Z -> L
T -> L
_ -> domain error
```


## datetime operations

General operations for all datetime types.

#### Keys

```no-highlight
`year   `month   `day   `hour   `minute   `second   `mill
```

### datetime diff

Description: return the difference between two times

```no-highlight
  d1 <- 2011.03.15d
  d2 <- 2011.03.16d
  datetime_diff(d1,d2)
> 1:i64
  t1 <- 15:12:57.001:t
  t2 <- 15:12:57.101:t
  datetime_diff(t1,t2)
> 100:i64
```

### datetime add

Description: add an interval to a time

```no-highlight
  d <- 2011.03.15d
  x <- 1:i64
  datetime_add(d,x,`year:sym)
> 2012.03.15d
  t <- 15.12.57.001:t
  x <- 1:i64
  datetime_add(t,x,`second)
> 15.12.58.001:t
```

### datetime sub

Description: substract an interval to a time

```no-highlight
  d <- 2011.03.15d
  x <- 1:i64
  datetime_sub(d,x,`year)
> 2010.03.15d
  t <- 15:12:57.001:t
  x <- 2:i64
  datetime_sub(t,x,`second)
> 15:12:56.001:t
```

