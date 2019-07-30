#!/bin/bash
runs=1 # number of runs
threads=1 # number of threads
scale=1
if [ "$#" -eq 1 ]; then
    :  # this is a null
elif [ "$#" -eq 2 ]; then
    runs=$2
elif [ "$#" -eq 3 ]; then
    runs=$2
    threads=$3
elif [ "$#" -eq 4 ]; then
    runs=$2
    threads=$3
    scale=$4
else
    echo "Usage: (default runs=1, threads=1, scale=1)"
    echo "  1) $0 <id> "
    echo "  2) $0 <id> <runs>"
    echo "  3) $0 <id> <runs> <threads>"
    echo "  4) $0 <id> <runs> <threads> <scale>"
    echo ""
    echo "Note: targ=compile-clang ./run-query.sh 6 10  # for compile-clang-q6"
    exit 1
fi

export OMP_PLACES=cores
export OMP_PROC_BIND=spread
#export OMP_PROC_BIND=true # For PGI
export OMP_NUM_THREADS=$threads

# 2 3 5 7 8 10 13 15 18
id=$1         # query id
if [ -z $targ ]; then
    targ=compile  # executable
fi

echo "`hostname`> Q$id, T$threads, Run$runs, SF$scale"
# normal
#echo "(cd ../ && ./compile-static/$targ-q$id $scale $opt $runs)"
(set -x && cd ../ && ./exp/$targ-q$id $scale $runs)

# profile: valgrind::callgrind/cachegrind
#(cd ../ && valgrind --tool=cachegrind ./compile/$targ 1 $opt $runs $id)
#(cd ../ && valgrind --tool=callgrind --instr-atstart=no ./compile/$targ 1 $opt $runs $id)

# profile: gperftools
#(cd ../ && CPUPROFILE=/tmp/gperftoolsdump.out ./compile/$targ 1 $opt $runs $id)

## Compile
#gcc -O3 -o testQ main.c -DQ$1 -std=c99 -fopenmp ../backend/*.c ../util/*.c ../analysis/*.c ../global.c ../frontend/pretty.c -lm -lstdc++ -I"../../../../libs/include/" ../../../../libs/lib/libpcre2-8.a -lm
#gcc -g -o $targ main.c \
#    ../backend/*.c ../util/*.c ../analysis/*.c ../global.c ../frontend/pretty.c \
#    -I"../../../../libs/include/" ../../../../libs/lib/libpcre2-8.a \
#    -DQALL -std=c99 -fopenmp -lm -lstdc++ -march=native

