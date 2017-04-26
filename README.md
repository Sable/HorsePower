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

In the summer 2017, we will start this project from scratch. We have great team
members, Hanfeng and Hongji, with the generous support from my advisor Laurie.

We plan to build the system in the first three months (12 weeks). Later, we
could have another month to improve the system and draft a paper.

## Schedule

Project settings

   - Programming languages: C/C++ 
   - Parallelism          : OpenMP/Pthread

Conventions: we will maintain reasonable amount of `conventions` to keep track of code styles.

### Hongji

Hongji is willing to assisting Hanfeng to in multiple ways.  He is assigned
with the following tasks.

1st week

    - Attend a lesson from Hanfeng about code style conventions
    - Learn IR designs from hanfeng
    - Start to build interpreters to parse IRs in different levels

2nd week

    - 

### Hanfeng

1st week

    - Advise Hongji to get started
    - Build the overall structure of the project, including documentations
    - Make decisions on the IR design, including the choices of primitives
    - Report to Laurie about the project progress


