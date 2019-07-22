#!/bin/bash
DIR=../../../../src/HorseIR/plan2ir/monetdb

for id in {1..22}
do
    if [ $id -ne 21 ]; then
        (cd ${DIR} && kind=aida ./run.sh dot $id) > q$id.dot
        dot -Tpdf q$id.dot -o q$id.pdf
    fi
done

echo done with q21 not available and q14 fails.

