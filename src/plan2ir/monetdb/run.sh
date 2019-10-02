#!/bin/bash

usage(){
    printf '%s\n' \
        "Usage: $0 <cmd>" "" \
        "  1) $0 <qid>              # python m2ir.py plans/<qid>.plan" "" \
        "  2) $0 <cmd> <qid>        # cmd: dump/show/dot for tpch" \
        "     $0 <cmd> <file>       # cmd: dump/show/dot for path/to/file" "" \
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

checkNumber(){
    num=$1
    if [ -n $num ] && [ "$num" -eq "$num" ] 2>/dev/null; then
        echo true
    else
        echo false
    fi
}


if [ "$#" -eq 1 ]; then
    qid=$1
    result=`checkNumber $qid`
    if [ $result = true ]; then
        #(set -x && python m2ir.py plans/q${qid}.plan)
        (set -x && python m2ir.py ${file_dir}${qid}.plan)
    else
        file_path=$1
        (set -x && python m2ir.py plans/udf/${file_path}.plan)
    fi
elif [ "$#" -eq 2 ]; then
    cmd=$1; qid=$2
    result=`checkNumber $qid`
    if [ $result = true ]; then
        (set -x && python m2ir.py ${cmd} ${file_dir}${qid}.plan)
    else
        file_path=$2
        (set -x && python m2ir.py ${cmd} plans/udf/${file_path}.plan)
    fi
else
    usage
fi



