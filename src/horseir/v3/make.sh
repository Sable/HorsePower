#!/bin/bash

usage(){
    printf '%s\n' \
        "Usage: $0 <cmd>   ## support sableintel/tigger" "" \
        "  1) $0 debug     ## clang debugging mode " "" \
        "  2) $0 release   ## clang release mode" "" \
        "Example: $0 debug " ""
    exit 0
}

setup_intel(){
    cc_path=/mnt/local/tools/clang6/bin/clang++
    extra_cmd="-DHORSE_CLANG=${cc_path}"
}

setup_tigger(){
    cc_path=/usr/bin/clang++-6.0
    extra_cmd="-DHORSE_CLANG=${cc_path}"
}

machine=`hostname` 
if [ $machine = "sableintel" ]; then
    setup_intel
elif [ $machine = "tigger" ]; then
    setup_tigger
else
    echo "Unknown machine: $machine"
    exit 99
fi


if [ "$#" -eq 1 ]; then
    mode=$1
    if [ $mode = "debug" ]; then
        (mkdir -p build && cd build && cmake .. ${extra_cmd} -DHORSE_MODE="-g" && make -j16)
    elif [ $mode = "release" ]; then
        (mkdir -p build && cd build && cmake .. ${extra_cmd} -DHORSE_MODE="-O3" && make -j16)
    else
        usage
    fi
else
    usage
fi

