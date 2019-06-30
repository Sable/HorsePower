# Take and Drop Items

## take

```
    @take(3:i32, (1,2,3,4,5,6):i32)
(1,2,3):i32
    @take(-3:i32, (4,5,6):i32)
(4,5,6):i32
```

Function `take` returns the leading `n` items from a value `v`.
If `n` is greater than the length of `v`, the extra slots are filled with the
default values depending on their current types.

### Type Rules

```
Int, Any -> determined by the 2nd argument
_  , _   -> domain error
```

### Shape Rules

[shape left value](../../../horseir/#shape-left-value)


## drop

```
    @drop(3:i32, (1,2,3,4,5,6):i32)
(4,5,6):i32
    @drop(-3:i32, (4,5,6):i32)
(1,2,3):i32
```

Function `drop` drops the tailing `n` from a value `v` and returns the remaining values.
If `n` is greater or equal to the length of `v`, an empty value is returned.

### Type Rules

```
Int, Any -> determined by the 2nd argument
_  , _   -> domain error
```

### Shape Rules

[Shape Left Value](../../../horseir/#shape-left-value)

## Discussion

- Function `take` and `drop` are interchangable in some cases
    + `take(3,x)` is the same as `drop(len(x)-3,x)` if `len(x)>=3`


