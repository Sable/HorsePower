# Random Functions

| Name        | Description                                |
| :---------- | :----------------------------------------- |
| seed(x)     | Set random seed to x                       |
| rand(x)     | Return an integer in `[0,x)`               |
| randk(n,x)  | Return a vector with 10 randoms in `[0,x)` |

### seed

```
    @seed(16807:i32);
```

Function `seed` takes an integer and set it as a global seed.
The default value of the seed is 16807.

**Type rules**

```
Int -> i64
_   -> domain Error
```

**Shape rules**

[Monadic elementwise shape rules](../../../horseir/#monadic-elementwise)


### rand

```
    @rand(100:i32)
13:i32
```

Function `rand` takes an integer `x` as an range and returns a random integer from
0 (inclusive) to x (exclusive).

**Type rules**

```
Int -> i64
_   -> domain Error
```


**Shape rules**

[Monadic elementwise shape rules](../../../horseir/#monadic-elementwise)


### randk

```
    @randk(10:i32, 100:i32)
(13,75,45,53,21,4,67,93,38,51):i64
```

Function `randk` takes two integers `randk(n,x)`, where `n` is the number of
random numbers and `x` is as an range.  It returns a vector with `n` random
numbers within 0 (inclusive) and x (exclusive).

Implementation: [Reservoir sampling](https://en.wikipedia.org/wiki/Reservoir_sampling)

**Type rules**

```
Int, Int -> i64
_  , _   -> domain Error
```

**Shape rules**

[Shape Left Value](../../../horseir/#shape-left-value)


