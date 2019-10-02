find_path(SPDLOG_INCLUDE_DIR
        NAMES spdlog/spdlog.h
        PATHS ../libs /usr
        PATH_SUFFIXES include/)

if (SPDLOG_INCLUDE_DIR)
    message(STATUS "found spdlog at ${SPDLOG_INCLUDE_DIR}")
    set(SPDLOG_INCLUDE_DIRS ${SPDLOG_INCLUDE_DIR})
    set(SPDLOG_LIBRARIES)
    set(SPDLOG_FOUND yes)
else()
    set(SPDLOG_INCLUDE_DIRS)
    set(SPDLOG_LIBRARIES)
    set(SPDLOG_FOUND no)
endif()
