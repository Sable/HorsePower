#!/bin/bash

usage(){
    printf '%s\n\n' \
        "Usage: $0 <cmd>" \
        "  1) $0 <kind> <qid|name>         ## kind: tpch/aida/udf for tpch id or file name " \
        "  2) $0 <kind> <cmd> <qid|name>   ## cmd: dump/show/dot for tpch id or file name" \
        "  3) $0 ls <kind>                 ## list queries available in a specific kind" \
        "  4) $0 batch <kind>              ## run query in batch mode and save to output/" \
        "  5) $0 cloc                      ## display the Python code information" \
        "  6) $0 plan <query>              ## \"plan + query\" for generating UDF plans" \
        "  7) $0 see <all|partial>         ## check if all or partial TPC-H queries pass" \
        "Examples:" \
        "  *) $0 aida show 6               ## show query 6 from AIDA" \
        "  *) $0 udf q6_proc               ## compile q6_proc.json into HorseIR code" \
        " tpch folder: ${tpch_folder}" \
        " aida folder: ${aida_folder}" \
        " udf  folder: ${udf_folder} "
    exit 1
}

tpch_folder="input/plan/tpch"
aida_folder="/mnt/local/script-tpch/aida/plans/monetdb"
udf_folder="input/plan/udf"

partial_set=( 1 6 )

error(){
    echo "Error: $1" > /dev/stderr
    exit 1
}

getFilePath(){
    kind=$1; name=$2
    if [ $kind = "tpch" ]; then
        file_path="${tpch_folder}/q${name}.plan"
    elif [ $kind = "aida" ]; then
        file_path="${aida_folder}/${name}.plan"
    elif [ $kind = "udf" ]; then
        file_path="${udf_folder}/${name}.plan"
    else
        usage
    fi
    echo $file_path
}

listFile(){
    kind=$1
    if [ $kind = "tpch" ]; then
        dest_folder=${tpch_folder}
    elif [ $kind = "aida" ]; then
        dest_folder=${aida_folder}
    elif [ $kind = "udf" ]; then
        dest_folder=${udf_folder}
    else
        usage
    fi
    (set -x && ls -lh ${dest_folder})
}

runBatch(){
    kind=$1
    if [ $kind = "tpch" ]; then
        for id in {1..22}
        do
            compilePlan `getFilePath $kind $id`
        done
    elif [ $kind = "aida" ]; then
        error "Add batch support for aida"
    elif [ $kind = "udf" ]; then
        error "Add batch support for udf"
    else
        usage
    fi
}


RED='\033[0;31m'
GN='\033[0;32m'
NO='\033[0m'

# Usage: result=`checkNumber $1` (@result = true)
function checkNumber(){
    num=$1
    if [ -n $num ] && [ "$num" -eq "$num" ] 2>/dev/null; then
        echo true
    else
        echo false
    fi
}

function seeCloc(){
    files="json2ir.py jsonPrinter.py m2ir.py parsing.py text2json.py util.py"
    (set -x && cloc ${files})
}

function genPlanUDF(){
    echo "-- Fetch execution plans for UDF: ${udf_folder}/$1.sql"
    echo "PLAN `cat ${udf_folder}/$1.sql`"
}

function seePass(){
    if [ $1 = "all" ]; then
        ids=`(seq 1 22)`
    else
        ids=${partial_set}
    fi
    for id in ${ids[@]}
    do
        python m2ir.py dump ${tpch_folder}/q$id.plan &> /dev/null
        if test ${PIPESTATUS[0]} -eq 0 ; then
            echo -e "query $id: ${GN}ok${NO}"
        else
            echo -e "query $id: ${RED}failed${NO}"
        fi
    done
}

compilePlan(){
    (set -x && python m2ir.py $1)
}

runCmdPlan(){
    (set -x && python m2ir.py $1 $2)
}

if [ "$#" -eq 1 ]; then
    if [ $1 = "cloc" ]; then
        seeCloc
    else 
        usage
    fi
elif [ "$#" -eq 2 ]; then
    cmd=$1
    if [ $cmd = "tpch" -o $cmd = "udf" -o $cmd = "aida" ]; then
        compilePlan `getFilePath $cmd $cc`
    elif [ ${cmd} = "plan" ]; then
        genPlanUDF $2
    elif [ ${cmd} = "see" ]; then
        seePass $2
    elif [ ${cmd} = "ls" ]; then
        listFile $2
    elif [ ${cmd} = "batch" ]; then
        runBatch $2
    else
        usage
    fi
elif [ "$#" -eq 3 ]; then
    kind=$1; cmd=$2
    if [ $kind = "tpch" -o $kind = "udf" -o $kind = "aida" ]; then
        if [ $cmd = "dump" -o $cmd = "show" -o $cmd = "dot" ]; then
            runCmdPlan $cmd `getFilePath $kind $3`
        else
            usage
        fi
    else
        usage
    fi
else
    usage
fi

