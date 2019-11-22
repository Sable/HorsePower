#!/bin/bash

usage(){
    printf '%s\n' \
        "Usage: $0 <cmd>" "" \
        "  1) $0 <qid>               ## python m2ir.py plans/<qid>.plan" "" \
        "  2) $0 <cmd> <qid|file>    ## cmd: dump/show/dot for tpch or path/to/file" "" \
        "  3) $0 udf <name>          ## python m2ir.py plans/udf/<name>.plan" "" \
        "Examples:" "" \
        "  * kind=aida $0 show 6" ""
    exit 1
}

if [ -z $kind ]; then
    kind="tpch"  #tpch/aida
    input_folder="input-json"
fi

if [ $kind = "tpch" ]; then
    tpch_folder="${input_folder}/plans"
    udf_folder="${input_folder}/udf"
elif [ $kind = "aida" ]; then
    tpch_folder="/mnt/local/script-tpch/aida/plans/monetdb"
else
    usage
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
        (set -x && python m2ir.py ${tpch_folder}/q${qid}.plan)
    else
        echo "qid must be an integer"
        exit 1
    fi
elif [ "$#" -eq 2 ]; then
    cmd=$1
    if [ ${cmd} = "udf" ]; then
        (set -x && python m2ir.py ${udf_folder}/$2.plan)
    else
        qid=$2
        result=`checkNumber $qid`
        if [ $result = true ]; then
            (set -x && python m2ir.py ${cmd} ${tpch_folder}/q${qid}.plan)
        else
            (set -x && python m2ir.py ${cmd} ${udf_folder}/$2.plan)
        fi
    fi
else
    usage
fi



