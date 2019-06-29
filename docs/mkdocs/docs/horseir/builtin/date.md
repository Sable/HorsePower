# A Collection of time Functions

!!! note "Date Time"
    Basic built-in functions for datetime types in database systems.

| Type Element        | Range                                 |
|:--------------------|:--------------------------------------|
| Year                | [1990, 2100]                          |
| Month               | [01, 12]                              |
| Day (non-leap year) | {31,28,31,30,31,30,31,31,30,31,30,31} |
| Day (leap year)     | {31,29,31,30,31,30,31,31,30,31,30,31} |
| Hour                | [00,23]                               |
| Minute              | [00,59]                               |
| Second              | [00,59]                               |
| Millisecond         | [000,999]                             |

### date

Description: extract a date from a time

```no-highlight
  x = 2011.03.15T15:12:57.001:dt
  @date(x)
> 2011.03.15:date
```

Type Rules

```no-highlight
dt -> date
_  -> domain error
```

### date year

Description: return a year from a date

```no-highlight
  d <- 2011.03.15d
  @date_year(d)
> 2011:i16
```

Type Rules

```no-highlight
month -> i16
date  -> i16
dt    -> i16
_     -> domain error
```

### date month

Description: return a month from a date

```no-highlight
    x = 2011.03.15:date;
    @date_month(x)
3:i16
```

Type Rules

```no-highlight
month -> i16
date  -> i16
dt    -> i16
_     -> domain error
```

### date day

Description: return a day from a date

```no-highlight
    x = 2011.03.15:date
    @date_month(x)
15:i16
```

Type Rules

```no-highlight
date -> i16
dt   -> i16
_    -> domain error
```

## About Time

### time

Description: extract a time from a date

```no-highlight
    x = 2011.03.15T15:12:57.001:dt
    @time(x)
15:12:57.001:t
```

Type Rules

```no-highlight
dt -> time
_  -> domain error
```

### time hour

Description: return an hour from a time

```no-highlight
    x = 15:12:57.001:time
    @time_hour(x)
15:i16
```

Type Rules

```no-highlight
dt     -> i16
minute -> i16
second -> i16
time   -> i16
_      -> domain error
```

### time minute

Description: return a minute from a time

```no-highlight
    x = 15:12:57.001:time
    @time_minute(x)
12:i16
```

Type Rules

```no-highlight
dt     -> i16
minute -> i16
second -> i16
time   -> i16
_      -> domain error
```

### time second

Description: return a second from a time

```no-highlight
    x = 15:12:57.001:time
    @time_second(x)
57:i16
```

Type Rules

```no-highlight
dt     -> i16
second -> i16
time   -> i16
_      -> domain error
```


### time mill

Description: return a millisecond from a time

```no-highlight
    x = 15:12:57.001:time
    @time_mill(x)
1:i64
```

Type Rules

```no-highlight
dt   -> i16
time -> i16
_    -> domain error
```


## datetime operations

General operations for all datetime types.

### Keys

```no-highlight
`year   `month   `day   `hour   `minute   `second   `mill
```

### datetime add

Description: add an interval to a time

```no-highlight
    d = 2011.03.15:date
    x = 1:i64
    @datetime_add(d,x,`year:sym)
2012.03.15:date
    t = 15.12.57.001:time
    x = 1:i64
    @datetime_add(t,x,`second)
15.12.58.001:time
```

Type rules

```
dt, Int, sym -> <determined by the first parameter>
_            -> domain error
```

Shape rules

- Return the same shape as the first parameter

### datetime sub

Description: substract an interval to a time

```no-highlight
    d = 2011.03.15:date
    x = 1:i64
    @datetime_sub(d,x,`year)
2010.03.15:date
    t = 15:12:57.001:time
    x = 2:i64
    @datetime_sub(t,x,`second)
15:12:56.001:time
```

Type rules

```
dt, Int, sym -> <determined by the first parameter>
_            -> domain error
```

Shape rules

- Return the same shape as the first parameter
