# HorsePower

In order to explore a new way for improving the performance of database
queries, we conduct the research in combining array programming languages and
database systems.   Our project, *HorsePower*, aims at optimizing standard SQL
queries at a level of an array-based intermediate representative (IR).  An
extensible framework is designed for improving the throughput of database
queries while keeping response time as low as possible.

HorsePower consists of several parts:

- [HorseIR](horseir/spec.md): An array-based IR for handling and optimizing queries
- **HorseSQL**: A translator for generating HorseIR from standard SQL
- **HorseDB**: A customized database system

