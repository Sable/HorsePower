antlr4_url="http://www.antlr.org/download/antlr4-cpp-runtime-4.7-source.zip"
current_dir="$PWD"
build_source_dir="$PWD/antlr4-cpp-runtime/"
static_lib_dir="$PWD/antlr4-cpp-runtime/dist/libantlr4-runtime.a"

if [ ! -f "${static_lib_dir}" ]
then
    rm -rf "${build_source_dir}" antlr4-cpp-runtime-source.zip
    wget -O antlr4-cpp-runtime-source.zip "${antlr4_url}"
    mkdir "${build_source_dir}"
    unzip -d "${build_source_dir}" antlr4-cpp-runtime-source.zip
    rm -f antlr4-cpp-runtime-source.zip
    
    mkdir "${build_source_dir}/build"
    cd "${build_source_dir}/build"
    cmake "${build_source_dir}" -DCMAKE_BUILD_TYPE=Debug
    make antlr4_static
else
    echo "static lib already exist... skip"
fi
