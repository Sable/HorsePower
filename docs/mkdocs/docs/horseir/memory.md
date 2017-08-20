# HorseIR memory management

## Introduction

Unlike a programming language, a database system considers more about resource
allocation in the purpose of improving its throughput.  For an In-Memory
Database system (IMDB), the memory management plays a crucial role in its
holistic system design.

In HorseIR, there is a rich set of data types (see [types](types.md)). Each
type requires various bytes. From a programming language point of view, the
"pay as you go" scheme is popular because programmers are responsible to choose
proper data structures and algorithms to reduce the overhead in utilizing
allocated memory.

We design an efficient memory system with a flexible scheme to allocate and
deallocate memory for HorseIR runtime execution. We implement a classic buddy
system which is grabage collector free.  For instance, if a request with 25
bytes is received, the system will allocate 32 bytes instead of exact 25 bytes.
Moreover, the coalescing nature of the buddy system could efficiently
deallocate dead blocks and allocate available memory.

## Buddy system

### Allocation

### Deallocation


## Copy-on-write

When passing parameters to a UDF (User-Defined Functions) in HorseIR, it
follows the pass-by-value semantics.  A parameter in HorseIR may be a vector or
a list of vectors.  It will not be efficient if data has to be copied to UDFs
in each method invocation.  Therefore, the strategy "copy-on-write" is
important for avoiding unncessary copies.

### Technical details


## Data compression

Two kinds of data compression

- Bit-vector
- Encoding

### Bit-vector

/* write docs here */

### Encoding

In a table with a large number of rows, if only a few rows are selected, the
overhead of operating on row columns is non-trivial. Inspired by sparse
techniques, we consider data transformation to make a dense vector.

**Steps**

1. Calculate selectivity in the first operation on a table column
2. If the selectivity is lower than a threshold (e.g. 0.02), goto **step 4**
3. Perform normal operations, goto **step 5**
4. Encode vectors and pass encoding types
5. Get the result of computation

**Ripple effect**

An encoding type can be propagated through valid operations implicitly.
For example,

```no-highlight
x:i64 = ... ;        // an encoding i64 type
y:i64 = @plus(x, 1); // y has the same type as x
```

Hence, built-in functions should support encoding types as well. There is
probably a need to convert an encoding type to a normal type.  In a table, if
system memory is sufficient, the column with the encoding type should be
expanded to a regular column so that the performance of queries can be
improved.


