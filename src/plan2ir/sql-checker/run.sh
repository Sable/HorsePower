#!/bin/bash

script_root=./query

if [ "$#" -eq 2 ]; then
    #script_path=/mnt/local/tpch_2_17_0/chf/myqueries
    script_path=${script_root}/$1
    (set -x && ./checker ${script_path}/$2.sql)
else
    echo "$0 <tpch/case> <qid>"
    exit 1
fi

