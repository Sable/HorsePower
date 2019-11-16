# library: -lfl (Linux) or -ll (MacOS)
all: clang-horse

test: flex bison

front = frontend
back  = backend
exe   = horse

flex_file  = h.l
bison_file = h.y

h_files = $(front)/*.h $(front)/horse.bison.hh $(back)/*.h analysis/*.h \
		  optimizer/*.h translator/*.h util/*.h global.h
c_files = $(front)/*.cc $(back)/*.cc analysis/*.cc optimizer/*.cc \
	      translator/*.cc util/*.cc global.cc main.cc

include_files = -I${HORSE_BASE}/libs/include 
include_libs  = ${HORSE_BASE}/libs/lib/libpcre2-8.a 
flag_clang    = -ll -fopenmp -lm -march=native -DPROFILE ${rtti_flag}
llvm_jit_libs = `llvm-config --cppflags --ldflags --libs core orcjit native`
# -std=c++11(default)/14, both pass

flex: $(front)/$(flex_file)
	cd $(front) && flex -o horse.flex.cc $(flex_file)

# --defines generates a header file
bison: $(front)/$(bison_file)
	cd $(front) && bison --yacc --defines $(bison_file) -v -o horse.bison.cc

clang-horse:  flex bison $(h_files)
	$(cc) $(release_flag) $(c_files) -o $(exe) \
		$(include_files) $(include_libs) \
		$(flag_clang) $(llvm_jit_libs)



