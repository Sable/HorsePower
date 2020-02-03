# Implementation Overview

## Usage

Note: In order to learn how to use these scripts, you can type `./<script>` to
see its usage.

Build library `libcore.a` which includes built-in functions with all types of parameters

    ./make.sh lib

Build with CMake

    ## case 1
    (mkdir -p build && cd build && cmake .. && make)

    ## case 2 (common)
    ./make.sh debug

    ## case 3
    ./make.sh release

Run a query with all optimizations enabled

    ./run.sh opt q 6

Run a query with optimizations

    opt=opt1 ./run.sh opt q 6   ## opt1 or opt2

    ## opt1 is for elementwise fusion and pattern-based fusion
    ## opt2 is for automatic fusion and pattern-based fusion

Run C code from user-defined functions

    ./horse -c cpu -f ${HORSE_BASE}/tests/udf/q6_proc.hir


## Folder Structure

### Source Code Folders

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


### Temporary Folders

Folder name included in `.gitignore`

    tmp/

Note that its sub-folders can be added/removed freely since these folders will
not be committed.


## Naming Convensions

Help improve the quality of source code.

### Functions

Leading words for macros/functions

- `get`    : a function considers different cases before return consistent results
    + `getNode`: a function operates on nodes
    + `getList`: a function operates on lists
- `obtain` : a function returns constant values (strdup may introduce memory leaks)
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
- `free`   : a macro / function frees memory
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

Entry/Exit function

- `init`   : initialize global variables (static or external fields)
- `clean`  : deallocate memory if necessary


### Variables

- `x`: could be anything
- `t`: temporary variables for anything
- `c`: integers for counting
- `i`, `j`, `k`, `m`, `n`: iterating variables
- `i2`, `j2`, `k2`, `m2`, `n2`: associated iterating variables for length


### Messages

General

- All messages start with an uppercase letter

Messages with macros

- Error messages
    + `EP`, prints basic debugging information (with a default newline)
    + Effect: exit programs immediately
- Warning messages
    + `WP`, prints messages (without a default newline)
    + No effect
- To-do messages
    + `TODO`, prints basic debugging information (with a default newline)
    + Effect: exit programs immediately
- Stop-and-see messages
    + `STOP`, prints messages (without a default newline)
    + Effect: stop, enter, and continue


### Code Styles

Variable declaration with a pointer (align to variable)

    type *var

Function declaration with a return pointer (align to function name)

    type *func(...)

Function declaration must come with explicit parameter names

    int func(int a); // ok
    int func(int  ); // fine, but not ok




## Input/Output Operations

Data input and output.

### Input

Data is read from disk/ramdisk using a system variable `HORSE_BASE` which
stores the path of the HorsePower project.

    ${HORSE_BASE}/data

TPC-H raw data with scale factor (e.g. sf1)

    ${HORSE_BASE}/data/tpch/db1/*.tbl

TPC-H bin data with scale factor (e.g. sf1)

    ${HORSE_BASE}/data/tpch-bin/db1/*.bin


### Output

- stdout: for actual meaningful output, such as emitted code
    + `P`: standard output
- stderr: for trivial debugging information
    + `WP`: warning print (non-stop)
    + `EP`: error print (stop)
    + `STOP`: stop after print (stop)
- file output:
    + `FP`: file print
- string print:
    + `SP`: string print with a specific format


## Tests

Test programs are important for validating our implementation.
There are two kinds of tests: unit test and benchmark test,
which can be found in the folder ${HORSE_BASE}/tests.


### Unit Test

- `pass`: tests for testing passed cases
- `fail`: tests for capturing correct error messages


### Benchmark Test

- `tpch`: tests generated from TPC-H queries
- `udf` : tests created with user-defined functions


### Paper Repositories

CC20: See instructions on the docker repository.

    https://hub.docker.com/r/wukefe/cc20-docker


 
