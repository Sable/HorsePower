# Programming Languages (PL)

### Overview

Optimizations for HorseIR: `Static + Dynamic`

### Static Analysis

Intra-procedural optimizations

- Constant propagation and foldering (CPF)
- Common Sub-expression Elimination (CSE)
- Peephole optimizations
- Program slicing

[Inter-procedural optimizations](https://en.wikipedia.org/wiki/Interprocedural_optimization)

- <red>Procedural inlining</red>
- <red>Inter-procedural dead code elimination</red>
- <red>Inter-procedural constant propagation</red>
- <red>Procedure reordering</red>

Others (before actual optimizations)

- A framework for optimizing HorseIR code (In progress)
- <red>Alias analysis</red>
- <red>Array access analysis</red>

<small>*<red>red</red> means not implemented yet.</small>

### Dynamic Analysis

The main goal is to provide a framework of efficient JIT-based optimizations to
generate efficient code on-the-fly.  For HorseIR, loop fusion based
optimizations are particularlly critical in many cases.  Many prior studies
have shown that loop fusion based optimizations could be NP-hard to find an
optimal solution.  Therefore, we intend to explore a couple of heuristics as
candidates for aggregating HorseIR primitive functions.

### Further Opportunities

The list of collected compiler optimizations: [read more](https://en.wikipedia.org/wiki/Optimizing_compiler)

