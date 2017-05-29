all: testsuits

ANTLR4_INCLUDE_DIR="/usr/local/include/antlr4-runtime"

PARSER_CC=HorseIRLexer.cc HorseIRParser.cc HorseIRListener.cc HorseIRBaseListener.cc HorseIRVisitor.cc HorseIRBaseVisitor.cc
PARSER_O =HorseIRLexer.o HorseIRParser.o HorseIRListener.o HorseIRBaseListener.o HorseIRVisitor.o HorseIRBaseVisitor.o
.PHONY: antlr4_runtime_lib
parser: HorseIR.g4
	antlr4 -Dlanguage=Cpp HorseIR.g4 -visitor -listener
	ls *.cpp | awk -F. '{system("mv " $$0 " " $$1 ".cc")}'
	@echo "Building HorseIRLexer.cc ..."
	@cc -g -std=c++11 -c -I$(ANTLR4_INCLUDE_DIR) HorseIRLexer.cc -Wno-attributes
	@echo "Building HorseIRParser.cc ... "
	@cc -g -std=c++11 -c -I$(ANTLR4_INCLUDE_DIR) HorseIRParser.cc -Wno-attributes
	@echo "Building HorseIRListener.cc ... "
	@cc -g -std=c++11 -c -I$(ANTLR4_INCLUDE_DIR) HorseIRListener.cc -Wno-attributes
	@echo "Building HorseIRBaseListener.cc ... "
	@cc -g -std=c++11 -c -I$(ANTLR4_INCLUDE_DIR) HorseIRBaseListener.cc -Wno-attributes
	@echo "Building HorseIRVisitor.cc ... "
	@cc -g -std=c++11 -c -I$(ANTLR4_INCLUDE_DIR) HorseIRVisitor.cc -Wno-attributes
	@echo "Building HorseIRBaseVisitor.cc ... "
	@cc -g -std=c++11 -c -I$(ANTLR4_INCLUDE_DIR) HorseIRBaseVisitor.cc -Wno-attributes
	@echo "Generating libMIRParser.a ... "
	@ar rvs libMIRParser.a $(PARSER_O)
	@rm -f $(PARSER_O)
	@rm -f $(PARSER_CC)
	@rm -f HorseIR.tokens HorseIRLexer.tokens

LINK_LIBS=-L. -lstdc++ -lantlr4-runtime -lMIRParser

testsuits: cxxtest.cc
	cxxtestgen --error-printer -o cxxtestRunner.cc cxxtest.cc
	@echo "Building cxxtestRunner.cc ... "
	@cc -g -std=c++11 -c -I$(ANTLR4_INCLUDE_DIR) cxxtestRunner.cc -Wno-attributes
	@echo "Linking ... (flag: ${LINK_LIBS})"
	@cc -g -std=c++11 *.o  ${LINK_LIBS} -o cxxtestRunner

.PHONY: clean
clean:
	rm -f HorseIR.tokens HorseIRLexer.tokens
	rm -f HorseIRLexer.h HorseIRParser.h HorseIRBaseListener.h HorseIRListener.h 
	rm -f HorseIRBaseVisitor.h HorseIRVisitor.h
	rm -f cxxtestRunner.cc cxxtestRunner
	rm -f libMIRParser.a *.o
