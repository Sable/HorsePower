cmake_minimum_required(VERSION 3.7)
project(HorseIR)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(ANTLR4CPP_JAR_LOCATION "${PROJECT_SOURCE_DIR}/jar/antlr-4.7-complete.jar")

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(ExternalAntlr4Cpp)
include_directories(${ANTLR4CPP_INCLUDE_DIRS})
link_directories(${ANTLR4CPP_LIBS})

antlr4cpp_process_grammar(horseIR horseIR
  ${CMAKE_CURRENT_SOURCE_DIR}/grammar/HorseIR.g4
  ${CMAKE_CURRENT_SOURCE_DIR}/grammar/HorseIR.g4)
include_directories(${antlr4cpp_include_dirs_horseIR})
list(REMOVE_ITEM antlr4cpp_src_files_horseIR
  ${ANTLR4CPP_GENERATED_SRC_DIR}/horseIR/HorseIRVisitor.cpp
  ${ANTLR4CPP_GENERATED_SRC_DIR}/horseIR/HorseIRBaseVisitor.cpp
  ${ANTLR4CPP_GENERATED_SRC_DIR}/horseIR/HorseIRListener.cpp
  ${ANTLR4CPP_GENERATED_SRC_DIR}/horseIR/HorseIRBaseListener.cpp)
foreach (generated_header ${antlr4cpp_src_headers_horseIR})
  file(COPY ${generated_header} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/ast)
endforeach(generated_header)

file(GLOB_RECURSE AST_SOURCES ./ast/*.cc)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ast/Structure.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/ast)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ast/Operand.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/ast)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ast/Type.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/ast)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ast/ASTNode.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/ast)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ast/ASTVisitor.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/ast)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ast/AST.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/ast)

add_library(HorseIR ${AST_SOURCES} ${antlr4cpp_src_files_horseIR})
add_dependencies(HorseIR antlr4cpp)
