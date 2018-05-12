# library: -lfl (Linux) or -ll (MacOS)
all: optimizer

optimizer: flex bison *.h analysis/*.h
	gcc -g -o opt lex.yy.c y.tab.c tree.c global.c pretty.c main.c analysis/*.c util/*.c -ll -std=c99

flex:
	flex -o lex.yy.c opt.l

bison:
	bison --yacc --defines opt.y -v

clean:
	rm lex.yy.c y.tab.c y.tab.h
