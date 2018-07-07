#!/usr/bin/env bash

# build compiler, 'horse' is generated
(cd ../src/HorseIR/optimizer && make) 

# compile compiled C code from HorseIR
(cd ../src/HorseIR/optimizer/compile && ./run.sh)


