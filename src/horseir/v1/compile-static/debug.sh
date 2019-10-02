#!/bin/bash
if [ "$#" -eq 2 ]; then
    :  # this is a null
else
    echo "Usage: "
    echo "  1) $0 <id> <cur/exp/opt> "
    exit 1
fi

(./make.sh $1 $2 debug) && (cd ../ && gdb --args compile-static/compile-q$1 1 3 0)

