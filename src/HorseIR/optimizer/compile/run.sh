#!/bin/bash
#if [ "$#" -ne 1 ]; then
#    echo "Usage: $0 <id>"
#    exit 1
#fi
#gcc -O3 -o testQ main.c -DQ$1 -std=c99 -fopenmp ../backend/*.c ../util/*.c ../analysis/*.c ../global.c ../frontend/pretty.c -lm -lstdc++ -I"../../../../libs/include/" ../../../../libs/lib/libpcre2-8.a -lm
gcc -O3 -o testAll main.c -DQALL -std=c99 -fopenmp ../backend/*.c ../util/*.c ../analysis/*.c ../global.c ../frontend/pretty.c -lm -lstdc++ -I"../../../../libs/include/" ../../../../libs/lib/libpcre2-8.a -lm -march=native

# 2 3 5 7 8 10 13 15 18
id=18
nn=1
(cd ../ && ./compile/testAll 1 3 $nn $id)

