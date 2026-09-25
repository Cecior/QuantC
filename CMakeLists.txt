cmake_minimum_required(VERSION 3.20)
project(QuantC VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(OpenMP REQUIRED)

add_library(quantc_common INTERFACE)
target_include_directories(quantc_common INTERFACE ${PROJECT_SOURCE_DIR}/include)
target_compile_options(quantc_common INTERFACE -Wall -Wextra -Wpedantic)

add_library(quantc_naive STATIC src/backends/naive.cpp)
target_link_libraries(quantc_naive PUBLIC quantc_common)
target_compile_options(quantc_naive PRIVATE -fno-tree-vectorize)

add_library(quantc_omp STATIC src/backends/omp.cpp)
target_link_libraries(quantc_omp PUBLIC quantc_common OpenMP::OpenMP_CXX)

add_library(quantc_avx STATIC src/backends/avx.cpp)
target_link_libraries(quantc_avx PUBLIC quantc_common OpenMP::OpenMP_CXX)
target_compile_options(quantc_avx PRIVATE -mavx2)

add_executable(quantc_bench apps/quantc_bench.cpp)
target_link_libraries(quantc_bench PRIVATE quantc_naive quantc_omp quantc_avx)