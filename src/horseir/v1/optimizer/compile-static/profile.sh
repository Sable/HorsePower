#!/bin/bash
runs=1 # number of runs
threads=1 # number of threads
scale=1
if [ "$#" -eq 1 ]; then
    :  # this is a null
elif [ "$#" -eq 2 ]; then
    runs=$2
else
    echo "Usage: (default runs=1, threads=1, scale=1)"
    echo "  1) $0 <id> "
    echo "  4) $0 <id> <scales>"
    exit 1
fi 
export OMP_PLACES=cores
export OMP_PROC_BIND=spread
export OMP_NUM_THREADS=$threads

# 2 3 5 7 8 10 13 15 18
id=$1         # query id
opt=3         # opt. level
targ=compile  # executable
echo "`hostname`> Q$id, T$threads, Run$runs, SF$scale"

if [ -z "$tool" ]; then
    tool="perf"
else
    if [ "$tool" != "valgrind" -a "$tool" != "perf" -a "$tool" != "intel" -a "$tool" != "perf_thread" ]; then
        echo "[Error] Please check tool: valgrind / perf / intel / perf_thread"
        exit 2
    fi
fi

valgrind_option="--instr-atstart=no"
#tool="vtune"
#tool="perf"
perf_option="-e task-clock,context-switches,cpu-migrations,page-faults,cycles,stalled-cycles-frontend,stalled-cycles-backend,instructions,branches,branch-misses,cache-references,cache-misses,L1-dcache-load-misses,L1-dcache-prefetch-misses"
perf_thread=50154
if [ $tool = "valgrind" ]; then
    # normal
    echo "(cd ../ && valgrind --tool=cachegrind ${valgrind_option} ./compile-static/$targ-q$id $scale $opt $runs)"
    (cd ../ && valgrind --tool=cachegrind ${valgrind_option} ./compile-static/$targ-q$id $scale $opt $runs)
elif [ $tool = "perf" ]; then
    echo "(cd ../ && perf stat ${perf_option} ./compile-static/$targ-q$id $scale $opt $runs)"
    (cd ../ && perf stat ${perf_option} ./compile-static/$targ-q$id $scale $opt $runs)
elif [ $tool = "perf_thread" ]; then
    (set -x && perf stat -p ${perf_thread} ${perf_option})
else
    # vtune on csdn (https://blog.csdn.net/fengfengdiandia/article/details/77046747)
    # step 1: collect hotspot information
    echo "(cd ../ && amplxe-cl -collect hotspots ./compile-static/$targ-q$id $scale $opt $runs)"
    (cd ../ && amplxe-cl -collect hotspots ./compile-static/$targ-q$id $scale $opt $runs)
    # step 2: analyze these information
    # (cd ../ && amplxe-cl -report hotspots -r r000hs) # dest file: r000hs
    # (cd ../ && amplxe-cl -report summary) # a brief summary
fi

# vtune usage

## helper
## amplxe-cl -help collect | less
## amplxe-cl -help report  | less

## memory access
## amplxe-cl -collect memory-access <commands>

## display hardware information
## amplxe-cl -report hw-events -r r002macc --format=csv > r002macc/chf_hw-events.csv


