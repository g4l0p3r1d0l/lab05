cmake_minimum_required(VERSION 3.10)
project(lab05_homework VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
add_compile_options(-Wno-error=maybe-uninitialized)

add_subdirectory(banking)

option(BUILD_TESTS "Build tests" ON)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(third-party/gtest)
    add_subdirectory(tests)
endif()
