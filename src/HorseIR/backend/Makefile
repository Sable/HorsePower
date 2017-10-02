CC=gcc-7
H_FILES=h_io.h  test.h  h_memory.h  h_symbol.h  h_system.h  h_primitive.h  h_libs.h
C_FILES=h_io.cc test.cc h_memory.cc h_symbol.cc h_system.cc h_primitive.cc h_libs.cc
D_FLAGS=-std=c++14 -DUSE_MMAP -fopenmp -I"../../../libs/include/" -g
R_FLAGS=-std=c++14 -DUSE_MMAP -fopenmp -I"../../../libs/include/" -O3

all: backend

backend: $(C_FILES) $(H_FILES) tpch/*.h
	$(CC) $(D_FLAGS) -c h_io.cc -Wno-attributes
	$(CC) $(D_FLAGS) -c h_memory.cc -Wno-attributes
	$(CC) $(D_FLAGS) -c h_libs.cc -Wno-attributes
	$(CC) $(D_FLAGS) -c h_symbol.cc -Wno-attributes
	$(CC) $(D_FLAGS) -c h_system.cc -Wno-attributes
	$(CC) $(D_FLAGS) -c h_primitive.cc -Wno-attributes
	$(CC) $(D_FLAGS) -c test.cc -Wno-attributes

release: $(C_FILES) $(H_FILES) tpch/*.h
	$(CC) $(R_FLAGS) -c h_io.cc -Wno-attributes
	$(CC) $(R_FLAGS) -c h_memory.cc -Wno-attributes
	$(CC) $(R_FLAGS) -c h_libs.cc -Wno-attributes
	$(CC) $(R_FLAGS) -c h_symbol.cc -Wno-attributes
	$(CC) $(R_FLAGS) -c h_system.cc -Wno-attributes
	$(CC) $(R_FLAGS) -c h_primitive.cc -Wno-attributes
	$(CC) $(R_FLAGS) -c test.cc -Wno-attributes

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

