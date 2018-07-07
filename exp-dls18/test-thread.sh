#!/bin/bash
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <scale> <thread>"
    exit 1
fi

scale=$1
nthread=$2

run_thread()
{
    (cd ../src/HorseIR/optimizer && ./compile/testAll $1 $2 $3 $4)
}

echo Starting ...
export OMP_PLACES=cores
export OMP_PROC_BIND=spread
export OMP_NUM_THREADS=$nthread

echo Compiling ...
(cd ../compile && ./run.sh) &> /dev/null

echo Running with $nthread threads
query=(1 4 6 12 14 16 19 22)

echo ">> HorseIR Optimization Level: 0 (No Optimization)"
for id in "${query[@]}"
do
    run_thread $scale 0 15 $id
done

echo ">> HorseIR Optimization Level: 1 (Fusing Elementwise Functions)"
for id in "${query[@]}"
do
    run_thread $scale 1 15 $id
done

echo ">> HorseIR Optimization Level: 2 (Fusing with Patterns)"
for id in "${query[@]}"
do
    run_thread $scale 2 15 $id
done

echo ">> HorseIR Optimization Level: 3 (All Optimizations On)"
for id in "${query[@]}"
do
    run_thread $scale 3 15 $id
done


