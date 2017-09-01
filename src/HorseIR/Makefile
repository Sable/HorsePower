EXE=horse
CXXFLAGS=-std=c++11 -g
INCLUDE_ANTLR4="../../libs/include/antlr4-runtime"
INCLUDE_PCRE2="../../libs/include"
LIB_GRAMMAR=./grammar
LIB_ANTLR4=../../libs/lib
LIB_PCRE2=../../libs/lib
LINK_LIBS=-lm -lstdc++ -L${LIB_ANTLR4} -lantlr4-runtime -Wl,-rpath,${LIB_PCRE2} -lpcre2-8
all: ${EXE}

${EXE}: *.cc backend
	cc ${CXXFLAGS} -c -I${INCLUDE_ANTLR4} -I${INCLUDE_PCRE2} main.cc
	cc *.o backend/*.o ${CXXFLAGS} ${LINK_LIBS} -o $@

.PHONY: grammar tests clean util backend
grammar:
	make -C grammar

tests:
	make -C tests

clean:
	rm -f *.o ${EXE}

util:
	make -C util/

backend:
	make -C backend/

