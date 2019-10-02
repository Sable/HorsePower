#!/bin/bash
# Incompelte tests
#ids=(2 5 7 8 9 11 15 17 20 21 22)
#ids=(1 6 4 14 12 13 16 17 19 22)
ids=(1 4 6 12 14 16 19 22)
RED='\033[0;31m'
GN='\033[0;32m'
NO='\033[0m'

# Complete tests
for id in {1..22}
#for id in ${ids[@]}
do
    ./run.sh dump $id &> /dev/null
    if test ${PIPESTATUS[0]} -eq 0 ; then
        echo -e "query $id: ${GN}ok${NO}"
    else
        echo -e "query $id: ${RED}failed${NO}"
    fi
done
