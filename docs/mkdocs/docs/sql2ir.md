# Translating SQL to HorseIR

## TPC-H benchmarks

Query profile

| ID       | Tables      | Pred. | Join | Aggr. | Group | Order | Return | Comment                       |
| :------: | :---------- | :---: | :--: | :---: | :---: | :---: | :----: | :---------------------------: |
| [1][q1]  | L           | 1     | 0    | 8     | 2     | 2     | 10     | Pass                          |
| [2][q2]  |             |       |      |       |       |       |        |                               |
| [3][q3]  | C,O,L       | 5     | 2    | 1     | 3     | 2     | 4      | Working                       |
| [4][q4]  |             |       |      |       |       |       |        |                               |
| [5][q5]  | C,O,L,S,N,R | 9     | 6    | 1     | 1     | 1     | 2      |                               |
| [6][q6]  | L           | 4     | 0    | 1     | 0     | 0     | 1      | Pass                          |
| [7][q7]  |             |       |      |       |       |       |        |                               |
| [8][q8]  |             |       |      |       |       |       |        |                               |
| [9][q9]  |             |       |      |       |       |       |        |                               |
| [10][q10]| C,O,L,N     | 6     | 3    | 1     | 7     | 1     | 8      |                               |
| [11][q11]|             |       |      |       |       |       |        |                               |
| [12][q12]| O,L         | 6     | 1    | 2     | 1     | 1     | 3      | Working                       |
| [13][q13]| C,O         | 2     | 1(o) | 2     | 2     | 2     | 4      | (Nested)                      |
| [14][q14]| L,P         | 3     | 2    | 1     | 0     | 0     | 1      | Working                       |
| [15][q15]| S,L         |       |      |       |       |       |        | (View)                        |
| [16][q16]| PS,P,S      | 6     | 1    | 1     | 3     | 4     | 5      |                               |
| [17][q17]| L,P         | 4     | 2    | 2     | 0     | 0     | 2      |                               |
| [18][q18]| C,O,L       | 3     | 2    | 1     | 5     | 2     | 7      |                               |
| [19][q19]|             |       |      |       |       |       |        |                               |
| [20][q20]|             |       |      |       |       |       |        |                               |
| [21][q21]|             |       |      |       |       |       |        |                               |
| [22][q22]|             |       |      |       |       |       |        |                               |


List of tables ([On GitHub](https://github.com/Sable/HorsePower/blob/master/docs/tpch/create-table.md))

```no-highlight
| (N)  Nation   | (R) Region   | (P) Part   | (S) Supplier |
| (PS) PartSupp | (C) Customer | (O) Orders | (L) Lineitem |
```

## Manual translation

## Formal methods


[q1]: https://github.com/Sable/HorsePower/blob/master/docs/tpch/q1.md
[q2]: https://github.com/Sable/HorsePower/blob/master/docs/tpch/q2.md
[q3]: https://github.com/Sable/HorsePower/blob/master/docs/tpch/q3.md
[q4]: https://github.com/Sable/HorsePower/blob/master/docs/tpch/q4.md
[q5]: https://github.com/Sable/HorsePower/blob/master/docs/tpch/q5.md
[q6]: https://github.com/Sable/HorsePower/blob/master/docs/tpch/q6.md
[q7]: https://github.com/Sable/HorsePower/blob/master/docs/tpch/q7.md
[q8]: https://github.com/Sable/HorsePower/blob/master/docs/tpch/q8.md
[q9]: https://github.com/Sable/HorsePower/blob/master/docs/tpch/q9.md
[q10]: https://github.com/Sable/HorsePower/blob/master/docs/tpch/q10.md
[q11]: https://github.com/Sable/HorsePower/blob/master/docs/tpch/q11.md
[q12]: https://github.com/Sable/HorsePower/blob/master/docs/tpch/q12.md
[q13]: https://github.com/Sable/HorsePower/blob/master/docs/tpch/q13.md
[q14]: https://github.com/Sable/HorsePower/blob/master/docs/tpch/q14.md
[q15]: https://github.com/Sable/HorsePower/blob/master/docs/tpch/q15.md
[q16]: https://github.com/Sable/HorsePower/blob/master/docs/tpch/q16.md
[q17]: https://github.com/Sable/HorsePower/blob/master/docs/tpch/q17.md
[q18]: https://github.com/Sable/HorsePower/blob/master/docs/tpch/q18.md
[q19]: https://github.com/Sable/HorsePower/blob/master/docs/tpch/q19.md
[q20]: https://github.com/Sable/HorsePower/blob/master/docs/tpch/q20.md
[q21]: https://github.com/Sable/HorsePower/blob/master/docs/tpch/q21.md
[q22]: https://github.com/Sable/HorsePower/blob/master/docs/tpch/q22.md
