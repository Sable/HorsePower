#!/bin/bash

# set executable
exe=./build/horse

usage() {
    printf '%s\n\n' \
        "Usage: $0 <options>" \
        " 1) $0 front                   ## build flex & bison" \
        " 2) $0 cloc                    ## show the number of lines of code" \
        " 3) $0 interp  q/t/f <id>      ## benchmarks (q: tests/tpch, t: tests/pass, f:tests/fail)" \
        " 4) $0 compile q/t/f <id>      ## compile code without any optimizations" \
        " 5) $0 opt     q/t/f <id>      ## compile code with all optimizations enabled" \
        " 6) $0 llvm    q/t/f <id>      ## compile with the jit support" \
        " 7) $0 stats load/dump         ## load/dump statistical information" \
        " 8) $0 print <cmd> q/t/f <id>  ## print cmd (pretty/dot/mermaid/symboltable/typeshape)" \
        " 9) $0 utility <cmd>           ## udf:q6, (pretty/dot/mermaid/symboltable/typeshape)"
    printf '%s\n\n' \
        "Examples:" \
        "  * run=1 sf=1 thread=1 $0 interp q 6   ## query 6 on interpreter" \
        "  * opt=opt1 $0 opt q 6                 ## optimizations for q6 (elementwise + patterns)" \
        "  * opt=opt2 $0 opt q 6                 ## optimizations for q6 (automatic fusion + patterns)" \
        "  * opt=opt2 $0 opt q 6 > \${HORSE_CC20}/gencode/opt2/q6.h" \
        "  * $0 print typeshape q 6              ## print type and shape only" \
        "  * $0 print pretty t 0                 ## pretty print with pass/t*.hir"
    exit 1
}

error(){
    echo "[Error] $1"
    exit 90
}

todo(){
    echo "[Todo] $1"
    exit 91
}

setOptFlags(){
    opt=$1
    if [ $opt = "opt1" ]; then
        opts="-o fp -o fe"
    elif [ $opt = "opt2" ]; then
        opts="-o fp1 -o fa -o fp2"
    else
        opts="-o ${opt}"
    fi
}

runInterpreter() {
    cmd=$1; tid=$2; sf=$3; run=$4; th=$5
    #echo $0  # --> ./run.sh
    if [ $cmd = "t" ]; then
        (set -x && ${exe} -t -f ${HORSE_BASE}/tests/pass/t${tid}.hir)
    elif [ $cmd = "f" ]; then
        (set -x && ${exe} -t -f ${HORSE_BASE}/tests/fail/t${tid}.hir)
    elif [ $cmd = "q" ]; then
        echo "Running TPC-H Query: q${tid}, sf$sf, run$run, thread$th"
        (set -x && ${exe} -t -f ${HORSE_BASE}/tests/tpch/q${tid}.hir --tpch=${tid})
    else
        usage
    fi
}

runCompiler() {
    cmd=$1; tid=$2
    if [ $cmd = "t" ]; then
        (set -x && ${exe} -c cpu -f ${HORSE_BASE}/tests/pass/t${tid}.hir)
    elif [ $cmd = "f" ]; then
        (set -x && ${exe} -c cpu -f ${HORSE_BASE}/tests/fail/t${tid}.hir)
    elif [ $cmd = "q" ]; then
        (set -x && ${exe} -c cpu -f ${HORSE_BASE}/tests/tpch/q${tid}.hir --tpch=${tid})
    else
        usage
    fi
}

runOptimizer() {
    cmd=$1; tid=$2
    if [ $cmd = "t" ]; then
        (set -x && ${exe} -c cpu ${opts} -f ${HORSE_BASE}/tests/pass/t${tid}.hir)
    elif [ $cmd = "f" ]; then
        (set -x && ${exe} -c cpu ${opts} -f ${HORSE_BASE}/tests/fail/t${tid}.hir)
    elif [ $cmd = "q" ]; then
        (set -x && ${exe} -c cpu ${opts} -f ${HORSE_BASE}/tests/tpch/q${tid}.hir --tpch=${tid})
    else
        usage
    fi
    # save output to ${HORSE_CC20}/gencode/opt2/
}

runJIT() {
    cmd=$1; tid=$2; opts=""
    if [ $cmd = "t" ]; then
        (set -x && ${exe} -c llvm ${opts} -f ${HORSE_BASE}/tests/pass/t${tid}.hir)
    elif [ $cmd = "f" ]; then
        (set -x && ${exe} -c llvm ${opts} -f ${HORSE_BASE}/tests/fail/t${tid}.hir)
    elif [ $cmd = "q" ]; then
        (set -x && ${exe} -c llvm ${opts} -f ${HORSE_BASE}/tests/tpch/q${tid}.hir --tpch=${tid})
    else
        usage
    fi
}

runStats() {
    cmd=$1
    if [ $cmd = "dump" ]; then
        (set -x && ${exe} -u --stats=dump)
    elif [ $cmd = "load" ]; then
        (set -x && ${exe} -u --stats=load)
    else
        usage
    fi
}

runPrinter() {
    item=$1; cmd=$2; tid=$3;
    if [ $cmd = "t" ]; then
        (set -x && ${exe} -u -f ${HORSE_BASE}/tests/pass/t${tid}.hir --print=${item})
    elif [ $cmd = "f" ]; then
        (set -x && ${exe} -u -f ${HORSE_BASE}/tests/fail/t${tid}.hir --print=${item})
    elif [ $cmd = "q" ]; then
        (set -x && ${exe} -u -f ${HORSE_BASE}/tests/tpch/q${tid}.hir --tpch=${tid} --print=${item})
    else
        usage
    fi
}

runUtility() {
    cmd=$1
    (set -x && ${exe} -u --print ${cmd} -f ${HORSE_BASE}/tests/udf/q6_proc.hir)
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

setOptFlags $opt

export OMP_PLACES=cores
export OMP_PROC_BIND=spread
export OMP_NUM_THREADS=$thread


## Entry: main
if [ $# -eq 1 ]; then
    cmd=$1
    if [ $cmd = "front" ]; then
        todo "Add front support in CMakeLists.txt"
    elif [ $cmd = "cloc" ]; then
        (set -x && cloc backend frontend analysis util translator optimizer global.cc global.h main.cc)
    else
        usage
    fi
elif [ $# -eq 2 ]; then
    mod=$1
    if [ $mod = "stats" ]; then
        runStats $2
    elif [ $mod = "utility" ]; then
        runUtility $2
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
        runOptimizer $2 $3
    elif [ $mod = "llvm" ]; then
        runJIT $2 $3
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


