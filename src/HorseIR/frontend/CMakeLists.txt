cmake_minimum_required(VERSION 3.5)
project(HorseIR)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfatal-errors -Werror -g")
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

set(ANTLR4CPP_JAR_LOCATION "${PROJECT_SOURCE_DIR}/jar/antlr-4.7-complete.jar")

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
find_package(ANTLR4CPP)
if (NOT ANTLR4CPP_FOUND)
    message(FATAL_ERROR "require antlr4-cpp-runtime")
endif ()
include_directories(${ANTLR4CPP_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/../backend/)

include(FindPCRE2)
if (NOT PCRE2_FOUND)
    message(FATAL_ERROR "require pcre2")
endif ()

include(CollectSource)

# == debug ==
add_executable(HorseIRDebug
        ${HORSE_IR_FRONTEND_SRC}
        ${CMAKE_SOURCE_DIR}/Debug.cc)
target_link_libraries(HorseIRDebug ${ANTLR4CPP_LIBRARIES})
target_link_libraries(HorseIRDebug ${CMAKE_SOURCE_DIR}/../backend/libBackend.a)
target_link_libraries(HorseIRDebug ${PCRE2_LIBRARIES})
