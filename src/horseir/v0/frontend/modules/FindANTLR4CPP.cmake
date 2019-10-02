find_path(ANTLR4CPP_INCLUDE_DIR
  NAMES antlr4-runtime.h
  PATHS ../libs NO_DEFAULT_PATH
  PATH_SUFFIXES include/antlr4-runtime)

find_library(ANTLR4CPP_LIBRARY
  NAMES libantlr4-runtime.a
  PATHS ../libs NO_DEFAULT_PATH
  PATH_SUFFIXES lib)

if (ANTLR4CPP_INCLUDE_DIR)
   message(STATUS "yes 1 ${ANTLR4CPP_INCLUDE_DIR}")
endif()

if (ANTLR4CPP_LIBRARY)
   message(STATUS "yes 2 ${ANTLR4CPP_LIBRARY}")
endif()

if (ANTLR4CPP_INCLUDE_DIR AND ANTLR4CPP_LIBRARY)
  message(STATUS "found antlr4-runtime headers at ${ANTLR4CPP_INCLUDE_DIR}")
  message(STATUS "found antlr4-runtime libraries at ${ANTLR4CPP_LIBRARY}")
  set(ANTLR4CPP_INCLUDE_DIRS ${ANTLR4CPP_INCLUDE_DIR})
  set(ANTLR4CPP_LIBRARIES ${ANTLR4CPP_LIBRARY})
  set(ANTLR4CPP_FOUND yes)
else()
  message(STATUS "entering")
  set(ANTLR4CPP_INCLUDE_DIRS)
  set(ANTLR4CPP_LIBRARIES)
  set(ANTLR4CPP_FOUND no)
endif()
