all: testsuits

LINK_LIBS=-lstdc++ -lantlr4-runtime

.PHONY: antlr4_runtime_lib
antlr4_runtime_lib: antlr4-runtime-url.inc
	rm -rf antlr4-cpp-runtime
	wget -O antlr4-cpp-runtime-source.zip -i - < antlr4-runtime-url.inc
	mkdir antlr4-cpp-runtime
	unzip -d./antlr4-cpp-runtime/ antlr4-cpp-runtime-source.zip
	cd antlr4-cpp-runtime && mkdir build 
	cd ./antlr4-cpp-runtime/build && cmake .. -DCMAKE_BUILD_TYPE=Debug
	cd ./antlr4-cpp-runtime/build && make antlr4_static
	rm -f antlr4-cpp-runtime-source.zip

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

testsuits: parser cxxtest.cc
	cxxtestgen --error-printer -o cxxtestRunner.cc cxxtest.cc
	@echo "Building cxxtestRunner.cc ... "
	@$(CC) -std=c++11 -c cxxtestRunner.cc -Wno-attributes
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
