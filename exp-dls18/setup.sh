#!/usr/bin/env bash

# build compiler, 'horse' is generated
(cd ../src/HorseIR/optimizer && make) 

# view all queries compiled 
(cd ../src/HorseIR/optimizer && ./see_compile.sh) 

# compile generated C code from HorseIR
(cd ../src/HorseIR/optimizer/compile && ./run.sh)


