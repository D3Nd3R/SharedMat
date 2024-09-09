cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(Receiver VERSION 1.0.0 LANGUAGES CXX)

message(TRACE "========= Start configure: ${PROJECT_NAME}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


set(src
    receiver.cpp
)

add_executable(Receiver
    ${src}
)

target_compile_options(Receiver PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
    -Wall -Wextra -Wpedantic>)

target_link_libraries(Receiver
    PRIVATE
    ${OpenCV_LIBS}
    SharedCvMat::sharedcvmat
)

message(TRACE "========= Finish configure: ${PROJECT_NAME}")