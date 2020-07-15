#!/bin/bash
if [ -z ${HORSE_BASE} ]; then
    echo "Please run [source ./setup_env.sh] first!"]
    exit 1
fi

echo "Step 1: deploy libraries"
(set -x && cd ${HORSE_BASE}/libs && sh deploy_linux.sh &&
    cd ${HORSE_SRC_CODE} && ./make.sh lib)
echo "[Done] See library under ${HORSE_LIB_FOLDER}"

echo "Step 2: setup tpc-h data (SF1)"
(set -x && cd ${HORSE_BASE}/data/tpch && ./run.sh deploy && ./run.sh gendb 1)
echo "[Done] See data under ${HORSE_BASE}/data/tpch/db1"



