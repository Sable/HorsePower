
## HorseIR Development Folders

Versions

- v3: experimental with an LLVM code gen backend (ongoing)
- v2: stable HorseIR (**latest version**)
- v1: naive HorseIR only support SQL queries (i.e. without control flows)
- v0: experimental HorseIR (syntax is different from subsequent versions)


***Note***

- Each version is independent from others and contains individual tests

### Version: HorseIR to LLVM (v3)

Features (experimental)

- HorseIR-to-LLVM
- C code to C++ code with `extern "C"`

### Version: HorseIR Stable Release (v2)

Features

- HorseIR-to-C compiler (naive and optimized)
- HorseIR interpreter


