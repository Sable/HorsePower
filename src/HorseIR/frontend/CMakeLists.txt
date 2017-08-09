cmake_minimum_required(VERSION 3.8)
project(HorseIR)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g")

set(ANTLR4CPP_JAR_LOCATION "${PROJECT_SOURCE_DIR}/jar/antlr-4.7-complete.jar")

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
find_package(ANTLR4CPP)
if (NOT ANTLR4CPP_FOUND)
    message(FATAL_ERROR "require antlr4-cpp-runtime")
endif()
include_directories(${ANTLR4CPP_INCLUDE_DIRS})

include(BuildHorseIRGrammar)

file(GLOB_RECURSE DIR_AST_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/ast/*.h)
file(GLOB DIR_MISC_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/misc/*.h)
file(GLOB DIR_INTERPRETER_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/interpreter/*.h)

foreach(header ${DIR_AST_HEADERS})
    file(COPY ${header} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/ast)
endforeach(header)
foreach(header ${DIR_MISC_HEADERS})
    file(COPY ${header} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/misc)
endforeach(header)
foreach(header ${DIR_INTERPRETER_HEADERS})
    file(COPY ${header} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/interpreter)
endforeach(header)

# == debug ==
add_executable(HorseIRDebug ${CMAKE_SOURCE_DIR}/Debug.cc ${DIR_AST_HEADERS})
target_link_libraries(HorseIRDebug ${ANTLR4CPP_LIBRARIES})