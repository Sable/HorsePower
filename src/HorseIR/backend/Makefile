H_FILES=h_io.h  test.h  h_memory.h  h_symbol.h  h_system.h  h_primitive.h 
C_FILES=h_io.cc test.cc h_memory.cc h_symbol.cc h_system.cc h_primitive.cc
FLAGS=-g -std=c++11

all: backend

backend: $(C_FILES) $(H_FILES)
	cc $(FLAGS) -c $(C_FILES) -Wno-attributes

.PHONY: clean
clean:
	rm -f *.o

