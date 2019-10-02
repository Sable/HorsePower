all: util

ANTLR4_INCLUDE_DIR="/usr/local/include/antlr4-runtime"

util: CSTPrettyPrinter.cc CSTPrettyPrinter.h
	cc -g -std=c++11 -c -I$(ANTLR4_INCLUDE_DIR) CSTPrettyPrinter.cc -Wno-attributes

.PHONY: clean
clean:
	rm -f *.o
