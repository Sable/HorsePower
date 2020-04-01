#!/bin/bash
export HORSE_BASE=$PWD

# Test-related
export HORSE_HYPER_SQL=${HORSE_BASE}/tests/hyper/online-query
export HORSE_HYPER_HIR=${HORSE_BASE}/tests/hyper/online-hir
export HORSE_TPCH_SQL=${HORSE_BASE}/tests/tpch/query
export HORSE_TPCH_HIR=${HORSE_BASE}/tests/tpch/hir
export HORSE_AIDA_SQL=${HORSE_BASE}/tests/aida/query
export HORSE_AIDA_HIR=${HORSE_BASE}/tests/aida/hir
export HORSE_TEST_PASS=${HORSE_BASE}/tests/unit/pass
export HORSE_TEST_FAIL=${HORSE_BASE}/tests/unit/fail

# Document-related
export HORSE_TPCH_RESULT=${HORSE_BASE}/doc/tpch
export HORSE_MKDOCS=${HORSE_BASE}/doc/mkdocs

# Data-related
export HORSE_TPCH_DATA_RAW=${HORSE_BASE}/data/tpch
export HORSE_TPCH_DATA_BIN=${HORSE_BASE}/data/tpch-bin

# Library-related
export HORSE_LIB_FOLDER=${HORSE_BASE}/libs
export HORSE_LIB_LIB=${HORSE_BASE}/libs/lib

# Source-related
export HORSE_SRC_BASE=${HORSE_BASE}/src/horseir
export HORSE_SRC_CODE=${HORSE_BASE}/src/horseir/v3
export HORSE_SRC_PLAN_HYPER=${HORSE_BASE}/src/plan2ir/hyper
export HORSE_SRC_PLAN_MONETDB=${HORSE_BASE}/src/plan2ir/monetdb
export HORSE_SRC_SQL_CHECKER=${HORSE_BASE}/src/plan2ir/sql-checker

echo "Successfully set up environment variables!"
echo "  >> root path: \${HORSE_BASE}=${HORSE_BASE}"
echo ""
