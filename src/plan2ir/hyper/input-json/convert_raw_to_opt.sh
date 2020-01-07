#!/usr/bin/env bash

usage(){
    printf '%s\n\n' \
        "Usage: $0 <option>" \
        " 1) $0 opt        ## generate optimized plans from raw input" \
        " 2) $0 unopt      ## generate unoptimized plans from raw input" \
        " 3) $0 opt-order  ## generate optimized plans with order from raw input"
    exit 1
}
handle_query(){
    folder=$1
    for x in {1..22}
    do
        echo "Processing query $x"
        python convert_raw_to_opt.py "raw/q${x}.json" > "${folder}/q${x}.json"  # using OrderedDict
        #python convert_raw_to_opt.py "raw/q${x}.json" > "opt/q${x}.json"       # not using OrderedDict
        #python convert_raw_to_opt.py "raw/q${x}.json" > "opt-order/q${x}.json"  # using OrderedDict
        #python convert_raw_to_opt.py "raw/q${x}.json" > "unopt/q${x}.json"
    done
}

if [ $# -eq 1 ]; then
    if [ $1 = "opt" -o $1 = "unopt" -o $1 = "opt-order" ]; then
        folder="plain-$1"
        mkdir -p ${folder} && (set -x && handle_query ${folder})
    else
        usage
    fi
else
    usage
fi




