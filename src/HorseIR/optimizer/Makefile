# library: -lfl (Linux) or -ll (MacOS)
all: optimizer

# Horse with optimizer
optimizer: flex bison *.h analysis/*.h translator/*.h backend/*.h
	gcc -g -o horse lex.yy.c y.tab.c tree.c global.c pretty.c main.c analysis/*.c util/*.c translator/*c backend/*.c -ll -fopenmp -std=c99 -I"../../../libs/include/" ../../../libs/lib/libpcre2-8.a -lm

flex:
	flex -o lex.yy.c opt.l

bison:
	bison --yacc --defines opt.y -v

clean:
	rm lex.yy.c y.tab.c y.tab.h
