cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(Sender VERSION 1.0.0 LANGUAGES CXX)

message(TRACE "========= Start configure: ${PROJECT_NAME}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


set(src
    sender.cpp
)

add_executable(Sender
    ${src}
)

target_compile_options(Sender PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
    -Wall -Wextra -Wpedantic>)

target_link_libraries(Sender
    PRIVATE
    ${OpenCV_LIBS}
)

message(TRACE "========= Finish configure: ${PROJECT_NAME}")