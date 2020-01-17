cmake_minimum_required(VERSION 3.10)


set(EXECUTABLE_OUTPUT_PATH bin CACHE PATH "Build directory" FORCE)
set(LIBRARY_OUTPUT_PATH bin CACHE PATH "Build directory" FORCE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_BUILD_FILES_DIRECTORY bin)
set(CMAKE_BUILD_DIRECTORY bin)
set(CMAKE_BINARY_DIR  bin)
SET(EXECUTABLE_OUTPUT_PATH bin)
SET(CMAKE_CACHEFILE_DIR bin)

add_executable(cun_server "entry/main.c")