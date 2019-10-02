#!/bin/bash
opt=debug
if [ "$#" -eq 0 ]; then
    :  # this is a null
elif [ "$#" -eq 1 ]; then
    opt=$1
else
    echo "Usage: default debug"
    echo "  1) $0 "
    echo "  2) $0 <debug/release>"
    exit 1
fi

sys=`hostname`
cmd_line="make ${opt} sys=${sys}"
echo "make.sh>> ${cmd_line}"
${cmd_line}

