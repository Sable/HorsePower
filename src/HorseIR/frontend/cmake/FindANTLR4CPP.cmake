find_path(ANTLR4CPP_INCLUDE_DIR
  NAMES antlr4-runtime.h
  PATHS ../../../libs
        /usr
  PATH_SUFFIXES include/antlr4-runtime)

find_library(ANTLR4CPP_LIBRARY
  NAMES antlr4-runtime
  PATHS ../../../libs
        /usr
  PATH_SUFFIXES lib)

if (ANTLR4CPP_INCLUDE_DIR AND ANTLR4CPP_LIBRARY)
  message(STATUS "found antlr4-runtime headers at ${ANTLR4CPP_INCLUDE_DIR}")
  message(STATUS "found antlr4-runtime libraries at ${ANTLR4CPP_LIBRARY}")
  set(ANTLR4CPP_INCLUDE_DIRS ${ANTLR4CPP_INCLUDE_DIR})
  set(ANTLR4CPP_LIBRARIES ${ANTLR4CPP_LIBRARY})
  set(ANTLR4CPP_FOUND yes)
else()
  set(ANTLR4CPP_INCLUDE_DIRS)
  set(ANTLR4CPP_LIBRARIES)
  set(ANTLR4CPP_FOUND no)
endif()
