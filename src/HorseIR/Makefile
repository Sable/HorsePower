EXE=horse
CC=gcc-5
CXXFLAGS=-std=c++11 -g
INCLUDE_ANTLR4="../../libs/include/antlr4-runtime"
INCLUDE_PCRE2="../../libs/include"
LIB_GRAMMAR=./grammar
LIB_PCRE2=../../libs/lib
LINK_LIBS=-lm -lstdc++ ${LIB_PCRE2}/libpcre2-8.a
all: ${EXE}

${EXE}: *.cc backend
	${CC} ${CXXFLAGS} -c -I${INCLUDE_ANTLR4} -I${INCLUDE_PCRE2} main.cc
	${CC} *.o backend/*.o ${CXXFLAGS} ${LINK_LIBS} -o $@

.PHONY: grammar tests clean util backend
grammar:
	make -C grammar

tests:
	make -C tests

clean:
	make clean -C backend/
	rm -f *.o ${EXE}

util:
	make -C util/

backend:
	make -C backend/

