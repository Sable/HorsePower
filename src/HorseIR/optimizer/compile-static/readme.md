
## Compile & run a query
Build a compiled query (single)

    make q6 # generate: horse-compile-q6

Run the query

    ./run.sh      # check help info
    ./run.sh 6 10 # Example: query 6 with 10 times

## Setup

Build library: libhorse.a

    cd ../ && ./build_lib.sh

Mirror files: compile.h and fuse.h

    # copied from ../compile/

Library order in linking (see Makefile)

    # libhorse.a must be placed before libpcre2-8.a

See a loaded file with a specific query id

    gcc -E loadcode.h -DINCLUDE_Q14

## Experiments

Sable-Intel

### Exp.1

Steps

    # step 1
    clang-6 -emit-llvm -S main.c -o clang-q6.ll -I../../../../libs/include/ -DINCLUDE_Q6 -DSELECT_C

    # step 2
    llc-6 -filetype=obj clang-q6.ll -o clang-q6.o

    # step 3
    gcc clang-q6.o -o clang-q6 ./libhorse.a ../../../../libs/lib/libpcre2-8.a -std=c99 -fopenmp -lm -lstdc++ -march=native

Results

      step 1       step 2      step 3
    C ------> llvm ------> obj ------> exec
      130 ms       60 ms       200 ms    

Variants (gnu/ld,140ms,faster)

	ld \
	  -dynamic-linker /lib64/ld-linux-x86-64.so.2 \
	  /usr/lib/x86_64-linux-gnu/crt1.o \
	  /usr/lib/x86_64-linux-gnu/crti.o \
	  /usr/lib/x86_64-linux-gnu/crtn.o \
	  /usr/lib/x86_64-linux-gnu/libm.so \
	  /usr/lib/x86_64-linux-gnu/libc.so \
	  clang-q6.o \
	  ./libhorse.a \
	  ../../../../libs/lib/libpcre2-8.a \
	  /mnt/sableintel/tools/clang/lib/libomp.so \
	  -o clang-q6

Note: linker command line can be seen by typing `gcc -Xlinker -v`

Variants (clang/lld,270ms,slow)

	/mnt/sableintel/tools/clang/bin/ld.lld \
	  -dynamic-linker /lib64/ld-linux-x86-64.so.2 \
	  /usr/lib/x86_64-linux-gnu/crt1.o \
	  /usr/lib/x86_64-linux-gnu/crti.o \
	  /usr/lib/x86_64-linux-gnu/crtn.o \
	  /usr/lib/x86_64-linux-gnu/libm.so \
	  /usr/lib/x86_64-linux-gnu/libc.so \
	  clang-q6.o \
	  ./libhorse.a \
	  ../../../../libs/lib/libpcre2-8.a \
	  /mnt/sableintel/tools/clang/lib/libomp.so \
	  -o clang-q6


