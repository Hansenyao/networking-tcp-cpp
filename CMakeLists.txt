cmake_minimum_required(VERSION 3.20)
project(networking-tcp-cpp VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src/server)
add_subdirectory(src/client-lib)
add_subdirectory(src/client-console)
