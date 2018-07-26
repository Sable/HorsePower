# library: -lfl (Linux) or -ll (MacOS)
all: optimizer

h_files = global.h frontend/*.h analysis/*.h translator/*.h backend/*.h
c_files = global.c frontend/*.c analysis/*.c translator/*.c backend/*.c util/*.c

# Horse with optimizer
optimizer: flex bison $(h_files)
	gcc -g -o horse main.c $(c_files) \
	-I"../../../libs/include/" ../../../libs/lib/libpcre2-8.a \
	-ll -fopenmp -std=c99 -lm -march=native -DPROFILE

flex:
	(cd frontend && flex -o lex.yy.c opt.l)

bison:
	(cd frontend && bison --yacc --defines opt.y -v)

clean:
	(cd frontedn && rm lex.yy.c y.tab.c y.tab.h)
