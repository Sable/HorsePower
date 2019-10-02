#!/usr/bin/env bash

function usage(){
    echo "Usage: $0 <tpch/aida> <id>"
    exit 1
}

function tpch_plain(){
    src="opt-order"
    #python genIR.py "$src/q$1.json" | tee gen/q$1.hir
    python genIR.py "$src/q$1.json"
    #python genIR.py "unopt/q$1.json"
    exit ${PIPESTATUS[0]}
}

function aida_plain(){
    src="/home/sable/hanfeng.c/Desktop/hobby/aida/tpch/hyper/plans"
    python genIR.py "$src/q$1.json"
    exit ${PIPESTATUS[0]}
}

if [ "$#" -eq 2 ]; then
    cmd=$1; qid=$2
    if [ $cmd = "tpch" ]; then
        tpch_plain $qid
    elif [ $cmd = "aida" ]; then
        aida_plain $qid
    else
        usage
    fi
else
    usage
fi

