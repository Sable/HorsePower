# where

### Description

`@where(x)`

- return the indices of elements which point to the 'true' values in x

### Type Rules

```no-highlight
bool -> i64
_    -> domain error
```

!!! Note "Extension"
    (Deprecated) {--*Treat a list&lt;bool&gt; as a boolean matrix, then return two
    vectors which contain the x- and y-index of true values separately.*--}

### Examples

```no-highlight
    @where((1,0,1,0):bool)
(0,2):i64
```
