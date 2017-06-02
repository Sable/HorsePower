# HorseIR

- [IR design](#ir-design)
- [Installation](#installation)
  + [Linux](#linux)
  + [Mac OS](#mac-os)

## IR design

Grammar

- See [MIR.g4](MIR.g4)

## Installation

### Linux

Install antlr4

    wget http://www.antlr.org/download/antlr-4.7-complete.jar

Create a script `antlr4` under `/usr/local/bin`

    #!/bin/bash
    java -jar "/PATH/TO/antlr-4.7-complete.jar" "$@"

Download C++ Target for linux (runtime-cpp)

    wget http://www.antlr.org/download/antlr4-cpp-runtime-4.7-source.zip

Unzip the zip file, then

    cd runtime-cpp && mkdir run && cmake . && make
    DESTDIR=/PATH/TO/run make install

Update `ANTLR4_INCLUDE_DIR` with the installation path in [Makefile](Makefile)


### Mac OS

Pre-requisites

- antlr4 (v4.7, [download](http://www.antlr.org/download.html))
- homebrew

Install antlr4 runtime library

    brew install antlr4-cpp-runtime

Install testing suits

    brew install cxxtest


