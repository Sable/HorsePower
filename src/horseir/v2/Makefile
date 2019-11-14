# library: -lfl (Linux) or -ll (MacOS)
all: clang-plus-debug

test: flex bison

front    = frontend
analysis = analysis
back     = backend
transt   = translator
util     = util
opt      = optimizer
cc       = gcc-8

exec_name = horse

flex_file  = h.l
bison_file = h.y
h_files = $(front)/*.h $(back)/*.h $(analysis)/*.h $(opt)/*.h $(transt)/*.h $(util)/*.h global.h
c_files = $(front)/*.c $(back)/*.c $(analysis)/*.c $(opt)/*.c $(transt)/*.c $(util)/*.c global.c main.c
include_files = -I"../../../libs/include/"
include_libs  = ../../../libs/lib/libpcre2-8.a
include_flags = -ll -fopenmp -std=c99 -lm -march=native -DPROFILE -D_POSIX_SOURCE
flag_clang    = -ll -fopenmp -std=c99 -lm -march=native -DPROFILE -D_POSIX_SOURCE
flag_clang_plus = -ll -fopenmp -std=c++11 -march=native -DPROFILE -D_POSIX_SOURCE

flex: $(front)/$(flex_file)
	cd $(front) && flex -o lex.yy.c $(flex_file)

bison: $(front)/$(bison_file)
	cd $(front) && bison --yacc --defines $(bison_file) -v

# ~ 9s (sable-intel)
debug: flex bison $(h_files)
	$(cc) -g $(c_files) -o $(exec_name) \
		$(include_files) \
		$(include_libs)  \
		$(include_flags) 

# ~ 29s (sable-intel)
release: flex bison $(h_files)
	$(cc) -O3 $(c_files) -o $(exec_name) \
		$(include_files) \
		$(include_libs)  \
		$(include_flags)

clang-debug:  flex bison $(h_files)
	clang-6 -g $(c_files) -o $(exec_name) \
		$(include_files) \
		$(include_libs)  \
		$(flag_clang)

clang-release:  flex bison $(h_files)
	clang-6 -O3 $(c_files) -o $(exec_name) \
		$(include_files) \
		$(include_libs)  \
		$(flag_clang)

# TODO: fix compatible issues with C code
clang-plus-debug: flex bison $(h_files)
	clang++-6 -g $(c_files) -o $(exec_name) \
		$(include_files) \
		$(include_libs)  \
		$(flag_clang_plus)

# ---------------- library functions ---------------- #

lib_opt   = -O3
lib_flags = -ll -fopenmp -std=c99 -lm -march=native -D_POSIX_SOURCE $(lib_opt)

libbackend.a: obj_backend
	ar rcs $@ *.o $(include_libs)
	@rm -f *.o

obj_backend: 
	$(cc) -c backend/*c $(include_files) $(lib_flags)

obj_frontend:
	$(cc) -c frontend/*c $(include_files) $(lib_flags) 
 
# obj_analysis:
# 	$(cc) -c analysis/*c $(include_files) $(lib_flags)
# 
# obj_util:
# 	$(cc) -c util/*c $(include_files) $(lib_flags)
# 
# obj_global:
# 	$(cc) -c global.c $(include_files) $(lib_flags)
# 
# obj_clean:
# 	rm -rf *.o
