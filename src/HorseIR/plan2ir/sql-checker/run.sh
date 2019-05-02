#!/bin/bash

if [ "$#" -eq 1 ]; then
    script_path=/mnt/local/tpch_2_17_0/chf/myqueries
    (set -x && ./checker ${script_path}/$1.sql)
else
    echo "$0 <qid>"
    exit 1
fi

