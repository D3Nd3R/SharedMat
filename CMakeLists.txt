cmake_minimum_required(VERSION 3.16)
project(
    SharedCvMatProject
    VERSION 1.0
    LANGUAGES CXX)

option(ENABLE_TEST "enable testing via gtest" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_EXAMPLES "enable build example applications" OFF)

if (UNIX)
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

#list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake/Modules)

add_subdirectory(lib)
if (ENABLE_EXAMPLES)
    add_subdirectory(example)
endif ()




