#!/bin/bash
ids=(1 4 6 12 14 16 19 22)
RED='\033[0;31m'
GN='\033[0;32m'
NO='\033[0m'

# build code
make

# Complete tests
for id in {1..22}
#for id in ${ids[@]}
do
    ./horse -c -q $id &> /dev/null
    if test ${PIPESTATUS[0]} -eq 0 ; then
        echo -e "query $id ${GN}pass${NO}"
    else
        echo -e "query $id ${RED}fail${NO}"
    fi
done
