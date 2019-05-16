# library: -lfl (Linux) or -ll (MacOS)
all: debug

test: flex bison

front = front
exec_name = horse

flex_file  = h.l
bison_file = h.y
h_files = $(front)/*.h global.h
c_files = $(front)/*.c global.c main.c

flex: $(front)/$(flex_file)
	cd $(front) && flex -o lex.yy.c $(flex_file)

bison: $(front)/$(bison_file)
	cd $(front) && bison --yacc --defines $(bison_file) -v

debug: flex bison $(h_files)
	gcc -g $(c_files) -o $(exec_name)


clean: $(front)_clean

$(front)_clean:
	rm -f lex.yy.c y.tab.c y.tab.h $(exec_name)

