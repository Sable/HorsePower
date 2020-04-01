#!/bin/bash

usage(){
    printf '%s\n\n' \
        "Usage:" \
        " 1) $0 deploy       ## unzip file and deploy it" \
        " 2) $0 gendb <SF>   ## a scale factor 1/2/4/8"
    exit 1
}

deploy(){
    echo "Deploying the TPC-H tool"
    (unzip -qq tpc-h-tool.zip && rm -r __MACOSX && cd tpch_2_17_0/dbgen && cp makefile.suite Makefile)
    echo "Note: please edit the tpch_2_17_0/dbgen/Makefile by updating the following lines (starting from line 103)"
    echo "      CC      = gcc"
    echo "      DATABASE=ORACLE"
    echo "      MACHINE =LINUX"
    echo "      WORKLOAD=TPCH"
    echo ""
    echo "Then, you can run ./run.sh gendb <scale_factor>, for example, ./run.sh gendb 1"
}

if [ $# -eq 1 ]; then
    if [ $1 = "deploy" ]; then
        deploy
    else
        usage
    fi
elif [ $# -eq 2  ]; then
    if [ $1 = "gendb" ]; then
        sf=$2
        dest=db${sf}
        (set -x && mkdir -p $dest  && cd tpch_2_17_0/dbgen && make && ./dbgen -s $sf && mv *.tbl ../../$dest)
    else
        usage
    fi
else
    usage
fi




