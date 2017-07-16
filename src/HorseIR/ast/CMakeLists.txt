cmake_minimum_required(VERSION 3.7)
project(AST)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

set(SOURCE_FILES
        ./Operand.h
        ./Type.h
        ./ASTNode.cc
        ./Type/DictionaryType.cc
        ./Type/EnumerationType.cc
        ./Type/FunctionType.cc
        ./Type/ListType.cc
        ./Type/ScalarType.cc
        ./Type/Type.cc
        ./Type/WildcardType.cc
        ./ASTVisitor.cc
        ./Operand/Literal.cc
        ./Operand/NilLiteral.cc
        ./Operand/ComplexLiteral.cc
        ./Operand/BoolLiteral.cc
        ./Operand/CharLiteral.cc
        ./Operand/IntegerLiteral.cc
        ./Operand/Integer8Literal.cc
        ./Operand/Integer16Literal.cc
        ./Operand/Integer32Literal.cc
        ./Operand/Integer64Literal.cc
        ./Operand/SymbolLiteral.cc
        ./Operand/FunctionLiteral.cc
        ./Operand/TableLiteral.cc
        ./Structure.h
        ./Structure/CompilationUnit.cc
        ./Structure/Module.cc
        ./Structure/Method.cc
        ./Structure/LabelStatement.cc
        ./Structure/BranchStatement.cc
        ./Structure/PhiStatement.cc
        ./Structure/AssignStatement.cc
        ./Structure/ReturnStatement.cc
        ./Structure/Statement.cc
        ./Operand/Identifier.cc
        ./Operand/Operand.cc
        ./AST.h
        ./Structure/StatementIterator.cc)

include_directories(
        /usr/local/include/antlr4-runtime
)

add_library(AST ${SOURCE_FILES})
