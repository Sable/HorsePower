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
    0.6 + 0.1 == 0.7
> 0
```

- Precision in floating number comparison
- Check results by printing `P("%.20lf",x)`


