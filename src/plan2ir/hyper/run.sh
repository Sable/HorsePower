#!/usr/bin/env bash

usage(){
    printf '%s\n' \
        "Usage: $0 <cmd>" "" \
        "  1) $0 <tpch | aida> <id>          ## gen code from tpch or aida queries" "" \
        "  2) $0 batch tpch <all | partial>  ## gen code from tpch or aida queries" "" \
        "  3) $0 profile horseir             ## profile HorseIR code under gen/" "" \
        "  4) $0 see diff                    ## diff gen-v1/q$.hir gen/q$.hir" "" \
        "  5) $0 see pass                    ## see if TPC-H queries pass" "" \
        "  6) $0 see size                    ## see the length info of files in gen/" "" \
        "Example: $0 tpch 6  ## query 6"
    exit 1
}

inputdata="input-json"
outputdata="output-hir"
postfolder="post-process"

RED='\033[0;31m'
GN='\033[0;32m'
NO='\033[0m'

function gen_tpch_plan(){
    src="${inputdata}/opt-order"
    #python genIR.py "$src/q$1.json" | tee gen/q$1.hir
    python genIR.py "$src/q$1.json"
    #python genIR.py "unopt/q$1.json"
    exit ${PIPESTATUS[0]}
}

function gen_aida_plan(){
    src="/home/sable/hanfeng.c/Desktop/hobby/aida/tpch/hyper/plans"
    python genIR.py "$src/q$1.json"
    exit ${PIPESTATUS[0]}
}

function gen_tpch_plan_batch(){
    if [ $1 = "all" ]; then
        ids = `(seq 1 22)`
    else
        # ids=( 1 4 6 12 14 16 19 22 )
        # ids=( 2 3 5 7 8 9 10 11 13 15 17 18 20 21 )
        ids=( 21 )
    fi
    for id in ${ids[@]}
    do
        echo "processing query $id"
        gen_tpch_plan $id > ${outputdata}/gen/q${id}.hir
        if test ${pipestatus[0]} -eq 0 ; then
            echo -e "query $id: ${GN}ok${NO}"
        else
            echo -e "query $id: ${RED}failed${NO}"
        fi
    done
}

function see_diff(){
    for id in {1..22}
    do
        diff ${outputdata}/gen-v1/q$id.hir ${outputdata}/gen/q$id.hir
    done
}

function see_pass(){
    # case 1: Incompelte tests
    #ids=(2 5 7 8 9 11 15 17 20 21 22)
    #ids=(1 6 4 14 12 13 16 17 19 22)
    #ids=(1 4 6 12 14 16 19 22)
    #for id in ${ids[@]}

    # case 2: Complete tests
    for id in {1..22}
    do
        gen_tpch_plan $id &> /dev/null
        if test ${PIPESTATUS[0]} -eq 0 ; then
            echo -e "query $id: ${GN}ok${NO}"
        else
            echo -e "query $id: ${RED}failed${NO}"
        fi
    done
}

function see_size(){
    total=0
    for id in {1..22}
    do
        size=$(cat ${outputdata}/gen/q$id.hir | wc -l)
        echo "q$id $size"
        total=$(($total + $size))
    done
    echo "Current total lines of code: $total"
}

if [ "$#" -eq 2 ]; then
    cmd=$1; qid=$2
    if [ $cmd = "tpch" ]; then
        (set -x && gen_tpch_plan $qid)
    elif [ $cmd = "aida" ]; then
        (set -x && gen_aida_plan $qid)
    elif [ $cmd = "profile" ] && [ $2 = "horseir" ]; then
        grep -REin "program slicing|time" ${outputdata}/gen/*.hir | python ${postfolder}/profile_horseir.py
    elif [ $cmd = "see" ] && [ $2 = "diff" ]; then
        see_diff
    elif [ $cmd = "see" ] && [ $2 = "pass" ]; then
        see_pass
    elif [ $cmd = "see" ] && [ $2 = "size" ]; then
        see_size
    else
        usage
    fi
elif [ $# -eq 3 ]; then
    if [ $1 = "batch" ] && [ $2 = "tpch" ]; then
        if [ $3 = "all" ] || [ $3 = "partial" ]; then
            gen_tpch_plan_batch $3
        else 
            usage
        fi
    else 
        usage
    fi
else
    usage
fi

