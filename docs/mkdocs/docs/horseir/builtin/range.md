## range

### Description

`range(x)`

- from `0 to x-1`, inclusive

### Type rules

Pre-requisite
- Let TypeI = [bool, i16, i32, i64]; //an integer

```
TypeI -> list<i64>
_ -> domain error
```

### Examples

```
range(3)
> 0 1 2:i64
```
