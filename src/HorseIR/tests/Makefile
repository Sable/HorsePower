all: testsuites

ANTLR4_INCLUDE_DIR="/usr/local/include/antlr4-runtime"
LINK_LIBS=-L. -lstdc++ -lantlr4-runtime -lMIRParser
GRAMMAR_INCLUDE_DIR="../grammar"
GRAMMAR_LIB="../grammar"

testsuites: cxxtest.cc
	cxxtestgen --error-printer -o cxxtestRunner.cc cxxtest.cc
	@echo "Building cxxtestRunner.cc ... "
	@cc -g -std=c++11 -c -I${ANTLR4_INCLUDE_DIR} -I${GRAMMAR_INCLUDE_DIR} cxxtestRunner.cc -Wno-attributes
	@echo "Linking ... (flag: ${LINK_LIBS})"
	@cc -g -std=c++11 *.o -L${GRAMMAR_LIB}  ${LINK_LIBS} -o cxxtestRunner

.PHONY: clean
clean:
	rm -f cxxtestRunner.cc cxxtestRunner *.o
