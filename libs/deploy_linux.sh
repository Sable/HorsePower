#!/bin/bash

## config
mkdir -p include
mkdir -p lib
mkdir -p pcre2

## Download pcre2
pcre2_file="pcre2-10.32"
mkdir -p ${pcre2_file}
(cd pcre2 && wget https://ftp.pcre.org/pub/pcre/${pcre2_file}.tar.gz && tar -xf ${pcre2_file}.tar.gz)

## Install pcre2
(cd pcre2/${pcre2_file} && CC=gcc ./configure --disable-shared --enable-jit && make)

mv pcre2/${pcre2_file}/.libs/* lib/
mv pcre2/${pcre2_file}/src/*.h include/ 

## Install linasm
linasm_path=$PWD/linasm
mkdir -p ${linasm_path}
(cd ${linasm_path} && wget https://iweb.dl.sourceforge.net/project/linasm/linasm-1.13%28stable%29.tar.gz && tar -xf linasm-1.13\(stable\).tar.gz)
(cd ${linasm_path}/linasm-1.13\(stable\) && make install prefix=${linasm_path})

