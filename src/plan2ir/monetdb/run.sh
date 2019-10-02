#!/bin/bash

usage(){
    printf '%s\n' \
        "Usage: $0 <cmd>" "" \
        "  1) $0 <qid>        # python m2ir.py plans/<qid>.plan" "" \
        "  2) $0 <cmd> <qid>  # cmd: dump/show/dot" "" \
        "Example: kind=aida $0 show 6"
    exit 1
}

if [ -z $kind ]; then
    kind="tpch" #tpch/aida
fi

if [ $kind = "tpch" ]; then
    file_dir="plans/q"
else
    file_dir="/mnt/local/script-tpch/aida/plans/monetdb/q"
fi

if [ "$#" -eq 1 ]; then
    qid=$1
    #(set -x && python m2ir.py plans/q${qid}.plan)
    (set -x && python m2ir.py ${file_dir}${qid}.plan)
elif [ "$#" -eq 2 ]; then
    op=$1; qid=$2
    (set -x && python m2ir.py ${op} ${file_dir}${qid}.plan)
else
    usage
fi



