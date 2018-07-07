#!/bin/bash

run_core(){
    #threads=(1 10 20 40 80)
    threads=(2 4 8 16 32 64)
    for x in "${threads[@]}"
    do
        echo "SF$1: thread $x"
        ./test-thread.sh $1 $x  &> "sf$1/log_thread_$x.log"
    done
    echo "SF$1: done"
}

scales=(1 2 4 8 16)
for x in "${scales[@]}"
do
    mkdir -p "sf$x"
    run_core $x
done


