all: testsuits

LINK_LIBS=-lstdc++ -lantlr4-runtime

.PHONY: antlr4_runtime_lib
antlr4_runtime_lib:
	@sh ./build_antlr4_runtime.sh

parser: MIR.g4
	antlr4 -Dlanguage=Cpp MIR.g4 -visitor -listener
	ls *.cpp | awk -F. '{system("mv " $$0 " " $$1 ".cc")}'
	@echo "Building MIRLexer.cc ..."
	@$(CC) -std=c++11 -c -I./antlr4-cpp-runtime/runtime/src/ MIRLexer.cc -Wno-attributes
	@echo "Building MIRParser.cc ... "
	@$(CC) -std=c++11 -c -I./antlr4-cpp-runtime/runtime/src/ MIRParser.cc -Wno-attributes
	@echo "Building MIRListener.cc ... "
	@$(CC) -std=c++11 -c -I./antlr4-cpp-runtime/runtime/src/ MIRListener.cc -Wno-attributes
	@echo "Building MIRBaseListener.cc ... "
	@$(CC) -std=c++11 -c -I./antlr4-cpp-runtime/runtime/src/ MIRBaseListener.cc -Wno-attributes
	@echo "Building MIRVisitor.cc ... "
	@$(CC) -std=c++11 -c -I./antlr4-cpp-runtime/runtime/src/ MIRVisitor.cc -Wno-attributes
	@echo "Building MIRBaseVisitor.cc ... "
	@$(CC) -std=c++11 -c -I./antlr4-cpp-runtime/runtime/src/ MIRBaseVisitor.cc -Wno-attributes

testsuits: antlr4_runtime_lib parser cxxtest.cc
	cxxtestgen --error-printer -o cxxtestRunner.cc cxxtest.cc
	@echo "Building cxxtestRunner.cc ... "
	@$(CC) -std=c++11 -c -I./antlr4-cpp-runtime/runtime/src/ cxxtestRunner.cc -Wno-attributes
	@echo "Linking ... (flag: ${LINK_LIBS})"
	@$(CC) -std=c++11 *.o  -L./antlr4-cpp-runtime/dist/ ${LINK_LIBS} -o cxxtestRunner

.PHONY: clean
clean:
	rm -f MIRLexer.cc MIRLexer.h MIRParser.cc MIRParser.h
	rm -f MIR.tokens MIRLexer.tokens
	rm -f MIRBaseListener.cc MIRBaseListener.h MIRListener.cc MIRListener.h
	rm -f MIRBaseVisitor.cc MIRBaseVisitor.h MIRVisitor.cc MIRVisitor.h
	rm -f cxxtestRunner.cc cxxtestRunner
	rm -f *.o
