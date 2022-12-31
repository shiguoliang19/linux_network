cmake_minimum_required(VERSION 3.8)

project(Tutorial)

message(STATUS "PROJECT_SOURCE_DIR " ${PROJECT_SOURCE_DIR})
message(STATUS "PROJECT_BINARY_DIR " ${PROJECT_BINARY_DIR})

set(CMAKEDEFINE_VAR1 1)
set(CMAKEDEFINE_VAR2 0)

configure_file(
    "${PROJECT_SOURCE_DIR}/Config.h.in"
    "${PROJECT_SOURCE_DIR}/Config.h"
)

include_directories("${PROJECT_SOURCE_DIR}")

add_executable(DEMO tutorial.c)
