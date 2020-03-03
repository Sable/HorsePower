#!/usr/bin/env bash

usage(){
    printf '%s\n\n' \
        "Usage: $0 <cmd>" \
        "  1) $0 tpch <id>                   ## gen code from tpch queries" \
        "  2) $0 batch tpch <all | partial>  ## all or partial queries, save to output-hir/gen" \
        "  3) $0 profile horseir             ## profile HorseIR code under gen/" \
        "  4) $0 profile plan <folder>       ## profile input execution plans <opt/raw/...>" \
        "  5) $0 see diff                    ## diff gen-v1/q$.hir gen/q$.hir" \
        "  6) $0 see pass                    ## see if TPC-H queries pass" \
        "  7) $0 see size                    ## see the length info of files in gen/" \
        "  8) $0 cloc                        ## report the code size" \
        "  9) $0 exec <id>                   ## run interpter with SF1 on a TPC-H query" \
        "Example:" \
        "  *) $0 tpch 6                        ## query 6" \
        "  *) format=latex $0 profile horseir  ## output in the latex format" \
        "  *) testfolder=latest $0 exec 4      ## output-hir/gen-latest"
    exit 1
}

inputdata="input-json"
outputdata="output-hir"
postfolder="post-process"
prefolder="pre-process"
genfolder="gen-latest"
gentemp="gen"

RED='\033[0;31m'
GN='\033[0;32m'
NO='\033[0m'

exe="${HORSE_BASE}/src/horseir/v3/build/horse"

function gen_tpch_plan(){
    src="${inputdata}/opt-order"
    #python genIR.py "$src/q$1.json" | tee gen/q$1.hir
    python genIR.py "$src/q$1.json"
    #python genIR.py "unopt/q$1.json"
    return ${PIPESTATUS[0]}
}

function gen_aida_plan(){
    src="/home/sable/hanfeng.c/Desktop/hobby/aida/tpch/hyper/plans"
    python genIR.py "$src/q$1.json"
    return ${PIPESTATUS[0]}
}

function gen_tpch_plan_batch(){
    dest=${outputdata}/${gentemp}
    mkdir -p ${dest}
    if [ $1 = "all" ]; then
        ids=`(seq 1 22)`
    else
        # ids=( 1 4 6 12 14 16 19 22 )
        # ids=( 2 3 5 7 8 9 10 11 13 15 17 18 20 21 )
        ids=( 21 )
    fi
    for id in ${ids[@]}
    do
        echo "processing query $id"
        gen_tpch_plan $id > ${dest}/q${id}.hir 2> /dev/null
        if test ${PIPESTATUS[0]} -eq 0 ; then
            echo -e "query $id: ${GN}ok${NO}"
        else
            echo -e "query $id: ${RED}failed${NO}"
        fi
    done
}

function see_diff(){
    for id in {1..22}
    do
        diff ${outputdata}/gen-latest/q$id.hir ${outputdata}/${genfolder}/q$id.hir
    done
}

function see_pass(){
    ## case 1: Incompelte tests
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
        size=$(cat ${outputdata}/${genfolder}/q$id.hir | wc -l)
        echo "q$id $size"
        total=$(($total + $size))
    done
    echo "Current total lines of code: $total"
}

## Usage:
##                   ./run.sh exec 4   # gen
## testfolder=latest ./run.sh exec 4   # gen-latest
if [ -z ${testfolder} ]; then
    testfolder=./output-hir/gen
else
    testfolder=./output-hir/gen-latest
fi


if [ "$#" -eq 1 ]; then
    cmd=$1
    if [ $cmd = "cloc" ]; then
        (set -x && cloc *.py ${inputdata}/*.py ${postfolder}/*.py ${prefolder}/convert_raw_to_opt.py ${prefolder}/convert_raw_to_opt.sh)
    else
        usage
    fi
elif [ "$#" -eq 2 ]; then
    cmd=$1; qid=$2
    if [ $cmd = "tpch" ]; then
        (set -x && gen_tpch_plan $qid)
    elif [ $cmd = "profile" ] && [ $2 = "horseir" ]; then
        grep -REin "program slicing|time" ${outputdata}/${genfolder}/*.hir | python ${postfolder}/profile_horseir.py ${format}
    elif [ $cmd = "see" ] && [ $2 = "diff" ]; then
        see_diff
    elif [ $cmd = "see" ] && [ $2 = "pass" ]; then
        see_pass
    elif [ $cmd = "see" ] && [ $2 = "size" ]; then
        see_size
    elif [ $1 = "exec" ]; then
        export OMP_NUM_THREADS=1
        (set -x && ${exe} -t -f ${testfolder}/q$2.hir --tpch=$2)
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
    elif [ $1 = "profile" ] && [ $2 = "plan" ]; then
        (set -x && cd ${inputdata} && python profile.py $3)
    else 
        usage
    fi
else
    usage
fi

