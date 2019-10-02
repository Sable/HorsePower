#!/bin/bash

deploy() {
    # include: head files *.h and library files *.a
    mkdir -p libs/lib && mkdir -p libs/include

    # antlr4
    (set -x && cd frontend/antlr4 && \
        unzip antlr4-cpp-runtime-4.7-source.zip && \
        mkdir -p build && cd build && cmake .. && make && \
        cp ../dist/libantlr4-runtime.a ../../../libs/lib/ &&
        cp -r ../runtime/src ../../../libs/include/antlr4-runtime)

    # yaml
    (set -x && cd frontend/yaml && \
        tar -xf yaml-cpp-yaml-cpp-0.5.3.tar.gz && \
        cd yaml-cpp-yaml-cpp-0.5.3 && \
        mkdir -p build && cd build && cmake .. && make && \
        cp libyaml-cpp.a ../../../../libs/lib && \
        cp -r ../include/yaml-cpp ../../../../libs/include/)

    # spdlog
    (set -x && cd frontend/spdlog && \
        tar -xf spdlog-0.14.0.tar.gz && \
        cd spdlog-0.14.0 && mkdir -p build && cd build && cmake .. && make && \
        cp -r ../include/spdlog ../../../../libs/include/)
}

compile() {
    echo "Building front-end"
    (set -x && cd ./frontend && mkdir -p build && cd build && cmake ../ && make lib -C ../../backend && make)
    echo "Building back-end"
    (set -x && cd ./backend  && make && ./horse)
}

# deploy library files
deploy
# compile source code
compile

