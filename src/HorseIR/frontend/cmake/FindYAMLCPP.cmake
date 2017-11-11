find_path(YAMLCPP_INCLUDE_DIR
  NAMES yaml-cpp/yaml.h
  PATHS ../../../libs /usr
  PATH_SUFFIXES include/)

find_library(YAMLCPP_LIBRARY
  NAMES yaml-cpp
  PATHS ../../../libs /usr
  PATH_SUFFIXES lib)

if (YAMLCPP_INCLUDE_DIR AND YAMLCPP_LIBRARY)
  message(STATUS "found yaml-cpp headers at ${YAMLCPP_INCLUDE_DIR}")
  message(STATUS "found yaml-cpp libraries at ${YAMLCPP_LIBRARY}")
  set(YAMLCPP_INCLUDE_DIRS ${YAMLCPP_INCLUDE_DIR})
  set(YAMLCPP_LIBRARIES ${YAMLCPP_LIBRARY})
  set(YAMLCPP_FOUND yes)
else()
  set(YAMLCPP_INCLUDE_DIRS)
  set(YAMLCPP_LIBRARIES)
  set(YAMLCPP_FOUND no)
endif()
