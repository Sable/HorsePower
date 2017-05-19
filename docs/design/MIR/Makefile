all: testsuits

ANTLR4_INCLUDE_DIR="/usr/local/include/antlr4-runtime"

PARSER_CC=MIRLexer.cc MIRParser.cc MIRListener.cc MIRBaseListener.cc MIRVisitor.cc MIRBaseVisitor.cc
PARSER_O =MIRLexer.o MIRParser.o MIRListener.o MIRBaseListener.o MIRVisitor.o MIRBaseVisitor.o
.PHONY: antlr4_runtime_lib
parser: MIR.g4
	antlr4 -Dlanguage=Cpp MIR.g4 -visitor -listener
	ls *.cpp | awk -F. '{system("mv " $$0 " " $$1 ".cc")}'
	@echo "Building MIRLexer.cc ..."
	@cc -g -std=c++11 -c -I$(ANTLR4_INCLUDE_DIR) MIRLexer.cc -Wno-attributes
	@echo "Building MIRParser.cc ... "
	@cc -g -std=c++11 -c -I$(ANTLR4_INCLUDE_DIR) MIRParser.cc -Wno-attributes
	@echo "Building MIRListener.cc ... "
	@cc -g -std=c++11 -c -I$(ANTLR4_INCLUDE_DIR) MIRListener.cc -Wno-attributes
	@echo "Building MIRBaseListener.cc ... "
	@cc -g -std=c++11 -c -I$(ANTLR4_INCLUDE_DIR) MIRBaseListener.cc -Wno-attributes
	@echo "Building MIRVisitor.cc ... "
	@cc -g -std=c++11 -c -I$(ANTLR4_INCLUDE_DIR) MIRVisitor.cc -Wno-attributes
	@echo "Building MIRBaseVisitor.cc ... "
	@cc -g -std=c++11 -c -I$(ANTLR4_INCLUDE_DIR) MIRBaseVisitor.cc -Wno-attributes
	@echo "Generating libMIRParser.a ... "
	@ar rvs libMIRParser.a $(PARSER_O)
	@rm -f $(PARSER_O)
	@rm -f $(PARSER_CC)
	@rm -f MIR.tokens MIRLexer.tokens

LINK_LIBS=-L. -lc++ -lantlr4-runtime -lMIRParser

testsuits: cxxtest.cc
	cxxtestgen --error-printer -o cxxtestRunner.cc cxxtest.cc
	@echo "Building cxxtestRunner.cc ... "
	@cc -g -std=c++11 -c -I$(ANTLR4_INCLUDE_DIR) cxxtestRunner.cc -Wno-attributes
	@echo "Building prettyPrinter.cc ... "
	@cc -g -std=c++11 -c -I$(ANTLR4_INCLUDE_DIR) prettyPrinter.cc -Wno-attributes
	@echo "Linking ... (flag: ${LINK_LIBS})"
	@cc -g -std=c++11 *.o  ${LINK_LIBS} -o cxxtestRunner

.PHONY: clean
clean:
	rm -f MIR.tokens MIRLexer.tokens
	rm -f MIRLexer.h MIRParser.h MIRBaseListener.h MIRListener.h 
	rm -f MIRBaseVisitor.h MIRVisitor.h
	rm -f cxxtestRunner.cc cxxtestRunner
	rm -f libMIRParser.a *.o
