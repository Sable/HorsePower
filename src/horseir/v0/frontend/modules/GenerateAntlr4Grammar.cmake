macro(GenerateANTLR4Grammar ANTLR4_PROJECT_NAMESPACE ANTLR4_FILE)
  find_package(Java COMPONENTS Runtime REQUIRED)

  if (NOT Java_FOUND)
    message(FATAL_ERROR "antlr4 requires Java to execuate")
  endif()
  if (NOT EXISTS "${ANTLR4CPP_JAR_LOCATION}")
    message(FATAL_ERROR "expect ANTLR4CPP_JAR_LOCATION to be set, but not found")
  endif()

  set(ANTLR4_GEN_DIR "${CMAKE_BINARY_DIR}/generated/antlr/${ANTLR4_PROJECT_NAMESPACE}")
  message(STATUS "generate ANTLR4 C++ files at ${ANTLR4_GEN_DIR}")
  file(MAKE_DIRECTORY "${ANTLR4_GEN_DIR}")
  execute_process(COMMAND "${Java_JAVA_EXECUTABLE}" -jar "${ANTLR4CPP_JAR_LOCATION}"
    -Werror -Dlanguage=Cpp -listener -visitor
    -o "${ANTLR4_GEN_DIR}"
    -package "${ANTLR4_PROJECT_NAMESPACE}"
    "${ANTLR4_FILE}")

  file(GLOB ANTLR4_${ANTLR4_PROJECT_NAMESPACE}_SOURCE_T ${ANTLR4_GEN_DIR}/*.cpp)
  file(GLOB ANTLR4_${ANTLR4_PROJECT_NAMESPACE}_HEADER_T ${ANTLR4_GEN_DIR}/*.h)
  
  foreach(source_file ${ANTLR4_${ANTLR4_PROJECT_NAMESPACE}_SOURCE_T})
    set_source_files_properties(
      ${source_file}
      PROPERTIES
      COMPILE_FLAGS -Wno-overloaded-virtual)
    list(APPEND ANTLR4_${ANTLR4_PROJECT_NAMESPACE}_SOURCE ${source_file})
  endforeach(source_file)

  foreach(header_file ${ANTLR4_${ANTLR4_PROJECT_NAMESPACE}_HEADER_T})
    list(APPEND ANTLR4_${ANTLR4_PROJECT_NAMESPACE}_HEADER ${header_file})
  endforeach(header_file)
endmacro()
