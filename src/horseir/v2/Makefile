# library: -lfl (Linux) or -ll (MacOS)
all: debug

test: flex bison

front    = frontend
analysis = analysis
back     = backend
transt   = translator
util     = util
opt      = optimizer
cc       = gcc-8

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
	$(cc) -g $(c_files) -o $(exec_name) \
		$(include_files) \
		$(include_libs)  \
		$(include_flags)

release: flex bison $(h_files)
	$(cc) -O3 $(c_files) -o $(exec_name) \
		$(include_files) \
		$(include_libs)  \
		$(include_flags)

# c_less_files = $(analysis)/*.c $(opt)/*.c $(transt)/*.c $(util)/*.c main.c
# 
# release: 
# 	$(cc) -O3 $(c_less_files) -o $(exec_name) \
# 		$(include_files) \
# 		libcore.a \
# 		$(include_flags)
# 
# clean: $(front)_clean
# 	rm -f $(exec_name)
# 
# $(front)_clean:
# 	(cd $(front) && rm -f lex.yy.c y.tab.c y.tab.h)
# 
# lib_opt   = -O3
# lib_flags = -ll -fopenmp -std=c99 -lm -march=native -D_POSIX_SOURCE $(lib_opt)
# 
# libcore.a: obj_frontend obj_backend obj_global
# 	ar rcs $@ *.o $(include_libs)
# 
# obj_frontend:
# 	$(cc) -c frontend/*c $(include_files) $(lib_flags) 
# 
# obj_analysis:
# 	$(cc) -c analysis/*c $(include_files) $(lib_flags)
# 
# obj_backend:
# 	$(cc) -c backend/*c $(include_files) $(lib_flags)
# 
# obj_util:
# 	$(cc) -c util/*c $(include_files) $(lib_flags)
# 
# obj_global:
# 	$(cc) -c global.c $(include_files) $(lib_flags)
# 
# obj_clean:
# 	rm -rf *.o
