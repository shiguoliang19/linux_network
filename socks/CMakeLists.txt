cmake_minimum_required(VERSION 3.8)

# 项目名称
project(proxy)

# 设置编译模式
set(CMAKE_BUILD_TYPE "Debug")

# 所有源文件列为一个集合
set(SRC_DIRS
    socks.h
    socks.c
    main.c
)

# 生成可执行文件 
add_executable(proxy ${SRC_DIRS})

message("CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR})
message("CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR})