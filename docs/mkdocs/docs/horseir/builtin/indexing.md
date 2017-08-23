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
