cmake_minimum_required(VERSION 3.20)
project(QuantC VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD REQUIRED ON)
set(CMAKE_CXX_EXTENSION OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(quantc_bench apps/quantc_bench.cpp)
target_compile_options(quantc_bench PRIVATE -Wall -Wextra -Wpedantic)