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

