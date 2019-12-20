#!/bin/bash

usage(){
    printf '%s\n\n' \
        "Usage: $0 <cmd>   ## support sableintel/tigger" \
        "  1) $0 debug     ## clang debugging mode " \
        "  2) $0 release   ## clang release mode" \
        "  3) $0 clean     ## remove the temp folder: build/" \
        "  4) $0 lib       ## create a static library" \
        "Example:" \
        "  *) $0 debug"
    exit 0
}

setup_intel(){
    cc_path=/mnt/local/tools/clang6/bin/clang++
    extra_cmd="-DHORSE_CLANG=${cc_path}"
}

setup_tigger(){
    cc_path=/usr/bin/clang++-6.0
    cc_omp_header=/usr/lib/gcc/x86_64-linux-gnu/8/include
    cc_omp_flag="-fopenmp=libiomp5"
    extra_cmd="-DHORSE_CLANG=${cc_path} -DHORSE_OMP_HEADER=${cc_omp_header} -DHORSE_OMP_FLAG=${cc_omp_flag}"
    echo "extra_cmd = ${extra_cmd}"
}

prepare_lib(){
    mkdir -p build-lib && cd build-lib
    src_files="../backend/*.cc ../util/*.cc ../global.cc ../frontend/pretty.cc"
    src_include_dirs="${HORSE_BASE}/libs/include/"
    src_include_libs="${HORSE_BASE}/libs/lib/libpcre2-8.a"
    cc_flags="-fopenmp -lm -lstdc++ -march=native"
    lib_name=libcore.a
    (set -x && g++-8 ${cc_flags} -c ${src_files} -I${src_include_dirs} && ar rcs ${lib_name} *.o ${src_include_libs} && mv ${lib_name} ..)
}


machine=`hostname` 
if [ $machine = "sableintel" ]; then
    setup_intel
elif [ $machine = "tigger" ]; then
    setup_tigger
else
    echo "Unknown machine: $machine"
    exit 99
fi


if [ "$#" -eq 1 ]; then
    mode=$1
    if [ $mode = "debug" ]; then
        (mkdir -p build && cd build && cmake .. ${extra_cmd} -DHORSE_MODE="-g" && make -j16)
    elif [ $mode = "release" ]; then
        (mkdir -p build && cd build && cmake .. ${extra_cmd} -DHORSE_MODE="-O3" && make -j16)
    elif [ $mode = "clean" ]; then
        (set -x && rm -rf build)
    elif [ $mode = "lib" ]; then
        # (mkdir -p build-lib && cd build-lib && cmake -D BUILD_STATIC_LIBS=ON .. ${extra_cmd} -DHORSE_MODE="-O3" && make -j16 && mv libcore.a ..)
        prepare_lib
    else
        usage
    fi
else
    usage
fi

