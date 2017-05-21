## nand

### Description

`nand(x,y)`

- $x \barwedge y$, return boolean

### Type rules

```
bool,bool -> bool
_ -> domain error
```

### Examples

```
nand(0 0 1, 0)
> 1 1 1:bool
```
