cmake_minimum_required(VERSION 3.5)
project(HorseIR)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(ANTLR4CPP_JAR_LOCATION "${PROJECT_SOURCE_DIR}/jar/antlr-4.7-complete.jar")

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(ANTLR4CPP)
if (NOT ANTLR4CPP_FOUND)
  message(FATAL_ERROR "require antlr4-cpp-runtime") 
endif()
include_directories(${ANTLR4CPP_INCLUDE_DIRS})

include(GenerateAntlr4Grammar)
GenerateANTLR4Grammar(horseIR "${CMAKE_CURRENT_SOURCE_DIR}/grammar/HorseIR.g4")
include_directories(${ANTLR4_GEN_DIR})
list(REMOVE_ITEM ANTLR4_horseIR_SOURCE ${ANTLR4_GEN_DIR}/HorseIRListener.cpp)
list(REMOVE_ITEM ANTLR4_horseIR_SOURCE ${ANTLR4_GEN_DIR}/HorseIRVisitor.cpp)
list(REMOVE_ITEM ANTLR4_horseIR_SOURCE ${ANTLR4_GEN_DIR}/HorseIRBaseListener.cpp)
list(REMOVE_ITEM ANTLR4_horseIR_SOURCE ${ANTLR4_GEN_DIR}/HorseIRBaseVisitor.cpp)
foreach(header_file ${ANTLR4_horseIR_HEADER})
  file(COPY ${header_file} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/ast)
endforeach()

file(GLOB_RECURSE AST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/ast/*.cc)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ast/Structure.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/ast)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ast/Operand.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/ast)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ast/Type.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/ast)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ast/ASTNode.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/ast)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ast/ASTVisitor.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/ast)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ast/AST.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/ast)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/misc/Suppiler.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/misc)

add_library(HorseIR ${AST_SOURCES} ${ANTLR4_horseIR_SOURCE})

# == debug == 
add_executable(HorseIRDebug ${CMAKE_CURRENT_SOURCE_DIR}/Debug.cc)
add_dependencies(HorseIRDebug HorseIR)
target_link_libraries(HorseIRDebug ${ANTLR4CPP_LIBRARIES} HorseIR)
