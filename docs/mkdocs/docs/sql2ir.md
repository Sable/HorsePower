# Translating SQL to HorseIR

## TPC-H benchmarks

Query profile

| ID       | Tables   | Pred. | Join | Aggr. | Group by | Order by| Return | Comment                       |
| :------: | :------- | :---: | :--: | :---: | :------: | :-----: | :----: | :---------------------------: |
| [1][p1]  | 1: L     | 1     | 0    | 8     | 2        | 2       | 10     |                               |
| [3][p3]  | 3: C,O,L | 5     | 2    | 1     | 3        | 2       | 4      |                               |
| [6][p6]  | 1: L     | 4     | 0    | 1     | 0        | 0       | 1      |                               |

[p1]: https://github.com/Sable/HorsePower/blob/master/docs/tpch/q1.md
[p3]: https://github.com/Sable/HorsePower/blob/master/docs/tpch/q3.md
[p6]: https://github.com/Sable/HorsePower/blob/master/docs/tpch/q6.md

List of tables ([On GitHub](https://github.com/Sable/HorsePower/blob/master/docs/tpch/create-table.md))

```no-highlight
| (N)  Nation   | (R) Region   | (P) Part   | (S) Supplier |
| (PS) PartSupp | (C) Customer | (O) Orders | (L) Lineitem |
```

## Manual translation

## Formal methods


