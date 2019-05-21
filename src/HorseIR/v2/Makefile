# library: -lfl (Linux) or -ll (MacOS)
all: debug

test: flex bison

front    = frontend
analysis = analysis
back     = backend
trans    = translator
exec_name = horse

flex_file  = h.l
bison_file = h.y
h_files = $(front)/*.h $(back)/*.h $(analysis)/*.h global.h
c_files = $(front)/*.c $(back)/*.c $(analysis)/*.c global.c main.c

flex: $(front)/$(flex_file)
	cd $(front) && flex -o lex.yy.c $(flex_file)

bison: $(front)/$(bison_file)
	cd $(front) && bison --yacc --defines $(bison_file) -v

debug: flex bison $(h_files)
	gcc -g $(c_files) -o $(exec_name)


clean: $(front)_clean
	rm -f $(exec_name)

$(front)_clean:
	(cd $(front) && rm -f lex.yy.c y.tab.c y.tab.h)

