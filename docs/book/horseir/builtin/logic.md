# Logic

All logic operations have the following type rules.

```
// dyadic
bool,bool -> bool
_ -> domain error

// monadic
bool -> bool
_ -> domain error
```

## and

`and(x,y)`

```
      and(0 0 1, 0)
0 0 0:bool
```

## nand

`nand(x,y)`

```
      nand(0 0 1, 0)
> 1 1 1:bool
```

## or

`or(x,y)`

```
      or(0 0 1, 0)
> 0 0 1:bool
```

## nor

`nor(x,y)`

```
      nor(0 0 1, 0)
> 1 1 0:bool
```

## not

`not(x)`

```
      not(0 0 1:bool)
> 1 1 0:bool
```

## xor

`xor(x,y)`

```
      xor(0 0 1,0)
> 0 0 1:bool
```

