## Lessons learned from projects

### SQLite

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


