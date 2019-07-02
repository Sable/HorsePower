#!/bin/bash

usage(){
    printf '%s\n' \
        "Usage: $0 <cmd>" "" \
        "  1) $0 <qid>  # python m2ir.py plans/<qid>.plan" ""
    exit 1
}

if [ "$#" -eq 1 ]; then
    qid=$1
    (set -x && python m2ir.py plans/q${qid}.plan)
else
    usage
fi



