# Array indexing

## Index

### Description

`index(x,k)`

- `x[k]`: fetch value from x based on k

### Type rules

Basic types

```no-highlight
B, (Int|List<Int>) -> (B|List<B>)
H, (Int|List<Int>) -> (H|List<H>)
I, (Int|List<Int>) -> (I|List<I>)
L, (Int|List<Int>) -> (L|List<L>)
F, (Int|List<Int>) -> (F|List<F>)
E, (Int|List<Int>) -> (E|List<E>)
X, (Int|List<Int>) -> (E|List<X>)
C  (Int|List<Int>) -> (E|List<C>)
Q  (Int|List<Int>) -> (E|List<Q>)
S  (Int|List<Int>) -> (E|List<S>)
M  (Int|List<Int>) -> (E|List<M>)
D  (Int|List<Int>) -> (E|List<D>)
Z  (Int|List<Int>) -> (E|List<Z>)
U  (Int|List<Int>) -> (E|List<U>)
W  (Int|List<Int>) -> (E|List<W>)
T  (Int|List<Int>) -> (E|List<T>)
```


### Examples

```no-highlight
index((-1,2,3),(2,1))
> (3,2):i64
```

## IndexA

`indexa(x,k,m)`

- is equivalent to `x[k]=m`
- returns the same type of m

### Type rules

Basic types (x,k,m -> m)

```no-highlight
B, Int , B
H, Int , (B|H)
I, Int , (B|H|I)
L, Int , (B|H|I|L)
F, Int , (B|H|I|L|F)
E, Int , (B|H|I|L|F|E)
X, Int , (B|H|I|L|F|E|X)
C, Int , C
Q, Int , Q
S, Int , S
M, Int , M
D, Int , D
Z, Int , Z
U, Int , U
W, Int , W
T, Int , T
```

**Note:**

- Think about indexing with *lists*.
- IndexA can't be parallelized because of the duplicated values of the array indices

### Examples

```no-highlight
    x = (-1,2,3)
    indexa(x,(2,1),(0))
> (0)
    x
> (-1,0,0)
```

