# Implementation of HorseIR

## Philosophy

HorseIR is a list based system with efficient C/C++ implementation.  Initially,
it is an interpreter which executes HorseIR in a naive way.  Later, JIT
compilation techniques will be added to improve its performance.

It is endless if we plan to implement all primitive functions for a database
system.  Instead, we design and implement a set of core primitive functions.
With these primitive functions, we could build other array-based primitives
efficiently.

See more about [primitives](primitives.md).

## Architecture

### Memory management

Goals

- Efficient memory management system
- [Buddy system](https://en.wikipedia.org/wiki/Buddy_memory_allocation)
- [Fibonacci heap](https://en.wikipedia.org/wiki/Fibonacci_heap)

Special cases

- No GC
- A table requires addtional memory for adding new records
  + Two possible solutions: 2^n or a variant of fibonacci number

See more about [memory management](memory.md).

## Discussions

### Fibonacci numbers

Some numbers:

```
f(0) = 64;      //64MB
f(1) = 128;     //128MB
...
f(7) = 1344;    //~1GB
...
f(11)= 9216;    //~10G
...
f(16)= 102208;  //~100G
f(17)= 165376;  //~165G
```

### SSA based code


