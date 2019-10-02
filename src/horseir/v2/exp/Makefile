# Note:
# libhorse.a must be placed before libpcre2-8.a

gcc_lynx       = gcc
gcc_tigger     = gcc-8
gcc_sableintel = gcc-8

targ = compile
cccc = ${gcc_${sys}} #clang-6 / gcc-8
cccl = clang-6
include_files = main.c 
include_dirs  = -I"../../../../libs/include/"
include_libs  =./libhorse.a ../../../../libs/lib/libpcre2-8.a
include_flags = -DQALL -std=c99 -fopenmp -lm -lstdc++ -march=native 
include_flags_c = $(include_flags) -DSELECT_C # -v -ftime-report

ifeq ($(opt),debug)
	opt_level = -g
else ifeq ($(opt),profile)
	opt_level = -g -O3
else
	opt_level = -O3
endif

all: q19

## GCC debugging option: https://stackoverflow.com/a/21861251
## adding the option `-v -ftime-report`

## @echo hello $@, shows the name of target
## `echo $@ | tr a-z A-Z` returns the name of the target with uppercase

## NO_PROFILE_INFO: don't show line profiling information
##       HORSE_EXP: load queries under exp/
##       HORSE_CUR: load queries under cur/

q%:
	$(cccc) -o "$(targ)-$@" \
    $(opt_level) \
    $(include_files) \
    $(include_dirs) \
    $(include_libs)  \
    $(include_flags_c) -DINCLUDE_`echo $@ | tr a-z A-Z` -DHORSE_${src}

clang_omp_dirs = -I"/mnt/sableintel/tools/clang/lib/clang/6.0.0/include/"
clang_omp_libs = -L"/mnt/sableintel/tools/clang/lib"
clang_opt_level= -O2

clang-q%:
	$(cccl) -o "$(targ)-$@" \
    $(clang_opt_level) \
    $(include_files) \
    $(include_dirs) \
    $(include_libs)  \
    $(include_flags_c) -DINCLUDE_Q6 -DHORSE_EXP \
	$(clang_omp_dirs) \
	$(clang_omp_libs) -fcolor-diagnostics

clean:
	rm -rf $(targ)-q* $(targ)-clang-q*

src_files = main.c ../backend/*.c ../util/*.c ../global.c ../frontend/pretty.c
src_include_dirs = -I"../../../../libs/include/"
src_include_libs = ../../../../libs/lib/libpcre2-8.a

source:
	$(cccc) -o "compile-q${qid}" \
    $(opt_level) \
	$(src_files) \
    $(src_include_dirs) \
    $(src_include_libs) \
    $(include_flags_c) -DINCLUDE_Q${qid} -DHORSE_${src}


