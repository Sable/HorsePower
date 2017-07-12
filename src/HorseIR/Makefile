EXE=horse
CXXFLAGS=-std=c++11 -g
ANTLR4_INCLUDE_DIR="/usr/local/include/antlr4-runtime"
LINK_LIBS=-L./grammar -lstdc++ -lantlr4-runtime -lMIRParser
all: ${EXE}

${EXE}: *.cc backend
	cc ${CXXFLAGS} -c -I${ANTLR4_INCLUDE_DIR} main.cc
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

