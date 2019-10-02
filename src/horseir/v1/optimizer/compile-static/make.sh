#!/bin/bash
if [ "$#" -eq 2 ]; then
    opt=release
    :  # this is a null
elif [ "$#" -eq 3 ]; then
    opt=$3
else
    echo "Usage: "
    echo "  1) $0 <id> <cur/exp/opt> "
    echo "  2) $0 <id> <cur/exp/opt> <debug/release/source/profile>"
    echo "  3) $0 <id> <cur/exp/opt> <debug/release> <source> (todo)"
    echo ""
    echo "*Note*: source == release + source code"
    exit 1
fi

qid=$1
src=`echo $2 | tr a-z A-Z`
sys=`hostname`

if [ $opt = "source" ]; then
    cmd_line="make source qid=${qid} src=${src} opt=release sys=${sys}"
else
    cmd_line="make q${qid} src=${src} opt=${opt} sys=${sys}"
fi
echo "run>> ${cmd_line}"
${cmd_line}

