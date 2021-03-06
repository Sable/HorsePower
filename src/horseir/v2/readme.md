## Repository Overview

### Common Command Lines

Note: In order to learn how to use these scripts, you can type `./<script>` to
see its usage.

Compile and run a query with all optimizations enabled

    make && ./run.sh opt q 4

Compile and run a query with a specific optimization (e.g. fusing with patterns)

    make && opt=fp ./run.sh opt q 4

### Folder Structure

Source code folders

```
- analysis  : program analysis
- backend   : library support for primitive functions
- frontend  : parser and pretty printer
- optimizer : program optimizations after analyses
- scripts   : SQL queries from tpch and aquery 
- translator: interpreter, compiler, and optimizer
- util      : utility functions
- llvm      : llvm codegen support
```

Trivial folders

```
- exp   : for tests and experiments
- tests : a aliased folder which contains unit tests
- libs  : an experiment to create a big library for built-in functions with all types of parameters
```

## Temporary Folder

Overview

```
    tmp/
        udf/
```

***Note***: New sub-folders can be added freely.

### User-defined Functions

Workspace `tmp/udf` is created for testing generated C code from HorseIR programs.

Run compiler to fetch C code

    ./horse -c cpu -f scripts/udf/q6_proc.hir



## Naming Convensions

### Functions

Leading words for macros/functions

- `get`    : a function considers different cases before return consistent results
    + `getNode`: a function operates on nodes
    + `getList`: a function operates on lists
- `obtain` : a function returns constant values
- `load`   : a function loads one or more values in bulks
- `fill`   : a function loads one value at a time
- `gen`    : a function generates and saves strings
- `glue`   : a macro / function writes strings to a buffer
- `print`  : a macro / function prints data to stdin
- `scan`   : a function iterates tree nodes
- `total`  : a function counts # of list (ends with 1 means `with dummy`, otherwise `no dummy`)
- `is`     : a macro / function checks conditions and returns a boolean value
- `find`   : a function searchs for one or more specific values
- `new`    : a macro / function allocates memory for a specific types
- `inc`    : a macro increases a counter
- `fetch`  : ...
- `gain`   : ...
- `acquire`: ...
- `put`    : ...
- `use`    : ...
- `take`   : ...

Classifications

- `node`   : a macro gets node's construct fields directly
- `chain`  : a macro for chain nodes
- `opt`    : for optimization strategies
- `debug`  : an indicator for debugging information

Function scopes

- `static` : local private functions/variables

### Variables

- `x`: could be anything
- `t`: temporary variables for anything
- `c`: integers for counting
- `i`, `j`, `k`, `m`, `n`: iterating variables
- `i2`, `j2`, `k2`, `m2`, `n2`: associated iterating variables for length


## I/O Operations

### Input

Data is read from disk/ramdisk using a system variable `HORSE_BASE` which
stores the path of the HorsePower project.


### Output

- stdout: for actual meaningful output, such as emitted code
    + P: standard output
- stderr: for trivial debugging information
    + WP: warning print (non-stop)
    + EP: error print (stop)
    + STOP: stop after print (stop)
- file output:
    + FP: file print
- string print:
    + SP: string print with a specific format


## Experimental Repository

CGO20

Create a temporary file and save into the folder `semi`

    make && opt=fp ./run.sh opt q 1 > cgo20/semi/q1.h
    opt=fe ./run.sh opt q 1




