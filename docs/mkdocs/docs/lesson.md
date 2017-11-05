## Lessons learned from projects

### SQL data types

- [Online tutorial](https://www.w3schools.com/sql/sql_datatypes.asp)
- `DECIMAL(size,d)`: A DOUBLE stored as a string , allowing for a fixed decimal point.
    + A trick is to multiply the number with 10^d to convert it from a floating
      point number to an integer (see the MAL code of tpc-h query 6)

### SQLite overview

- [Architecture](https://sqlite.org/arch.html)
- [Core functions](https://sqlite.org/lang_corefunc.html)
- [Virtual database engine](https://sqlite.org/vdbe.html)

### Floating point precision problem

Description

```no-highlight
    0.06 + 0.01 == 0.07
> 0
```

- Precision in floating number comparison
- Check results by printing `P("%.20lf",x)`


### Not stable sort in NumPy

See [discussion](https://github.com/numpy/numpy/issues/6357)

```no-highlight
    np.argsort(x) -> np.argsort(x, kind='mergesort')
```

## Related systems

- [Pig Latin Reference Manual 2](https://pig.apache.org/docs/r0.7.0/piglatin_ref2.html)
- [Apache Ignite](https://ignite.apache.org/use-cases/database/in-memory-database.html)


