include(BuildHorseIRGrammar)

file(GLOB DIR_AST_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/ast/*.h)
foreach (header ${DIR_AST_HEADERS})
    file(COPY ${header} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/ast)
endforeach (header)

file(GLOB DIR_AST_TYPE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/ast/type/*.h)
foreach (header ${DIR_AST_TYPE_HEADERS})
    file(COPY ${header} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/ast/type)
endforeach (header)

file(GLOB DIR_AST_OPERAND_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/ast/operand/*.h)
foreach (header ${DIR_AST_OPERAND_HEADERS})
    file(COPY ${header} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/ast/operand)
endforeach (header)

file(GLOB DIR_AST_STATEMENT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/ast/statement/*.h)
foreach (header ${DIR_AST_STATEMENT_HEADERS})
    file(COPY ${header} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/ast/statement)
endforeach (header)

list(APPEND HORSE_IR_AST_SRC ${DIR_AST_HEADERS})
list(APPEND HORSE_IR_AST_SRC ${DIR_AST_TYPE_HEADERS})
list(APPEND HORSE_IR_AST_SRC ${DIR_AST_OPERAND_HEADERS})
list(APPEND HORSE_IR_AST_SRC ${DIR_AST_STATEMENT_HEADERS})
list(APPEND HORSE_IR_AST_SRC ${ANTLR4_horseIR_SOURCE})
list(APPEND HORSE_IR_AST_SRC ${ANTLR4_horseIR_HEADERS})
list(APPEND HORSE_IR_AST_SRC ${CMAKE_SOURCE_DIR}/ast/ASTPrinter.cc)
list(APPEND HORSE_IR_AST_SRC ${CMAKE_SOURCE_DIR}/ast/CSTConverter.cc)
list(APPEND HORSE_IR_AST_SRC ${CMAKE_SOURCE_DIR}/ast/type/TypeUtils.cc)

file(GLOB DIR_MISC_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/misc/*.h)
foreach (header ${DIR_MISC_HEADERS})
    file(COPY ${header} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/misc)
endforeach (header)

list(APPEND HORSE_IR_MISC_SRC ${DIR_MISC_HEADERS})

file(GLOB DIR_INTERPRETER_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/interpreter/*.h)
foreach (header ${DIR_INTERPRETER_HEADERS})
    file(COPY ${header} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/interpreter)
endforeach (header)

list(APPEND HORSE_IR_INTERPRETER_SRC ${DIR_INTERPRETER_HEADERS})
list(APPEND HORSE_IR_INTERPRETER_SRC ${CMAKE_SOURCE_DIR}/interpreter/StatementFlow.cc)
list(APPEND HORSE_IR_INTERPRETER_SRC ${CMAKE_SOURCE_DIR}/interpreter/LiteralConverter.cc)
list(APPEND HORSE_IR_INTERPRETER_SRC ${CMAKE_SOURCE_DIR}/interpreter/Interpreter.cc)
list(APPEND HORSE_IR_INTERPRETER_SRC ${CMAKE_SOURCE_DIR}/interpreter/Dispatcher.cc)


file(GLOB DIR_ANALYSIS_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/analysis/*.h)
foreach (header ${DIR_ANALYSIS_HEADERS})
    file(COPY ${header} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/analysis)
endforeach (header)

list(APPEND HORSE_IR_ANALYSIS_SRC ${DIR_ANALYSIS_HEADERS})

list(APPEND HORSE_IR_FRONTEND_SRC ${HORSE_IR_AST_SRC})
list(APPEND HORSE_IR_FRONTEND_SRC ${HORSE_IR_MISC_SRC})
list(APPEND HORSE_IR_FRONTEND_SRC ${HORSE_IR_INTERPRETER_SRC})
list(APPEND HORSE_IR_FRONTEND_SRC ${HORSE_IR_ANALYSIS_SRC})