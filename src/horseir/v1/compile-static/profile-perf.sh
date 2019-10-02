#!/bin/bash
if [ "$#" -eq 1 ]; then
    input_option=$1
    input_thread=`ps au | pgrep compile`
    :
elif [ "$#" -eq 2 ]; then
    input_option=$1
    input_thread=$2
else
    echo "Usage: (default option=process)"
    echo "  1) $0 <option>               # $0 process <auto>"
    echo "  1) $0 <option> <thread_id>   # $0 process 50514 "
    exit 1
fi 

echo "$0 ${input_option} ${input_thread}"

perf_option="-e task-clock,context-switches,cpu-migrations,page-faults,cycles,stalled-cycles-frontend,stalled-cycles-backend,instructions,branches,branch-misses,cache-references,cache-misses,L1-dcache-load-misses,L1-dcache-prefetch-misses"

if [ ${input_option} = "process" ]; then
    (set -x && perf stat -p ${input_thread} ${perf_option})
fi

