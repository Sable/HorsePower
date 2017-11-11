cmake_minimum_required(VERSION 3.5)
project(HorseIR)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-attributes -g")
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
include_directories(${PCRE2_INCLUDE_DIRS})

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

include(CollectSource)

find_package(YAMLCPP)
if (NOT YAMLCPP_FOUND)
  message(FATAL_ERROR "require yaml-cpp")
endif()
include_directories(${YAMLCPP_INCLUDE_DIRS})

find_package(SpdLog)
if (NOT SPDLOG_FOUND)
    message(FATAL_ERROR "require spdlog")
endif()
include_directories(${SPDLOG_INCLUDE_DIRS})

add_executable(HorseIRInterpreter
  ${HORSE_IR_FRONTEND_SRC}
  ${CMAKE_SOURCE_DIR}/interpreter/Main.cc)
target_link_libraries(HorseIRInterpreter ${ANTLR4CPP_LIBRARIES})
target_link_libraries(HorseIRInterpreter ${CMAKE_SOURCE_DIR}/../backend/libBackend.a)
target_link_libraries(HorseIRInterpreter ${PCRE2_LIBRARIES})
target_link_libraries(HorseIRInterpreter ${YAMLCPP_LIBRARIES})

# == debug ==
add_executable(HorseIRDebug
        ${HORSE_IR_FRONTEND_SRC}
        ${CMAKE_SOURCE_DIR}/Debug.cc)
target_link_libraries(HorseIRDebug ${ANTLR4CPP_LIBRARIES})
target_link_libraries(HorseIRDebug ${CMAKE_SOURCE_DIR}/../backend/libBackend.a)
target_link_libraries(HorseIRDebug ${PCRE2_LIBRARIES})
