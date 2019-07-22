#!/bin/bash
DIR=../../../../src/HorseIR/plan2ir/monetdb

for id in {1..22}
do
    (cd ${DIR} && ./run.sh dot $id) > q$id.dot
    dot -Tpdf q$id.dot -o q$id.pdf
done

echo done.

