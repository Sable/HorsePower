#!/bin/bash

## config
mkdir -p include
mkdir -p lib

## Download C++ target for Linux
wget http://www.antlr.org/download/antlr4-cpp-runtime-4.7-source.zip

## Install antlr4
unzip antlr4-cpp-runtime-4.7-source.zip -d antlr4-cpp
mkdir -p antlr4
(cd antlr4-cpp && mkdir build && cd build && cmake ../ && make && DESTDIR=../../antlr4 make install)

mv antlr4/usr/local/lib/* lib/
mv antlr4/usr/local/include/* include

## Download pcre2
mkdir -p pcre2
(cd pcre2 && wget https://ftp.pcre.org/pub/pcre/pcre2-10.30.zip && unzip pcre2-10.30.zip)

## Install pcre2
(cd pcre2/pcre2-10.30 && ./configure --disable-shared --enable-jit && make)

mv pcre2/pcre2-10.30/.libs/* lib/
mv pcre2/pcre2-10.30/src/*.h include/ 

## Download yaml-cpp
wget https://github.com/jbeder/yaml-cpp/archive/yaml-cpp-0.5.3.tar.gz
tar -xf yaml-cpp-0.5.3.tar.gz
mv yaml-cpp-yaml-cpp-0.5.3 yaml-cpp
(cd yaml-cpp && mkdir build && cd build && cmake ../ && make)
mv yaml-cpp/build/libyaml-cpp.a lib/
mv yaml-cpp/include/yaml-cpp include/

## Download spdlog
wget https://github.com/gabime/spdlog/archive/v0.14.0.tar.gz
tar -xf v0.14.0.tar.gz
mv spdlog-0.14.0/include/spdlog include/


