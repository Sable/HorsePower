#!/bin/bash
## ./run.sh <mode> ./horse -c cpu -q 5 -o f
## mode: test_

usage(){
    printf '%s\n' \
      "1) build an executable with source and then execute it" \
      "   $1 src <id> [runs]  ## ./make.sh <id> exp source && ./run-query.sh <id> [runs]" ""\
      "2) build an executable without source and then execute it" \
      "   $1 exp <id> [runs]  ## ./make.sh <id> exp && ./run-query.sh <id> [runs]" ""\
      "3) create a debug version for later gdb profiling" \
      "   $1 debug <id>       ## ./make.sh <id> exp source debug (in gdb, r 1 3 1) " ""\
      "4) build an executable with source and then execute it" \
      "   $1 cur <id> [runs]  ## ./make.sh <id> cur source && ./run-query.sh <id> [runs]" ""
    exit 1
}

if [ "$#" -eq 2 ]; then
    mode=$1; qid=$2
    if [ ${mode} = "src" ]; then
        (set -x && ./make.sh ${qid} exp source && ./run-query.sh ${qid} 0)
    elif [ ${mode} = "exp" ]; then
        (set -x && ./make.sh ${qid} exp && ./run-query.sh ${qid})
    elif [ ${mode} = "debug" ]; then
        (set -x && ./make.sh ${qid} exp source debug) && \
            (cd ../ && gdb ./compile-static/compile-q${qid})
    elif [ ${mode} = "cur" ]; then
        (set -x && ./make.sh ${qid} cur source && ./run-query.sh ${qid} 0)
    else
        usage $0
    fi
elif [ "$#" -eq 3 ]; then
    mode=$1; qid=$2; runs=$3
    if [ ${mode} = "src" ]; then
        (set -x && ./make.sh ${qid} exp source && ./run-query.sh ${qid} ${runs})
    elif [ ${mode} = "exp" ]; then
        (set -x && ./make.sh ${qid} exp && ./run-query.sh ${qid} ${runs})
    elif [ ${mode} = "cur" ]; then
        (set -x && ./make.sh ${qid} cur source && ./run-query.sh ${qid} ${runs})
    fi
else
    usage $0
fi

