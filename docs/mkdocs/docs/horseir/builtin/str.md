## str

### Description

`str(x)`

- format `x`, output string

### Type rules

```no-highlight
bool   -> str
i8     -> str
i16    -> str
i32    -> str
i64    -> str
f32    -> str
f64    -> str
str    -> str
sym    -> str
month  -> str
date   -> str
dt     -> str
minute -> str
second -> str
time   -> str
_      -> str
```

**Note:** `dict`,`enum` and `table` have special string formats.

### Examples

```no-highlight
str((-1,2,3):i32)
> '-1,2,3':str   //length 6
```
