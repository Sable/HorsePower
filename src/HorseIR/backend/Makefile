CC=gcc-5
H_FILES=h_io.h  test.h  h_memory.h  h_symbol.h  h_system.h  h_primitive.h  h_libs.h
C_FILES=h_io.cc test.cc h_memory.cc h_symbol.cc h_system.cc h_primitive.cc h_libs.cc
FLAGS=-std=c++14 -g -fopenmp -I"../../../libs/include/"

all: backend

backend: $(C_FILES) $(H_FILES) tpch/*.h
	$(CC) $(FLAGS) -c h_io.cc -Wno-attributes
	$(CC) $(FLAGS) -c h_memory.cc -Wno-attributes
	$(CC) $(FLAGS) -c h_libs.cc -Wno-attributes
	$(CC) $(FLAGS) -c h_symbol.cc -Wno-attributes
	$(CC) $(FLAGS) -c h_system.cc -Wno-attributes
	$(CC) $(FLAGS) -c h_primitive.cc -Wno-attributes
	$(CC) $(FLAGS) -c test.cc -Wno-attributes

lib: $(C_FILES) $(H_FILES)
	$(CC) $(FLAGS) -c h_io.cc -Wno-attributes
	$(CC) $(FLAGS) -c h_memory.cc -Wno-attributes
	$(CC) $(FLAGS) -c h_libs.cc -Wno-attributes
	$(CC) $(FLAGS) -c h_symbol.cc -Wno-attributes
	$(CC) $(FLAGS) -c h_system.cc -Wno-attributes
	$(CC) $(FLAGS) -c h_primitive.cc -Wno-attributes
	ar rcs libBackend.a *.o

.PHONY: clean
clean:
	rm -f *.o

