# HorsePower

HorsePower is a set of infrastructures for parsing and optimizing database queries.
It involves all phases of query compilation from source language to execution
plans.  Moreover, it provides a couple of well-designed IRs in array programming
before generating low-level code.  The IRs have multiple levels so that
sophisticated compiler optimizations for each level can be applied.
Furthermore, using array programming offers a promising option for the
fine-grained parallelism.

In HorsePower, we focus on the following parts.

    - Source language design
    - Multiple-level IR design
    - Static analysis for array-based IRs
    - Highly tuned library support
    - Query optimizations with heuristics
    - Query optimizations with data-flow analysis

## Summer 2017

In the summer 2017, we will start this project from scratch.  We have a great team
, Hanfeng and Hongji, with the strong support from Laurie.

We plan to build the system in the first three months (12 weeks). Later, we
could have another month to improve the system and draft a paper.

- Schedule
  + [Hongji](docs/plans/hongji.md)
  + [Hanfeng](docs/plans/hanfeng.md)

## Project

Figure 1 describes the workflow of the Horse framework.  A candidate for the
source language is our *Horse* language which is extended from SQL.  The Horse
language is designed for data analytics with extended SQL features and array
programming.  Moreover, we provide a front end for parsing and transforming
source code to IRs, called Horse IRs.  After the optimization phases inside IRs,
several back ends are supported, i.e. LLVM, C/C++ and interpreter.  LLVM and
C/C++ need to generate bitcode before they could output results. For sure, some
optimizations take place before bitcode is generated.  On the other hand, the
interpreter can output result directly so that it could save the compilation
time.  That means it may be faster than compiled code.

<p align="center"><img src="docs/figures/horse-flow.png" /></p>
<p align="center">Figure 1. The workflow of the Horse framework.</p>

Figure 2 introduces the design of three levels of IRs.  The workflow dependence
of IRs consists of an acyclic graph.  The workflow is described as follows.

1. Source code is converted to medium-level IR (MIR, 3-address code);
2. MIR has two options, either to Low-level IR (LIR) or to High-level IR (HIR);
3. HIR is designed for optimizations with high-level perspectives;
4. LIR is close to target code which usually is relatively low-level;
5. In each level, IR code is optimized with different optimizations.

<p align="center"><img src="docs/figures/horse-ir.png" /></p>
<p align="center">Figure 2. The design of 3 IRs.</p>

### External links

About Horse

- [HorseIR design](http://www.sable.mcgill.ca/~hanfeng.c/f17/horse_spec/) | Horse: An array programming language with Database components
- [Horse program](http://www.sable.mcgill.ca/~hanfeng.c/f17/horseir/) | Design of Horse program

Related work

- [Introduction](http://www.sable.mcgill.ca/~hanfeng.c/f17/talk2/slide.html) | A query compiler for database
- [Query compiler](http://www.sable.mcgill.ca/~hanfeng.c/f17/qcompiler/) | A paper

### Settings

- Platform       : Cross-platform
- Languages      : C/C++
- Parallelism    : OpenMP/Pthread
- Conventions    : [docs/conventions](docs/conventions)
- GitHub Issue   : [Issues](https://github.com/Sable/HorsePower/issues)

### Resources

- MonetDB : [home](https://www.monetdb.org/Home) | [reference](https://www.monetdb.org/Documentation/SQLreference)
- Q/KDB+  : [kx](kx.com) | [reference](code.kx.com)


### Chat room

[Enter here](https://gitter.im/Sable/HorsePower)

Let's rock the summer 2017!
