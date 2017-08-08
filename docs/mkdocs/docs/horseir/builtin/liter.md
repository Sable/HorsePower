# Literals

## Enum

An enumeration consists of two parts: a list of unique items (denoted **P**)
and a list of non-unique items (denoted **F**).  Any item in F can be found in
P.

### Literal example

```
pValue = `chen`hendren`lavoie;
fValue = `chen`chen`lavoie`hendren;
eItem  = enum(`pValue, `fValue);
```

Description

- `pValue` stores three unique names
- `fValue` enumerates the values from `pValue`
- `eItem` saves the enum created by the function `enum`

Expected results

```
`pValue: 0 0 2 1
```


