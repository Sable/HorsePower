H_FILES=h_io.h  test.h  h_memory.h  h_symbol.h  h_system.h  h_primitive.h 
C_FILES=h_io.cc test.cc h_memory.cc h_symbol.cc h_system.cc h_primitive.cc
FLAGS=-g -std=c++11 -I"/usr/local/include/antlr4-runtime"

all: backend

backend: $(C_FILES) $(H_FILES)
	cc $(FLAGS) -c h_io.cc -Wno-attributes
	cc $(FLAGS) -c h_memory.cc -Wno-attributes
	cc $(FLAGS) -c h_symbol.cc -Wno-attributes
	cc $(FLAGS) -c h_system.cc -Wno-attributes
	cc $(FLAGS) -c h_primitive.cc -Wno-attributes
	cc $(FLAGS) -c test.cc -Wno-attributes

.PHONY: clean
clean:
	rm -f *.o

