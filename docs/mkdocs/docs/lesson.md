# Lessons learned from projects

!!! tip "Note"
    Keep updating this page!


### Common

**Search for gcc include directories**

- `echo | gcc -E -Wp,-v -`
    + For example, math.h : `/usr/local/lib/gcc/x86_64-apple-darwin15.6.0/7.2.0/include-fixed/math.h`

**Hash functions**

- [MurmurHash3](https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp): see [wiki](https://en.wikipedia.org/wiki/MurmurHash)

**pcre2 Library**

- Resources for learning the library: [rust-lang/regex][rust-regex], [regex engines][regex-engines]

[rust-regex]: https://github.com/rust-lang/regex/blob/3de8c44f5357d5b582a80b7282480e38e8b7d50d/bench/src/ffi/pcre2.rs#L42
[regex-engines]: https://rust-leipzig.github.io/regex/2017/03/28/comparison-of-regex-engines/

**Related systems**

- [Pig Latin Reference Manual 2](https://pig.apache.org/docs/r0.7.0/piglatin_ref2.html)
- [Apache Ignite](https://ignite.apache.org/use-cases/database/in-memory-database.html)

### SQL

**SQL data types**

- [Online tutorial](https://www.w3schools.com/sql/sql_datatypes.asp)
- `DECIMAL(size,d)`: A DOUBLE stored as a string , allowing for a fixed decimal point.
    + A trick is to multiply the number with 10^d to convert it from a floating
      point number to an integer (see the MAL code of tpc-h query 6)

**SQLite overview**

- [Architecture](https://sqlite.org/arch.html)
- [Core functions](https://sqlite.org/lang_corefunc.html)
- [Virtual database engine](https://sqlite.org/vdbe.html)

### PL

**Floating point precision problem**

```no-highlight
    0.06 + 0.01 == 0.07
> 0
```

- Precision in floating number comparison
- Check results by printing `P("%.20lf",x)`


**Not stable sort in NumPy**

See [discussion](https://github.com/numpy/numpy/issues/6357)

```no-highlight
    np.argsort(x) -> np.argsort(x, kind='mergesort')
```

### Hash Join

- An example of [hash join](https://rosettacode.org/wiki/Hash_join)
- [Hash join in a phd thesis](http://www.dcs.ed.ac.uk/home/tz/phd/thesis/node21.htm)



