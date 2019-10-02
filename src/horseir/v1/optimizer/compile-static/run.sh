#!/bin/bash
## ./run.sh <mode> ./horse -c cpu -q 5 -o f
## mode: test_

usage(){
    printf '%s\n' \
      "1) build an executable with source and then execute it" \
      "   $1 src <id> [runs]  ## ./make.sh <id> exp source && ./run-query.sh <id> [runs]" ""\
      "2) build an executable without source and then execute it" \
      "   $1 exp <id>         ## ./make.sh <id> exp && ./run-query.sh <id>" ""
    exit 1
}

if [ "$#" -eq 2 ]; then
    mode=$1; qid=$2
    if [ ${mode} = "src" ]; then
        (set -x && ./make.sh ${qid} exp source && ./run-query.sh ${qid})
    elif [ ${mode} = "exp" ]; then
        (set -x && ./make.sh ${qid} exp && ./run-query.sh ${qid})
    else
        usage $0
    fi
elif [ "$#" -eq 3 ]; then
    mode=$1; qid=$2; runs=$3
    if [ ${mode} = "src" ]; then
        (set -x && ./make.sh ${qid} exp source && ./run-query.sh ${qid} ${runs})
    fi
else
    usage $0
fi

