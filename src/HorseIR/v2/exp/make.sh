#!/bin/bash
if [ "$#" -eq 2 ]; then
    fmt=library
    opt=release
    :  # this is a null
elif [ "$#" -eq 3 ]; then
    fmt=$3
    opt=release
elif [ "$#" -eq 4 ]; then
    fmt=$3
    opt=$4
else
    echo "Usage: (default:library + release)"
    echo "  1) $0 <id> <raw/opt> "
    echo "  2) $0 <id> <raw/opt> <source/library>"
    echo "  3) $0 <id> <raw/opt> <source/library> <debug/release/profile>"
    echo ""
    exit 1
fi

qid=$1
src=`echo $2 | tr a-z A-Z`
sys=`hostname`
src_path=$2/q${qid}.h

(set -x && cp ${src_path} gencode.h)

if [ $fmt = "source" ]; then
    cmd_line="make source qid=${qid} src=${src} opt=${opt} sys=${sys}"
else
    cmd_line="make q${qid} src=${src} opt=${opt} sys=${sys}"
fi
echo "run>> ${cmd_line}"
${cmd_line}

