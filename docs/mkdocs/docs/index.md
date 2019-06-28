# HorsePower

In order to explore a new method to improve the performance of database
queries, we conduct the research in combining array programming and
in-memory column-based database systems.
We propose *HorsePower* which aims at optimizing SQL queries with user-defined
functions (UDFs) at a level of a new array-based intermediate representative
(HorseIR).
Moreover, an extensible framework is designed for improving the throughput of
database queries with low response time.

HorsePower has the following parts:

- [HorseIR](horseir): An array-based IR for handling and optimizing queries
- [HorseSQL](sql2ir): An SQL to HorseIR translator
- [HorseSystem](system):  A system for executing HorseIR programs
- [HorseRuntime](runtime): A runtime system for In-Memory Database Systems (IMDB)


