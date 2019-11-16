# library: -lfl (Linux) or -ll (MacOS)
all: debug  #clang-plus-debug

test: flex bison

front    = frontend
analysis = analysis
back     = backend
transt   = translator
util     = util
opt      = optimizer
cc       = gcc-8
cclang   = clang

exec_name = horse

flex_file  = h.l
bison_file = h.y
h_files = $(front)/*.h $(back)/*.h $(analysis)/*.h $(opt)/*.h $(transt)/*.h $(util)/*.h global.h
c_files = $(front)/*.c $(back)/*.c $(analysis)/*.c $(opt)/*.c $(transt)/*.c $(util)/*.c global.c main.c

include_files = -I${HORSE_BASE}/libs/include
include_libs  = ${HORSE_BASE}/libs/lib/libpcre2-8.a
include_flags = -ll -fopenmp -std=c99 -lm -march=native -DPROFILE -D_POSIX_SOURCE
flag_clang    = -ll -fopenmp -std=c99 -lm -march=native -DPROFILE -D_POSIX_SOURCE

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

#flag_clang_plus = -ll -fopenmp -std=c++14 -march=native -DPROFILE -D_POSIX_SOURCE
# TODO: fix compatible issues with C code
#clang-plus-debug: flex bison $(h_files) libcore.a
#	clang++ -g main.cc  -o $(exec_name) \
#		$(include_files) \
#		libcore.a $(include_libs) \
#	    -ll -fopenmp -lm -march=native

# ---------------- library functions ---------------- #

lib_opt   = -O3
lib_flags = -ll -fopenmp -std=c99 -lm -march=native -D_POSIX_SOURCE $(lib_opt)

libbackend.a: obj_backend
	ar rcs $@ *.o $(include_libs)
	@rm -f *.o

libcore.a: obj_backend obj_frontend obj_optimizer obj_analysis obj_util obj_global obj_translator
	ar rcs $@ *.o $(include_libs)
	@rm -f *.o

obj_backend: 
	$(cclang) -c backend/*c $(include_files) $(lib_flags)

obj_frontend:
	$(cclang) -c frontend/*c $(include_files) $(lib_flags) 
 
obj_optimizer:
	$(cclang) -c optimizer/*c $(include_files) $(lib_flags)

obj_analysis:
	$(cclang) -c analysis/*c $(include_files) $(lib_flags)

obj_util:
	$(cclang) -c util/*c $(include_files) $(lib_flags)

obj_global:
	$(cclang) -c global.c $(include_files) $(lib_flags)

obj_translator:
	$(cclang) -c translator/*.c $(include_files) $(lib_flags)

# obj_clean:
# 	rm -rf *.o
