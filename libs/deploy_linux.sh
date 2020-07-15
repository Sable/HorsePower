#!/bin/bash

root=`hostname`

## config
lib_inc="${root}/include"
lib_lib="${root}/lib"
lib_src="${root}/src"

mkdir -p ${lib_inc} ${lib_lib} ${lib_src}

## Download pcre2
pcre2_file="pcre2-10.32"
(set -x && cd ${lib_src} && wget https://ftp.pcre.org/pub/pcre/${pcre2_file}.tar.gz && tar -xf ${pcre2_file}.tar.gz)

## Install pcre2
pcre2_src=${lib_src}/${pcre2_file}
(cd ${pcre2_src} && CC=gcc ./configure --disable-shared --enable-jit && make)

mv ${pcre2_src}/.libs/* ${lib_lib}/
mv ${pcre2_src}/src/*.h ${lib_inc}/

## Install linasm
linasm_ins=$PWD/${root}/linasm
(cd ${lib_src} && wget https://iweb.dl.sourceforge.net/project/linasm/linasm-1.13%28stable%29.tar.gz && tar -xf linasm-1.13\(stable\).tar.gz)
(cd ${lib_src}/linasm-1.13\(stable\) && make install prefix=${linasm_ins})

mv ${linasm_ins}/lib/*       ${lib_lib}/
mv ${linasm_ins}/include/*.h ${lib_inc}/
rm -rf ${linasm_ins}
