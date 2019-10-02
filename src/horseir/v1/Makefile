# library: -lfl (Linux) or -ll (MacOS)
all: debug

check: flex bison

cc = gcc-8 #gcc-8
h_files = global.h frontend/*.h analysis/*.h translator/*.h backend/*.h
c_files = global.c frontend/*.c analysis/*.c translator/*.c backend/*.c util/*.c
include_files = -I"../../../libs/include/"
include_libs  = ../../../libs/lib/libpcre2-8.a
include_flags = -ll -fopenmp -std=c99 -lm -march=native -DPROFILE -D_POSIX_SOURCE

debug: flex bison $(h_files)
	gcc -g -o horse main.c \
    $(c_files) \
    $(include_files) \
    $(include_libs) \
    $(include_flags)

release: flex bison $(h_files)
	gcc -O3 -o horse main.c \
    $(c_files) \
    $(include_files) \
    $(include_libs) \
    $(include_flags)

flex:
	(cd frontend && flex -o lex.yy.c opt.l)

bison:
	(cd frontend && bison --yacc --defines opt.y -v)

clean:
	(cd frontedn && rm lex.yy.c y.tab.c y.tab.h)

# docs
#
# Enable POSIX (https://stackoverflow.com/a/15749311)
#   to disable a warning: fileno is not defined
#
# Add a flag: -D_POSIX_SOURCE

lib_opt   = -O3
lib_flags = -ll -fopenmp -std=c99 -lm -march=native -D_POSIX_SOURCE $(lib_opt)

libhorse.a: obj_frontend obj_analysis obj_backend obj_util obj_global
	ar rcs $@ *.o 

obj_frontend:
	$(cc) -c frontend/*c $(include_files) $(lib_flags) 

obj_analysis:
	$(cc) -c analysis/*c $(include_files) $(lib_flags)

obj_backend:
	$(cc) -c backend/*c $(include_files) $(lib_flags)

obj_util:
	$(cc) -c util/*c $(include_files) $(lib_flags)

obj_global:
	$(cc) -c global.c $(include_files) $(lib_flags)

obj_clean:
	rm -rf *.o

