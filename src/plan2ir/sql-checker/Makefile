# library: -lfl (Linux) or -ll (MacOS)
all: debug

test: flex bison

file_name = sql
exec_name = checker
h_files = y.tab.h global.h
c_files = lex.yy.c y.tab.c main.c

flex: $(file_name).l
	flex -o lex.yy.c $(file_name).l

bison: $(file_name).y
	bison --yacc --defines $(file_name).y -v

debug: flex bison $(h_files)
	gcc -g $(c_files) -o $(exec_name)

clean:
	rm -f lex.yy.c y.tab.c y.tab.h $(exec_name)

