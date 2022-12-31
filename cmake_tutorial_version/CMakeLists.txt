cmake_minimum_required(VERSION 3.8)
include(cmake/utils.cmake)

project(Tutorial)

message(STATUS "PROJECT_SOURCE_DIR " ${PROJECT_SOURCE_DIR})
message(STATUS "PROJECT_BINARY_DIR " ${PROJECT_BINARY_DIR})

set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)
message("VERSION: " ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

string(TIMESTAMP INDENTED_BUILD_TIMESTAMP "%Y-%m-%d %H:%M:%S")
message("INDENTED_BUILD_TIMESTAMP: " ${INDENTED_BUILD_TIMESTAMP})

string(TIMESTAMP COMPACT_BUILD_TIMESTAMP "%Y%m%d%H%M%S")
message("COMPACT_BUILD_TIMESTAMP: " ${COMPACT_BUILD_TIMESTAMP})

set(GIT_BRANCH "")
get_git_branch(GIT_BRANCH)
message("GIT_BRANCH: " ${GIT_BRANCH})

set(GIT_HASH "")
get_git_hash(GIT_HASH)
message("GIT_HASH: " ${GIT_HASH})

configure_file(
    ${PROJECT_SOURCE_DIR}/Config.h.in
    ${PROJECT_SOURCE_DIR}/Config.h
)

include_directories(${PROJECT_SOURCE_DIR})

add_executable(DEMO tutorial.c)
