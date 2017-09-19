EXE=horse
CC=gcc-7
D_FLAGS=-std=c++14 -fopenmp -g
R_FLAGS=-std=c++14 -fopenmp -O3
INCLUDE_ANTLR4="../../libs/include/antlr4-runtime"
INCLUDE_PCRE2="../../libs/include"
LIB_GRAMMAR=./grammar
LIB_PCRE2=../../libs/lib
LINK_LIBS=-lm -lstdc++ ${LIB_PCRE2}/libpcre2-8.a
all: debug

debug: *.cc backend_debug
	${CC} ${D_FLAGS} -c -I${INCLUDE_ANTLR4} -I${INCLUDE_PCRE2} main.cc
	${CC} *.o backend/*.o ${D_FLAGS} ${LINK_LIBS} -o ${EXE}

release: *.cc backend_release
	${CC} ${R_FLAGS} -c -I${INCLUDE_ANTLR4} -I${INCLUDE_PCRE2} main.cc
	${CC} *.o backend/*.o ${R_FLAGS} ${LINK_LIBS} -o ${EXE}

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

backend_debug:
	make -C backend/

backend_release:
	make release -C backend/

