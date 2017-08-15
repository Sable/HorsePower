## len

### Description

`len(x)`

- if x is a table, return the number of rows of the table
- else, return the length of x

### Type rules

```no-highlight
_ -> i64
```

### Examples

```no-highlight
len((-1,2,3))
> 3:i64
```
