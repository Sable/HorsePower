## str

### Description

`str(x)`

- format `x`, output string

### Type rules

```no-highlight
bool -> string (y)
i16  -> string (y)
i32  -> string (y)
i64  -> string (y)
f32  -> string (y)
f64  -> string (y)
char -> string (y)
sym  -> string (y)
m    -> string
d    -> string
z    -> string
u    -> string
v    -> string
t    -> string
_    -> string
```

**Note:** `dict`,`enum` and `table` have special string formats.

### Examples

```no-highlight
str((-1,2,3))
> '-1,2,3':str   //length 6
```
