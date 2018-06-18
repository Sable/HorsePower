#!/bin/bash
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <id>"
    exit 1
fi
gcc -g -o testQ$1 main.c -DQ$1 -std=c99 -fopenmp ../backend/*.c ../util/*.c ../analysis/*.c ../global.c ../pretty.c -lm -lstdc++ -I"../../../../libs/include/" ../../../../libs/lib/libpcre2-8.a -lm

