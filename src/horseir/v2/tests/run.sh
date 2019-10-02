#!/bin/bash

usage() {
    printf '%s\n' \
        "Usage: $0 <command> " "" \
        " 1) $0 view        ## view the test information" "" \
        " 2) $0 next <case> ## generate a new test file in <case: pass/fail>" "" 
}

if [ $# -eq 1 ]; then
    cmd=$1
    if [ ${cmd} = "view" ]; then
        echo "folder: pass"
        (set +x && cd pass && ls *.hir | wc -l | xargs python ../gen-docs.py)
        echo "folder: fail"
        (set +x && cd fail && ls *.hir | wc -l | xargs python ../gen-docs.py)
    else
        usage
    fi
elif [ $# -eq 2 ]; then
    cmd=$1
    folder=$2
    if [ ${cmd} = "next" ]; then
        (set +x && cd ${folder} && ls | python ../next-test.py)
    else
        usage
    fi
else
    usage
fi

