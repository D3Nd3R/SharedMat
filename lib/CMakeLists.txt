cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(libsharedcvmat VERSION 1.0.0 LANGUAGES CXX)

message(TRACE "========= Start configure: ${PROJECT_NAME}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


set(header_interface
    include/SharedCvMat/SharedReceiver.hpp
    include/SharedCvMat/SharedSender.hpp
)

set(src
    src/SharedReceiver.cpp
    src/SharedSender.cpp
)


add_library(sharedcvmat
    ${header_interface}
    ${src}
)

add_library(SharedCvMat::sharedcvmat ALIAS sharedcvmat)

target_include_directories(sharedcvmat
    PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(sharedcvmat PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
    -Wall -Wextra -Wpedantic>)

target_link_libraries(sharedcvmat PRIVATE
    ${OpenCV_LIBS}
    Threads::Threads
)

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/SharedCvMat)

install(TARGETS sharedcvmat
    EXPORT sharedcvmat-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

set_target_properties(sharedcvmat PROPERTIES EXPORT_NAME SharedCvMat)


message(TRACE "========= Finish configure: ${PROJECT_NAME}")