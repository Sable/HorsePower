#!/usr/bin/env bash

# run HorseIR generator
(cd ../src/HorseIR/plan2ir/hyper && ./see_pass.sh)

# copy generated HorseIR programs (*.hir)
(cd ../src/HorseIR/optimizer/data && ./copy_from_genir.sh) 


