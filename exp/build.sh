#!/usr/bin/env bash

# build front-end
(cd ../src/HorseIR/frontend && mkdir -p build && cd build && cmake ../ && make)

# build back-end
(cd ../src/HorseIR && make && ./horse)

