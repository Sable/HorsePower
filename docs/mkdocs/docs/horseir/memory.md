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
Moreover, the coalescing nature of the buddy system could allocate and
deallocate memory efficiently.

## Buddy system

**Initialization**

A initial block is created for initializing systems, including a symbol pool.

**Allocation**

When a request is received, our system tries to find a "just fit" block for it.
It may result in allocating a big memory chunk for a relatively small request
(e.g. request 33, and return 64).

**Deallocation**

When a block should be deallocated, our system checks its adjacent block.  If
both are free, they could be coalesced in a larger block.  This operation could
do recursively until one side is occupied.

## Copy-on-write

When passing parameters to a UDF (User-Defined Functions) in HorseIR, it
follows the pass-by-value semantics.  A parameter in HorseIR may be a vector or
a list of vectors.  It will not be efficient if data has to be copied to UDFs
in each method invocation.  Therefore, the strategy "copy-on-write" is
important for avoiding unncessary copies.

### Technical details

**Reference count**

A leading slot in the allocated memory stores a counter to get the reference
count of the memory block.

Here is a simple example to explain how reference count works.

```no-highlight
1. A -> M0, ref 1   // allocate M0 to A, initialize reference count to 1
2. B -> A , ref 2   // B points to A, ref(M0) = 2
3. <Update B>       // replicate M0 to M1
4. <Update ref>     // update ref(M0) = 1, ref(M1) = 1
```

When a reference count goes to zero, it means that the corresponding memory
block is not used anymore and it should be freed in some way.

## Data compression

Two kinds of data compression

- Bit-vector
- Encoding

### Bit-vector

In a boolean type, there are only two states: true and false.  Since byte is
the smallest unit for resource allocation, it is convenient to use one byte to
represent the boolean type.  On the other hand, one byte consists of 8 bits.
One bit is sufficient to store the boolean information: 0 (false) and 1 (true).
It is obvious that using bits could save the required space. Moreover, it is
much faster in using bit-based operations than normal operators.

However, implementing bit-vector may significantly increase the complexity of
the system.  It is necessary to perform bit fetching and updating before the
real bit values are manipulated.


### Encoding

In a table with a large number of rows, if only a few rows are selected, the
overhead of operating on row columns is non-trivial. Inspired by sparse
techniques, we consider data transformation to make a dense vector.

**Steps**

1. Calculate selectivity in the first operation on a table column
2. If the selectivity is lower than a threshold (e.g. 1/64), goto **step 4**
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

**Compressed format**

From the bit-vector section, we know that bits could save at most 8x space.
When many zeros are found, we intend to store non-zero elements.
A basic approach is to remember the location of non-zero elements, such as:

```no-highlight
  0 0 1 0 0 1 0 ... 0  // only 2 non-zeros
> 2 5                  // store indices
```

We think 0.015625 (or 1/64) is a reasonable threshold for saving indices
instead of using bit vectors.  One index needs 8 bytes and one byte has 8 bits.
Therefore, at least 64 bits for one non-zero element are necessary for using
the compressed format.

More intuitively, we can see the example below.

```no-highlight
  x:bool = @gt(salary, 100000);
```

We try to find all recores whose salary is greater than 100K.  Presumably,
there are 1 thousand records.  If there are less than 16 records satisfy the
selection criteria, we would like to change the value of `x` to a compressed
format.




