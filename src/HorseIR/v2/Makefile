# library: -lfl (Linux) or -ll (MacOS)
all: debug

test: flex bison

front    = frontend
analysis = analysis
back     = backend
transt   = translator
util     = util
opt      = optimizer

exec_name = horse

flex_file  = h.l
bison_file = h.y
h_files = $(front)/*.h $(back)/*.h $(analysis)/*.h $(opt)/*.h $(transt)/*.h $(util)/*.h global.h
c_files = $(front)/*.c $(back)/*.c $(analysis)/*.c $(opt)/*.c $(transt)/*.c $(util)/*.c global.c main.c
include_files = -I"../../../libs/include/"
include_libs  = ../../../libs/lib/libpcre2-8.a
include_flags = -ll -fopenmp -std=c99 -lm -march=native -DPROFILE -D_POSIX_SOURCE

flex: $(front)/$(flex_file)
	cd $(front) && flex -o lex.yy.c $(flex_file)

bison: $(front)/$(bison_file)
	cd $(front) && bison --yacc --defines $(bison_file) -v

debug: flex bison $(h_files)
	gcc -g $(c_files) -o $(exec_name) \
		$(include_files) \
		$(include_libs)  \
		$(include_flags)

release: flex bison $(h_files)
	gcc -g -O3 $(c_files) -o $(exec_name) \
		$(include_files) \
		$(include_libs)  \
		$(include_flags)

clean: $(front)_clean
	rm -f $(exec_name)

$(front)_clean:
	(cd $(front) && rm -f lex.yy.c y.tab.c y.tab.h)

