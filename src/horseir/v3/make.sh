#!/bin/bash

usage(){
    printf '%s\n' \
        "Usage: $0 <cmd>  # support sableintel/tigger" "" \
        "1) $0 debug      # clang debugging mode " "" \
        "2) $0 release    # clang release mode " "" \
        "Example: $0 debug " ""
    exit 0
}

setup_intel(){
    cc_path=/mnt/local/tools/clang6/bin/clang++
    no_rtti="-fno-rtti"
}

setup_tigger(){
    cc_path=/usr/bin/clang++-6.0
    no_rtti=""
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
        make cc=${cc_path} release_flag="-g" rtti_flag=${no_rtti}
    elif [ $mode = "release" ]; then
        make cc=${cc_path} release_flag="-O3" rtti_flag=${no_rtti}
    else
        usage
    fi
else
    usage
fi

