#!/bin/bash
echo Starting ...
#export OMP_NUM_THREADS=20;
#export GOMP_CPU_AFFINITY="0-17:4 1-18:4 2-19:4 3-20:4"
#export OMP_NUM_THREADS=40;
#export GOMP_CPU_AFFINITY="0-37:4 1-38:4 2-39:4 3-40:4"
#export OMP_NUM_THREADS=60;
#export GOMP_CPU_AFFINITY="0-57:4 1-58:4 2-59:4 3-60:4"
#export OMP_NUM_THREADS=80;
#export GOMP_CPU_AFFINITY="0-77:4 1-78:4 2-79:4 3-80:4"
#export OMP_PLACES=cores
#export OMP_PROC_BIND=spread
#export OMP_NUM_THREADS=40
(cd ../ && make release && time ./horse -t 4)
echo Done.
