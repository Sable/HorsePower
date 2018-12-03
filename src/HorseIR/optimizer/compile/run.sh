#!/bin/bash
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <id>"
    exit 1
fi

# 2 3 5 7 8 10 13 15 18
id=$1  # query id
runs=1 # number of runs
opt=3  # opt. level
echo "(cd ../ && ./compile/testAll 1 $opt $runs $id)"
(cd ../ && ./compile/testAll 1 $opt $runs $id)
#(cd ../ && CPUPROFILE=/tmp/gperftoolsdump.out ./compile/testAll 1 $opt $runs $id)

## Compile
#gcc -O3 -o testQ main.c -DQ$1 -std=c99 -fopenmp ../backend/*.c ../util/*.c ../analysis/*.c ../global.c ../frontend/pretty.c -lm -lstdc++ -I"../../../../libs/include/" ../../../../libs/lib/libpcre2-8.a -lm
#gcc -g -o testAll main.c \
#    ../backend/*.c ../util/*.c ../analysis/*.c ../global.c ../frontend/pretty.c \
#    -I"../../../../libs/include/" ../../../../libs/lib/libpcre2-8.a \
#    -DQALL -std=c99 -fopenmp -lm -lstdc++ -march=native

