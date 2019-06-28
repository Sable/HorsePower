# str

### Description

```no-highlight
@str(x)
```

- input with the value `x`, output string
- default format: `json`

### Type Rules

```no-highlight
Any -> str
_   -> domain error
```

### Examples

```no-highlight
    @str((-1,2,3):i32)
"-1,2,3":str
```
