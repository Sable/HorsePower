# LLVM manual
## https://releases.llvm.org/6.0.1/docs/CMake.html#embedding-llvm-in-your-project

cmake_minimum_required(VERSION 3.5)
project(HorseIR VERSION 0.3)

find_package(LLVM 6.0 REQUIRED CONFIG)
message(STATUS "[chf] Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "[chf] Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True) 

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

if (NOT HORSE_CLANG)
    message(FATAL_ERROR "[chf] Clang is not setup properly.")
endif()

find_package(OpenMP REQUIRED)

set(CMAKE_CXX_COMPILER ${HORSE_CLANG})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${HORSE_MODE}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPROFILE")
if (HORSE_OMP_FLAG)
    # sable-tigger
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${HORSE_OMP_FLAG}")
else()
    # sable-intel
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()
if (NOT LLVM_ENABLE_RTTI)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

find_package(BISON REQUIRED)
find_package(FLEX  REQUIRED)

# CMAKE_CURRENT_BINARY_DIR : /path/to/build 
bison_target(HorseBison frontend/h.y ${CMAKE_SOURCE_DIR}/frontend/horse.bison.cc
                        DEFINES_FILE ${CMAKE_SOURCE_DIR}/frontend/horse.bison.hh)
flex_target(HorseFlex   ${CMAKE_SOURCE_DIR}/frontend/h.l
                        ${CMAKE_SOURCE_DIR}/frontend/horse.flex.cc)

ADD_FLEX_BISON_DEPENDENCY(HorseFlex HorseBison)

file(GLOB SRC_FRONTEND   ${CMAKE_SOURCE_DIR}/frontend/*.cc  )
file(GLOB SRC_BACKEND    ${CMAKE_SOURCE_DIR}/backend/*.cc   )
file(GLOB SRC_ANALYSIS   ${CMAKE_SOURCE_DIR}/analysis/*.cc  )
file(GLOB SRC_OPTIMIZER  ${CMAKE_SOURCE_DIR}/optimizer/*.cc )
file(GLOB SRC_TRANSLATOR ${CMAKE_SOURCE_DIR}/translator/*.cc)
file(GLOB SRC_UTIL       ${CMAKE_SOURCE_DIR}/util/*.cc      )

# `llvm-config --cppflags --ldflags --libs core orcjit native`
set(llvm_libs LLVMCore LLVMObject LLVMSupport LLVMInstCombine LLVMScalarOpts
    LLVMExecutionEngine LLVMX86AsmParser LLVMX86CodeGen LLVMX86Desc LLVMX86Info)


#################### Build HorsePower ##################################
set(EXE_NAME horse) 

if (NOT HORSE_MODE)
    set(HORSE_MODE "-g")
endif()

# message(STATUS "[chf] Building HorsePower: ${EXE_NAME}")
# message(STATUS "[chf] Debugging flags: ${HORSE_CLANG} ; ${HORSE_MODE}")

set(SRC_FILES ${SRC_FRONTEND}
              ${SRC_BACKEND}
              ${SRC_ANALYSIS}
              ${SRC_OPTIMIZER}
              ${SRC_TRANSLATOR}
              ${SRC_UTIL}
              global.cc main.cc)
    
add_executable(${EXE_NAME} ${SRC_FILES}
                           ${BISON_HorseBison_OUTPUTS}
                           ${FLEX_HorseFlex_OUTPUTS})

target_include_directories(${EXE_NAME} PUBLIC $ENV{HORSE_LIB_INCLUDE})

target_link_libraries(${EXE_NAME} m)
target_link_libraries(${EXE_NAME} stdc++)
target_link_libraries(${EXE_NAME} $ENV{HORSE_LIB_LIB}/libpcre2-8.a)
target_link_libraries(${EXE_NAME} ${llvm_libs})

# sable-tigger
if (HORSE_OMP_HEADER)
    target_include_directories(${EXE_NAME} PUBLIC ${HORSE_OMP_HEADER})
endif()

message(STATUS "[chf] Executable path: ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}")

