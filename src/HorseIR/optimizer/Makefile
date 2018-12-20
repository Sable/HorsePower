# library: -lfl (Linux) or -ll (MacOS)
all: optimizer

h_files = global.h frontend/*.h analysis/*.h translator/*.h backend/*.h
c_files = global.c frontend/*.c analysis/*.c translator/*.c backend/*.c util/*.c
include_files = -I"../../../libs/include/"
include_libs  = ../../../libs/lib/libpcre2-8.a
include_flags = -ll -fopenmp -std=c99 -lm -march=native -DPROFILE -D_POSIX_SOURCE

# Horse with optimizer
optimizer: flex bison $(h_files)
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
#
