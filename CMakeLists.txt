cmake_minimum_required(VERSION 3.10)
project(networking-tcp-cpp VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src/server)
add_subdirectory(src/client-lib)
add_subdirectory(src/client-console)

option(BUILD_TESTS "Build test project" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
