#!/bin/bash

# ./horse -c cpu -f ./scripts/pl/BlkSchls.hir

usage() {
    printf '%s\n' \
        "Usage: $0 <cmd>" "" \
        " 1) $0 front                   ## build flex & bison" "" \
        " 2) $0 interp  q/t/f <id>      ## test with benchmarks (q: scripts, t: tests/pass, f:tests/fail)" "" \
        " 3) $0 compile q/t/f <id>      ## compile code without any optimizations" "" \
        " 4) $0 opt     q/t/f <id>      ## compile code with optimizations" "" \
        " 5) $0 stats load/dump         ## load/dump statistical information" "" \
        " 6) $0 print <item> q/t/f <id> ## print item (pretty/dot/symboltable/typeshape)" "" \
        " 7) $0 cloc                    ## show the number of lines of code" ""

    echo "Example: run=1 sf=1 thread=1 ./run.sh interp q 6"
    echo "         opt=fa ./run.sh opt q 6      ## automatic fusion for q6"
    echo "         ./run.sh print typeshape q 6 ## print type and shape only"
    exit 1
}

runInterpreter() {
    cmd=$1; tid=$2; sf=$3; run=$4; th=$5
    #echo $0  # --> ./run.sh
    if [ $cmd = "t" ]; then
        (set -x && ./horse -t -f ./tests/pass/t${tid}.hir)
    elif [ $cmd = "f" ]; then
        (set -x && ./horse -t -f ./tests/fail/t${tid}.hir)
    elif [ $cmd = "q" ]; then
        echo "Running TPC-H Query: q${tid}, sf$sf, run$run, thread$th"
        (set -x && ./horse -t -f ./scripts/q${tid}.hir --tpch=${tid})
    else
        usage
    fi
}

runCompiler() {
    cmd=$1; tid=$2
    if [ $cmd = "t" ]; then
        (set -x && ./horse -c cpu -f ./tests/pass/t${tid}.hir)
    elif [ $cmd = "f" ]; then
        (set -x && ./horse -c cpu -f ./tests/fail/t${tid}.hir)
    elif [ $cmd = "q" ]; then
        echo "Running TPC-H Query: q${tid}, sf$sf, run$run, thread$th"
        (set -x && ./horse -c cpu -f ./scripts/q${tid}.hir --tpch=${tid})
    else
        usage
    fi
}

runOptimizer() {
    cmd=$1; tid=$2; opt=$3
    opts="-o $opt"
    if [ $cmd = "t" ]; then
        (set -x && ./horse -c cpu ${opts} -f ./tests/pass/t${tid}.hir)
    elif [ $cmd = "f" ]; then
        (set -x && ./horse -c cpu ${opts} -f ./tests/fail/t${tid}.hir)
    elif [ $cmd = "q" ]; then
        echo "Running TPC-H Query: q${tid}, sf$sf, run$run, thread$th"
        (set -x && ./horse -c cpu ${opts} -f ./scripts/q${tid}.hir --tpch=${tid})
    else
        usage
    fi
}

runStats() {
    cmd=$1
    if [ $cmd = "dump" ]; then
        (set -x && ./horse -u --stats=dump)
    elif [ $cmd = "load" ]; then
        (set -x && ./horse -u --stats=load)
    else
        usage
    fi
}

runPrinter() {
    item=$1; cmd=$2; tid=$3;
    if [ $cmd = "t" ]; then
        (set -x && ./horse -u -f ./tests/pass/t${tid}.hir --print=${item})
    elif [ $cmd = "f" ]; then
        (set -x && ./horse -u -f ./tests/fail/t${tid}.hir --print=${item})
    elif [ $cmd = "q" ]; then
        (set -x && ./horse -u -f ./scripts/q${tid}.hir --tpch=${tid} --print=${item})
    else
        usage
    fi
}


if [ -z $sf ]; then
    sf=1
fi

if [ -z $run ]; then
    run=1
fi

if [ -z $thread ]; then
    thread=1
fi

if [ -z $opt ]; then
    opt=all
fi

export OMP_PLACES=cores
export OMP_PROC_BIND=spread
export OMP_NUM_THREADS=$thread

## Entry: main
if [ $# -eq 1 ]; then
    cmd=$1
    if [ $cmd = "front" ]; then
        (set -x && make test)
    elif [ $cmd = "cloc" ]; then
        (set -x && cloc backend frontend analysis util translator optimizer global.c global.h main.c)
    else
        usage
    fi
elif [ $# -eq 2 ]; then
    mod=$1
    if [ $mod = "stats" ]; then
        runStats $2
    else
        usage
    fi
elif [ $# -eq 3 ]; then
    mod=$1
    if [ $mod = "interp" ]; then
        runInterpreter $2 $3 $sf $run $thread
    elif [ $mod = "compile" ]; then
        runCompiler $2 $3
    elif [ $mod = "opt" ]; then
        runOptimizer $2 $3 $opt
    else
        usage
    fi
elif [ $# -eq 4 ]; then
    mod=$1
    if [ $mod = "print" ]; then
        runPrinter $2 $3 $4
    else
        usage
    fi
else
    usage
fi

