# Notes for TPC-H Queries

## Individual

### q1

- `q1_combine_0`: most expensive

Improved 

- late materialization: `compress` is delayed after group by

### q2

- LINE 36: solved
    + `q2_new_0`: merge two conditions with 'eq' and 'like' respectively
    + Add `LIKE2Q` and `LIKE2S` when processing single string matching
- LINE 61 & 87: UNSOLVED
    + Both are the operation: pfnJoinIndex

### q3

- Profiled three lines: 26, 36, 41

### q4

- One enumeration: key(1500000) and fkey(6001215)

Think: Does hyper pre-compute a hash table for the key?

### q5

Think: maybe the LINE 47 is unnecessary; check its HorseIR code

See also: q7,q8,q9,q10,q11,q17,q18

### q6

Think: possible with changing i64 to i32; <need to be solved>

### q7

Think: maybe the LINE 62 is unnecessary; check its HorseIR code


### q12

- Hoisting getSymbol helps improve its performance and makes its performance
  close to HyPer.
- New pattern: exp/q12.h:q12_new_0

### q13

- LINE 5: pfnLike is super slow

```sql
-- output: 1533923
-- note: table orders only has row: 1500000
select
    count(*)
from
    customer left outer join orders on
        c_custkey = o_custkey
    and o_comment not like '%special%requests%'
```

### q14

- Improve `pfnJoinIndex` by considering when |left| > |right|
    + Solution: swap both sides before sending to the core computation, and
      later swap back after the result is returned (i.e. swap2(z))

### q15

- A runtime error needs to be fixed when printing result
- LINE 6: ph-0
- LINE 9: pfnGroup

### q18

- LINE  8: most expensive
- LINE 36: expensive

See debug/q18.log

### q20

Think:

- Need to improve the performance of group in LINE 35
- Maybe check HorseIR code to see if the group operation is necessary

### q21

### q22

Happy

## Issues

### Hoist `getSymbol("string")` from a Loop

Influence:

- q8
- q12
- q16
- q17
- q19

