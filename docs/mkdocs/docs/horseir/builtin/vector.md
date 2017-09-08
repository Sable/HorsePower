## vector

### Description

`vector(x,y)`

- initialize a vector based on y with the length x

### Type rules

```no-highlight
Int, Basic -> Basic
Int, List  -> List
_  , _     -> domain error
```

Note: `Int` on the left side must be a non-negative number.

### Examples

```no-highlight
vector(2,(-1,2,3))
> (-1,2)
vector(5,(-1,2,3))
> (-1,2,3,-1,2)
```
